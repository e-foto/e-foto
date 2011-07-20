#include "BundleAdjustment.h"
#include <sys/time.h>
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

15 iterações e 4 mudança de pesos
*/
//#include "qdebug.h"
#define N3 8
BundleAdjustment::BundleAdjustment()
{
	Matrix dummy(N3,N3);
	for (int i=1; i<=N3; i++)
		for (int j=1; j<=N3; j++)
			dummy.set(i,j,i*N3+j);

	setAFP(dummy);

//	for (int j=1;j<=dummy.getCols();j++)
//		qDebug("%f",dummy.get(1,j));


	c     = 0.0;
	fliDir= 0.0;
}


BundleAdjustment::BundleAdjustment(Matrix x, Matrix y, Matrix z, Matrix col, Matrix lin, Matrix OIs, Matrix BLC, double focalCalibrada, int flightDirection)
{
    X   = x;
    Y   = y;
    Z   = z;
    Col = col;
    Lin = lin;
    ois   = OIs;
    blc   = BLC;
    c     = focalCalibrada;
    fliDir= flightDirection;

/*
	Matrix N1(120,240);
	Matrix N2(240,120);

	timeval tim;
	gettimeofday(&tim, NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
		 N2*N1;
	gettimeofday(&tim, NULL);
	double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("%.6lf seconds elapsed\n", t2-t1);
*/

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

    /*Matrizes das coordenadas dos pontos do bloco. As imagens estÃ£o representadas nas linhas e os pontos nas
    colunas, onde o nÃºmero da linha Ã© o mesmo do identificador da imagem e o da coluna, o mesmo do identificador do ponto.
    Valor -1 indica ponto fotogramÃ©trico, e valor 0 indica ponto nÃ£o contido na imagem.
    */
    //espaço Objeto

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

    /* Matriz de configuraÃ§Ã£o do bloco. Valor 1 - ponto de controle, valor -1, ponto fotogramÃ©trico, valor 0, ponto nÃ£o
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

	//createcXsicEta();


    xsi0 = 0;
    eta0 = 0;

}

bool BundleAdjustment::calculate()
{



	Matrix L1=createL(X,Y);
    Matrix mm1=createMl(Col,Lin);
    Matrix mm2=createM2();

    Matrix m11=getM11(mm1);
    Matrix m12=getM12(mm1,mm2);
    Matrix m22=getM22(mm2);
    Matrix m1=getm1(mm1,L1);
    Matrix m2=getm2(mm2,L1);

    Matrix paf=getPAf(m11,m12,m22,m1,m2);
    Matrix xypf=getXYpf(m22,m2,m12,paf);

    double zpf=getZpf();

    X=getPX(xypf);
    Y=getPY(xypf);
    Z=getPZ(zpf);

    Matrix pc=firstPntCtrl();

    //getMediaScale(1);
    //printf("media scale 1 = %.6f\n",getMediaScale(1));
    //printf("media scale 2 = %.6f\n",getMediaScale(2));//imprime(mm1,"M1");

    //printf("KappaZero1 = %f \n",getKappaZero(getPTA(paf,1),1));
    //printf("KappaZero2= %f \n",getKappaZero(getPTA(paf,2),2));
    Matrix O=getInicialsValues(paf);
    setAFP(O);

    return true;
        //imprime(getInicialsValues(paf),"O");*/
}


//pnts[1]são os pontos de controle e pnts[2] são os pontos fotogrametricos;
int* BundleAdjustment::pointsToAdjustament()
{
    //imprime(newBLC,"BLC");
    int linhas=blc.getRows();
    int colunas= blc.getCols();
    int *aux=(int*)malloc(colunas*sizeof(int));
    int *pnts=(int*)malloc(3*sizeof(int));

    pnts[0]=0;
    pnts[1]=0;
    pnts[2]=0;

    for (int i=0;i<colunas;i++)
        aux[i]=0;

    for (int i=1;i<=linhas;i++)
    {
        for (int j=1;j<=colunas;j++)
        {
            if (blc.getInt(i,j)!=0)
                aux[j]=blc.getInt(i,j);
        }
    }
    for (int i=1;i<=colunas;i++)
    {
        if (aux[i]==1)
            pnts[1]++;
        if( aux[i]==-1)
            pnts[2]++;
    }

    pnts[0]=pnts[1]+pnts[2];
    return pnts;
}

