/**************************************************************************
														   OrthoManager.cpp
**************************************************************************/


#include "EDom.h"
#include "OrthoManager.h"
#include "EFotoManager.h"
#include "OrthoUserInterface.h"
#include "OrthoUserInterface_Qt.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {


// Constructors and destructors
//

OrthoManager::OrthoManager()
{
	started = false;
	status = false;
#ifdef INTEGRATED_EFOTO
    project = NULL;
#endif //INTEGRATED_EFOTO
}

OrthoManager::OrthoManager(EFotoManager* manager)
{
	this->manager = manager;
	started = false;
    status = false;
	grid = NULL;
	ortho = NULL;
	flag_cancel = false;
	show_image = false;
	inter_method = 0;
    project = manager->getProject();
}

OrthoManager::~OrthoManager()
{
	if (grid != NULL)
		delete grid;

	if (ortho != NULL)
        delete ortho;
}

// Association Methods
//

void OrthoManager::setInterface(OrthoUserInterface* newInterface)
{
	myInterface = newInterface;
}

OrthoUserInterface* OrthoManager::getInterface()
{
	return myInterface;
}

// Other Methods
//

bool OrthoManager::exec()
{
	if (manager != NULL)
	{
		if (manager->getInterfaceType().compare("Qt") == 0)
		{
			myInterface = OrthoUserInterface_Qt::instance(this);
		}
		started = true;
		if (myInterface != NULL)
		{
			myInterface->exec();
			addImagesToForm();
		}
	}

    if (project->allImages().size() < 1)
	{
		OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;
		oui->showErrorMessage("There is no image to run this application");
		returnProject();
    }


	return status;
}

void OrthoManager::addImagesToForm()
{
	OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;

    for (int i=1; i<=project->allImages().size(); i++)
        oui->comboBox->addItem("Image "+QString::fromStdString(project->allImages().at(i-1)->getImageId()));
}

int OrthoManager::loadDemGrid(char * filename, int fileType)
{
	if (grid != NULL)
		delete grid;

	// Create custom grid. Load will fix these values.
	grid = new DemGrid(1.0, 2.0, 1.0, 2.0, 1.0, 1.0);
	grid->loadDem(filename,fileType);

	// Report grid resolution
		double Xi, Yi, Xf, Yf, res_x, res_y, Zi, Zf, res_z;
	grid->getDemParameters(Xi, Yi, Xf, Yf, res_x, res_y);
		grid->getMinMax(Zi,Zf);
	OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;
	oui->doubleSpinBox1->setValue(res_x);
	oui->doubleSpinBox2->setValue(res_y);

	// Display results
	if (show_image)
	{
				res_z = (Zf-Zi)/255.0;
		Matrix * img = grid->getDemImage();
				oui->showImage3D(img,Xi , res_x, Yi, res_y, Zi, res_z, 1);
		delete img;
    }
	return 1;
}

void OrthoManager::loadOrtho(char *filename)
{
	if (ortho != NULL)
		delete ortho;

	// Create custom. Load will set correct values.
	ortho = new Orthorectification(1.0, 1.0, 2.0, 2.0, 1.0, 1.0);
	ortho->loadOrtho(filename,0);

	double Xi, Yi, Xf, Yf, res_x, res_y;
	ortho->getOrthoParametersA(Xi, Yi, Xf, Yf, res_x, res_y);

	// Display results
	if (show_image)
	{
			OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;
			Matrix * img = ortho->getOrthoImage();
			oui->showImage2D(img, Xi, res_x, Yi, res_y, false);
			delete img;
    }
}

