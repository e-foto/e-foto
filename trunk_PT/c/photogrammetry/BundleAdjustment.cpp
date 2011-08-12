#include "BundleAdjustment.h"
#include "Dms.h"
/*
pontos de controle
  omega       phi         kappa       X0      Y0      Z0
deltaOmega  deltaPhi    deltaKappa  deltaX0 deltaY0 deltaZ0

pontos fotogrametricos
0     0      0       X0     Y0     Z0
0     0      0     deltaX deltaY deltaZ

AFP
1.7697356457    -0.7688118809   -1.1453517757   680604.933846218    7465090.87025837    1317.1186409173
	0                   0               -0        -0.0000000032       -0.0000000013      -0.0000000094
0.6647216963    -0.899885954    -1.7753587946   681317.250762181    7465080.89358158    1321.0706144451
	-0           0.0000000001   0.0000000001       0.0000001227       -0.0000000023     -0.0000002349
	0                   0               0       680541.006295581    7464680.17672666    11.3455967171
	0                   0               0           0.0173889526        0.0038905975    -0.0043300409
	0                   0               0       680931.229759843    7465299.79667213    68.8406838744
	0                   0               0           -0.0035636276       0.6617979107    -2.4392336491

15 iteraÃ§Ãµes e 4 mudanÃ§a de pesos
*/
#include <qdebug.h>

#define MAXITERATIONS 10
#define CONVERGENCY 0.001
#define MAXRESIDUO 0.001

BundleAdjustment::BundleAdjustment()
{
	Matrix dummy(8,8);
	for (int i=1; i<=8; i++)
		for (int j=1; j<=8; j++)
			dummy.set(i,j,i*8+j);
//	setAFP(dummy);
//	for (int j=1;j<=dummy.getCols();j++)
//		qDebug("%f",dummy.get(1,j));
	c     = 0.0;
	fliDir= 0.0;
}

	/* c=153.528;*/

/*
	L[1][1]=1848;  C[1][1]=5133;
	L[1][2]=2135;  C[1][2]=9734;
	L[1][3]=10016; C[1][3]=8328;
	L[1][4]=8096;  C[1][4]=5300;
	L[1][5]=4498;  C[1][5]=7464;

	L[2][1]=1716;  C[2][1]=1038;
	L[2][2]=1989;  C[2][2]=5763;
	L[2][3]=9893;  C[2][3]=4262;
	L[2][4]=8019;  C[2][4]=1231;
	L[2][5]=4402;  C[2][5]=3246;
*/

	/*Matrizes das coordenadas dos pontos do bloco. As imagens estÃÂ£o representadas nas linhas e os pontos nas
	colunas, onde o nÃÂºmero da linha ÃÂ© o mesmo do identificador da imagem e o da coluna, o mesmo do identificador do ponto.
	Valor -1 indica ponto fotogramÃÂ©trico, e valor 0 indica ponto nÃÂ£o contido na imagem.
	*/
	//espaÃ§o Objeto

	/*
	X={{    0    ,     0     ,     0     ,     0     ,    0    ,    0    },
	   {    0    , 680539.053, 681377.247, 681065.362,   -1    ,   -1    },
	   {    0    , 680539.053, 681377.247, 681065.362,   -1    ,   -1    }

	Y={{    0    ,     0      ,     0      ,     0      ,    0    ,    0    },
	   {    0    , 7465798.024, 7465732.756, 7464329.844,   -1    ,   -1    },
	   {    0    , 7465798.024, 7465732.756, 7464329.844,   -1    ,   -1    }
	  };
	Z={{  0  ,   0   ,   0   ,   0   ,  0 ,  0 },
	   {  0  , 18.475, 10.725, 11.738, -1 , -1 },
	   {  0  , 18.475, 10.725, 11.738, -1 , -1 }
	  };*/

	/* Matriz de configuraÃÂ§ÃÂ£o do bloco. Valor 1 - ponto de controle, valor -1, ponto fotogramÃÂ©trico, valor 0, ponto nÃÂ£o
	contido na imagem. linha = imagem, coluna = ponto.
	*/
	/*
	blc={{    0    ,     0     ,     0     ,     0     ,    0    ,    0    },
		 {    0    ,     1     ,     1     ,     1     ,   -1    ,   -1    },
		 {    0    ,     1     ,     1     ,     1     ,   -1    ,   -1    }
		}

	/*Resultado da OI das imagens;*/
  //  double matrixXA16[6]={-118.9025073577284400,0.021001571200814785,-0.00013043219281323558,116.7800380235976800,-0.000121134075620769601,-0.0209922789399759820};
  //  double matrixXA17[6]={-118.7921178522728400,0.0210042583045778,-0.0000211860692717,116.0421313171841900,-0.0000157955917203,-0.0209949671357402};

BundleAdjustment::BundleAdjustment(deque<Image*>listImages, deque<Point*> listPoints, int flightDirection)
{
	this->listImages=listImages;
	this->listPoints=listPoints;
	fliDir=flightDirection;

	numImages=listImages.size();
	numPoints=listPoints.size();
	numControlPoints=0;
	numFotogrametricPoints=0;

	for (int i=0;i<numPoints;i++)
	{
		if(listPoints.at(i)->is("ControlPoint"))
			numControlPoints++;
		else if(listPoints.at(i)->is("PhotogrammetricPoint"))
			numFotogrametricPoints++;
	}

	fillAnalogCoordinates();
	numEquations=numberOfEquations();
	numUnknows=6*numImages+3*numFotogrametricPoints;

	/** ser´a eliminiado em breve*/
	c= listImages.at(0)->getSensor()->getFocalDistance();
	xsi0=listImages.at(0)->getSensor()->getPrincipalPointCoordinates().getXi();
	eta0=listImages.at(0)->getSensor()->getPrincipalPointCoordinates().getEta();

}

