/**************************************************************************
DEMManager.h
**************************************************************************/
/*Copyright 2002-2014 e-foto team (UERJ)
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

#ifndef DEMMANAGER_H
#define DEMMANAGER_H

#include "StereoPair.h"
#include "MatchingPoints.h"


#include <deque>
#include <tuple>

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class DEMUserInterface;
class EFotoManager;
class ImageMatching;
class DemGrid;
class DemFeatures;
class Point;
class ExteriorOrientation;
enum class InterpolationMethod;

enum class Filetype;
enum class MAWeight;
enum class TSSurface;

/**
* \file DEMManager.h
* \class DEMManager
* \brief Class to manage DEM extraction.
* \attention Images between 0,0 and 1,0.
*/
class DEMManager {
public:
    /**
    * \brief default Constructor.
    */
    DEMManager();
    /**
    * \brief Construtor que já identifica o seu gerenciador, as imagens que serão
    *        usadas e os dados de uma orientação exterior a serem extraídas.
    */
    explicit DEMManager(EFotoManager* manager_, std::deque<Image*> images,
                        std::deque<ExteriorOrientation*> eos);
    /**
    * \brief Destrutor padrão.
    */
    ~DEMManager();

    /**
    * \brief Método que inicia o Gerenciador do DEM.
    */
    bool exec();
    /**
    * \brief Método que interrompe o Gerenciador do DEM.
    */
    void returnProject() const;
    /**
    * \brief Construtor.
    */
    int getPairs();
    /**
    * \brief Extração do DEM.
    * Opções:     0 = todos os pares,
    *       1 = Par 1,
    *       2 = Par 2 e
    *       etc.
    */
    int extractDEM(int, bool);

    /**
    * \brief Método que altera as configurações de extração automática.
    */
    void setAutoExtractionSettings(int, int match_method, int, double);

    /**
    * \brief Método que altera as configurações do NCC.
    */
    void setNCCSettings(int, int, double, double);

    /**
    * \brief Método que altera as configurações do LSM.
    */
    void setLSMSettings(int, int, double, double, int, double,
                        double, double, int, double);

    /**
    * \brief Método que altera a barra de progresso na interface.
    */
    void setProgress(int) const;

    /**
    * \brief Método que salva o DEM.
    */
    void saveDem(const char* const filename, int fileType);

    /**
    * \brief Método que salva a grade.
    */
    void saveDemGrid(const char* const filename, Filetype fileType);

    /**
    * \brief Método que carrega o DEM.
    */
    bool loadDem(char* filename, int fileType);

    /**
    * \brief Método que carrega a grade.
    */
    bool loadDemGrid(char* filename, Filetype fileType);

    /**
    * \brief Método que interpola a grade.
    */
    void interpolateGrid(InterpolationMethod method,
                         int garea,
                         double Xi,
                         double Yi,
                         double Xf,
                         double Yf,
                         double res_x,
                         double res_y,
                         TSSurface tsurface,
                         double ma_exp,
                         double ma_dist,
                         MAWeight ma_weight,
                         int gridSource);

    /**
    * \brief Método que ativa a função de cancelamento. Interrompe a extração do DEM ou a  ação
    *        de interpolação da grade.
    */
    void setCancel();

    /**
    * \brief Método que altera o flag showImage. Se selecionado, após a extração do DEM
    *        ou interpolação da grade, uma janela surge mostrando os resultados.
    */
    void setShowImage(int _state)
    {
        isShowImage_ = _state;
    };

    /**
    * \brief Método que verifica se alguma operação foi cancelada.
    */
    bool cancelFlag() const
    {
        return cancel_flag_;
    };

    /**
    * \brief Método que verifica se a DEM foi salva.
    */
    bool isDemUnsaved() const
    {
        return dem_unsaved_;
    };

    /**
    * \brief Método que verifica se a grade foi salva.
    */
    bool isGridUnsaved() const
    {
        return grid_unsaved_;
    };

    /**
    * \brief Método que adiciona pares às sementes.
    */
    void getPointList(MatchingPointsList& sd, MatchingPointsList& pr) const;

    /**
    * \brief Método que retorna uma lista de instâncias da classe Image.
    */
    std::deque<Image*> getImages()
    {
        return listAllImages_;
    };

    /**
    * \brief Método que retorna uma lista de pares de imagens.
    */
    std::deque<int> getImagesPairs() const
    {
        return listPairs;
    };

    /**
    * \brief Método que força a sobreescrita na lista de sementes.
    *        Chamada pelo editor de sementes.
    */
    void overwriteSeedsList(MatchingPointsList sedlist)
    {
        seeds_ = sedlist;
        updateNoSeeds();
    };

