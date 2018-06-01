#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h> 
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

#define FPS (50.0)
#define SCREEN_W 1920 
#define SCREEN_H 900

class allegroClass
	{
	public:
		allegroClass();
		~allegroClass();
		ALLEGRO_EVENT_QUEUE * getEventQueue();
		bool load_music(const char * music_file);
		void play_music();
		void updateDisplay();
		void start_timer();
	private:
		ALLEGRO_DISPLAY * display = NULL;
		ALLEGRO_EVENT_QUEUE * ev_queue = NULL;
		ALLEGRO_SAMPLE * music = NULL;
		ALLEGRO_TIMER * timer = NULL;
	};