int BundleAdjustment::numberOfEquations(int images, int pnts)
{
    int n=0;
    for (int i=0;i<images;i++)
        for (int j=0;j<pnts;j++)
            n+=2;
    return n;
}


void BundleAdjustment::createcXsicEta()
{
    int rows=blc.getRows();
    int cols=blc.getCols();

    Matrix cEta(rows,cols);
    Matrix cXsi(rows,cols);

    double *ana;
    for (int i=1;i<rows;i++)
        for(int j=cols;j>1;j--)
            if(blc.getInt(i+1,j)!=0)
            {
        ana=digitalToAnalog(ois,Lin.getInt(i+1,j),Col.getInt(i+1,j),i);
        cEta.set(i+1,j,ana[1]);
        cXsi.set(i+1,j,ana[0]);
    }
    this->cXsi=cXsi;
    this->cEta=cEta;
    //imprime(cXsi,"CXSI");
    //imprime(cEta,"CETA");
}

//Monta a matrix de todos os pontos para imagem
Matrix BundleAdjustment::createMl(Matrix C, Matrix L)
{
    int rows= C.getRows();
    int cols= C.getCols();

    Matrix auxc1(1,3);
    Matrix auxc2(1,3);

    Matrix total(0,6);
    double *ana;

    //itera por todas as imagens
    for (int i=1;i<rows;i++)
    {
        //Matriz de apenas uma imagem
        Matrix empilhada(0,6);
        //itera por todos os pontos
        for(int j=cols;j>1;j--)
        {
            ana=digitalToAnalog(ois,L.getInt(i+1,j),C.getInt(i+1,j),i);
            auxc1.set(1,1,1);
            auxc1.set(1,2,ana[1]);
            auxc1.set(1,3,ana[0]);
            auxc2=auxc1;
            auxc1.putMatrix(auxc2,2,4);
			printf ("createMl");
			auxc2=auxc1|empilhada;
        }
        //põe as matrizes diagonalmente em relaçao a outra
        empilhada.putMatrix(total,total.getRows()+1,6*(i-1)+1);
    }
    //imprime(total,"M1");
    return total;
}

Matrix BundleAdjustment::createL(Matrix X2, Matrix Y2)
{
    int rows=blc.getRows();
    int cols=blc.getCols();

    Matrix L2(2*(rows-1)*(cols-1),1);
    int k=1;
    for (int i=1;i<=rows;i++)
    {
        for(int j=1;j<=cols;j++)
        {
            if(blc.getInt(i,j)==-1)
            {
                L2.set(k++,1,0.0);
                L2.set(k++,1,0.0);
            }
            else if(blc.getInt(i,j)==1)
            {
                L2.set(k++,1,X2.get(i,j));
                L2.set(k++,1,Y2.get(i,j));
            }
        }
    }
    //imprime(L2,"L");
    return L2;
}

Matrix BundleAdjustment::createM2()
{
    int rows=blc.getRows();
    int cols=blc.getCols();

    Matrix M2aux(2*(rows-1)*(cols-1),2*pointsToAdjustament()[2]);
    int pntctr=1;

    for (int i=1;i<=rows;i++)
    {
        int pntftg=1;

        for(int j=1;j<=cols;j++)
        {
            if (blc.getInt(i,j)==-1)
            {
                // printf("Val[%d,%d]= %d\tlin= %d col= %d\n",i,j,BLC.getInt(i,j),pntctr,pntftg);
                M2aux.set(pntctr++, pntftg++ ,-1);
                M2aux.set(pntctr++, pntftg++ ,-1);
            }
            else if (blc.getInt(i,j)==1)
                pntctr+=2;
        }
    }
    //imprime(M2aux,"M2");

    return M2aux;
}

Matrix BundleAdjustment::getM11(Matrix M1)
{
    //printf("getM11\n");
    return M1.transpose()*M1;
}

Matrix BundleAdjustment::getM12(Matrix M1, Matrix M2)
{

    //printf("getM12=M1[%dx%d]*M2[%dx%d]\n",M1.getCols(),M1.getRows(),M2.getRows(),M2.getCols());
    return M1.transpose()*M2;
}

Matrix BundleAdjustment::getM22(Matrix M2)
{
    //printf("getM22 %dx%d\n",M2.getRows(),M2.getCols());
    return M2.transpose()*M2;
}

