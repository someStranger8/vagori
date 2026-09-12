#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

void audio_init(void);
void audio_play_ambient(const char* filepath, float fade_duration);
void audio_play_sfx(const char* filepath);
void audio_update(float delta_time);
void audio_cleanup(void);

#endif
