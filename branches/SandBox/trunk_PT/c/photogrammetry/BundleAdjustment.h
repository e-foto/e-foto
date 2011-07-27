#ifndef BUNDLEADJUSTMENT_H
#define BUNDLEADJUSTMENT_H

#include "Matrix.h"
#include <stdlib.h>
#include <deque>
#include <math.h>

#include "Sensor.h"
#include "Image.h"
#include "Point.h"
//#include ""

//using namespace std;
class BundleAdjustment
{

/* Matriz de configuraĂ§ĂŁo do bloco. Valor 1 - ponto de controle, valor -1, ponto fotogramĂ� trico, valor 0, ponto nĂŁo
contido na imagem. linha = imagem, coluna = ponto.
*/
/*int blc[3][6];
Matrix BLC;*/

protected:

// Matrizes de coordenadas dos pontos(E N H) e de pixel(Col Lin), na seguinte conformaçao, para cada coordenada ENH:
//      #  --> pontos de controle
//      -1 --> pontos fotogramétricos
//           p1   p2    p3    p4 ....
//      img1 -1    #    -1    #
//      img2 -1    #    -1    #
//       .
//       .
    Matrix X, Y, Z , Col, Lin;
/* Se fosse passado matrizes(deque<Matrix>) de pontos, a BundleAdjustment teria que saber que ponto está em que imagem,
*  e qual a coordenada em pixel daquele ponto para aquela imagem, ou seja, acesso ao xml,
*  portanto é dever da PTManager criar as matrizes X Y Z Col Lin
*/

    // Matriz blc: essa matriz é responsavel por dizer se ponto está na imagem, se é de controle ou fotogramétrico
//  0 --> ponto năo está na imagem
//  1 --> ponto de controle
// -1 --> ponto fotogramétrico
//           p1   p2    p3    p4 ....
//      img1 -1    0    0     1
//      img2 -1    1    -1    1
//      img3 -1    1    0     0
//       .
//       .
    Matrix blc;

// Matriz ois: onde cada linha tem os seis parametros de uma imagem: com a seguinte conformaçăo
//
//           a0   a1   a2    b0    b1    b2
//      img1 #    #    #     #     #     #
//      img2 #    #    #     #     #     #
//       .
//       .
    Matrix ois;
    double c;     // Distancia focal calibrada
    int fliDir;   // Marca fiducial com a direçao de vôo
    //Coordenadas do ponto principal das imagens em milimetros
    double xsi0,eta0;
    int numEquations;
    int numUnknows;
    int numImages;
    int numPoints;
    int numControlPoints;
    int numFotogrametricPoints;


    Matrix afp; //Matriz com resultados finais
/* Variaveis auxiliares */
    Matrix Lba;

    Matrix cXsi;
    Matrix cEta;

    Matrix rot;
    Matrix JacCtrl;
    Matrix JacFoto;

    Sensor* sensor;
    bool done;

public:
    BundleAdjustment();
    BundleAdjustment(Matrix x, Matrix y, Matrix z, Matrix col, Matrix lin, Matrix OIs, Matrix BLC, Sensor * sensor, int flightDirection);
    //pnts[0]săo os pontos de controle e pnts[1] săo os pontos fotogrametricos;
    void pointsToAdjustament();
    int numberOfEquations();

    bool calculate();

    double* digitalToAnalog(Matrix Xa,int linha, int coluna,int indexImg);
    int* analogToDigital(Matrix Xa, double xsi, double eta, int indexImg);

    //metodos para debug
    void imprime(Matrix mat, char *id);

// metodos para fototri

// monta a matrix das observaçőes
    Matrix createL(Matrix X2, Matrix Y2);

    void createcXsicEta();

//funçőes para a montagem das matrizes para o ajustamento
    Matrix createMl();
    Matrix createM2();

    Matrix getM11(Matrix M1);
    Matrix getM22(Matrix M2);
    Matrix getM12(Matrix M1,Matrix M2);
    Matrix getm1(Matrix M1, Matrix L1);
    Matrix getm2(Matrix M2,Matrix L1);

    //Calculo das aproximaçőes para as coordenadas planimetricas dos pontos fotogrametricos
    Matrix getXYpf(Matrix M22, Matrix m2, Matrix M12, Matrix paf);
    //Calculo das aproximaçőes para as coordenadas Z dos pontos fotogrametricos
    double getZpf();

    Matrix getPTA(Matrix PAf,int imageId);

//chute dos parametros iniciais
    double getKappaZero(Matrix pta,int imageId);

//matrix das aproximaçőes iniciais O
    void getInicialsValues();

//retorna a matriz com os indices dos 1şs pontos de controle de cada imagem
    Matrix firstPntCtrl();
//calcula o denominador da escala media da imagem
    double getMediaScale(int imageId);

    //passagem dos valores calculados para a matrix X das coordenadas de terreno
    Matrix getPX(Matrix xypf);
    //passagem dos valores calculados para a matrix Y das coordenadas de terreno
    Matrix getPY(Matrix xypf);
    //passagem dos valores calculados para a matrix Z das coordenadas de terreno
    Matrix getPZ(double zpf);

    /** Cálculo dos parâmetros de transformaçăo entre o espaço imagem e o espaço objeto
 para todas as imagens do bloco*/
    Matrix getPAf(Matrix M11,Matrix M12, Matrix M22,Matrix m1, Matrix m2);

    void setAFP(Matrix aFP);
    Matrix getAFP();


    Matrix matAdjust;
    Matrix getL0();
    Matrix getLb();
    Matrix getMatRes();
 /** Matriz de rotaçőes
  */
    /**retirar*/
    Matrix N11,N12,N22;

protected:
    Matrix A1,A2,P,Lb,L0,x1,x2,matRes;
    int totalIterations;

    //Seta a matrix baseado nos angulos
    void setRot(double omega, double phi, double kappa);
    double r11,r12,r13,r21,r22,r23,r31,r32,r33;

    //Devolve a matrix previamente setada
    Matrix getRot();

    //Seta as equaçőes baseado nos parametros passados
   // void setEquationsColinear(Matrix parameters);
    //Devolve coordenadas(xsi, eta) pelas equaçőes colinearidades previamente setada
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

    Matrix invertN11(Matrix N11);
    Matrix invertN22(Matrix N22);
    Matrix getx1(Matrix N11,Matrix N12,Matrix N22,Matrix n1,Matrix n2);
    Matrix getx2(Matrix N12, Matrix N22, Matrix n2, Matrix x1);

    void setx1(Matrix x1);
    void setx2(Matrix x2);


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
    void updateCoordFotog();
    bool isConverged();

    int numberCntrlPnts(int imageId);
    int numberFtgPnts(int imageId);


    Matrix getCoordColinearTerrain(double xsi, double eta, double z, int imageId);
    Matrix calculateResiduos();
    bool testResiduo();
    void calculatePeso();

    //Retorna os residuos nas coordenadas no ponto 'pointId' da imagem 'imageId'
    double getRx(int imageId, int pointId);
    double getRy(int imageId, int pointId);


   /* Em Teste*/
    Matrix imageToMatrix(Image* img);
    Matrix imagesToMatrixes(deque<Image*> listImgs);
    void putPointsInImage(Image *img,deque<Point*> listPoints);

};

#endif // BUNDLEADJUSTMENT_H