void OrthoManager::runAllOrthoTogheter()
{
	// List of all centers
	vector <int> Cx;
	vector <int> Cy;
	vector <int> img_width;
	vector <int> img_height;
	vector <ProjectiveRay> pr;

	// Get image centers in pixels
	Image *img;
	int w,h;
	double best_dist_inital=0.0;
    for (int i=0; i<project->allImages().size(); i++)
	{
        img = project->allImages().at(i);
		w = img->getWidth();
		h = img->getHeight();
		img_width.push_back(w);
		img_height.push_back(h);
		Cx.push_back(w/2);
		Cy.push_back(h/2);
		if (w>best_dist_inital)
			best_dist_inital = double(w);
				pr.push_back(ProjectiveRay(img));
	}

	best_dist_inital *= best_dist_inital;

	//    left = listAllImages.at(image - 1);
	//    ProjectiveRay pr(left);

	//
		// Run ortho-image image by image
	//
	OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;
    string filename, strimg;
	Matrix img_m;

        for (int curr_image=0; curr_image<project->allImages().size(); curr_image++)
	{
		if (flag_cancel)
			return;

                img = project->allImages().at(curr_image);

		// Load image as matrix
				strimg = Conversion::intToString(curr_image + 1);
		oui->setCurrentWork("Loading image "+strimg);
		filename = img->getFilepath() + "/" + img->getFilename();
				oui->loadImage(img_m, (char *)filename.c_str(),1.0);

				//
				// Run Ortho
				//
				DetectorSpaceCoordinate analog_coord;
				Interpolation interp;
				interp.setMode(1); // Color
				double meanZ = grid->getMeanZ(), Z;
				double Xi, Yi, Xf, Yf, res_x, res_y, best_dist, dist, pixel;
				double lin, col, best_lin, best_col;
				int best_img;

				grid->getDemParameters(Xi, Yi, Xf, Yf, res_x, res_y);
				res_x = ortho->getGridResX();
				res_y = ortho->getGridResY();

				oui->setProgress(0);
				oui->setCurrentWork("Calculating ortho-rectification for image "+strimg);
				for (double Y=Yi; Y<Yf; Y+=res_y)
				{
						for (double X=Xi; X<Xf; X+=res_x)
						{
								if (flag_cancel)
										return;

								// Get X, Y, Z coordinates
								Z = grid->getHeightXY(X, Y);
								if (fabs(Z-0.0) < 0.00000000000000001)
										Z = meanZ;

								best_img = -1;
								best_dist = best_dist_inital;
                                for (int k=0; k<project->allImages().size(); k++)
								{
										analog_coord = pr.at(k).objectToDetector(X, Y, Z);
										pr.at(k).detectorToImage(analog_coord.getXi(), analog_coord.getEta(), col, lin);
										col += 1.0; // Matrix coordinate system
										lin += 1.0; // Matrix coordinate system

										if (lin < 1.0 || col < 1.0 || lin > img_height.at(k) || col > img_width.at(k))
												continue;

										// Calculate distance to the center
										dist = sqrt(pow(col - Cx.at(k),2) + pow(lin - Cy.at(k),2));

										if (dist < best_dist)
										{
												best_dist = dist;
												best_img = k;
												best_lin = lin;
												best_col = col;
										}
								}

								// Is best image = current image, copy pixel
								if (best_img == curr_image)
								{
										(best_img == -1) ? pixel = 0.0 : pixel = interp.interpolate(&img_m, best_col, best_lin, inter_method);
										ortho->setOrthoimagePixel(X, Y, pixel);
								}
						}
						oui->setProgress(int(100.0*((Y-Yi)/(Yf-Yi-1))));
				}
        }
}

void OrthoManager::runOrthoIndividual(int image)
{
    if (image < 1 || image > project->allImages().size())
		return;

	// Get image data
    Image *img = project->allImages().at(image-1);
	unsigned int img_width, img_height;
	img_width = img->getWidth();
	img_height = img->getHeight();

	// 3D coordinates
	DetectorSpaceCoordinate analog_coord;
	ProjectiveRay pr(img);

	//
	// Calculate image bounding box (check whole DEM where is inside image)
	//
	double meanZ = grid->getMeanZ(), Z;
	double Xi, Yi, Xf, Yf, res_x, res_y;
	double Xi_img, Yi_img, Xf_img, Yf_img;
	OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;
	double lin, col;

	grid->getDemParameters(Xi, Yi, Xf, Yf, res_x, res_y);
	Xi_img = Xf; Yi_img = Yf; Xf_img = Xi; Yf_img = Yi;
	oui->setProgress(0);
	string strimg = Conversion::intToString(image);
	oui->setCurrentWork("Calculating DEM bounding box for image "+strimg);

	for (double Y=Yi; Y<Yf; Y+=res_y)
	{
		for (double X=Xi; X<Xf; X+=res_x)
		{
			if (flag_cancel)
				return;

			Z = grid->getHeightXY(X, Y);

						if (fabs(Z-0.0) < 0.00000000000000001)
							Z = meanZ;

			analog_coord = pr.objectToDetector(X, Y, Z);
			pr.detectorToImage(analog_coord.getXi(), analog_coord.getEta(), col, lin);

			if (lin < 0.0 || col < 0.0 || lin >= img_height || col >= img_width)
				continue;

			if (X<Xi_img) Xi_img = X;
			if (X>Xf_img) Xf_img = X;
			if (Y<Yi_img) Yi_img = Y;
			if (Y>Yf_img) Yf_img = Y;
		}
		oui->setProgress(int(100.0*((Y-Yi)/(Yf-Yi-1))));
	}

	//
	// Load image as matrix
	//
	oui->setCurrentWork("Loading image "+strimg);
	string filename = img->getFilepath() + "/" + img->getFilename();
		Matrix img_matrix;
		oui->loadImage(img_matrix, (char *)filename.c_str(), 1.0);

	//
	// Run Ortho
	//
	res_x = ortho->getGridResX();
	res_y = ortho->getGridResY();
	ortho->createNewGrid(Xi_img, Yi_img, Xf_img, Yf_img, res_x, res_y);
	oui->setProgress(0);
	oui->setCurrentWork("Calculating ortho-rectification for image "+strimg);
	double pixel;
	Interpolation interp;
	interp.setMode(1); // Color

	for (double Y=Yi_img; Y<Yf_img; Y+=res_y)
	{
		for (double X=Xi_img; X<Xf_img; X+=res_x)
		{
			if (flag_cancel)
				return;

			Z = grid->getHeightXY(X, Y);

						if (fabs(Z-0.0) < 0.00000000000000001)
							Z = meanZ;

			analog_coord = pr.objectToDetector(X, Y, Z);
			pr.detectorToImage(analog_coord.getXi(), analog_coord.getEta(), col, lin);
			col += 1.0; // Matrix coordinate system
			lin += 1.0; // Matrix coordinate system

			if (lin < 1.0 || col < 1.0 || lin > img_height || col > img_width)
				continue;

						pixel = interp.interpolate(&img_matrix, col, lin, inter_method);

			ortho->setOrthoimagePixel(X, Y, pixel);
		}
		oui->setProgress(int(100.0*((Y-Yi_img)/(Yf_img-Yi_img-1))));
    }
}

