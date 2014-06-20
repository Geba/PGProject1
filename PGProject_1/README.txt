Especificação do trabalho:
P3-4: Aproximação de Uma Curva de Bézier Funcional:
Descrição: o usuário entra via mouse com os pontos ordenados (do tipo (ti,fi), valor de t e valor de função). O usuário escolhe o grau da curva e o sistema deve apresentar a curva de Bézier com o grau desejado que melhor se aproxima dos pontos dados, pelo critério dos mínimos quadrados. O usuário poderá mover o ponto e o sistema deve responder em tempo real com o desenho da curva mais próxima. O sistema também deve apresentar os pontos de controle da curva encontrada. Este projeto pode utilizar resolvedores prontos  de sistemas lineares de grau arbitrário.
O uso do projeto é simples, clique com o botão esquerdo do mouse para adicionar um ponto da curva. Clique e segure e você o arrastará. Ou clique com o botão direito do mouse para deletar um ponto.
Nós implementamos duas configurações que são ativadas por default, mas podem ser alteradas a qualquer momento.
Ajuste automatico de grau da curva: Se ativada, o grau da curva é definido como o número de pontos inseridos menos um.  O usuário pode definir um grau fixo, se preferir. O projeto trabalha bem com grau menores que o numero de pontos, diferente disso, a curve gerada se torna instável devido as infinitas soluções dos sistemas lineares envolvidos.
Ajuste automatico de parametro:  Se ativada, o valor dos parametros ( t) dos pontos do usuário serão distribuidos igualmente entre a quantidade de pontos, seguindo a ordem em que forem inseridos. Por exemplo, o usuario inseriu 4 pontos, os pontos terão valores de t iguais a 0.0, 0.25, 0.75 e 1.0, seguindo a ordem em que foram inseridos. O usuário pode definir o valor de t de cada ponto individualmente  no momento da insersão de cada um. Para isso basta desativar o ajuste automático.

A maioria dos comando que podem ser utilizados no programa encontram se no console para ajudar a interação do usuário, os comando usados são:

*p:imprimir
        c: Imprimir Pontos de Controle
        u: Imprimir Pontos Inseridos
        b: Imprimir Pontos da Bezier
        g: Imprimir Grau da Curva
        a: Imprimir Quantidade de Avalicoes
        t: Imprimir Ajuste Automatico de Parametro
        v: Imprimir Ajuste Automatico de Grau
*a: alterar:
        g: Alterar grau da curva
        a: Alterar quantidade de avaliacoes
        t: Ligar ou desligar ajuste automatico de parametro
        v: Ligar ou desligar ajuste automatico de grau
*r: reset do sistema.
*e: sair do sistema.

OBS.: Os comando com * devem ser usados na janela do openGL, o próximo comando deve ser utilizado no console, caso tenha apertado o primeiro comando errado, pode-se digitar qualquer tecla que não esteja entre as opções que uma exceção será lançada, porém será possível continuar a manipular a curva e as demais interações.

A nossa abordagem foi a seguinte: O usuário entrará com os pontos(x,y,t)  que pertencem à curva de Bézier.  Utilizamos o método dos minimos quadrados pra aproximar esses pontos para uma função P(t)  polinomial em t de grau n, definido pelo usuário.
P(t) = a0+ a1t+a2t^2+a3^3….. ant^n
Assumimos P(t) como a mesma equação da representação de curvas de Bezier pela forma de Berinstein. Então todos os coeficientes de P(t) podem ser escritos como uma combinação linear f dos pontos de controle.
	a0 = f0(B0,B1,B2...Bn);
	a1 = f1(B0,B1,B2….Bn);
	an = fn(B0, B1, B2….Bn);
Bastou resolver esse novo sistema para encontrar os nossos Pontos de controle que governam a curva de bezier.
Com os pontos de controle em mãos, Utilizamos o algoritmo de De Castelljau para desenhar a curva, com o numero de avaliações definido pelo o usuário.
	
Como compilar:
O projeto em c++ foi criado utilizando a IDE Code:Blocks e compilado e testado utilizando o MinGw em computadores Windows.. Para seu correto funcionamento, é necessária a biblioteca Glut para o funcionamento do OpenGL. É necessãrio referenciar as bibliotecas  glu32 glut32 e opengl32.
Utilizamos a biblioteca Eigen para funções de resolução de sistemas lineares. Os arquivos da biblioteca Eigen devem estar na pasta include do projeto.
Para configuração do sistemas utilizamos o seguinte tutorial:
http://www.deannicholls.co.uk/tutorials/show/cpp_glut
O Eigen pode ser encontrado aqui:
http://eigen.tuxfamily.org/index.php?title=Main_Page
Todos os arquivos necessários acompanharão o projeto. Para o abrir, basta abrir o arquivo “PGProject_1.cbp”