Matrix BundleAdjustment::getm1(Matrix M1, Matrix L1)
{
    //printf("getm1\n");
    return (M1.transpose())*L1;
}

Matrix BundleAdjustment::getm2(Matrix M2, Matrix L1)
{
    //printf("getm2\n");
    return (M2.transpose())*L1;
}

Matrix BundleAdjustment::getPAf(Matrix M11, Matrix M12, Matrix M22, Matrix m1, Matrix m2)
{
    //printf("paf\n");
    return ( (M11-M12*M22.inverse()*M12.transpose()).inverse() )*( m1-M12*M22.transpose()*m2 );
}

Matrix BundleAdjustment::getXYpf(Matrix M22, Matrix m2, Matrix M12, Matrix paf)
{
    return M22.inverse()*m2-M22.inverse()*M12.transpose()*paf;
}

double BundleAdjustment::getZpf()
{
    int rows=blc.getRows()-1;
    int cols=blc.getCols()-1;
    int n=0;
    double z=0;
    for (int i=1;i<=rows;i++)
    {
        for(int j=1;j<=cols;j++)
        {
            if(blc.getInt(i,j)==1)
            {
                z+=Z.get(i,j);
                n++;
            }
        }
    }
    z/=n;
    // printf("zpf= %f\n",z);
    return z;
}

Matrix BundleAdjustment::getPX(Matrix xypf)
{
    int rows=blc.getRows();
    Matrix coord=X;
    int *dp=pointsToAdjustament();
    int d=dp[1];
    // imprime(X,"X");
    //  printf("ctr=%d ft=%d\n",dp[1],dp[2]);
    for (int i=1;i<=rows;i++)
    {
        for (int j=1;j<=dp[2]+1;j++)
        {
            if (blc.getInt(i,j+d)==-1)
            {
                //printf("If-X[%d,%d]=%f\n",i,j,X.get(i,j));
                //          printf("i=%d\tj=%d\tj+d=%d\n",i,j,j+d);
                coord.set(i,j+d,xypf.get((j-1)*2-1,1));
            }
        }
    }
    // imprime(coord,"coord X");
    //imprime(X,"X");
    return coord;
}

Matrix BundleAdjustment::getPY(Matrix xypf)
{
    int rows=blc.getRows();
    Matrix coord=Y;
    int *dp=pointsToAdjustament();
    int d=dp[1];
    // imprime(X,"X");
    //  printf("ctr=%d ft=%d\n",dp[1],dp[2]);
    for (int i=1;i<=rows;i++)
    {
        for (int j=1;j<=dp[2]+1;j++)
        {
            if (blc.getInt(i,j+d)==-1)
            {
                // printf("If-X[%d,%d]=%f\n",i,j,Y.get(i,j));
                //          printf("i=%d\tj=%d\tj+d=%d\n",i,j,j+d);
                coord.set(i,j+d,xypf.get((j-1)*2,1));
            }
        }
    }
    // imprime(coord,"coord Y");
    // imprime(Y,"Y");
    return coord;
}

Matrix BundleAdjustment::getPZ(double zpf)
{
    int rows=blc.getRows();
    Matrix coord=Z;
    int *dp=pointsToAdjustament();
    int d=dp[1];
    // imprime(X,"X");
    //  printf("ctr=%d ft=%d\n",dp[1],dp[2]);
    for (int i=1;i<=rows;i++)
    {
        for (int j=1;j<=dp[2]+1;j++)
        {
            if (blc.getInt(i,j+d)==-1)
            {
                //printf("If-X[%d,%d]=%f\n",i,j,Z.get(i,j));
                //          printf("i=%d\tj=%d\tj+d=%d\n",i,j,j+d);
                coord.set(i,j+d,zpf);
            }
        }
    }
    // imprime(coord,"coord Z");
    // imprime(Z,"Z");
    return coord;
}
//retorna os parametros da imagem numero imageID
Matrix BundleAdjustment::getPTA(Matrix PAf, int imageId)
{
    Matrix result(6,1);
    for (int i=6*(imageId-1), j=1;i<6*(imageId);i++,j++)
        result.set(j,1,PAf.get(i+1,1));
    return result;
}

