#ifndef PROJECT_H
#define PROJECT_H
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
#include "EObject.h"
#include "PhotoTri.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

class ProjectHeader;
class SensorWithFiducialMarks;
class Image;
class Point;
class Terrain;
class Sensor;
class Flight;
class InteriorOrientation;
class ExteriorOrientation;
class PhotoTri;


class Project
{

private:
    class DEM; //Dummy class. To be implemented
    class OrthoImage; //Dummy class. To be implemented
    class featuresFile; //Dummy class. To be implemented

    std::string xmlData;
    std::string xmlState;

    ProjectHeader* theHeader;
    Terrain* theTerrain;
    std::deque<Sensor*> sensors;
    std::deque<Flight*> flights;
    std::deque<Image*> images;
    std::deque<Point*> points;
    std::deque<DEM*> DEMs;
    std::deque<OrthoImage*> orthos;
    std::deque<featuresFile*> feats;
    std::string processStates;
    std::deque<InteriorOrientation*> IOs;
    std::deque<ExteriorOrientation*> EOs;
    PhotoTri* thePhotoTri;



    ProjectHeader* instanceHeader();

    /**
    * \brief Método que cria uma instância da classe Terrain.
    * \return Terrain Retorna a instância criada.
    */
    Terrain* instanceTerrain();

    /**
    * \brief Método que cria uma instância da classe Sensor.
    * \param id Identificador do Sensor.
    * \return Sensor Retorna a instância criada.
    */
    Sensor* instanceSensor(int id);

    /**
    * \brief Método que cria uma instância da classe Flight.
    * \param id Identificador do Vôo.
    * \return Flight Retorna a instância criada.
    */
    Flight* instanceFlight(int id);

    /**
    * \brief Método que cria uma instância da classe Image.
    * \param id Identificador da Imagem.
    * \return Image Retorna a instância criada.
    */
    Image* instanceImage(int id);


    /**
    * \brief Método que cria instâncias de classes para todos os pontos existentes do projeto.
    */
    void instanceAllPoints();

    /**
    * \brief Método que cria instâncias de classes para todas as orientações interiores existentes do projeto.
    */
    void instanceAllIOs();

    /**
    * \brief Método que cria instâncias de classes para todas as orientações exteriores existentes do projeto.
    */
    void instanceAllEOs();

    /**
    * \brief Método que cria instâncias de classes para todas as imagens existentes do projeto.
    */
    void instanceAllImages();

    /**
    * \brief Método que cria uma instância da classe Point.
    * \param id Identificador do Ponto.
    * \return Point Retorna a instância criada.
    */
    Point* instancePoint(int id);

    /**
    * \brief Método que cria uma instância da classe InteriorOrientation.
    * \param imageId Identificador da imagem a ser utilizada na Orientação Interior.
    * \return InteriorOrientation Retorna a instância criada.
    */
    InteriorOrientation* instanceIO(int imageId);

    /**
    * \brief Método que cria uma instância da classe ExteriorOrientation.
    * \param imageId Identificador da imagem a ser utilizada na Orientação Exterior.
    * \return ExteriorOrientation Retorna a instância criada.
    */
    ExteriorOrientation* instanceEO(int imageId);

    PhotoTri* instancePhotoTri();

    /**
    * \brief Método que apaga uma instância da classe ProjectHeader.
    */
    void deleteHeader(bool makeReconnections = true);

    /**
    * \brief Método que apaga uma instância da classe Terrain.
    */
    void deleteTerrain(bool makeReconnections = true);

    void deletePhotoTri(bool makeReconnections = true);

    void linkAll();

public:

    /**
    * \brief Construtor padrão.
    */
    Project();

    /**
    * \brief Destrutor padrão.
    */
    ~Project();

    ProjectHeader* header();

    /**
    * \brief Método que retorna uma instância existente da classe Terrain.
    * \return Terrain Instância da classe Terrain.
     */
    Terrain* terrain();

    /**
    * \brief Método que retorna uma instância existente da classe Sensor.
    * \param id Identificador do Sensor.
    * \return Sensor Instância da classe Sensor.
    */
    Sensor* sensor(int id);

    /**
    * \brief Método que retorna uma instância existente da classe Flight.
    * \param id Identificador do Vôo.
    * \return Flight Instância da classe Flight.
    */
    Flight* flight(int id);

    /**
    * \brief Método que retorna uma instância existente da classe Image.
    * \param id Identificador da Imagem.
    * \return Image Instância da classe Image.
    */
    Image* image(int id);

    /**
    * \brief Método que retorna todas as instâncias existente da classe Image.
    * \return std::deque<Image*> std::deque com ponteiros para as instâncias da classe Image.
    */
    std::deque<Image*> allImages() {return images;}

    /**
    * \brief Método que retorna uma instância existente da classe Point.
    * \param id Identificador do Ponto.
    * \return Point Instância da classe Point.
    */
    Point* point(int id);

    /**
    * \brief Método que retorna todas as instâncias existente da classe Point.
    * \return std::deque<Point*> std::deque com ponteiros para as instâncias da classe Point.
    */
    std::deque<Point*> allPoints() {return points;}

