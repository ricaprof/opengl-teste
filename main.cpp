#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "scene.h"
#include "camera.h"

Scene scene;
Camera camera;

bool lightingEnabled = true;
bool showAxes = true;
bool showGrid = true;
bool orthoProjection = false;
int selectedObject = -1;

int windowWidth = 800, windowHeight = 600;
int lastMouseX = 0, lastMouseY = 0;
bool leftButtonDown = false;
bool rightButtonDown = false;
// Menu
int mainMenu, objectMenu, lightMenu, projMenu, colorMenu;
// Menus de Ajuda
int helpMenu, helpMoveMenu, helpTransformMenu, helpColorMenu, helpSystemMenu;

// Controle de posição
int selectedAxis = 0; // 0 = Nenhum, 1 = X, 2 = Y, 3 = Z

void updateTitle() {
    std::string title = "Editor 3D OpenGL";
    if (selectedObject >= 0) {
        title += " | Obj " + std::to_string(selectedObject);
    }
    glutSetWindowTitle(title.c_str());
}

void drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);

    // Linha 1
    std::string info = "Objetos: " + std::to_string(scene.objects.size()) +
                       " | Selecionado: " + (selectedObject >= 0 ? std::to_string(selectedObject) : "Nenhum");
    glRasterPos2f(10, windowHeight - 20);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)info.c_str());

    // Linha 2 - Projeção e Iluminação
    // Linha 2 - Projeção e Iluminação
    // \xE7 = ç | \xE3 = ã
    info = "Proje\xE7\xE3o: " + std::string(orthoProjection ? "ORT" : "PERS") +
           " | Ilumina\xE7\xE3o: " + std::string(lightingEnabled ? "ON" : "OFF");
    glRasterPos2f(10, windowHeight - 40);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)info.c_str());

    if (selectedObject >= 0) {
        auto& obj = scene.objects[selectedObject];
        
      // Posição, Rotação e Escala atuais
        glRasterPos2f(10, windowHeight - 65);
        char posStr[200]; 
        sprintf(posStr, "Pos: X=%.1f Y=%.1f Z=%.1f | Rot: X=%.0f Y=%.0f Z=%.0f | Esc: %.2fx | Eixo: %s", 
                obj.pos[0], obj.pos[1], obj.pos[2],
                obj.rot[0], obj.rot[1], obj.rot[2],
                obj.scale,
                selectedAxis == 1 ? "X" : (selectedAxis == 2 ? "Y" : (selectedAxis == 3 ? "Z" : "Nenhum")));
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)posStr);
        // Cor
        glRasterPos2f(10, windowHeight - 85);
        sprintf(posStr, "Cor: R=%.1f G=%.1f B=%.1f", obj.color[0], obj.color[1], obj.color[2]);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)posStr);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera.apply();

    if (lightingEnabled) glEnable(GL_LIGHTING);
    else glDisable(GL_LIGHTING);

    scene.draw(showAxes, showGrid);

    // Highlight do objeto selecionado (amarelo forte)
    if (selectedObject >= 0) {
        scene.highlightObject(selectedObject);
    }

    drawHUD();
    glutSwapBuffers();
}

// ==================== CONTROLE DE POSIÇÃO ====================
void moveSelectedObject(float dx, float dy, float dz) {
    if (selectedObject >= 0) {
        if (selectedAxis == 1) scene.moveObject(selectedObject, dx, 0, 0);
        else if (selectedAxis == 2) scene.moveObject(selectedObject, 0, dy, 0);
        else if (selectedAxis == 3) scene.moveObject(selectedObject, 0, 0, dz);
        else scene.moveObject(selectedObject, dx, dy, dz); // movimento livre
    }
}