Matrix BundleAdjustment::firstPntCtrl()
{
    //matrix com os indices dos 1º pontos de controle
    int rows=blc.getRows();
    int cols=blc.getCols();
    Matrix pc(1,rows-1);

    for (int i=2;i<=rows;i++)
    {
        int aux=0;
        for (int j=2;aux==0 && j<=cols;j++)
            if(blc.getInt(i,j)==1)
                aux=j-1;
        pc.setInt(1,i-1,aux);
    }

    //imprime(BLC,"BLC");
    //imprime(pc,"pc");

    return pc;

}

double BundleAdjustment::getMediaScale(int imageId)
{
    Matrix pc=firstPntCtrl();

    int pnts=blc.getCols()-1;
    int count=0;
    imageId++;

    double media=0.0;
    double dis=0.0;
    double Dis=0.0;
    /*
    Matrix pc(1,2);
    pc.set(1,1,5);
    pc.set(1,2,5);
*/
    double cxsi=cXsi.get(imageId,pc.get(1,imageId-1)+1);
    double ceta=cEta.get(imageId,pc.get(1,imageId-1)+1);
    double x=X.get(imageId,pc.get(1,imageId-1)+1);
    double y=Y.get(imageId,pc.get(1,imageId-1)+1);
    double z=Z.get(imageId,pc.get(1,imageId-1)+1);

    //printf("cxsi=%f ceta=%f\n",cxsi,ceta);
    //imprime(BLC,"BLC");
    // imprime(X,"X");
    // imprime(Y,"Y");
    //imprime(Z,"Z");
    for (int j=1;j<=pnts+1;j++)
    {
        if(blc.getInt(imageId,j)!=0  && j!=pc.get(1,imageId-1)+1)//)
        {
            dis=sqrt(pow(cxsi-cXsi.get(imageId,j),2)+pow(ceta-cEta.get(imageId,j),2));
            // printf("i=%d j=%d\n",imageId,j);
            // printf("sqrt(( %.3f - %.3f )^2) + ( %.3f - %.3f)^2 ))",cxsi,cXsi.get(imageId,j),ceta,cEta.get(imageId,j));
            //printf("= %f\n",dis);

            Dis=sqrt(pow(x-X.get(imageId,j),2)+pow(y-Y.get(imageId,j),2)+pow(z-Z.get(imageId,j),2));
            //printf("sqrt(( %.3f - %.3f )^2) + ( %.3f - %.3f)^2 ) + ( %.3f - %.3f)^2 ))",x,X.get(imageId,j),y,Y.get(imageId,j),z,Z.get(imageId,j));
            //printf("= %f\n",Dis);
            media+=Dis/dis;
            count++;
        }
    }
    return media/count;
}

double BundleAdjustment::getKappaZero(Matrix pta,int imageId)
{
    double kappa0=0.002;
    //mf[0]=MFX mf[1]=MFY
    /*
    double linha=Lba[2*(fliDir-1)];
    double coluna=Lba[2*(fliDir-1)+1];
    double *mf= (double*)malloc(2*sizeof(double));
    mf[0]= pta.get(1,1) + pta.get(2,1)*coluna + pta.get(3,1)*linha;
    mf[1]= pta.get(4,1) + pta.get(5,1)*coluna + pta.get(6,1)*linha;

    // imprime(pta,"PTA");
    //printf ("Lba[%d]= %f\nLba[%d] = %f\n\n",2*(mfS-1),coluna,2*(mfS-1)+1,linha);
    // printf ("MFX[%d]= %f\nMFY[%d] = %f\n\n",imageId,mf[0],imageId,mf[1]);

    double Ox=(pta.get(1,1) + pta.get(2,1)*xsi0 + pta.get(3,1)*eta0);
    double Oy=(pta.get(4,1) + pta.get(5,1)*xsi0 + pta.get(6,1)*eta0);
    double dX=mf[0]-Ox;
    double dY=mf[1]-Oy;

    // printf("dX= %f\n",dX);
    // printf("dY= %f\n\n",dY);

    double kappa0;
    if (fabs(dY)<1e-14 && mf[0]>Ox)
    {
        //   printf("dy=0 mfx>omg\n");
        kappa0= 0;
    }
    else if (fabs(dY)<1e-14 && mf[0]<Ox)
    {
        //   printf("dy=0 mfx<omg\n");
        kappa0= 180;
    }
    else if (fabs(dX)<1e-14 && mf[1]>Oy)
    {
        //  printf("dx=0 mfy>omg\n");
        kappa0=90;
    }
    else if (fabs(dX)<1e-14 && mf[1]<Oy)
    {
        //  printf("dx=0 mfy<omg\n");
        kappa0= 270;
    }
    else if (fabs(dX)>1e-14 && fabs(dY)>1e-14)
    {
        double alpha=atan(fabs(dY/dX));
        //double alpha=atan(fabs(8.086553/977.873755));
        //  printf( "alpha=%.10f\n",alpha);
        if (dY > 0 && dX > 0)
            return alpha;
        if (dY > 0 && dX < 0)
            return 180-alpha;
        if (dY < 0 && dX < 0)
            return 180+alpha;
        if (dY < 0 && dX > 0)
            return -alpha;
    }*/
    return kappa0;
}

