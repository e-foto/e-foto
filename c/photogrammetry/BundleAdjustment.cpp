/*Copyright 2002-2023 e-foto team (UERJ)
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

#include "BundleAdjustment.h"

#include <QTime>
#include <iostream>

#include "ConvertionsSystems.h"
#include "Dms.h"
#include "Flight.h"
#include "Image.h"
#include "InteriorOrientation.h"
#include "Matrix.h"
#include "Point.h"
#include "RayTester.h"
#include "Sensor.h"

#define MAXRESIDUO 0.0001

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

BundleAdjustment::BundleAdjustment(std::deque<Image *> listSelectedImages,
                                   std::deque<Point *> listSelectedPoints) {
  listImages = listSelectedImages;
  listPoints = listSelectedPoints;
  numImages = listImages.size();
  numPoints = listPoints.size();
  numControlPoints = 0;
  numFotogrametricPoints = 0;

  zeroingCoordinatesPhotogrammetrics();

  for (int i = 0; i < numPoints; i++) {
    if (listPoints.at(i)->getType() == Point::CONTROL) {
      listControlPoints.push_back(listPoints.at(i));
      numControlPoints++;
    } else if (listPoints.at(i)->getType() == Point::PHOTOGRAMMETRIC) {
      listPhotogrammetricPoints.push_back(listPoints.at(i));
      numFotogrametricPoints++;
    }
  }

  fillDetectorCoordinates();
  numEquations = numberOfEquations();
  numUnknows = 6 * numImages + 3 * numFotogrametricPoints;
  c = listImages.at(0)->getSensor()->getFocalDistance();
  xsi0 = listImages.at(0)->getSensor()->getPrincipalPointCoordinates().getXi();
  eta0 = listImages.at(0)->getSensor()->getPrincipalPointCoordinates().getEta();

  userInitialValues = hasAllImagesInitialValues();
}

int BundleAdjustment::numberOfEquations() {
  int n = 0;
  for (int i = 0; i < numImages; i++) {
    n += numberControlPoints(listImages.at(i)) +
         numberPhotogrammetricPoints(listImages.at(i));
  }
  return 2 * n;
}

Point *BundleAdjustment::getPointFrom(int imageIndex, int pointIndex) {
  return listImages.at(imageIndex)->getPointAt(pointIndex);
}

void BundleAdjustment::fillDetectorCoordinates() {
  for (int i = 0; i < numImages; i++) {
    int pnts = listImages.at(i)->countPoints();
    for (int j = 0; j < pnts; j++) {
      auto imgId = listImages.at(i)->getId();
      auto col = getPointFrom(i, j)
                     ->getImageCoordinate(listImages.at(i)->getId())
                     .getCol();
      auto lin = getPointFrom(i, j)
                     ->getImageCoordinate(listImages.at(i)->getId())
                     .getLin();

      RayTester ray(listImages.at(i));
      ray.setIOParameters(listImages.at(i)->getIO()->getXa());
      DetectorSpaceCoordinate coordinates = ray.imageToDetector(col, lin);
      DetectorSpaceCoordinate aux(imgId);
      aux.setXi(coordinates.getXi());
      aux.setEta(coordinates.getEta());
      auto point = getPointFrom(i, j);
      if (point->hasDetectorCoordinate(imgId)) {
        point->deleteDetectorCoordinate(imgId);
      }
      point->putDetectorCoordinate(aux);
    }
  }
}

bool BundleAdjustment::calculate(bool makeReport) {
  if (!userInitialValues) {
    calculateInicialsValues();
    for (unsigned int i = 0; i < listImages.size(); i++) {
      Image *img = listImages.at(i);
      if (img->isInsAvailable() && img->getInsType() == "Initial") {
        matInitialValues.set(i + 1, 1, img->getInsOmega());
        matInitialValues.set(i + 1, 2, img->getInsPhi());
        matInitialValues.set(i + 1, 3, img->getInsKappa());
      }
      if (img->isGnssAvailable() && img->getGnssType() == "Initial") {
        matInitialValues.set(i + 1, 4, img->getGnssX0());
        matInitialValues.set(i + 1, 5, img->getGnssY0());
        matInitialValues.set(i + 1, 6, img->getGnssZ0());
      }
    }
  }

  matAdjust = matInitialValues;
  P.identity(numEquations);
  totalIterations = 0;

  if (numFotogrametricPoints != 0) {
    int conv = 0;

    createLb();

    while (totalIterations < maxIterations && conv != 1) {
      createA1();
      createA2();
      createL0();
      Matrix l = Lb - L0;
      n11 = getN11();
      n12 = getN12();
      n22 = getN22();
      n1 = getn1(l);
      n2 = getn2(l);

      setInverseN11(n11);
      setx2(n12, n22, n2, n1);
      setx1(n12, n1);
      conv = testConverged();
      if (conv == -2) {
        std::cout << "Infinite value appeared" << std::endl;
        break;
      }
      if (conv == -1) {
        std::cout << "NAN value appeared" << std::endl;
        break;
      }
      updateMatAdjust();
      updateCoordFotog();
      totalIterations++;

      if (makeReport) {
        calculateResiduos();
        listRMSE.push_back(calculateRMSE());
      }
    }

    calculateResiduos();
  } else {
    totalIterations = 0;
    int conv = 0;
    createLb();
    while (totalIterations < maxIterations && conv != 1) {
      createA1();
      createL0();
      Matrix l = Lb - L0;
      Matrix n11 = getN11();
      Matrix n1 = getn1(l);
      setInverseN11(n11);
      x1 = Matrix(inverseN11) * n1;
      conv = testConverged();
      if (conv == -2) {
        std::cout << "Infinite value appeared" << std::endl;
        break;
      }
      if (conv == -1) {
        std::cout << "NAN value appeared" << std::endl;
        break;
      }
      totalIterations++;
      updateMatAdjust();
    }
    calculateResiduos();
  }
  calculateResiduos();
  setAFP();
  return true;
}

/* calculation methods*/
Matrix BundleAdjustment::getM11(Matrix M1) {
  return Matrix(M1.transpose()) * M1;
}

Matrix BundleAdjustment::getM12(Matrix M1, Matrix M2) {
  return Matrix(M1.transpose()) * M2;
}

Matrix BundleAdjustment::getM22(Matrix M2) {
  return Matrix(M2.transpose()) * M2;
}

Matrix BundleAdjustment::getm1(Matrix M1, Matrix L1) {
  return Matrix(M1.transpose()) * L1;
}

Matrix BundleAdjustment::getm2(Matrix M2, Matrix L1) {
  return Matrix(M2.transpose()) * L1;
}

Matrix BundleAdjustment::getPAf(Matrix M12, Matrix m1, Matrix xypf) {
  Matrix temp1 = Matrix(inverseM11);
  return temp1 * m1 - Matrix(temp1 * M12) * xypf;
}

