#include <GLFW/glfw3.h>
#ifndef freqy_graphics
#define freqy_graphics

/* Create a GLFW window with freqy defaults */
GLFWwindow* freqy_graphics_init_glfw();

/* Terminate Freqy graphics window */
void freqy_graphics_terminate_glfw(GLFWwindow* window);

#endif
