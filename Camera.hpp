#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    float r;
    float theta;
    float phi;
    glm::vec3 target;
    glm::vec3 up;

    bool dragging;
    double lastX, lastY;
    float sensitivity;

    Camera()
        : r(8.66f), theta(glm::radians(45.0f)), phi(glm::radians(35.0f)),
          target(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f),
          dragging(false), lastX(0), lastY(0), sensitivity(0.005f) {}

    glm::vec3 getPosition() const
    {
        float x = r * cos(phi) * cos(theta);
        float y = r * sin(phi);
        float z = r * cos(phi) * sin(theta);
        return glm::vec3(x, y, z);
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(getPosition(), target, up);
    }

    void processMouseMovement(double xpos, double ypos)
    {
        if (dragging)
        {
            float dx = static_cast<float>(xpos - lastX);
            float dy = static_cast<float>(ypos - lastY);
            theta -= dx * sensitivity;
            phi -= dy * sensitivity;

            if (phi < 0.1f)
                phi = 0.1f;
            if (phi > glm::radians(89.0f))
                phi = glm::radians(89.0f);
        }
        lastX = xpos;
        lastY = ypos;
    }

    void processMouseButton(int button, int action)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            dragging = (action == GLFW_PRESS);
        }
    }

    void processKey(int key, int action)
    {
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            if (key == GLFW_KEY_UP)
            {
                r -= 0.5f;
                if (r < 1.0f)
                    r = 1.0f;
            }
            if (key == GLFW_KEY_DOWN)
            {
                r += 0.5f;
            }
        }
    }
};

#endif