// Initial approximation to the planimetric coordinates (XY) of the
// photogrammetric points
Matrix BundleAdjustment::getXYpf(Matrix M12, Matrix M22, Matrix m1, Matrix m2) {
  Matrix temp1 = Matrix(Matrix(M12.transpose()) * inverseM11);
  return Matrix((M22 - temp1 * M12).inverse()) * (m2 - temp1 * m1);
}

Matrix BundleAdjustment::getN11() {
  return Matrix(Matrix(A1.transpose()) * P) * A1;
}

Matrix BundleAdjustment::getN12() {
  return Matrix(Matrix(A1.transpose()) * P) * A2;
}

Matrix BundleAdjustment::getN22() { return Matrix(A2.transpose()) * A2; }

Matrix BundleAdjustment::getn1(Matrix L) {
  return Matrix(Matrix(A1.transpose()) * P) * L;
}

Matrix BundleAdjustment::getn2(Matrix L) { return Matrix(A2.transpose()) * L; }

void BundleAdjustment::setx1(Matrix N12, Matrix n1) {
  Matrix temp1 = Matrix(inverseN11);
  x1 = temp1 * n1 - Matrix(temp1 * N12) * x2;
}

void BundleAdjustment::setx2(Matrix N12, Matrix N22, Matrix n2, Matrix n1) {
  Matrix temp1 = Matrix(Matrix(N12.transpose()) * inverseN11);
  x2 = Matrix((N22 - temp1 * N12).osuInverse()) * (n2 - temp1 * n1);
}

void BundleAdjustment::setInverseN11(Matrix n11) {
  int rows = n11.getRows();
  inverseN11 = n11;
  for (int i = 1; i < rows; i += 6) {
    Matrix unit = n11.sel(i, i + 5, i, i + 5);
    inverseN11.putMatrix(unit.inverse(), i, i);
  }
}

void BundleAdjustment::setInverseN22(Matrix n22) {
  int rows = n22.getRows();
  inverseN22 = n22;
  for (int i = 1; i < rows; i += 3) {
    Matrix unit = n22.sel(i, i + 2, i, i + 2);
    inverseN22.putMatrix(unit.inverse(), i, i);
  }
}

void BundleAdjustment::setInverseM11(Matrix m11) {
  int rows = m11.getRows();
  inverseM11 = m11;
  for (int i = 1; i < rows; i += 3) {
    Matrix unit = m11.sel(i, i + 2, i, i + 2);
    inverseM11.putMatrix(unit.inverse(), i, i);
  }
}

void BundleAdjustment::setRot(double omega, double phi, double kappa) {
  r11 = cos(phi) * cos(kappa);
  r12 = -cos(phi) * sin(kappa);
  r13 = sin(phi);
  r21 = cos(omega) * sin(kappa) + sin(omega) * sin(phi) * cos(kappa);
  r22 = cos(omega) * cos(kappa) - sin(omega) * sin(phi) * sin(kappa);
  r23 = -sin(omega) * cos(phi);
  r31 = sin(omega) * sin(kappa) - cos(omega) * sin(phi) * cos(kappa);
  r32 = sin(omega) * cos(kappa) + cos(omega) * sin(phi) * sin(kappa);
  r33 = cos(omega) * cos(phi);
}

Matrix BundleAdjustment::getCoordinatesEqColin(double X, double Y, double Z,
                                               int imageId) {
  Matrix coord(1, 2);
  double X0 = getXAdjus(imageId);
  double Y0 = getYAdjus(imageId);
  double Z0 = getZAdjus(imageId);

  setRot(getOmegaAdjus(imageId), getPhiAdjus(imageId), getKappaAdjus(imageId));

  double xi = xsi0 - c * (r11 * (X - X0) + r21 * (Y - Y0) + r31 * (Z - Z0)) /
                         (r13 * (X - X0) + r23 * (Y - Y0) + r33 * (Z - Z0));
  double eta = eta0 - c * (r12 * (X - X0) + r22 * (Y - Y0) + r32 * (Z - Z0)) /
                          (r13 * (X - X0) + r23 * (Y - Y0) + r33 * (Z - Z0));

  coord.set(1, 1, xi);
  coord.set(1, 2, eta);

  return coord;
}

