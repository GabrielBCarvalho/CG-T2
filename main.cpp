///Bibliotecas openGL para sistemas especificos
#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#elif __linux__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

///Bibliotecas padr�o
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

///Constantes que definem primitivas
#define CUBO 1
#define CONE 2
#define ESFERA 3
#define TORUS 4

///Definem estados para o codigo, bem como escolhas do usuario
enum State {scanAngle, scanX, scanY, scanZ, print, waitS};
enum Action
{
    waitA, translate, rotate, scale,
    mirrorX, mirrorY, mirrorZ, zoomRotate
};
enum Projection {paralela, perspectiva};

///Variaveis globais
std::string toPrint = "";
std::string input = "";

//variaveis de escolha
State state;
Action action;
Projection projection;

GLfloat angle, ang, fAspect;//Define parametros para redimensionar a tela
GLint primitiva, width, heigth;//Guarda a primitiva escolhida para ser mostrada


int mainWindow;//ID para a janela principal
//Guarda as rotacoes, transl e scale.
float rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
float translateX = 0.0f, translateY = 0.0f, translateZ = 0.0f;
float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;

float xRotate = 0.0f, yRotate = 0.0f;
float a, b, z, angAux;



/// Inicializa a luz
void initLighting() {
    // Informa que ir� utilizar ilumina��o
    glEnable(GL_LIGHTING);
    // Liga a luz0
    glEnable(GL_LIGHT0);
    // Informa que ir� utilizar as cores do material
    glEnable(GL_COLOR_MATERIAL);
}

/// Define a posi��o da luz 0
void setLight() {
    GLfloat light_position[4] = {10.0, 10.0, -20.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

// Fun��o utilizada para definir as propriedades do material
void setMaterial(int currentMaterial){
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat no_shininess[] = { 0.0 };
    GLfloat low_shininess[] = { 5.0 };
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

    switch (currentMaterial){
    case 0:
        // Diffuse reflection only; no ambient or specular
        glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
        break;

    case 1:
        // Diffuse and specular reflection; low shininess; no ambient
        glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
        break;

    case 2:
        // Diffuse and specular reflection; high shininess; no ambient
        glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
        break;

    case 3:
        // Diffuse refl.; emission; no ambient or specular reflection
        glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        break;
    }
}

// Fun��o para definir o tipo de tonaliza��o
void setShading(int sType) {
    switch(sType) {
        case 0 :
            glShadeModel(GL_SMOOTH);
            break;
        case 1 :
            glShadeModel(GL_FLAT);
            break;
    }
}



/// Fun��o callback chamada para fazer o desenho
void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ///Escolha de transformacao
    if(state == print)
    {
        switch(action)
        {
        case translate:
            translateX = a;
            translateY = b;
            translateZ = z;
            glTranslatef(translateX, translateY, translateZ);
            break;
        case scale:
            scaleX = a;
            scaleY = b;
            scaleZ = z;
            glScalef(scaleX, scaleY, scaleZ);
            break;
        case rotate:
            angle += angAux;
            rotationX = a;
            rotationY = b;
            rotationZ = z;
            glRotatef(angle, rotationX, rotationY, rotationZ);
            break;
        case mirrorX:
            glScalef(-scaleX, scaleY, scaleZ);
            break;
        case mirrorY:
            glScalef(scaleX, -scaleY, scaleZ);
            break;
        case mirrorZ:
            glScalef(scaleX, scaleY, -scaleZ);
            break;
        case zoomRotate:
            glRotatef(xRotate, 1.0f, 0.0f, 0.0f);
            glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
            break;
        }
        state = waitS;
        action = waitA;
    }

    glMatrixMode(GL_MODELVIEW);
    setLight();
    setShading(0);


    ///Escolha de primitiva
    switch (primitiva)
    {
        setMaterial(3);
    case CUBO:
        glutSolidCube(50.0f);
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
    }

    ///Escolha de projecao
    switch (projection)
    {
    case perspectiva:
        // Especifica sistema de coordenadas de proje��o
        glMatrixMode(GL_PROJECTION);
        // Inicializa sistema de coordenadas de proje��o
        glLoadIdentity();
        // Especifica a proje��o perspectiva
        gluPerspective(ang,fAspect,1,1000);
        // Especifica sistema de coordenadas do modelo
        glMatrixMode(GL_MODELVIEW);
        // Inicializa sistema de coordenadas do modelo
        glLoadIdentity();
        // Especifica posi��o do observador, do alvo e da parte de cima
        gluLookAt(0,80,200, 0,0,0, 0,1,0);
        break;

    case paralela:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-width, width, -heigth,heigth, 1, 1000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,80,200, 0,0,0, 0,1,0);
        break;
    }

    /// Printa a string
    glRasterPos2i(-50, -50);
    void * font = GLUT_BITMAP_9_BY_15;
    for (std::string::iterator i = toPrint.begin(); i != toPrint.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }

    ///Execucao e troca de buffer
    glFlush();
    glutSwapBuffers();
}

///Inicializa par�metros de rendering e como a tela inicial sera apresentada
void InicializaMain (void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    primitiva = CUBO;
    state = waitS;
    action = waitA;
    projection = paralela;
    angle = 45;
    glColor3f(0.0f, 1.0f, 0.0f);
    ang=45;
}

