#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    float eye[3] = {0.0f, 5.0f, 10.0f};
    float center[3] = {0.0f, 0.0f, 0.0f};
    float up[3] = {0.0f, 1.0f, 0.0f};
    float angleX = 0.0f, angleY = 0.0f;

    void apply();
    void rotate(float dx, float dy);
    void setProjection(bool ortho, int width, int height);
};

#endif
