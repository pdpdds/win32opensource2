// fluidSynthEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <fluidsynth.h>

#ifdef _DEBUG
#pragma comment(lib, "fluidsynth_debug.lib")
#else
#pragma comment(lib, "fluidsynth.lib")
#endif

int main(int argc, char** argv)
{
	int i;
	fluid_settings_t* settings;
	fluid_synth_t* synth;
	fluid_player_t* player;
	fluid_audio_driver_t* adriver;
	settings = new_fluid_settings();
	synth = new_fluid_synth(settings);
	player = new_fluid_player(synth);
	adriver = new_fluid_audio_driver(settings, synth);
	/* process command line arguments */
	for (i = 1; i < argc; i++) {
		if (fluid_is_soundfont(argv[i])) {
			fluid_synth_sfload(synth, argv[1], 1);
		}
		if (fluid_is_midifile(argv[i])) {
			fluid_player_add(player, argv[i]);
		}
	}
	/* play the midi files, if any */
	fluid_player_play(player);
	/* wait for playback termination */
	fluid_player_join(player);
	/* cleanup */
	delete_fluid_audio_driver(adriver);
	delete_fluid_player(player);
	delete_fluid_synth(synth);
	delete_fluid_settings(settings);
	return 0;
}