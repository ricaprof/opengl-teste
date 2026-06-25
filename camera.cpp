#include "camera.h"
#include <GL/glut.h>
#include <cmath>

void Camera::apply() {
    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              up[0], up[1], up[2]);
}

void Camera::rotate(float dx, float dy) {
    angleY += dx;
    angleX += dy;

    if (angleX > 89.0f) angleX = 89.0f;
    if (angleX < -89.0f) angleX = -89.0f;

    float radius = 10.0f;
    eye[0] = center[0] + radius * sin(angleY * M_PI / 180.0f) * cos(angleX * M_PI / 180.0f);
    eye[1] = center[1] + radius * sin(angleX * M_PI / 180.0f);
    eye[2] = center[2] + radius * cos(angleY * M_PI / 180.0f) * cos(angleX * M_PI / 180.0f);
}

void Camera::setProjection(bool ortho, int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (ortho) {
        float aspect = (float)width / height;
        glOrtho(-5 * aspect, 5 * aspect, -5, 5, 0.1, 100);
    } else {
        gluPerspective(60.0, (double)width / height, 0.1, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);
}
