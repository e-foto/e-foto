#ifndef BUNDLEADJUSTMENT_H
#define BUNDLEADJUSTMENT_H
/*Copyright 2002-2021 e-foto team (UERJ)
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
* \brief Classe que calcula os parametros das orientações exteriores de todas as imagens de um bloco
*/

class BundleAdjustment {

    /* Matriz de configuração do bloco. Valor 1 - ponto de controle, valor -1, ponto
     * fotogramétrico, valor 0, ponto não contido na imagem. linha = imagem, coluna = ponto.
    */


protected:

    double c;     // Distancia focal calibrada
    //Coordenadas do ponto principal das imagens em milimetros
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
    Matrix afp; //Matriz com resultados finais

    Sensor* sensor;
    std::deque<Image*> listImages;
    std::deque<Point*> listPoints;

    std::deque<Point*> listControlPoints;
    std::deque<Point*> listPhotogrammetricPoints;

    std::deque<double> listRMSE;

    bool done;
    bool converged;
    bool userInitialValues;

public:
    /**
    * \brief Construtor basico para o calculo
    * \param listSelectedimages : Lista com todos as imagens que serão consideradas no calculo
    * \param listSelectedPoints : Lista com todos os pontos que serão consideradas no calculo
    */
    explicit BundleAdjustment(std::deque<Image*>listSelectedImages, std::deque<Point*> listSelectedPoints);

    /**
    * \brief Retorna o numero de equações que o calculo atual tem que resolver
    * \return int : Numero de equações
    */
    int numberOfEquations();

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
    Matrix getM12(Matrix M1, Matrix M2);

    /**
    * \brief Calcula a matriz m1, necessaria para o calculo inicial dos parametros da orientaÃ§oes exteriores
    * \param    M1   : Matriz formada pelas coordenadas analogicas de todos os pontos
    * \param    L1   : Matriz formada pelas coordenadas X e Y dos pontos de controle
    * \return Matrix : Matriz m1
    */
    Matrix getm1(Matrix M1, Matrix L1);

    /**
    * \brief Calcula a matriz m1, necessaria para o calculo inicial dos parametros da orientaÃ§oe exteriores
    * \param    M2   : Matriz formada por submatrizes para cada ponto fotogramétrico
    * \param    L1   : Matriz formada pelas coordenadas X e Y dos pontos de controle
    * \return Matrix : Matriz m1
    */
    Matrix getm2(Matrix M2, Matrix L1);

    /**
    * \brief Metodo que retorna a matriz com as primeiras aproximaçoes dos parametros da orientaçoes exteriores
    * \param    M12  : Matriz M12
    * \param    m1   : Matriz m1
    * \param    xypf : Matriz xypf, matriz com as primeiras aproximaÃ§oes das coordenadas dos pontos fotogrametricos
    * \return Matrix : Matriz paf, matriz com as primeiras aproximaÃ§oes dos parametros da orientaçao exterior
    */
    Matrix getPAf(Matrix M12, Matrix m1, Matrix xypf);

    /**
    * \brief Metodo que retorna a matriz com as primeiras aproximaçoes das coordernadas dos pontos fotogrametricos
    * \param    M22  : Matriz M22
    * \param    m2   : Matriz m2
    * \param    M12  : Matriz M12
    * \param    paf  : Matriz paf que contem os valores iniciais dos parametros da orientaÃ§ao exterior
    * \return Matrix : Matriz xypf, matriz com as primeiras aproximaÃ§oes das coordenadas dos pontos fotogramétricos
    */
    Matrix getXYpf(Matrix M22, Matrix m2, Matrix M12, Matrix paf);

    /**
    * \brief Metodo que retorna apenas os coeficientes de uma funçao afim para converter coordenada analogica(xsi,eta) para coordenada de terreno
    * \param   Paf   : Matriz com todos os parametros da orientaÃ§ao exterior de todas as imagens
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return Matrix : Matriz com os parametros da orientaÃ§ao exterior de uma imagem
    */
    Matrix getPTA(Matrix PAf, int imageId);

