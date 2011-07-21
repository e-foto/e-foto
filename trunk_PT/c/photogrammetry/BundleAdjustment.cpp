#include "BundleAdjustment.h"
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

15 itera√ß√µes e 4 mudan√ßa de pesos
*/
#include <qdebug.h>

#define N3 8
#define ITERATIONS 10
#define CONVERGENCY 0.001
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


BundleAdjustment::BundleAdjustment(Matrix x, Matrix y, Matrix z, Matrix col, Matrix lin, Matrix OIs, Matrix BLC, Sensor *sensor, int flightDirection)
{
    X   = x;
    Y   = y;
    Z   = z;
    Col = col;
    Lin = lin;
    ois   = OIs;
    blc   = BLC;

    c     = sensor->getFocalDistance();
    fliDir= flightDirection;

    this->sensor=sensor;
    xsi0 = sensor->getPrincipalPointCoordinates().getXi();
    eta0 = sensor->getPrincipalPointCoordinates().getEta();


    //ois.show();
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

    /*Matrizes das coordenadas dos pontos do bloco. As imagens est√É¬£o representadas nas linhas e os pontos nas
    colunas, onde o n√É¬∫mero da linha √É¬© o mesmo do identificador da imagem e o da coluna, o mesmo do identificador do ponto.
    Valor -1 indica ponto fotogram√É¬©trico, e valor 0 indica ponto n√É¬£o contido na imagem.
    */
    //espa√ßo Objeto

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

    /* Matriz de configura√É¬ß√É¬£o do bloco. Valor 1 - ponto de controle, valor -1, ponto fotogram√É¬©trico, valor 0, ponto n√É¬£o
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

        createcXsicEta();



}

bool BundleAdjustment::calculate()
{
    Matrix L1=createL(X,Y);
    Matrix mm1=createMl();
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

    //X.show();
    //Y.show();
    //Z.show();


    matAdjust=getInicialsValues(paf);
//    matAdjust.show();

    int cont=0;
    bool converged=false;
    while(cont<ITERATIONS && !converged)
    {
        createA1();
        //printf()
        createA2();
        Matrix p;
        p.identity(A1.getRows());
        createL0();
        createLb();
        Matrix l=Lb-L0;
        setP(p);
        Matrix n11=getN11();
        Matrix n12=getN12();
        Matrix n22=getN22();
        Matrix n1=getn1(l);
        Matrix n2=getn2(l);

        setx1(getx1(n11,n12,n22,n1,n2));
        setx2(getx2(n12,n22,n2,x1));

        updateMatAdjust();
        updateCoordFotog();
        converged=isConverged();
        //testconvergency();
//matAdjust.set();
        cont++;
    }
    printf("Numero de iteracoes: %d",cont);
    setAFP(matAdjust);
    return true;
}


//pnts[1]s√£o os pontos de controle e pnts[2] s√£o os pontos fotogrametricos;
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
   // cXsi.show();
    //cEta.show();
}

//Monta a matrix de todos os pontos para imagem
Matrix BundleAdjustment::createMl()
{
    int rows= Col.getRows();
    int cols= Col.getCols();

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
            Matrix auxc1(1,3);
            Matrix auxc2(1,3);
            ana=digitalToAnalog(ois,Lin.getInt(i+1,j),Col.getInt(i+1,j),i);
            auxc1.set(1,1,1);
            auxc1.set(1,2,ana[1]);
            auxc1.set(1,3,ana[0]);
            auxc2=auxc1;
            auxc1.putMatrix(auxc2,2,4);//auxc1=matriz 2x6 auxc2= matriz 1x3

            empilhada=auxc1|empilhada;
           // auxc1.show();
            //auxc2.show();
            //empilhada.show();
            //printf ("createMl");
        }
        //p√µe as matrizes diagonalmente em rela√ßao a outra
        total.putMatrix(empilhada,total.getRows()+1,6*(i-1)+1);
    }
    //imprime(total,"M1");
    //printf("saiu do create M1");
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
    //matrix com os indices dos 1¬∫ pontos de controle
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
    //double kappa0=0.002;
    //mf[0]=MFX mf[1]=MFY


    //coordenadas das marcas fiduciais em milimetros
    double coluna=0.016;
    double linha=113.0;

    /*
    int linha=sensor->setCalculationMode();
    int coluna=sensor->getImage(imageId)->getDigFidMarkAt(0).getCol();
    */


    double mfX= pta.get(1,1) + pta.get(2,1)*coluna + pta.get(3,1)*linha;
    double mfY= pta.get(4,1) + pta.get(5,1)*coluna + pta.get(6,1)*linha;

//    qDebug("mfx %f",mfX);
 //   qDebug("mfy %f",mfY);

    // imprime(pta,"PTA");
    //printf ("Lba[%d]= %f\nLba[%d] = %f\n\n",2*(mfS-1),coluna,2*(mfS-1)+1,linha);
    // printf ("MFX[%d]= %f\nMFY[%d] = %f\n\n",imageId,mf[0],imageId,mf[1]);

    double Ox=(pta.get(1,1) + pta.get(2,1)*xsi0 + pta.get(3,1)*eta0);
    double Oy=(pta.get(4,1) + pta.get(5,1)*xsi0 + pta.get(6,1)*eta0);

  //  qDebug("Ox %f",Ox);
   // qDebug("Oy %f",Oy);

    double dX=mfX-Ox;
    double dY=mfY-Oy;

    // printf("dX= %f\n",dX);
    // printf("dY= %f\n\n",dY);

    double kappa0;
    if (fabs(dY)<1e-14 && mfX>Ox)
    {
        //   printf("dy=0 mfx>omg\n");
        kappa0= 0;
    }
    else if (fabs(dY)<1e-14 && mfX<Ox)
    {
        //   printf("dy=0 mfx<omg\n");
        kappa0= M_PI;
    }
    else if (fabs(dX)<1e-14 && mfY>Oy)
    {
        //  printf("dx=0 mfy>omg\n");
        kappa0=M_PI/2;
    }
    else if (fabs(dX)<1e-14 && mfY<Oy)
    {
        //  printf("dx=0 mfy<omg\n");
        kappa0= 3*M_PI/2;
    }
    else if (fabs(dX)>1e-14 && fabs(dY)>1e-14)
    {
        double alpha=atan(fabs(dY/dX));
        //double alpha=atan(fabs(8.086553/977.873755));
        //  printf( "alpha=%.10f\n",alpha);
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

/** omega, phi, kappa, X0, Y0, Z0)*/
Matrix BundleAdjustment::getInicialsValues(Matrix paf)
{
    int rows=blc.getRows();
    Matrix result(1,6);
    for (int i=1;i<rows;i++)
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
        result=result|O;
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
    a0 = Xa.get(indexImg,1);
    a1 = Xa.get(indexImg,2);
    a2 = Xa.get(indexImg,3);
    b0 = Xa.get(indexImg,4);
    b1 = Xa.get(indexImg,5);
    b2 = Xa.get(indexImg,6);

    int *result= (int*)malloc(2*sizeof(int));
    //Id√©ia do Rafael Aguiar para contornar erro de cast (somar 0.1)
    result[0]=(int)((b2*xsi - b2*a0 - a2*eta + b0*a2) / (a1*b2 - b1*a2) + 0.1 );
    result[1]=(int)((a1*eta - a1*b0 - b1*xsi + b1*a0) / (a1*b2 - b1*a2) + 0.1 );
    return result;
}




































/** 20_07_2011*/

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

    Matrix ROT(3,3);
    ROT.set(1,1,r11);
    ROT.set(1,2,r12);
    ROT.set(1,3,r13);
    ROT.set(2,1,r21);
    ROT.set(2,2,r22);
    ROT.set(2,3,r23);
    ROT.set(3,1,r31);
    ROT.set(3,2,r32);
    ROT.set(3,3,r33);

    rot=ROT;
}

Matrix BundleAdjustment::getRot()
{
    return rot;
}

Matrix BundleAdjustment::getCoordinatesEqColin(double X, double Y, double Z, int imageId)
{
    Matrix coord(1,2);
    double omega=matAdjust.get(imageId+1,1);
    double phi=matAdjust.get(imageId+1,2);
    double kappa=matAdjust.get(imageId+1,3);
    double X0=matAdjust.get(imageId+1,4);
    double Y0=matAdjust.get(imageId+1,5);
    double Z0=matAdjust.get(imageId+1,6);

    setRot(omega,phi,kappa);
    double xi=xsi0-c*(r11*(X-X0)+r21*(Y-Y0)+r31*(Z-Z0))/(r13*(X-X0)+r23*(Y-Y0)+r33*(Z-Z0));
    double eta=eta0-c*(r12*(X-X0)+r22*(Y-Y0)+r32*(Z-Z0))/(r13*(X-X0)+r23*(Y-Y0)+r33*(Z-Z0));

    coord.set(1,1,xi);
    coord.set(1,2,eta);

    return coord;
}

Matrix BundleAdjustment::getJacobianaControl(double X, double Y, double Z, int imageId)
{
    Matrix JacCtrl(2,6);

    double omega=matAdjust.get(imageId+1,1);
    double phi=matAdjust.get(imageId+1,2);
    double kappa=matAdjust.get(imageId+1,3);
    double X0=matAdjust.get(imageId+1,4);
    double Y0=matAdjust.get(imageId+1,5);
    double Z0=matAdjust.get(imageId+1,6);

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


Matrix BundleAdjustment::getJacobianaFotogrametric(double X, double Y, double Z, int imageId)
{
    Matrix JacFoto(2,3);

    double omega=matAdjust.get(imageId+1,1);
    double phi=matAdjust.get(imageId+1,2);
    double kappa=matAdjust.get(imageId+1,3);
    double X0=matAdjust.get(imageId+1,4);
    double Y0=matAdjust.get(imageId+1,5);
    double Z0=matAdjust.get(imageId+1,6);


    JacFoto.set(1,1,-c*cos(kappa)*cos(phi)/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*sin(phi));
    JacFoto.set(1,2,-c*(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*sin(omega));
    JacFoto.set(1,3,-c*(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(cos(kappa)*cos(phi)*(X-X0)+(sin(kappa)*cos(omega)+cos(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(sin(kappa)*sin(omega)-cos(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*cos(omega));

    JacFoto.set(2,1,c*sin(kappa)*cos(phi)/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*sin(phi));
    JacFoto.set(2,2,-c*(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))-c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*sin(omega));
    JacFoto.set(2,3,-c*(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))/(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0))+c*(-sin(kappa)*cos(phi)*(X-X0)+(cos(kappa)*cos(omega)-sin(kappa)*sin(phi)*sin(omega))*(Y-Y0)+(cos(kappa)*sin(omega)+sin(kappa)*sin(phi)*cos(omega))*(Z-Z0))/pow(sin(phi)*(X-X0)-cos(phi)*sin(omega)*(Y-Y0)+cos(phi)*cos(omega)*(Z-Z0),2)*cos(phi)*cos(omega));

    this->JacFoto=JacFoto;

    return JacFoto;
}


void BundleAdjustment::createA1()
{
    int rows= blc.getRows();
    int cols= blc.getCols();

    Matrix total(0,6);

    //itera por todas as imagens
    for (int i=1;i<rows;i++)
    {
        //Matriz de apenas uma imagem
        Matrix empilhada(0,6);
        //itera por todos os pontos
        for(int j=cols;j>1;j--)
        {
            //Se o ponto estiver na imagem, sendo fotogramÈtrico ou n„o.
            if (blc.getInt(i+1,j)!=0)
            {
                double x=X.get(i+1,j);
                double y=Y.get(i+1,j);
                double z=Z.get(i+1,j);
                //qDebug("x%d: %f\ty%d: %f\tz%d: %f\t",j,x,j,y,j,z);
                Matrix auxc2=getJacobianaControl(x,y,z,i);
                //auxc2.show('f',6);
                empilhada=auxc2|empilhada;
            }
        }
        //empilhada.show();
        //p√µe as matrizes diagonalmente em rela√ßao a outra
        total.putMatrix(empilhada,total.getRows()+1,6*(i-1)+1);
    }
    //imprime(total,"M1");
    A1=total;
}

void BundleAdjustment::createA2()
{
    int rows= blc.getRows();
    int cols= blc.getCols();
    int pf=pointsToAdjustament()[2];
    Matrix total(0,3*pf);
    //itera por todas as imagens
    for (int i=1;i<rows;i++)
    {
        //Matriz de apenas uma imagem
        Matrix empilhada(0,3*pf);
        int npc=numberCntrlPnts(i);
        int npf=0;
        //itera por todos os pontos
        for(int j=1;j<=cols;j++)
        {
            //Se o ponto for fotogramÈtrico.
            if (blc.getInt(i+1,j)==-1)
            {
                npf++;
                double x=X.get(i+1,j);
                double y=Y.get(i+1,j);
                double z=Z.get(i+1,j);
                Matrix auxc2=getJacobianaFotogrametric(x,y,z,i);
                empilhada.putMatrix(auxc2,2*(npc+npf)-1,3*(j-npc-2)+1);
            }
        }
        //empilhada.show();
        total=total|empilhada;
    }
    A2=total;

}

void BundleAdjustment::createLb()
{
    Matrix lb(0,1);//2*(cXsi.getCols()-1),1);

    int numImages=blc.getRows()-1;
    int numPnts=blc.getCols()-1;
    for (int i=1;i<=numImages;i++)
        for(int j=1;j<=numPnts;j++)
        {
            Matrix temp(2,1);
            // se o ponto est· na imagem i
            if (blc.getInt(i+1,j+1)!=0)
            {
                temp.set(1,1,cXsi.get(i+1,j+1));
                temp.set(2,1,cEta.get(i+1,j+1));
            }
            lb=lb|temp;
        }
    Lb=lb;
}

void BundleAdjustment::createL0()
{
    Matrix l0(0,1);

    int numImages=blc.getRows()-1;//2
    int numPnts=blc.getCols()-1;//5
    //qDebug("num images: %d\tnum pontos:%d",numImages,numPnts);
    for (int i=1;i<=numImages;i++)//1 a 2
        for(int j=1;j<=numPnts;j++)//1 a 5
        {
            // se o ponto est· na imagem i
            if (blc.getInt(i+1,j+1)!=0)//2,2 a 3,6
            {
                double x=X.get(i+1,j+1);
                double y=Y.get(i+1,j+1);
                double z=Z.get(i+1,j+1);
                Matrix coord=getCoordinatesEqColin(x,y,z,i);
                l0=l0|coord.transpose();
            }
        }
    L0=l0;
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
    return matAdjust.get(imageId+1,1);
}

double BundleAdjustment::getPhiAdjus(int imageId)
{
    return matAdjust.get(imageId+1,2);
}

double BundleAdjustment::getKappaAdjus(int imageId)
{
    return matAdjust.get(imageId+1,3);
}

double BundleAdjustment::getXAdjus(int imageId)
{
    return matAdjust.get(imageId+1,4);
}

double BundleAdjustment::getYAdjus(int imageId)
{
    return matAdjust.get(imageId+1,5);
}

double BundleAdjustment::getZAdjus(int imageId)
{
    return matAdjust.get(imageId+1,6);
}

Matrix BundleAdjustment::invertN11(Matrix N11)
{
    int numMatrixs=blc.getRows()-1;
    Matrix N11i=N11;
    for (int i=0;i<numMatrixs;i++)
    {
        /*iteraÁao i=0
        sel[(6*0+1 x 6*0+1 ; 6*(0+1) x 6*(0+1) ] = 1x1;6x6
        iteraÁao i=1
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
    int numMatrixs=blc.getCols()-1;
    Matrix N22i=N22;
    for (int i=0;i<numMatrixs;i++)
    {
        /*iteraÁao i=0
        sel[(3*0+1 x 3*0+1 ; 3*(0+1) x 3*(0+1) ] = 1x1;3x3
        iteraÁao i=1
        sel[(3*1+1 x 3*1+1 ; 3*(1+1) x 3*(1+1) ] = 4x4;6x6
        */
        Matrix partial(3,3);
        partial=N22.sel(3*i+1,3*(i+1),3*i+1,3*(i+1));
        N22i.putMatrix(partial.inverse(),3*i+1,3*i+1);
    }
    return N22i;
}

void BundleAdjustment::setP(Matrix p)
{
    P=p;
}

Matrix BundleAdjustment::getP()
{
    return P;
}

void BundleAdjustment::updateMatAdjust()
{
    int numImages= blc.getRows()-1;
    for (int i=1;i<=numImages;i++)
    {
        matAdjust.set(i+1,0, getOmegaAdjus(i) + getdOmegax1(i) );//ajustando omega
        matAdjust.set(i+1,1, getPhiAdjus(i)   + getdPhix1(i)   );//ajustando phi
        matAdjust.set(i+1,2, getKappaAdjus(i) + getdKappax1(i) );//ajustando kappa
        matAdjust.set(i+1,3, getXAdjus(i)     + getdXx1(i)     );//ajustando X
        matAdjust.set(i+1,4, getYAdjus(i)     + getdYx1(i)     );//ajustando Y
        matAdjust.set(i+1,5, getZAdjus(i)     + getdZx1(i)     );//ajustando Z
    }
}

void BundleAdjustment::updateCoordFotog()
{
    int numImags = blc.getRows();//3
    int numPnts  = blc.getCols();//3

    // melhorar esquema de pontos com urgencia: sÛ È necessario conhecer os pontos uma unica vez
    // sendo fotogramÈtrico ou n„o
    for (int i=2;i<=numImags;i++)
    {
        for (int j=2;j<=numPnts;i++)
        {
            // Se È ponto fotogramÈtrico
            int cont=0;
            if (blc.getInt(i,j)==-1)
            {
                cont++;
                X.set(i,j, X.get(i,j) + getdXx2(cont));//ajustando X fotogramÈtrico
                Y.set(i,j, Y.get(i,j) + getdYx2(cont));//ajustando Y fotogramÈtrico
                Z.set(i,j, Z.get(i,j) + getdZx2(cont));//ajustando Z fotogramÈtrico
            }
        }
    }
}

bool BundleAdjustment::isConverged()
{
    int rowsX1=x1.getRows();
    int rowsX2=x2.getRows();
    printf("testando X1");
    for (int i=1;i<=rowsX1;i++)
    {
        if (fabs(x1.get(i,1)>CONVERGENCY))
        {
            return false;
        }
    }
    printf("testando X2");
    for (int i=1;i<=rowsX2;i++)
    {
        if (fabs(x2.get(i,1)>CONVERGENCY))
        {
            return false;
        }
    }
    return true;
}

















int BundleAdjustment::numberCntrlPnts(int imageId)
{
    int num=0;
    int cols=blc.getCols();
    for (int j=1;j<=cols;j++)
        if(blc.getInt(imageId+1,j)==1)
            num++;
    return num;
}
int BundleAdjustment::numberFtgPnts(int imageId)
{
    int num=0;
    int cols=blc.getCols();
    for (int j=1;j<=cols;j++)
        if(blc.getInt(imageId+1,j)==-1)
            num++;
    return num;
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





























/* Em teste*/

// Retorna um matriz correspondente a uma imagem na Matriz A
Matrix BundleAdjustment::imageToMatrix(Image *img)
{
    int rows=2*img->countPoints();
    Matrix result(0,6);
    for (int i=0;i<rows;i++)
    {
        string xyz=img->getPoint(i)->getAnalogCoordinate(img->getId()).getGmlPos();
        int p1=xyz.find_first_of(" ");
        int p2=xyz.find_last_of(" ");
        double x=stringToDouble(xyz.substr(0,p1));
        double y=stringToDouble(xyz.substr(p1+1,p2));
        double z=stringToDouble(xyz.substr(p2+1,xyz.size()));
        result=getJacobianaControl(x,y,z,img->getId())|result;
    }
    return result;
}

/* Teoricamente: retornaria toda a matriz A1 se todas as imagens
*  forem passadas com os seus pontos indexados
*/
Matrix BundleAdjustment::imagesToMatrixes(deque<Image*> listImgs)
{
    int numImgs=listImgs.size();
    Matrix result;
    int currentRows=0;
    for (int i=0;i<numImgs;i++)
    {
        Matrix partial=imageToMatrix(listImgs.at(i));
        result.putMatrix(partial,currentRows+1,6*i+1);
        currentRows+=partial.getRows();
    }
    return result;
}

// Anexa uma lista de pontos na imagem
// ApÛs a execuÁ„o. Esse metodo ajudaria a saber quais pontos est„o na imagem
void BundleAdjustment::putPointsInImage(Image *img, deque<Point*> listPoints)
{
    int numPnts=listPoints.size();
    for (int i=0;i<numPnts;i++)
        img->putPoint(listPoints.at(i));
}





























