#include "RasterResource.h"

#include <QMessageBox>

RasterResource::RasterResource(QString filepath, bool withSmoothIn, bool withSmoothOut)
{
	// Abrir a imagem e testar se é válida
	QImage* srcImage = new QImage();
	_isValid = srcImage->load(filepath);
	_levels = 0;
	_useSmoothIn = withSmoothIn;
	_useSmoothOut = withSmoothOut;
	if (!_isValid || srcImage->width() == 0 || srcImage->height() == 0)
	{
		if (filepath != "")
			emitLoadError();
		return;
	}

	// Calcular o numero de niveis da piramide
	_imageDim = srcImage->size();
	_levels =  log(_imageDim.width() < _imageDim.height() ? _imageDim.width() : _imageDim.height() ) / log(2) -4;

	// Alocando espaço para a piramide
	_pyramid = (QImage**) calloc(_levels, sizeof(QImage*));

	// Atribui a imagem original ao primeiro nível
	_pyramid[0] = new QImage(srcImage->convertToFormat(QImage::Format_ARGB32));
	delete(srcImage);
	if (_pyramid[0]->width() == 0 || _pyramid[0]->height() == 0)
	{
		_isValid = false;
		delete(_pyramid[0]);
		free(_pyramid);
		emitLoadError();
		return;
	}

	// Construindo imagens
	for(int l = 1; l < _levels; l++)
	{
		// Cada imagem do novo nível é igual ao nível anterior reduzida pela metade
		int w = _pyramid[l-1]->width()/2;
		int h = _pyramid[l-1]->height()/2;
		_pyramid[l] = new QImage(_pyramid[l-1]->scaled(w,h,Qt::KeepAspectRatioByExpanding, _useSmoothOut ? Qt::SmoothTransformation : Qt::FastTransformation));

		if (_pyramid[l]->width() == 0 || _pyramid[l]->height() == 0)
		{
			_isValid = false;
			for (int k = l; l >= 0; l--)
				delete(_pyramid[k]);
			free(_pyramid);
			emitLoadError();
			return;
		}
	}
}

RasterResource::~RasterResource()
{
	if (!_isValid)
		return;
	for(int l = 0; l < _levels; l++)
		delete(_pyramid[l]);
	free(_pyramid);
}

void RasterResource::emitLoadError()
{
	QMessageBox* msgBox = new QMessageBox();
	msgBox->setText("Error: The image loading process.");
	msgBox->exec();
}

void RasterResource::useSmoothIn(bool useSmooth)
{
	_useSmoothIn = useSmooth;
}

void RasterResource::transformImage(double H[9])
{
	if (_isValid)
	{
		QImage newImage(*_pyramid[0]);
		QTransform h(H[0],H[3],H[6],H[1],H[4],H[7],H[2],H[5],H[8]);
		//qDebug("\n%f %f %f\n%f %f %f\n%f %f %f",H[0],H[3],H[6],H[1],H[4],H[7],H[2],H[5],H[8]);
		load(newImage.transformed(h));
	}
}

bool RasterResource::load(QImage image)
{
	// Impede carga de imagem nula
	if (image.isNull())
	{
		emitLoadError();
		return false;
	}

	// Remove imagem pré-existente
	if (_isValid)
	{
		for(int l = 0; l < _levels; l++)
			delete(_pyramid[l]);
		free(_pyramid);
	}
	_isValid = true;

	// Calcular o numero de niveis da piramide
	_imageDim = image.size();
	_levels =  log(_imageDim.width() < _imageDim.height() ? _imageDim.width() : _imageDim.height() ) / log(2) -4;
	if (_levels < 0) _levels = 0;

	// Alocando espaço para a piramide
	_pyramid = (QImage**) calloc(_levels, sizeof(QImage*));

	// Atribui a imagem original ao primeiro nível
	_pyramid[0] = new QImage(image.convertToFormat(QImage::Format_ARGB32));
	if (_pyramid[0]->width() == 0 || _pyramid[0]->height() == 0)
	{
		_isValid = false;
		delete(_pyramid[0]);
		free(_pyramid);
		emitLoadError();
		return false;
	}

	// Construindo imagens
	for(int l = 1; l < _levels; l++)
	{
		// Cada imagem do novo nível é igual ao nível anterior reduzida pela metade
		int w = _pyramid[l-1]->width()/2;
		int h = _pyramid[l-1]->height()/2;
		_pyramid[l] = new QImage(_pyramid[l-1]->scaled(w,h,Qt::IgnoreAspectRatio, _useSmoothOut ? Qt::SmoothTransformation : Qt::FastTransformation));

		if (_pyramid[l]->width() == 0 || _pyramid[l]->height() == 0)
		{
			_isValid = false;
			for (int k = l; l >= 0; l--)
				delete(_pyramid[k]);
			free(_pyramid);
			emitLoadError();
			return false;
		}
	}
	return _isValid;
}

