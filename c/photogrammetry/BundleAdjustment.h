#ifndef BUNDLEADJUSTMENT_H
#define BUNDLEADJUSTMENT_H
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

#include "CommonDef.h"
#include "GeoSystem.h"
#include "Matrix.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class Point;
class Sensor;
class Image;
class InteriorOrientation;

/**
 * \file BundleAdjustment.h
 * \class BundleAdjustment
 * \brief Class that calculates the parameters of the exterior orientations of
 * all images in a block
 */

class BundleAdjustment
{

protected:
 double c;  // calibrated focal length
 // Coordinates of the main point of the images in millimeters
 double xsi0, eta0;
 int numEquations;
 int numUnknows;
 int numImages;
 int numPoints;
 int numControlPoints;
 int numFotogrametricPoints;

 int maxIterations;
 int totalIterations;
 double metricConvergency, angularConvergency;

 double r11, r12, r13, r21, r22, r23, r31, r32, r33;
 Matrix A1, A2, P, Lb, L0, x1, x2, matRes;
 Matrix matAdjust, matInitialValues;
 Matrix inverseM11, inverseN11, inverseN22;
 Matrix n11, n12, n22, n1, n2;
 Matrix afp;  // Matrix with final results
 // Auxiliary variables

 Sensor *sensor;
 std::deque<Image *> listImages;
 std::deque<Point *> listPoints;

 std::deque<Point *> listControlPoints;
 std::deque<Point *> listPhotogrammetricPoints;

 std::deque<double> listRMSE;

 bool done;
 bool converged;
 bool userInitialValues;

public:
 /**
  * \brief Basic constructor for the calculation
  * \param listSelectedimages : List of all images that will be considered in the calculation
  * \param listSelectedPoints : List of all points that will be considered in the calculation
  */
 explicit BundleAdjustment(std::deque<Image *> listSelectedImages,
                           std::deque<Point *> listSelectedPoints);

 /**
  * \brief Returns the number of equations the current calculation has to solve
  * \return int : number of equations
  */
 int numberOfEquations();

 /**
  * \brief Method for debug prints in terminal matrix with a label
  * \deprecated The Matrix Class already has a similar method, namely, Matrix::show()
  * \param mat : Matrix to display in terminal
  * \param id  : label
  * text
  */
 void imprime(Matrix mat, char *id);

 /**
  * \brief Calculates the matrix M11
  * \param    M1   : Matrix formed by the analog coordinates of all points
  * \return Matrix : Matrix M11, following the Boltz method
  */
 Matrix getM11(Matrix M1);

 /**
  * \brief Calculates the matrix M22
  * \param    M2   : Matrix formed by submatrices for each photogrammetric point
  * \return Matrix : Matrix M22, following the Boltz method
  */
 Matrix getM22(Matrix M2);

 /**
  * \brief Calculates the matrix M12
  * \param    M1   : Matrix formed by the analog coordinates of all points
  * \param    M2   : Matrix formed by submatrices for each photogrammetric point
  * \return Matrix : Matrix M12, following the Boltz method
  */
 Matrix getM12(Matrix M1, Matrix M2);

 /**
  * \brief Calculates the m1 matrix, necessary for the initial calculation of
  * the exterior orientation parameters
  * \param    M1   : Matrix formed by the analog coordinates of all points
  * \param    L1   : Matrix formed by the X and Y coordinates of the control points
  * \return Matrix : Matrix m1
  */
 Matrix getm1(Matrix M1, Matrix L1);

 /**
  * \brief Calculates the m1 matrix, necessary for the initial calculation of
  * the exterior orientation parameters
  * \param    M2   : Matrix formed by submatrices for each photogrammetric point
  * \param    L1   : Matrix formed by the X and Y coordinates of the control points
  * \return Matrix : Matrix m1
  */
 Matrix getm2(Matrix M2, Matrix L1);

