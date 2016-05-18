/*Copyright 2002-2014 e-foto team (UERJ)
  This file is part of e-foto.

    e-foto is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    e-foto is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with e-foto.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#endif
#ifdef unix
#include <math.h>
#endif

#include "DemFeatures.h"

#include "MatchingPoints.h"

//#include <math.h>

#include "ogrsf_frmts.h"
#include <gdal_priv.h>
#include <ogr_spatialref.h>

#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

/*
 * Dem features
 **/

DemFeatures::DemFeatures()
{
    // No feature and point selected
    selected_feat = -1;
    selected_pt = -1;
    img_left_width = img_left_height = img_right_width = img_right_height = -1;
}


//
// Common functions
//

FeatureClass * DemFeatures::getFeatureClass(int classid)
{
    if (classid < 1 || unsigned(classid) > feature_classes.size())
        return NULL;

    return &feature_classes.at(classid-1);
}

/*
 * Feature type:
 * 1- Point
 * 2- Line
 * 3- Polygon
 */
std::string DemFeatures::getFeatureTypeName(int ftype)
{
    switch (ftype)
    {
    case 1: return "Point"; break;
    case 2: return "Line"; break;
    case 3: return "Polygon"; break;
    default: return "Unkown";
    }
}

int DemFeatures::loadFeatures(char *filename, bool append=false)
{
    return loadFeatSp165(filename, append);
}

int DemFeatures::saveFeatures(char *filename)
{
    return saveFeatSp165(filename);
}

// Export features
int DemFeatures::exportFeatures(char *filename, int mode)
{
    // 0- Txt file
    // 1- Shape file

    std::string basename;
    std::string name = std::string(filename);

    size_t pos = name.find_last_of(".");
    if (pos == -1)
    {
        basename = name;
    }
    else
    {
        std::string ext = name.substr(pos+1);
        basename = name.substr(0,pos);

        if (ext == std::string("shp"))
            mode = 1;
        else if (ext ==  std::string("txt"))
            mode = 0;
    }

    if (mode == 1)
        return exportShpFeatures((char*)basename.c_str());
    return exportTxtFeatures((char*)basename.c_str());
}

// Export features to txt file
int DemFeatures::exportTxtFeatures(char *filename)
{
    std::string name = std::string(filename);
    name.append(".txt");

    // Open file to save
    std::ofstream arq(name);
    if (arq.fail())
    {
        printf("Problems while saving ...\n");
        return 0;
    }

    arq << "E-FOTO Features File\n";
    arq << "=====================\n\n";
    arq << "Total features: " << features.size() << "\n\n";

    // Change number precision
    arq.precision(20);

    DemFeature df;

    for (unsigned int i=0; i<features.size(); i++)
    {
        df = features.at(i);
        arq << "Feature # " << i+1 << "\n";
        arq << " Type: " << getFeatureTypeName(df.feature_type).c_str() << "\n";
        arq << " Class: " << getFeatureClass(df.feature_class)->name.c_str() << "\n";
        arq << " Name: " << df.name.c_str() << "\n";
        arq << " Description: " << df.description.c_str() << "\n";
        arq << " Number of points: " << df.points.size() << "\n";
        arq << " Centroid coordinates: " << df.centroid.X << ", " << df.centroid.Y << "," << df.centroid.Z << "\n";
        arq << " Perimeter: " << df.perimeter << "\n";
        arq << " Area: " << df.area << "\n";
        arq << " Points: number, X, Y, Z\n";

        for (unsigned int k=0; k<df.points.size(); k++)
            arq << "  " << k+1 << ", " << df.points.at(k).X << ", " << df.points.at(k).Y << ", " << df.points.at(k).Z << "\n";

        arq << "\n";
    }

    arq.close();

    return 1;
}

