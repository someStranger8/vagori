#include "../include/audio.h"
#include "../include/engine.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    ma_sound sounds[2];
    int active_idx;
    int fade_out_idx;
    
    bool slot_active[2];
    char paths[2][256];

    float fade_duration;
    float fade_timer;
    bool is_fading;
} CrossfadeState;

static CrossfadeState g_crossfade = {
    .active_idx = -1,
    .fade_out_idx = -1,
    .slot_active = {false, false},
    .paths = {"", ""}
};

void audio_play_ambient(const char* filepath, float fade_duration) {
    if (!filepath || strlen(filepath) == 0) return;

    if (g_crossfade.active_idx != -1 && strcmp(g_crossfade.paths[g_crossfade.active_idx], filepath) == 0) {
        return;
    }

    int next_idx = (g_crossfade.active_idx == 0) ? 1 : 0;

    if (g_crossfade.fade_out_idx != -1) {
        int old_idx = g_crossfade.fade_out_idx;
        if (g_crossfade.slot_active[old_idx]) {
            ma_sound_stop(&g_crossfade.sounds[old_idx]);
            ma_sound_uninit(&g_crossfade.sounds[old_idx]);
            g_crossfade.slot_active[old_idx] = false;
            g_crossfade.paths[old_idx][0] = '\0';
        }
    }

    g_crossfade.fade_out_idx = g_crossfade.active_idx;
    ma_result result = ma_sound_init_from_file(
        &g_engine.audio,
        filepath,
        MA_SOUND_FLAG_LOOPING,
        NULL,
        NULL,
        &g_crossfade.sounds[next_idx]
    );

    if (result == MA_SUCCESS) {
        ma_sound_set_volume(&g_crossfade.sounds[next_idx], 0.0f);
        ma_sound_start(&g_crossfade.sounds[next_idx]);
        
        g_crossfade.slot_active[next_idx] = true;
        strncpy(g_crossfade.paths[next_idx], filepath, 255);
        g_crossfade.active_idx = next_idx;
    } else {
        printf("[AUDIO ERROR] Failed to load sound: %s\n", filepath);
        g_crossfade.active_idx = -1;
        return;
    }

    g_crossfade.fade_duration = (fade_duration > 0.1f) ? fade_duration : 0.1f;
    g_crossfade.fade_timer = 0.0f;
    g_crossfade.is_fading = true;
}

void audio_update(float delta_time) {
    if (!g_crossfade.is_fading) return;

    g_crossfade.fade_timer += delta_time;
    float progress = g_crossfade.fade_timer / g_crossfade.fade_duration;

    if (progress >= 1.0f) {
        progress = 1.0f;
        g_crossfade.is_fading = false;
    }

    if (g_crossfade.active_idx != -1 && g_crossfade.slot_active[g_crossfade.active_idx]) {
        ma_sound_set_volume(&g_crossfade.sounds[g_crossfade.active_idx], progress);
    }

    if (g_crossfade.fade_out_idx != -1 && g_crossfade.slot_active[g_crossfade.fade_out_idx]) {
        int out_idx = g_crossfade.fade_out_idx;
        ma_sound_set_volume(&g_crossfade.sounds[out_idx], 1.0f - progress);

        if (!g_crossfade.is_fading) {
            ma_sound_stop(&g_crossfade.sounds[out_idx]);
            ma_sound_uninit(&g_crossfade.sounds[out_idx]);
            g_crossfade.slot_active[out_idx] = false;
            g_crossfade.paths[out_idx][0] = '\0';
            g_crossfade.fade_out_idx = -1;
        }
    }
}

void audio_cleanup(void) {
    for (int i = 0; i < 2; ++i) {
        if (g_crossfade.slot_active[i]) {
            ma_sound_uninit(&g_crossfade.sounds[i]);
            g_crossfade.slot_active[i] = false;
        }
    }
}

void audio_play_sfx(const char* filepath) {
    ma_engine_play_sound(&g_engine.audio, filepath, NULL);
}
