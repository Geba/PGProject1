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

void printUserPoints(){
    printf("\n(x, y, t)\n");
    for(unsigned int i=0; i< userPoints.size(); i++)
    {
        printf("%d, %d, %f\n", userPoints[i].x,userPoints[i].y, userPoints[i].t);
    }
}
void printGrauAtual(){
cout<<"Grau atual: "<<grauAtual<<endl;
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
        if(userPoints.size()>0)
            grauAtual++;
            maxgrau++;
    }
    userPoints.push_back(aux);
    printUserPoints();
    //printGrauAtual();

}




// Função desenhar

/*void Desenha(int x, int y)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Limpa a janela de visualização com a cor de fundo especificada
    glClear(GL_COLOR_BUFFER_BIT);
    // Desenha um ponto preenchido com a cor corrente
    glPointSize(10);
    glLineWidth(2);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 97.0f/255.0f, 127.0f/255.0f);//cor do ponto
    glVertex2i(x,tamanhoJanela-y);
    glEnd();
    // Executa os comandos OpenGL
    glFlush();
}*/


// Called to draw scene
//Essa tem que estar aqui



/*
void MinimosQuadrados()
{
    //mandar o cidadão inserir um grau
    int npontos = userPoints.size();
    int grau;
    scanf("%d",grau);
    if(grau > npontos) {}

    printf("%d", npontos);
    //int[][] MSomatorio = MSomatorio[npontos][];


}*/

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
    for (int i = 0; i < grau+1; i++)
    {
        if(i > 0)
        {
            aux = aux- 1;
        }
        //cout << endl;
        //preenchendo as linhas ou seja b0, b1...
        for (int j = 0; j < grau + 1; j++)
        {
            if(j%2!=0)
            {
                double auxdentu = (combinacao(aux, j));
                if (auxdentu != 0)
                    coef(grau - j,i) = (-1)*(combinacao(aux, j));
            }
            else
            {
                coef(grau - j,i) = combinacao(aux, j);
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
    double coef[grau];
    for(int k = 0; k<grau; k++)
    {
        coef[k] = resp1(k);
    }

    //cout<<"minQuadResp:" <<resp1<<endl;
    return resp1;
}
//minQuad(double x[],double y[] ,int ovo, int nPontos)

vector<Ponto_de_Controle> getControlPoints(int grau, vector<point> usrPoints ){
    int size  = usrPoints.size();
    double x[size];
    double y[size];
    double t[size];
    //inicializando
    for (int i =0;i<size;i++){
        x[i] = usrPoints[i].x;
        y[i] = usrPoints[i].y;
        t[i] = usrPoints[i].t;
    }
    VectorXd xcoef;
    VectorXd ycoef;
    VectorXd xCtrlPnts;
    VectorXd yCtrlPnts;
    xcoef.setZero(); ycoef.setZero();
    xCtrlPnts.setZero();
    yCtrlPnts.setZero();
    //inicializando
    xcoef = minQuad(x,t, grau, size);
    cout<<xcoef<<endl;
    ycoef = minQuad(y,t, grau, size);
    MatrixXd atcoef = getMatriz(grau);
    xCtrlPnts = atcoef.partialPivLu().solve(xcoef);
    yCtrlPnts = atcoef.partialPivLu().solve(ycoef);
    vector<Ponto_de_Controle> pcs;
    pcs.clear();
    for (int i = 0;i<=grau;i++){
        Ponto_de_Controle pc;
        pc.x = xCtrlPnts[i];
        pc.y = yCtrlPnts[i];
        pcs.push_back(pc);
        }
        return pcs;
}




vector<Ponto_de_Controle> getCurvePoints(int avaliacoes, vector<Ponto_de_Controle> Pontos_Controle)
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Limpa a janela de visualização com a cor de fundo especificada
    glClear(GL_COLOR_BUFFER_BIT);
    // Desenha um ponto preenchido com a cor corrente
    glPointSize(8);
    glLineWidth(2);
    glBegin(GL_POINTS);
    for(int i =0; i < userPoints.size(); i++)
    {
        //  glColor3f(0.0f, 97.0f/255.0f, 127.0f/255.0f);//cor do ponto
        glColor3f(1.0f, 1.0f, 1.0f);//cor do ponto
        glVertex2i(userPoints[i].x,tamanhoJanela-userPoints[i].y);

    }
    glEnd();
    // glBegin(GL_LINE_STRIP);
    //vectorXf coeficientes = minQuad()

    /*
    pc.x = 203;
    pc.y = 280;
    PC.push_back(pc);
    pc.x = 123;
    pc.y = 157;
    PC.push_back(pc);
    pc.x = 235;
    pc.y = 351;
    PC.push_back(pc);
    */
    PC = getControlPoints(grauAtual,userPoints);
    PB = getCurvePoints(2000,PC);
    glColor3f(0.0f, 0.5f, 0.5f);
    glBegin(GL_POINTS);
    glPointSize(3);
    for(int i =0; i < PC.size(); i++)
    {
        glVertex2i(PC[i].x,tamanhoJanela-PC[i].y);
    }
    glEnd();
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
    for(int i =1; i < userPoints.size(); i++)
    {
        point u = userPoints[i-1];
        glColor3f(26.0f/255.0f, 1.0f, 188.0f/255.0f);//cor da linha
        glVertex2i(userPoints[i].x,tamanhoJanela-userPoints[i].y);
        glVertex2i(u.x,tamanhoJanela-u.y);
        //u = userPoints[i];
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


getControlPoints(grauAtual,userPoints);
    //glVertex2i(x,tamanhoJanela-y);
    //Executa os comandos OpenGL
    glFlush();

}

void AlterarCena(void)
{
    // Limpa a janela com a cor
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(2, 2, 2);//nao é a cor de fundo
    glFlush();
}

void MouseClick (int button, int estado, int x, int y)
{
    float intervalo = 5.0f;
    float intervaloMin = -5.0f;
    switch (button)
    {
    case GLUT_LEFT_BUTTON:

        printf("ESQ ");
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

                }
            }
        }


        printf("Pressionado na posição: ");
        if (estado == GLUT_UP)
        {
            //printf("Posiçao solta: ");
            addPoint(x,y);
        }
        break;
    case GLUT_RIGHT_BUTTON:
        printf("DIR ");
        break;
    case GLUT_MIDDLE_BUTTON:
        printf("MEIO ");
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


int main(void)
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(900,700);
    glutInitWindowPosition(1000,100);
    glutCreateWindow("Quadrado");
    //glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    // movimento SEM botão pressionado
    // glutPassiveMotionFunc(MouseAndandoNaoApertando);
    glutDisplayFunc(AlterarCena);
    // movimento COM botão pressionado
    //glutMotionFunc(MouseAndandoApertando);
    // em um botão
    glutMouseFunc(MouseClick);
    Inicializa();
    MatrixXd test = getMatriz(4.0);
    cout<<test;
       //chama os minimos quadrados e pede o grau da bezier
    //MinimosQuadrados();
    glutMainLoop();

}





// Setup the rendering state
/*
void CorTela(void)
{
    glClearColor(0, 48, 64, 1.0f);
}
*/