Matrix BundleAdjustment::getJacobianaControl(double X, double Y, double Z,
                                             int imageIndex) {
  Matrix JacCtrl(2, 6);

  double omega = matAdjust.get(imageIndex, 1);
  double phi = matAdjust.get(imageIndex, 2);
  double kappa = matAdjust.get(imageIndex, 3);
  double X0 = matAdjust.get(imageIndex, 4);
  double Y0 = matAdjust.get(imageIndex, 5);
  double Z0 = matAdjust.get(imageIndex, 6);

  JacCtrl.set(
      1, 1,
      c * cos(kappa) * cos(phi) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) -
          c *
              (cos(kappa) * cos(phi) * (X - X0) +
               (sin(kappa) * cos(omega) + cos(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (sin(kappa) * sin(omega) - cos(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow((sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                   cos(phi) * cos(omega) * (Z - Z0)),
                  2) *
              sin(phi));
  JacCtrl.set(
      1, 2,
      -c * (-sin(kappa) * cos(omega) - cos(kappa) * sin(phi) * sin(omega)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) +
          c *
              (cos(kappa) * cos(phi) * (X - X0) +
               (sin(kappa) * cos(omega) + cos(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (sin(kappa) * sin(omega) - cos(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow((sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                   cos(phi) * cos(omega) * (Z - Z0)),
                  2) *
              cos(phi) * sin(omega));
  JacCtrl.set(
      1, 3,
      -c * (-sin(kappa) * sin(omega) + cos(kappa) * sin(phi) * cos(omega)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) -
          c *
              (cos(kappa) * cos(phi) * (X - X0) +
               (sin(kappa) * cos(omega) + cos(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (sin(kappa) * sin(omega) - cos(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow((sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                   cos(phi) * cos(omega) * (Z - Z0)),
                  2) *
              cos(phi) * cos(omega));
  JacCtrl.set(
      1, 4,
      -c *
              ((-sin(kappa) * sin(omega) + cos(kappa) * sin(phi) * cos(omega)) *
                   (Y - Y0) +
               (sin(kappa) * cos(omega) + cos(kappa) * sin(phi) * sin(omega)) *
                   (Z - Z0)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) +
          c *
              (cos(kappa) * cos(phi) * (X - X0) +
               (sin(kappa) * cos(omega) + cos(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (sin(kappa) * sin(omega) - cos(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow((sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                   cos(phi) * cos(omega) * (Z - Z0)),
                  2) *
              (-cos(phi) * cos(omega) * (Y - Y0) -
               cos(phi) * sin(omega) * (Z - Z0)));
  JacCtrl.set(
      1, 5,
      -c *
              (-cos(kappa) * sin(phi) * (X - X0) +
               cos(kappa) * cos(phi) * sin(omega) * (Y - Y0) -
               cos(kappa) * cos(phi) * cos(omega) * (Z - Z0)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) +
          c *
              (cos(kappa) * cos(phi) * (X - X0) +
               (sin(kappa) * cos(omega) + cos(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (sin(kappa) * sin(omega) - cos(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow((sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                   cos(phi) * cos(omega) * (Z - Z0)),
                  2) *
              (cos(phi) * (X - X0) + sin(phi) * sin(omega) * (Y - Y0) -
               sin(phi) * cos(omega) * (Z - Z0)));
  JacCtrl.set(
      1, 6,
      -c *
          (-sin(kappa) * cos(phi) * (X - X0) +
           (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) *
               (Y - Y0) +
           (cos(kappa) * sin(omega) + sin(kappa) * sin(phi) * cos(omega)) *
               (Z - Z0)) /
          (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
           cos(phi) * cos(omega) * (Z - Z0)));

  JacCtrl.set(
      2, 1,
      -c * sin(kappa) * cos(phi) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) -
          c *
              (-sin(kappa) * cos(phi) * (X - X0) +
               (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (cos(kappa) * sin(omega) + sin(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow((sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                   cos(phi) * cos(omega) * (Z - Z0)),
                  2) *
              sin(phi));
  JacCtrl.set(
      2, 2,
      -c * (-cos(kappa) * cos(omega) + sin(kappa) * sin(phi) * sin(omega)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) +
          c *
              (-sin(kappa) * cos(phi) * (X - X0) +
               (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (cos(kappa) * sin(omega) + sin(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow((sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                   cos(phi) * cos(omega) * (Z - Z0)),
                  2) *
              cos(phi) * sin(omega));
  JacCtrl.set(
      2, 3,
      -c * (-cos(kappa) * sin(omega) - sin(kappa) * sin(phi) * cos(omega)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) -
          c *
              (-sin(kappa) * cos(phi) * (X - X0) +
               (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (cos(kappa) * sin(omega) + sin(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow((sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                   cos(phi) * cos(omega) * (Z - Z0)),
                  2) *
              cos(phi) * cos(omega));
  JacCtrl.set(
      2, 4,
      -c *
              ((-cos(kappa) * sin(omega) - sin(kappa) * sin(phi) * cos(omega)) *
                   (Y - Y0) +
               (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) *
                   (Z - Z0)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) +
          c *
              (-sin(kappa) * cos(phi) * (X - X0) +
               (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (cos(kappa) * sin(omega) + sin(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow((sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                   cos(phi) * cos(omega) * (Z - Z0)),
                  2) *
              (-cos(phi) * cos(omega) * (Y - Y0) -
               cos(phi) * sin(omega) * (Z - Z0)));
  JacCtrl.set(
      2, 5,
      -c *
              (sin(kappa) * sin(phi) * (X - X0) -
               sin(kappa) * cos(phi) * sin(omega) * (Y - Y0) +
               sin(kappa) * cos(phi) * cos(omega) * (Z - Z0)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) +
          c *
              (-sin(kappa) * cos(phi) * (X - X0) +
               (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (cos(kappa) * sin(omega) + sin(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow((sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                   cos(phi) * cos(omega) * (Z - Z0)),
                  2) *
              (cos(phi) * (X - X0) + sin(phi) * sin(omega) * (Y - Y0) -
               sin(phi) * cos(omega) * (Z - Z0)));
  JacCtrl.set(
      2, 6,
      -c *
          (-cos(kappa) * cos(phi) * (X - X0) +
           (-sin(kappa) * cos(omega) - cos(kappa) * sin(phi) * sin(omega)) *
               (Y - Y0) +
           (-sin(kappa) * sin(omega) + cos(kappa) * sin(phi) * cos(omega)) *
               (Z - Z0)) /
          (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
           cos(phi) * cos(omega) * (Z - Z0)));

  return JacCtrl;
}

Matrix BundleAdjustment::getJacobianaFotogrametric(double X, double Y, double Z,
                                                   int imageIndex) {
  Matrix JacFoto(2, 3);

  double omega = matAdjust.get(imageIndex, 1);
  double phi = matAdjust.get(imageIndex, 2);
  double kappa = matAdjust.get(imageIndex, 3);
  double X0 = matAdjust.get(imageIndex, 4);
  double Y0 = matAdjust.get(imageIndex, 5);
  double Z0 = matAdjust.get(imageIndex, 6);

  JacFoto.set(
      1, 1,
      -c * cos(kappa) * cos(phi) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) +
          c *
              (cos(kappa) * cos(phi) * (X - X0) +
               (sin(kappa) * cos(omega) + cos(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (sin(kappa) * sin(omega) - cos(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow(sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                      cos(phi) * cos(omega) * (Z - Z0),
                  2) *
              sin(phi));
  JacFoto.set(
      1, 2,
      -c * (sin(kappa) * cos(omega) + cos(kappa) * sin(phi) * sin(omega)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) -
          c *
              (cos(kappa) * cos(phi) * (X - X0) +
               (sin(kappa) * cos(omega) + cos(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (sin(kappa) * sin(omega) - cos(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow(sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                      cos(phi) * cos(omega) * (Z - Z0),
                  2) *
              cos(phi) * sin(omega));
  JacFoto.set(
      1, 3,
      -c * (sin(kappa) * sin(omega) - cos(kappa) * sin(phi) * cos(omega)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) +
          c *
              (cos(kappa) * cos(phi) * (X - X0) +
               (sin(kappa) * cos(omega) + cos(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (sin(kappa) * sin(omega) - cos(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow(sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                      cos(phi) * cos(omega) * (Z - Z0),
                  2) *
              cos(phi) * cos(omega));

  JacFoto.set(
      2, 1,
      c * sin(kappa) * cos(phi) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) +
          c *
              (-sin(kappa) * cos(phi) * (X - X0) +
               (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (cos(kappa) * sin(omega) + sin(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow(sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                      cos(phi) * cos(omega) * (Z - Z0),
                  2) *
              sin(phi));
  JacFoto.set(
      2, 2,
      -c * (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) -
          c *
              (-sin(kappa) * cos(phi) * (X - X0) +
               (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (cos(kappa) * sin(omega) + sin(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow(sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                      cos(phi) * cos(omega) * (Z - Z0),
                  2) *
              cos(phi) * sin(omega));
  JacFoto.set(
      2, 3,
      -c * (cos(kappa) * sin(omega) + sin(kappa) * sin(phi) * cos(omega)) /
              (sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
               cos(phi) * cos(omega) * (Z - Z0)) +
          c *
              (-sin(kappa) * cos(phi) * (X - X0) +
               (cos(kappa) * cos(omega) - sin(kappa) * sin(phi) * sin(omega)) *
                   (Y - Y0) +
               (cos(kappa) * sin(omega) + sin(kappa) * sin(phi) * cos(omega)) *
                   (Z - Z0)) /
              pow(sin(phi) * (X - X0) - cos(phi) * sin(omega) * (Y - Y0) +
                      cos(phi) * cos(omega) * (Z - Z0),
                  2) *
              cos(phi) * cos(omega));

  return JacFoto;
}

// returns image parameters
Matrix BundleAdjustment::getPTA(Matrix PAf, int imageId) {
  Matrix result(6, 1);
  for (int i = 6 * imageId + 1, j = 1; i <= 6 * (imageId + 1); i++, j++) {
    result.set(j, 1, PAf.get(i, 1));
  }
  return result;
}

// auxiliary methods for fetching data
double BundleAdjustment::getdOmegax1(int imageId) {
  return x1.get(6 * (imageId - 1) + 4, 1);
}

double BundleAdjustment::getdPhix1(int imageId) {
  return x1.get(6 * (imageId - 1) + 5, 1);
}

double BundleAdjustment::getdKappax1(int imageId) {
  return x1.get(6 * (imageId - 1) + 6, 1);
}

double BundleAdjustment::getdXx1(int imageId) {
  return x1.get(6 * (imageId - 1) + 1, 1);
}

double BundleAdjustment::getdYx1(int imageId) {
  return x1.get(6 * (imageId - 1) + 2, 1);
}

double BundleAdjustment::getdZx1(int imageId) {
  return x1.get(6 * (imageId - 1) + 3, 1);
}

double BundleAdjustment::getdXx2(int fotogrPointId) {
  return x2.get(3 * (fotogrPointId - 1) + 1, 1);
}

double BundleAdjustment::getdYx2(int fotogrPointId) {
  return x2.get(3 * (fotogrPointId - 1) + 2, 1);
}

double BundleAdjustment::getdZx2(int fotogrPointId) {
  return x2.get(3 * (fotogrPointId - 1) + 3, 1);
}

double BundleAdjustment::getOmegaAdjus(int imageId) {
  return matAdjust.get(imageId, 1);
}

double BundleAdjustment::getPhiAdjus(int imageId) {
  return matAdjust.get(imageId, 2);
}

double BundleAdjustment::getKappaAdjus(int imageId) {
  return matAdjust.get(imageId, 3);
}

double BundleAdjustment::getXAdjus(int imageId) {
  return matAdjust.get(imageId, 4);
}

double BundleAdjustment::getYAdjus(int imageId) {
  return matAdjust.get(imageId, 5);
}

double BundleAdjustment::getZAdjus(int imageId) {
  return matAdjust.get(imageId, 6);
}

// Finds where the point is in the list of all points
// (ie in the list of selected points) and returns its index in the list
int BundleAdjustment::whereInPoints(Point *pnt) {
  for (int i = 0; i < numPoints; i++) {
    if (pnt == listPoints.at(i)) {
      return i;
    }
  }
  return -1;  // if not found returns -1
}

int BundleAdjustment::whereInPhotogrammetricPoints(Point *pnt) {
  for (int i = 0; i < numFotogrametricPoints; i++) {
    if (pnt == listPhotogrammetricPoints.at(i)) {
      return i;
    }
  }
  return -1;  // if not found returns -1
}

void BundleAdjustment::updateMatAdjust() {
  for (int i = 1; i <= numImages; i++) {
    matAdjust.set(i, 1,
                  getOmegaAdjus(i) + getdOmegax1(i));    // adjusting omega
    matAdjust.set(i, 2, getPhiAdjus(i) + getdPhix1(i));  // adjusting phi
    matAdjust.set(i, 3,
                  getKappaAdjus(i) + getdKappax1(i));  // adjusting kappa
    matAdjust.set(i, 4, getXAdjus(i) + getdXx1(i));    // adjusting X
    matAdjust.set(i, 5, getYAdjus(i) + getdYx1(i));    // adjusting Y
    matAdjust.set(i, 6, getZAdjus(i) + getdZx1(i));    // adjusting Z
  }
}

Matrix BundleAdjustment::getCoordColinearTerrain(double xsi, double eta,
                                                 double z, int imageId) {
  Matrix result(1, 2);
  setRot(getOmegaAdjus(imageId), getPhiAdjus(imageId), getKappaAdjus(imageId));

  double x = getXAdjus(imageId) +
             (z - getZAdjus(imageId)) *
                 (r11 * (xsi - xsi0) + r12 * (eta - eta0) - r13 * c) /
                 (r31 * (xsi - xsi0) + r32 * (eta - eta0) - r33 * c);
  double y = getYAdjus(imageId) +
             (z - getZAdjus(imageId)) *
                 (r21 * (xsi - xsi0) + r22 * (eta - eta0) - r23 * c) /
                 (r31 * (xsi - xsi0) + r32 * (eta - eta0) - r33 * c);

  result.set(1, 1, x);
  result.set(1, 2, y);
  return result;
}

// omega, phi, kappa, X0, Y0, Z0)
void BundleAdjustment::calculateInicialsValues() {
  if (numFotogrametricPoints != 0) {
    Matrix L = createL();
    Matrix M1 = createM1();
    Matrix M2 = createM2();
    Matrix m11 = getM11(M1);
    setInverseM11(m11);

    Matrix m12 = getM12(M1, M2);
    Matrix m22 = getM22(M2);
    Matrix m1 = getm1(M1, L);
    Matrix m2 = getm2(M2, L);
    Matrix xypf = getXYpf(m12, m22, m1, m2);

    Matrix paf = getPAf(m12, m1, xypf);
    updateCoordinatesAllPoints(xypf, getInicialZPhotogrammetricPoints());

    Matrix temp(numImages, 6);
    matInitialValues = temp;
    for (int i = 0; i < numImages; i++) {
      xsi0 =
          listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getXi();
      eta0 = listImages.at(i)
                 ->getSensor()
                 ->getPrincipalPointCoordinates()
                 .getEta();
      c = listImages.at(i)->getSensor()->getFocalDistance();

      Matrix pta = getPTA(paf, i);

      matInitialValues.set(i + 1, 1, 0);
      matInitialValues.set(i + 1, 2, 0);
      matInitialValues.set(i + 1, 3, listImages.at(i)->getFlightDirection());
      matInitialValues.set(
          i + 1, 4,
          pta.get(1, 1) + pta.get(2, 1) * xsi0 + pta.get(3, 1) * eta0);
      matInitialValues.set(
          i + 1, 5,
          pta.get(4, 1) + pta.get(5, 1) * xsi0 + pta.get(6, 1) * eta0);
      matInitialValues.set(
          i + 1, 6, c * listImages.at(i)->getFlight()->getScaleDen() / 1000);
    }
  } else {
    Matrix L = createL();
    Matrix M1 = createM1();
    Matrix m11 = getM11(M1);
    setInverseM11(m11);
    Matrix m1 = getm1(M1, L);
    Matrix paf = inverseM11 * m1;

    Matrix temp(numImages, 6);
    matInitialValues = temp;
    for (int i = 0; i < numImages; i++) {
      xsi0 =
          listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getXi();
      eta0 = listImages.at(i)
                 ->getSensor()
                 ->getPrincipalPointCoordinates()
                 .getEta();
      c = listImages.at(i)->getSensor()->getFocalDistance();
      Matrix pta = getPTA(paf, i);
      matInitialValues.set(i + 1, 1, 0);
      matInitialValues.set(i + 1, 2, 0);
      matInitialValues.set(i + 1, 3, listImages.at(i)->getFlightDirection());
      matInitialValues.set(
          i + 1, 4,
          pta.get(1, 1) + pta.get(2, 1) * xsi0 + pta.get(3, 1) * eta0);
      matInitialValues.set(
          i + 1, 5,
          pta.get(4, 1) + pta.get(5, 1) * xsi0 + pta.get(6, 1) * eta0);
      matInitialValues.set(
          i + 1, 6, c * listImages.at(i)->getFlight()->getScaleDen() / 1000);
    }
  }
}

Matrix BundleAdjustment::createL() {
  Matrix L(0, 1);
  for (int i = 0; i < numImages; i++) {
    int numpnts = listImages.at(i)->countPoints();
    for (int j = 0; j < numpnts; j++) {
      if (whereInPoints(getPointFrom(i, j)) != -1) {
        Matrix temp(2, 1);
        if (isControlPoint(i, j)) {
          temp.set(
              1, 1,
              listImages.at(i)->getPointAt(j)->getObjectCoordinate().getX());
          temp.set(
              2, 1,
              listImages.at(i)->getPointAt(j)->getObjectCoordinate().getY());
        }
        L = L | temp;
      }
    }
  }
  return L;
}

Matrix BundleAdjustment::createM1() {
  Matrix result(0, 0);
  for (int i = 0; i < numImages; i++) {
    Matrix temp = imageToMatrixDetectorCoordenates(listImages.at(i));
    result.putMatrix(temp, result.getRows() + 1, 6 * i + 1);
  }
  return result;
}

Matrix BundleAdjustment::createM2() {
  Matrix result(0, 2 * numFotogrametricPoints);
  Matrix B(2, 2);
  B.set(1, 1, -1);
  B.set(2, 2, -1);
  int posCol = 0;
  for (int i = 0; i < numImages; i++) {
    int pnts = listImages.at(i)->countPoints();
    Matrix oneImage(0, 2 * numFotogrametricPoints);
    for (int j = 0; j < pnts; j++) {
      if (whereInPoints(getPointFrom(i, j)) != -1) {
        posCol = whereInPhotogrammetricPoints(getPointFrom(i, j));
        Matrix auxPoint(2, 2 * numFotogrametricPoints);
        if (posCol != -1) {
          posCol = 2 * posCol + 1;
          auxPoint.putMatrix(B, 1, posCol);
        }
        oneImage = oneImage | auxPoint;
      }
    }
    result = result | oneImage;
  }
  return result;
}

// Returns an Matrix corresponding to an image in Matrix M
Matrix BundleAdjustment::imageToMatrixDetectorCoordenates(Image *img) {
  int numpnts = img->countPoints();

  Matrix result(0, 6);
  for (int i = 0; i < numpnts; i++) {
    // configuration of each temp matrix
    // |1	eta xi	0	 0		0 |
    // |0	 0	0	1	eta 	xi|

    if (whereInPoints(img->getPointAt(i)) != -1 &&
        img->getPointAt(i)->getType() != Point::CHECKING) {
      Matrix temp(2, 6);
      double lin =
          img->getPointAt(i)->getImageCoordinate(img->getId()).getLin();
      double col =
          img->getPointAt(i)->getImageCoordinate(img->getId()).getCol();

      RayTester ray(img);
      ray.setIOParameters(img->getIO()->getXa());
      DetectorSpaceCoordinate coordinates = ray.imageToDetector(col, lin);

      temp.set(1, 1, 1);
      temp.set(1, 2, coordinates.getEta());
      temp.set(1, 3, coordinates.getXi());
      temp.set(2, 4, 1);
      temp.set(2, 5, coordinates.getEta());
      temp.set(2, 6, coordinates.getXi());

      result = result | temp;
    }
  }
  return result;
}

Matrix BundleAdjustment::digitalToAnalog(InteriorOrientation *oi, double linha,
                                         double coluna) {
  double a0, a1, a2, b0, b1, b2;

  a0 = oi->getXa().get(1, 1);
  a1 = oi->getXa().get(2, 1);
  a2 = oi->getXa().get(3, 1);
  b0 = oi->getXa().get(4, 1);
  b1 = oi->getXa().get(5, 1);
  b2 = oi->getXa().get(6, 1);

  Matrix result(1, 2);
  result.set(1, 1, a0 + a1 * coluna + a2 * linha);
  result.set(1, 2, b0 + b1 * coluna + b2 * linha);
  return result;
}

Matrix BundleAdjustment::analogToDigital(InteriorOrientation *oi, double xsi,
                                         double eta) {
  double a0, a1, a2, b0, b1, b2;
  a0 = oi->getXa().get(1, 1);
  a1 = oi->getXa().get(2, 1);
  a2 = oi->getXa().get(3, 1);
  b0 = oi->getXa().get(4, 1);
  b1 = oi->getXa().get(5, 1);
  b2 = oi->getXa().get(6, 1);
  // Hack to work around a cast error (add 0.1)
  Matrix result(1, 2);
  result.set(
      1, 1,
      ((b2 * xsi - b2 * a0 - a2 * eta + b0 * a2) / (a1 * b2 - b1 * a2) + 0.1));
  result.set(
      1, 2,
      ((a1 * eta - a1 * b0 - b1 * xsi + b1 * a0) / (a1 * b2 - b1 * a2) + 0.1));
  return result;
}

int BundleAdjustment::numberControlPoints(Image *img) {
  int cont = 0;
  int points = img->countPoints();
  for (int i = 0; i < points; i++) {
    if (img->getPointAt(i)->getType() == Point::CONTROL &&
        whereInPoints(img->getPointAt(i)) != -1) {
      cont++;
    }
  }
  return cont;
}

int BundleAdjustment::numberPhotogrammetricPoints(Image *img) {
  int cont = 0;
  int points = img->countPoints();
  for (int i = 0; i < points; i++)
    if (img->getPointAt(i)->getType() == Point::PHOTOGRAMMETRIC &&
        whereInPoints(img->getPointAt(i)) != -1) {
      cont++;
    }
  return cont;
}

double BundleAdjustment::getInicialZPhotogrammetricPoints() {
  double z = 0;
  int n = 0;
  for (int i = 0; i < numImages; i++) {
    int pnts = listImages.at(i)->countPoints();
    for (int j = 0; j < pnts; j++) {
      if (isControlPoint(i, j) && whereInPoints(getPointFrom(i, j)) != -1) {
        z += getPointFrom(i, j)->getObjectCoordinate().getZ();
        n++;
      }
    }
  }
  return z /= n;
}
// updates the coordinates of all points
void BundleAdjustment::updateCoordinatesAllPoints(Matrix xypf,
                                                  double zphotogrammetric) {
  for (int i = 0; i < numImages; i++) {
    int pnts = listImages.at(i)->countPoints();
    int n = 1;
    for (int j = 0; j < pnts; j++) {
      if (isPhotogrammetricPoint(i, j) &&
          whereInPoints(getPointFrom(i, j)) != -1) {
        ObjectSpaceCoordinate coord = getPointFrom(i, j)->getObjectCoordinate();
        coord.setX(xypf.get(n++, 1));
        coord.setY(xypf.get(n++, 1));
        coord.setZ(zphotogrammetric);
        getPointFrom(i, j)->setObjectCoordinate(coord);
      }
    }
  }
}
void BundleAdjustment::zeroingCoordinatesPhotogrammetrics() {
  for (unsigned int i = 0; i < listPhotogrammetricPoints.size(); i++) {
    ObjectSpaceCoordinate coord =
        listPhotogrammetricPoints.at(i)->getObjectCoordinate();
    coord.setX(0);
    coord.setY(0);
    coord.setZ(0);
    listPhotogrammetricPoints.at(i)->setObjectCoordinate(coord);
  }
}

// Returns an matrix corresponding to an image in Matrix A
Matrix BundleAdjustment::imageToMatrixJacobiana(int indexImage) {
  Image *img = listImages.at(indexImage);
  int pnts = img->countPoints();
  Matrix result(0, 6);
  for (int i = 0; i < pnts; i++) {
    if (whereInPoints(img->getPointAt(i)) != -1 &&
        !isCheckingPoint(indexImage, i)) {
      ObjectSpaceCoordinate aux = img->getPointAt(i)->getObjectCoordinate();
      double x = aux.getX();
      double y = aux.getY();
      double z = aux.getZ();
      result = result | getJacobianaControl(x, y, z, indexImage + 1);
    }
  }
  return result;
}

// Returns the entire matrix A1
void BundleAdjustment::createA1() {
  Matrix result;
  int currentRows = 0;
  for (int i = 0; i < numImages; i++) {
    Matrix partial = imageToMatrixJacobiana(i);
    result.putMatrix(partial, currentRows + 1, 6 * i + 1);
    currentRows += partial.getRows();
  }
  A1 = result;
}

void BundleAdjustment::createA2() {
  Matrix result(0, 3 * numFotogrametricPoints);
  int posCol = 0;
  for (int i = 0; i < numImages; i++) {
    int pnts = listImages.at(i)->countPoints();
    Matrix oneImage(0, 3 * numFotogrametricPoints);

    for (int j = 0; j < pnts; j++) {
      if (whereInPoints(getPointFrom(i, j)) != -1) {
        posCol = whereInPhotogrammetricPoints(getPointFrom(i, j));
        Matrix auxPoint(2, 3 * numFotogrametricPoints);
        if (posCol != -1) {
          ObjectSpaceCoordinate aux = getPointFrom(i, j)->getObjectCoordinate();
          double x = aux.getX();
          double y = aux.getY();
          double z = aux.getZ();
          Matrix jf = getJacobianaFotogrametric(x, y, z, i + 1);
          posCol = 3 * posCol + 1;
          auxPoint.putMatrix(jf, 1, posCol);
        }
        oneImage = oneImage | auxPoint;
      }
    }
    result = result | oneImage;
  }
  A2 = result;
}

void BundleAdjustment::createL0() {
  Matrix result(0, 1);
  for (int i = 0; i < numImages; i++) {
    int pnts = listImages.at(i)->countPoints();
    for (int j = 0; j < pnts; j++) {
      if (whereInPoints(getPointFrom(i, j)) != -1 && !isCheckingPoint(i, j)) {
        ObjectSpaceCoordinate aux = getPointFrom(i, j)->getObjectCoordinate();
        double x = aux.getX();
        double y = aux.getY();
        double z = aux.getZ();
        Matrix coord = getCoordinatesEqColin(x, y, z, i + 1);
        result = result | (coord.transpose());
      }
    }
  }
  L0 = result;
}

void BundleAdjustment::createLb() {
  Matrix result(0, 1);
  for (int i = 0; i < numImages; i++) {
    int pnts = listImages.at(i)->countPoints();
    for (int j = 0; j < pnts; j++) {
      if (whereInPoints(getPointFrom(i, j)) != -1 && !isCheckingPoint(i, j)) {
        DetectorSpaceCoordinate aux = getPointFrom(i, j)->getDetectorCoordinate(
            listImages.at(i)->getId());
        double xi = aux.getXi();
        double eta = aux.getEta();
        Matrix temp(2, 1);
        temp.set(1, 1, xi);
        temp.set(2, 1, eta);
        result = result | temp;
      }
    }
  }
  Lb = result;
}

void BundleAdjustment::calculateResiduos() {
  Matrix result(0, 1);
  for (int i = 0; i < numImages; i++) {
    int pnts = listImages.at(i)->countPoints();
    for (int j = 0; j < pnts; j++) {
      if (isControlPoint(i, j) && whereInPoints(getPointFrom(i, j)) != -1) {
        double x = getPointFrom(i, j)->getObjectCoordinate().getX();
        double y = getPointFrom(i, j)->getObjectCoordinate().getY();
        double z = getPointFrom(i, j)->getObjectCoordinate().getZ();
        double xi = getPointFrom(i, j)
                        ->getDetectorCoordinate(listImages.at(i)->getId())
                        .getXi();
        double eta = getPointFrom(i, j)
                         ->getDetectorCoordinate(listImages.at(i)->getId())
                         .getEta();

        Matrix coord = getCoordColinearTerrain(xi, eta, z, i + 1);
        double resx = x - coord.get(1, 1);
        double resy = y - coord.get(1, 2);
        // reusing the matrix that will no longer be used in this loop;
        coord = coord.transpose();
        coord.set(1, 1, resx);
        coord.set(2, 1, resy);
        result = result | coord;
      }
    }
  }
  matRes = result;
}

void BundleAdjustment::calculatePeso() {
  int d = 1;
  for (int i = 0; i < numImages; i++) {
    int numPoints = listImages.at(i)->countPoints();
    for (int j = 0; j < numPoints; j++) {
      if (isControlPoint(i, j) && whereInPoints(getPointFrom(i, j)) != -1) {
        double p =
            (numEquations - numUnknows) / (pow(getRx(listImages.at(i), j), 2));
        P.set(d, d, p);
        d++;
        p = (numEquations - numUnknows) /
            ((pow(getRy(listImages.at(i), j), 2)));
        P.set(d, d, p);
        d++;
      }
      if (isPhotogrammetricPoint(i, j) &&
          whereInPoints(getPointFrom(i, j)) != -1) {
        P.set(d, d, 1);
        d++;
        P.set(d, d, 1);
        d++;
      }
    }
  }
}
void BundleAdjustment::updateCoordFotog() {
  int n = 1;
  for (int i = 0; i < numPoints; i++) {
    Point *pnt = listPoints.at(i);
    if (pnt->getType() == Point::PHOTOGRAMMETRIC && whereInPoints(pnt) != -1) {
      ObjectSpaceCoordinate coord = pnt->getObjectCoordinate();
      double x = coord.getX();
      double y = coord.getY();
      double z = coord.getZ();
      coord.setX(x + getdXx2(n));
      coord.setY(y + getdYx2(n));
      coord.setZ(z + getdZx2(n));
      pnt->setObjectCoordinate(coord);
      n++;
    }
  }
}

double BundleAdjustment::getRx(Image *img, int pointIndex) {
  int base = 1;
  int pos = whereInImages(img);
  for (int i = 0; i < pos; i++) {
    base += 2 * numberControlPoints(listImages.at(i));
  }
  return matRes.get(base + 2 * (pointIndex), 1);
}

double BundleAdjustment::getRy(Image *img, int pointIndex) {
  int base = 1;
  int pos = whereInImages(img);
  for (int i = 0; i < pos; i++) {
    base += 2 * numberControlPoints(listImages.at(i));
  }
  return matRes.get(base + 2 * (pointIndex) + 1, 1);
}

int BundleAdjustment::whereInImages(Image *img) {
  for (int i = 0; i < numImages; i++) {
    if (listImages.at(i) == img) return i;
  }
  return -1;
}

// Methods to increase calculation performance
Matrix BundleAdjustment::getMatRes() { return matRes; }

std::deque<double> BundleAdjustment::getListRMSE() { return listRMSE; }

bool BundleAdjustment::getConvergencyStatus() { return converged; }

int BundleAdjustment::getTotalIterations() { return totalIterations; }

void BundleAdjustment::setAFP() { afp = matAdjust; }

Matrix BundleAdjustment::getAFP() { return afp; }

// auxiliary methods for testing
bool BundleAdjustment::isControlPoint(int imageIndex, int pointIndex) {
  return listImages.at(imageIndex)->getPointAt(pointIndex)->getType() ==
         Point::CONTROL;
}

bool BundleAdjustment::isPhotogrammetricPoint(int imageIndex, int pointIndex) {
  return listImages.at(imageIndex)->getPointAt(pointIndex)->getType() ==
         Point::PHOTOGRAMMETRIC;
}

bool BundleAdjustment::isCheckingPoint(int imageIndex, int pointIndex) {
  return listImages.at(imageIndex)->getPointAt(pointIndex)->getType() ==
         Point::CHECKING;
}

bool BundleAdjustment::testResiduo() {
  int rowsMatRes = matRes.getRows();
  for (int i = 1; i <= rowsMatRes; i++) {
    if (fabs(double(matRes.get(i, 1) > MAXRESIDUO))) {
      return false;
    }
  }
  return true;
}

// If it gives infinity it returns -2,
// if it gives NAN it returns -1,
// if it does not converge it returns 0
// and if it converges it returns 1
int BundleAdjustment::testConverged() {
  int rowsX1 = x1.getRows();
  int rowsX2 = x2.getRows();

  for (int i = 0; i < rowsX1; i++) {
    if (std::isinf(fabs(x1.get(i + 1, 1)))) {
      return -2;
    }
    if (std::isnan(fabs(x1.get(i + 1, 1)))) {
      return -1;
    }
    if (i % 6 < 3)  // The first three elements are Xo,Yo,Zo
    {
      if (fabs(double(x1.get(i + 1, 1) > metricConvergency))) return 0;
    } else  // The last three elements are omega, phi, kappa
    {
      if (fabs(double(x1.get(i + 1, 1) > angularConvergency))) {
        return 0;
      }
    }
  }

  for (int i = 1; i <= rowsX2; i++) {
    if (std::isinf(fabs(x2.get(i, 1)))) {
      return -2;
    }
    if (std::isnan(fabs(x2.get(i, 1)))) {
      return -1;
    }
    if (fabs(double(x2.get(i, 1) > metricConvergency))) {
      return 0;
    }
  }
  return 1;
}

bool BundleAdjustment::isConverged() {
  if (totalIterations <= maxIterations && testConverged() == 1) {
    return converged = true;
  } else {
    return converged = false;
  }
}

void BundleAdjustment::setUserInitialValues(Matrix initialValues) {
  matInitialValues = initialValues;
  userInitialValues = true;
}

std::string BundleAdjustment::printAll() {
  std::string result = "";
  int numImages = listImages.size();
  for (int i = 0; i < numImages; i++) {
    Image *img = listImages.at(i);
    result += img->getFilename().c_str();
    result += "\n";
    int numPoints = img->countPoints();
    for (int j = 0; j < numPoints; j++) {
      if (whereInPoints(getPointFrom(i, j)) != -1) {
        Point *pnt = img->getPointAt(j);
        double col = pnt->getImageCoordinate(img->getId()).getCol();
        double lin = pnt->getImageCoordinate(img->getId()).getLin();
        double X = pnt->getObjectCoordinate().getX();
        double Y = pnt->getObjectCoordinate().getY();
        double Z = pnt->getObjectCoordinate().getZ();
        result += "\t";
        result += pnt->getPointId().c_str();
        result += " col: ";
        result += Conversion::doubleToString(col);
        result += " lin: ";
        result += Conversion::doubleToString(lin);
        result += " X: ";
        result += Conversion::doubleToString(X);
        result += " Y: ";
        result += Conversion::doubleToString(Y);
        result += " Z: ";
        result += Conversion::doubleToString(Z);
        result += "\n";
      }
    }
    result += "\n";
  }
  return result;
}

Matrix BundleAdjustment::getMVC() {
  // This is done only for Display of angles in Degrees
  Matrix tempX1(numImages, 6);
  for (int i = 0; i < numImages; i++) {
    tempX1.set(i + 1, 1, Dms::radianoToDegreeDecimal(x1.get(6 * i + 4, 1)));
    tempX1.set(i + 1, 2, Dms::radianoToDegreeDecimal(x1.get(6 * i + 5, 1)));
    tempX1.set(i + 1, 3, Dms::radianoToDegreeDecimal(x1.get(6 * i + 6, 1)));
    tempX1.set(i + 1, 4, x1.get(6 * i + 1, 1));
    tempX1.set(i + 1, 5, x1.get(6 * i + 2, 1));
    tempX1.set(i + 1, 6, x1.get(6 * i + 3, 1));
  }
  return tempX1;
}

bool BundleAdjustment::isPossibleCalculate() {
  if (numEquations < numUnknows) {
    return false;
  } else {
    return true;
  }
}

Matrix BundleAdjustment::getL0() { return L0; }

Matrix BundleAdjustment::getLb() { return Lb; }

void BundleAdjustment::setMetricConvergencyValue(double value) {
  metricConvergency = value;
}

double BundleAdjustment::getMetricConvergencyValue() {
  return metricConvergency;
}

void BundleAdjustment::setAngularConvergencyValue(double value) {
  angularConvergency = value;
}

double BundleAdjustment::getAngularConvergencyValue() {
  return angularConvergency;
}

void BundleAdjustment::setMaxNumberIterations(int value) {
  maxIterations = value;
}

int BundleAdjustment::getMaxNumberIterations() { return maxIterations; }

Matrix BundleAdjustment::getMatrixInicialValues() { return matInitialValues; }

Matrix BundleAdjustment::getResiduo(Point *photogrammetricPoint) {
  Matrix result(1, 3);
  int posFoto = whereInPhotogrammetricPoints(photogrammetricPoint);

  if (posFoto > 0) {
    result.set(1, 1, getdXx2(posFoto));
    result.set(1, 2, getdYx2(posFoto));
    result.set(1, 3, getdZx2(posFoto));
  }

  return result;
}

std::deque<Point *> BundleAdjustment::getPhotogrammetricList() {
  return listPhotogrammetricPoints;
}

bool BundleAdjustment::hasAllImagesInitialValues() {
  for (unsigned int i = 0; i < listImages.size(); i++) {
    Image *img = listImages.at(i);
    if (!img->isInsAvailable()) return false;
    if (!img->isGnssAvailable()) return false;
    if (img->getInsType() != "Initial") return false;
    if (img->getGnssType() != "Initial") return false;
  }
  return true;
}

Matrix BundleAdjustment::convertToGeocentric(Matrix coordinates, GeoSystem sys,
                                             int hemi, int zona) {
  int rows = coordinates.getRows();
  for (int i = 1; i <= rows; i++) {
    double E = coordinates.get(i, 4);
    double N = coordinates.get(i, 5);
    double H = coordinates.get(i, 6);
    Matrix temp =
        ConvertionsSystems::utmToGeocentrica(E, N, H, zona, hemi, sys);
    coordinates.set(i, 4, temp.get(1, 1));
    coordinates.set(i, 5, temp.get(1, 2));
    coordinates.set(i, 6, temp.get(1, 3));
  }
  return coordinates;
}

Matrix BundleAdjustment::convertToUTM(Matrix coordinates, GeoSystem sys) {
  int rows = coordinates.getRows();
  for (int i = 1; i <= rows; i++) {
    double X = coordinates.get(i, 4);
    double Y = coordinates.get(i, 5);
    double Z = coordinates.get(i, 6);
    Matrix temp = ConvertionsSystems::GeocentricaToUtm(X, Y, Z, sys);
    coordinates.set(i, 4, temp.get(1, 1));
    coordinates.set(i, 5, temp.get(1, 2));
    coordinates.set(i, 6, temp.get(1, 3));
  }
  return coordinates;
}

void BundleAdjustment::convertToGeocentric(std::deque<Point *> points,
                                           GeoSystem sys, int hemi, int zona) {
  int rows = points.size();
  for (int i = 0; i < rows; i++) {
    Point *pnt = points.at(i);
    ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
    double E = temp.getX();
    double N = temp.getY();
    double H = temp.getZ();
    Matrix gc = ConvertionsSystems::utmToGeocentrica(E, N, H, zona, hemi, sys);
    temp.setX(gc.get(1, 1));
    temp.setY(gc.get(1, 2));
    temp.setZ(gc.get(1, 3));
    pnt->setObjectCoordinate(temp);
  }
}

void BundleAdjustment::convertToUTM(std::deque<Point *> points, GeoSystem sys) {
  int rows = points.size();
  for (int i = 0; i < rows; i++) {
    Point *pnt = points.at(i);
    ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
    double E = temp.getX();
    double N = temp.getY();
    double H = temp.getZ();
    Matrix gc = ConvertionsSystems::GeocentricaToUtm(E, N, H, sys);
    temp.setX(gc.get(1, 1));
    temp.setY(gc.get(1, 2));
    temp.setZ(gc.get(1, 3));
    pnt->setObjectCoordinate(temp);
  }
}

double BundleAdjustment::calculateRMSE() {
  int numRes = matRes.getRows();
  double sum = 0;
  for (int i = 1; i <= numRes; i++) {
    sum += pow(matRes.get(i, 1), 2);
  }
  return sqrt(sum / (numEquations - numUnknows));
}

void BundleAdjustment::normalize(std::deque<Point *> points, double &range,
                                 double &offset) {
  int rows = points.size();
  double minX, minY, minZ, maxX, maxY, maxZ, minT, maxT;

  Point *pnt = points.at(0);
  ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();

  minX = maxX = temp.getX();
  minY = maxY = temp.getY();
  minZ = maxZ = temp.getZ();

  for (int i = 1; i < rows; i++) {
    Point *pnt = points.at(i);
    ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
    double X = temp.getX();
    double Y = temp.getY();
    double Z = temp.getZ();

    if (X < minX) {
      minX = X;
    }
    if (Y < minY) {
      minY = Y;
    }
    if (Z < minZ) {
      minZ = Z;
    }
    if (X >= maxX) {
      maxX = X;
    }
    if (Y >= maxY) {
      maxY = Y;
    }
    if (Z >= maxZ) {
      maxZ = Z;
    }
  }

  // Minimum and maximum calculation

  if (maxX >= maxY) {
    maxT = maxX;
  } else {
    maxT = maxY;
  }

  if (maxT < maxZ) {
    maxT = maxZ;
  }

  if (minX <= minY) {
    minT = minX;
  } else {
    minT = minY;
  }

  if (minT > minZ) {
    minT = minZ;
  }

  double myRange = maxT - minT;
  double myOffset = -(minT);

  for (int i = 0; i < rows; i++) {
    Point *pnt = points.at(i);
    ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
    double X = temp.getX();
    double Y = temp.getY();
    double Z = temp.getZ();

    temp.setX((X + myOffset) / myRange);
    temp.setY((Y + myOffset) / myRange);
    temp.setZ((Z + myOffset) / myRange);
    pnt->setObjectCoordinate(temp);
  }

  for (unsigned int i = 1; i <= matInitialValues.getRows(); i++) {
    double X0 = matInitialValues.get(i, 4);
    double Y0 = matInitialValues.get(i, 5);
    double Z0 = matInitialValues.get(i, 6);

    matInitialValues.set(i, 4, (X0 + myOffset) / myRange);
    matInitialValues.set(i, 5, (Y0 + myOffset) / myRange);
    matInitialValues.set(i, 6, (Z0 + myOffset) / myRange);
  }

  metricConvergency = (metricConvergency + myOffset) / myRange;
  range = myRange;
  offset = myOffset;
}

void BundleAdjustment::desnormalize(std::deque<Point *> points, double range,
                                    double offset) {
  int rows = points.size();
  for (int i = 0; i < rows; i++) {
    Point *pnt = points.at(i);
    ObjectSpaceCoordinate temp = pnt->getObjectCoordinate();
    double X = temp.getX();
    double Y = temp.getY();
    double Z = temp.getZ();
    temp.setX(X * range - offset);
    temp.setY(Y * range - offset);
    temp.setZ(Z * range - offset);
    pnt->setObjectCoordinate(temp);
  }

  for (unsigned int i = 1; i <= matAdjust.getRows(); i++) {
    double X0 = matAdjust.get(i, 4);
    double Y0 = matAdjust.get(i, 5);
    double Z0 = matAdjust.get(i, 6);

    matAdjust.set(i, 4, X0 * range - offset);
    matAdjust.set(i, 5, Y0 * range - offset);
    matAdjust.set(i, 6, Z0 * range - offset);
  }

  metricConvergency = metricConvergency * range - offset;
}
}  // namespace efoto
}  // namespace eng
}  // namespace uerj
}  // namespace br
