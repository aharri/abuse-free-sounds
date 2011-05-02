/*
 * Copyright (c) 2011 Antti Harri <iku@openbsd.fi>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <sys/param.h>
#include <unistd.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "hmi.h"

void
usage(char *cmd)
{
		printf("%s file.hmi\n", cmd);
		exit(1);
}

int 
main(int argc, char *argv[])
{
	unsigned char  *data = NULL;
	SDL_RWops      *rw = NULL;
	Mix_Music      *music = NULL;
	unsigned int    data_size;

	if (argc <= 1)
		usage(argv[0]);

	if (SDL_Init(SDL_INIT_AUDIO) == -1) {
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("Sound: Unable to open audio - %s\nSound: Disabled (error)\n", SDL_GetError());
		return 0;
	}
//	Mix_AllocateChannels(1);

	data = load_hmi(argv[1], data_size);

	if (!data) {
		printf("Sound: ERROR - could not load %s\n", argv[1]);
		Mix_CloseAudio();
		return 1;
	}
	rw = SDL_RWFromMem(data, data_size);
	music = Mix_LoadMUS_RW(rw);

	if (!music) {
		printf("Sound: ERROR - %s while loading %s\n",
		       Mix_GetError(), argv[1]);
		SDL_FreeRW(rw);
		free(data);
		Mix_CloseAudio();
		return 1;
	}
	Mix_PlayMusic(music, 0);
	Mix_VolumeMusic(128);
	while (Mix_PlayingMusic()) {
		sleep(1);
	}
	Mix_FreeMusic(music);
	SDL_FreeRW(rw);
	free(data);
	Mix_CloseAudio();

	return 0;
}
