#include "EventManager.h"

void EventManager::init( GLFWwindow *window)
{
    glfwSetKeyCallback(window, keybEventManager);
    glfwSetMouseButtonCallback(window, mouseButtonEvent);
    glfwSetCursorPosCallback(window, mousePosEvent);


    //inicializar posición de ratón a centro de pantalla
        //tamaño de pantalla
    int w;
    int h;
    glfwGetWindowSize(window, &w, &h);
    mouseState.x = w / 2;
    mouseState.y = h / 2;
    //ocultar ratón
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, mouseState.x, mouseState.y);
}

void EventManager::keybEventManager(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS  :
        keyState[key] = true;
        break;
    case GLFW_REPEAT:
        break;
    case GLFW_RELEASE:
        keyState[key] = false;
        break;
    }
}

void EventManager::mousePosEvent(GLFWwindow* window, double xpos, double ypos)
{
    mouseState.x = xpos;
    mouseState.y = ypos;
}

void EventManager::mouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
    switch (action)
    {
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