 /**
  * \brief Method that returns the matrix with the first approximations of the
  * exterior orientation parameters
  * \param    M12  : Matrix M12
  * \param    m1   : Matrix m1
  * \param    xypf : Matrix xypf, matrix with the first approximations
  * of the coordinates of the photogrammetric points
  * \return Matrix : Matrix paf, matrix with the first approximations of
  * the exterior orientation parameters
  */
 Matrix getPAf(Matrix M12, Matrix m1, Matrix xypf);

 /**
  * \brief Method that returns the matrix with the first approximations of the
  * coordinates of the photogrammetric points
  * \param    M22  : Matrix M22
  * \param
  * m2   : Matrix m2
  * \param    M12  : Matrix M12
  * \param    paf  : Matrix paf
  * which contains the initial values ​​of the exterior orientation parameters
  * \return Matrix : Matrix xypf, matrix with the first
  * approximations of the coordinates of the photogrammetric points
  */
 Matrix getXYpf(Matrix M22, Matrix m2, Matrix M12, Matrix paf);

 /**
  * \deprecated Initial guess Z values ​​are currently calculated based on
  * the Flight form
  */
 double getZpf();

 /**
  * \brief Method that returns only the coefficients of an affine function
  *        to convert analog coordinate(xsi,eta) to terrain coordinate
  *
  * \param   Paf   : Matrix with all the parameters of the exterior orientation
  * of all the images
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager,
  * but the line in which its parameters are found in the adjustment
  * matrix(matAdjust)
  * \return Matrix : Matrix with the parameters of the
  * exterior orientation of an image
  */
 Matrix getPTA(Matrix PAf, int imageId);

 /**
  * \brief Changes the final version (AFP) of the adjustment matrix (matAdjust)
  *        of the exterior orientation parameters
  */
 void setAFP();

 /**
  * \brief Method that returns the final version of the adjustment matrix
  * (matAdjust) of the exterior orientation parameters
  *
  * \return Matrix : Matrix AFP that contains all parameters of all images with
  * omega, phi and kappa in degrees
  */
 Matrix getAFP();

 /**
  * \brief Method that returns a matrix with the analog coordinates of the
  * points calculated by the collinearity equations
  * \return Matrix : Returns the
  * L0 matrix
  */
 Matrix getL0();

 /**
  * \brief Method that returns a matrix with the analog coordinates of the
  * points calculated by interior orientation
  * \return Matrix : Returns the Lb
  * matrix
  */
 Matrix getLb();

 /**
  * \brief Method that returns the matrix of residuals from the last iteration
  * \return Matrix : Returns the matrix of residuals
  */
 Matrix getMatRes();

 std::deque<double> getListRMSE();

protected:
 // defines the matrix based on the angles
 /**
  * \brief Changes the rotation matrix values
  * \param omega : omega angle in radians
  * \param phi   : phi angle in radians
  * \param kappa : kappa angle in radians
  * \attention rotation matrix values are contained in attributes double :
  * r11,r12,r13,r21,r22,r23,r31,r32 e r33
  */
 void setRot(double omega, double phi, double kappa);

 /**
  * \brief Method that returns the x and straight values of an X Y Z coordinate
  * contained in an image through collinearity equations
  * \param    X    :
  * Coordinate value on axis X
  * \param    Y    : Coordinate value on axis Y
  * \param    Z    : Coordinate value on axis Z
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager,
  * but the line in which its parameters are found in the adjustment matrix (matAdjust)
  * \return Matrix : Matrix with xi and eta coordinates
  */
 Matrix getCoordinatesEqColin(double X, double Y, double Z, int imageId);

 /**
  * \brief Method that returns a Jacobian matrix of an X Y Z coordinate of a
  * Control point
  * \param    X    : Coordinate value on axis X
  * \param    Y    : Coordinate value on axis Y
  * \param    Z    : Coordinate value on axis Z
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager,
  * but the line in which its parameters are found in the adjustment matrix (matAdjust)
  * \return Matrix : Jacobian Matrix of the coordinate
  */
 Matrix getJacobianaControl(double X, double Y, double Z, int imageId);

