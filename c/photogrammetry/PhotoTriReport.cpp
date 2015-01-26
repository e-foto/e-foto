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

#include "PhotoTriReport.h"
#include "ProjectiveRay.h"
#include "Project.h"
#include "EFotoManager.h"
#include "SpatialIntersection.h"
#include "SpatialRessection.h"
#include "InteriorOrientation.h"
#include "Image.h"
#include "Point.h"
#include "FrameSensor.h"
#include "StereoPair.h"

#include "math.h"

#include <iomanip>
#include <fstream>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

PhotoTriReport::PhotoTriReport(EFotoManager* newManager)
{
    manager = newManager;
    project = manager->getProject();
}

/***********************************************
 *  Photo-triangulation Report
 ***********************************************/
int PhotoTriReport::createReport(char *filename)
{
    // Open file to save
    std::ofstream arq(filename);
    if (arq.fail())
    {
        printf("Problems while saving ...\n");
                return 0;
    }

    arq << "E-FOTO Photo-triangulation Full Report\n";
    arq << "======================================\n\n";
    arq << "Digital image unit: pixels\n";
    arq << "Angle unit: degrees\n";
    arq << "Ground X, Y, Z unit: meters\n\n";

    // Change number precision
    arq.precision(15);

    int no_imgs = project->allImages().size();

    arq << "Total images: " << no_imgs << "\n\n\n";

    //
    // Input images
    //
    arq << "Input Images:\n";
    arq << "=============\n\n";

    InteriorOrientation* io;
    SpatialRessection* eo;
    Image *img;
    Point *pnt;
    double row, col;
    double rad_to_deg = 180.0 / M_PI;

    for (int i=0; i<no_imgs; i++)
    {
        arq << "Image ID = " << i+1 << "\n\n";

        img = project->allImages().at(i);

        arq << "Point ID" << std::setw(9) << "Col" << std::setw(16) << "Row\n" ;

        for (int j=0; j< img->countPoints(); j++)
        {
            pnt = img->getPointAt(j);
            col = (double) pnt->getImageCoordinate(img->getId()).getCol();
            row = (double) pnt->getImageCoordinate(img->getId()).getLin();

            arq << std::setw(4) << pnt->getId() << std::setw(15) << Conversion::doubleToString(col,3) << std::setw(15) << Conversion::doubleToString(row,3) << "\n";
        }

        arq << "\n";

        io = img->getIO();

        arq << "Affine coefficients from image (pixels) to film (millimeters):\n";
        arq << std::setw(9) << "A0 " << std::setw(20) << "A1" << std::setw(20) << "A2" << std::setw(20) << "B0" << std::setw(20) << "B1" << std::setw(20) << "B2\n";
        arq << Conversion::doubleToString(io->getXa().get(1,1),10) << std::setw(20) << Conversion::doubleToString(io->getXa().get(2,1),10) << std::setw(20) << Conversion::doubleToString(io->getXa().get(3,1),10) << std::setw(20) << Conversion::doubleToString(io->getXa().get(4,1),10) << std::setw(20) << Conversion::doubleToString(io->getXa().get(5,1),10) << std::setw(20) << Conversion::doubleToString(io->getXa().get(6,1),10) << "\n";

        arq << "\n\n";
    }

    //
    // Photo-triangulation
    //
    arq << "Photo-triangulation results:\n";
    arq << "============================\n\n";

    std::string converged = "Yes";
    if (!project->photoTri()->getConverged()) converged = "No";

    arq << "Number of iterations: " << project->photoTri()->getTotalIterations() << "\n";
    arq << "Converged: " << converged << "\n";
    arq << "RMSE: " << Conversion::doubleToString(project->photoTri()->getRmse(),5) << "\n";
    arq << "\n\n";

    if(!project->photoTri()->getConverged())
    {
        arq.close();
        return 1;
    }

    arq << "The Exterior Orientation parameters:\n";
    arq << "Image ID " << std::setw(15) << "X0 " << std::setw(20) << "Y0" << std::setw(20) << "Z0" << std::setw(23) << "Omega" << std::setw(20) << "Phy" << std::setw(20) << "Kappa\n";

    for (int i=0; i<no_imgs; i++)
    {
        img = project->allImages().at(i);
        eo = img->getEO();
        arq << img->getId() << std::setw(30) << Conversion::doubleToString(eo->getXa().get(1,1),10) << std::setw(20) << Conversion::doubleToString(eo->getXa().get(2,1),10) << std::setw(20) << Conversion::doubleToString(eo->getXa().get(3,1),10) << std::setw(20) << Conversion::doubleToString(eo->getXa().get(4,1)*rad_to_deg,10) << std::setw(20) << Conversion::doubleToString(eo->getXa().get(5,1)*rad_to_deg,10) << std::setw(20) << Conversion::doubleToString(eo->getXa().get(6,1)*rad_to_deg,10) << "\n";
    }

    arq << "\n\n";
    arq << "The Interior Orientation parameters:\n";
    arq << "Image ID " << std::setw(7) << "f(mm)" << std::setw(15) << "x0" << std::setw(15) << "y0\n";
    Sensor * sensor;
    for (int i=0; i<no_imgs; i++)
    {
        img = project->allImages().at(i);
        sensor = img->getSensor();
        arq << img->getId() << std::setw(17) << Conversion::doubleToString(sensor->getFocalDistance(),5) << std::setw(15) << Conversion::doubleToString(sensor->getPrincipalPointCoordinates().getXi(),5) << std::setw(15) << Conversion::doubleToString(sensor->getPrincipalPointCoordinates().getEta(),5) << "\n";
    }

    arq << "\n\n";

    arq << "The residuals of control points (meters):\n";
    arq << "Point ID " << std::setw(13) << "rX " << std::setw(15) << "rY" << std::setw(15) << "rZ\n";

    Matrix mat_aux = calculateXYZerror();

    for (unsigned int i=1; i<=mat_aux.getRows(); i++)
        arq << int(mat_aux.get(i,4)) << std::setw(22) << Conversion::doubleToString(mat_aux.get(i,1),5) << std::setw(15) << Conversion::doubleToString(mat_aux.get(i,2),5) << std::setw(15) << Conversion::doubleToString(mat_aux.get(i,3),5) << "\n";

    mat_aux = calculateAvgStd(mat_aux);

    arq << "Average" << std::setw(16) << Conversion::doubleToString(mat_aux.get(1,1),5) << std::setw(15) << Conversion::doubleToString(mat_aux.get(1,2),5) << std::setw(15) << Conversion::doubleToString(mat_aux.get(1,3),5) << "\n";
    arq << "Std.dev." << std::setw(15) << Conversion::doubleToString(mat_aux.get(2,1),5) << std::setw(15) << Conversion::doubleToString(mat_aux.get(2,2),5) << std::setw(15) << Conversion::doubleToString(mat_aux.get(2,3),5) << "\n\n\n";


    arq << "The coordinates of object points:\n";
    arq << "Image ID " << std::setw(13) << "X " << std::setw(15) << "Y" << std::setw(15) << "Z" << std::setw(30) << "Measured in image(s)\n";

    for (unsigned int i=0; i<project->allPoints().size(); i++)
    {
        pnt = project->allPoints().at(i);
        arq << pnt->getId() << std::setw(25) << Conversion::doubleToString(pnt->getObjectCoordinate().getX(),5) << std::setw(15) << Conversion::doubleToString(pnt->getObjectCoordinate().getY(),5) << std::setw(15) << Conversion::doubleToString(pnt->getObjectCoordinate().getZ(),5) << std::setw(10);

        // Add images where points appears
        for (int j=0; j < pnt->countImages(); j++)
            arq << pnt->getImageAt(j)->getId() << " ";

        arq << "\n";
    }

    arq << "\n\n";

    arq << "The residuals of control points reprojected to the image (pixels):\n";
    arq << "Point ID " << std::setw(13) << "Image ID " << std::setw(10) << "Vx" << std::setw(15) << "Vy\n";

    mat_aux = calculatePixelerror();

    for (unsigned int i=1; i<=mat_aux.getRows(); i++)
        arq << int(mat_aux.get(i,3)) << std::setw(13) << int(mat_aux.get(i,4)) << std::setw(20) << Conversion::doubleToString(mat_aux.get(i,1),5) << std::setw(15) << Conversion::doubleToString(mat_aux.get(i,2),5) << "\n";

    mat_aux = calculateAvgStd(mat_aux);

    arq << "Average" << std::setw(27) << Conversion::doubleToString(mat_aux.get(1,1),5) << std::setw(15) << Conversion::doubleToString(mat_aux.get(1,2),5) << "\n";
    arq << "Std.dev." << std::setw(26) << Conversion::doubleToString(mat_aux.get(2,1),5) << std::setw(15) << Conversion::doubleToString(mat_aux.get(2,2),5) << "\n\n";

    arq << "End of report.";

    arq.close();

    return 1;
}

