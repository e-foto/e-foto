#include "AboutForm.h"
#include "EDom.h"

#include <QFile>
#include <QDebug>

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

	// Ler versão e revisão
	QString ver = "YYYY.MM", rev = "DDD";
	QFile version(":/text/version"), revision(":/text/revision");
	if(version.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream date(&version);
        ver = date.readLine();
    }
	if(revision.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream num(&revision);
        rev = num.readLine();
    }

	// Preencher o conteúdo da mensagem descritiva via DOM
	br::uerj::eng::efoto::EDomElement tag;
	QString software_name = "e-foto";
	QString descript_name = tr("A free GNU/GPL educational<br /> digital photogrammetric workstation<br />");
	QString date_ver_name = tr("Version ");
	QString size_ver_name = tr("(64 bits, beta)");
	QString revision_name = tr("Revision ");
	date_ver_name += ver + size_ver_name;
	revision_name += rev;

	tag = message.elementByTagAtt("span", "id", "software_name");
    tag.addChildAtTagName("span",software_name.toUtf8().data());
	message.replaceChildByTagAtt("span", "id", "software_name",tag.getContent());

	tag = message.elementByTagAtt("span", "id", "descript_name");
    tag.addChildAtTagName("span",descript_name.toUtf8().data());
	message.replaceChildByTagAtt("span", "id", "descript_name",tag.getContent());

	tag = message.elementByTagAtt("span", "id", "date_ver_name");
    tag.addChildAtTagName("span",date_ver_name.toUtf8().data());
	message.replaceChildByTagAtt("span", "id", "date_ver_name",tag.getContent());

	tag = message.elementByTagAtt("span", "id", "revision_name");
    tag.addChildAtTagName("span",revision_name.toUtf8().data());
	message.replaceChildByTagAtt("span", "id", "revision_name",tag.getContent());

	// Inserir mensagem descritiva no formulário
	aboutmessage = QString::fromUtf8(message.getContent().c_str());
	descLabel->setText(aboutmessage);
}
