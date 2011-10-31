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

/**
* \file BundleAdjustment.h
* \class BundleAdjustment
* \brief Classe que calcula os parametros das orientações exteriores de todas as imagens de um bloco
* \author Paulo André Batista Pupim e Rafael Aguiar
*/
//using namespace std;
class BundleAdjustment
{

/* Matriz de configuraÃÂÃÂ§ÃÂÃÂo do bloco. Valor 1 - ponto de controle, valor -1, ponto fotogramÃÂÃ trico, valor 0, ponto nÃÂÃÂo
contido na imagem. linha = imagem, coluna = ponto.
*/
/*int blc[3][6];
Matrix BLC;*/

protected:

    double c;     // Distancia focal calibrada
	//Coordenadas do ponto principal das imagens em milimetros
    double xsi0,eta0;
    int numEquations;
    int numUnknows;
    int numImages;
    int numPoints;
    int numControlPoints;
    int numFotogrametricPoints;

	int maxIterations;
	int totalIterations;
	double metricConvergency, angularConvergency;

	double r11,r12,r13,r21,r22,r23,r31,r32,r33;
	Matrix A1,A2,P,Lb,L0,x1,x2,matRes;
	Matrix matAdjust, matInicialValues;
	Matrix inverseM11,inverseN11, inverseN22;
	Matrix n11,n12,n22,n1,n2;
    Matrix afp; //Matriz com resultados finais
/* Variaveis auxiliares */

    Sensor* sensor;
	deque<Image*> listImages;
	deque<Point*> listPoints;

	deque<Point*> listControlPoints;
	deque<Point*> listPhotogrammetricPoints;

	bool done;
	bool converged;
	bool userInitialValues;

public:
	/**
	* \brief Construtor padrão
	* \attention NUNCA use esse construtor para calculo, futuramente será eliminado
	*/
    BundleAdjustment();

	/**
	* \brief Construtor basico para o calculo
	* \param listSelectedimages : Lista com todos as imagens que serão consideradas no calculo
	* \param listSelectedPoints : Lista com todos os pontos que serão consideradas no calculo
	*/
	BundleAdjustment(deque<Image*>listSelectedImages, deque<Point*> listSelectedPoints);

	/**
	* \brief Retorna o numero de equações que o calculo atual tem que resolver
	* \return int : Numero de equações
	*/
	int numberOfEquations();

	/**
	* \brief Metodo para debug imprime no terminal matriz com uma label
	* \deprecated A Classe Matrix já possui metodo parecido, a saber, Matrix::show()
	* \param mat : Matriz a ser exibida no terminal
	* \param id  : Texto do label
	*/
    void imprime(Matrix mat, char *id);

	/**
	* \brief Calcula a matriz M11
	* \param    M1   : Matriz formada pelas coordenadas analogicas de todos os pontos
	* \return Matrix : Matriz M11, seguindo metodo de Boltz
	*/
    Matrix getM11(Matrix M1);

	/**
	* \brief Calcula a matriz M11
	* \param    M2   : Matriz formada por submatrizes para cada ponto fotogramétrico
	* \return Matrix : Matriz M22, seguindo metodo de Boltz
	*/
    Matrix getM22(Matrix M2);

	/**
	* \brief Calcula a matriz M12
	* \param    M1   : Matriz formada pelas coordenadas analogicas de todos os pontos
	* \param    M2   : Matriz formada por submatrizes para cada ponto fotogramétrico
	* \return Matrix : Matriz M12, seguindo metodo de Boltz
	*/
    Matrix getM12(Matrix M1,Matrix M2);

	/**
	* \brief Calcula a matriz m1, necessaria para o calculo inicial dos parametros da orientaçoes exteriores
	* \param    M1   : Matriz formada pelas coordenadas analogicas de todos os pontos
	* \param    L1   : Matriz formada pelas coordenadas X e Y dos pontos de controle
	* \return Matrix : Matriz m1
	*/
    Matrix getm1(Matrix M1, Matrix L1);

	/**
	* \brief Calcula a matriz m1, necessaria para o calculo inicial dos parametros da orientaçoe exteriores
	* \param    M2   : Matriz formada por submatrizes para cada ponto fotogramétrico
	* \param    L1   : Matriz formada pelas coordenadas X e Y dos pontos de controle
	* \return Matrix : Matriz m1
	*/
    Matrix getm2(Matrix M2,Matrix L1);