    /**
    * \brief Método que retorna uma instância existente da classe InteriorOrientation.
    * \param id Identificador da Orientação Interior.
    * \return InteriorOrientation Instância da classe InteriorOrientation.
    */
    InteriorOrientation* IO(int id);
    std::deque<InteriorOrientation*> allIOs() {return IOs;}

    /**
    * \brief Método que retorna uma instância existente da classe ExteriorOrientation.
    * \param id Identificador da Orientação Exterior.
    * \return ExteriorOrientation Instância da classe ExteriorOrientation.
    */
    ExteriorOrientation* EO(int id);
    std::deque<ExteriorOrientation*> allEOs() {return EOs;}

    PhotoTri* photoTri();

    /**
    * \brief Método que retorna os valores de um nó do XML.
    * \param tagname Nome da tag do XML.
    * \return std::string Valores do Nó do XML que foi requisitado.
    */
    std::string getXml(std::string tagname);

    /**
    * \brief Método que retorna os valores de um nó do XML.
    * \param tagname Nome da tag do XML.
    * \param att Valor do atributo da tag do XML.
    * \param value Valor do nó da tag do XML.
    * \return std::string Valores do Nó do XML que foi requisitado.
    */
    std::string getXml(std::string tagname, std::string att, std::string value);

    /**
    * \brief Método para emitir o nome de classe.
    * \return std::string	Retorna o nome de classe do objeto.
    */
    std::string objectType(void);

    /**
    * \brief Método para emitir as associações de uma instância.
    * \return std::string	Retorna vazio para esta classe.
    * \deprecated Este método não possui uso ou deve ser evitado o seu uso, pois ele será removido em versões futuras.
    */
    std::string objectAssociations(void);

    /**
    * \brief Método de teste para o nome/tipo de instância.
    * \param s	Texto com o nome da classe que é esperado.
    * \return bool	Retorna verdadeiro caso o nome passado seja EFotoManager. Retorna falso no caso contrário.
    */
    bool is(std::string s);

    /**
    * \brief Método para setar os valores de atributos de uma instância de Projeto utilizando sua descrição em xml.
    * \param xml	String contendo o xml com todos os valores de atributos adequados a uma instância da classe Project.
    */
    void setXml(std::string xml);

    /**
    * \brief Método para extrair o equivalente em xml de uma instância de Projeto.
    * \return std::string	Retorna o string contendo o xml para uma instância da classe Project.
    */
    std::string getXml();

    /**
    * \brief Método que retorna um identificador de sensor que ainda não foi utilizado.
    * \return int Identificador da sensor.
    */
    int getFreeSensorId();

    /**
    * \brief Método que retorna um identificador de voo que ainda não foi utilizado.
    * \return int Identificador do voo.
    */
    int getFreeFlightId();

    /**
    * \brief Método que retorna um identificador de imagem que ainda não foi utilizada.
    * \return int Identificador da Imagem.
    */
    int getFreeImageId();

    /**
    * \brief Método que retorna um identificador de ponto que ainda não foi utilizado.
    * \return int Identificador do Ponto.
    */
    int getFreePointId();


    /**
    * \brief Método que checa se a estrutura foi alterada em relação a sua versão salva.
    * \return bool Retorno verdadeiro se a estrutura instanciada equivale ao xml de permanencia para essa estrutura.
    */
    bool getSaveState();

    /**
    * \brief Método que apaga uma instância da classe Sensor.
    * \param id Identificador do Sensor.
    */
    void deleteSensor(int id, bool makeReconnections = true);

    /**
    * \brief Método que apaga uma instância da classe Flight.
    * \param id Identificador do Vôo.
    */
    void deleteFlight(int id, bool makeReconnections = true);

    /**
    * \brief Método que apaga uma instância da classe Image.
    * \param id Identificador da Imagem.
    */
    void deleteImage(int id, bool makeReconnections = true);

    /**
    * \brief Método que apaga uma instância da classe Point.
    * \param id Identificador do Ponto.
    */
    void deletePoint(int id, bool makeReconnections = true);

    /**
    * \brief Método que apaga uma instância da classe InteriorOrientation.
    * \param id Identificador da Orientação Interior.
    */
    void deleteIO(int id, bool makeReconnections = true);

    /**
    * \brief Método que apaga uma instância da classe ExteriorOrientation.
    * \param id Identificador da Orientação Exterior.
    */
    void deleteEO(int id, bool makeReconnections = true);

    void addSensor(std::string data = "", bool makeReconnections = true);

    void addFlight(std::string data = "", bool makeReconnections = true);

    void addImage(std::string data = "", bool makeReconnections = true);

    void addPoint(std::string data = "", bool makeReconnections = true);

    void addIO(std::string data, bool makeReconnections = true);

    void addEO(std::string data, bool makeReconnections = true);

    //std::string getProcessStates() {return processStates;} Excluído o código em Setembro de 2021

    //void setProcessStates(std::string state) {processStates = state;} Excluído o código em Setembro de 2021

    void closeProject();

    // Rever e adicionar aqui os métodos de new, load e save para uso de permanencia de disco
    // Rever e adicionar aqui os métodos de add, instance e delete dos itens Dem, EOI e Feat.
};

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br

#endif // PROJECT_H

