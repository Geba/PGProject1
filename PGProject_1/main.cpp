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

struct point
{
    int x;
    int y;
    float t;
};

std:: vector <point> userPoints;

void addPoint(int x, int y)
{
    point aux;
    aux.x = x;
    aux.y =y;
    if(temT){
        aux.t = auxT;
        temT = false;
    }else{
        printf("\nPor favor, digite o t desse ponto: ");
      //  scanf("%f", &aux.t);
    }

    userPoints.push_back(aux);

    printf("\n(x, y, t)\n");
    for(int i=0;i< userPoints.size();i++){
        printf("%d, %d, %f\n", userPoints[i].x,userPoints[i].y, userPoints[i].t);
    }

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
    glBegin(GL_LINES);


    //  glBegin(GL_LINE_STRIP);
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



    //glVertex2i(x,tamanhoJanela-y);
    //Executa os comandos OpenGL
    glFlush();

}

// Called to draw scene
//Essa tem que estar aqui
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
        if (estado == GLUT_DOWN){
            bool existe = false;
            for(int i = 0;(!existe)&(i< userPoints.size()); i++){
                if((userPoints[i].x-x>=intervaloMin)&&(userPoints[i].x-x<=intervalo)
                   &&(userPoints[i].y-y>=intervaloMin)&&(userPoints[i].y-y<=intervalo)){
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



void MinimosQuadrados()
{
    //mandar o cidadão inserir um grau
    int npontos = userPoints.size();
    int grau;
    scanf("%d",grau);
    if(grau > npontos) {}

    printf("%d", npontos);
    //int[][] MSomatorio = MSomatorio[npontos][];


}

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
    if (w <= h){
        gluOrtho2D (0.0f, 250.0f, 0.0f, 250.0f*h/w);
        refresh();
    }
    else{
        gluOrtho2D (0.0f, w, 0.0f, h);
        refresh();
    }

    tamanhoJanela = h;

    refresh();
}




// Programa Principal
int main(void)
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400,350);
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
    //resp1  = M.partialPivLu().solve(Y);
    resp2  = M.fullPivLu().solve(Y);//mais lenta, porem mais acurada
    //resp3  = M.householderQr().solve(Y);
    //resp6  = M.llt().solve(Y);

/*
    cout<<"M:"<<endl<<M<<endl;
    cout<<"Y:"<<endl<<Y<<endl;
    cout<<"Resp1:"<<endl<<resp1<<endl;
    cout<<"Resp2:"<<endl<<resp2<<endl;
    cout<<"Resp3:"<<endl<<resp3<<endl;
    cout<<"Resp6:"<<endl<<resp6<<endl;
  */
    double coef[grau];
    for(int k = 0; k<grau;k++){
        coef[k] = resp1(k);
    }
    return coef;


}


