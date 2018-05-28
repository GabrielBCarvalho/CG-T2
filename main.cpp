#include <GL/glut.h>
#include <stdlib.h>
#include <cstdlib> 
#include <iostream>
#include <string>

using namespace std;

/// Constantes
#define CUBO 1
#define CONE 2
#define ESFERA 3
#define TORUS 4
#define DODECAEDRO 5

enum State {scanAngle, scanX, scanY, scanZ, waitS};
enum Action {waitA, translate, rotate, scale};

std::string toPrint = "";
std::string input = "";

State state = waitS;
Action action = waitA;

///Define parametros para redimensionar a tela
GLfloat angle, fAspect;
///Guarda a primitiva escolhida para ser mostrada
GLint primitiva;
///ID para a janela principal
int mainWindow;
///Guarda as rotacoes
float rotationX, rotationY, rotationZ;
float translateX, translateY, translateZ;
float scaleX, scaleY, scaleZ;

float x, y, z, ang;

int lineHight = 25; // the hight of each line
int lineMargin = 10; // the left margin for your text
int currentHight = 25; // the y position of the cursor.

/// Fun��o callback chamada para fazer o desenho
void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT);


    if(state == waitS){
        switch(action){
        case translate:
            translateX += x;
            translateY += y;
            translateZ += z;
            glTranslatef(translateX, translateY, translateZ);
            action = waitA;
            break;
        case scale:
            scaleX += x;
            scaleY += y;
            scaleZ += z;
            glScalef(scaleX, scaleY, scaleZ);
            action = waitA;
            break;
        case rotate:
            rotationX += x;
            rotationY += y;
            rotationZ += z;
            glRotatef(ang, rotationX, rotationY, rotationZ);
            action = waitA;
            break;
        }
    }
    

    //glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    //glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    //glRotatef

    switch (primitiva)
    {
    case CUBO:
        glutWireCube(50.0f);
        break;
    case CONE:
        glutWireCone(50.0f, 50.0f,25,25);
        break;
    case ESFERA:
        glutWireSphere(50.0f, 25, 25);
        break;
    case TORUS:
        glutWireTorus(15.0f, 35.0f, 25, 25);
        break;
    ///ESTA COMENTADO POR QUE FICA MUITO PEQUENO, MAS SE TIRAR EH SO DAR ZOOM('+') AO MAXIMO
    //case DODECAEDRO:
    //    glutWireDodecahedron();
    //    break;
    }

    // Printar string
    glRasterPos2i(-50, -50);
    void * font = GLUT_BITMAP_9_BY_15;
    for (std::string::iterator i = toPrint.begin(); i != toPrint.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }

    glFlush();
    glutSwapBuffers();
}

///Inicializa par�metros de rendering
void InicializaMain (void)
{
    //primitiva = CUBO;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    primitiva = CUBO;
    glColor3f(0.0f, 1.0f, 0.0f);
    angle=45;
}

///Fun��o usada para especificar o volume de visualiza��o
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de proje��o
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de proje��o
	glLoadIdentity();

	// Especifica a proje��o perspectiva
	gluPerspective(angle,fAspect,1,1000);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posi��o do observador, do alvo e da parte de cima
	gluLookAt(0,80,200, 0,0,0, 0,1,0);
}

///Fun��o callback chamada quando o tamanho da janela � alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divis�o por zero
	if ( h == 0 ) h = 1;
    // Calcula a corre��o de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;
	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);
	EspecificaParametrosVisualizacao();
}

///Menu de escolha da cor
void MenuCor(int op)
{
   switch(op) {
            case 0:
                glColor3f(1.0f, 0.0f, 0.0f);
                     break;
            case 1:
                glColor3f(0.0f, 1.0f, 0.0f);
                     break;
            case 2:
                glColor3f(0.0f, 0.0f, 1.0f);
                     break;
            case 3:
                     exit(0);
                     break;
    }
    glutPostRedisplay();
}

///Menu de escolha da primitiva
void MenuPrimitiva(int op)
{
   switch(op) {
            case 0:
                     primitiva = CUBO;
                     break;
            case 1:
                     primitiva = CONE;
                     break;
            case 2:
                     primitiva = ESFERA;
                     break;
            case 3:
                     primitiva = TORUS;
                     break;
            case 4:
                     primitiva = DODECAEDRO;
                     break;
    }
    glutPostRedisplay();
}

