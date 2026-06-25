#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include <vector>
#include <string>

class Scene {
public:
    std::vector<Object> objects;

    void draw(bool showAxes, bool showGrid) const;
    void highlightObject(int index) const;

    void createCube();
    void createSphere();

    void moveObject(int index, float dx, float dy, float dz);
    void rotateObject(int index, float dx, float dy);
    void scaleObject(int index, float factor);

    // Picking atualizado
    int pickObject(int mouseX, int mouseY, int width, int height, bool& hit);

    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
};

#endif