    /**
    * \brief Altera a versão final(AFP) da matriz de ajustamento(matAdjust) dos parametros das orientaçoes exteriores
    */
    void setAFP();

    /**
    * \brief Metodo que retorn a versÃ£o final da matriz de ajustamento(matAdjust) dos parametros das orientaçoes exteriores
    * \return Matrix : Matrix AFP que contem todos os parametros de todas as imagens com omega, phi e kappa em graus
    */
    Matrix getAFP();

    /**
    * \brief Metodo que retorna a matriz dos residuos da ultima iteraÃ§Ã£o
    * \return Matrix : Retorna a matriz dos residuos
    */
    Matrix getMatRes();


    std::deque<double> getListRMSE();

protected:
    //Seta a matrix baseado nos angulos
    /**
    * \brief Altera os valores da matriz de rotaÃ§Ã£o
    * \param omega : Angulo omega em radianos
    * \param phi   : Angulo phi em radianos
    * \param kappa : Angulo kappa em radianos
    * \attention os valores da matriz de rotaÃ§ao estÃ£o contidos nos atributos double : r11,r12,r13,r21,r22,r23,r31,r32 e r33
    */
    void setRot(double omega, double phi, double kappa);

    /**
    * \brief Metodo que retorn os valores xi e eta de uma coordenada X Y Z contida numa imagem atraves das equaçoes de colinearidade
    * \param    X    : Valor da coordenada no eixo X
    * \param    Y    : Valor da coordenada no eixo Y
    * \param    Z    : Valor da coordenada no eixo Z
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return Matrix : Matriz com as coordenadas xi e eta
    */
    Matrix getCoordinatesEqColin(double X, double Y, double Z, int imageId);

    /**
    * \brief Metodo que retorn a matriz Jacobiana de uma coordenada X Y Z de um ponto de Controle
    * \param    X    : Valor da coordenada no eixo X
    * \param    Y    : Valor da coordenada no eixo Y
    * \param    Z    : Valor da coordenada no eixo Z
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return Matrix : Matriz Jacobiana da coordenada
    */
    Matrix getJacobianaControl(double X, double Y, double Z, int imageId);

    /**
    * \brief Metodo que retorn a matriz Jacobiana de uma coordenada X Y Z de um ponto de fotogramétrico
    * \param    X    : Valor da coordenada no eixo X
    * \param    Y    : Valor da coordenada no eixo Y
    * \param    Z    : Valor da coordenada no eixo Z
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO á a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
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
    * \brief Metodo que cria ou altera uma matriz com as coordenadas analogicas dos pontos calculadas por orientaÃ§ao interior
    */
    void createLb();

    /**
    * \brief Metodo que cria ou altera uma matriz com as coordenadas analogicas dos pontos calculadas pelas equaçoes de colinearidades
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
    * \brief Calcula a matriz n1, necessaria para o calculo do ajustamento dos parametros da orientaÃ§ao exterior
    * \param    l    : Matriz diferenÃ§a entre Lb e L0
    * \return Matrix : Matriz n1
    */
    Matrix getn1(Matrix l);

    /**
    * \brief Calcula a matriz n2, necessaria para o calculo do ajustamento dos parametros da orientaÃ§ao exterior
    * \param    l    : Matriz diferenÃ§a entre Lb e L0
    * \return Matrix : Matriz n2
    */
    Matrix getn2(Matrix l);

    /**
    * \brief Calcula e altera a matrix x1 a qual e a matriz que contem os "deltas" a serem somados nos parametros da orientaçao exterior
    * \param    N12  : Matriz N12
    * \param    n1   : Matriz n1
    */
    void setx1(Matrix N12, Matrix n1);

    /**
    * \brief Calcula e altera a matrix x2 a qual e a matriz que contem os "deltas" a serem somados nas coordenadas dos pontos fotogrametricos
    * \param    N12  : Matriz N12
    * \param    N22  : Matriz N22
    * \param    n2   : Matriz n2
    * \param    n1   : Matriz n1
    */
    void setx2(Matrix N12, Matrix N22, Matrix n2, Matrix n1);