void rotateSelectedObject(float angle) {
    if (selectedObject >= 0) {
        auto& obj = scene.objects[selectedObject];
        if (selectedAxis == 1) obj.rot[0] += angle;
        else if (selectedAxis == 2) obj.rot[1] += angle;
        else if (selectedAxis == 3) obj.rot[2] += angle;
        else {
            // Se o movimento for livre (0), roda um pouco em todos para dar feedback
            obj.rot[0] += angle;
            obj.rot[1] += angle;
            obj.rot[2] += angle;
        }
        
        // Mantém os ângulos entre 0 e 360 para não ter números gigantes
        if (obj.rot[0] >= 360.0f) obj.rot[0] -= 360.0f; else if (obj.rot[0] < 0.0f) obj.rot[0] += 360.0f;
        if (obj.rot[1] >= 360.0f) obj.rot[1] -= 360.0f; else if (obj.rot[1] < 0.0f) obj.rot[1] += 360.0f;
        if (obj.rot[2] >= 360.0f) obj.rot[2] -= 360.0f; else if (obj.rot[2] < 0.0f) obj.rot[2] += 360.0f;
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (selectedObject >= 0) {
        auto& obj = scene.objects[selectedObject];

        switch (key) {
            // Seleção de Eixo
            case 'x': case 'X': selectedAxis = 1; break;
            case 'y': case 'Y': selectedAxis = 2; break;
            case 'z': case 'Z': selectedAxis = 3; break;
            case '0': selectedAxis = 0; break; // movimento livre

            // Movimento fino
            case '+': case '=': moveSelectedObject(0.2f, 0.2f, 0.2f); break;
            case '-': case '_': moveSelectedObject(-0.2f, -0.2f, -0.2f); break;

            // Cor
            case 'r': case 'R': obj.color[0] = std::min(1.0f, obj.color[0] + 0.1f); break;
            case 'e': case 'E': obj.color[0] = std::max(0.0f, obj.color[0] - 0.1f); break;
            case 'g': case 'G': obj.color[1] = std::min(1.0f, obj.color[1] + 0.1f); break;
            case 'f': case 'F': obj.color[1] = std::max(0.0f, obj.color[1] - 0.1f); break;
            case 'b': case 'B': obj.color[2] = std::min(1.0f, obj.color[2] + 0.1f); break;
            case 'v': case 'V': obj.color[2] = std::max(0.0f, obj.color[2] - 0.1f); break;

            // Escala (Tamanho)
            case ']': scene.scaleObject(selectedObject, 1.1f); break; // Aumenta 10%
            case '[': scene.scaleObject(selectedObject, 0.9f); break; // Diminui 10%

            // Rotação (Orientação)
            case '.': case '>': rotateSelectedObject(5.0f); break;  // Roda +5 graus
            case ',': case '<': rotateSelectedObject(-5.0f); break; // Roda -5 graus

            case 'c': case 'C': scene.createCube(); selectedObject = scene.objects.size() - 1; break;
            case 's': case 'S': scene.createSphere(); selectedObject = scene.objects.size() - 1; break;
        }
    }

    switch (key) {
        case 'l': case 'L': lightingEnabled = !lightingEnabled; break;
        case 'a': case 'A': showAxes = !showAxes; break;
        case 'g': case 'G': showGrid = !showGrid; break;
        case 'p': case 'P': orthoProjection = !orthoProjection; camera.setProjection(orthoProjection, windowWidth, windowHeight); break;
        case 'r': scene.saveToFile("scene.txt"); break;
        case 'o': scene.loadFromFile("scene.txt"); break;
        case 27: exit(0); break;
    }
    updateTitle();
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    float step = 0.5f;
    if (selectedObject >= 0) {
        switch (key) {
            case GLUT_KEY_LEFT:  moveSelectedObject(-step, 0, 0); break;
            case GLUT_KEY_RIGHT: moveSelectedObject( step, 0, 0); break;
            case GLUT_KEY_UP:    moveSelectedObject(0, step, 0); break;
            case GLUT_KEY_DOWN:  moveSelectedObject(0, -step, 0); break;
            case GLUT_KEY_PAGE_UP:   moveSelectedObject(0, 0, step); break;
            case GLUT_KEY_PAGE_DOWN: moveSelectedObject(0, 0, -step); break;
            case GLUT_KEY_HOME: // Centralizar objeto
                if (selectedObject >= 0) {
                    scene.objects[selectedObject].pos[0] = 0;
                    scene.objects[selectedObject].pos[1] = 0;
                    scene.objects[selectedObject].pos[2] = 0;
                }
                break;
        }
    }
    glutPostRedisplay();
}

bool hoveredObject = false;

void mouse(int button, int state, int x, int y) {
    lastMouseX = x; lastMouseY = y;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        selectedObject = scene.pickObject(x, y, windowWidth, windowHeight, hoveredObject);
        updateTitle();
    }
    leftButtonDown = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);
    rightButtonDown = (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN);
}

void motion(int x, int y) {
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    if (leftButtonDown) camera.rotate(dx * 0.5f, dy * 0.5f);
    if (rightButtonDown && selectedObject >= 0) {
        scene.rotateObject(selectedObject, dy * 1.5f, dx * 1.5f);
    }

    lastMouseX = x; lastMouseY = y;
    glutPostRedisplay();
}

