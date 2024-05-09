#include <glad/glad.h>
#include "freqy_graphics.h"
#include <stdio.h>

char* freqy_load_shader(char* filename) {
	char* buf = 0;
	long len;
	FILE* f = fopen(filename, "r");

	if (f) {
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);
		buf = malloc(len+1);
		
		if (buf) {
			fread(buf, 1, len, f);
		}
		buf[len] = '\0';
		fclose(f);
		return buf;
	} else {
		fprintf(stderr, "ERR: Could not read shader file at %s\n", filename);
		exit(EXIT_FAILURE);
	}
}

void freqy_log_programiv(unsigned int program) {
	int success;
	char infolog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	
	if(!success) {
		glGetShaderInfoLog(program, 512, NULL, infolog);
		printf("%s\n", infolog);
	} else {
		printf("Successfully linked program\n");
	}
}

void freqy_log_shaderiv(unsigned int shader, const char* shader_name) {
	int success;
	char infolog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if(!success) {
		glGetShaderInfoLog(shader, 512, NULL, infolog);
		printf("Error Compiling %s: %s\n", shader_name, infolog);
	} else {
		printf("Successfully compiled %s\n", shader_name);
	}
}

void freqy_graphics_init_gl(freqy_graphics_state* state) {
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	state->vao = VAO;
	state->vbo = VBO;
	state->ebo = EBO;

	char* def_vert_shader = freqy_load_shader("shaders/simple.vert");
	char* def_frag_shader = freqy_load_shader("shaders/simple.frag");

	// setup shaders
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char* const *)&def_vert_shader, NULL);
	glCompileShader(vertexShader);

	freqy_log_shaderiv(vertexShader, "Vert Shader");

	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, (const char* const *)&def_frag_shader, NULL);
	glCompileShader(fragShader);

	freqy_log_shaderiv(fragShader, "Frag Shader");

	// setup shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	
	freqy_log_programiv(shaderProgram);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	state->shader_program = shaderProgram;
	return;
}
