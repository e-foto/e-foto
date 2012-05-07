#include <QApplication>

#include "EfotoMDI.h"

int main(int argc, char **argv)
{
	QApplication aplic(argc,argv);

	EfotoMDI *wind=new EfotoMDI();

	wind->show();

	return aplic.exec();
}