///Container para todo o menu(tambem guarda a funcao de sair do programa)
void MenuPrincipal(int op)
{
    switch(op){
        case 3:
            rotationX = rotationY = rotationZ = 0;
            translateX = translateY = translateZ = 0;
            scaleX = scaleY = scaleZ = 1;
            glLoadIdentity();
            break;
        case 4: //transladar
            state = scanX;
            toPrint = "Insira o valor de X: ";
            action = translate;
            break;
        case 5: //escalar
            state = scanX;
            toPrint = "Insira o valor de X: ";
            action = scale;
            break;
        case 6: //rotacionar
            state = scanAngle;
            toPrint = "Insira o valor do �ngulo: ";
            action = rotate;
            break;
        case 7:
            exit(0);
            break;
    }
}

///Criacao do Menu
void CriaMenu()
{
    int menu,submenu1,submenu2;

    submenu1 = glutCreateMenu(MenuPrimitiva);
    glutAddMenuEntry("Cubo",0);
    glutAddMenuEntry("Cone",1);
    glutAddMenuEntry("Esfera",2);
    glutAddMenuEntry("Torus",3);
    glutAddMenuEntry("Dodecaedro",4);

    submenu2 = glutCreateMenu(MenuCor);
    glutAddMenuEntry("Vermelho",0);
    glutAddMenuEntry("Verde",1);
    glutAddMenuEntry("Azul",2);


    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Primitivas",submenu1);
    glutAddSubMenu("Cor",submenu2);
    glutAddMenuEntry("Reiniciar", 3); ///Reinicia o solido para a posicao inicial
    glutAddMenuEntry("Transladar", 4);
    glutAddMenuEntry("Escalar", 5);
    glutAddMenuEntry("Rotacionar", 6);
    glutAddMenuEntry("Sair", 7);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

///Fun��o callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
        //Quando o botao eh clicado
         if (state == GLUT_DOWN)
            CriaMenu();
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

/// Fun��o callback chamada para gerenciar eventos de teclas especiais do teclado
void GerenciaTecladoEspecial(int button, int x, int y)
{
	if (button == GLUT_KEY_UP) // Zoom-in
			rotationX = 10.0f;
	if (button == GLUT_KEY_DOWN) // Zoom-out
			rotationX = -10.0f;
    if (button == GLUT_KEY_RIGHT)
            rotationY = 10.0f;
    if (button == GLUT_KEY_LEFT)
            rotationY = -10.0f;
    Desenha();
}


/// Fun��o callback chamada para gerenciar eventos de teclas normais do teclado
void GerenciaTeclado(unsigned char key, int x, int y){
    ///Zoom in e Zoom out
    if(key == '+'){
        if (angle >= 10) angle -= 5;
    }
    else if(key == '-'){
        if (angle <= 130) angle += 5;
    }
    else if(key == 13){ // Enter
        if(state == scanAngle){
            ang = std::stof(input);
            toPrint = "Insira o valor de X: ";
            state = scanX;
        }
        else if (state == scanX){
            x = std::stof(input);
            toPrint = "Insira o valor de Y: ";
            state = scanY;
        }
        else if (state == scanY){
            y = std::stof(input);
            toPrint = "Insira o valor de Z: ";
            state = scanZ;
        }
        else if (state == scanZ){
            z = std::stof(input);
            state = waitS;
            //action = waitA;
            toPrint = "";
        }
        input = "";
    }  
    else{
        if(state != waitS){
            input += key;
            toPrint += key;
            //toPrint.append(input);
        }
    }
    
    //glColor3f(0.0, 1.0, 0.0); // text color
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, key); // print the color

    //EspecificaParametrosVisualizacao();
	glutPostRedisplay();
    //Desenha();
}

/// Programa Principal
int main(int argc, char **argv)
{
    ///Janela principal
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800,600);
	mainWindow = glutCreateWindow("Visualizacao 3D");
	///Callbacks
	glutDisplayFunc(Desenha);

    //glutKeyboardFunc(myKey); // register the key handler.

	
    glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);
	glutKeyboardFunc(GerenciaTeclado);
    //glRasterPos2f(lineMargin, currentHight); // set the cursor to the initial position.
	glutSpecialFunc(GerenciaTecladoEspecial);
	InicializaMain();

	glutMainLoop();
}


