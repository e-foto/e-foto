/**************************************************************************
		  DatabaseConnection.cpp
**************************************************************************/

#include "DatabaseConnection.h"

namespace br {
namespace uerj {
namespace eng {
namespace efoto {

DatabaseConnection::DatabaseConnection()
{
    conected=false;
}


DatabaseConnection::~DatabaseConnection()
{

}

bool DatabaseConnection::isConected()
{
    return conected;
}

bool DatabaseConnection::connect(std::string dbms_name_, std::string host_, std::string user_, std::string password_, int port_, std::string database_, int databaseMode)
{
    params.dbms_name_ = dbms_name_;
    params.host_ = host_;
    params.user_ = user_;
    params.password_ = password_;
    params.port_ = port_;
    params.database_ = database_;

    if(databaseMode==0)
    {
        if(pService.createDatabase(params) == false)
        {
                std::cout << "---------NÃO Criou o banco" << std::endl;
                std::cout << "---------Erro: "<< pService.getErrorMessage() << std::endl;
                return false;
        }
    }

    if(pService.connect(params) == false)
    {
            std::cout << "---------NÃO Conectou ao banco" << std::endl;
            std::cout << "---------Erro: "<< pService.getErrorMessage() << std::endl;
            return false;
    }
    conected=true;
    return true;
}

void DatabaseConnection::disconnect()
{
    pService.disconnect();
    conected=false;
}

void DatabaseConnection::existo()
{
    std::cout<<"Estou em DatabaseConnection"<<std::endl;
}

} // namespace efoto
} // namespace eng
} // namespace uerj
} // namespace br
