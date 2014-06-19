#include <windows.h>
#include <gl/glut.h>
#include <stdio.h>
#include<vector>
#include <math.h>
#include <iostream>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
int tamanhoJanela;
int a ,b ;
bool temT = false;
float auxT = 0.0f;
int grauAtual = 0;
int maxgrau = 0;
bool comecou = false;
int avaliacoes = 1000;
bool andando;


struct point
{
    int x;
    int y;
    float t;
};

typedef struct ponto_controle
{
    int x;
    int y;

} Ponto_de_Controle;

vector <point> userPoints;
vector<Ponto_de_Controle> PC ;
vector<Ponto_de_Controle> PB ;

void printUserPoints()
{
    printf("\n(x, y, t)\n");
    for(unsigned int i=0; i< userPoints.size(); i++)
    {
        printf("%d, %d, %f\n", userPoints[i].x,userPoints[i].y, userPoints[i].t);
    }
}

void printPontosControle(vector<Ponto_de_Controle> v){
    printf("Pontos de controle\n(x, y)\n");
    for(unsigned int i=0; i< v.size(); i++)
    {
        printf("%d, %d, \n", v[i].x,v[i].y);
    }


}

void printGrauAtual()
{
    cout<<"Grau atual: "<<grauAtual<<endl;
}


void igualarPoints()
{

    if(userPoints.size()>0){
        double step = 1.0/(userPoints.size()-1);
        for (int i = 0; i<userPoints.size();i++){
        userPoints[i].t = i*step;

            }
    }

}
void addPoint(int x, int y)
{
    point aux;
    aux.x = x;
    aux.y =y;
    if(temT)
    {
        aux.t = auxT;
        temT = false;
    }
    else
    {
        printf("\nPor favor, digite o t desse ponto: ");
       scanf("%f", &aux.t);
        comecou  = true;
        maxgrau++;
    }
    userPoints.push_back(aux);
    //igualarPoints();
    printUserPoints();


}

// Called to draw scene
//Essa tem que estar aqui

void MouseAndandoNaoApertando (int x, int y)
{
    printf("Mouse andando. Pos: (%d, %d)\n", x,y);
}

void MouseAndandoApertando (int x, int y)
{
    printf("Mouse segurando clique. Pos: (%d, %d)\n", x,y);
}


// Inicializa parâmetros de rendering
void Inicializa (void)
{
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f,48.0f/255.0f, 64.0f/255.0f, 1.0f);
}

// Função callback chamada quando o tamanho da janela é alterado

double fatorial(double k)
{
    if(k == 0)
    {
        return 1;
    }
    else
    {
        return k *fatorial(k-1);
    }
}

double combinacao(double n, double p)
{
    if(p > n)
    {
        return 0;
    }
    else
    {
        double c = fatorial(n)/(fatorial(p)*fatorial(n-p));
        return c;
    }
}

MatrixXd getMatriz(int grau)
{
    MatrixXd coef(grau+1,grau+1);
    coef.setZero();
    int aux = grau;
    //for de todos os b's e já transpõe
    for (int i = 0; i < grau+1; i++){
        if(i > 0){
            aux = aux- 1;
        }
        //cout << endl;
        //preenchendo as linhas ou seja b0, b1...
        for (int j = 0; j < grau + 1; j++){

            /*if(((j%2!=0&&i%2==0)||(j%2==0&&i%2!=0))){
                double auxdentu = (combinacao(aux, j));
                if (auxdentu != 0)
                    coef(grau - j,i) = (-1)*(combinacao(aux, j));
            }
            else{*/
                coef(grau - j,i) = combinacao(aux, j);
            //}
            coef(grau - j,i)*=combinacao(grau,i);
        }

    }

    for (int i =0;i<=grau;i++){
        for( int j = 0;j<=grau;j++){
            if(((i%2!=0&&j%2==0)||(i%2==0&&j%2!=0))&&(coef(i,j)!=0)){
                    coef(i, j) = (-1)*(coef(i, j));
            }
        }

    }
    return coef;// matriz ta pronta agora
}

