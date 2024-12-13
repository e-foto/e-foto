#include "AboutForm.h"
#include "EDom.h"
#include "revision_info.h"

#include <QFile>
#include <QDate>

AboutForm::AboutForm()
{
  setupUi(this);

  // Ler o base da mensagem descritiva e carregá-la em EDOM
  QString aboutmessage;
  QFile file(":/text/AboutMessage.html");
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }
  QTextStream in(&file);
  while (!in.atEnd()) {
    aboutmessage += in.readLine();
  }
  br::uerj::eng::efoto::EDomElement message(aboutmessage.toUtf8().data());

  // Ler versão
  QString ver = BUILD_DATE;
  QString rev = BUILD_COMMIT_HASH;
  QDate date = QDate::fromString(ver, "MMM dd yyyy");
  if (date.isValid()) {
    int year = date.year();
    int month = date.month();
    ver = QString::number(year) + "." + QString::number(month);
  }

  // Preencher o conteúdo da mensagem descritiva via DOM
  br::uerj::eng::efoto::EDomElement tag;
  QString software_name = "e-foto";
  QString descript_name = tr("A free GNU/GPL educational<br /> digital photogrammetric workstation<br />");
  QString date_ver_name = tr("Version ");
  QString size_ver_name = tr(" (64 bits)");
  date_ver_name += ver + "." + rev + size_ver_name;

  tag = message.elementByTagAtt("span", "id", "software_name");
  tag.addChildAtTagName("span",software_name.toUtf8().data());
  message.replaceChildByTagAtt("span", "id", "software_name",tag.getContent());

  tag = message.elementByTagAtt("span", "id", "descript_name");
  tag.addChildAtTagName("span",descript_name.toUtf8().data());
  message.replaceChildByTagAtt("span", "id", "descript_name",tag.getContent());

  tag = message.elementByTagAtt("span", "id", "date_ver_name");
  tag.addChildAtTagName("span",date_ver_name.toUtf8().data());
  message.replaceChildByTagAtt("span", "id", "date_ver_name",tag.getContent());

  // Inserir mensagem descritiva no formulário
  aboutmessage = QString::fromUtf8(message.getContent().c_str());
  descLabel->setText(aboutmessage);
}
