#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>

/// Constantes
#define CUBO 1
#define CONE 2
#define ESFERA 3
#define TORUS 4
#define DODECAEDRO 5

///Define parametros para redimensionar a tela
GLfloat angle, fAspect;
///Guarda a primitiva escolhida para ser mostrada
GLint primitiva;
///ID para a janela principal
int mainWindow;
///Guarda as rotacoes
float rotationX, rotationY;

/// Função callback chamada para fazer o desenho
void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

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

    glFlush();
    glutSwapBuffers();
}

///Inicializa parâmetros de rendering
void InicializaMain (void)
{
    //primitiva = CUBO;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    primitiva = CUBO;
    glColor3f(0.0f, 1.0f, 0.0f);
    angle=45;
}

///Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(angle,fAspect,1,1000);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador, do alvo e da parte de cima
	gluLookAt(0,80,200, 0,0,0, 0,1,0);
}

///Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;
    // Calcula a correção de aspecto
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
    if(op == 3){
        rotationX = rotationY = 0;
        glLoadIdentity();
    }
    if(op == 4)
        exit(0);
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
    glutAddMenuEntry("Sair", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

///Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
        //Quando o botao eh clicado
         if (state == GLUT_DOWN)
            CriaMenu();
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

/// Função callback chamada para gerenciar eventos de teclas especiais do teclado
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

/// Função callback chamada para gerenciar eventos de teclas normais do teclado
void GerenciaTeclado(unsigned char button, int x, int y){
    ///Zoom in e Zoom out
    if(button == '+')
        if (angle >= 10) angle -= 5;
    if(button == '-')
        if (angle <= 130) angle += 5;

    EspecificaParametrosVisualizacao();
	glutPostRedisplay();
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
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);
	glutKeyboardFunc(GerenciaTeclado);
	glutSpecialFunc(GerenciaTecladoEspecial);
	InicializaMain();

	glutMainLoop();
}