    /**
    * \brief Método que carrega as feições do DEM.
    */
    bool loadDemFeature(char* filename);

    /**
    * \brief Método que carrega um arquivo com as qualidades do DEM.
    */
    std::string getDemQuality(char* filename, int option) const;

    /**
    * \brief Método que muda os parâmetros do LSM e NCC.
    */
    void setStdParameters(int _lsm_gr, int _lsm_ms, int _ncc_gr, int _ncc_ms,
                          int _perf_RG)
    {
        lsm_temp_growth_step_ = _lsm_gr;
        lsm_temp_max_size_ = _lsm_ms;
        ncc_temp_growth_step_ = _ncc_gr;
        ncc_temp_max_size_ = _ncc_ms;
        perf_RG = _perf_RG;
    };

    /**
    * \brief Método que calcula a resolução do DEM.
    */
    double calculateDemRes(double ds);

    /**
    * \brief Registra no XML o endereço de um arquivo de matching.
    */
    void addPairsToXML(std::string filename);

    /**
    * \brief Registra no XML o endereço de um arquivo de sementes para matching.
    */
    void addSeedsToXML(std::string filename);

    /**
    * \brief Registra no XML o endereço de um arquivo de DEM.
    */
    void addDEMToXML(std::string filename);

    /**
    * \brief Ativar eliminação de pontos ruins.
    */
    void eliminateBadPoints()
    {
        pairs.filterBadPoints2D();
    }

private:
    EFotoManager* manager_;
    bool cancel_flag_;
    std::deque<Image*> listAllImages_;
    std::deque<ExteriorOrientation*> listEOs_;
    DemGrid* grid_;
    ImageMatching* image_matching_;
    DemFeatures* dem_feature_;
    bool isShowImage_;
    bool dem_unsaved_;
    bool grid_unsaved_;
    bool elim_bad_pts_;
    bool bounding_box_empty_;
    int lsm_temp_growth_step_;
    int lsm_temp_max_size_;
    int ncc_temp_growth_step_;
    int ncc_temp_max_size_;

    double bounding_box_Xi_, bounding_box_Yi_, bounding_box_Xf_, bounding_box_Yf_;
    int rad_cor_, match_method_, rgx_, rgy_, lsm_temp_, lsm_it_, lsm_dist_,
        ncc_temp_,
        ncc_sw_;
    double lsm_th_, lsm_std_, lsm_shift_, lsm_shear_, lsm_scale_, ncc_th_, ncc_std_,
           downsample_;
    bool perf_RG;
    bool over_it_;
    double over_it_dist_;
    DEMUserInterface* myInterface;
    std::deque<Point*> listAllPoints;
    std::deque<int> listPairs;
    MatchingPointsList seeds_, pairs;
    StereoPair sp;
    double dem_total_elapsed_time;
    bool started_;

    void updateBoundingBox(double Xi, double Yi, double Xf, double Yf);
    /**
    * \brief Método privado que atualiza os número de sementes de dados.
    */
    void updateNoSeeds();

    /**
    * \brief Método privado que retorna um ponteiro para uma instância Image.
    */
    Image* getImage(int) const;

    /**
    * \brief Método privado que refaz a lista de pontos do projeto.
    */
    void setListPoint();

    /**
    * \brief Método privado que conecta pontos às imagens correspondentes.
    */
    bool connectImagePoints();

    /**
    * \brief Método privado que retorna os identificadores das imagens através do
    *        identificador do par dessas imagens.
    */
    std::tuple<int, int> getImagesId(int pos) const;

    /**
    * \brief Método privado que adiciona um par à interface.
    */
    void addPairsToInterface() const;

    /**
    * Metodo que realiza uma reamostragem nas coordenadas dos pontos, caso exista
    * uma redução de qualidade da imagem. As coordenadas dos pontos devem ser
    * ajustadas para a imagem em dimensoes reduzidas. Depois da correlacao, as
    * coordenadas devem ser corrigidas por um fator igual a 1/taxa de reamostragem.
    */
    static void resamplePoints(MatchingPointsList* list, double resample);

    /**
    * \brief Método que adiciona pontos de controle, fotogramétricos e de verificação
    *        à lista de sementes.
    */
    void createInitialSeeds();

    /**
    * \brief Método que inicia a extração automática.
    */
    void extractDEMPair(int);

    /**
    * \brief Método que calcula os pontos X, Y e Z depois do matching.
    *        A função de matching só encontra coordenadas 2D.
    */
    void calcPointsXYZ();
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //DEMMANAGER_H