int BundleAdjustment::numberOfEquations()
{
	int n=0;
	for (int i=0;i<numImages;i++)
		n+=numberControlPoints(listImages.at(i))+numberPhotogrammetricPoints(listImages.at(i));
	return 2*n;
}

Point *BundleAdjustment::getPointFrom(int imageIndex, int pointIndex)
{
	return listImages.at(imageIndex)->getPointAt(pointIndex);
}

void BundleAdjustment::fillAnalogCoordinates()
{
	for (int i=0; i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		for (int j=0;j<pnts;j++)
		{
			double col=getPointFrom(i,j)->getDigitalCoordinate(listImages.at(i)->getId()).getCol();
			double lin=getPointFrom(i,j)->getDigitalCoordinate(listImages.at(i)->getId()).getLin();
			Matrix coord=digitalToAnalog(listImages.at(i)->getIO(),lin,col);
			AnalogImageSpaceCoordinate aux(listImages.at(i)->getId());
			aux.setXi(coord.get(1,1));
			aux.setEta(coord.get(1,2));
			getPointFrom(i,j)->putAnalogCoordinate(aux);
		}
	}
}

bool BundleAdjustment::calculate()
{
	getInicialsValues();

	matAdjust.show('f',5,"matAdjust Inicial Values");
	P.identity(numEquations);
	bool resOk=false;
	int changePesos=0;
	Matrix tempRes;
	totalIterations=0;


	while(!resOk)
	{
		int iterations=0;
		bool converge=false;
		while(iterations<MAXITERATIONS && !converged)
		{
			createA1();
			A1.show('f',3,"A1");
			createA2();
			A2.show('f',3,"A2");
			createL0();
			createLb();
			Matrix l=Lb-L0;
			Matrix n11=getN11();
			Matrix n12=getN12();
			Matrix n22=getN22();
			Matrix n1=getn1(l);
			Matrix n2=getn2(l);
			setx1(getx1(n11,n12,n22,n1,n2));
			setx2(getx2(n12,n22,n2,x1));
			//matAdjust.show('f',5,"MatAdjus dentro do loop");
			updateMatAdjust();
			updateCoordFotog();
			converge=testConverged();
			iterations++;
		}
		//matAdjust.show('f',5,"MatAdjus depois da iteracao");
		calculateResiduos();
		//matRes.show('f',5,"MatRes depois da iteracao");
		resOk=testResiduo();
		if (changePesos==0)
			tempRes.resize(matRes.getRows(),1);
		calculatePeso();
		matRes=matRes-tempRes;
		tempRes=matRes;
		changePesos++;
		totalIterations+=iterations;
		//matRes.show('f',5,"MatRes");
	}

	printf("Numero de troca de pesos: %d\n",changePesos);
	calculateResiduos();
	//matRes.show('f',8,"MatResCalculate");
	setAFP();
	return true;
}


/* metodos de calculo*/
Matrix BundleAdjustment::getM11(Matrix M1)
{
	return M1.transpose()*M1;
}

Matrix BundleAdjustment::getM12(Matrix M1, Matrix M2)
{
	return M1.transpose()*M2;
}

Matrix BundleAdjustment::getM22(Matrix M2)
{
	return M2.transpose()*M2;
}

Matrix BundleAdjustment::getm1(Matrix M1, Matrix L1)
{
	return (M1.transpose())*L1;
}

Matrix BundleAdjustment::getm2(Matrix M2, Matrix L1)
{
	return (M2.transpose())*L1;
}

Matrix BundleAdjustment::getPAf(Matrix M11, Matrix M12, Matrix M22, Matrix m1, Matrix m2)
{
	return ( (M11-M12*M22.inverse()*M12.transpose()).inverse() )*( m1-M12*M22.transpose()*m2 );
}

// Aproximaçao inicial para as coordenadas planimetricas(XY) dos pontos fotogrametricos
Matrix BundleAdjustment::getXYpf(Matrix M22, Matrix m2, Matrix M12, Matrix paf)
{
	return M22.inverse()*m2-M22.inverse()*M12.transpose()*paf;
}

Matrix BundleAdjustment::getN11()
{
	return A1.transpose()*P*A1;
}

Matrix BundleAdjustment::getN12()
{
	return A1.transpose()*P*A2;
}

Matrix BundleAdjustment::getN22()
{
	return A2.transpose()*A2;
}

Matrix BundleAdjustment::getn1(Matrix L)
{
	return A1.transpose()*P*L;
}

Matrix BundleAdjustment::getn2(Matrix L)
{
	return A2.transpose()*L;
}

Matrix BundleAdjustment::getx1(Matrix N11, Matrix N12, Matrix N22, Matrix n1, Matrix n2)
{
	return (N11-N12*N22.inverse()*N12.transpose()).inverse()*(n1-N12*N22.inverse()*n2);
}

Matrix BundleAdjustment::getx2(Matrix N12, Matrix N22, Matrix n2, Matrix x1)
{
	return N22.inverse()*n2-N22.inverse()*N12.transpose()*x1;
}

void BundleAdjustment::setx1(Matrix x1)
{
	this->x1=x1;
}

