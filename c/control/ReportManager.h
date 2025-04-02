#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H
/**************************************************************************
		  ReportManager.h
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

#include "Dms.h"

#include <QImage>

class QTreeWidgetItem;

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ReportUserInterface;
class EFotoManager;

//New
class Project;


class ReportManager
{
	// Private Attributes
	//
	bool started;
	bool status;
    EFotoManager* efotoManager;
    Project* project;
	ReportUserInterface* myInterface;

public:

	// Constructors and Destructors
	//
	/**
	* \brief Construtor vazio.
	*/
	ReportManager();
	/**
	* \brief Construtor vazio.
	*/
    explicit ReportManager(EFotoManager* manager);
	/**
	* \brief Destrutor padro.
	*/
	~ReportManager();

	// Association Methods
	//
	/**
    * \brief Metodo de associcao que seta uma interface.
	* \param newInterface Interface que ser usada.
	*/
	void setInterface(ReportUserInterface* newInterface);
	/**
    * \brief Metodo que retorna a Interface corrente.
	* \return IOUserInterface Interface corrente.
	*/
	ReportUserInterface* getInterface();

	// Other Methods
	//
	/**
    * \brief Metodo que inicia o mdulo de Orientao Interior e retorna se foi feito algum clculo neste mesmo mdulo anteriormente para a alertar o usurio, caso o projeto no tenha sido salvo.
	* \return bool Retorna verdadeiro se o projeto foi salvo. Retorna falso, caso contrrio.
	*/
	bool exec();
	/**
    * \brief Metodo que carrega a interface do projeto quando o mdulo de Orientao Interior  finalizado.
	*/
	void returnProject();

    /**
    * \brief Metodo que bloqueia as opcoes da arvore que ainda nao foram obtidas durante o projeto fotogrametrico.
    * \param treeItems Arvore de opcoes do relatorio fotogrametrico.
    */
    void checkTree(QList<QTreeWidgetItem*> treeItems);
    /**
    * \brief Metodo que retorna o XML da opcao pai Header.
    * \return string XML da opcao pai Header.
    */
    std::string eprHeader();
    /**
    * \brief Metodo que retorna o XML da opcao Terrain.
    * \return string XML da opcao pai Terrain.
    */
    std::string eprTerrain();
    /**
    * \brief Metodo que retorna o XML da opcao pai Sensor.
    * \return string XML da opcao pai Sensor.
    */
    std::string eprSensors();
    /**
    * \brief Metodo que retorna o XML da opcao pai Flight.
    * \return string XML da opcao pai Flight.
    */
    std::string eprFlights();
    /**
    * \brief Metodo que retorna o XML da opcao pai Images.
    * \param treeItems Arvore de opcoes do relatorio fotogrametrico.
    * \return string XML da opcao pai Images.
    */
    std::string eprImages(QList<QTreeWidgetItem*> treeItems);
    /**
    * \brief Metodo que retorna o XML da opcao pai Points.
    * \param treeItems Arvore de opcoes do relatorio fotogrametrico.
    * \return string XML da opcao pai Points
    */
    std::string eprBlockPoints(QList<QTreeWidgetItem*> treeItems);
    /**
    * \brief Metodo que retorna o XML da opcao pai Affine Transformation.
    * \param treeItems Arvore de opcoes do relatorio fotogrametrico.
    * \return string XML da opcao pai Affine Transformation.
    */
    std::string eprAffineTransformation(QList<QTreeWidgetItem*> treeItems);
    /**
    * \brief Metodo que retorna o XML da opcao pai Spatial Resection.
    * \param treeItems Arvore de opcoes do relatorio fotogrametrico.
    * \return string XML da opcao pai Spatial Resection.
    */
    std::string eprSpatialRessection(QList<QTreeWidgetItem*> treeItems);
    /**
    * \brief Metodo que retorna o XML da opcao pai Phtogrammetric Block.
    * \param treeItems Arvore de opcoes do relatorio fotogrametrico.
    * \return string XML da opcao pai Phtogrammetric Block.
    */
    std::string eprPhotogrammetricBlock(QList<QTreeWidgetItem*> treeItems);
    /**
    * \brief Metodo que retorna o XML da opcao pai Interior Orientation.
    * \param treeItems Arvore de opcoes do relatorio fotogrametrico.
    * \return string XML da opcao pai Interior Orientation.
    */
    std::string eprInteriorOrientation(QList<QTreeWidgetItem*> treeItems);
    /**
    * \brief Metodo que retorna o XML da opcao pai Exterior Orientation.
    * \param treeItems Arvore de opcoes do relatorio fotogrametrico.
    * \return string XML da opcao pai Exterior Orientation.
    */
    std::string eprExteriorOrientation(QList<QTreeWidgetItem*> treeItems);
    /**
    * \brief Metodo que retorna o XML da opcao pai Stereo Pairs.
    * \return string XML da opcao pai Stereo Pairs.
    */
    std::string eprStereoPairs();
    /**
    * \brief Metodo que retorna o XML da opcao pai Stereo Plotting.
    * \return string XML da opcao pai Stereo Plotting.
    * \todo Metodo nao finalizado. Usar o metodo makeThumbnail.
    */
    std::string eprStereoPlotting();
    /**
    * \brief Metodo que retorna o XML da opcao pai DSM.
    * \return string XML da opcao pai DSM.
    * \todo Metodo nao finalizado. Usar o metodo makeThumbnail.
    */
    std::string eprDSM();
    /**
    * \brief Metodo que retorna o XML da opcao pai Ortho Rectification.
    * \return string XML da opcao pai Ortho Rectification.
    * \todo Metodo nao finalizado. Usar o metodo makeThumbnail.
    */
    std::string eprOrthorectification();

    /**
    * \brief Metodo que chama os metodos necessarios, de acordo com as opcoes marcadas da arvore, e cria o arquivo XML do relatorio.
    * \param filename Caminho total e nome do arquivo.
    * \param idExt Id do tipo de formato escolhido pelo usuario para a inclusao, se necessario, do header xsl.
    * \param treeItems Arvore de opcoes do relatorio fotogrametrico.
    * \return bool Retorna se o arquivo XML foi criado com sucesso.
    */
    bool makeFile(std::string filename, int idExt, QList<QTreeWidgetItem*> treeItems);
    /**
    * \brief Metodo que cria o arquivo XSL correspondente a escolha do usuario
    * \param idExt Id do tipo de formato escolhido pelo usuario.
    * \param path Caminho para salvar o arquivo.
    * \return bool Retorna se o arquivo XSL foi criado com sucesso.
    */
    bool makeXslt(int idExt, std::string path);
    /**
    * \brief Metodo que cria o arquivo CSS para atribuir estilo ao relatorio.
    * \param path Caminho para salvar o arquivo.
    * \return bool Retorna se o arquivo CSS foi criado com sucesso.
    */
    bool makeCss(std::string path);

    /**
    * \brief Metodo que gera thumbnail de imagens.
    * \param filename Nome do arquivo imagem.
    * \param maxW Tamanho maximo da largura.
    * \param maxH Tamanho maximo da altura.
    * \return QImage O thumbnail gerado.
    */
    QImage makeThumbnail(QString filename, int maxW, int maxH);

    /*
    #ifdef Q_WS_X11 //LINUX
    #endif
    #ifdef Q_WS_WIN //WINDOWS
    #endif
    #ifdef Q_WS_MACX //MAC
    #endif
    */
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif //REPORTMANAGER_H