 /**
  * \brief Method that returns the Jacobian matrix of an X Y Z coordinate of a
  * photogrammetric point
  * \param    X    : Coordinate value on axis X
  * \param    Y    : Coordinate value on axis Y
  * \param    Z    : Coordinate value on axis Z
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager,
  * but the line in which its parameters are found in the adjustment matrix (matAdjust)
  * \return Matrix : Jacobian Matrix of the coordinate
  */
 Matrix getJacobianaFotogrametric(double X, double Y, double Z, int imageId);

 /**
  * \brief Creates or changes the Matrix A1 attribute, which is the matrix
  * formed by the Jacobian matrices of all points as if they were control
  */
 void createA1();

 /**
  * \brief Creates or changes the Matrix A2 attribute, which is the matrix
  * formed by the Jacobian matrices of the photogrammetric points only
  */
 void createA2();

 /**
  * \brief Method that creates or changes a matrix with the analogical
  * coordinates of the points calculated by interior orientation
  */
 void createLb();

 /**
  * \brief Method that creates or changes a matrix with the analog coordinates
  * of the points calculated by the collinearity equations
  */
 void createL0();

 /**
  * \brief Calculate matrix N11 using matrix A1
  * \return Matrix : Matrix N11, following Boltz method
  */
 Matrix getN11();

 /**
  * \brief Calculate matrix N22 using matrix A2
  * \return Matrix : Matrix N22, following Boltz method
  */
 Matrix getN22();

 /**
  * \brief Calculate matrix N12 using matrix A1 e A2
  * \return Matrix : Matrix N12, following Boltz method
  */
 Matrix getN12();

 /**
  * \brief Calculates matrix n1, necessary for calculating the adjustment of the
  * exterior orientation parameters
  * \param    l    : Difference matrix between Lb and L0
  * \return Matrix : Matrix n1
  */
 Matrix getn1(Matrix l);

 /**
  * \brief Calculates matrix n2, necessary for calculating the adjustment of the
  * exterior orientation parameters
  * \param    l    : Difference matrix between Lb and L0
  * \return Matrix : Matrix n2
  */
 Matrix getn2(Matrix l);

 /**
  * \brief Calculates and changes the matrix of weights
  * \param p : matrix of weights
  * \attention Currently the weight matrix is ALWAYS the identity matrix in the
  * calculation of the parameters of the exterior orientation
  */
 void setPeso(Matrix p);

 /**
  * \brief Returns the current weight matrix
  * \attention Currently, the weight matrix is ALWAYS the identity matrix in the
  * calculation of the exterior orientation parameters
  */
 Matrix getPeso();

 /**
  * \brief Calculates and alters the matrix x1 which is the matrix that contains
  * the "deltas" to be summed in the parameters of the exterior orientation
  * \param    N12  : Matrix N12
  * \param    n1   : Matrix n1
  */
 void setx1(Matrix N12, Matrix n1);

 /**
  * \brief Calculates and alters the matrix x2 which is the matrix that contains
  * the "deltas" to be added in the coordinates of the photogrammetric points
  * \param    N12  : Matrix N12
  * \param    N22  : Matrix N22
  * \param    n2   : Matrix n2
  * \param    n1   : Matrix n1
  */
 void setx2(Matrix N12, Matrix N22, Matrix n2, Matrix n1);

 /**
  * \brief Auxiliary method that returns the Omega angle adjustment value
  * (delta) contained in the x1 matrix
  *
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager,
  * but the line in which its parameters are found in the adjustment matrix (matAdjust)
  * \return double : Delta value of the Omega angle contained in the previously
  * calculated matrix x1
  */
 double getdOmegax1(int imageId);

 /**
  * \brief Auxiliary method that returns the adjustment value (delta) of the Phi
  * angle contained in the matrix x1
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the
  * ProjectManager, but the line in which its parameters are found in the
  * adjustment matrix (matAdjust)
  * \return double : Delta value of the Phi angle contained in the previously
  * calculated matrix x1
  */
 double getdPhix1(int imageId);