int DemFeatures::exportShpFeatures(char *filename)
{
    // Retorna se não houverem feições para salvar
    if (features.size() == 0)
        return 1;

    std::deque<SHPObject*> shpObjs;
    std::deque<std::string> objNames;
    int nShapeType;
    DemFeature df;

    // Para cada tipo de feição
    for (int k = 0; k < feature_classes.size(); k++)
    {
        bool flag = false;

        // Definindo o tipo do shape
        FeatureClass fc = feature_classes.at(k);
        switch (fc.type)
        {
        case 1: nShapeType = SHPT_POINTZ; break;
        case 2: nShapeType = SHPT_ARCZ; break;
        case 3: nShapeType = SHPT_POLYGONZ; break;
        default: nShapeType = SHPT_MULTIPOINTZ;
        }

        // Procura uma classe de feiçoes entre todas as feições
        for (unsigned int i=0; i<features.size(); i++)
        {
            if (getFeatureClassId(i+1) == k+1)
            {
                // Se for encontrada uma feição da classe procurada habilita a gravação
                flag = true;

                // Aloca espaço para a criação de um novo objeto do shape
                //SHPObject* obj = (SHPObject*) malloc(sizeof(SHPObject));

                // Armazenando vétices da feição
                df = features.at(i);
                unsigned int nVertices = df.points.size();
                if (nShapeType == SHPT_POLYGONZ) nVertices++;
                double* padfX = (double *) malloc(sizeof(double) * nVertices);
                double* padfY = (double *) malloc(sizeof(double) * nVertices);
                double* padfZ = (double *) malloc(sizeof(double) * nVertices);
                for (unsigned int j=0; j<nVertices; j++)
                {
                    DemFeaturePoints dfp;
                    if (j != df.points.size())
                        dfp = df.points.at(j);
                    else
                        dfp = df.points.at(0);
                    padfX[j] = dfp.X;
                    padfY[j] = dfp.Y;
                    padfZ[j] = dfp.Z;
                }

                // Cria objeto
                SHPObject* obj = SHPCreateSimpleObject(nShapeType, nVertices, padfX, padfY, padfZ );

                // Guarda objeto e o nome
                objNames.push_back(df.name);
                shpObjs.push_back(obj);

                //Liberando memória
                free( padfX );
                free( padfY );
                free( padfZ );
            }
        }

        // Gravando novo shape
        if (flag)
        {
            // Concatenando nome de arquivo e nome de classe
            std::string current_filename = std::string(filename);
            current_filename.append("_").append(fc.name);

            // Criando novo shape
            SHPHandle mySHP = SHPCreate(current_filename.c_str(), nShapeType);
            if( mySHP == NULL )
            {
                printf( "Unable to create shapefile.\n" );
                return 2;
            }

            // Criando DBF
            DBFHandle myDBF = DBFCreate(current_filename.c_str());
            if( myDBF == NULL )
            {
                printf( "Unable to create DBF file.\n" );
                return 3;
            }

            // Criando campo para armazenar os nomes das feições
            int iField = DBFAddField(myDBF, "Nome", FTString, 255, 0);

            // Inserindo objetos(feições) no novo shape
            unsigned int nObjs = shpObjs.size();
            for (unsigned int i=0; i<nObjs; i++)
            {
                std::string name = objNames.at(i);
                SHPObject* obj = shpObjs.at(i);

                // Identificando objeto
                obj->nShapeId = i;

                // Escrevendo objeto no shape
                SHPWriteObject( mySHP, -1, obj );

                // Inserindo registros para os objetos
                DBFWriteStringAttribute( myDBF, obj->nShapeId, iField, name.c_str());

                // Liberando memória
                SHPDestroyObject( obj );
            }

            //Limpando as filas
            shpObjs.clear();
            objNames.clear();

            // Fechando arquivos
            SHPClose( mySHP );
            DBFClose( myDBF );
        }
    }

    return 0;
}

int DemFeatures::saveFeatSp165(char *filename)
{
    // If list is empty, return
    if (features.size() == 0)
        return 0;

    std::string name = std::string(filename);
    //name.append(".txt");

    // Open file to save
    std::ofstream arq(name);
    if (arq.fail())
    {
        printf("Problems while saving ...\n");
        return 0;
    }

    DemFeature df;

    // Save polygon features first
    arq << "<EFOTO_FEATURES>\n";
    for (unsigned int i=0; i<features.size(); i++)
    {
        df = features.at(i);
        arq << i+1 << "\n"; // Feature ID
        arq << getClassIdToSp165(df.feature_class) <<"\n"; // Class type
        arq << getFeatureTypeName(df.feature_type) << "\n"; // Name of feature
        arq << df.name << "\n"; // Description (changed to name in this version)
    }
    arq << "</EFOTO_FEATURES>\n";

    // Now, save the points
    arq << "<EFOTO_POINTS>\n";
    arq.precision(20);
    //double lc,lr,rc,rr,X,Y,Z;
    DemFeaturePoints dfp;
    for (unsigned int i=0; i<features.size(); i++)
    {
        df = features.at(i);
        //pos = findFeature2D(i+1);

        for (unsigned int j=0; j<df.points.size(); j++)
        {
            dfp = df.points.at(j);

            arq << i+1 << "\n"; // Feature ID
            arq << j+1 << "\n"; // Point ID
            arq << dfp.left_x << "\n"; // Left Column
            arq << dfp.left_y << "\n"; // Left Row
            arq << dfp.right_x << "\n"; // Right Column
            arq << dfp.right_y << "\n"; // Right Row
            arq << dfp.X << "\n"; // X
            arq << dfp.Y << "\n"; // Y
            arq << dfp.Z << "\n"; // Z
        }
        // If feature is a polygon, close it with a C1 marker
        if (df.feature_type > 2)
        {
            dfp = df.points.at(0);

            arq << i+1 << "\n"; // Feature ID
            arq << "C1" << "\n"; // Point ID
            arq << dfp.left_x << "\n"; // Left Column
            arq << dfp.left_y << "\n"; // Left Row
            arq << dfp.right_x << "\n"; // Right Column
            arq << dfp.right_y << "\n"; // Right Row
            arq << dfp.X << "\n"; // X
            arq << dfp.Y << "\n"; // Y
            arq << dfp.Z << "\n"; // Z
        }
    }
    arq << "</EFOTO_POINTS>\n";

    arq.close();
    return 1;
}

// Feature id ranges from 1-N
// Return just a copy for reading
DemFeature DemFeatures::getFeature(int feat)
{
    // Create empty object
    DemFeature df;
    if (feat < 1 || unsigned(feat) > features.size())
        return df;

    return features.at(feat-1);
}

// Feature id ranges from 1-N
DemFeature* DemFeatures::getFeatureLink(int feat)
{
    // Create empty object
    if (feat < 1 || unsigned(feat) > features.size())
        return NULL;

    return &features.at(feat-1);
}

// Returns feature id
int DemFeatures::addNewFeature(std::string name, std::string fdesc, int fclass, int ftype, int layer)
{
    DemFeature df;
    df.name = name;
    df.description = fdesc;
    df.feature_class = fclass;
    df.feature_type = ftype;
    df.layer = layer;

    features.push_back(df);

    return features.size();
}

