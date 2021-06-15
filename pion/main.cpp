#include "pch.h"
#include "main_window.h"

// https://stackoverflow.com/questions/30024364/any-ways-to-run-program-debugging-in-visual-studio-on-nvidia-graphics-card
#ifdef _WIN32
extern "C" {
    __declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
}
#endif

int main(int argc, char *argv[])
{
    glfwInit();

    main_window wnd;

    glewExperimental = GL_TRUE;
    glewInit();

    wnd.initialize();
    wnd.loop();

    glfwTerminate();
}
