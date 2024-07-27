#ifdef __APPLE__
#else
#include <windows.h>
#endif
#include <GL/glut.h>
#include <cmath>

// Variáveis de controle da câmera
float anguloCameraX = 0.0f;
float anguloCameraY = 0.0f;
float posicaoCameraX = 0.0f;
float posicaoCameraY = 1.0f;
float posicaoCameraZ = 5.0f;

// Variáveis para animação de dia e noite
float anguloLuz = 0.0f;
float intensidadeAmbiente = 0.5f;
float intensidadeDifusa = 1.0f;
float corCeu[3] = {0.5f, 0.8f, 1.0f};
bool ehDia = true;
int tempoAnimacao = 0;
const int DURACAO_DIA = 5000; // 5 segundos em milissegundos
const int DURACAO_NOITE = 5000; // 5 segundos em milissegundos

void definirMaterial(float r, float g, float b) {
    GLfloat corMaterial[] = {r, g, b, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, corMaterial);
    glMaterialfv(GL_FRONT, GL_AMBIENT, corMaterial);
}

void desenharCasa() {
    // Desenhar a casa
    glPushMatrix();

    // Base da casa
    glPushMatrix();
    definirMaterial(0.0f, 0.0f, 1.0f); // Azul
    glTranslatef(0.0, 0.5, 0.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Telhado da casa
    glPushMatrix();
    definirMaterial(1.0f, 0.0f, 0.0f); // Vermelho
    glTranslatef(0.0, 1.0, 0.0);
    glRotatef(45, 0.0, 1.0, 0.0);
    glScalef(1.2, 0.5, 1.2);
    glutSolidOctahedron();
    glPopMatrix();

    // Chaminé
    glPushMatrix();
    definirMaterial(0.5f, 0.5f, 0.5f); // Cinza
    glTranslatef(0.3, 1.3, 0.0);
    glScalef(0.2, 0.5, 0.2);
    glutSolidCube(1.0);
    glPopMatrix();

    // Porta da casa
    glPushMatrix();
    definirMaterial(0.6f, 0.3f, 0.1f); // Marrom
    glTranslatef(0.0, 0.2, 0.5); // Posiciona a porta na frente da casa
    glScalef(0.2, 0.4, 0.01); // Reduz a profundidade da porta
    glutSolidCube(1.0);
    glPopMatrix();

    // Janela direita
    glPushMatrix();
    definirMaterial(1.0f, 1.0f, 1.0f); // Branco
    glTranslatef(0.35, 0.4, 0.5); // Posiciona a janela na frente da casa no lado direito
    glScalef(0.1, 0.1, 0.01); // Reduz a profundidade da janela
    glutSolidCube(2.0);
    glPopMatrix();

    // Janela esquerda
    glPushMatrix();
    definirMaterial(1.0f, 1.0f, 1.0f); // Branco
    glTranslatef(-0.35, 0.4, 0.5); // Posiciona a janela na frente da casa no lado esquerdo
    glScalef(0.1, 0.1, 0.01); // Reduz a profundidade da janela
    glutSolidCube(2.0);
    glPopMatrix();

    glPopMatrix();
}

void desenharChao() {
    // Desenhar o chão
    glPushMatrix();
    definirMaterial(0.0f, 1.0f, 0.0f); // Verde
    glBegin(GL_QUADS);
    glVertex3f(-5.0, 0.0, -5.0);
    glVertex3f(-5.0, 0.0, 5.0);
    glVertex3f(5.0, 0.0, 5.0);
    glVertex3f(5.0, 0.0, -5.0);
    glEnd();
    glPopMatrix();
}

void desenharSol() {
    // Desenhar o sol
    glPushMatrix();
    definirMaterial(1.0f, 1.0f, 0.0f); // Amarelo
    glTranslatef(4.0 * cos(anguloLuz), 4.0 * sin(anguloLuz), 0.0);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}

void desenharLua() {
    // Desenhar a lua
    glPushMatrix();
    definirMaterial(0.8f, 0.8f, 1.0f); // Cor da Lua (mais sólida)
    glTranslatef(-4.0 * cos(anguloLuz), -4.0 * sin(anguloLuz), 0.0); // Lua oposta ao Sol
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}

void desenharPoste() {
    // Desenhar o poste de luz
    glPushMatrix();
    definirMaterial(0.5f, 0.5f, 0.5f); // Cinza
    glTranslatef(-2.0, 0.0, 2.0); // Posição do poste
    glScalef(0.1, 2.0, 0.1); // Escala do poste
    glutSolidCube(1.0);
    glPopMatrix();

    // Desenhar a lâmpada do poste
    glPushMatrix();
    definirMaterial(1.0f, 1.0f, 0.0f); // Amarelo (lâmpada acesa)
    glTranslatef(-2.0, 2.0, 2.0); // Posição da lâmpada no topo do poste
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();
}

void definirIluminacao() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float luzAmbiente[] = { intensidadeAmbiente, intensidadeAmbiente, intensidadeAmbiente, 1.0 };
    float luzDifusa[] = { intensidadeDifusa, intensidadeDifusa, intensidadeDifusa, 1.0 };
    float luzEspecular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);

    float posicaoLuz[] = { 4.0f * cos(anguloLuz), 4.0f * sin(anguloLuz), 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    // Configuração da luz do poste
    glEnable(GL_LIGHT1);
    float luzPosteDifusa[] = { 1.0f, 1.0f, 0.8f, 1.0f };
    float luzPostePosicao[] = { -2.0f, 2.2f, 2.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzPosteDifusa);
    glLightfv(GL_LIGHT1, GL_POSITION, luzPostePosicao);
    if (ehDia) {
        glDisable(GL_LIGHT1); // Apaga a luz do poste durante o dia
    } else {
        glEnable(GL_LIGHT1); // Acende a luz do poste durante a noite
    }
}

