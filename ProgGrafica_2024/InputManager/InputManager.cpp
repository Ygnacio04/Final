#include "InputManager.h"
#include "../prgr/common.h"
using namespace std;

void InputManager::init(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyboardManager);
    glfwSetCursorPosCallback(window, mousePosManager);
    glfwSetMouseButtonCallback(window, mouseButtonManager);

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    mouseState.x = w / 2;
    mouseState.y = h / 2;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, mouseState.x, mouseState.y);
}

void InputManager::keyboardManager(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (action) {
        case GLFW_PRESS:
            keyState[key] = true;
            break;
        case GLFW_REPEAT:
            break;
        case GLFW_RELEASE:
            keyState[key] = false;
            break;
    }
}

void InputManager::mousePosManager(GLFWwindow* window, double xpos, double ypos)
{
    mouseState.x = xpos;
    mouseState.y = ypos;
}

void InputManager::mouseButtonManager(GLFWwindow* window, int button, int action, int mods)
{
    switch (action) {
    case GLFW_PRESS:
        mouseState.mouseButtonState[button] = true;
        break;
    case GLFW_REPEAT:
        break;
    case GLFW_RELEASE:
        mouseState.mouseButtonState[button] = false;
        break;
    }
}