 /**
  * \brief Auxiliary method that returns the adjustment value (delta) of the
  * Kappa angle contained in the matrix x1
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the
  * ProjectManager, but the line in which its parameters are found in the
  * adjustment matrix (matAdjust)
  * \return double : Delta value of the Kappa angle contained in the previously
  * calculated matrix x1
  */
 double getdKappax1(int imageId);

 /**
  * \brief Auxiliary method that returns the adjustment value (delta) of
  * coordinate X0 contained in matrix x1
  * \param imageId : image id in current
  * calculation
  * \attention the imageId is NOT the key of the image in the
  * ProjectManager, but the line in which its parameters are found in the
  * adjustment matrix (matAdjust)
  * \return double : Delta value of the X0 coordinate contained in the
  * previously calculated x1 matrix
  */
 double getdXx1(int imageId);

 /**
  * \brief Auxiliary method that returns the adjustment value (delta) of the Y0
  * coordinate contained in the x1 matrix
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the
  * ProjectManager, but the line in which its parameters are found in the
  * adjustment matrix (matAdjust)
  * \return double : Delta value of the Y0
  * coordinate contained in the previously calculated x1 matrix
  */
 double getdYx1(int imageId);

 /**
  * \brief Auxiliary method that returns the adjustment value (delta) of the Z0
  * coordinate contained in the x1 matrix
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the
  * ProjectManager, but the line in which its parameters are found in the
  * adjustment matrix (matAdjust)
  * \return double : Delta value of the Z0 coordinate contained in the
  * previously calculated x1 matrix
  */
 double getdZx1(int imageId);

 /**
  * \brief Auxiliary method that returns the value of Omega in the fit matrix
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager,
  * but the line in which its parameters are found in the adjustment matrix
  * (matAdjust)
  * \return double : Omega value contained in matAdjust Matrix
  */
 double getOmegaAdjus(int imageId);

 /**
  * \brief Auxiliary method that returns the value of Phi in the adjustment matrix
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager,
  * but the line in which its parameters are found in the adjustment matrix (matAdjust)
  * \return double : Phi value contained in the matAdjust Matrix
  */
 double getPhiAdjus(int imageId);

 /**
  * \brief Auxiliary method that returns the Kappa value in the adjustment matrix
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager, but the line in
  * which its parameters are found in the adjustment matrix (matAdjust)
  * \return double : Kappa value contained in the matAdjust Matrix
  */
 double getKappaAdjus(int imageId);

 /**
  * \brief Auxiliary method that returns the X0 coordinate value in the adjustment matrix
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager,
  * but the line in which its parameters are found in the adjustment matrix (matAdjust)
  * \return double : Value of the X0 coordinate contained in the
  * matAdjust Matrix
  */
 double getXAdjus(int imageId);

 /**
  * \brief Auxiliary method that returns the value of the Y0 coordinate in the
  * adjustment matrix
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager,
  * but the line in which its parameters are found in the adjustment matrix (matAdjust)
  * \return double : Value of the Y0 coordinate contained in the matAdjust Matrix
  */
 double getYAdjus(int imageId);

 /**
  * \brief Auxiliary method that returns the Z0 coordinate value in the
  * adjustment matrix
  * \param imageId : image id in current calculation
  * \attention the imageId is NOT the key of the image in the ProjectManager,
  * but the line in which its parameters are found in the adjustment matrix (matAdjust)
  * \return double : Value of the Z0 coordinate contained in the matAdjus Matrixt
  */
 double getZAdjus(int imageId);

 /**
  * \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da
  * coordenada X do ponto fotogramétrico contida na matriz x2
  * \param fotogrPointId : id in current calculation
  * \attention the fotogrPointId is NOT the key or pointId of the point in the
  * ProjectManager, but the position
  * in the list of points where it is located
  * \return double : Delta value of the X coordinate of the point contained
  * in the previously calculated x2 matrix
  */
 double getdXx2(int fotogrPointId);