int DemFeatures::deleteFeature(int featid)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return 0;

    features.erase(features.begin()+featid-1);

    return 1;
}

int DemFeatures::copyFeature(int featid, double shift=0.0)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return 0;

    DemFeature df;
    df = features.at(featid-1);

    // Add shift to NE direction
    for (unsigned int i=0; i<df.points.size(); i++)
    {
        df.points.at(i).X +=  shift;
        df.points.at(i).Y +=  shift;
    }

    features.push_back(df);

    return features.size();
}

// Returns point id - 0, if feature_id is invalid
void DemFeatures::addNewPoint(int featid, int pointid, double X, double Y, double Z)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    // If feature is point, no more than 1 point is allowed
    if (features.at(featid-1).points.size() > 0 && features.at(featid-1).feature_type == 1)
        return;

    // If point id is invalid, return
    if (pointid < 1)
        return;

    DemFeaturePoints dfp;
    dfp.X = X;
    dfp.Y = Y;
    dfp.Z = Z;

    // Insert point
    if (unsigned(pointid) > features.at(featid-1).points.size())
        features.at(featid-1).points.push_back(dfp);
    else
        features.at(featid-1).points.insert(features.at(featid-1).points.begin()+pointid-1, dfp);

    calculateFeatureAttributes(featid);
}

void DemFeatures::updatePoint(int featid, int pointid, double X, double Y, double Z)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    // check if point is valid
    if (pointid < 1 || unsigned(pointid) > features.at(featid-1).points.size())
        return;

    features.at(featid-1).points.at(pointid-1).X = X;
    features.at(featid-1).points.at(pointid-1).Y = Y;
    features.at(featid-1).points.at(pointid-1).Z = Z;

    calculateFeatureAttributes(featid);
}

void DemFeatures::update2DPoint(int featid, int pointid, double lx, double ly, double rx, double ry)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    // check if point is valid
    if (pointid < 1 || unsigned(pointid) > features.at(featid-1).points.size())
        return;

    features.at(featid-1).points.at(pointid-1).left_x = lx;
    features.at(featid-1).points.at(pointid-1).left_y = ly;
    features.at(featid-1).points.at(pointid-1).right_x = rx;
    features.at(featid-1).points.at(pointid-1).right_y = ry;

    checkIsOnScreen(featid);
}

int DemFeatures::deletePoint(int featid, int pointid)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return 0;

    // check if point is valid
    if (pointid < 1 || unsigned(pointid) > features.at(featid-1).points.size())
        return 0;

    features.at(featid-1).points.erase(features.at(featid-1).points.begin()+pointid-1);

    calculateFeatureAttributes(featid);

    return 1;
}

void DemFeatures::setName(int featid, std::string _name)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    features.at(featid-1).name = _name;
}

std::string DemFeatures::getName(int featid)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return "";

    return features.at(featid-1).name;
}

void DemFeatures::setDescription(int featid, std::string _desc)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    features.at(featid-1).description = _desc;
}

std::string DemFeatures::getDescription(int featid)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return "";

    return features.at(featid-1).description;
}

void DemFeatures::setFeatureClassId(int featid, int fclass)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    features.at(featid-1).feature_class = fclass;
}

int DemFeatures::getFeatureClassId(int featid)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return -1;

    return features.at(featid-1).feature_class;
}

void DemFeatures::setFeatureType(int featid, int ftype)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    features.at(featid-1).feature_type = ftype;
}

int DemFeatures::getFeatureType(int featid)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return -1;

    return features.at(featid-1).feature_type;
}

void DemFeatures::setLayer(int featid, int layer)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    features.at(featid-1).layer = layer;
}

int DemFeatures::getLayer(int featid)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return -1;

    return features.at(featid-1).layer;
}

int DemFeatures::getNumPoints(int featid)
{
    // Check if feature is valid
    if (featid < 1 || unsigned(featid) > features.size())
        return -1;

    return features.at(featid-1).points.size();
}

// Return class id
int DemFeatures::addFeatureClass(std::string name, std::string description, int ccolor, int fcolor)
{
    FeatureClass fc;
    fc.name = name;
    fc.description = description;
    fc.outline_color = ccolor;
    fc.fill_color = fcolor;

    return addFeatureClass(fc);
}

// Return class id
int DemFeatures::addFeatureClass(FeatureClass fc)
{
    feature_classes.push_back(fc);

    return feature_classes.size();
}

//
// Arithmetic functions
//

void DemFeatures::calculateFeaturesAttributes()
{
    for (unsigned int i=0; i<features.size(); i++)
    {
        // Check if centroid was already calculated
        if (features.at(i).flag_calc_centroid)
            continue;

        calculateCentroid(i+1);
        calculatePerimeter(i+1);
        calculateArea(i+1);
    }
}

void DemFeatures::calculateFeatureAttributes(int featid)
{
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    calculateCentroid(featid);
    calculatePerimeter(featid);
    calculateArea(featid);
}

// Calculate the average
void DemFeatures::calculateCentroid(int featid)
{
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    DemFeature *df =  &features.at(featid-1);

    if (df->points.size() == 0)
    {
        df->flag_calc_centroid = false;
        return;
    }

    double CX, CY, CZ;
    CX = CY = CZ = 0;

    for (unsigned int k=0; k<df->points.size(); k++)
    {
        CX += df->points.at(k).X;
        CY += df->points.at(k).Y;
        CZ += df->points.at(k).Z;
    }

    df->centroid.X = CX/double(df->points.size());
    df->centroid.Y = CY/double(df->points.size());
    df->centroid.Z = CZ/double(df->points.size());
    df->flag_calc_centroid = true;
}

