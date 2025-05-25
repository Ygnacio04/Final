#include "../prgr/common.h"
#include "../prgr/lib.h"
using namespace std;

typedef struct {
	double x, y;
	map<int, bool> mouseButtonState;
} mouseState_t;

class InputManager {
public:
    inline static map<int, bool> keyState;
    inline static mouseState_t mouseState;

    static void init(GLFWwindow* window);
    static void keyboardManager(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mousePosManager(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonManager(GLFWwindow* window, int button, int action, int mods);
};