    /**
    * \brief Metodo auxiliar que retorna o valor(delta) de ajustamento do angulo Omega contido na matriz x1
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor delta do angulo Omega contido na matriz x1 previamente calculada
    */
    double getdOmegax1(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor(delta) de ajustamento do angulo Phi contido na matriz x1
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor delta do angulo Phi contido na matriz x1 previamente calculada
    */
    double getdPhix1(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor(delta) de ajustamento do angulo Kappa contido na matriz x1
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor delta do angulo Kappa contido na matriz x1 previamente calculada
    */
    double getdKappax1(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada X0 contida na matriz x1
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor delta da coordenada X0 contida na matriz x1 previamente calculada
    */
    double getdXx1(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada Y0 contida na matriz x1
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor delta da coordenada Y0 contida na matriz x1 previamente calculada
    */
    double getdYx1(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada Z0 contida na matriz x1
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor delta da coordenada Z0 contida na matriz x1 previamente calculada
    */
    double getdZx1(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor de Omega na matriz de ajustamento
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor de Omega contido na Matrix matAdjust
    */
    double getOmegaAdjus(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor de Phi na matriz de ajustamento
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor de Phi contido na Matrix matAdjust
    */
    double getPhiAdjus(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor de Kappa na matriz de ajustamento
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor de Kappa contido na Matrix matAdjust
    */
    double getKappaAdjus(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor da coordenada X0 na matriz de ajustamento
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor da coordenada X0 contida na Matrix matAdjust
    */
    double getXAdjus(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor da coordenada Y0 na matriz de ajustamento
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor da coordenada Y0 contida na Matrix matAdjust
    */
    double getYAdjus(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor da coordenada Z0 na matriz de ajustamento
    * \param imageId : id da imagem no calculo atual
    * \attention o imageId NÃO é a key da imagem na ProjectManager e sim a linha em que seus parametros se encontram na matriz de ajustamento(matAdjust)
    * \return double : Valor da coordenada Z0 contida na Matrix matAdjust
    */
    double getZAdjus(int imageId);

    /**
    * \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada X do ponto fotogramétrico contida na matriz x2
    * \param fotogrPointId : id da imagem no calculo atual
    * \attention o fotogrPointId NÃO é a key ou pointId do ponto na ProjectManager e sim a posiÃ§ao na lista de pontos em que ele se encontra
    * \return double : Valor delta da coordenada X do ponto contida na matriz x2 previamente calculada
    */
    double getdXx2(int fotogrPointId);

    /**
    * \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada Y do ponto fotogramétrico contida na matriz x2
    * \param fotogrPointId : id da imagem no calculo atual
    * \attention o fotogrPointId NÃO é a key ou pointId do ponto na ProjectManager e sim a posiÃ§ao na lista de pontos em que ele se encontra
    * \return double : Valor delta da coordenada Y do ponto contida na matriz x2 previamente calculada
    */
    double getdYx2(int fotogrPointId);

    /**
    * \brief Metodo auxiliar que retorna o valor(delta) de ajustamento da coordenada Z do ponto fotogramétrico contida na matriz x2
    * \param fotogrPointId : id da imagem no calculo atual
    * \attention o fotogrPointId NÃO é a key ou pointId do ponto na ProjectManager e sim a posiÃ§ao na lista de pontos em que ele se encontra
    * \return double : Valor delta da coordenada Z do ponto contida na matriz x2 previamente calculada
    */
    double getdZx2(int fotogrPointId);

    /**
    * \brief Metodo que atualiza os valores dos parametros das orientaçoes exteriores utilizando os valores contidos na matriz x1
    */
    void updateMatAdjust();

    /**
    * \brief Metodo que retorna um matriz com as coordenadas de terreno pelas equaçoes de interseÃ§Ã£o espacial
    * \param    xsi  : Coordenada analogica xi em milimetros
    * \param    eta  : Coordenada analogica eta em milimetros
    * \param    z    : Coordenada z  em metros
    * \return Matrix : Matriz que contém as coordenadas de terreno X e Y
    */
    Matrix getCoordColinearTerrain(double xsi, double eta, double z, int imageId);


public:

    /**
    * \brief Metodo que preenche os objetos Point com as coordenadas analogicas baseado na orientaÃ§ao interior
    */
    void fillDetectorCoordinates();

    /**
    * \brief Metodo que converte coordenada digital(col,lin) em coordenada analogica(xi,eta)
    * \param oi  : OrientaÃ§Ã£o interior da imagem em que a coordenada estÃ¡
    * \param linha : Coodernada analogica linha
    * \param coluna : Coodernada analogica coluna
    * \deprecated This method will be replaced by the existent method on class *nome da classe MOTOR DE FOTOGRAMETRIAm*
    */
    Matrix digitalToAnalog(InteriorOrientation* oi, double linha, double coluna);

    /**
    * \brief Metodo que calcula os parametros iniciais das orientaÃ§oes exteriores
    */
    void calculateInicialsValues();

    /**
    * \brief Altera a matriz dos parametros iniciais das orientaçoes exteriores para aqueles fornecido pelo usuÃ¡rio na interface
    * \param initialValues : Matriz com os parametros iniciais, com uma imagem por linha na sequencia: Omega, Phi, Kappa, X0, Y0, Z0
    */
    void setUserInitialValues(Matrix initialValues);

    /**
    * \brief Metodo auxiliar que retorna uma matriz correspondente a uma imagem na matriz M1
    * \param img    : Objeto imagem contendos os pontos pertencentes a ela
    * \param Matrix : Matriz correspondente a uma imagem
    */
    Matrix imageToMatrixDetectorCoordenates(Image* img);

    /**
    * \brief Metodo auxiliar que retorna uma matriz correspondente a uma imagem na matriz A1
    * \param img    : Objeto imagem contendos os pontos pertencentes a ela
    * \param Matrix : Matriz correspondente a uma imagem
    */
    Matrix imageToMatrixJacobiana(int indexImage);

    /**
    * \brief Metodo responsavel por todo o calculo do ajustamento por Bundle Adjustment
    * \return bool: Informa se o calculo foi bem sucedido ou nÃ£o
    */
    bool calculate(bool makeReport);

    /**
    * \brief Metodo que informa se é possivel calcular o ajustamento ou nÃ£o, baseado no numero de incognitas e equaçoes
    * \return bool : Informa se é possivel ou nÃ£o calcular
    */
    bool isPossibleCalculate();

    /**
    * \brief Metodo que cria ou altera a matriz L
    * \return Matrix : retorna a matriz formada pelas coordenadas X e Y dos pontos de controle
    */
    Matrix createL();

    /**
    * \brief Metodo que cria ou altera a matriz M1
    * \return Matrix : retorna a matriz formada M1 necessÃ¡ria para o calculo inicial dos parametros
    */
    Matrix createM1();

    /**
    * \brief Metodo que cria ou altera a matriz M2
    * \return Matrix : retorna a matriz formada M2 necessÃ¡ria para o calculo inicial dos parametros
    */
    Matrix createM2();

    /**
    * \brief Metodo auxiliar que calcula a coordenada incial Z para cada ponto fotogramétrico
    * \return double : Valor inicial da coordenada Z dos pontos fotogramétricos
    */
    double getInicialZPhotogrammetricPoints();

    /**
    * \brief Metodo que atualiza as coordenadas iniciais de todos os pontos fotogramétricos baseada na matriz xypf
    * \param xypf : Matriz contendo as coordenadas de pontos fotogramétricos
    * \param zphotogrammetric : Valor de Z da coordenada de ponto fotogramétrico
    */
    void updateCoordinatesAllPoints(Matrix xypf, double zphotogrammetric);

    //Esses metodos tem como referencia a ordem deles na LISTA DE PONTOS INDEXADOS DE CADA IMAGEM e nao na lista do xml
    /**
    * \brief Metodo auxiliar que testa se determinado ponto contido em determinada imagem é fotogramétrico
    * \param imageIndex : PosiÃ§ao da imagem na lista de imagens
    * \param pointIndex : PosiÃ§ao do ponto na lista de pontos da imagem informada pelo parametro imageIndex
    * \return bool : informa se o ponto é fotogramétrico ou nÃ£o
    */
    bool isPhotogrammetricPoint(int imageIndex, int pointIndex);

    /**
    * \brief Metodo auxiliar que testa se determinado ponto contido em determinada imagem é de controle
    * \param imageIndex : PosiÃ§ao da imagem na lista de imagens
    * \param pointIndex : PosiÃ§ao do ponto na lista de pontos da imagem informada pelo parametro imageIndex
    * \return bool : informa se o ponto é controle ou nÃ£o
    */
    bool isControlPoint(int imageIndex, int pointIndex);

    /**
    * \brief Metodo auxiliar que testa se determinado ponto contido em determinada imagem é de 'checking'
    * \param imageIndex : PosiÃ§ao da imagem na lista de imagens
    * \param pointIndex : PosiÃ§ao do ponto na lista de pontos da imagem informada pelo parametro imageIndex
    * \return bool : informa se o ponto é de 'checking' ou nÃ£o
    */
    bool isCheckingPoint(int imageIndex, int pointIndex);

    /**
    * \brief Metodo que informa se o calculo convergiu ou nÃ£o baseado no numero de iteraÃ§oes e no valor de convergencia estipulado pelo usuÃ¡rio
    * \return bool : informa se o calculo convergiu ou nÃ£o
    */
    bool isConverged();

    /**
    * \brief Metodo que analisa se a matriz x1 e x2 atingiram sua convergencia
    * \return int : Codigo para a analise da convergencia do calculo, se o calculo convergiu retorna 1, se nao convergiu e nÃ£o houve problemas nas matrizes retorna 0, se houver algum NAN na matriz retorna -1, se houver algum valor INF retorna -2
    */
    int testConverged();

    /**
    * \brief Metodo auxiliar que retorna um ponteiro para o ponto na imagem informada
    * \param imageIndex : PosiÃ§ao da imagem na lista de imagens
    * \param pointIndex : PosiÃ§ao do ponto na lista de pontos da imagem informada pelo parametro imageIndex
    * \return Point : informa se os residuos convergiram ou nÃ£o
    */
    Point* getPointFrom(int imageIndex, int pointIndex);

    /**
    * \brief Metodo auxiliar que informa o numero de pontos de controle contidos numa imagem
    * \param img : Objeto imagem a qual se deseja saber o numero de pontos de controle
    * \return int : Numero de pontos de controle
    */
    int numberControlPoints(Image* img);

    /**
    * \brief Metodo auxiliar que informa o numero de pontos fotogramétricos contidos numa imagem
    * \param img : Objeto imagem a qual se deseja saber o numero de pontos fotogramétricos
    * \return int : Numero de pontos fotogramétricos
    */
    int numberPhotogrammetricPoints(Image* img);

    /**
    * \brief Metodo auxiliar que informa a posiÃ§ao de determinado ponto na lista de todos os pontos
    * \param pnt : Objeto ponto a qual se deseja saber sua posiÃ§ao na lista
    * \return int : PosiÃ§ao na lista, se o ponto nÃ£o estiver na lista retorna -1;
    */
    int whereInPoints(Point* pnt);

    /**
    * \brief Metodo auxiliar que informa a posiÃ§ao de determinado ponto na lista de pontos fotogramétricos
    * \param pnt : Objeto ponto a qual se deseja saber sua posiÃ§ao na lista
    * \return int : PosiÃ§ao na lista, se o ponto nÃ£o estiver na lista retorna -1;
    */
    int whereInPhotogrammetricPoints(Point* pnt);

    /**
    * \brief Metodo que zera todas as coordenadas de ponto fotogramétricos
    * \deprecated Metodo desnecessÃ¡rio tendo em vista que as coordenadas iniciais dos pontos fotogramétricos sao irrelevantes para o calculo
    */
    void zeroingCoordinatesPhotogrammetrics();

    /**
    * \brief Metodo que retorna o total de iteraÃ§oes até o momento
    * \return int : total de iteraÃ§oes
    */
    int getTotalIterations();

    /**
    * \brief Metodo que altera o numero maximo de iteraçoes
    * \param value : Numero maximo de iteraçoes
    */
    void setMaxNumberIterations(int value);

    /**
    * \brief Metodo que altera o valor da convergencia das coordenadas dos parametros das orientaçoes exteriores
    * \param value : Valor da convergencia das coordenadas
    */
    void setMetricConvergencyValue(double value);

    /**
    * \brief Metodo que altera o valor da convergencia dos angulos dos parametros das orientaçoes exteriores
    * \param value : Valor da convergencia dos angulos
    */
    void setAngularConvergencyValue(double value);

    /**
    * \brief Metodo que retorna o valor da convergencia das coordenadas dos parametros das orientaçoes exteriores
    * \return double : Valor de convergencia das coordenadas
    */
    double getMetricConvergencyValue();

    /**
    * \brief Metodo que retorna o valor da convergencia dos angulos dos parametros das orientaçoes exteriores
    * \return double : Valor de convergencia dos angulos
    */
    double getAngularConvergencyValue();


    /**
    * \brief Metodo que retorna a matrix x1 formatada para apresentar os angulos em graus e cada orientaÃ§ao exterior em uma linha
    * \return Matrix : Matriz formatada
    */
    Matrix getMVC();

    /**
    * \brief Metodo que calcula os residuos apÃ³s a convergencia do calculo e altera o atributo matRes que guarda os residuos
    */
    void calculateResiduos();

    /**
    *	\brief Metodo que atualiza, isto é, ajusta as coordenadas de pontos fotogramétricos baseado na matriz x2
    */
    void updateCoordFotog();


    /**
    * \brief Metodo que inverte apenas as matrizes da diagonal principal da matriz m11, evitando assim calculos desnecessÃ¡rios
    * \param m11 : Matriz M11 a ser invertida
    */
    void setInverseM11(Matrix m11);


    /**
    * \brief Metodo que inverte apenas as matrizes da diagonal principal da matriz n11, evitando assim calculos desnecessÃ¡rios
    * \param n11 : Matriz N11 a ser invertida
    */
    void setInverseN11(Matrix n11);


    /**
    * \brief Metodo auxiliar que retorna os residuos de um ponto fotogrametrico, se o ponto nao for fotogrametrico retorna uma matriz com os elementos zerados
    * \param photogrammetricPoint : Objeto ponto fotogrametrico
    * \return Matrix : Matriz[1x3] com os residuos dos pontos na sequencia X, Y, Z
    */
    Matrix getResiduo(Point* photogrammetricPoint);


    /**
    * \brief Metodo que retorna a matriz com as aproximaÃ§oes iniciais dos parametros de orientaÃ§ao exterior de todas as imagens
    * \return Matrix : Matrix com os parametros de orientaÃ§ao exterior
    *
    */
    Matrix getMatrixInicialValues();

    /**
    * \brief Metodo que checa se o usuario colocou para todas as imagens todos os parametros da orinentação exterior como valores iniciais
    * \return bool : Informa se todas as imagens tem valores iniciais para o calculo dos parametros de orientaçao exterior
    */
    bool hasAllImagesInitialValues();

    /**
    * \brief Metodo que retorna o indice na lista de imagens
    * \param img : Imagem cujo posiçao na lista deseja-se saber
    * \return int : posição na lista de imagens
    */
    int whereInImages(Image* img);


    /**
    * \brief Metodo que calcula o RMSE do bloco
    * \return double: Valor do RMSE
    */
    double calculateRMSE();


};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // BUNDLEADJUSTMENT_H
