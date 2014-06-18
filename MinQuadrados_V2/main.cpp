#include <iostream>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;

double* minQuad(double x[],double y[] ,int ovo, int nPontos)
{
    int grau = ovo + 1;
    MatrixXd A = MatrixXd(nPontos, grau);
    MatrixXd AT = MatrixXd(grau, nPontos);
    VectorXd Y(grau);
    VectorXd Yaux(nPontos);
    for(int i = 0; i<nPontos; i++)
    {
        A(i,0) = 1;//inicia a primeira coluna da MatrixXdriz A com 1
        AT(0,i) = 1;
        for(int j =1 ; j<grau; j++)
        {
            A(i,j) = A(i,(j-1))*x[i];
            AT(j,i) = A(i,j);
        }
    }
    MatrixXd M = AT*A;
    cout<<2<<endl;
    for(int i = 0; i<nPontos; i++)
    {        Yaux(i) = y[i];
    }
    Y = AT*Yaux;
    VectorXd resp1(grau);
    VectorXd resp2(grau);
    VectorXd resp3(grau);
    VectorXd resp4(grau);
    VectorXd resp5(grau);
    VectorXd resp6(grau);
    VectorXd resp7(grau);
    resp1  = M.partialPivLu().solve(Y);
    resp2  = M.fullPivLu().solve(Y);
    resp3  = M.householderQr().solve(Y);
    resp6  = M.llt().solve(Y);


    cout<<"M:"<<endl<<M<<endl;
    cout<<"Y:"<<endl<<Y<<endl;
    cout<<"Resp1:"<<endl<<resp1<<endl;
    cout<<"Resp2:"<<endl<<resp2<<endl;
    cout<<"Resp3:"<<endl<<resp3<<endl;
    cout<<"Resp6:"<<endl<<resp6<<endl;


    double coef[grau];
    for(int k = 0; k<grau;k++){
        coef[k] = resp1(k);
    }
    return coef;


}


int main()
{
   //double x[] = {1872, 1890,1900,1920,1940,1950,1960,1970,1980,1991,1996};
   double x [] = {1,2,3,4,5};
   double y [] = {10,40,50,32,8};
//    double y[] = {9.9 ,14.3 ,17.4 ,30.6, 41.2, 51.9, 70.2, 93.1, 119.0, 146.2, 157.1};
//double y [] = {2.29253, 2.66026 ,2.85647, 3.421 ,3.71844 ,3.94932 ,4.25135 ,4.53367 ,4.77912, 4.98498 ,5.05688};
/*
    double x[] = {1,2,3,4,5,6,7,8};
    double y[] = {.5,.6,.9,.8,1.2,1.5,1.7,2};
*/

    //double x[] = {40,27}
    //cout << "Hello world!" <<endl;
    //minQuad(x, y, , 5);
    //minQuad(x, y, 4, 5);
 MatrixXd A = MatrixXd(3, 3);
    VectorXd B = VectorXd(3);
    /*A(0,0) = -0.421 ;
    A(0,1) =  0.784;
    A(0,2) =  0.279;
    A(1,0) =  0.448;
    A(1,1) =  0.832;
    A(1,2) =  0.193;
    A(2,0) =  0.421;
    A(2,1) =  0.784;
    A(2,2) =  -0.207;
    B(0) = 0;
    B(1) = 1;
    B(2) = 0;
    //a resposta é
    */
    VectorXd resp1(3);
    VectorXd resp2(3);
    VectorXd resp3(3);
    VectorXd resp4(3);
    VectorXd resp5(3);
    VectorXd resp6(3);
    VectorXd resp7(3);
    resp1  = A.partialPivLu().solve(B);
    resp2  = A.fullPivLu().solve(B);
    resp3  = A.householderQr().solve(B);
  //  resp6  = A.llt().solve(B);//Nao bateu


    cout<<"A:"<<endl<<A<<endl;
    cout<<"B:"<<endl<<B<<endl;
    cout<<"Resp1:"<<endl<<resp1<<endl;
    cout<<"Resp2:"<<endl<<resp2<<endl;
    cout<<"Resp3:"<<endl<<resp3<<endl;
    cout<<"Resp6:"<<endl<<resp6<<endl;





    return 0;
}
