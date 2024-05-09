#include <jack/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <jack/jack.h>

#include "freqy_audio.h"

jack_port_t *left_in_port;
jack_port_t *right_in_port;
jack_client_t *client;

int process (jack_nframes_t nframes, void *arg) {
	FreqyBuffer *bmut = (FreqyBuffer*)arg;
	int buf_size;
	if (nframes*sizeof(sample_t) < bmut->buf_size){
		buf_size = nframes*sizeof(sample_t);
	} else {
		buf_size = bmut->buf_size;
	}
	sample_t *left_in, *right_in;
	left_in = jack_port_get_buffer(left_in_port, nframes);
	right_in = jack_port_get_buffer(right_in_port, nframes);

	if (bmut->mut == MUT_OPEN) {
		bmut->mut = MUT_LOCKED;
		memcpy(bmut->l_buf, left_in, buf_size);
		memcpy(bmut->r_buf, right_in, buf_size);
		bmut->mut = MUT_OPEN;
	}
	return 0;
}

void jack_shutdown(void *arg) {
	exit(1);
}

void freqy_jack_stop() {
	jack_client_close (client);
}

jack_client_t* jack_start_with_buf(FreqyBuffer *f_buf) {
	const char **ports;
	const char *client_name = "breakerz";
	const char *server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;

	client = jack_client_open(client_name, options, &status, server_name);
	if (client == NULL) {
		fprintf (stderr, "jack_client_open() failed, "
		   "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) {
		fprintf (stderr, "JACK server started\n");
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		fprintf (stderr, "unique name `%s' assigned\n", client_name);
	}


	jack_set_process_callback(client, process, f_buf);

	jack_on_shutdown(client, jack_shutdown, 0);

	printf ("engine sample rate: %" PRIu32 "\n", jack_get_sample_rate (client));

	left_in_port = jack_port_register(client, "in_l", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	right_in_port = jack_port_register(client, "in_r", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

	if ((left_in_port == NULL) || (right_in_port == NULL)) {
		fprintf(stderr, "no more JACK ports available\n");
		exit (1);
	}

	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

	ports = jack_get_ports (client, "PulseAudio JACK Sink:front-left", NULL, 0);
	if (ports == NULL) {
		fprintf(stderr, "Could not get capture_1 (left)\n");
		exit (1);
	}

	if (jack_connect (client, ports[0], jack_port_name (left_in_port))) {
		fprintf (stderr, "cannot connect to ports: %s, %s\n", ports[0], jack_port_name(left_in_port));
	}

	free (ports);

	ports = jack_get_ports (client, "PulseAudio JACK Sink:front-right", NULL, 0);
	if (ports == NULL) {
		fprintf(stderr, "Could not get capture_2\n");
		exit (1);
	}

	if (jack_connect (client, ports[0], jack_port_name (right_in_port))) {
		fprintf (stderr, "cannot connect to ports: %s, %s\n", ports[0], jack_port_name(right_in_port));
	}

	free (ports);

	return client;
}

// Allocates a stereo buffer and returns a pointer to the FreqyBuffer
FreqyBuffer *freqy_build_buffer(int buf_len) {
	FreqyBuffer *f_buf = malloc(sizeof *f_buf);
	f_buf -> buf_len = buf_len;
	f_buf -> buf_size = buf_len*sizeof(sample_t);
	f_buf -> l_buf = malloc(buf_len*sizeof(sample_t));
	f_buf -> r_buf = malloc(buf_len*sizeof(sample_t));
	f_buf -> mono = malloc(buf_len*sizeof(sample_t));
	f_buf -> mut = MUT_OPEN;
	return f_buf;
}

void freqy_buf_update_mono(FreqyBuffer* f_buf) {
	for(int i = 0; i < f_buf->buf_len; i++) {
		f_buf->mono[i] = (f_buf->l_buf[i] + f_buf->r_buf[i])/2;
	}
}

void freqy_get_fft_buf(FreqyBuffer* f_buf) {
	
}

// Deallocates buffers
void freqy_destroy_buffer(FreqyBuffer *f_buf) {
	free(f_buf->l_buf);
	free(f_buf->r_buf);
	free(f_buf);
	return;
}