int OrthoManager::orthoRectification(char * filename, int fileType, int option, double user_res_x, double user_res_y)
{
	// Create new orthoimage
	if (ortho != NULL)
		delete ortho;

	double Xi, Yi, Xf, Yf, res_x, res_y;
	grid->getDemParameters(Xi, Yi, Xf, Yf, res_x, res_y);
	ortho = new Orthorectification(Xi, Yi, Xf, Yf, user_res_x, user_res_y);
		ortho->setNumberOfBands(3);
	flag_cancel = false;

	if (option == 0)
	{
		runAllOrthoTogheter();

		if (flag_cancel)
		{
			flag_cancel = false;
			return -1;
		}

		ortho->saveOrtho(filename, 0);
	}

	if (option == 1)
	{
		string base_fname(filename);
		string ext(".ort");
		size_t expos = base_fname.find(ext);
		if (expos != -1)
			base_fname = base_fname.substr(0,expos);
		base_fname = base_fname + "_";
		string fname;

        for (int i=1; i<=project->allImages().size(); i++)
		{
			runOrthoIndividual(i);
			fname = base_fname + Conversion::intToString(i) + ext;

			if (flag_cancel)
			{
				flag_cancel = false;
				return -1;
			}

			ortho->saveOrtho((char *)fname.c_str(),0);
		}
	}

	if (option > 1)
	{
		runOrthoIndividual(option-1);

		if (flag_cancel)
		{
			flag_cancel = false;
			return -1;
		}

		ortho->saveOrtho(filename, 0);
	}

	// Display results
	if (show_image)
	{
		OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;
		Matrix * img = ortho->getOrthoImage();
				oui->showImage2D(img, Xi, res_x, Yi, res_y, false);
		delete img;
	}

    // Expanção do XML
    addOrthoToXML(string(filename));

	return 1;
}

void OrthoManager::returnProject()
{
	manager->reloadProject();
}

void OrthoManager::setProgress(int progress)
{
	OrthoUserInterface_Qt *oui = (OrthoUserInterface_Qt *)myInterface;
	oui->setProgress(progress);
}

//#include <QDebug>
void OrthoManager::addOrthoToXML(string filename)
{
    //Rever!
    /*
    stringstream add;
    add << "<eoiFilename>";
    add << filename;
    add << "</eoiFilename>";

    EDomElement newXml(manager->xmlGetData());

    if (newXml.elementByTagName("orthoImages").getContent() == "")
        newXml.addChildAtTagName("efotoPhotogrammetricProject","<orthoImages>\n</orthoImages>");
    newXml.addChildAtTagName("orthoImages", add.str());

    manager->xmlSetData(newXml.getContent());
    manager->setSavedState(false);
    */
}

void OrthoManager::addOrthoToXML2(string filename)
{
    //Fazer
    qDebug("Ortho2: %s",filename.c_str());
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