void DemFeatures::calculatePerimeter(int featid)
{
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    DemFeature *df =  &features.at(featid-1);
    double RX, RY, RZ, perimeter = 0.0;

    // If point, no perimeter
    if (df->feature_type == 1 || df->points.size() < 2)
    {
        df->perimeter = 0.0;
        return;
    }

    for (unsigned int k=0; k<df->points.size()-1; k++)
    {
        RX = df->points.at(k+1).X - df->points.at(k).X;
        RY = df->points.at(k+1).Y - df->points.at(k).Y;
        RZ = df->points.at(k+1).Z - df->points.at(k).Z;
        perimeter += sqrt(pow(RX,2) + pow(RY,2) + pow(RZ,2));
    }

    // Add the closing line to the polygon
    int last_pt = df->points.size() - 1;
    if (df->feature_type == 3 && df->points.size() > 2)
    {
        RX = df->points.at(0).X - df->points.at(last_pt).X;
        RY = df->points.at(0).Y - df->points.at(last_pt).Y;
        RZ = df->points.at(0).Z - df->points.at(last_pt).Z;
        perimeter += sqrt(pow(RX,2) + pow(RY,2) + pow(RZ,2));
    }

    df->perimeter = perimeter;
}

void DemFeatures::calculateArea(int featid)
{
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    DemFeature *df = &features.at(featid-1);

    // Calculate area only for polygons
    if (df->feature_type < 3)
    {
        df->area = 0.0;
        return;
    }

    int num_points = df->points.size();
    if (num_points < 3)
    {
        df->area = 0.0;
        return;
    }

    // Calculate normal vector
    double NX, NY, NZ, X1, Y1, Z1, X2, Y2, Z2, VX, VY, VZ;
    NX = NY = NZ = VX = VY = VZ = 0.0;
    for (int i=0; i<num_points; i++)
    {
        X1 = df->points.at(i).X;
        Y1 = df->points.at(i).Y;
        Z1 = df->points.at(i).Z;
        X2 = df->points.at((i+1) % num_points).X;
        Y2 = df->points.at((i+1) % num_points).Y;
        Z2 = df->points.at((i+1) % num_points).Z;
        NX += Y1*Z2 - Y2*Z1;
        NY += - (Z2*X1 - Z1*X2);
        NZ += X1*Y2 - X2*Y1;
    }

    // Calculate area
    for (int i=0; i<num_points - 1; i++)
    {
        X1 = df->points.at(i).X;
        Y1 = df->points.at(i).Y;
        Z1 = df->points.at(i).Z;
        X2 = df->points.at((i+1) % num_points).X;
        Y2 = df->points.at((i+1) % num_points).Y;
        Z2 = df->points.at((i+1) % num_points).Z;
        VX += X1-X2;
        VY += Y1-Y2;
        VZ += Z1-Z2;
    }

    df->area = sqrt(pow(VX-NX,2)+pow(VY-NY,2)+pow(VZ-NZ,2)) / 2.0;
}

void DemFeatures::checkAllIsOnScreen()
{
    for (unsigned int i=1; i<=features.size(); i++)
        checkIsOnScreen(i);
}

void DemFeatures::checkIsOnScreen(int featid)
{
    if (featid < 1 || unsigned(featid) > features.size())
        return;

    DemFeature *df = &features.at(featid-1);

    df->is_on_screen = 0;

    if (img_left_width == -1 || img_left_height == -1 || img_right_width == -1 || img_right_height == -1)
        return;

    // Flag - set if at least one point is found on screen
    // 1 - Feature is on left image
    // 2 - Feature is on right image
    // 3 - Feature is on both images
    for (unsigned int i=0; i<df->points.size(); i++)
    {
        if (int(df->points.at(i).left_x) >= 0 && int(df->points.at(i).left_x) < img_left_width && int(df->points.at(i).left_y) >= 0 && int(df->points.at(i).left_y) < img_left_height)
            df->is_on_screen = df->is_on_screen | 1;

        if (int(df->points.at(i).right_x) >= 0 && int(df->points.at(i).right_x) < img_right_width && int(df->points.at(i).right_y) >= 0 && int(df->points.at(i).right_y) < img_right_height)
            df->is_on_screen = df->is_on_screen | 2;
    }
}

// Return 0, if feature list is empty
int DemFeatures::getNearestFeature(double X, double Y, double Z)
{
    if (features.size() < 1)
        return 0;

    int best_feat = 1;
    double dist, best_dist = sqrt(pow(X - features.at(0).centroid.X, 2) + pow(Y - features.at(0).centroid.Y, 2) + pow(Z - features.at(0).centroid.Z, 2));
    double dist_k;

    for (unsigned int i=1; i<features.size(); i++)
    {
        dist = sqrt(pow(X - features.at(i).centroid.X, 2) + pow(Y - features.at(i).centroid.Y, 2) + pow(Z - features.at(i).centroid.Z, 2));

        // If line, set nearest point as distance
        if (features.at(i).feature_type == 2)
        {
            if (features.at(i).points.size() < 1)
                break;

            dist = sqrt(pow(X - features.at(i).points.at(0).X, 2) + pow(Y - features.at(i).points.at(0).Y, 2) + pow(Z - features.at(i).points.at(0).Z, 2));

            for (unsigned int k=1; k<features.at(i).points.size(); k++)
            {
                dist_k = sqrt(pow(X - features.at(i).points.at(k).X, 2) + pow(Y - features.at(i).points.at(k).Y, 2) + pow(Z - features.at(i).points.at(k).Z, 2));

                if (dist_k < dist)
                    dist = dist_k;
            }
        }

        if (dist < best_dist)
        {
            best_dist = dist;
            best_feat = i+1;
        }
    }

    return best_feat;
}

