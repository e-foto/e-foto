#include "RasterResource.h"
#include "math.h"

RasterResource::RasterResource(QString filepath)
{
	// Abrir a imagem e testar se é válida
	QImage* srcImage = new QImage();
	isValid_ = srcImage->load(filepath);
	levels_ = 0;
	if (!isValid_)
		return;

	// Calcular o numero de niveis da piramide
	imageDim_ = srcImage->size();
	levels_ =  log(imageDim_.width() < imageDim_.height() ? imageDim_.width() : imageDim_.height() ) / log(2) -4;

	// Alocando espaço para a piramide
	pyramid_ = (QImage**) calloc(levels_, sizeof(QImage*));

	// Atribui a imagem original ao primeiro nível
	pyramid_[0] = srcImage;

	// Construindo imagens
	for(int l = 1; l < levels_; l++)
	{
		// Cada imagem do novo nível é igual ao nível anterior reduzida pela metade
		int w = pyramid_[l-1]->width()/2;
		int h = pyramid_[l-1]->height()/2;
		pyramid_[l] = new QImage(pyramid_[l-1]->scaled(w,h));
	}
}

RasterResource::~RasterResource()
{
	if (!isValid_)
		return;
	for(int l = 0; l < levels_; l++)
		delete(pyramid_[l]);
	free(pyramid_);
}

bool RasterResource::isValid()
{
	return isValid_;
}

int RasterResource::width()
{
	return imageDim_.width();
}

int RasterResource::height()
{
	return imageDim_.height();
}

QSize RasterResource::size()
{
	return imageDim_;
}

int RasterResource::levels()
{
	return levels_;
}

QPointF RasterResource::center()
{
	return QPointF(width()/2.0,height()/2.0);
}

QImage RasterResource::getImageCut(QSize targetSize, QRectF imageCut)
{
	// Este método poderia possuir a assinatura getImageCut(QSize targetSize, QPointF viewPoint, double scale)
	// Contudo devido a possível incompatibilidade com o TeCanvas ele ficou da forma atual

	QImage result;
	if (isValid_)
	{
		// Inicialização de variáveis para o recorte na imagem do topo da pilha
		QRect rectToCut = imageCut.toRect();
		QImage* img = pyramid_[0];
		int l = 0;

		// Na prática, qualquer corte deveria possuir a mesma escala em width e height
		// mas isso não ocorre para o recorte de Overview. Por isso recuperamos a escala assim:
		double wscale = targetSize.width() / imageCut.width();
		double hscale = targetSize.height() / imageCut.height();
		double scale = (wscale > hscale) ? wscale : hscale;

		// Seleciona a imagem correta a recortar, faz o recorte e ajustes para perfeito encaixe do centro do recorte na imagem com a imagem resultante
		if (scale > 1.0)
		{
			// Este caso precisa ser tratado com atenção.
			// Precisamos manter o aspécto da imagem, o zoom correto e o ponto central do recorte corretamente alinhado

			// Primeiro evitamos problemas com o espaço recortado aumentando ligeiramente a área de recorte
			rectToCut.setLeft(rectToCut.left()-1);
			rectToCut.setTop(rectToCut.top()-1);
			rectToCut.setWidth(rectToCut.width()+2);
			rectToCut.setHeight(rectToCut.height()+2);

			// Criamos um newTargetSize para a nova escala com o aspécto do primeiro recorte
			QSize newTargetSize = rectToCut.size() * scale;

			// Um recorte final vai garantir a posição corretamente alinhada e o targetSize pedido
			QRect finalCut(((imageCut.topLeft() - QPointF(rectToCut.topLeft())) * scale).toPoint(), targetSize);

			result = img->copy(rectToCut).scaled(newTargetSize,Qt::KeepAspectRatioByExpanding).copy(finalCut);
		}
		else if (scale > 0.5)
		{
			// Corta e reduz a imagem a partir do primeiro nível da pirâmide. Este é o caso mais simples
			result = img->copy(rectToCut).scaled(targetSize,Qt::KeepAspectRatioByExpanding);
		}
		else
		{
			// Procura o nível correto da pirâmide que será utilizado e o novo imageCut
			while (scale <= 0.5 && l<levels_-1)
			{
				scale *= 2;
				l++;
				QPointF center(QPointF(imageCut.center().x()/2.0,imageCut.center().y()/2.0));
				imageCut.setSize(imageCut.size()/2.0);
				imageCut.moveCenter(center);
			}

			// Troca a imagem pela imagem do nível correto, seleciona o novo corte e recorta.
			img = pyramid_[l];
			rectToCut = imageCut.toRect();
			result = img->copy(rectToCut).scaled(targetSize,Qt::KeepAspectRatioByExpanding);
		}
	}
	return result;
}
