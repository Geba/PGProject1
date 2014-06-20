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

void printControlPoints()
{
    printf("Pontos de controle\n(x, y)\n");
    for(int i=0; i< PC.size(); i++)
    {
        printf("%d, %d, \n", PC[i].x,PC[i].y);
    }
}
void printBezierPoints()
{
    printf("Pontos de controle\n(x, y)\n");
    for(int i=0; i< PB.size(); i++)
    {
        printf("%d, %d, \n", PB[i].x,PB[i].y);
    }
}

void printGrauAtual()
{
    cout<<"Grau atual: "<<endl<<
        grauAtual<<endl;
}
void printQntAvaliacoes()
{
    cout<<"Quantidade de avaliações atual: "<<endl<<avaliacoes<<endl;

}
void printAutoTValue()
{
    cout<<"A Geracao automatica de Parametro está: ";
    if(autoT){
        cout<<"Ativada"<<endl;
    }else{
            cout<<"Desativada"<<endl;
    }

}
void printAutoGrauValue()
{
    cout<<"A Geracao automatica de Grau está: ";
    if(autoGrau){
        cout<<"Ativada"<<endl;
    }else{
            cout<<"Desativada"<<endl;
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
    userPoints.push_back(aux);
    if(autoT)
    {
        igualarPoints();
    }
    else
    {
        cout<<"Digite o t Para esse ponto:"<<endl;


    }
    if(autoGrau)
    {
        maxgrau = userPoints.size()-1;
        grauAtual=userPoints.size()-1;
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

        /*
            glBegin(GL_LINE_STRIP);
                for (int x = -4.0; x <4.0; x+=0.1){
                float y = sin(3.14 * x) / (3.14 * x);
                glVertex2f (x,y);
        }
        glEnd();
        */
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
        refresh();
    }

}
void MouseClick (int button, int estado, int x, int y)
{
    float intervalo = 8.0f;
    float intervaloMin = -8.0f;
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        //   printf("ESQ ");
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
        break;
    case GLUT_RIGHT_BUTTON:
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
void configure()
{
    cout<<"Digite o grau desejado para as curvas desenhadas"<<endl;
    scanf("%d", &grauAtual);
    cout<<"Digite a quantidade de avaliações desejadas para o algoritmo de Decastelljeau"<<endl;
    scanf("%d", &avaliacoes);

}

void hadleKeyboard(unsigned char key, int x, int y)
{
    /*Comandos:
    p = print
    c: printControlPoints//ffeita
    u: printUserPoints//feita//
    b: printBezierPoints//feita
    g: printGrauAtual;//feita
    a: printQntAvaliacoes;//feita
    t: printAutoTValue;//feita
    v: printAutoGrauValue//feita

    a = alterar
    g: alterarGrauAtual;
    h: alterarqntAvaliacoes
    t: alterarAutoTValue;
    v: alterarAutograuValue

    c =  clearConsole
    esc = exit

    */
    if(key == 'e')
    {
        exit(0);
    }
    else if(key == 'p')
    {
        char escolha;
        cout<<"digite a sua opção: "<<endl
        <<"c: Imprimir Pontos de Controle"<<endl
        <<"u: Imprimir Pontos Inseridos"<<endl
        <<"b: Imprimir Pontos da Bezier"<<endl
        <<"g: Imprimir Grau da Curva"<<endl
        <<"a: Imprimir Quantidade de Avalições"<<endl
        <<"t: Imprimir Ajuste automatico de Parametro"<<endl
        <<"v: Imprimir Ajuste Automatico de Grau"<<endl;
        scanf("%c", &escolha);
        if(escolha=='c')
        {
            printControlPoints();
        }
        else if(escolha=='u')
        {
                        printUserPoints();
        }
        else if(escolha=='b')
        {
                printBezierPoints();
        }
        else if(escolha=='g')
        {
                printGrauAtual();
        }
        else if(escolha=='a')
        {
                printQntAvaliacoes();
        }
        else if(escolha=='t')
        {
                printAutoTValue();
        }
        else if(escolha=='v')
        {
                printAutoGrauValue();
        }else{
            cout<<"Opcao Não encontrada!"<<endl;
        }
    }
    else if(key == 'a')
    {
        char escolha;
        cout<<"digite a sua opção: "<<endl
        <<"g: Alterar grau da curva"<<endl
        <<"h: Alterar Quantidade de Avaliações"<<endl
        <<"t: Ligar ou desligar Ajuste Automático de Parametro:"<<endl
        <<"v: Ligar ou desligar Ajuste Automatico de Grau"<<endl;
        scanf("%c", &escolha);
        if(escolha=='v')
        {
            printAutoGrauValue();
            cout<<"Digite a para ativar, d para desativar"<<endl;
            char aux;
            scanf("%c", &aux);
            if(aux =='a')
            {
                autoGrau=true;
            }
            else if(aux=='d')
            {
                autoGrau = false;
            }

        }
        else if(escolha=='t')
        {
            printAutoTValue();
            cout<<"Digite a para ativar, d para desativar"<<endl;
            char aux;
            scanf("%c", &aux);
            if(aux =='a')
            {
                autoT=true;
            }
            else if(aux=='d')
            {
                autoT = false;
            }
        }
        else if (escolha=='g')
        {
            printGrauAtual();
            cout<<"Digite o novo valor para o grau da curva"<<endl;
            scanf("%d", &grauAtual);
            if (grauAtual>(userPoints.size()-1)){
            cout<<"Note que um grau de curva maior ou igual que a quantidade de pontos  deixará a curva instável"<<endl;
            }
        }
        else if(escolha=='a')
        {
            printQntAvaliacoes();
            cout<<"Digite a nova quantidade de avaliações do algoritmo de De Casteljau"<<endl;
            scanf("%d", &avaliacoes);
        }
        mudou = true;
    }
    else if(key == 'c')
    {
        //clearConsole();
    }
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
    glutMouseFunc(MouseClick);
    glutMotionFunc(mouseClicking);
    glutKeyboardUpFunc(hadleKeyboard);
    Inicializa();
    glutMainLoop();

}