/***********************************************
 *  Errors calculation
 ***********************************************/
Matrix PhotoTriReport::calculateXYZerror()
{
    // Matrix layout
    // rX  rY  rZ  point_id

    double X, Y, Z, pX, pY, pZ;
    int no_gcp = 0;
    Matrix aux(project->allPoints().size(), 4);
    SpatialIntersection spc_inter;
    ObjectSpaceCoordinate object;
    StereoPair sp;
    Point *pnt;
    int count;

    for (unsigned int i=0; i<project->allPoints().size(); i++)
    {
        pnt = project->allPoints().at(i);

        if (pnt->getType() != Point::CONTROL)
            continue;

        // Only if point is stereo
        if (pnt->countImages() < 2)
            continue;

        // Add point id
        aux.set(no_gcp+1, 4, (double) pnt->getId());

        // Get GCP reference points
        X = pnt->getObjectCoordinate().getX();
        Y = pnt->getObjectCoordinate().getY();
        Z = pnt->getObjectCoordinate().getZ();

        // Reproject points - Calculate average X, Y, Z
        pX = 0.0; pY = 0.0; pZ = 0.0;
        count=0;
        for (int k=0; k<pnt->countImages(); k++)
        {
            for (int l=0; l<pnt->countImages(); l++)
            {
                if (k<l)
                {
                    sp.setLeftImage(pnt->getImageAt(k));
                    sp.setRightImage(pnt->getImageAt(l));
                    spc_inter.setStereoPair(&sp);
                    object = spc_inter.calculateIntersectionSubPixel(pnt->getImageCoordinateAt(k).getCol(), pnt->getImageCoordinateAt(k).getLin(), pnt->getImageCoordinateAt(l).getCol(), pnt->getImageCoordinateAt(l).getLin());

                    pX += object.getX();
                    pY += object.getY();
                    pZ += object.getZ();

                    count++;
                }
            }
        }

        pX /= (double) count;
        pY /= (double) count;
        pZ /= (double) count;

        // Calculate error
        aux.set(no_gcp+1, 1, X - pX);
        aux.set(no_gcp+1, 2, Y - pY);
        aux.set(no_gcp+1, 3, Z - pZ);

        no_gcp++;
    }

    aux = aux.resize(no_gcp, 4);

    return aux;
}