///Fun��o callback chamada quando o tamanho da janela � alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{

    // Para previnir uma divis�o por zero
    if ( h == 0 )
        h = 1;
    // Calcula a corre��o de aspecto
    fAspect = (GLfloat)w/(GLfloat)h;
    // Especifica o tamanho da viewport
    glViewport(0, 0, w, h);
    width = w/5;
    heigth = h/5;
}

///Container para todo o menu(tambem guarda a funcao de sair do programa)
void MenuPrincipal(int op)
{
    switch(op)
    {
    case 3:
        rotationX = rotationY = rotationZ = 0.0f;
        translateX = translateY = translateZ = 0.0f;
        scaleX = scaleY = scaleZ = 1.0f;
        xRotate = yRotate = 0.0f;
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
        toPrint = "Insira o valor do angulo: ";
        action = rotate;
        break;
    case 7:
        exit(0);
        break;
    }
    Desenha();
}

///Menu de escolha da cor
void MenuCor(int op)
{
    switch(op)
    {
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

///Submenu de escolha da primitiva
void MenuPrimitiva(int op)
{
    switch(op)
    {
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
    }
    glutPostRedisplay();
}

///Submenu de espelhamento
void MenuMirror(int op)
{
    if(op == 0)
        action = mirrorX;
    else if(op == 1)
        action = mirrorY;
    else if(op == 2)
        action = mirrorZ;
    state = print;
    Desenha();
}

///Submenu de projecao
void MenuProjecao(int op)
{
    if(op == 0)
        projection = paralela;
    else if(op == 1)
        projection = perspectiva;
    Desenha();
}

///Criacao do Menu
void CriaMenu()
{
    int submenu1,submenu2, submenu3, submenu4;

    ///Submenu para primitivas
    submenu1 = glutCreateMenu(MenuPrimitiva);
    glutAddMenuEntry("Cubo",0);
    glutAddMenuEntry("Cone",1);
    glutAddMenuEntry("Esfera",2);
    glutAddMenuEntry("Torus",3);

    ///Submenu para cores
    submenu2 = glutCreateMenu(MenuCor);
    glutAddMenuEntry("Vermelho",0);
    glutAddMenuEntry("Verde",1);
    glutAddMenuEntry("Azul",2);

    ///Submenu para espelhamento
    submenu3 = glutCreateMenu(MenuMirror);
    glutAddMenuEntry("Eixo X",0);
    glutAddMenuEntry("Eixo Y",1);
    glutAddMenuEntry("Eixo Z",2);

    ///Submenu para projecao
    submenu4 = glutCreateMenu(MenuProjecao);
    glutAddMenuEntry("Paralela",0);
    glutAddMenuEntry("Perspectiva",1);

    ///Criacao do menu, uniao dos submenus
    glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Primitivas",submenu1);
    glutAddSubMenu("Cor",submenu2);
    glutAddMenuEntry("Reiniciar", 3); ///Reinicia o solido para a posicao inicial
    glutAddMenuEntry("Transladar", 4);
    glutAddMenuEntry("Escalar", 5);
    glutAddMenuEntry("Rotacionar", 6);
    glutAddSubMenu("Espelhar em relacao a eixo", submenu3);
    glutAddSubMenu("Projecoes", submenu4);
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
    Desenha();
}

/// Fun��o callback chamada para gerenciar eventos de teclas especiais do teclado
void GerenciaTecladoEspecial(int button, int x, int y)
{
    //Rotacoes pelo teclado
    if (button == GLUT_KEY_UP)
    {
        xRotate += 10.0f;
        state = print;
        action = zoomRotate;
    }
    if (button == GLUT_KEY_DOWN)
    {
        xRotate += -10.0f;
        state = print;
        action = zoomRotate;
    }
    if (button == GLUT_KEY_RIGHT)
    {
        yRotate += 10.0f;
        state = print;
        action = zoomRotate;
    }
    if (button == GLUT_KEY_LEFT)
    {
        yRotate += -10.0f;
        state = print;
        action = zoomRotate;
    }
    Desenha();
}

/// Fun��o callback chamada para gerenciar eventos de teclas normais do teclado
void GerenciaTeclado(unsigned char key, int x, int y)
{
    if(key == 13)  // Enter
    {
        if(state == scanAngle)  // Leu o �ngulo
        {
            angAux = std::stof(input);
            toPrint = "Insira o valor de X: ";
            state = scanX;
        }
        else if (state == scanX)    // Leu o X
        {
            a = std::stof(input);
            toPrint = "Insira o valor de Y: ";
            state = scanY;
        }
        else if (state == scanY)    // Leu o Y
        {
            b = std::stof(input);
            toPrint = "Insira o valor de Z: ";
            state = scanZ;
        }
        else if (state == scanZ)    // Leu o Z
        {
            z = std::stof(input);
            state = print;
            toPrint = "";
        }
        input = "";
    }
    else if(key == 8 && (input.size() > 0))
    {
        toPrint = toPrint.substr(0, toPrint.size()-1);
        input = input.substr(0, input.size()-1);
    }
    else
    {
        if(state != waitS)
        {
            input += key;
            toPrint += key;
        }
    }

    Desenha();
}

/// Programa Principal
int main(int argc, char **argv)
{
    ///Janela principal
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,400);
    mainWindow = glutCreateWindow("Visualizacao 3D");
    glEnable(GL_DEPTH_TEST);
    initLighting();

    ///Callbacks
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(GerenciaTeclado);
    glutSpecialFunc(GerenciaTecladoEspecial);
    InicializaMain();

    glutMainLoop();
}


