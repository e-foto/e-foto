#include <QApplication>
#include "TestWindow.h"
#include <stdio.h>


int main( int argc, char ** argv )
{

	//for (int i = 1; i <= 250; i++)
	//printf("\t<point key=\"%d\" type=\"control\">\n\t\t<pointId>P%d</pointId>\n\t\t<description>Praça Varnhagem</description>\n\t\t<spatialCoordinates uom=\"#m\">\n\t\t\t<gml:pos>681100.209 7464305.984 12.002</gml:pos>\n\t\t\t<sigma>\n\t\t\t\t<mml:matrix>\n\t\t\t\t\t<mml:matrixrow>\n\t\t\t\t\t\t<mml:cn>1</mml:cn>\n\t\t\t\t\t\t<mml:cn>0</mml:cn>\n\t\t\t\t\t\t<mml:cn>0</mml:cn>\n\t\t\t\t\t</mml:matrixrow>\n\t\t\t\t\t<mml:matrixrow>\n\t\t\t\t\t\t<mml:cn>0</mml:cn>\n\t\t\t\t\t\t<mml:cn>1</mml:cn>\n\t\t\t\t\t\t<mml:cn>0</mml:cn>\n\t\t\t\t\t</mml:matrixrow>\n\t\t\t\t\t<mml:matrixrow>\n\t\t\t\t\t\t<mml:cn>0</mml:cn>\n\t\t\t\t\t\t<mml:cn>0</mml:cn>\n\t\t\t\t\t\t<mml:cn>1</mml:cn>\n\t\t\t\t\t</mml:matrixrow>\n\t\t\t\t</mml:matrix>\n\t\t\t</sigma>\n\t\t</spatialCoordinates>\n\t\t<imagesMeasurements>\n\t\t\t<imageCoordinates uom=\"px\" image_key=\"1\"><gml:pos>2196 2445</gml:pos></imageCoordinates>\n\t\t\t<imageCoordinates uom=\"px\" image_key=\"2\"><gml:pos>1206 2415</gml:pos></imageCoordinates>\n\t\t\t<imageCoordinates uom=\"px\" image_key=\"3\"><gml:pos>167 2529</gml:pos></imageCoordinates>\n\t\t</imagesMeasurements>\n\t</point>\n",i,i);

	QApplication a(argc, argv);

	a.setStyle("plastique");
	a.quitOnLastWindowClosed();

	InitWindow w;
	w.show();

	return a.exec();

	// To Do
	/*
   1.
   Alterar o programa para podermos substituir o funcionamento da classe RasterResource.
   Alternativamente ela vai se ligar ao conjunto de classes da terraLib e deixar que a terraLib se encarregue
   de entregar imagens do disco ou de um base.

   2.
   Desenhar os cursores no stereo. Marcar a área visível sobre o overview. Movimentar a visualização do detalhe quando a
   marca de medição se mover no stereo. Permitir que esta marca seja controlada pelo mouse ou joystick.
   Manter o cursor atualizado entre o stereoDisplay e seus dois singleDisplay associados evitando ainda que a
   marca de medição saia da área visível devido a uma diferença de aspécto entre os displays.

   3.
   Tratar de corrigir os lixos de código... generalizar o máximo possível de métodos de controle dos displays,
   garantir que o estéreo funciona bem e sabe trocar entre os modos anaglífo, interlaçado e polarizado/shutter.

   5.
   Integrar com a fotogrametria e com o te.geo

   6.
   Iniciar o uso dos displays no e-foto
   */
}
