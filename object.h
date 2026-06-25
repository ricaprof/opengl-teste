#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glut.h>
#include <string>

class Object {
public:
    float pos[3] = {0.0f, 0.0f, 0.0f};
    float rot[3] = {0.0f, 0.0f, 0.0f};
    float scale = 1.0f;
    float color[3] = {0.8f, 0.8f, 0.8f};
    std::string type;  // "cube" ou "sphere"

    Object(std::string t = "cube");

    void draw() const;
    void drawHighlight() const;
};

#endif
