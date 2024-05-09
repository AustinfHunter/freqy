#include <glad/glad.h>
#include <complex.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include "window.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0,0,width,height);
}

void freqy_default_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void freqy_error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW Error: %s\n", description);
}


GLFWwindow* freqy_graphics_init_glfw() {
	GLFWwindow* window;

	glfwSetErrorCallback(freqy_error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "FREQY", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("ERROR: Failed to initialize GLAD\n");
		exit(EXIT_FAILURE);
	}

	glViewport(0,0,800,600);

	glfwSetKeyCallback(window, freqy_default_key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

void freqy_graphics_terminate_glfw(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}
