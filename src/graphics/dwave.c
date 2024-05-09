#include <glad/glad.h>
#include "freqy_graphics.h"
#include <stdio.h>

freqy_dwave* freqy_create_dwave(int nsamples, GLfloat width, GLfloat height, GLfloat origin_x, GLfloat origin_y, GLfloat origin_z) {
	//4 points per rect, 3 coords per point
	GLfloat* vertices = calloc(sizeof(GLfloat), nsamples*12); 
	//2 trangles per rect, 3 indices per triangle
	GLuint* indices = calloc(sizeof(GLuint), nsamples*6);

	freqy_vertex_t* origin = malloc(sizeof(GLfloat)*3);
	origin->x = origin_x;
	origin->y = origin_y;
	origin->z = origin_z;

	freqy_dwave* dwave = malloc(sizeof(freqy_dwave));
	dwave->nsamples = nsamples;
	dwave->width = width;
	dwave->height = height;
	dwave->origin = origin;
	dwave->vertices = vertices;
	dwave->indices = indices;

	return dwave;
}

void freqy_dwave_make_rects(freqy_dwave* dwave, GLfloat* heights) {
	// one rect per sample, spread over width
	GLfloat width_per = (GLfloat)(dwave->width/dwave->nsamples);
	printf("width per: %f\n", width_per);
	printf("Total width: %f\n", dwave->nsamples*width_per);
	// pattern:
	// 		top_right = (origin_x + width_per(i+1), origin_y + 10, z);
	// 		bot_right = (origin_x + width_per(i+1), origin_y + 0, z);
	// 		bot_left = (origin_x + with_per*i, origin_y + 0, z);
	// 		top_left = (origin_x + width_per*i, origin_y + 10, z);
	//
	for (long i = 0; i < dwave->nsamples*12; i+= 12) {
		//top right
		dwave->vertices[i] = dwave->origin->x + width_per*(i/12 + 1);
		dwave->vertices[i+1] = dwave->origin->y + heights[i/12];
		dwave->vertices[i+2] = dwave->origin->z;
		
		//bot right
		dwave->vertices[i+3] = dwave->origin->x + width_per*(i/12 + 1);
		dwave->vertices[i+4] = dwave->origin->y;
		dwave->vertices[i+5] = dwave->origin->z;

		//bot left
		dwave->vertices[i+6] = dwave->origin->x + width_per*(i/12 +1)  - width_per ;
		dwave->vertices[i+7] = dwave->origin->y;
		dwave->vertices[i+8] = dwave->origin->z;

		//top left
		dwave->vertices[i+9] = dwave->origin->x + width_per*(i/12 +1)- width_per;
		dwave->vertices[i+10] = dwave->origin->y + heights[i/12];
		dwave->vertices[i+11] = dwave->origin->z;
	}
}

void freqy_dwave_make_indices(freqy_dwave* dwave) {
	// indices pattern for ith rect: 
	// 			[top right, bot_right, top_left
	// 			bot_right, bot_left, top_left]
	// 			[i*4 ,i*4 + 1, i*4 + 3,
	// 			i*4 +1, i*4 + 2, i*4 + 3]
	int j = 0;
	for (int i = 0; i < dwave->nsamples*6; i += 6) {
		dwave->indices[i] = j;
		dwave->indices[i+1] = j + 1;
		dwave->indices[i+2] = j + 3;
		dwave->indices[i+3] = j + 1;
		dwave->indices[i+4] = j + 2;
		dwave->indices[i+5] = j + 3;
		j+=4;
	}
}

void freqy_dwave_set_heights(GLfloat* new_heights, freqy_dwave* dwave) {
	for (int i = 0; i < dwave->nsamples*12; i += 12){	
		dwave->vertices[i+1] = dwave->origin->y + new_heights[i];
		dwave->vertices[i + 10] = dwave->origin->y + new_heights[i];
	}
}

void freqy_dwave_render(freqy_graphics_state* state, freqy_dwave* dwave) {
	glBindVertexArray(state->vao);
	glBindBuffer(GL_ARRAY_BUFFER, state->vbo);
	glBufferData(GL_ARRAY_BUFFER, dwave->nsamples*12*sizeof(GLfloat), dwave->vertices, GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dwave->nsamples*sizeof(GLuint)*6, dwave->indices, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(state->shader_program);
	glDrawElements(GL_TRIANGLES, dwave->nsamples*6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void freqy_destroy_dwave(freqy_dwave* dwave) {
	free(dwave->origin);
	free(dwave->vertices);
	free(dwave->indices);
	free(dwave);
}

void freqy_print_dwave(freqy_dwave* dwave) {
	printf("DWAVE INFO\n");
	printf("ORIGIN: (%.2f, %.2f, %.2f)\n", dwave->origin->x, dwave->origin->y, dwave->origin->z);
	printf("VERTICES: [\n");
	for (int i = 0; i < dwave->nsamples*12; i += 3) {
		printf("\t%.2f, %.2f, %.2f,\n", dwave->vertices[i], dwave->vertices[i+1], dwave->vertices[i+2]);
	}
	printf("]\n");
	printf("INDICES: [\n");
	for(int i = 0; i < dwave->nsamples*6; i+=3) {
		printf("\t%d, %d, %d,\n", dwave->indices[i], dwave->indices[i+1], dwave->indices[i+2]);
	}
	printf("]\n");
}
