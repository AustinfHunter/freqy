#ifndef FREQY_AUDIO_H
#define FREQY_AUDIO_H

#include <jack/types.h>

typedef jack_default_audio_sample_t sample_t;

/**
 * Used to track the state of a FreqyBuffer. Quick, Dirty, Non-locking!
 */
typedef enum {
	MUT_LOCKED = 0,
	MUT_OPEN = 1,
} freqy_buf_state_e;

/**
 * Represents our audio data, two channels, left and right.
 * l_buf is the buffer for the left audio channel.
 * r_buf is the buffer for the right audio channel.
 * Both l_buf and r_buf have a size of buf_size*sizeof(double) bytes
 * Mut is used to prevent simulataneous access without locking. Again, quick and dirty.
 */
typedef struct {
	unsigned int buf_len; // number of entries in buffers
	unsigned int buf_size; // number of bytes in buffers
	sample_t* l_buf;
	sample_t* r_buf;
	sample_t* mono;
	freqy_buf_state_e mut;
} FreqyBuffer;

FreqyBuffer *freqy_build_buffer(int buf_size);

void freqy_destroy_buffer(FreqyBuffer *f_buf);

void freqy_clear_buffers(FreqyBuffer *f_buf);

void freqy_buf_update_mono(FreqyBuffer* f_buf);

void freqy_jack_stop();

jack_client_t* jack_start_with_buf(FreqyBuffer *f_buf);
#endif
