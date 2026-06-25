#include "object.h"

Object::Object(std::string t) : type(t) {}

void Object::draw() const {
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(rot[0], 1, 0, 0);
    glRotatef(rot[1], 0, 1, 0);
    glRotatef(rot[2], 0, 0, 1);
    glScalef(scale, scale, scale);

    glColor3fv(color);

    if (type == "cube") {
        glutSolidCube(1.0);
    } else if (type == "sphere") {
        glutSolidSphere(0.7, 32, 32);
    }

    glPopMatrix();
}

void Object::drawHighlight() const {
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glRotatef(rot[0], 1, 0, 0);
    glRotatef(rot[1], 0, 1, 0);
    glRotatef(rot[2], 0, 0, 1);
    glScalef(scale * 1.05f, scale * 1.05f, scale * 1.05f);

    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(3.0f);

    if (type == "cube") {
        glutWireCube(1.0);
    } else if (type == "sphere") {
        glutWireSphere(0.7, 32, 32);
    }

    glPopMatrix();
}
