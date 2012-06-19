/**************************************************************************
														   DEMManager.h
**************************************************************************/


#ifndef DEMMANAGER_H
#define DEMMANAGER_H

#include "EObject.h"
#include "Matrix.h"
#include "ProgressPublisher.h"
#include "Features.h"

#ifdef INTEGRATED_EFOTO
#include "Point.h"
#include "Image.h"
#include "ExteriorOrientation.h"
#include "ImageMatching.h"
#include "MatchingPoints.h"
#include "DemGrid.h"
#include "SpatialIntersection.h"
#endif //INTEGRATED_EFOTO REVER!
#ifdef SYNAPSE_EFOTO
	// Aqui vai entrar uma referência para classes da sinapse engine
#endif //SYNAPSE_EFOTO

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class DEMUserInterface;
class EFotoManager;

/**
* \file DEMManager.h
* \class DEMManager
* \brief Classe que gerencia a extração do DEM.
* \attention Imagens que variam entre 0,0 e 1,0.
* \copyright E-Foto group
* \authors Marcelo Teixeira Silveira
*/
class DEMManager : public ProgressPublisher
{
	// Private Attributes
	//
	double bb_Xi, bb_Yi, bb_Xf, bb_Yf;
	bool bb_empty;
	void updateBoundingBox(double Xi, double Yi, double Xf, double Yf);
	int rad_cor, match_method, rgx, rgy, lsm_temp, lsm_it, lsm_dist, ncc_temp, ncc_sw;
	double lsm_th, lsm_std, lsm_shift, lsm_shear, lsm_scale, ncc_th, ncc_std, downsample;
	bool isShowImage;
	bool started;
	bool status;
	bool over_it;
	bool cancel_flag;
	bool dem_unsaved;
	bool grid_unsaved;
	bool elim_bad_pts;
	double over_it_dist;
	DEMUserInterface* myInterface;
	EFotoManager* manager;
    deque<int> listPairs;

#ifdef INTEGRATED_EFOTO
	deque<Image*> listAllImages;
    deque<Point*> listAllPoints;
	deque<ExteriorOrientation*> listEOs;
    MatchingPointsList seeds, pairs;
    ImageMatching *im;
    DemGrid *grid;
    StereoPair sp;

    /**
    * \brief Método privado que retorna um ponteiro para uma instância Image.
    */
    Image * getImage(int);
    /**
    * \brief Metodo que realiza uma reamostragem nas coordenadas dos pontos, caso exista uma redução de qualidade da imagem. As coordenadas dos pontos devem ser ajustadas para a imagem em dimensoes reduzidas. Depois da correlacao, as coordenadas devem ser corrigidas por um fator igual a 1/taxa de reamostragem.
    */
    void resamplePoints(MatchingPointsList *list, double resample);

    /**
    * \brief Método que converte pontos e linhas em lista de pares
    * \param df - feição de onde serão extraidos os pontos e linhas
    * \param mpl - lista a ser preenchida
    * \param usePPolygons - determina o uso de linhas dos poligonos
    */
    void addFeaturesToPairList(Features* df, MatchingPointsList *mpl, bool usePolygons = false);
#endif //INTEGRATED_EFOTO REVER!

	/**
	* \brief Método privado que atualiza os número de sementes de dados.
	*/
    void updateNoSeeds();
	/**
	* \brief Método privado que refaz a lista de pontos do projeto.
	*/
	void setListPoint();
	/**
	* \brief Método privado que conecta pontos às imagens correspondentes.
	*/
    bool connectImagePoints();
	/**
	* \brief Método privado que retorna os identificadores das imagens através do identificador do par dessas imagens.
	*/
	void getImagesId(int, int&, int&);
	/**
	* \brief Método privado que adiciona um par à interface.
	*/
    void addPairsToInterface();
	/**
	* \brief Método que adiciona pontos de controle, fotogramétricos e de verificação à lista de sementes.
	*/
	void createInitialSeeds();
	/**
	* \brief Método que inicia a extração automática.
	*/
	void extractDEMPair(int);
	/**
	* \brief Método que calcula os pontos X, Y e Z depois do matching. A função de matching só encontra coordenadas 2D.
	*/
	void calcPointsXYZ();
	/**
	* \brief Método que fiixa o ângulo num intervalo de zero a 2pi. Utilizado pelo método checkAnglesAlligned.
	*/
	double fixAngle(double angle);
	/**
	* \brief Método que calcula o ângulo entre dois pontos.
	*/
	double getAngleBetweenImages(double X1, double Y1, double X2, double Y2);
	/**
	* \brief Método que verifica se dois ângulos tem alinhamento.
	*/
	bool checkAnglesAlligned(double angle1, double angle2, double tolerance);

    Features *df;
	int lsm_temp_growth_step, lsm_temp_max_size, ncc_temp_growth_step, ncc_temp_max_size;
	double dem_total_elapsed_time;

public:

	// Constructors and Destructors
	//
	/**
	* \brief Construtor padrão.
	*/
    DEMManager();
    /**
    * \brief Construtor que já identifica o seu gerenciador, as imagens que serão usadas e os dados de uma orientação exterior a ser extraídas.
    */
    DEMManager(EFotoManager* manager);
	/**
	* \brief Destrutor padrão.
	*/
	~DEMManager();



#ifdef INTEGRATED_EFOTO
    /**
    * \brief Método que adiciona pares às sementes.
    */
    void getPointList(MatchingPointsList &sd, MatchingPointsList &pr);
    /**
    * \brief Método que retorna uma lista de instâncias da classe Image.
    */
    deque<Image*> getImages() { return listAllImages; }
    /**
    * \brief Método que força a sobreescrita na lista de sementes. Chamada pelo editor de sementes.
    */
    void overwriteSeedsList(MatchingPointsList sedlist) { seeds = sedlist; updateNoSeeds(); }
#endif //INTEGRATED_EFOTO REVER!


	// Association Methods
	//
	/**
	* \brief Método de associação que seta uma interface.
	*/
	void setInterface(DEMUserInterface* newInterface);
	/**
	* \brief Método que retorna a Interface corrente.
	*/
	DEMUserInterface* getInterface();

	// Other Methods
	//
	/**
	* \brief Método que inicia o Gerenciador do DEM.
	*/
	bool exec();
	/**
	* \brief Método que interrompe o Gerenciador do DEM.
	*/
	void returnProject();
	/**
	* \brief Construtor.
	*/
	int getPairs();
	/**
	* \brief Extração do DEM.
	* Opções: 	0 = todos os pares,
	* 		1 = Par 1,
	* 		2 = Par 2 e
	* 		etc.
	*/
	int extractDEM(int, bool);
	/**
	* \brief Método que altera as configurações de extração automática.
	*/
	void setAutoExtractionSettings(int, int, int, double);
	/**
	* \brief Método que altera as configurações do NCC.
	*/
	void setNCCSettings(int, int, double, double);
	/**
	* \brief Método que altera as configurações do LSM.
	*/
	void setLSMSettings(int, int, double, double, int, double, double, double, int, double);
	/**
	* \brief Método que altera a barra de progresso na interface.
	*/
	void setProgress(int);
	/**
	* \brief Método que salva o DEM.
	*/
	void saveDem(char * filename, int fileType);
	/**
	* \brief Método que salva a grade.
	*/
	void saveDemGrid(char * filename, int fileType);
	/**
	* \brief Método que carrega o DEM.
	*/
	int loadDem(char * filename, int fileType);
	/**
	* \brief Método que carrega a grade.
	*/
	int loadDemGrid(char * filename, int fileType);
	/**
	* \brief Método que interpola a grade.
	*/
	void interpolateGrid(int source, int method, int garea, double Xi, double Yi, double Xf, double Yf, double res_x, double res_y, int tsurface, double ma_exp, double ma_dist, int ma_weight, int gridSource);
	/**
	* \brief Método que ativa a função de cancelamento. Interrompe a extração do DEM ou a  ação de interpolação da grade.
	*/
	void setCancel();
	/**
	* \brief Método que altera o flag showImage. Se selecionado, após a extração do DEM ou interpolação da grade, uma janela surge mostrando os resultados.
	*/
    void setShowImage(int _state) { isShowImage = _state; }
	/**
	* \brief Método que define a eliminação de pontos ruins.
	*/
    void setEliminateBadPoints(bool _el) { elim_bad_pts = _el; }
	/**
	* \brief Método que verifica se alguma operação foi cancelada.
	*/
    bool cancelFlag() { return cancel_flag; }
	/**
	* \brief Método que verifica se a DEM foi salva.
	*/
    bool isDemUnsaved() { return dem_unsaved; }
	/**
	* \brief Método que verifica se a grade foi salva.
	*/
    bool isGridUnsaved() { return grid_unsaved; }
    /**
	* \brief Método que retorna uma lista de pares de imagens.
	*/
    deque<int> getImagesPairs() { return listPairs; }
	/**
	* \brief Método que carrega as feições do DEM.
	*/
	int loadDemFeature(char * filename);
	/**
	* \brief Método que carrega um arquivo com as qualidades do DEM.
	*/
	string getDemQuality(char * filename, int option);
	/**
	* \brief Método que muda os parâmetros do LSM e NCC.
	*/
    void setStdParameters(int _lsm_gr, int _lsm_ms, int _ncc_gr, int _ncc_ms) { lsm_temp_growth_step = _lsm_gr; lsm_temp_max_size = _lsm_ms; ncc_temp_growth_step = _ncc_gr; ncc_temp_max_size = _ncc_ms; }
	/**
	* \brief Método que calcula a resolução do DEM.
	*/
	double calculateDemRes(double ds);
	/**
	* \brief Registra no XML o endereço de um arquivo de matching.
	*/
	void addPairsToXML(string filename);
	/**
	* \brief Registra no XML o endereço de um arquivo de sementes para matching.
	*/
	void addSeedsToXML(string filename);
	/**
	* \brief Registra no XML o endereço de um arquivo de DEM.
	*/
	void addDEMToXML(string filename);
	/**
	* \brief Registra no XML o endereço de um arquivo de DEM Quality.
	*/
	void addDEMQualityToXML(string filename);

};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //DEMMANAGER_H