int DemFeatures::getNearestPoint(int fid, double X, double Y, double Z)
{
    if (fid < 1 || unsigned(fid) > features.size())
        return 0;

    int best_pt = 1;
    double best_dist = sqrt(pow(X - features.at(fid-1).points.at(0).X, 2) + pow(Y - features.at(fid-1).points.at(0).Y, 2) + pow(Z - features.at(fid-1).points.at(0).Z, 2));
    double dist;

    for (unsigned int i=1; i<features.at(fid-1).points.size(); i++)
    {
        dist = sqrt(pow(X - features.at(fid-1).points.at(i).X, 2) + pow(Y - features.at(fid-1).points.at(i).Y, 2) + pow(Z - features.at(fid-1).points.at(i).Z, 2));
        if (dist < best_dist)
        {
            best_pt = i+1;
            best_dist = dist;
        }
    }

    return best_pt;
}

void DemFeatures::getNearestPoint(double X, double Y, double Z, int& fid, int& pid)
{
    if (features.size() < 1)
        return;

    int best_pid = 1;
    int best_fid = 1;
    double best_dist = 10e100, dist;

    for (fid=1; unsigned(fid)<= features.size(); fid++)
    {

        for (pid=1; unsigned(pid)<=features.at(fid-1).points.size(); pid++)
        {
            dist = sqrt(pow(X - features.at(fid-1).points.at(pid-1).X, 2) + pow(Y - features.at(fid-1).points.at(pid-1).Y, 2) + pow(Z - features.at(fid-1).points.at(pid-1).Z, 2));
            if (dist < best_dist)
            {
                best_pid = pid;
                best_fid = fid;
                best_dist = dist;
            }
        }
    }

    fid = best_fid;
    pid = best_pid;
}

//
// Stereoplotter 1.65 and DEM 2.5 compatibility
//
void DemFeatures::convertClassesIdsFromSp165()
{
    int fc, ft;
    for (unsigned int i=0; i<features.size(); i++)
    {
        ft = features.at(i).feature_type;
        fc = features.at(i).feature_class;
        features.at(i).feature_class = convClassFromSp165(ft, fc);
    }
}

int DemFeatures::convClassFromSp165(int ft, int fc)
{
    int new_fc;

    if (ft == 1)
        return 2;

    if (fc == 0)
        return 1;

    if (ft == 2)
    {
        switch (fc)
        {
        case 1 : new_fc = 3; break;
        case 2 : new_fc = 4; break;
        case 3 : new_fc = 5; break;
        case 4 : new_fc = 6; break;
        case 5 : new_fc = 7; break;
        case 6 : new_fc = 8; break;
        default: new_fc = 1;
        }
    }

    if (ft == 3)
    {
        switch (fc)
        {
        case 1 : new_fc = 9; break;
        case 2 : new_fc = 10; break;
        case 3 : new_fc = 11; break;
        case 4 : new_fc = 12; break;
        case 5 : new_fc = 13; break;
        case 6 : new_fc = 14; break;
        case 7 : new_fc = 15; break;
        case 8 : new_fc = 16; break;
        case 9 : new_fc = 17; break;
        case 10 : new_fc = 18; break;
        case 11 : new_fc = 19; break;
        default: new_fc = 1;
        }
    }

    return new_fc;
}

int DemFeatures::getClassIdToSp165(int new_class)
{
    int old_class;

    switch (new_class)
    {
    case 1 : old_class = 0; break;
    case 2 : old_class = 1; break;
    case 3 : old_class = 1; break;
    case 4 : old_class = 2; break;
    case 5 : old_class = 3; break;
    case 6 : old_class = 4; break;
    case 7 : old_class = 5; break;
    case 8 : old_class = 6; break;
    case 9 : old_class = 1; break;
    case 10 : old_class = 2; break;
    case 11 : old_class = 3; break;
    case 12 : old_class = 4; break;
    case 13 : old_class = 5; break;
    case 14 : old_class = 6; break;
    case 15 : old_class = 7; break;
    case 16 : old_class = 8; break;
    case 17 : old_class = 9; break;
    case 18 : old_class = 10; break;
    case 19 : old_class = 11; break;
    }

    return old_class;
}