void BundleAdjustment::setx2(Matrix x2)
{
	this->x2=x2;
}

void BundleAdjustment::setRot(double omega, double phi, double kappa)
{
	r11=cos(phi) * cos(kappa);
	r12=-cos(phi)*sin(kappa);
	r13=sin(phi);
	r21=cos(omega) * sin(kappa) + sin(omega) * sin(phi) * cos(kappa);
	r22=cos(omega) * cos(kappa) - sin(omega) * sin(phi) * sin(kappa);
	r23=-sin(omega) * cos(phi);
	r31=sin(omega) * sin(kappa) - cos(omega) * sin(phi) * cos(kappa);
	r32=sin(omega) * cos(kappa) + cos(omega) * sin(phi) * sin(kappa);
	r33=cos(omega) * cos(phi);
}

Matrix BundleAdjustment::getCoordinatesEqColin(double X, double Y, double Z, int imageId)
{
	Matrix coord(1,2);
	double omega=matAdjust.get(imageId,1);
	double phi=matAdjust.get(imageId,2);
	double kappa=matAdjust.get(imageId,3);
	double X0=matAdjust.get(imageId,4);
	double Y0=matAdjust.get(imageId,5);
	double Z0=matAdjust.get(imageId,6);

	setRot(omega,phi,kappa);
	double xi=xsi0-c*(r11*(X-X0)+r21*(Y-Y0)+r31*(Z-Z0))/(r13*(X-X0)+r23*(Y-Y0)+r33*(Z-Z0));
	double eta=eta0-c*(r12*(X-X0)+r22*(Y-Y0)+r32*(Z-Z0))/(r13*(X-X0)+r23*(Y-Y0)+r33*(Z-Z0));

	coord.set(1,1,xi);
	coord.set(1,2,eta);

	return coord;
}