void atualizarIluminacao() {
    anguloLuz += 0.01f;
    if (anguloLuz >= 2 * M_PI) anguloLuz = 0.0f;

    float posicaoLuz[] = { 4.0f * cos(anguloLuz), 4.0f * sin(anguloLuz), 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    // Transição gradual entre dia e noite
    float anguloLuzNormalizado = (sin(anguloLuz) + 1.0f) / 2.0f;
    intensidadeAmbiente = 0.2f + 0.3f * anguloLuzNormalizado;
    intensidadeDifusa = 0.2f + 0.8f * anguloLuzNormalizado;
    corCeu[0] = 0.0f + 0.5f * anguloLuzNormalizado;
    corCeu[1] = 0.0f + 0.8f * anguloLuzNormalizado;
    corCeu[2] = 0.2f + 0.8f * anguloLuzNormalizado;

    glClearColor(corCeu[0], corCeu[1], corCeu[2], 1.0);
}

void mostrar() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(posicaoCameraX, posicaoCameraY, posicaoCameraZ, 0.0, 0.5, 0.0, 0.0, 1.0, 0.0);

    definirIluminacao();
    atualizarIluminacao();

    desenharChao();
    desenharCasa();

    desenharSol(); // Desenhar o sol sempre
    desenharLua(); // Desenhar a lua sempre
    desenharPoste(); // Desenhar o poste de luz

    glutSwapBuffers();
}

void alterarTamanho(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void teclasEspeciais(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            anguloCameraY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            anguloCameraY += 5.0f;
            break;
        case GLUT_KEY_UP:
            anguloCameraX -= 5.0f;
            break;
        case GLUT_KEY_DOWN:
            anguloCameraX += 5.0f;
            break;
    }

    posicaoCameraX = 5.0f * sin(anguloCameraY * M_PI / 180.0f);
    posicaoCameraZ = 5.0f * cos(anguloCameraY * M_PI / 180.0f);
    posicaoCameraY = 5.0f * sin(anguloCameraX * M_PI / 180.0f);

    glutPostRedisplay();
}

void teclasNormais(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            posicaoCameraY += 0.1f;
            break;
        case 's':
            posicaoCameraY -= 0.1f;
            break;
        case 'a':
            posicaoCameraX -= 0.1f;
            break;
        case 'd':
            posicaoCameraX += 0.1f;
            break;
        case 'q':
            posicaoCameraZ -= 0.1f;
            break;
        case 'e':
            posicaoCameraZ += 0.1f;
            break;
    }
    glutPostRedisplay();
}

void temporizador(int valor) {
    tempoAnimacao += 33;
    if (tempoAnimacao >= DURACAO_DIA + DURACAO_NOITE) {
        tempoAnimacao = 0;
        ehDia = !ehDia;
    }

    glutPostRedisplay();
    glutTimerFunc(33, temporizador, 0);
}

void inicializar() {
    glEnable(GL_DEPTH_TEST);
    definirIluminacao();
    glClearColor(corCeu[0], corCeu[1], corCeu[2], 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cena 3D com Casa, Chaminé, Poste de Luz e Animação Dia e Noite");

    inicializar();
    glutDisplayFunc(mostrar);
    glutReshapeFunc(alterarTamanho);
    glutSpecialFunc(teclasEspeciais);
    glutKeyboardFunc(teclasNormais);
    glutTimerFunc(33, temporizador, 0); // Inicia o temporizador para animação

    glutMainLoop();
    return 0;
}
