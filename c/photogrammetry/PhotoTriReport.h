/**************************************************************************
PhotoTriReport
**************************************************************************/

#ifndef PHOTOTRIREPORT_H
#define PHOTOTRIREPORT_H

#include "EObject.h"
#include "Matrix.h"
#include "EFotoManager.h"
#include "ProgressPublisher.h"
#include "SpatialIntersection.h"
#include "Project.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class EFotoManager;

/**
* \file PhotoTriReport.h
* \class PhotoTriReport
* \brief Photo-triangulation report generator
* \copyright E-Foto group
* \author Marcelo Teixeira Silveira - July 2013
*/

class PhotoTriReport
{
private:
    EFotoManager* manager;
    Project * project;
    Matrix calculateAvgStd(Matrix m);
    Matrix calculateXYZerror();
    Matrix calculatePixelerror();
    Matrix formatBundleX1();
    Matrix formatBundleX2();

public:
    PhotoTriReport (EFotoManager* newManager);
    int createReport(char *filename);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PHOTOTRIREPORT_H
