#ifndef BUNDLEADJUSTMENT_H
#define BUNDLEADJUSTMENT_H

#include "Matrix.h"
#include "stdlib.h"
#include <deque>
#include <math.h>
//#include ""

//using namespace std;
class BundleAdjustment
{

/* Matriz de configuraĂ§ĂŁo do bloco. Valor 1 - ponto de controle, valor -1, ponto fotogramĂŠtrico, valor 0, ponto nĂŁo
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

	Matrix afp; //Matriz com resultados finais
/* Variaveis auxiliares */
    Matrix Lba;

    Matrix cXsi;
    Matrix cEta;

    Matrix rot;
    Matrix JacCtrl;
    Matrix JacFoto;

    bool done;

public:
    BundleAdjustment();
    BundleAdjustment(Matrix x, Matrix y, Matrix z, Matrix col, Matrix lin, Matrix OIs, Matrix BLC,double focalCalibrada, int flightDirection);
    //pnts[0]săo os pontos de controle e pnts[1] săo os pontos fotogrametricos;
    int* pointsToAdjustament();
    int numberOfEquations(int images, int pnts);

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
    Matrix createMl(Matrix C,Matrix L);
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
    Matrix getInicialsValues(Matrix pfa);

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


/** Matriz de rotaçőes
  */
    //Seta a matrix baseado nos angulos
    Matrix setRot(double omega, double phi, double kappa);
    //Devolve a matrix previamente setada
    Matrix getRot();

    //Seta as equaçőes baseado nos parametros passados
    Matrix setEquationsColinear(Matrix parameters);
    //Devolve coordenadas(xsi, eta) pelas equaçőes colinearidades previamente setada
    Matrix getCoordinatesEqColin(double X, double Y, double Z);
    Matrix getJacobianaControl(Matrix parameters,double X, double Y, double Z);
    Matrix getJacobianaFotogrametric(Matrix parameters,double X, double Y, double Z);
    Matrix getA1();



	void setAFP(Matrix aFP);
	Matrix getAFP();
};

#endif // BUNDLEADJUSTMENT_H