Matrix BundleAdjustment::getJacobianaControl(double X, double Y, double Z, int imageIndex)
{
	Matrix JacCtrl(2,6);

	double omega=matAdjust.get(imageIndex,1);
	double phi=matAdjust.get(imageIndex,2);
	double kappa=matAdjust.get(imageIndex,3);
	double X0=matAdjust.get(imageIndex,4);
	double Y0=matAdjust.get(imageIndex,5);
	double Z0=matAdjust.get(imageIndex,6);

	JacCtrl.set(1,1,c*cos(kappa)*cos(phi)/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*sin(phi));
	JacCtrl.set(1,2,-c*(-sin(kappa)*cos(omega)-cos(kappa)*sin(phi)*sin(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*cos(phi)*sin(omega));
	JacCtrl.set(1,3,-c*(-sin(kappa)*sin(omega)+cos(kappa)*sin(phi)*cos(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*cos(phi)*cos(omega));
	JacCtrl.set(1,4,-c*((-sin(kappa)*sin(omega)+cos(kappa)*sin(phi)*cos(omega))*(Y-Y0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*(-cos(phi)*cos(omega)*(Y-Y0)-cos(phi)*sin(omega)*(Z-Z0)));
	JacCtrl.set(1,5,-c*(-cos(kappa)*sin(phi)*(X-X0)+cos(kappa)*cos(phi)*sin(omega)*(Y-Y0)-cos(kappa)*cos(phi)*cos(omega)*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*(cos(phi)*(X-X0)+sin(phi)*sin(omega)*(Y-Y0)-sin(phi)*cos(omega)*(Z-Z0)));
	JacCtrl.set(1,6,-c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)));

	JacCtrl.set(2,1,-c*sin(kappa)*cos(phi)/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*sin(phi));
	JacCtrl.set(2,2,-c*(-cos(kappa)*cos(omega)+sin(kappa)*sin(phi)*sin(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*cos(phi)*sin(omega));
	JacCtrl.set(2,3,-c*(-cos(kappa)*sin(omega)-sin(kappa)*sin(phi)*cos(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*cos(phi)*cos(omega));
	JacCtrl.set(2,4,-c*((-cos(kappa)*sin(omega)-sin(kappa)*sin(phi)*cos(omega))*(Y-Y0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*(-cos(phi)*cos(omega)*(Y-Y0)-cos(phi)*sin(omega)*(Z-Z0)));
	JacCtrl.set(2,5,-c*(sin(kappa)*sin(phi)*(X-X0)-sin(kappa)*cos(phi)*sin(omega)*(Y-Y0)+sin(kappa)*cos(phi)*cos(omega)*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow((sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)),2)*(cos(phi)*(X-X0)+sin(phi)*sin(omega)*(Y-Y0)-sin(phi)*cos(omega)*(Z-Z0)));
	JacCtrl.set(2,6,-c*(-cos(kappa)*cos(phi)*(X-X0)+(-sin(kappa)*cos(omega)-cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(-sin(kappa)*sin(omega)+cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0)));

	return JacCtrl;
}

Matrix BundleAdjustment::getJacobianaFotogrametric(double X, double Y, double Z, int imageIndex)
{
	Matrix JacFoto(2,3);

	double omega=matAdjust.get(imageIndex,1);
	double phi=matAdjust.get(imageIndex,2);
	double kappa=matAdjust.get(imageIndex,3);
	double X0=matAdjust.get(imageIndex,4);
	double Y0=matAdjust.get(imageIndex,5);
	double Z0=matAdjust.get(imageIndex,6);


	JacFoto.set(1,1,-c*cos(kappa)*cos(phi)/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*sin(phi));
	JacFoto.set(1,2,-c*(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*sin(omega));
	JacFoto.set(1,3,-c*(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*cos(omega));

	JacFoto.set(2,1,c*sin(kappa)*cos(phi)/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*sin(phi));
	JacFoto.set(2,2,-c*(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*sin(omega));
	JacFoto.set(2,3,-c*(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*cos(omega));

	return JacFoto;
}

//retorna os parametros da imagem numero imageID
Matrix BundleAdjustment::getPTA(Matrix PAf, int imageId)
{
	Matrix result(6,1);
	for (int i=6*imageId+1,j=1 ; i<=6*(imageId+1) ; i++,j++)
		result.set(j,1,PAf.get(i,1));
	return result;
}

double BundleAdjustment::getKappaZero(Matrix pta,int imageId)
{
	//double kappa0=0.002;
	//mf[0]=MFX mf[1]=MFY

	//coordenadas das marcas fiduciais em milimetros
	double coluna=0.016;
	double linha=113.0;

	double mfX= pta.get(1,1) + pta.get(2,1)*coluna + pta.get(3,1)*linha;
	double mfY= pta.get(4,1) + pta.get(5,1)*coluna + pta.get(6,1)*linha;

	double Ox=(pta.get(1,1) + pta.get(2,1)*xsi0 + pta.get(3,1)*eta0);
	double Oy=(pta.get(4,1) + pta.get(5,1)*xsi0 + pta.get(6,1)*eta0);

	double dX=mfX-Ox;
	double dY=mfY-Oy;

	double kappa0;
	if (fabs(dY)<1e-14 && mfX>Ox)
		kappa0= 0;
	else if (fabs(dY)<1e-14 && mfX<Ox)
		kappa0= M_PI;
	else if (fabs(dX)<1e-14 && mfY>Oy)
		kappa0=M_PI/2;
	else if (fabs(dX)<1e-14 && mfY<Oy)
		kappa0= 3*M_PI/2;
	else if (fabs(dX)>1e-14 && fabs(dY)>1e-14)
	{
		double alpha=atan(fabs(dY/dX));
		if (dY > 0 && dX > 0)
			return alpha;
		if (dY > 0 && dX < 0)
			return M_PI-alpha;
		if (dY < 0 && dX < 0)
			return M_PI+alpha;
		if (dY < 0 && dX > 0)
			return -alpha;
	}
	return kappa0;
}

void BundleAdjustment::imprime(Matrix mat, char *id)
{
	printf("%s\n",id);
	int rows=mat.getRows();
	int cols=mat.getCols();
	for (int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
			printf("%.6f\t",mat.get(i+1,j+1));
		printf("\n");
	}
}

/* metodos auxialiares para buscar dados*/
double BundleAdjustment::getdOmegax1(int imageId)
{
	return x1.get(6*(imageId-1)+4,1);
}

double BundleAdjustment::getdPhix1(int imageId)
{
	return x1.get(6*(imageId-1)+5,1);
}

double BundleAdjustment::getdKappax1(int imageId)
{
	return x1.get(6*(imageId-1)+6,1);
}

double BundleAdjustment::getdXx1(int imageId)
{
	return x1.get(6*(imageId-1)+1,1);
}

double BundleAdjustment::getdYx1(int imageId)
{
	return x1.get(6*(imageId-1)+2,1);
}

double BundleAdjustment::getdZx1(int imageId)
{
	return x1.get(6*(imageId-1)+3,1);
}

double BundleAdjustment::getdXx2(int fotogrPointId)
{
	return x2.get(3*(fotogrPointId-1)+1,1);
}

double BundleAdjustment::getdYx2(int fotogrPointId)
{
	return x2.get(3*(fotogrPointId-1)+2,1);
}

double BundleAdjustment::getdZx2(int fotogrPointId)
{
	return x2.get(3*(fotogrPointId-1)+3,1);
}

double BundleAdjustment::getOmegaAdjus(int imageId)
{
	return matAdjust.get(imageId,1);
}

double BundleAdjustment::getPhiAdjus(int imageId)
{
	return matAdjust.get(imageId,2);
}

double BundleAdjustment::getKappaAdjus(int imageId)
{
	return matAdjust.get(imageId,3);
}

double BundleAdjustment::getXAdjus(int imageId)
{
	return matAdjust.get(imageId,4);
}

double BundleAdjustment::getYAdjus(int imageId)
{
	return matAdjust.get(imageId,5);
}

double BundleAdjustment::getZAdjus(int imageId)
{
	return matAdjust.get(imageId,6);
}

// Procura onde esta o ponto na lista de todos os pontos e retorna o seu indice na lista
int BundleAdjustment::whereInPoints(Point *pnt)
{
	for (int i=0;i<numPoints;i++)
		if (pnt==listPoints.at(i))
			return i;
	return -1;
}

void BundleAdjustment::updateMatAdjust()
{
	for (int i=1;i<=numImages;i++)
	{
		matAdjust.set(i,1, getOmegaAdjus(i) + getdOmegax1(i) );//ajustando omega
		matAdjust.set(i,2, getPhiAdjus(i)   + getdPhix1(i)   );//ajustando phi
		matAdjust.set(i,3, getKappaAdjus(i) + getdKappax1(i) );//ajustando kappa
		matAdjust.set(i,4, getXAdjus(i)     + getdXx1(i)     );//ajustando X
		matAdjust.set(i,5, getYAdjus(i)     + getdYx1(i)     );//ajustando Y
		matAdjust.set(i,6, getZAdjus(i)     + getdZx1(i)     );//ajustando Z
	}
}

/*funções para o calculo dos residuos*/
Matrix BundleAdjustment::getCoordColinearTerrain(double xsi, double eta, double z, int imageId)
{
	Matrix result(1,2);
	//imprime(matAdjust,"matAdjust");
	setRot(getOmegaAdjus(imageId),getPhiAdjus(imageId),getKappaAdjus(imageId));

	double x=getXAdjus(imageId)+(z-getZAdjus(imageId))*(r11*(xsi-xsi0)+r12*(eta-eta0)-r13*c)/(r31*(xsi-xsi0)+r32*(eta-eta0)-r33*c);
	double y=getYAdjus(imageId)+(z-getZAdjus(imageId))*(r21*(xsi-xsi0)+r22*(eta-eta0)-r23*c)/(r31*(xsi-xsi0)+r32*(eta-eta0)-r33*c);
   // printf("\nX: %.7f\n",x);

	result.set(1,1,x);
	result.set(1,2,y);
	return result;
}

/** omega, phi, kappa, X0, Y0, Z0)*/
void BundleAdjustment::getInicialsValues()
{
	Matrix L=createL();
	Matrix M1=createM1();
	M1.show('f',3,"M1");
	Matrix M2=createM2();
	M2.show('f',3,"M2");
	Matrix m11=getM11(M1);
	Matrix m12=getM12(M1,M2);
	m12.show('f',3,"M12");

	Matrix m22=getM22(M2);
	m22.show('f',3,"M22");
	Matrix m1=getm1(M1,L);
	Matrix m2=getm2(M2,L);

	Matrix paf=getPAf(m11,m12,m22,m1,m2);
	Matrix xypf=getXYpf(m22,m2,m12,paf);

	updateCoordinatesAllPoints(xypf,getInicialZPhotogrammetricPoints());

	Matrix temp(numImages,6);
	matAdjust=temp;
	for (int i=0;i<numImages;i++)
	{
		xsi0=listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getXi();
		eta0=listImages.at(i)->getSensor()->getPrincipalPointCoordinates().getEta();
		c=listImages.at(i)->getSensor()->getFocalDistance();

		Matrix pta=getPTA(paf,i);

		matAdjust.set(i+1,1,0);
		matAdjust.set(i+1,2,0);
		matAdjust.set(i+1,3,getKappaZero(pta,i));
		matAdjust.set(i+1,4,pta.get(1,1) + pta.get(2,1)*xsi0 + pta.get(3,1)*eta0);
		matAdjust.set(i+1,5,pta.get(4,1) + pta.get(5,1)*xsi0 + pta.get(6,1)*eta0);
		matAdjust.set(i+1,6,c*getMediaScale(i));
	}
}

Matrix BundleAdjustment::createL()
{
	Matrix L(0,1);
	for (int i=0;i<numImages;i++)
	{
		int numpnts=listImages.at(i)->countPoints();
		for(int j=0;j<numpnts;j++)
		{
			Matrix temp(2,1);
			// talvez esse teste possa ser eliminado, tendo em vista que a matriz sempre e inicializada com zeros
			if(isPhotogrammetricPoint(i,j))
			{
				temp.set(1,1,0.0);
				temp.set(2,1,0.0);
			}
			else if(isControlPoint(i,j))
			{
				temp.set(1,1,listImages.at(i)->getPointAt(j)->getObjectCoordinate().getX());
				temp.set(2,1,listImages.at(i)->getPointAt(j)->getObjectCoordinate().getY());
			}
			L=L|temp;
		}
	}
	//imprime(L,"Lobject");
	return L;
}

Matrix BundleAdjustment::createM1()
{
	Matrix result(0,0);
	for (int i=0;i<numImages;i++)
	{
		Matrix temp=imageToMatrixAnalogicalCoordenates(listImages.at(i));
		result.putMatrix(temp,result.getRows()+1,6*i+1);
	}
	//imprime(result,"M1object");
	return result;
}

Matrix BundleAdjustment::createM2()
{
	Matrix result(0,2*numFotogrametricPoints);
	/* configuraçao de cada matriz B
		 |-1  0|
		 | 0 -1|
	*/
	Matrix B(2,2);
	B.set(1,1,-1);
	B.set(2,2,-1);
	for (int i=0;i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		Matrix oneImage(0,2*numFotogrametricPoints);
		for (int j=0;j<pnts;j++)
		{
			if(isPhotogrammetricPoint(i,j))
			{
				int pos=whereInPoints(getPointFrom(i,j));
			//	qDebug("put in i=%d j=%d",2*j+1,2*(pos-numControlPoints)+1);//2*
				oneImage.putMatrix(B,2*j+1,2*(pos-numControlPoints)+1);
			}
		}
		//oneImage.show('f',3,"OneImage");
		result=result|oneImage;
		//result.show('f',3,"Result");
	}

	//imprime(result,"M2object");
	return result;
}

// Retorna uma matriz correspondente a uma imagem na matriz M
Matrix BundleAdjustment::imageToMatrixAnalogicalCoordenates(Image *img)
{
	int numpnts=img->countPoints();

	Matrix result(0,6);
	for (int i=0;i<numpnts;i++)
	{
		/* configuraçao de cada matriz temp
				   |1	eta xi	0	 0		0 |
				   |0	 0	0	1	eta 	xi|
		*/
		Matrix temp(2,6);
		/** conversao digital to analog sera feito pela classe do Rafael em breve */
				int lin=img->getPointAt(i)->getDigitalCoordinate(img->getId()).getLin();
				int col=img->getPointAt(i)->getDigitalCoordinate(img->getId()).getCol();

				//qDebug("object col: %d\t lin: %d",col, lin);
				Matrix coord=digitalToAnalog(img->getIO(),lin,col);

				temp.set(1,1,1);
				temp.set(1,2,coord.get(1,2));
				temp.set(1,3,coord.get(1,1));
				temp.set(2,4,1);
				temp.set(2,5,coord.get(1,2));
				temp.set(2,6,coord.get(1,1));

				result=result|temp;
	}
	   // imprime(result,"image");
		return result;
}

Matrix BundleAdjustment::digitalToAnalog(InteriorOrientation *oi,int linha, int coluna)
{
		double a0, a1, a2, b0, b1, b2;

		a0 = oi->getXa().get(1,1);
		a1 = oi->getXa().get(2,1);
		a2 = oi->getXa().get(3,1);
		b0 = oi->getXa().get(4,1);
		b1 = oi->getXa().get(5,1);
		b2 = oi->getXa().get(6,1);

		//imprime(oi->getXa(),"Xa");

		Matrix result(1,2);
		result.set(1,1, a0 + a1*coluna + a2*linha);
		result.set(1,2, b0 + b1*coluna + b2*linha);
		return result;
}

Matrix BundleAdjustment::analogToDigital(InteriorOrientation *oi,double xsi, double eta)
{
		double a0, a1, a2, b0, b1, b2;
		a0 = oi->getXa().get(1,1);
		a1 = oi->getXa().get(2,1);
		a2 = oi->getXa().get(3,1);
		b0 = oi->getXa().get(4,1);
		b1 = oi->getXa().get(5,1);
		b2 = oi->getXa().get(6,1);
		//Ideia do Rafael Aguiar para contornar erro de cast (somar 0.1)
		Matrix result(1,2);
		result.setInt(1,1, (int)((b2*xsi - b2*a0 - a2*eta + b0*a2) / (a1*b2 - b1*a2) + 0.1 ) );
		result.setInt(1,2, (int)((a1*eta - a1*b0 - b1*xsi + b1*a0) / (a1*b2 - b1*a2) + 0.1 ) );
		return result;
}

int BundleAdjustment::numberControlPoints(Image *img)
{
	int cont=0;
	int points=img->countPoints();
	for (int i=0;i<points;i++)
		if(img->getPointAt(i)->is("ControlPoint"))
			cont++;
	return cont;
}

int BundleAdjustment::numberPhotogrammetricPoints(Image *img)
{
	int cont=0;
	int points=img->countPoints();
	for (int i=0;i<points;i++)
		if(img->getPointAt(i)->is("PhotogrammetricPoint"))
			cont++;
	return cont;
}

double BundleAdjustment::getInicialZPhotogrammetricPoints()
{
	double z=0;
	int n=0;
	for (int i=0;i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		for(int j=0;j<pnts;j++)
			if(isControlPoint(i,j))
			{
				z+=getPointFrom(i,j)->getObjectCoordinate().getZ();
				n++;
			}
	}
	return z/=n;
}
// atualiza as coordenadas de todos os pontos
void BundleAdjustment::updateCoordinatesAllPoints(Matrix xypf, double zphotogrammetric)
{
	for (int i=0;i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		int n=1;
		for(int j=0;j<pnts;j++)
			if(isPhotogrammetricPoint(i,j))
			{
				ObjectSpaceCoordinate coord = getPointFrom(i,j)->getObjectCoordinate();
				coord.setX(xypf.get(n++,1));
				coord.setY(xypf.get(n++,1));
				coord.setZ(zphotogrammetric);
				getPointFrom(i,j)->setObjectCoordinate(coord);
			}
	}
}

double BundleAdjustment::getMediaScale(int imageIndex)
{

	double media=0.0;
	double normaObjeto=0.0;
	double normaImagem=0.0;

	int pnts=listImages.at(imageIndex)->countPoints();

	double xFirstPoint=getPointFrom(imageIndex,0)->getObjectCoordinate().getX();
	double yFirstPoint=getPointFrom(imageIndex,0)->getObjectCoordinate().getY();
	double zFirstPoint=getPointFrom(imageIndex,0)->getObjectCoordinate().getZ();
	double xiFirstPoint=getPointFrom(imageIndex,0)->getAnalogCoordinate(listImages.at(imageIndex)->getId()).getXi();
	double etaFirstPoint=getPointFrom(imageIndex,0)->getAnalogCoordinate(listImages.at(imageIndex)->getId()).getEta();

	for(int j=1;j<pnts;j++)
	{
		double xCurrentPoint=getPointFrom(imageIndex,j)->getObjectCoordinate().getX();
		double yCurrentPoint=getPointFrom(imageIndex,j)->getObjectCoordinate().getY();
		double zCurrentPoint=getPointFrom(imageIndex,j)->getObjectCoordinate().getZ();

		double xiCurrentPoint=getPointFrom(imageIndex,j)->getAnalogCoordinate(listImages.at(imageIndex)->getId()).getXi();
		double etaCurrentPoint=getPointFrom(imageIndex,j)->getAnalogCoordinate(listImages.at(imageIndex)->getId()).getEta();

		normaObjeto =sqrt(pow(xFirstPoint-xCurrentPoint,2)+pow(yFirstPoint-yCurrentPoint,2)+pow(zFirstPoint-zCurrentPoint,2));
		normaImagem =sqrt(pow(xiFirstPoint-xiCurrentPoint,2)+pow(etaFirstPoint-etaCurrentPoint,2));
		//printf("Objeto sqrt(( %.3f - %.3f )^2) + ( %.3f - %.3f)^2 ))= %f\n",xiFirstPoint,xiCurrentPoint,etaFirstPoint,etaCurrentPoint,normaImagem);
		media+=normaObjeto/normaImagem;
	}
	//printf("object media %f count: %d",media,pnts);
	return media/(pnts-1);
}

// Retorna uma matriz correspondente a uma imagem na Matriz A
Matrix BundleAdjustment::imageToMatrixJacobiana(Image *img)
{
	int pnts=img->countPoints();
	Matrix result(0,6);
	for (int i=0;i<pnts;i++)
	{
		ObjectSpaceCoordinate aux=img->getPointAt(i)->getObjectCoordinate();
		double x=aux.getX();
		double y=aux.getY();
		double z=aux.getZ();
		result=result|getJacobianaControl(x,y,z,img->getId()); // esse id pode gerar inconsistencia// soluçao seria setar os ids de todas as imagens e pontos no inicio logo apos o sort de pontos
	}
	return result;
}

/* Retorna toda a matriz A1*/
void BundleAdjustment::createA1()
{
	Matrix result;
	int currentRows=0;
	for (int i=0;i<numImages;i++)
	{
		Matrix partial=imageToMatrixJacobiana(listImages.at(i));
		result.putMatrix(partial,currentRows+1,6*i+1);
		currentRows+=partial.getRows();
	}
	A1=result;
}

void BundleAdjustment::createA2()
{
	Matrix result(0,3*numFotogrametricPoints);

	for (int i=0;i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		Matrix oneImage(0,3*numFotogrametricPoints);
		for (int j=0;j<pnts;j++)
		{
			if(isPhotogrammetricPoint(i,j))
			{
				ObjectSpaceCoordinate aux=getPointFrom(i,j)->getObjectCoordinate();
				double x=aux.getX();
				double y=aux.getY();
				double z=aux.getZ();
				Matrix jf=getJacobianaFotogrametric(x,y,z,i+1);
				int pos=whereInPoints(listImages.at(i)->getPointAt(j));
				oneImage.putMatrix(jf,2*j+1,3*(pos-numControlPoints)+1);
			}
		}
		result=result|oneImage;
	}
	A2=result;
}

void BundleAdjustment::createL0()
{
	Matrix result(0,1);
	for (int i=0;i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		for (int j=0;j<pnts;j++)
		{
			ObjectSpaceCoordinate aux=getPointFrom(i,j)->getObjectCoordinate();
			double x=aux.getX();
			double y=aux.getY();
			double z=aux.getZ();
			/** Pegar esse metodo na classe do Rafael, as coordenadas pela equaçao de colinearidade*/
			Matrix coord=getCoordinatesEqColin(x,y,z,i+1);
			result=result|(coord.transpose());
		}
	}
	L0=result;
}

void BundleAdjustment::createLb()
{
	Matrix result(0,1);
	for (int i=0;i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		for (int j=0;j<pnts;j++)
		{
			AnalogImageSpaceCoordinate aux=getPointFrom(i,j)->getAnalogCoordinate(listImages.at(i)->getId());
			double xi=aux.getXi();
			double eta=aux.getEta();
			Matrix temp(2,1);
			temp.set(1,1,xi);
			temp.set(2,1,eta);
			result=result|temp;
		}
	}
	Lb=result;
}


void BundleAdjustment::calculateResiduos()
{
	Matrix result(0,1);
	for (int i=0;i<numImages;i++)
	{
		int pnts=listImages.at(i)->countPoints();
		for (int j=0;j<pnts;j++)
		{
			if (isControlPoint(i,j))
			{
				double x=getPointFrom(i,j)->getObjectCoordinate().getX();
				double y=getPointFrom(i,j)->getObjectCoordinate().getY();
				double z=getPointFrom(i,j)->getObjectCoordinate().getZ();
				double xi=getPointFrom(i,j)->getAnalogCoordinate(listImages.at(i)->getId()).getXi();
				double eta=getPointFrom(i,j)->getAnalogCoordinate(listImages.at(i)->getId()).getEta();

				/** Usar classe do Rafael para isso */
				Matrix coord=getCoordColinearTerrain(xi,eta,z,i+1);
				double resx=x-coord.get(1,1);
				double resy=y-coord.get(1,2);
				// reaproveitando a matrix que não será mais utilizada nesse loop;
				coord=coord.transpose();
				coord.set(1,1,resx);
				coord.set(2,1,resy);
				result=result|coord;
			}
		}
	}
	matRes=result;
}

void BundleAdjustment::calculatePeso()
{
	int d=1;
	for (int i=0;i<numImages;i++)
	{
		int numPoints=listImages.at(i)->countPoints();
		for (int j=0;j<numPoints;j++)
		{
			if (isControlPoint(i,j))
			{
				double p=(numEquations-numUnknows)/(pow(getRx(listImages.at(i),j+1),2));
				P.set(d,d,p);
				d++;
				p=(numEquations-numUnknows)/((pow(getRy(listImages.at(i),j+1),2)));
					P.set(d,d,p);
				d++;
			}
			if (isPhotogrammetricPoint(i,j))
			{
				P.set(d,d,1);
				d++;
				P.set(d,d,1);
				d++;
			}
		}
	}
}
void BundleAdjustment::updateCoordFotog()
{
	int n=1;
	for (int i=0;i<numPoints;i++)
	{
		Point *pnt=listPoints.at(i);
		if(pnt->is("PhotogrammetricPoint"))
		{
			ObjectSpaceCoordinate coord = pnt->getObjectCoordinate();
			double x=coord.getX();
			double y=coord.getY();
			double z=coord.getZ();
			//printf("%f+%f\n",x, getdXx2(n));

			coord.setX(x+getdXx2(n));
			coord.setY(y+getdYx2(n));
			coord.setZ(z+getdZx2(n));

			pnt->setObjectCoordinate(coord);
			n++;
		}
	}
}

double BundleAdjustment::getRx(Image *img, int pointId)
{
	int base=0;
	for(int i=1;i<img->getId();i++)
		base+=2*numberControlPoints(img);
	return matRes.get(base+2*(pointId-1)+1,1);
}

double BundleAdjustment::getRy(Image *img, int pointId)
{
	int base=0;
	for(int i=1;i<img->getId();i++)
		base+=2*numberControlPoints(img);
	return matRes.get(base+2*(pointId-1)+2,1);
}

/*Metodos para aumento de performance do calculo*/
Matrix BundleAdjustment::invertN11(Matrix N11)
{
	int numMatrixs=numImages;
	Matrix N11i=N11;
	for (int i=0;i<numMatrixs;i++)
	{
		/*iteraçao i=0
		sel[(6*0+1 x 6*0+1 ; 6*(0+1) x 6*(0+1) ] = 1x1;6x6
		iteraçao i=1
		sel[(6*1+1 x 6*1+1 ; 6*(1+1) x 6*(1+1) ] = 7x7;12x12
		*/
		Matrix partial(6,6);
		partial=N11.sel(6*i+1,6*(i+1),6*i+1,6*(i+1));
		N11i.putMatrix(partial.inverse(),6*i+1,6*i+1);
	}
	return N11i;
}

Matrix BundleAdjustment::invertN22(Matrix N22)
{
	int numMatrixs=numPoints;
	Matrix N22i=N22;
	for (int i=0;i<numMatrixs;i++)
	{
		/*iteraçao i=0
		sel[(3*0+1 x 3*0+1 ; 3*(0+1) x 3*(0+1) ] = 1x1;3x3
		iteraçao i=1
		sel[(3*1+1 x 3*1+1 ; 3*(1+1) x 3*(1+1) ] = 4x4;6x6
		*/
		Matrix partial(3,3);
		partial=N22.sel(3*i+1,3*(i+1),3*i+1,3*(i+1));
		N22i.putMatrix(partial.inverse(),3*i+1,3*i+1);
	}
	return N22i;
}

/* setters and getters*/
Matrix BundleAdjustment::getMatRes()
{
	matRes.show('f',8,"MatRes");
	return matRes;
}

bool BundleAdjustment::getConvergencyStatus()
{
	return converged;
}

int BundleAdjustment::getTotalIterations()
{
	return totalIterations;
}

void BundleAdjustment::setAFP()
{
	//Isso é feito apenas para:
	//Exibiçao dos angulos em Graus.
	Matrix result(matAdjust.getRows(),matAdjust.getCols());
	for (int i=1;i<=numImages;i++)
	{
		result.set(i,1,Dms::radianoToDegreeDecimal(matAdjust.get(i,1)));
		result.set(i,2,Dms::radianoToDegreeDecimal(matAdjust.get(i,2)));
		result.set(i,3,Dms::radianoToDegreeDecimal(matAdjust.get(i,3)));
		result.set(i,4,matAdjust.get(i,4));
		result.set(i,5,matAdjust.get(i,5));
		result.set(i,6,matAdjust.get(i,6));
	}
	afp=result;
}

Matrix BundleAdjustment::getAFP()
{
	return afp;
}

/* metodos auxiliares para teste*/
bool BundleAdjustment::isControlPoint(int imageIndex ,int pointIndex)
{
	return listImages.at(imageIndex)->getPointAt(pointIndex)->is("ControlPoint");
}

bool BundleAdjustment::isPhotogrammetricPoint(int imageIndex, int pointIndex)
{
	return listImages.at(imageIndex)->getPointAt(pointIndex)->is("PhotogrammetricPoint");
}

bool BundleAdjustment::isCheckingPoint(int imageIndex, int pointIndex)
{
	return listImages.at(imageIndex)->getPointAt(pointIndex)->is("CheckingPoint");
}

bool BundleAdjustment::testResiduo()
{
	int rowsMatRes=matRes.getRows();
	for (int i=1;i<=rowsMatRes;i++)
	{
		if (fabs(matRes.get(i,1)>MAXRESIDUO))
		{
			return false;
		}
	}
	return true;
}

bool BundleAdjustment::testConverged()
{
	int rowsX1=x1.getRows();
	int rowsX2=x2.getRows();
   // printf("testando X1");
	for (int i=1;i<=rowsX1;i++)
	{
		if (fabs(x1.get(i,1)>CONVERGENCY))
		{
			return false;
		}
	}
	//printf("testando X2");
	for (int i=1;i<=rowsX2;i++)
	{
		if (fabs(x2.get(i,1)>CONVERGENCY))
		{
			return false;
		}
	}
	return true;
}

bool BundleAdjustment::isConverged()
{
	if(totalIterations<=MAXITERATIONS)
		return converged=true;
	else
		return converged=false;
}


