#include <GL/gl.h>
#include <stdlib.h>

#ifndef FREQY_GRAPHICS
#define FREQY_GRAPHICS
typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} freqy_vertex_t;

typedef struct {
	freqy_vertex_t* start_vert;
	freqy_vertex_t* end_vert;
} freqy_sample_bar;

typedef struct {
	int nsamples;
	GLfloat width;
	GLfloat height;
	freqy_vertex_t* origin;
	GLfloat* vertices;
	GLuint* indices;
} freqy_dwave;

typedef struct {
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int shader_program;
} freqy_graphics_state;


freqy_dwave* freqy_create_dwave(int nsamples, GLfloat width, GLfloat height, GLfloat origin_x, GLfloat origin_y, GLfloat origin_z);

void freqy_dwave_make_rects(freqy_dwave* dwave, GLfloat* heights);

void freqy_dwave_make_indices(freqy_dwave* dwave);

void freqy_dwave_set_heights(GLfloat* new_heights, freqy_dwave* dwave); 

void freqy_destroy_dwave(freqy_dwave* dwave);

char* freqy_load_shader(char* filename);

void freqy_print_dwave(freqy_dwave* dwave);

void freqy_dwave_render(freqy_graphics_state* state, freqy_dwave* dwave);

void freqy_log_programiv(unsigned int program);

void freqy_log_shaderiv(unsigned int shader, const char* shader_name); 

void freqy_graphics_init_gl(freqy_graphics_state* state);

#endif