	/**
	* \brief Metodo que retorna a matriz com as primeiras aproximações dos parametros da orientações exteriores
	* \param    M12  : Matriz M12
	* \param    m1   : Matriz m1
	* \param    xypf : Matriz xypf, matriz com as primeiras aproximaçoes das coordenadas dos pontos fotogramétricos
	* \return Matrix : Matriz paf, matriz com as primeiras aproximaçoes dos parametros da orientaçao exterior
	*/
	Matrix getPAf(Matrix M12,Matrix m1, Matrix xypf);

	/**
	* \brief Metodo que retorna a matriz com as primeiras aproximações das coordernadas dos pontos fotogramétricos
	* \param    M22  : Matriz M22
	* \param    m2   : Matriz m2
	* \param    M12  : Matriz M12
	* \param    paf  : Matriz paf que contem os valores iniciais dos parametros da orientaçao exterior
	* \return Matrix : Matriz xypf, matriz com as primeiras aproximaçoes das coordenadas dos pontos fotogramétricos
	*/
	Matrix getXYpf(Matrix M22, Matrix m2, Matrix M12, Matrix paf);

	/**
	* \deprecated O chute inicial dos valores de Z, atualmente, são calculados baseado no formulario de Flight
	*/
	double getZpf();

	/**
	* \brief Metodo que retorna apenas os parametros da orientaçao exterior inicias de uma imagem
	* \param   Paf   : Matriz com todos os parametros da orientaçao exterior de todas as imagens
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return Matrix : Matriz com os parametros da orientaçao exterior de uma imagem
	*/
    Matrix getPTA(Matrix PAf,int imageId);

//chute dos parametros iniciais
	/**
	* \deprecated O chute inicial dos valores de kappa, atualmente, são feitos pelo usuario na interface antes do calculo ser disparado
	*/
    double getKappaZero(Matrix pta,int imageId);

    //passagem dos valores calculados para a matrix Z das coordenadas de terreno
   // Matrix getPZ(double zpf);

	/**
	* \brief Altera a versão final(AFP) da matriz de ajustamento(matAdjust) dos parametros das orientações exteriores
	*/
	void setAFP();

	/**
	* \brief Metodo que retorn a versão final da matriz de ajustamento(matAdjust) dos parametros das orientações exteriores
	* \return Matrix : Matrix AFP que contem todos os parametros de todas as imagens com omega, phi e kappa em graus
	*/
    Matrix getAFP();

	/**
	* \brief Metodo que retorna uma matriz com as coordenadas analogicas dos pontos calculadas pelas equações de colinearidades
	* \return Matrix : Retorna a matriz L0
	*/
    Matrix getL0();

	/**
	* \brief Metodo que retorna uma matriz com as coordenadas analogicas dos pontos calculadas por orientaçao interior
	* \return Matrix : Retorna a matriz Lb
	*/
	Matrix getLb();

	/**
	* \brief Metodo que retorna a matriz dos residuos da ultima iteração
	* \return Matrix : Retorna a matriz dos residuos
	*/
    Matrix getMatRes();



protected:
    //Seta a matrix baseado nos angulos
	/**
	* \brief Altera os valores da matriz de rotação
	* \param omega : Angulo omega em radianos
	* \param phi   : Angulo phi em radianos
	* \param kappa : Angulo kappa em radianos
	* \attention os valores da matriz de rotaçao estão contidos nos atributos double : r11,r12,r13,r21,r22,r23,r31,r32 e r33
	*/
    void setRot(double omega, double phi, double kappa);

	/**
	* \brief Metodo que retorn os valores xi e eta de uma coordenada X Y Z contida numa imagem atraves das equações de colinearidade
	* \param    X    : Valor da coordenada no eixo X
	* \param    Y    : Valor da coordenada no eixo Y
	* \param    Z    : Valor da coordenada no eixo Z
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return Matrix : Matriz com as coordenadas xi e eta
	*/
    Matrix getCoordinatesEqColin(double X, double Y, double Z, int imageId);

	/**
	* \brief Metodo que retorn a matriz Jacobiana de uma coordenada X Y Z de um ponto de Controle
	* \param    X    : Valor da coordenada no eixo X
	* \param    Y    : Valor da coordenada no eixo Y
	* \param    Z    : Valor da coordenada no eixo Z
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return Matrix : Matriz Jacobiana da coordenada
	*/
    Matrix getJacobianaControl(double X, double Y, double Z,int imageId);