void DemFeatures::createClassesFromSp165()
{
    //int p_class = feature_classes.size();

    FeatureClass fc;

    // Undefined
    fc.type = 0;
    fc.description = "";
    fc.fill_transparency = 0xFFFFFF;
    fc.line_type = 1;
    fc.outline_color = 0;
    fc.fill_color = 0;
    fc.name = "Undefined";
    feature_classes.push_back(fc);

    // Point
    fc.type = 1;
    fc.outline_color = 0xFF0000;
    fc.name = "Point";
    feature_classes.push_back(fc);

    // Line
    fc.type = 2;
    fc.outline_color = 0xFF0000;
    fc.fill_color = 0;
    fc.name = "Paved street";
    feature_classes.push_back(fc);
    fc.outline_color = 0xFF808000;
    fc.name = "Unpaved street"; /// ???
    feature_classes.push_back(fc);
    fc.outline_color = 0xFF00FF00 ;
    fc.name = "Trail";
    feature_classes.push_back(fc);
    fc.outline_color = 0xFFFF00FF;
    fc.name = "Railway";
    feature_classes.push_back(fc);
    fc.outline_color = 0xFF00FFFF;
    fc.name = "River";
    feature_classes.push_back(fc);
    fc.outline_color = 0xFF0000FF;
    fc.name = "Bridge";
    feature_classes.push_back(fc);

    // Polygon
    fc.type = 3;
    fc.outline_color = 0xFFFF8080;
    fc.fill_color = 0xFFFF8000;
    fc.name = "House";
    feature_classes.push_back(fc);
    fc.fill_color = 0xFFC0C0C0;
    fc.name = "Building";
    feature_classes.push_back(fc);
    fc.fill_color = 0xFF808080;
    fc.name = "Industrial";
    feature_classes.push_back(fc);
    fc.fill_color = 0xFFFFFF80;
    fc.name = "Club";
    feature_classes.push_back(fc);
    fc.fill_color = 0xFFFF00FF;
    fc.name = "Station";
    feature_classes.push_back(fc);
    fc.fill_color = 0xFF804000;
    fc.name = "Wasteland";
    feature_classes.push_back(fc);
    fc.fill_color = 0xFFC0FFC0;
    fc.name = "Square";
    feature_classes.push_back(fc);
    fc.fill_color = 0xFF00FF00;
    fc.name = "Park";
    feature_classes.push_back(fc);
    fc.fill_color = 0xFF00FF00;
    fc.name = "Forest";
    feature_classes.push_back(fc);
    fc.fill_color = 0xFF0000FF;
    fc.name = "Lagoon";
    feature_classes.push_back(fc);
    fc.fill_color = 0xFF00FFFF;
    fc.name = "Pool";
    feature_classes.push_back(fc);
}

int DemFeatures::loadFeatSp165(char *filename, bool append=false)
{
    // Clear list, if not append
    if (!append)
        features.clear();

    // Get pointer to the last feature
    int p_feat = features.size();

    // Start to read file info
    std::ifstream arq(filename); // open the emf file

    if (arq.fail())
    {
        printf("Error while opening file !\n");
        return 0;
    }

    // Search for features (search the <EFOTO_FEATURES> tag in the first 20 lines)
    std::string tag;
    bool flag = false;
    int count=0;
    while ((!flag) && (!arq.fail()) && (count < 20))
    {
        getline(arq,tag);
        tag = tag.substr(0,16);
        if (tag.compare("<EFOTO_FEATURES>")==0)
            flag = true;
        count++;
    }

    if (!flag)
    {
        printf("This is not a E-FOTO Feature file %d !\n",count);
        arq.close();
        return 0;
    }

    // Start to read the features
    // Create empty features
    //	arq.seekg (0, ios::beg);
    DemFeature df;
    df.layer = 1;
    df.description = "";
    df.flag_calc_centroid = false;
    while (!arq.fail())
    {
        // Features EOF
        getline(arq,tag); // 1st line id#
        tag = tag.substr(0,17);
        if ((tag.compare("</EFOTO_FEATURES>")==0) || (arq.fail()))
            break;

        // Else, insert element
        getline(arq,tag); // 2nd line class
        df.feature_class = atoi(tag.c_str());

        getline(arq,tag); // 3rd line type of feature
        if (tag.substr(0,5).compare("Point")==0)
            df.feature_type = 1;
        if (tag.substr(0,4).compare("Line")==0)
            df.feature_type = 2;
        if (tag.substr(0,7).compare("Polygon")==0)
            df.feature_type = 3;

        getline(arq,tag); // 4th line description
        df.name = tag;

        features.push_back(df);
    }

    // Search for points (search the <EFOTO_POINTS> tag in the first 20 lines)
    flag = false;
    count=0;
    while ((!flag) && (!arq.fail()) && (count < 20))
    {
        getline(arq,tag);
        tag = tag.substr(0,14);
        if (tag.compare("<EFOTO_POINTS>")==0)
            flag = true;
        count++;
    }

    if (!flag)
    {
        printf("Could not find the points. This file may be corrupted !\n");
        arq.close();
        return 0;
    }

    // Start to read the points
    int feature_id, point_id;
    DemFeaturePoints dfp;
    //double lx, ly, rx, ry;
    while (!arq.fail())
    {
        // Points EOF
        getline(arq,tag); // 1st line feature_id
        tag = tag.substr(0,15);
        if ((tag.compare("</EFOTO_POINTS>")==0) || (arq.fail()))
            break;

        feature_id = atoi(tag.c_str());
        feature_id += p_feat;
        getline(arq,tag); // 2nd line point_id
        point_id = atoi(tag.c_str());

        // Check if point is not a closing point (string "C1")
        if (point_id != 0)
        {
            // Digital coords
            getline(arq,tag); // 3rd line left_column
            dfp.left_x = Conversion::stringToDouble(tag);
            getline(arq,tag); // 4th line left_row
            dfp.left_y = Conversion::stringToDouble(tag);
            getline(arq,tag); // 5th line right_column
            dfp.right_x = Conversion::stringToDouble(tag);
            getline(arq,tag); // 6th line right_row
            dfp.right_y = Conversion::stringToDouble(tag);

            //insertFeature2D(feature_id, lx, ly, rx, ry);

            // X, Y, Z
            getline(arq,tag); // 7th line X
            dfp.X = Conversion::stringToDouble(tag);
            getline(arq,tag); // 8th line Y
            dfp.Y = Conversion::stringToDouble(tag);
            getline(arq,tag); // 9th line Z
            dfp.Z = Conversion::stringToDouble(tag);

            features.at(feature_id-1).points.push_back(dfp);
        }
        else
        {
            // Jump to another point (read file blindly)
            for (unsigned int i=0; i<7; i++)
                getline(arq,tag);
        }
    }

    arq.close();

    // Calculate centroids
    calculateFeaturesAttributes();

    // Create classes from SP 1.65
    createClassesFromSp165();
    convertClassesIdsFromSp165();

    // Check if points are on screen
    checkAllIsOnScreen();

    return 1;
}