VectorXd minQuad(double x[],double y[] ,int ovo, int nPontos)
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

    for(int i = 0; i<nPontos; i++)
    {
        Yaux(i) = y[i];
    }
    Y = AT*Yaux;
    VectorXd resp1(grau);
    // VectorXd resp3(grau);
    //VectorXd resp4(grau);
    //resp1  = M.partialPivLu().solve(Y);
    resp1  = M.householderQr().solve(Y);
    //    cout<<"M:"<<endl<<M<<endl;
    //  cout<<"Y:"<<endl<<Y<<endl;
    //cout<<"Resp1:"<<endl<<resp1<<endl;
    // cout<<"Resp2:"<<endl<<resp2<<endl;
    //cout<<"Resp3:"<<endl<<resp3<<endl;
//    cout<<"Resp6:"<<endl<<resp6<<endl;
    //double coef[grau];
    //for(int k = 0; k<grau; k++)
    //{
        //coef[k] = resp1(k);
    //}

    //cout<<"minQuadResp:" <<resp1<<endl;
    return resp1;
}
//minQuad(double x[],double y[] ,int ovo, int nPontos)

vector<Ponto_de_Controle> getControlPoints(int grau, vector<point> usrPoints )
{
    int size_  = usrPoints.size();
    double x[size_];
    double y[size_];
    double t[size_];
    //inicializando
    for (int i =0; i<size_; i++)
    {
        x[i] = usrPoints[i].x;
        y[i] = usrPoints[i].y;
        t[i] = usrPoints[i].t;
    }
    VectorXd xcoef;
    VectorXd ycoef;
    VectorXd xCtrlPnts;
    VectorXd yCtrlPnts;
    xcoef.setZero();
    ycoef.setZero();
    xCtrlPnts.setZero();
    yCtrlPnts.setZero();
    //inicializando
    xcoef = minQuad(t,x, grau, size_);
    //cout<<xcoef<<endl;
    ycoef = minQuad(t,y, grau, size_);
    //cout<<ycoef<<endl;
    MatrixXd atcoef = getMatriz(grau);
    xCtrlPnts = atcoef.partialPivLu().solve(xcoef);
    yCtrlPnts = atcoef.partialPivLu().solve(ycoef);
    vector<Ponto_de_Controle> pcs;
    pcs.clear();
    for (int i = 0; i<=grau; i++)
    {
        Ponto_de_Controle pc;
        pc.x = xCtrlPnts[i];
        pc.y = yCtrlPnts[i];
        pcs.push_back(pc);
    }
    return pcs;
}




vector<Ponto_de_Controle> getCurvePoints(int avaliacoes, vector<Ponto_de_Controle> Pontos_Controle)//decastelljeau
{
    vector<Ponto_de_Controle> Q ;
//    cout<<"chamou uma";
    vector<Ponto_de_Controle> retorno;
    double step = 1.0/avaliacoes;
    for(double t=0; t<=(1.0f+step); t+=step)
    {
        //cout<<t<<endl;
        //getchar();
        Q.clear();
        for(int i=0; i<=Pontos_Controle.size(); i++)
        {
            Q.push_back(Pontos_Controle[i]);
        }
        for(int k=1; k<=Pontos_Controle.size(); k++)
        {
            for(int i=0; i<(Pontos_Controle.size()-k); i++)
            {
                Q[i].x = (1-t)*Q[i].x + t*Q[i+1].x;
                Q[i].y = (1-t)*Q[i].y + t*Q[i+1].y;
            }
        }

        retorno.push_back(Q[0]);
    }
    int a = (Q.size()-1);
    //cout<<Q[a].x<<" "<<Q[a].y<< endl;
    return retorno;
}