	/**
	* \brief Metodo que retorn a matriz Jacobiana de uma coordenada X Y Z de um ponto de fotogramétrico
	* \param    X    : Valor da coordenada no eixo X
	* \param    Y    : Valor da coordenada no eixo Y
	* \param    Z    : Valor da coordenada no eixo Z
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return Matrix : Matriz Jacobiana da coordenada
	*/
    Matrix getJacobianaFotogrametric(double X, double Y, double Z, int imageId);

	/**
	* \brief Cria ou altera o atributo Matrix A1 que é a matriz formada pelas matrizes jacobianas de todos os pontos como se de controle fossem
	*/
    void createA1();

	/**
	* \brief Cria ou altera o atributo Matrix A2 que é a matriz formada pelas matrizes jacobianas apenas dos pontos fotogramétricos
	*/
    void createA2();

	/**
	* \brief Metodo que cria ou altera uma matriz com as coordenadas analogicas dos pontos calculadas por orientaçao interior
	*/
    void createLb();

	/**
	* \brief Metodo que cria ou altera uma matriz com as coordenadas analogicas dos pontos calculadas pelas equações de colinearidades
	*/
    void createL0();

	/**
	* \brief Calcula a matriz N11 utilizando a matriz A1
	* \return Matrix : Matriz N11, seguindo metodo de Boltz
	*/
    Matrix getN11();

	/**
	* \brief Calcula a matriz N22 utilizando a matriz A2
	* \return Matrix : Matriz N22, seguindo metodo de Boltz
	*/
    Matrix getN22();

	/**
	* \brief Calcula a matriz N12 utilizando a matriz A1 e A2
	* \return Matrix : Matriz N12, seguindo metodo de Boltz
	*/
    Matrix getN12();

	/**
	* \brief Calcula a matriz n1, necessaria para o calculo do ajustamento dos parametros da orientaçao exterior
	* \param    l    : Matriz diferença entre Lb e L0
	* \return Matrix : Matriz n1
	*/
	Matrix getn1(Matrix l);

	/**
	* \brief Calcula a matriz n2, necessaria para o calculo do ajustamento dos parametros da orientaçao exterior
	* \param    l    : Matriz diferença entre Lb e L0
	* \return Matrix : Matriz n2
	*/
	Matrix getn2(Matrix l);

	/**
	* \brief Calcula e altera a matriz de pesos
	* \param p : Matriz de pesos
	* \attention Atualmente a matriz de pesos É SEMPRE a matriz identidade no calculo dos parametros da orientaçao exterior
	*/
    void setPeso(Matrix p);

	/**
	* \brief Retorna a matriz de pesos atual
	* \attention Atualmente a matriz de pesos É SEMPRE a matriz identidade no calculo dos parametros da orientaçao exterior
	*/
    Matrix getPeso();

	//Matrix invertN11(Matrix N11);
	//Matrix invertN22(Matrix N22);
	/**
	* \brief Calcula e altera a matrix x1 a qual é a matriz que contem os "deltas" a serem somados nos parametros da orientaçao exterior
	* \param    N12  : Matriz N12
	* \param    n1   : Matriz n1
	*/
	void setx1(Matrix N12,Matrix n1);

	/**
	* \brief Calcula e altera a matrix x2 a qual é a matriz que contem os "deltas" a serem somados nas coordenadas dos pontos fotogramétricos
	* \param    N12  : Matriz N12
	* \param    N22  : Matriz N22
	* \param    n2   : Matriz n2
	* \param    n1   : Matriz n1
	*/
	void setx2(Matrix N12, Matrix N22, Matrix n2, Matrix n1);