 /**
  * \brief Auxiliary method that returns the adjustment value (delta) of the Y
  * coordinate of the photogrammetric point contained in the matrix x2
  * \param fotogrPointId : id in current calculation
  * \attention the fotogrPointId is NOT the key or pointId of the point in the
  * ProjectManager, but the position in the list of points where it is located
  * \return double : Delta value of the Y coordinate of the point contained in
  * the previously calculated x2 matrix
  */
 double getdYx2(int fotogrPointId);

 /**
  * \brief Auxiliary method that returns the adjustment value (delta) of the Z
  * coordinate of the photogrammetric point contained in the x2 matrix
  * \param fotogrPointId : id in current calculation
  * \attention the fotogrPointId is NOT the key or pointId of the point in the
  * ProjectManager, but the position in the list of points where it is located
  * \return double : Delta value of the Z coordinate of the point contained in
  * the previously calculated x2 matrix
  */
 double getdZx2(int fotogrPointId);

 /**
  * \brief Method that updates the parameter values of the exterior orientations
  * using the values contained in the x1 matrix
  */
 void updateMatAdjust();

 /**
  * \brief Method that returns a matrix with terrain coordinates by spatial
  * intersection equations
  * \param    xsi  : Analog coordinate xi in millimeters
  * \param    eta  : Analog coordinate eta in millimeters
  * \param    z    : z coordinate in meters
  * \return Matrix : Matrix containing X and Y terrain coordinates
  */
 Matrix getCoordColinearTerrain(double xsi, double eta, double z, int imageId);

public:
 /**
  * \brief Method that fills Point objects with analog coordinates based on
  * interior orientation
  */
 void fillDetectorCoordinates();

 /**
  * \brief Method that converts analog coordinates (xi,eta) to digital coordinates (col,lin)
  * \param oi  : Interior orientation of the image the coordinate is in
  * \param xsi : analog coordinate xi
  * \param eta : analog coordinate eta
  * \deprecated This method will be replaced by the existent method on
  * class *nome da classe MOTOR DE FOTOGRAMETRIAm*
  */
 Matrix analogToDigital(InteriorOrientation *oi, double xsi, double eta);

 /**
  * \brief Metodo que converte coordenada digital(col,lin) em coordenada
  * analogica(xi,eta)
  * \param oi  : Orientação interior da imagem em que a coordenada está
  * \param linha : Coodernada analogica linha
  * \param coluna : Coodernada analogica coluna
  * \deprecated This method will be replaced by the existent method on another class
  */
 Matrix digitalToAnalog(InteriorOrientation *oi, double linha, double coluna);

 /**
  * \brief Method that calculates the initial parameters of exterior
  * orientations
  */
 void calculateInicialsValues();

 /**
  * \brief Changes the matrix of the initial parameters of the exterior
  * orientations to those provided by the user in the interface
  * \param initialValues : Matrix with the initial parameters, with one image per line
  * in the sequence: Omega, Phi, Kappa, X0, Y0, Z0
  */
 void setUserInitialValues(Matrix initialValues);

 /**
  * \brief Auxiliary method that returns an matrix corresponding to an image in matrix M1
  * \param img    : Image object containing the points belonging to it
  * \param Matrix : Matrix corresponding to an image
  */
 Matrix imageToMatrixDetectorCoordenates(Image *img);

 /**
  * \brief Auxiliary method that returns an matrix corresponding to an image in matrix A1
  * \param img    : Image object containing the points belonging to it
  * \param Matrix : Matrix corresponding to an image
  */
 Matrix imageToMatrixJacobiana(int indexImage);

 /**
  * \brief Method responsible for all the calculation of the adjustment by Bundle Adjustment
  * \return bool: Informs whether the calculation was
  * successful or not
  */
 bool calculate(bool makeReport);

 /**
  * \brief Method that informs whether it is possible to calculate the
  * adjustment or not, based on the number of unknowns and equations
  * \return bool : Informs whether or not it is possible to calculate
  */
 bool isPossibleCalculate();