//
// Output
//

void DemFeatures::showFeatures(bool full=false)
{
    printf("Total features: %d\n",(int)features.size());

    DemFeature df;

    for (unsigned int i=0; i<features.size(); i++)
    {
        df = features.at(i);
        printf("Feature #%d:\n",i+1);
        printf(" Type: %s\n",getFeatureTypeName(df.feature_type).c_str());
        printf(" Class: %s\n",getFeatureClass(df.feature_class)->name.c_str());
        printf(" Name: %s\n",df.name.c_str());
        printf(" Description: %s\n",df.description.c_str());
        printf(" Points: %d\n",(int)df.points.size());
        if (full)
        {
            for (unsigned int k=0; k<df.points.size(); k++)
                printf("  %f, %f, %f\n",df.points.at(k).X, df.points.at(k).Y, df.points.at(k).Z);
        }
        printf(" Centroid: %f, %f, %f\n",df.centroid.X, df.centroid.Y, df.centroid.Z);
        printf(" Perimeter: %f\n",df.perimeter);
        printf(" Area: %f\n",df.area);
        printf(" Is on screen flag: %d\n",df.is_on_screen);
    }
}

void DemFeatures::showClasses()
{
    printf("Total classes: %d\n",(int)feature_classes.size());

    for (unsigned int i=0; i<feature_classes.size(); i++)
    {
        printf("Class #%d\n",i+1);
        printf(" Name: %s\n",feature_classes.at(i).name.c_str());
        printf(" Description: %s\n",feature_classes.at(i).description.c_str());
        printf(" Outline color: %06X\n",feature_classes.at(i).outline_color);
        printf(" Fill color: %06X\n",feature_classes.at(i).fill_color);
    }
}

/*
 * This function converts points and lines to pair list
 * usePolygons enable using polygons too
 * The pair list is not cleared !
 */
void DemFeatures::addFeaturesToPairList(MatchingPointsList *mpl, bool usePolygons = false)
{
    // Search for features
    for (unsigned int f=0; f<features.size(); f++)
    {
        DemFeature *df =  &features.at(f);

        if (!usePolygons && df->feature_type == 3)
            continue;

        // Add point
        for (unsigned int k=0; k<df->points.size(); k++)
            mpl->add(df->points.at(k).X, df->points.at(k).Y, df->points.at(k).Z);

    }
}

/*
 * This function creates a polygon map
 */
Matrix DemFeatures::createPolygonMap(double Xi, double Yi, double Xf, double Yf, double res_x, double res_y)
{
    Matrix polyMap;

    // Calculate DEM size
    int dem_width, dem_height;
    dem_width = int(1.0 + floor((Xf - Xi) / res_x));
    dem_height = int(1.0 + floor((Yf - Yi) / res_y));

    polyMap.resize(dem_height, dem_width);

    for (unsigned int i=1; i<=features.size(); i++)
        addPolygonToMap(i, &polyMap, Xi, Yi, res_x, res_y);

    return polyMap;
}

void DemFeatures::addPolygonToMap(int feat_id, Matrix *map, double Xi, double Yi, double res_x, double res_y)
{
    Matrix polMap = mapPolygon(feat_id, res_x, res_y);

    double fXi, fXf, fYi, fYf;
    calculateBoundingBox(feat_id, fXi, fYi, fXf, fYf);

    //double offset_X = fXi - Xi;
    //double offset_Y = fYi - Yi;

    double X, Y;
    int row, col;
    for (unsigned int i=1; i<=polMap.getRows(); i++)
    {
        for (unsigned int j=1; j<=polMap.getCols(); j++)
        {
            if (polMap.get(i,j) - 0.0 < 0.000000000001)
                continue;

            // Get poly map coords
            X = fXi + j;
            Y = fYi + i;

            // Get main map row, col
            col = 1 + floor((X-Xi)/res_x);
            row = 1 + floor((Y-Yi)/res_y);

            if (col < 1 || row < 1 || unsigned(col) > map->getCols() || unsigned(row) > map->getRows())
                continue;

            if (polMap.get(i,j) > map->get(row,col))
                map->set(row,col,polMap.get(i,j));
        }
    }
}

Matrix DemFeatures::mapPolygon(int feat_id, double res_x, double res_y)
{
    Matrix pmap;

    if (feat_id < 1 || unsigned(feat_id) > features.size())
        return pmap;

    // Only polygons
    if (features.at(feat_id-1).feature_type !=3)
        return pmap;

    // Calculate bounding box
    double Xi, Xf, Yi, Yf;
    calculateBoundingBox(feat_id, Xi, Yi, Xf, Yf);
    double D0 = sqrt(pow(Xf-Xi,2) + pow(Yf-Yi,2));

    // Resize map
    int pmap_width = int(1.0 + floor((Xf - Xi) / res_x));
    int pmap_height = int(1.0 + floor((Yf - Yi) / res_y));
    pmap.resize(pmap_height, pmap_width);

    // Map
    double X,Y;
    for (int i=0; i<pmap_height; i++)
    {
        for (int j=0; j<pmap_width; j++)
        {
            X = Xi + j;
            Y = Yi + i;

            if (!(isInside(feat_id, X, Y)))
                continue;

            pmap.set(i, j, interpolateXYPolygon(feat_id, X, Y, D0));
        }
    }

    return pmap;
}

