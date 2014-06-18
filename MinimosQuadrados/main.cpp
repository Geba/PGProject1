#include <iostream>
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
#include <armadillo>
using namespace std;
using namespace arma;

double* minQuad(double x[],double y[] ,int grau, int nPontos);


int main()
{

    //ae_matrix e;
   // double x[] = {-1, 0, 2, 3};
    //double y[] = {-4, -1.5, -0.5, 0};
     double x[] = {1872, 1890,1900,1920,1940,1950,1960,1970,1980,1991,1996};
    double y[] = {9.9 ,14.3 ,17.4 ,30.6, 41.2, 51.9, 70.2, 93.1, 119.0, 146.2, 157.1};
    //cout << "Hello world!" <<endl;
    minQuad(x, y, 100, 11);

    return 0;
}
/*
Recebe os arrray de valores do parametro, do valor da funcao e o grau do polinomiode aproximacao
Retorna um array com os parametros.
*/
double* minQuad(double x[],double y[] ,int ovo, int nPontos){
    int grau = ovo + 1;
    double A[nPontos][grau];
    double AT[grau][nPontos];
    double Y[nPontos];
    //cout<<"Matriz Normal"<<endl;
    for(int i = 0; i<nPontos; i++){
        A[i][0] = 1;//inicia a primeira coluna da matriz A com 1
        AT[0][i] = A[i][0];
        Y[i] = 0;//inicia a posicao i de Y com 0;
        //cout<<A[i][0]<< " ";
        for(int j =1 ;j<grau;j++){
            A[i][j] = A[i][j-1]*x[i];
            AT[j][i] = A[i][j];
            //Y[j] = Y[j]+ (A[i][j]*y[i]);
            //cout<<A[i][j]<<" ";
        }

        //cout<<endl;

    }
    //cout<<"Matriz Transposta"<<endl;
    for(int i=0;i<grau;i++){
            for(int j=0;j<nPontos; j++){
                //cout<<AT[i][j]<<" ";
            }
            //cout<<endl;
        }
   // for(int i=0;i<nPontos;i++){
   ////         cout<<Y[i]<<endl;
    // }

    double M  [grau][grau];
    for (int lin  = 0; lin < nPontos; lin ++){//fixe uma linha
        for(int col = 0; col < grau; col++){//para cada linha percorra a coluna
            M[lin][col]=0;
            for(int mul = 0; mul < nPontos; mul++){ //for da soma de multiplicação elemento a elemento
            ////    cout<< "AT["<<lin<<"]["<<mul<<"]* A["<< col<<"]["<<mul<<"]"<<"="<<AT[lin][mul]*A[mul][lin]<<endl;
              M[lin][col] = M[lin][col] + AT[lin][mul]*A[mul][col]; // a linha da transposta é a coluna da normal
            }
        }
    }


    for (int lin  = 0; lin < nPontos; lin ++){//fixe uma linha
            for(int mul = 0; mul < nPontos; mul++){ //for da soma de multiplicação elemento a elemento
            ////    cout<< "AT["<<lin<<"]["<<mul<<"]* A["<< col<<"]["<<mul<<"]"<<"="<<AT[lin][mul]*A[mul][lin]<<endl;
              Y[lin]= Y[lin]+ AT[lin][mul]*y[mul]; // a linha da transposta é a coluna da normal
            }
    }


    //cout<<"Resultado: "<<endl;
    for(int i=0;i<grau;i++){
            for(int j=0;j<grau; j++){
                //cout<<M[i][j]<<" ";
            }
            //cout<<endl;
        }
    double a [grau];
    return a;
}