	/**
	* \brief Metodo auxiliar que retorna o valor(delta) de ajustamento do angulo Omega contido na matriz x1
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor delta do angulo Omega contido na matriz x1 previamente calculada
	*/
    double getdOmegax1(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor(delta) de ajustamento do angulo Phi contido na matriz x1
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor delta do angulo Phi contido na matriz x1 previamente calculada
	*/
	double getdPhix1(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor(delta) de ajustamento do angulo Kappa contido na matriz x1
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor delta do angulo Kappa contido na matriz x1 previamente calculada
	*/
    double getdKappax1(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada X0 contida na matriz x1
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor delta da coordenada X0 contida na matriz x1 previamente calculada
	*/
	double getdXx1(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada Y0 contida na matriz x1
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor delta da coordenada Y0 contida na matriz x1 previamente calculada
	*/
    double getdYx1(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada Z0 contida na matriz x1
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor delta da coordenada Z0 contida na matriz x1 previamente calculada
	*/
    double getdZx1(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor de Omega na matriz de ajustamento
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor de Omega contido na Matrix matAdjust
	*/
    double getOmegaAdjus(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor de Phi na matriz de ajustamento
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor de Phi contido na Matrix matAdjust
	*/
	double getPhiAdjus(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor de Kappa na matriz de ajustamento
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor de Kappa contido na Matrix matAdjust
	*/
	double getKappaAdjus(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor da coordenada X0 na matriz de ajustamento
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor da coordenada X0 contida na Matrix matAdjust
	*/
	double getXAdjus(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor da coordenada Y0 na matriz de ajustamento
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor da coordenada Y0 contida na Matrix matAdjust
	*/
	double getYAdjus(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor da coordenada Z0 na matriz de ajustamento
	* \param imageId : id da imagem no calculo atual
	* \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
	* \return double : Valor da coordenada Z0 contida na Matrix matAdjust
	*/
	double getZAdjus(int imageId);

	/**
	* \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada X do ponto fotogramétrico contida na matriz x2
	* \param fotogrPointId : id da imagem no calculo atual
	* \attention o fotogrPointId NÃO é a key ou pointId do ponto na ProjectManager e sim a posiçao na lista de pontos em que ele se encontra
	* \return double : Valor delta da coordenada X do ponto contida na matriz x2 previamente calculada
	*/
    double getdXx2(int fotogrPointId);

	/**
	* \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada Y do ponto fotogramétrico contida na matriz x2
	* \param fotogrPointId : id da imagem no calculo atual
	* \attention o fotogrPointId NÃO é a key ou pointId do ponto na ProjectManager e sim a posiçao na lista de pontos em que ele se encontra
	* \return double : Valor delta da coordenada Y do ponto contida na matriz x2 previamente calculada
	*/
	double getdYx2(int fotogrPointId);

	/**
	* \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada Z do ponto fotogramétrico contida na matriz x2
	* \param fotogrPointId : id da imagem no calculo atual
	* \attention o fotogrPointId NÃO é a key ou pointId do ponto na ProjectManager e sim a posiçao na lista de pontos em que ele se encontra
	* \return double : Valor delta da coordenada Z do ponto contida na matriz x2 previamente calculada
	*/
	double getdZx2(int fotogrPointId);

	/**
	* \brief Metodo que atualiza os valores dos parametros das orientações exteriores utilizando os valores contidos na matriz x1
	*/
    void updateMatAdjust();

 // Em duvida sobre esse brief
	/**
	* \brief Metodo que retorna um matriz com as coordenadas de terreno pelas equações de interseção espacial
	* \param    xsi  : Coordenada analogica xi em milimetros
	* \param    eta  : Coordenada analogica eta em milimetros
	* \param    z    : Coordenada z  em metros
	* \return Matrix : Matriz que contém as coordenadas de terreno X e Y
	*/
    Matrix getCoordColinearTerrain(double xsi, double eta, double z, int imageId);


   /* Em Teste; Bundle com objetos*/
public:

	/**
	* \brief Metodo que preenche os objetos Point com as coordenadas analogicas baseado na orientaçao interior
	*/
	void fillAnalogCoordinates();

	/**
	* \brief Metodo que converte coordenada analogica(xi,eta) em coordenada digital(col,lin)
	* \param oi  : Orientação interior da imagem em que a coordenada está
	* \param xsi : Coodernada analogica xi
	* \param eta : Coodernada analogica eta
	* \deprecated Esse metodo logo será substituido pelo existente na classe /*nome da classe MOTOR DE FOTOGRAMETRIAm*
	*/
	Matrix analogToDigital(InteriorOrientation *oi,double xsi, double eta);

	/**
	* \brief Metodo que converte coordenada digital(col,lin) em coordenada analogica(xi,eta)
	* \param oi  : Orientação interior da imagem em que a coordenada está
	* \param linha : Coodernada analogica linha
	* \param coluna : Coodernada analogica coluna
	* \deprecated Esse metodo logo será substituido pelo existente na classe /*nome da classe MOTOR DE FOTOGRAMETRIAm*
	*/
	Matrix digitalToAnalog(InteriorOrientation *oi,int linha, int coluna);

	/**
	* \brief Metodo que calcula os parametros iniciais das orientaçoes exteriores
	*/
	void calculateInicialsValues();

	/**
	* \brief Altera a matriz dos parametros iniciais das orientações exteriores para aqueles fornecido pelo usuário na interface
	* \param initialValues : Matriz com os parametros iniciais, com uma imagem por linha na sequencia: Omega, Phi, Kappa, X0, Y0, Z0
	*/
	void setUserInitialValues(Matrix initialValues);

	/**
	* \brief Metodo auxiliar que retorna uma matriz correspondente a uma imagem na matriz M1
	* \param img    : Objeto imagem contendos os pontos pertencentes a ela
	* \param Matrix : Matriz correspondente a uma imagem
	*/
	Matrix imageToMatrixAnalogicalCoordenates(Image *img);

	/**
	* \brief Metodo auxiliar que retorna uma matriz correspondente a uma imagem na matriz A1
	* \param img    : Objeto imagem contendos os pontos pertencentes a ela
	* \param Matrix : Matriz correspondente a uma imagem
	*/
	Matrix imageToMatrixJacobiana(int indexImage);

	/**
	* \brief Metodo responsavel por todo o calculo do ajustamento por Bundle Adjustment
	* \return bool: Informa se o calculo foi bem sucedido ou não
	*/
	bool calculate();

	/**
	* \brief Metodo que informa se é possivel calcular o ajustamento ou não, baseado no numero de incognitas e equações
	* \return bool : Informa se é possivel ou não calcular
	*/
	bool isPossibleCalculate();

	/**
	* \brief Metodo que cria ou altera a matriz L
	* \return Matrix : retorna a matriz formada pelas coordenadas X e Y dos pontos de controle
	*/
	Matrix createL();

	/**
	* \brief Metodo que cria ou altera a matriz M1
	* \return Matrix : retorna a matriz formada M1 necessária para o calculo inicial dos parametros
	*/
	Matrix createM1();

	/**
	* \brief Metodo que cria ou altera a matriz M2
	* \return Matrix : retorna a matriz formada M2 necessária para o calculo inicial dos parametros
	*/
	Matrix createM2();

	/**
	* \brief Metodo auxiliar que calcula a coordenada incial Z para cada ponto fotogramétrico
	* \return double : Valor inicial da coordenada Z dos pontos fotogramétricos
	*/
	double getInicialZPhotogrammetricPoints();

	/**
	* \brief Metodo que calcula a escala de cada imagem
	* \deprecated Metodo em desuso, atualmente esse valor é pego do formulario de Flight
	* \param imageIndex : indice da imagem na matriz de ajustamento
	* \return double : denominador da escala da imagem
	*/
	double getMediaScale(int imageIndex);

	/**
	* \brief Metodo que atualiza as coordenadas iniciais de todos os pontos fotogramétricos baseada na matriz xypf
	* \param xypf : Matriz contendo as coordenadas de pontos fotogramétricos
	* \param zphotogrammetric : Valor de Z da coordenada de ponto fotogramétrico
	*/
	void updateCoordinatesAllPoints(Matrix xypf,double zphotogrammetric);

	//Esses metodos tem como referencia a ordem deles na LISTA DE PONTOS INDEXADOS DE CADA IMAGEM e nao na lista do xml
	/**
	* \brief Metodo auxiliar que testa se determinado ponto contido em determinada imagem é fotogramétrico
	* \param imageIndex : Posiçao da imagem na lista de imagens
	* \param pointIndex : Posiçao do ponto na lista de pontos da imagem informada pelo parametro imageIndex
	* \return bool : informa se o ponto é fotogramétrico ou não
	*/
	bool isPhotogrammetricPoint(int imageIndex, int pointIndex);

	/**
	* \brief Metodo auxiliar que testa se determinado ponto contido em determinada imagem é de controle
	* \param imageIndex : Posiçao da imagem na lista de imagens
	* \param pointIndex : Posiçao do ponto na lista de pontos da imagem informada pelo parametro imageIndex
	* \return bool : informa se o ponto é controle ou não
	*/
	bool isControlPoint(int imageIndex, int pointIndex);

	/**
	* \brief Metodo auxiliar que testa se determinado ponto contido em determinada imagem é de 'checking'
	* \param imageIndex : Posiçao da imagem na lista de imagens
	* \param pointIndex : Posiçao do ponto na lista de pontos da imagem informada pelo parametro imageIndex
	* \return bool : informa se o ponto é de 'checking' ou não
	*/
	bool isCheckingPoint(int imageIndex, int pointIndex);

	/**
	* \brief Metodo que informa se o calculo convergiu ou não baseado no numero de iteraçoes e no valor de convergencia estipulado pelo usuário
	* \return bool : informa se o calculo convergiu ou não
	*/
	bool isConverged();

	/**
	* \brief Metodo que analisa se a matriz x1 e x2 atingiram sua convergencia
	* \return int : Codigo para a analise da convergencia do calculo, se o calculo convergiu retorna 1, se nao convergiu e não houve problemas nas matrizes retorna 0, se houver algum NAN na matriz retorna -1, se houver algum valor INF retorna -2
	*/
	int testConverged();

	/**
	* \brief Metodo que analisa se a matriz de residuos atingiram sua convergencia
	* \return bool : informa se os residuos convergiram ou não
	* \deprecated Atualmente esse teste não é feito pois não há iteraçao sobre a matriz de pesos P
	*/
	bool testResiduo();

	/**
	* \brief Metodo auxiliar que retorna um ponteiro para o ponto na imagem informada
	* \param imageIndex : Posiçao da imagem na lista de imagens
	* \param pointIndex : Posiçao do ponto na lista de pontos da imagem informada pelo parametro imageIndex
	* \return Point : informa se os residuos convergiram ou não
	*/
	Point *getPointFrom(int imageIndex,int pointIndex);

	/**
	* \brief Metodo auxiliar que informa o numero de pontos de controle contidos numa imagem
	* \param img : Objeto imagem a qual se deseja saber o numero de pontos de controle
	* \return int : Numero de pontos de controle
	*/
	int numberControlPoints(Image *img);

	/**
	* \brief Metodo auxiliar que informa o numero de pontos fotogramétricos contidos numa imagem
	* \param img : Objeto imagem a qual se deseja saber o numero de pontos fotogramétricos
	* \return int : Numero de pontos fotogramétricos
	*/
	int numberPhotogrammetricPoints(Image *img);

	/**
	* \brief Metodo auxiliar que informa a posiçao de determinado ponto na lista de todos os pontos
	* \param pnt : Objeto ponto a qual se deseja saber sua posiçao na lista
	* \return int : Posiçao na lista, se o ponto não estiver na lista retorna -1;
	*/
	int whereInPoints(Point *pnt);

	/**
	* \brief Metodo auxiliar que informa a posiçao de determinado ponto na lista de pontos fotogramétricos
	* \param pnt : Objeto ponto a qual se deseja saber sua posiçao na lista
	* \return int : Posiçao na lista, se o ponto não estiver na lista retorna -1;
	*/
	int whereInPhotogrammetricPoints(Point *pnt);

	/**
	* \brief Metodo que zera todas as coordenadas de ponto fotogramétricos
	* \deprecated Metodo desnecessário tendo em vista que as coordenadas iniciais dos pontos fotogramétricos sao irrelevantes para o calculo
	*/
	void zeroingCoordinatesPhotogrammetrics();

	/**
	* \brief Metodo auxiliar que retorna o residuo na coordenada X de determinado ponto em determinada imagem
	* \param img : Objeto imagem em que o ponto está contido
	* \param pointId : Posiçao do ponto na lista de pontos pertencentes a essa imagem
	* \return double : Valor do residuo em X
	*/
	double getRx(Image *img, int pointId);

	/**
	* \brief Metodo auxiliar que retorna o residuo na coordenada Y de determinado ponto em determinada imagem
	* \param img : Objeto imagem em que o ponto está contido
	* \param pointId : Posiçao do ponto na lista de pontos pertencentes a essa imagem
	* \return double : Valor do residuo em Y
	*/
	double getRy(Image *img, int pointId);

	/**
	* \brief Metodo que retorna o estado atual de convergencia do calculo
	* \return bool : Estado atual da convergencia
	*/
	bool getConvergencyStatus();

	/**
	* \brief Metodo que retorna o total de iteraçoes até o momento
	* \return int : total de iteraçoes
	*/
	int getTotalIterations();

	/**
	* \brief Metodo que altera o numero maximo de iterações
	* \param value : Numero maximo de iterações
	*/
	void setMaxNumberIterations(int value);

	/**
	* \brief Metodo que retorna o numero maximo de iterações
	* \return int : Numero maximo de iterações
	*/
	int getMaxNumberIterations();

	/**
	* \brief Metodo que altera o valor da convergencia das coordenadas dos parametros das orientações exteriores
	* \param value : Valor da convergencia das coordenadas
	*/
	void setMetricConvergencyValue(double value);

	/**
	* \brief Metodo que altera o valor da convergencia dos angulos dos parametros das orientações exteriores
	* \param value : Valor da convergencia dos angulos
	*/
	void setAngularConvergencyValue(double value);

	/**
	* \brief Metodo que retorna o valor da convergencia das coordenadas dos parametros das orientações exteriores
	* \return double : Valor de convergencia das coordenadas
	*/
	double getMetricConvergencyValue();

	/**
	* \brief Metodo que retorna o valor da convergencia dos angulos dos parametros das orientações exteriores
	* \return double : Valor de convergencia dos angulos
	*/
	double getAngularConvergencyValue();


	/**
	* \brief Metodo que retorna a matrix x1 formatada para apresentar os angulos em graus e cada orientaçao exterior em uma linha
	* \return Matrix : Matriz formatada
	*/
	Matrix getMVC();

	/**
	* \brief Metodo que calcula os residuos após a convergencia do calculo e altera o atributo matRes que guarda os residuos
	*/
	void calculateResiduos();

	/**
	* \brief Metodo que calcula a matriz de pesos após a convergencia do calculo e altera o atributo P que guarda os pesos
	*/
	void calculatePeso();

	/**
	*	\brief Metodo que atualiza, isto é, ajusta as coordenadas de pontos fotogramétricos baseado na matriz x2
	*/
	void updateCoordFotog();


	/**
	* \brief Metodo que inverte apenas as matrizes da diagonal principal da matriz m11, evitando assim calculos desnecessários
	* \param m11 : Matriz M11 a ser invertida
	*/
	void setInverseM11(Matrix m11);


	/**
	* \brief Metodo que inverte apenas as matrizes da diagonal principal da matriz n11, evitando assim calculos desnecessários
	* \param n11 : Matriz N11 a ser invertida
	*/
	void setInverseN11(Matrix n11);

	/**
	* \brief Metodo que inverte apenas as matrizes da diagonal principal da matriz n22, evitando assim calculos desnecessários
	* \param n22 : Matriz N22 a ser invertida
	*/
	void setInverseN22(Matrix n22);
	//imprime todas as imagens com seus pontos


	string printAll();

	/** Metodos Eigen*/
//	Matrix convertEigenToMatrix(Eigen::MatrixXd eigen);
//	Matrix convertVectorEigenToMatrix(Eigen::VectorXd eigen);
//	Eigen::MatrixXd convertMatrixToEigen(Matrix mat);


	/**
	* \brief Metodo auxiliar que retorna os residuos de um ponto fotogrametrico, se o ponto nao for fotogrametrico retorna uma matriz com os elementos zerados
	* \param photogrammetricPoint : Objeto ponto fotogrametrico
	* \return Matrix : Matriz[1x3] com os residuos dos pontos na sequencia X, Y, Z
	*/
	Matrix getResiduo(Point *photogrammetricPoint);


	/**
	* \brief Metodo que retorna a matriz com as aproximaçoes iniciais dos parametros de orientaçao exterior de todas as imagens
	* \return Matrix : Matrix com os parametros de orientaçao exterior
	*/
	Matrix getMatrixInicialValues();

	/**
	* \brief Metodo que retorna a lista de pontos fotogrametricos usados no calculo do ajustamento
	* \return deque<Point*> : Lista com os objetos Point dos pontos fotogrametricos
	*/
	deque<Point*> getPhotogrammetricList();

};

#endif // BUNDLEADJUSTMENT_H
