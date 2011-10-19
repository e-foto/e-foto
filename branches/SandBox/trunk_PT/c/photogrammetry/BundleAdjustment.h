#ifndef BUNDLEADJUSTMENT_H
#define BUNDLEADJUSTMENT_H

#include "Matrix.h"
#include <stdlib.h>
#include <deque>
#include <math.h>

#include "Sensor.h"
#include "Image.h"
#include "Point.h"
#include "Flight.h"
#include "InteriorOrientation.h"
#include "SparseMatrix.h"

//EIGEN
//#include "Core"
//#include "Dense"

//using namespace std;
class BundleAdjustment
{

/* Matriz de configuraÄÂ§ÄÅo do bloco. Valor 1 - ponto de controle, valor -1, ponto fotogramÄÅ trico, valor 0, ponto nÄÅo
contido na imagem. linha = imagem, coluna = ponto.
*/
/*int blc[3][6];
Matrix BLC;*/

protected:

    double c;     // Distancia focal calibrada
    int fliDir;   // Marca fiducial com a direÃ§ao de vÃ´o
    //Coordenadas do ponto principal das imagens em milimetros
    double xsi0,eta0;
    int numEquations;
    int numUnknows;
    int numImages;
    int numPoints;
    int numControlPoints;
    int numFotogrametricPoints;

	int maxIterations;
	double metricConvergency, angularConvergency;


    Matrix afp; //Matriz com resultados finais
/* Variaveis auxiliares */

    Sensor* sensor;
    bool done;

public:
    BundleAdjustment();

	int numberOfEquations();

	//metodos para debug
    void imprime(Matrix mat, char *id);

// metodos para fototri

    Matrix getM11(Matrix M1);
    Matrix getM22(Matrix M2);
    Matrix getM12(Matrix M1,Matrix M2);
    Matrix getm1(Matrix M1, Matrix L1);
    Matrix getm2(Matrix M2,Matrix L1);

    //Calculo das aproximaÃ§Åes para as coordenadas planimetricas dos pontos fotogrametricos
    Matrix getXYpf(Matrix M22, Matrix m2, Matrix M12, Matrix paf);
    //Calculo das aproximaÃ§Åes para as coordenadas Z dos pontos fotogrametricos
    double getZpf();

    Matrix getPTA(Matrix PAf,int imageId);

//chute dos parametros iniciais
    double getKappaZero(Matrix pta,int imageId);

    //passagem dos valores calculados para a matrix Z das coordenadas de terreno
    Matrix getPZ(double zpf);

    /** CÃ¡lculo dos parÃ¢metros de transformaÃ§Äo entre o espaÃ§o imagem e o espaÃ§o objeto
 para todas as imagens do bloco*/
    Matrix getPAf(Matrix M11,Matrix M12, Matrix M22,Matrix m1, Matrix m2);

	void setAFP();
    Matrix getAFP();


    Matrix matAdjust;
    Matrix getL0();
    Matrix getLb();
    Matrix getMatRes();





protected:
	Matrix A1,A2,P,Lb,L0,x1,x2,matRes;
	Matrix inverseN22;
    int totalIterations;

    //Seta a matrix baseado nos angulos
    void setRot(double omega, double phi, double kappa);
    double r11,r12,r13,r21,r22,r23,r31,r32,r33;

    //Devolve a matrix previamente setada
    Matrix getRot();

    //Seta as equaÃ§Åes baseado nos parametros passados
   // void setEquationsColinear(Matrix parameters);
    //Devolve coordenadas(xsi, eta) pelas equaÃ§Åes colinearidades previamente setada
    Matrix getCoordinatesEqColin(double X, double Y, double Z, int imageId);
    Matrix getJacobianaControl(double X, double Y, double Z,int imageId);
    Matrix getJacobianaFotogrametric(double X, double Y, double Z, int imageId);

    void createA1();
    void createA2();
    void createLb();
    void createL0();

    Matrix getN11();
    Matrix getN22();
    Matrix getN12();
    Matrix getn1(Matrix L1);
    Matrix getn2(Matrix L1);

    void setPeso(Matrix p);
    Matrix getPeso();