Matrix PhotoTriReport::calculatePixelerror()
{
    // Matrix layout
    // Vx  Vy  point_id  img_id

    double X, Y, Z;
    double x, y, px, py;
    Matrix aux(1,4);
    Image *img;
    Point *pnt;
    ProjectiveRay *pr;
    ImageSpaceCoordinate isc;
    int p=1;

    for (unsigned int i=0; i<project->allPoints().size(); i++)
    {
        pnt = project->allPoints().at(i);

        if (pnt->getType() != Point::CONTROL)
            continue;

        // Get GCP reference points
        X = pnt->getObjectCoordinate().getX();
        Y = pnt->getObjectCoordinate().getY();
        Z = pnt->getObjectCoordinate().getZ();

        // Get XYZ pixels in each image
        for (int j=0; j<pnt->countImages(); j++)
        {
            // Resize matrix
            aux.resize(p,4);

            img = pnt->getImageAt(j);
            pr = new ProjectiveRay(img);

            // Add point id and imgae id
            aux.set(p, 3, (double) pnt->getId());
            aux.set(p, 4, (double) img->getId());

            // Get x,y
            x = pnt->getImageCoordinate(img->getId()).getCol();
            y = pnt->getImageCoordinate(img->getId()).getLin();

            // Calculate projected x and y
            isc = pr->objectToImage(X, Y, Z, false);
            px = isc.getCol();
            py = isc.getLin();

            aux.set(p, 1, x - px);
            aux.set(p, 2, y - py);

            delete pr;

            p++;
        }
    }

    return aux;
}

/***********************************************
 *  Average and Standard deviation
 ***********************************************/
Matrix PhotoTriReport::calculateAvgStd(Matrix m)
{
    int rows = m.getRows();
    int cols = m.getCols();

    Matrix aux(2, cols);

    // Clear new matrix
    aux.zero();

    // Calculate Average
    for (int i=1; i<=rows; i++)
    {
       for  (int j=1; j<=cols; j++)
           aux.set(1, j, aux.get(1,j) + m.get(i,j));
    }

    for  (int j=1; j<=cols; j++)
        aux.set(1, j, aux.get(1,j) / double(rows));


    // Calculate Standard Deviation
    for (int i=1; i<=rows; i++)
    {
       for  (int j=1; j<=cols; j++)
           aux.set(2, j, aux.get(2,j) + pow(m.get(i,j) - aux.get(1,j), 2));
    }

    for  (int j=1; j<=cols; j++)
        aux.set(2, j, sqrt(aux.get(2,j) / double(rows)));

    return aux;

}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