bool RasterResource::load(QString filepath)
{
	// Abrir a imagem e testar se é válida
	QImage image;
	if (image.load(filepath))
	{
		for(int l = 0; l < _levels; l++)
			delete(_pyramid[l]);
		free(_pyramid);
		_isValid = true;
	}
	else
	{
		return false;
	}

	// Calcular o numero de niveis da piramide
	_imageDim = image.size();
	_levels =  log(_imageDim.width() < _imageDim.height() ? _imageDim.width() : _imageDim.height() ) / log(2) -4;
	if (_levels < 0) _levels = 0;

	// Alocando espaço para a piramide
	_pyramid = (QImage**) calloc(_levels, sizeof(QImage*));

	// Atribui a imagem original ao primeiro nível
	_pyramid[0] = new QImage(image.convertToFormat(QImage::Format_ARGB32));
	if (_pyramid[0]->width() == 0 || _pyramid[0]->height() == 0)
	{
		_isValid = false;
		delete(_pyramid[0]);
		free(_pyramid);
		emitLoadError();
		return false;
	}

	// Construindo imagens
	for(int l = 1; l < _levels; l++)
	{
		// Cada imagem do novo nível é igual ao nível anterior reduzida pela metade
		int w = _pyramid[l-1]->width()/2;
		int h = _pyramid[l-1]->height()/2;
		_pyramid[l] = new QImage(_pyramid[l-1]->scaled(w,h,Qt::IgnoreAspectRatio, _useSmoothOut ? Qt::SmoothTransformation : Qt::FastTransformation));

		if (_pyramid[l]->width() == 0 || _pyramid[l]->height() == 0)
		{
			_isValid = false;
			for (int k = l; l >= 0; l--)
				delete(_pyramid[k]);
			free(_pyramid);
			emitLoadError();
			return false;
		}
	}
	return _isValid;
}

bool RasterResource::save(QString filepath, QString format)
{
    if (_pyramid && _pyramid[0]->save(filepath,format.toLocal8Bit().constData()))
		return true;
	return false;
}

bool RasterResource::isValid()
{
	return _isValid;
}

int RasterResource::width()
{
	return _imageDim.width();
}

int RasterResource::height()
{
	return _imageDim.height();
}

QSize RasterResource::size()
{
	return _imageDim;
}

int RasterResource::levels()
{
	return _levels;
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
	if (_isValid)
	{
		// Inicialização de variáveis para o recorte na imagem do topo da pilha
		QRect rectToCut = imageCut.toRect();
		QImage* img = _pyramid[0];
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
            //comentar depois
			rectToCut.setLeft(rectToCut.left()-1);
			rectToCut.setTop(rectToCut.top()-1);
			rectToCut.setWidth(rectToCut.width()+2);
			rectToCut.setHeight(rectToCut.height()+2);

			// Criamos um newTargetSize para a nova escala com o aspécto do primeiro recorte
			QSize newTargetSize = rectToCut.size() * scale;

			// Um recorte final vai garantir a posição corretamente alinhada e o targetSize pedido
			QRect finalCut(((imageCut.topLeft() - QPointF(rectToCut.topLeft())) * scale).toPoint(), targetSize);

			result = img->copy(rectToCut).scaled(newTargetSize,Qt::KeepAspectRatioByExpanding, _useSmoothIn ? Qt::SmoothTransformation : Qt::FastTransformation).copy(finalCut);
		}
		else if (scale > 0.5)
		{
			// Corta e reduz a imagem a partir do primeiro nível da pirâmide. Este é o caso mais simples
			result = img->copy(rectToCut).scaled(targetSize,Qt::KeepAspectRatioByExpanding, _useSmoothOut ? Qt::SmoothTransformation : Qt::FastTransformation);
		}
		else
		{
			// Procura o nível correto da pirâmide que será utilizado e o novo imageCut
			while (scale <= 0.5 && l<_levels-1)
			{
				scale *= 2;
				l++;
				QPointF center(QPointF(imageCut.center().x()/2.0,imageCut.center().y()/2.0));
				imageCut.setSize(imageCut.size()/2.0);
				imageCut.moveCenter(center);
			}

			// Troca a imagem pela imagem do nível correto, seleciona o novo corte e recorta.
			img = _pyramid[l];
			rectToCut = imageCut.toRect();
			result = img->copy(rectToCut).scaled(targetSize,Qt::KeepAspectRatioByExpanding, _useSmoothOut ? Qt::SmoothTransformation : Qt::FastTransformation);
		}
	}
    //Nem toda tela tem 96 dpi, refazer depois
    //result.setDotsPerMeterX(3780); Excluído o código em Setembro de 2021
    //result.setDotsPerMeterY(3780); Excluído o código em Setembro de 2021
	return result;
}

QColor RasterResource::getColor(QPoint at)
{
	if (_isValid && at.x() >= 0 && at.y() >= 0 && at.x() < width() -1 && at.y() < height() - 1)
		return QColor(_pyramid[0]->pixel(at));
	else
		return QColor();
}

unsigned int RasterResource::getGrayColor(QPointF at, bool linear)
{
	unsigned int result = 0;
	if (_isValid && at.x() >= 0 && at.y() >= 0 && at.x() < width() -1 && at.y() < height() - 1)
	{
		if (linear)
		{
			// adicionar a parte linear aqui depois.
		}
		else result = qGray(_pyramid[0]->pixel((int)floor(at.x()),(int)floor(at.y())));
	}
	return result;
}