 /**
  * \brief Method that creates or alters the matrix L
  * \return Matrix : returns the matrix formed by the X and Y coordinates of the control points
  */
 Matrix createL();

 /**
  * \brief Method that creates or alters the matrix M1
  * \return Matrix : returns the formed matrix M1 necessary for the initial
  * calculation of the parameters
  */
 Matrix createM1();

 /**
  * \brief Method that creates or alters the matrix M2
  * \return Matrix : returns the formed matrix M2 necessary for the initial
  * calculation of the parameters
  */
 Matrix createM2();

 /**
  * \brief Auxiliary method that calculates the initial Z coordinate for each
  * photogrammetric point
  * \return double : Initial value of the Z coordinate of
  * the photogrammetric points
  */
 double getInicialZPhotogrammetricPoints();

 /**
  * \brief Method that updates the initial coordinates of all photogrammetric
  * points based on the xypf matrix
  * \param xypf : Matrix containing coordinates of photogrammetric points
  * \param zphotogrammetric : Z value of the photogrammetric point coordinate
  */
 void updateCoordinatesAllPoints(Matrix xypf, double zphotogrammetric);

 // These methods refer to their order in the LIST OF INDEX POINTS OF EACH IMAGE
 // and not in the xml list
 /**
  * \brief Auxiliary method that tests whether a given point contained in a
  * given image is photogrammetric
  * \param imageIndex : Image position in the image list
  * \param pointIndex : Position of the point in the list of points
  * in the image informed by the parameter imageIndex
  * \return bool : informs if the point is photogrammetric or not
  */
 bool isPhotogrammetricPoint(int imageIndex, int pointIndex);

 /**
  * \brief Auxiliary method that tests whether a given point contained in a
  * given image is a control one
  * \param imageIndex : Image position in the image list
  * \param pointIndex : Position of the point in the list of points in the
  * image informed by the parameter imageIndex
  * \return bool : informs if the point is control or not
  */
 bool isControlPoint(int imageIndex, int pointIndex);

 /**
  * \brief Auxiliary method that tests whether a given point contained in a
  * given image is a 'checking'
  * \param imageIndex : Image position in the image list
  * \param pointIndex : Position of the point in the list of points in the
  * image informed by the parameter imageIndex
  * \return bool : informs whether the point is a 'checking' point or not
  */
 bool isCheckingPoint(int imageIndex, int pointIndex);

 /**
  * \brief Method that informs whether the calculation converged or not based on
  * the number of iterations and on the convergence value stipulated by the user
  * \return bool : informs whether the calculation converged or not
  */
 bool isConverged();

 /**
  * \brief Method that analyzes whether the matrix x1 and x2 have reached their convergence
  * \return int : Code for the calculation convergence analysis, if
  * the calculation converged it returns 1, if it did not converge and there
  * were no problems in the matrices it returns 0, if there is any NAN in the
  * matrix it returns -1, if there is any INF value it returns -2
  */
 int testConverged();

 /**
  * \brief Method that analyzes whether the residual matrix has reached its convergence
  * \return bool : reports whether the residuals converged or not
  * \deprecated Currently this test is not done because there is no iteration
  * over the weight matrix P
  */
 bool testResiduo();

 /**
  * \brief Auxiliary method that returns a pointer to the point in the given image
  * \param imageIndex : Image position in the image list
  * \param pointIndex : Position of the point in the list of points in the image
  * informed by the parameter imageIndex
  * \return Point : reports whether the residuals converged or not
  */
 Point *getPointFrom(int imageIndex, int pointIndex);

 /**
  * \brief Auxiliary method that informs the number of control points contained in an image
  * \param img : Image object for which you want to know the number of control points
  * \return int : Number of control points
  */
 int numberControlPoints(Image *img);

 /**
  * \brief Auxiliary method that informs the number of photogrammetric points
  * contained in an image
  * \param img : Image object for which you want to know the number of photogrammetric points
  * \return int : Number of photogrammetric points
  */
 int numberPhotogrammetricPoints(Image *img);