void refresh()
{
     // Limpa a janela com a cor
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(2, 2, 2);//nao é a cor de fundo
    glFlush();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT); // Limpa a janela de visualização com a cor de fundo especificada
    glPointSize(8);
    glLineWidth(2);
   // igualarPoints();
    glBegin(GL_POINTS);
    for(int i =0; i < userPoints.size(); i++)//desenhas pontos inseridos pelo usuario
    {
        glColor3f(1.0f, 1.0f, 1.0f);//Branco
        glVertex2i(userPoints[i].x,tamanhoJanela-userPoints[i].y);
    }
    glEnd();
    if(userPoints.size()>0){

    //calcula e desenha os pontos de controle
    PC = getControlPoints(grauAtual,userPoints);
    glColor3f(1.f, (203.0f/255.0f), 0.0f);//amarelo
    glBegin(GL_POINTS);
    glPointSize(4);
    for(int i =0; i < PC.size(); i++)
    {
        glVertex2i(PC[i].x,tamanhoJanela-PC[i].y);
    }
    glEnd();
    printPontosControle(PC);
    PB = getCurvePoints(avaliacoes,PC);
    glPointSize(2);
    glBegin(GL_POINTS);
    for(int i =0; i < PB.size(); i++)
    {
        glColor3f(1.0f, 0.5f, .5f);//cor do ponto
        glVertex2i(PB[i].x,tamanhoJanela-PB[i].y);

    }
    glEnd();
    //testando pontos da curva


    glBegin(GL_LINES);
    for(int i =1; i < PB.size(); i++)
    {
        Ponto_de_Controle u = PB[i-1];
        glColor3f(26.0f/255.0f, 1.0f, 188.0f/255.0f);//cor da linha
        glVertex2i(PB[i].x,tamanhoJanela-PB[i].y);
        glVertex2i(u.x,tamanhoJanela-u.y);
        u = PB[i];
    }



    glEnd();

    /*
        glBegin(GL_LINE_STRIP);
            for (int x = -4.0; x <4.0; x+=0.1){
            float y = sin(3.14 * x) / (3.14 * x);
            glVertex2f (x,y);
    }
    glEnd();
    */
}

 //   getControlPoints(grauAtual,userPoints);
    //glVertex2i(x,tamanhoJanela-y);
    //Executa os comandos OpenGL
    glFlush();

}

void AtualizarCurva(int i){
    while(false){
        int x,y;
        //x = ;
        //y = ;
        cout<<"teste";
        //PB[i].x = x;
        //PB[i].y = y;
    }
    cout<<"oi"<<endl;
}

void MouseClick (int button, int estado, int x, int y)
{
    float intervalo = 8.0f;
    float intervaloMin = -8.0f;
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (estado == GLUT_DOWN)
        {
            bool existe = false;
            for(int i = 0; (!existe)&(i< userPoints.size()); i++)
            {
                if((userPoints[i].x-x>=intervaloMin)&&(userPoints[i].x-x<=intervalo)
                        &&(userPoints[i].y-y>=intervaloMin)&&(userPoints[i].y-y<=intervalo))
                {
                    auxT = userPoints[i].t;
                    temT = true;
                    userPoints.erase(userPoints.begin() + i);
                    andando = true;
                    AtualizarCurva(i);
                    return;
//                    userPoints[i].x = x;userPoints[i].y = y;

                }
            }
        }
       // printf("Pressionado na posição: ");
        if (estado == GLUT_UP)
        {
            andando = false;
            //printf("Posiçao solta: ");
            addPoint(x,y);
        }
        break;
    case GLUT_RIGHT_BUTTON:
        //printf("DIR ");
        break;
    case GLUT_MIDDLE_BUTTON:
        //printf("MEIO ");
        break;
    }
//    Desenha(x,y);
    refresh();
}













































void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Evita a divisao por zero
    if(h == 0) h = 1;
    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);
    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Estabelece a janela de seleção (left, right, bottom, top)
    if (w <= h)
    {
        gluOrtho2D (0.0f, 250.0f, 0.0f, 250.0f*h/w);
        refresh();
    }
    else
    {
        gluOrtho2D (0.0f, w, 0.0f, h);
        refresh();
    }

    tamanhoJanela = h;

    refresh();
}
void configure(){
    cout<<"Digite o grau desejado para as curvas desenhadas"<<endl;
      scanf("%d", &grauAtual);
      cout<<"Digite a quantidade de avaliações desejadas para o algoritmo de Decastelljeau"<<endl;
      scanf("%d", &avaliacoes);

}

int main(void)
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(900,700);
    glutInitWindowPosition(1000,100);
    glutCreateWindow("PG");
    PC.clear();PB.clear();userPoints.clear();
    //glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutDisplayFunc(refresh);
    glutMouseFunc(MouseClick);
    Inicializa();

    configure();
    glutMainLoop();

}
