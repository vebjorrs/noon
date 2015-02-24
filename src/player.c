#ifndef PLAYER
#define PLAYER
#include "player.h"

#define BITS 8

int init_player(char *music)
{
	mpg123_handle *mh;
	unsigned char *buffer;
	size_t buffer_size;
	size_t done;
	int err;

	int driver;
	ao_device *dev;

	ao_sample_format format;
	int channels, encoding;
	long rate;

	printf("Starting stuff.\n");
	if (!music) {
		printf("No filename supplied.\n");
		return 1;
	}

	/* Init */
	ao_initialize();
	driver = ao_default_driver_id();
	mpg123_init();
	mh = mpg123_new(NULL, &err);
	printf("ERR = %u\n", err);
	buffer_size = mpg123_outblock(mh);
	buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

	/* Get encoding format of file */
	mpg123_open(mh, music);
	mpg123_getformat(mh, &rate, &channels, &encoding);

	/* Set output format, open output device */
	format.bits = mpg123_encsize(encoding) * BITS;
	format.rate = rate;
	format.channels = channels;
	format.byte_format = AO_FMT_NATIVE;
	format.matrix = 0;
	dev = ao_open_live(driver, &format, NULL);

	/* Decode and play */
	while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
		printf("Playing song.\n");
		ao_play(dev, buffer, done);
	}

	/* Clean up */
	free(buffer);
	ao_close(dev);
	mpg123_close(mh);
	mpg123_delete(mh);
	mpg123_exit();
	ao_shutdown();

	return 0;
}

#endif