 /**
  * \brief Auxiliary method that informs the position of a certain point in the
  * list of all points
  * \param pnt : Point object which you want to know its
  * position in the list
  * \return int : Position in the list, if the point is not in the list returns -1;
  */
 int whereInPoints(Point *pnt);

 /**
  * \brief Auxiliary method that informs the position of a certain point in the
  * list of photogrammetric points
  * \param pnt : Point object which you want to know its position in the list
  * \return int : Position in the list, if the point is not in the list returns -1;
  */
 int whereInPhotogrammetricPoints(Point *pnt);

 /**
  * \brief Method that resets all photogrammetric point coordinates
  * \deprecated Unnecessary method considering that the initial coordinates of
  * the photogrammetric points are irrelevant for the calculation
  */
 void zeroingCoordinatesPhotogrammetrics();

 /**
  * \brief Auxiliary method that returns the residual at the X coordinate of a
  * given point in a given image
  * \param img : Image object in which the point is contained
  * \param pointId : Position of the point in the list of points belonging to this image
  * \return double : Residue value in X
  */
 double getRx(Image *img, int pointId);

 /**
  * \brief Auxiliary method that returns the residual at the Y coordinate of a
  * given point in a given image
  * \param img : Image object in which the point is contained
  * \param pointId : Position of the point in the list of points belonging to this image
  * \return double : Residue value in Y
  */
 double getRy(Image *img, int pointId);

 /**
  * \brief Method that returns the current state of convergence of the calculation
  * \return bool : Current state of convergence
  */
 bool getConvergencyStatus();

 /**
  * \brief Method that returns the total number of iterations so far
  * \return int : total number of iterations
  */
 int getTotalIterations();

 /**
  * \brief Method that changes the maximum number of iterations
  * \param value : Maximum number of iterations
  */
 void setMaxNumberIterations(int value);

 /**
  * \brief Method that returns the maximum number of iterations
  * \return int : Maximum number of iterations
  */
 int getMaxNumberIterations();

 /**
  * \brief Method that changes the value of the convergence of the coordinates
  * of the parameters of the exterior orientations
  * \param value : Value of the convergence of coordinates
  */
 void setMetricConvergencyValue(double value);

 /**
  * \brief Method that changes the value of the convergence of the angles of the
  * parameters of the exterior orientations
  * \param value : Value of convergence of angles
  */
 void setAngularConvergencyValue(double value);

 /**
  * \brief Method that returns the value of the convergence of the coordinates
  * of the parameters of the exterior orientations
  * \return double : Coordinate convergence value
  */
 double getMetricConvergencyValue();

 /**
  * \brief Method that returns the value of the convergence of the angles of the
  * parameters of the exterior orientations
  * \return double : Convergence value of angles
  */
 double getAngularConvergencyValue();

 /**
  * \brief Method that returns the x1 matrix formatted to display angles in
  * degrees and each exterior orientation on a line
  * \return Matrix : formatted matrix
  */
 Matrix getMVC();

 /**
  * \brief Method that calculates the residuals after the calculation converges
  * and changes the matRes attribute that stores the residuals
  */
 void calculateResiduos();

 /**
  * \brief Method that calculates the matrix of weights after the calculation
  * converges and changes the attribute P that stores the weights
  */
 void calculatePeso();

 /**
  *	\brief Method that updates, that is, adjusts the coordinates of
  *photogrammetric points based on the x2 matrix
  */
 void updateCoordFotog();

 /**
  * \brief Method that inverts only the main diagonal matrices of the m11
  * matrix, thus avoiding unnecessary calculations
  * \param m11 : Matrix M11 to be inverted
  */
 void setInverseM11(Matrix m11);

 /**
  * \brief Method that inverts only the main diagonal matrices of matrix n11,
  * thus avoiding unnecessary calculations
  * \param n11 : Matrix N11 to be inverted
  */
 void setInverseN11(Matrix n11);

