#include "scene.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

void Scene::draw(bool showAxes, bool showGrid) const {
    // Grid
    if (showGrid) {
        glColor3f(0.3f, 0.3f, 0.3f);
        glBegin(GL_LINES);
        for (int i = -20; i <= 20; i++) {
            glVertex3f(i, 0, -20);
            glVertex3f(i, 0, 20);
            glVertex3f(-20, 0, i);
            glVertex3f(20, 0, i);
        }
        glEnd();
    }

    // Eixos
    if (showAxes) {
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        glColor3f(1, 0, 0); glVertex3f(0,0,0); glVertex3f(5,0,0);
        glColor3f(0, 1, 0); glVertex3f(0,0,0); glVertex3f(0,5,0);
        glColor3f(0, 0, 1); glVertex3f(0,0,0); glVertex3f(0,0,5);
        glEnd();
        glLineWidth(1.0f);
    }

    for (const auto& obj : objects) {
        obj.draw();
    }
}

void Scene::highlightObject(int index) const {
    if (index >= 0 && index < (int)objects.size())
        objects[index].drawHighlight();
}

void Scene::createCube() {
    objects.emplace_back("cube");
    objects.back().pos[0] = objects.size() * 1.5f - 3.0f;
}

void Scene::createSphere() {
    objects.emplace_back("sphere");
    objects.back().pos[0] = objects.size() * 1.5f - 3.0f;
}

void Scene::moveObject(int index, float dx, float dy, float dz) {
    if (index >= 0 && index < (int)objects.size()) {
        objects[index].pos[0] += dx;
        objects[index].pos[1] += dy;
        objects[index].pos[2] += dz;
    }
}

void Scene::rotateObject(int index, float dx, float dy) {
    if (index >= 0 && index < (int)objects.size()) {
        objects[index].rot[0] += dx;
        objects[index].rot[1] += dy;
    }
}

void Scene::scaleObject(int index, float factor) {
    if (index >= 0 && index < (int)objects.size()) {
        objects[index].scale *= factor;
    }
}

int Scene::pickObject(int mouseX, int mouseY, int width, int height, bool& hit) {
    hit = false;
    if (objects.empty()) return -1;

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLdouble nearX, nearY, nearZ;
    GLdouble farX, farY, farZ;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    float winX = (float)mouseX;
    float winY = (float)viewport[3] - (float)mouseY;   // Inverte o Y

    gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &nearX, &nearY, &nearZ);
    gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &farX, &farY, &farZ);

    int closestIndex = -1;
    float minDist = 999999.0f;

    for (int i = 0; i < (int)objects.size(); i++) {
        const auto& obj = objects[i];
        
        float dx = obj.pos[0] - nearX;
        float dy = obj.pos[1] - nearY;
        float dz = obj.pos[2] - nearZ;
        
        float dirX = farX - nearX;
        float dirY = farY - nearY;
        float dirZ = farZ - nearZ;
        
        float len = sqrtf(dirX*dirX + dirY*dirY + dirZ*dirZ);
        if (len < 0.0001f) continue;
        
        dirX /= len; dirY /= len; dirZ /= len;

        float crossX = dy * dirZ - dz * dirY;
        float crossY = dz * dirX - dx * dirZ;
        float crossZ = dx * dirY - dy * dirX;
        float dist = sqrtf(crossX*crossX + crossY*crossY + crossZ*crossZ);

        float radius = obj.scale * (obj.type == "cube" ? 0.85f : 0.75f);

        if (dist < radius * 1.8f) {
            float distToCam = sqrtf(dx*dx + dy*dy + dz*dz);
            if (distToCam < minDist) {
                minDist = distToCam;
                closestIndex = i;
            }
        }
    }

    if (closestIndex != -1) hit = true;
    return closestIndex;
}

void Scene::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (const auto& obj : objects) {
        file << obj.type << " "
             << obj.pos[0] << " " << obj.pos[1] << " " << obj.pos[2] << " "
             << obj.rot[0] << " " << obj.rot[1] << " " << obj.rot[2] << " "
             << obj.scale << " "
             << obj.color[0] << " " << obj.color[1] << " " << obj.color[2] << "\n";
    }
    std::cout << "Cena salva em " << filename << std::endl;
}

void Scene::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    objects.clear();
    std::string line, type;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Object obj("cube");
        iss >> type;
        obj.type = type;
        iss >> obj.pos[0] >> obj.pos[1] >> obj.pos[2];
        iss >> obj.rot[0] >> obj.rot[1] >> obj.rot[2];
        iss >> obj.scale;
        iss >> obj.color[0] >> obj.color[1] >> obj.color[2];
        objects.push_back(obj);
    }
    std::cout << "Cena carregada de " << filename << std::endl;
}