void menu(int value) {
    if (selectedObject >= 0) {
        auto& obj = scene.objects[selectedObject];
        if (value >= 10 && value <= 12) { // Cores pré-definidas
            if (value == 10) { obj.color[0]=1; obj.color[1]=0; obj.color[2]=0; } // Vermelho
            if (value == 11) { obj.color[0]=0; obj.color[1]=1; obj.color[2]=0; } // Verde
            if (value == 12) { obj.color[0]=0; obj.color[1]=0; obj.color[2]=1; } // Azul
        }
    }

    switch (value) {
        case 1: scene.createCube(); selectedObject = scene.objects.size()-1; break;
        case 2: scene.createSphere(); selectedObject = scene.objects.size()-1; break;
        case 3: lightingEnabled = !lightingEnabled; break;
        case 4: orthoProjection = !orthoProjection; camera.setProjection(orthoProjection, windowWidth, windowHeight); break;
        case 5: showAxes = !showAxes; break;
        case 6: showGrid = !showGrid; break;
        case 7: scene.saveToFile("scene.txt"); break;
        case 8: scene.loadFromFile("scene.txt"); break;
    }
    updateTitle();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Editor 3D OpenGL");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.12f, 0.12f, 0.15f, 1.0f);

    GLfloat lightPos[] = {8.0f, 8.0f, 8.0f, 1.0f};
    GLfloat lightAmb[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDif[] = {0.9f, 0.9f, 0.9f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);

   // Menus de Ferramentas
    objectMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Criar Cubo", 1);
    glutAddMenuEntry("Criar Esfera", 2);

    colorMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Vermelho", 10);
    glutAddMenuEntry("Verde", 11);
    glutAddMenuEntry("Azul", 12);

    // ================= MENUS DE AJUDA =================
    helpMoveMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Setas: Mover no plano X/Y", 0);
    glutAddMenuEntry("PageUp/Down: Mover no eixo Z", 0);
    glutAddMenuEntry("X, Y, Z: Travar movimento num eixo", 0);
    glutAddMenuEntry("0: Movimento livre (destravar)", 0);
    glutAddMenuEntry("+ / - : Movimento fino", 0);
    glutAddMenuEntry("Home: Centralizar objeto", 0);

    helpTransformMenu = glutCreateMenu(menu);
    glutAddMenuEntry("] / [ : Aumentar / Diminuir tamanho", 0);
    glutAddMenuEntry(". / , : Rodar no eixo selecionado", 0);

    helpColorMenu = glutCreateMenu(menu);
    glutAddMenuEntry("R / E : + / - canal Vermelho", 0);
    glutAddMenuEntry("G / F : + / - canal Verde", 0);
    glutAddMenuEntry("B / V : + / - canal Azul", 0);

    helpSystemMenu = glutCreateMenu(menu);
    glutAddMenuEntry("C: Novo Cubo | S: Nova Esfera", 0);
    glutAddMenuEntry("L: Ligar / Desligar Ilumina\xE7\xE3o", 0); // \xE7\xE3 = çã
    glutAddMenuEntry("P: Alternar Proje\xE7\xE3o (Pers / Ort)", 0);
    glutAddMenuEntry("A: Mostrar Eixos | G: Mostrar Grelha", 0);
    glutAddMenuEntry("K: Guardar Cena  | O: Carregar Cena", 0);
    glutAddMenuEntry("Esc: Fechar programa", 0);

    // Menu agrupador de ajuda
    helpMenu = glutCreateMenu(menu);
    glutAddSubMenu("Teclas de Movimento", helpMoveMenu);
    glutAddSubMenu("Tamanho e Rota\xE7\xE3o", helpTransformMenu);
    glutAddSubMenu("Cores (Canais RGB)", helpColorMenu);
    glutAddSubMenu("Sistema e Visuais", helpSystemMenu);
    // ==================================================

    // Menu Principal
    mainMenu = glutCreateMenu(menu);
    // Adicionamos a Ajuda logo no topo para ser fácil de encontrar
    glutAddSubMenu("[?] AJUDA E TECLAS", helpMenu); 
    glutAddSubMenu("Objetos", objectMenu);
    glutAddSubMenu("Cor (selecionado)", colorMenu);
    glutAddMenuEntry("Alternar Ilumina\xE7\xE3o", 3);
    glutAddMenuEntry("Alternar Proje\xE7\xE3o", 4);
    glutAddMenuEntry("Alternar Eixos", 5);
    glutAddMenuEntry("Alternar Grid", 6);
    glutAddMenuEntry("Salvar Cena", 7);
    glutAddMenuEntry("Carregar Cena", 8);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    
    // CORREÇÃO: Adicionado glViewport para mapear o raio e o clique de forma precisa
    glutReshapeFunc([](int w, int h){
        windowWidth = w; windowHeight = h;
        if (h == 0) h = 1;
        glViewport(0, 0, w, h); 
        camera.setProjection(orthoProjection, w, h);
    });
    
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    scene.createCube();
    updateTitle();

    glutMainLoop();
    return 0;
}