/*
Computer Science Center - CIn
Federal University of Pernambuco - UFPE
2014.1


@authors:
Daniel Sulman de Albuquerque Eloi - @dsae
Dimas Albuquerque Mendes - @dam4
Gabriela Mota de Lacerda - @gml
Geovane Silva Pereira - @gsp



Descrição: o usuário entra via mouse com os pontos ordenados
(do tipo (ti,fi),valor de t e valor de função). O usuário escolhe
o grau da curva e o sistema deve apresentar a curva de Bézier com
o grau desejado que melhor se aproxima dos pontos dados,
pelo critério dos mínimos quadrados. O usuário poderá mover o ponto
e o sistema deve responder em tempo real com o desenho da curva mais
próxima. O sistema também deve apresentar os pontos de controle da curva
encontrada. Este projeto pode utilizar resolvedores prontos  de sistemas lineares
de grau arbitrário.
*/

#include <windows.h>
#include <gl/glut.h>
#include <stdio.h>
#include<vector>
#include <math.h>
#include <iostream>
#include <Eigen/Dense>
#include<cmath>
#include <cstdlib>
#include <conio.h>



using namespace std;
using namespace Eigen;
int tamanhoJanela;
int grauAtual = 2;
int maxgrau = 0;
int avaliacoes = 100;
bool autoT = true;//gera os Ts Na ordem em que foram criados
bool autoGrau = true;//aproxiiima os pontos para uma curva de grau = nPontos-1
//estados do mouse
const int NOTHING = 1;
const int SELECTING = 2;
const int ADDING = 3;
const int DELETING = 4;
int estadoUser = NOTHING;
int indiceSelecionado = 0;
bool mudou = true;

//Variáveis auxiliares coordenadas
int menu_x_aux = 0;
int menu_y_aux = 0;
float intervalo = 8.0f;
float intervaloMin = -8.0f;

//Modos dos Teclado
const int SELECT_POINTS = 0;
const int DELETE_POINTS = 1;
int modo = SELECT_POINTS;

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
void clearConsole()
{
    std::system("cls");

}



void printControlPoints()
{
    printf("\nPontos de controle\n(x, y)\n");
    for(int i=0; i< PC.size(); i++)
    {
        printf("%d, %d \n\n", PC[i].x,PC[i].y);
    }
}
void printBezierPoints()
{
    printf("\nPontos da Bezier\n(x, y)\n");
    for(int i=0; i< PB.size(); i++)
    {
        printf("%d, %d \n\n", PB[i].x,PB[i].y);
    }
}

void printGrauAtual()
{
    cout<< endl << "Grau atual: "<<endl<<
        grauAtual<<endl;
}
void printQntAvaliacoes()
{
    cout<< endl << "Quantidade de avaliacoes atual: "<<endl<<avaliacoes<<endl;

}
void printAutoTValue()
{
    cout<<"A Geracao automatica de Parametro esta: ";
    if(autoT)
    {
        cout<<"Ativada"<<endl << endl;
    }
    else
    {
        cout<<"Desativada" << endl << endl;
    }

}
void printAutoGrauValue()
{
    cout<<"A Geracao automatica de Grau esta: ";
    if(autoGrau)
    {
        cout<<"Ativada"<<endl << endl;
    }
    else
    {
        cout<<"Desativada" <<endl << endl;
    }
}

void igualarPoints()
{
    if(userPoints.size()>0)
    {
        double step = 1.0/(userPoints.size()-1);
        for (int i = 0; i<userPoints.size(); i++)
        {
            userPoints[i].t = i*step;
        }
    }
}

void addPoint(int x, int y)
{
    point aux;
    aux.x = x;
    aux.y =y;

    if(autoT)
    {
        igualarPoints();
    }
    else
    {
        cout<<"Digite o T Para esse ponto:" << endl;
        scanf("%f", &aux.t);

    }
    userPoints.push_back(aux);

    if(autoGrau)
    {
        maxgrau = userPoints.size()-1;
        grauAtual = userPoints.size()-1;
    }
}