 /**
  * \brief Method that inverts only the main diagonal matrices of matrix n22,
  * thus avoiding unnecessary calculations
  * \param n22 : Matrix M22 to be inverted
  */
 void setInverseN22(Matrix n22);

 // prints all images with their points
 std::string printAll();

 /**
  * \brief Auxiliary method that returns the residuals of a photogrammetric
  * point, if the point is not photogrammetric it returns a matrix with zeroed elements
  * \param photogrammetricPoint : photogrammetric point object
  * \return Matrix : Matrix[1x3] with the residuals of the points in the sequence X, Y, Z
  */
 Matrix getResiduo(Point *photogrammetricPoint);

 /**
  * \brief Method that returns the matrix with the initial approximations of the
  * exterior orientation parameters of all images
  * \return Matrix : Matrix with external orientation parameters
  *
  */
 Matrix getMatrixInicialValues();

 /**
  * \brief Method that returns the list of photogrammetric points used in the
  * adjustment calculation
  * \return std::deque<Point*> : List with Point objects of photogrammetric points
  */
 std::deque<Point *> getPhotogrammetricList();

 /**
  * \brief Method that checks if the user has set all parameters of the exterior
  * orientation as initial values for all images
  * \return bool : Informs whether all images have initial values for the calculation
  * of exterior orientation parameters
  */
 bool hasAllImagesInitialValues();

 /**
  * \brief Method that returns the index in the list of images
  * \param img : Image whose position in the list you want to know
  * \return int : position in image list
  */
 int whereInImages(Image *img);

 /**
  * \brief Method that converts all coordinates to geocentric
  * \param coordinates : Matrix containing the coordinates (adjustment matrix)
  * \param sys : Geodetic system in which the coordinates are
  * \param hemi : integer informed in which hemisphere are the coordinates; in.
  * -1=South +1=North
  * \param zona : Zone in which the coordinates are
  * \return Matrix: Matrix with coordinates converted to geocentric
  */
 Matrix convertToGeocentric(Matrix coordinates, GeoSystem sys, int hemi,
                            int zona);

 /**
  * \brief Method that converts all coordinates to UTM
  * \param coordinates : Matrix containing the coordinates (adjustment matrix)
  * \param sys : Geodetic system in which the coordinates must be
  * \return Matrix: Matrix with coordinates converted to UTM
  */
 Matrix convertToUTM(Matrix coordinates, GeoSystem sys);

 /**
  * \brief Method that converts all coordinates to geocentric
  * \param points : list of points containing coordinates
  * \param sys : Geodetic system in which the coordinates are
  * \param hemi : integer informed in which hemisphere are the coordinates; in.
  * -1=South +1=North
  * \param zona : Zone in which the coordinates are
  * \return Matrix: Matrix with coordinates converted to geocentric
  */
 void convertToGeocentric(std::deque<Point *> points, GeoSystem sys, int hemi,
                          int zona);

 /**
  * \brief Method that converts all coordinates to UTM
  * \param points : list of points containing coordinates
  * \param sys : Geodetic system in which the coordinates must be
  * \return Matrix: Matrix with coordinates converted to UTM
  */
 void convertToUTM(std::deque<Point *> points, GeoSystem sys);

 /**
  * \brief Method that calculates the RMSE of the block
  * \return double: RMSE value
  */
 double calculateRMSE();

 /**
  * \brief Method that returns the matrix x1
  * \return Matrix
  */
 Matrix getx1() { return x1; };

 /**
  * \brief Method that returns the matrix x2
  * \return Matrix
  */
 Matrix getx2() { return x2; };

 /**
  * \brief Method that normalizes the coordinates
  * \param points : list of points
  * \param range : value in which the range value will be placed
  * \param offset : value in which the offset value will be placed
  */
 void normalize(std::deque<Point *> points, double &range, double &diff);

 /**
  * \brief Method that denormalizes coordinates
  * \param points : list of points
  * \param range : range value
  * \param offset : offset value
  */
 void desnormalize(std::deque<Point *> points, double range, double offset);
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // BUNDLEADJUSTMENT_H