	//Matrix invertN11(Matrix N11);
	//Matrix invertN22(Matrix N22);
    Matrix getx1(Matrix N11,Matrix N12,Matrix N22,Matrix n1,Matrix n2);
	Matrix getx2(Matrix N12, Matrix N22, Matrix n2);//, Matrix x1);

	//void setx1(Matrix x1);
	//void setx2(Matrix x2);


    double getdOmegax1(int imageId);
    double getdPhix1(int imageId);
    double getdKappax1(int imageId);
    double getdXx1(int imageId);
    double getdYx1(int imageId);
    double getdZx1(int imageId);

    double getOmegaAdjus(int imageId);
    double getPhiAdjus(int imageId);
    double getKappaAdjus(int imageId);
    double getXAdjus(int imageId);
    double getYAdjus(int imageId);
    double getZAdjus(int imageId);


    double getdXx2(int fotogrPointId);
    double getdYx2(int fotogrPointId);
    double getdZx2(int fotogrPointId);

    void updateMatAdjust();


    Matrix getCoordColinearTerrain(double xsi, double eta, double z, int imageId);


   /* Em Teste; Bundle com objetos*/
public:
	deque<Image*> listImages;
	deque<Point*> listPoints;

	deque<Point*> listControlPoints;
	deque<Point*> listPhotogrammetricPoints;

	bool converged;
	bool userInitialValues;

	BundleAdjustment(deque<Image*>listSelectedImages, deque<Point*> listSelectedPoints, int flightDirection);

	void fillAnalogCoordinates();

	//conversao de espaço objeto e espaço imagem

	Matrix analogToDigital(InteriorOrientation *oi,double xsi, double eta);
	Matrix digitalToAnalog(InteriorOrientation *oi,int linha, int coluna);

	void getInicialsValues();
	void setUserInitialValues(Matrix initialValues);

	Matrix imageToMatrixAnalogicalCoordenates(Image *img);
	Matrix imageToMatrixJacobiana(int indexImage);
	Matrix imagesToMatrixesJacobianas(deque<Image*> listImgs);


	bool calculate();
	bool isPossibleCalculate();
	Matrix createL();
	Matrix createM1();
	Matrix createM2();

	double getInicialZPhotogrammetricPoints();
	double getMediaScale(int imageIndex);//calcula o denominador da escala media da imagem
	void updateCoordinatesAllPoints(Matrix xypf,double zphotogrammetric);

	//Esses metodos tem como referencia a ordem deles na LISTA DE PONTOS INDEXADOS DE CADA IMAGEM e nao na lista do xml
	bool isPhotogrammetricPoint(int imageIndex, int pointIndex);
	bool isControlPoint(int imageIndex, int pointIndex);
	bool isCheckingPoint(int imageIndex, int pointIndex);
	bool isConverged();

	int testConverged();
	bool testResiduo();
	//

	Point *getPointFrom(int imageIndex,int pointIndex);

	int numberControlPoints(Image *img);
	int numberPhotogrammetricPoints(Image *img);
	int whereInPoints(Point *pnt);
	int whereInPhotogrammetricPoints(Point *pnt);

	void zeroingCoordinatesPhotogrammetrics();

	double getRx(Image *img, int pointId);
	double getRy(Image *img, int pointId);
	bool getConvergencyStatus();
	int getTotalIterations();
	void setMaxNumberIterations(int value);
	int getMaxNumberIterations();
	void setMetricConvergencyValue(double value);
	void setAngularConvergencyValue(double value);
	double getMetricConvergencyValue();
	double getAngularConvergencyValue();
	Matrix getMVC();
	void calculateResiduos();
	void calculatePeso();
	void updateCoordFotog();

	//Matrix getSparseN11();
	void setInverseN22(Matrix n22);
	//imprime todas as imagens com seus pontos
	string printAll();

	/** Metodos Eigen*/
//	Matrix convertEigenToMatrix(Eigen::MatrixXd eigen);
//	Matrix convertVectorEigenToMatrix(Eigen::VectorXd eigen);
//	Eigen::MatrixXd convertMatrixToEigen(Matrix mat);

};

#endif // BUNDLEADJUSTMENT_H
