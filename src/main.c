#include "audio/freqy_audio.h"
#include <glad/glad.h>
#include "graphics/window.h"
#include "graphics/freqy_graphics.h"
#include <GLFW/glfw3.h>
#include <jack/types.h>

int buffer_size;

int main(int argc, char **argv) {
	if (argc <= 1) {
		buffer_size = 42000;
	} else {
		buffer_size = (int)*argv[1];
	}

	FreqyBuffer* fbuf = freqy_build_buffer(buffer_size);

	jack_client_t* client = jack_start_with_buf(fbuf);


	// Window initialization
	GLFWwindow* window = freqy_graphics_init_glfw();

	freqy_graphics_state state;
	freqy_graphics_init_gl(&state);

	freqy_dwave* dwave = freqy_create_dwave(350, 1.0f, 1.5f, -0.5f, 0.0f, 0.0f); 

	GLfloat heights[350];
	for (int i = 0; i < 350; i++) {
		heights[i] = i * 0.08f;
	}

	freqy_dwave_make_rects(dwave, heights);
	freqy_dwave_make_indices(dwave);




	// Program loop 
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		freqy_buf_update_mono(fbuf);
		freqy_dwave_set_heights((GLfloat*)fbuf->mono, dwave);
		freqy_dwave_render(&state, dwave);
		glBindVertexArray(state.vao);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);
		glfwPollEvents();
		//printf("Left: %f\nRight: %f\n\n", *fbuf->l_buf, *fbuf->r_buf);
	}

	freqy_destroy_dwave(dwave);
	freqy_jack_stop();
	freqy_destroy_buffer(fbuf);
	freqy_graphics_terminate_glfw(window);
	return 0;
}
