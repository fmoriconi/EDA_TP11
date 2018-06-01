#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h> 
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <list>
#include <vector>

#define SCREEN_W 1366
#define SCREEN_H 768

enum class BITMAPS {MINER};

class allegroClass
	{
	public:
		allegroClass();
		~allegroClass();
		ALLEGRO_EVENT_QUEUE * getEventQueue();
		void updateDisplay();
	private:
		ALLEGRO_DISPLAY * display = NULL;
		ALLEGRO_EVENT_QUEUE * ev_queue = NULL;
		ALLEGRO_TIMER * timer = NULL;
		std::vector<ALLEGRO_BITMAP*> bitmaps;
		void newBitmap(ALLEGRO_BITMAP* bitmap_);
		
	};