// Moving average
double DemFeatures::interpolateXYPolygon(int feat_id, double X, double Y, double D0 = -1.0)
{
    if (feat_id < 1 || unsigned(feat_id) > features.size())
        return 0.0;

    // Calculate D0, if not given
    if (D0 < 0.0)
    {
        double Xi, Yi, Xf, Yf, delta_X, delta_Y;
        calculateBoundingBox(feat_id, Xi, Yi, Xf, Yf);
        delta_X = Xf-Xi;
        delta_Y = Yf-Yi;
        D0 = sqrt(delta_X*delta_X + delta_Y*delta_Y);
    }

    DemFeature *df = &features.at(feat_id-1);
    int n = df->points.size();

    // Calculate weights
    double d, D, sum1, sum2, weight;
    sum1=0.0;
    sum2=0.0;
    for (unsigned int i=0; i < df->points.size(); i++)
    {
        D = sqrt(pow(X - df->points.at(i).X,2) + pow(Y - df->points.at(i).Y,2));
        if (D < D0)
        {
            d = D/D0;
            weight = (1/pow(d,n))-1;
            //            weight = 1 - pow(d,n);
            sum1 += weight*df->points.at(i).Z;
            sum2 += weight;
        }
    }

    if (sum2 > 0.00000000000000000001)
        return sum1/sum2;

    return 0.0;
}

void DemFeatures::calculateBoundingBox(int feat_id, double &Xi, double &Yi, double &Xf, double &Yf)
{
    if (feat_id < 1 || unsigned(feat_id) > features.size())
        return;

    DemFeature df =  features.at(feat_id-1);
    int n = df.points.size();

    if (n < 3)
        return;

    Xi = Xf = df.points.at(0).X;
    Yi = Yf = df.points.at(0).Y;

    for (int i=1; i<n; i++)
    {
        if (df.points.at(i).X < Xi) Xi = df.points.at(i).X;
        if (df.points.at(i).Y < Yi) Yi = df.points.at(i).Y;
        if (df.points.at(i).X > Xf) Xf = df.points.at(i).X;
        if (df.points.at(i).Y > Yf) Yf = df.points.at(i).Y;
    }
}

/*
 * Check if a given point is inside a polygon
 */
double DemFeatures::angle2D(double X1, double Y1, double X2, double Y2)
{
    double dtheta,theta1,theta2;

    theta1 = atan2(Y1,X1);
    theta2 = atan2(Y2,X2);
    dtheta = theta2 - theta1;
    while (dtheta > M_PI)
        dtheta -= M_PI*2;
    while (dtheta < -M_PI)
        dtheta += M_PI*2;

    return dtheta;
}

bool DemFeatures::isInside(int feat_id, double X, double Y)
{
    if (feat_id < 1 || unsigned(feat_id) > features.size())
        return false;

    double angle=0.0;
    DemFeature df =  features.at(feat_id-1);
    unsigned int n = df.points.size();

    // Calculate
    for (unsigned int i=0; i<n; i++)
        angle += angle2D(df.points.at(i).X - X, df.points.at(i).Y - Y, df.points.at((i+1)%n).X - X, df.points.at((i+1)%n).Y - Y);

    if (fabs(angle) < M_PI)
        return false;

    return true;
}

std::string DemFeatures::getFeaturesList()
{
    std::stringstream txt;
    DemFeature df;

    txt << std::fixed << std::setprecision(5);

    for (unsigned int i=0; i<features.size(); i++)
    {
        df = features.at(i);

        txt << "Feature #" << i+1 << "\t" << getFeatureTypeName(df.feature_type) << ", " << getFeatureClass(df.feature_class)->name << ", " << df.name << "\n";

        for (unsigned int k=0; k<df.points.size(); k++)
            txt << "\tPoint #" << k+1 << "\tX=" << df.points.at(k).X << ", Y=" << df.points.at(k).Y << ", Z=" << df.points.at(k).Z << "\n";
    }

    return txt.str();
}

std::string DemFeatures::getFeaturesToDisplay(int mode)
{
    // Mode 0 = Digital coordinates
    // Mode 1 = Terrain coordinartes

    std::stringstream txt;
    DemFeature df;

    txt << std::fixed << std::setprecision(5);

    // Number of features
    txt << features.size() << "\n";

    //int pos;
    //double lx, ly, rx, ry;
    for (unsigned int i=0; i<features.size(); i++)
    {
        df = features.at(i);

        txt << df.feature_type << "\t " << df.points.size() << "\n";

        for (unsigned int k=0; k<df.points.size(); k++)
        {
            if (mode == 1)
                txt << df.points.at(k).X << "\t" << df.points.at(k).Y << "\t" << df.points.at(k).Z << "\n";
            else
                txt << df.points.at(k).left_x << "\t" << df.points.at(k).left_y << "\t" << df.points.at(k).right_x << "\t" << df.points.at(k).right_y << "\n";
        }
    }

    return txt.str();
}

void DemFeatures::setImagePairSize(int lw, int lh, int rw, int rh)
{
    img_left_width = lw;
    img_left_height = lh;
    img_right_width = rw;
    img_right_height = rh;
}

/* Method into disuse:
 *
void DemFeatures::setFeatureClass(int classid, FeatureClass fc)
{
    if (classid < 1 || classid > feature_classes.size())
        return;
}
*/

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