/** omega, phi, kappa, X0, Y0, Z0)*/
Matrix BundleAdjustment::getInicialsValues(Matrix paf)
{
    int rows=blc.getRows()-1;
    Matrix result(1,6);
    for (int i=rows;i>0;i--)
    {
        //    printf("imagens i=%d\n",i);
        Matrix pta=getPTA(paf,i);
        Matrix O(1,6);
        O.set(1,1,0);
        O.set(1,2,0);
        O.set(1,3,getKappaZero(pta,i));
        O.set(1,4,pta.get(1,1) + pta.get(2,1)*xsi0 + pta.get(3,1)*eta0);
        O.set(1,5,pta.get(4,1) + pta.get(5,1)*xsi0 + pta.get(6,1)*eta0);
        O.set(1,6,c*getMediaScale(i));
        //  imprime(O,"parcial");
        result=O|result;
        // imprime(result,"result");
    }

    return result;
}



























void BundleAdjustment:: imprime(Matrix mat, char *id)
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

double* BundleAdjustment::digitalToAnalog(Matrix Xa, int linha, int coluna, int indexImg)
{
    double *result= (double*)malloc(2*sizeof(double));
    result[0]= Xa.get(indexImg,1) + Xa.get(indexImg,2)*coluna + Xa.get(indexImg,3)*linha;
    result[1]= Xa.get(indexImg,4) + Xa.get(indexImg,5)*coluna + Xa.get(indexImg,6)*linha;

    // printf("row %d col %d\n",linha,coluna);
    //printf("%f %f\n",result[0],result[1]);
    return result;
}

int* BundleAdjustment::analogToDigital(Matrix Xa, double xsi, double eta, int indexImg)
{
    double a0, a1, a2, b0, b1, b2;
    a0 = Xa.get(1,indexImg);
    a1 = Xa.get(1,indexImg);
    a2 = Xa.get(1,indexImg);
    b0 = Xa.get(1,indexImg);
    b1 = Xa.get(1,indexImg);
    b2 = Xa.get(1,indexImg);

    int *result= (int*)malloc(2*sizeof(int));
    //Idéia do Rafael Aguiar para contornar erro de cast (somar 0.1)
    result[0]=(int)((b2*xsi - b2*a0 - a2*eta + b0*a2) / (a1*b2 - b1*a2) + 0.1 );
    result[1]=(int)((a1*eta - a1*b0 - b1*xsi + b1*a0) / (a1*b2 - b1*a2) + 0.1 );
    return result;
}






































Matrix BundleAdjustment::setRot(double omega, double phi, double kappa)
{
    Matrix rot;

    return rot;
}

Matrix BundleAdjustment::getRot()
{
    return rot;
}

Matrix BundleAdjustment::setEquationsColinear(Matrix parameters)
{
    Matrix eqcln(1,6);

    return eqcln;
}

Matrix BundleAdjustment::getJacobianaControl(Matrix parameters, double X, double Y, double Z)
{
    //JacCtrl(1,3);


    return JacCtrl;
}


Matrix BundleAdjustment::getJacobianaFotogrametric(Matrix parameters, double X, double Y, double Z)
{
    //JacFoto(1,3);



    return JacFoto;
}


Matrix BundleAdjustment::getA1()
{
    Matrix A1(1,3);

    return A1;
}


















































void BundleAdjustment::setAFP(Matrix aFP)
{
	afp=aFP;
}

Matrix BundleAdjustment::getAFP()
{
	imprime(afp, "BA:afp");
	return afp;
}