void delPoint(int indice)
{
    userPoints.erase(userPoints.begin() + indice);
    if(autoT)
    {
        igualarPoints();
    }
    if(autoGrau)
    {
        maxgrau = userPoints.size()-1;
        grauAtual=userPoints.size()-1;
    }

}


void Inicializa (void)
{
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f,48.0f/255.0f, 64.0f/255.0f, 1.0f);
    estadoUser = NOTHING;
}
double fatorial1(double k)
{
    if(k == 0)
    {
        return 1;
    }
    else
    {
        return k *fatorial1(k-1);
    }
}
long double resultados[1000] = {1,1,0};
bool resultadosAchou[1000] = {true,true,false};

long double fatorial(int n)
{
    if (resultadosAchou[n])
    {
        return resultados[n];
    }
    else
    {
        if (n == 0 || n == 1)
        {
            return 1;
        }
        else
        {
            if (resultadosAchou[n - 1])
            {
                resultados[n] = (floor(resultados[n-1] + 0.5) * (double) n);
                resultadosAchou[n] = true;
                return resultados[n];
            }
            else
            {
                resultados[n] = ((n) * fatorial(n-1));
                return resultados[n];
            }


        }

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
            coef(grau - j,i) = combinacao(aux, j) * combinacao(grau,i);
            if(((i%2!=0&&((grau-j)%2==0)||(i%2==0&&(grau-j)%2!=0))&&(coef(grau-j,i)!=0)))
            {
                coef(grau-j, i) = (-1)*(coef(grau-j, i));
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

vector<Ponto_de_Controle> getCurvePoints(int avaliacoes, vector<Ponto_de_Controle> Pontos_Controle)//De Casteljau
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
    glClear(GL_COLOR_BUFFER_BIT); // Limpa a janela de visualizacao com a cor de fundo especificada
    glPointSize(8);
    glLineWidth(2);
    //deseha pontos do usuário
    glBegin(GL_POINTS);
    for(int i =0; i < userPoints.size(); i++)//desenhas pontos inseridos pelo usuario
    {
        glColor3f(1.0f, 1.0f, 1.0f);//Branco
        glVertex2i(userPoints[i].x,tamanhoJanela-userPoints[i].y);
    }
    glEnd();
    //
    if(userPoints.size()>0)
    {
        //calcula e desenha os pontos de controle
        if(mudou)
        {
            PC = getControlPoints(grauAtual,userPoints);
            PB = getCurvePoints(avaliacoes,PC);//pontos da bezier
            mudou = false;
        }

        glColor3f(1.f, (203.0f/255.0f), 0.0f);//amarelo
        glBegin(GL_POINTS);
        glPointSize(4);
        for(int i =0; i < PC.size(); i++)
        {
            glVertex2i(PC[i].x,tamanhoJanela-PC[i].y);
        }
        glEnd();//

        //desenha a curva usando os pontos da curva
        glBegin(GL_LINES);
        for(int i =1; i < PB.size(); i++)
        {
            Ponto_de_Controle u = PB[i-1];
            glColor3f(26.0f/255.0f, 1.0f, 188.0f/255.0f);//cor da linha
            glVertex2i(PB[i].x,tamanhoJanela-PB[i].y);
            glVertex2i(u.x,tamanhoJanela-u.y);
        }
        glEnd();


        glEnd();

    }
    glFlush();
}

void mouseClicking(int x, int y)
{
    if (estadoUser == SELECTING)
    {
        //  cout<<userPoints[indiceSelecionado].x <<" " <<userPoints[indiceSelecionado].y <<endl;
        userPoints[indiceSelecionado].x = x;
        userPoints[indiceSelecionado].y = y;
        mudou = true;
        refresh();
    }

}

void configure(int opcao)
{
    switch (opcao)
    {
    case 0:
        cout<<"Digite o grau desejado para as curvas desenhadas"<<endl;
        scanf("%d", &grauAtual);
        break;
    case 1:
        cout<<"Digite a quantidade de avaliacoes desejadas para o algoritmo de De Casteljau"<<endl;
        scanf("%d", &avaliacoes);
    }
}

void reset()
{

    userPoints.clear();
    mudou=true;
    refresh();
    clearConsole();
    cout << "Voce esta no modo de adicionar pontos" << endl;
    modo = SELECT_POINTS;
}

void MenuPrincipal(int op)
{

    switch (op)
    {
    case 0:
        reset();
        break;
    case 1:
        clearConsole();
        break;
    case 2:
        exit(0);
        break;
    }
}



void MenuImprimir(int op)
{

    switch (op)
    {
    case 0:
        if (userPoints.size() > 0)
        printControlPoints();
        else
            cout << endl << "Por favor, adicione algum ponto" << endl;
        break;
    case 1:
        if (userPoints.size() > 0)
        printUserPoints();
        else
            cout << endl << "Por favor, adicione algum ponto" << endl;
        break;
    case 2:
        if (userPoints.size() > 0)
        printBezierPoints();
          else
            cout << endl << "Por favor, adicione algum ponto" << endl;
        break;

    case 3:
        if (userPoints.size() > 0)
        printGrauAtual();
          else
            cout << endl << "Por favor, adicione algum ponto" << endl;
        break;

    case 4:
        if (userPoints.size() > 0)
        printQntAvaliacoes();
          else
            cout << endl << "Por favor, adicione algum ponto" << endl;
        break;

    case 5:
        if (userPoints.size() > 0)
        printAutoTValue();
          else
            cout << endl << "Por favor, adicione algum ponto" << endl;
        break;

    case 6:
        if (userPoints.size() > 0)
        printAutoGrauValue();
          else
            cout << endl << "Por favor, adicione algum ponto" << endl;
        break;
    default:
        cout<<"Opcao Não encontrada!"<<endl;
    }
}




void MenuLigaDes(int op)
{
    switch (op)
    {
    case 0:
        if (autoGrau)
        {
            printf("\nGrau Automatico Desativado\n");
            autoGrau = false;
        }
        else
        {
            printf("\nGrau Automatico Ativado\n");
            autoGrau = true;
        }

        break;
    case 1:
        if (autoT)
        {
            printf("\nParametro T Automatico Desativado\n");
            autoT = false;
        }
        else
        {
            printf("\nParametro T Automatico Ativado\n");
            autoT = true;
        }
         break;
    }
}

void MenuGrauAva(int op)
{

    switch(op)
    {
    case 0:
        printGrauAtual();
        cout<<"Digite o novo valor para o grau da curva"<<endl;
        autoGrau = false;
        scanf("%d", &grauAtual);
        cout<<endl<<"O ajuste automatico de grau foi desativado"<<endl;
        if (grauAtual>(userPoints.size()-1))
            cout<<"Note que um grau de curva maior ou igual que a quantidade de pontos" << endl << "deixara a curva instavel"
            <<endl <<endl << "Grau Alterado" << endl;
        break;
    case 1:
        printQntAvaliacoes();
        cout<<"Digite a nova quantidade de avaliacoes do algoritmo de De Casteljau: "<<endl;
        scanf("%d", &avaliacoes);
        cout << endl << "Avaliacoes Alterado" << endl;
        break;
    default:
        cout<<"Opcao nao encontrada!"<<endl;
    }
    mudou = true;
}

void nada(int op)
{
}



void setModo(int op)
{
    if(op == 0)
    {
        modo = SELECT_POINTS;
        cout<< "Voce esta no modo de selecao de pontos "<< endl;
    }
    else if(op == 1)
    {
        modo = DELETE_POINTS;
        cout<< "Voce esta no modo de Delecao de pontos"<<endl;
    }
}


void MouseClick (int button, int estado, int x, int y)
{
    //cout<<modo<<endl;
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if(modo==SELECT_POINTS)
        {
            if (estado == GLUT_DOWN)
            {
                bool existe = false;
                for(int i = 0; (!existe)&(i< userPoints.size()); i++)
                {
                    if((userPoints[i].x-x>=intervaloMin)&&(userPoints[i].x-x<=intervalo)
                            &&(userPoints[i].y-y>=intervaloMin)&&(userPoints[i].y-y<=intervalo))
                    {
                        indiceSelecionado = i;
                        estadoUser=SELECTING;
                        existe = true;
                    }
                }
                if(!existe)
                {
                    estadoUser=ADDING;
                }
            }
            // printf("Pressionado na posição: ");
            if (estado == GLUT_UP)
            {
                //printf("Posiçao solta: ");
                if(estadoUser==ADDING)
                {
                    addPoint(x,y);
                }
                estadoUser = NOTHING;
                mudou = true;
            }
        }
        else if(modo==DELETE_POINTS)
        {
           //cout <<"estado de delecao"<<endl;
            if (estado == GLUT_DOWN)
            {
                bool existe = false;
                for(int i = 0; (!existe)&(i< userPoints.size()); i++)
                {
                    if((userPoints[i].x-x>=intervaloMin)&&(userPoints[i].x-x<=intervalo)
                            &&(userPoints[i].y-y>=intervaloMin)&&(userPoints[i].y-y<=intervalo))
                    {
                        indiceSelecionado = i;
                        estadoUser=DELETING;
                        existe = true;
                    }
                }

            }
            // printf("Pressionado na posição: ");
            if (estado == GLUT_UP&&estadoUser==DELETING)
            {
                delPoint(indiceSelecionado);
                estadoUser = NOTHING;
                mudou = true;
            }
        }

        //   printf("ESQ ");

        break;
    case GLUT_RIGHT_BUTTON:

        break;
    case GLUT_MIDDLE_BUTTON:
        //printf("MEIO ");
        break;
    }
//    Desenha(x,y);
    refresh();
}


void CriaMenu()
{


//    menu_x_aux = x;
//    menu_y_aux = y;
    int menu,submenuImprimir,submenuConfig, submenuAlterar,submenuLigDes;

    modo = glutCreateMenu(setModo);
    glutAddMenuEntry("Adicionar Pontos", 0);
    glutAddMenuEntry("Deletar Pontos", 1);

    submenuImprimir = glutCreateMenu(MenuImprimir);
    glutAddMenuEntry("Pontos de Controle",0);
    glutAddMenuEntry("Pontos Inseridos",1);
    glutAddMenuEntry("Pontos de Bezier",2);
    glutAddMenuEntry("Grau da Curva",3);
    glutAddMenuEntry("Quantidade de Avaliações",4);
    glutAddMenuEntry("Ajuste Automático Parâmetro",5);
    glutAddMenuEntry("Ajuste Automático de Grau",6);

    submenuAlterar = glutCreateMenu(MenuGrauAva);
    glutAddMenuEntry("Grau",0);
    glutAddMenuEntry("Avaliações",1);


    submenuLigDes = glutCreateMenu(MenuLigaDes);
    glutAddMenuEntry("Auto Ajuste Grau",0);
    glutAddMenuEntry("Auto Ajuste Parâmetro",1);


    submenuConfig = glutCreateMenu(nada);
    glutAddSubMenu("Alterar",submenuAlterar);
    glutAddSubMenu("Ligar/Desligar",submenuLigDes);


    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Modo", modo);
    glutAddSubMenu("Imprimir Valores",submenuImprimir);
    glutAddSubMenu("Configuração",submenuConfig);
    glutAddMenuEntry("Limpar Desenho",0);
    glutAddMenuEntry("Limpar Console", 1);
    glutAddMenuEntry("Fechar Programa",2);


    glutAttachMenu(GLUT_RIGHT_BUTTON);
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
    glutCreateWindow("PG");
    PC.clear();
    PB.clear();
    userPoints.clear();
    glutReshapeFunc(AlteraTamanhoJanela);
    glutDisplayFunc(refresh);
    CriaMenu();
    glutMouseFunc(MouseClick);
    glutMotionFunc(mouseClicking);
    //glutKeyboardUpFunc(handleKeyboard);

    Inicializa();
    reset();
    glutMainLoop();

}
