#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h> 
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <list>
#include <vector>
#include <string>
#include "Nodo.h"
#include "EDAevent.h"
#include "Timer.h"

#define SCREEN_W (1000)
#define SCREEN_H (600)
#define START_X (0)
#define START_Y (0)
#define END_X (SCREEN_W)
#define END_Y (SCREEN_H)
#define INFO_GRAPH_LIMIT_X (350)
#define BIGFONT_SIZE (35)
#define SMALLFONT_SIZE (18)
#define VERYSMALLFONT_SIZE (8)
#define TITLE_POS_X (INFO_GRAPH_LIMIT_X/2)
#define TITLE_POS_Y (30)
#define AL_ORANGE (al_map_rgb(255, 128, 0))
#define AL_DARK_ORANGE (al_map_rgb(255, 64, 0))
#define AL_GREEN (al_map_rgb(0, 153, 0))
#define AL_BLACK (al_map_rgb(0,0,0))
#define PAY_BUTTON_Y1 (END_Y - 100)
#define PAY_BUTTON_Y2 (PAY_BUTTON_Y1 + 50)
#define PAY_BUTTON_X1 (START_X + INFO_GRAPH_LIMIT_X / 11)
#define PAY_BUTTON_X2 (START_X + 5 * INFO_GRAPH_LIMIT_X / 11)
#define MALICIOUS_PAY_BUTTON_Y1 (PAY_BUTTON_Y1)
#define MALICIOUS_PAY_BUTTON_Y2 (PAY_BUTTON_Y2)
#define MALICIOUS_PAY_BUTTON_X1 (START_X + 6 * INFO_GRAPH_LIMIT_X / 11)
#define MALICIOUS_PAY_BUTTON_X2 (START_X + 10* INFO_GRAPH_LIMIT_X / 11)
#define PAY_BUTTON_TEXT "Pay bitcoins"
#define MALICIOUS_PAY_BUTTON_TEXT "Malicious pay"
#define INFO_WINDOW_X1 (PAY_BUTTON_X1)
#define INFO_WINDOW_Y1 (TITLE_POS_Y + 80)
#define INFO_WINDOW_X2 (MALICIOUS_PAY_BUTTON_X2)
#define INFO_WINDOW_Y2 (PAY_BUTTON_Y1 - 60)
#define GRAPH_RADIUS (SCREEN_H/2 - 75)
#define CIRCLE_RADIUS (30)
#define INFO_WINDOW_TITLE_X ((INFO_WINDOW_X1 + INFO_WINDOW_X2) / 2)
#define INFO_WINDOW_TITLE_Y (INFO_WINDOW_Y1 + 20 - SMALLFONT_SIZE / 2)
#define INFO_WINDOW_TEXT_X (INFO_WINDOW_X1 + 20)
#define INFO_WINDOW_TEXT_Y1 (INFO_WINDOW_TITLE_Y + 40 - SMALLFONT_SIZE/2)
#define INFO_WINDOW_TEXT_Y2 (INFO_WINDOW_TEXT_Y1 + 40 - SMALLFONT_SIZE/2)
#define INFO_WINDOW_TEXT_Y3 (INFO_WINDOW_TEXT_Y2 + 40 - SMALLFONT_SIZE/2)
#define INFO_WINDOW_TEXT_Y4 (INFO_WINDOW_TEXT_Y3+ 60 - SMALLFONT_SIZE/2)
#define INFO_WINDOW_TEXT_Y5 (INFO_WINDOW_TEXT_Y4 + 40 - SMALLFONT_SIZE/2)
#define PICK_SIZE (15)
#define DOUBLE_CLICK_TIME (150)
#define SHOW_BUTTON_PRESSED_TIME 30

class allegroClass
	{
	public:
		allegroClass(unsigned quantity_);
		~allegroClass();
		ALLEGRO_EVENT_QUEUE * getEventQueue();
		void updateDisplay(std::vector<Nodo*>& nodos);
		void calculateVertices();
		void drawNodes(std::vector<Nodo*>& nodos);
		void drawConnection(std::vector<Nodo*>& nodos);
		unsigned int drawInfo = 0;
		EDAevent& getInput(std::vector<Nodo*>& nodos);
	private:
		ALLEGRO_DISPLAY * display = NULL;
		ALLEGRO_EVENT_QUEUE * ev_queue = NULL;
		ALLEGRO_TIMER * timer = NULL;
		ALLEGRO_FONT* smallfont = NULL;
		ALLEGRO_FONT* bigfont = NULL;
		ALLEGRO_FONT* verysmallfont = NULL;
		ALLEGRO_BITMAP* pick = NULL;
		std::vector<ALLEGRO_BITMAP*> bitmaps;
		std::vector<int> vert;
		void drawButton(unsigned x1, unsigned y1, unsigned x2, unsigned y2, ALLEGRO_COLOR color, std::string msg);
		void infoWindow(Nodo* nodito);
		unsigned quantity;
		bool firstclick = false;
		bool firstletgo = false;
		bool buttonClick = false;
		bool payclick = false;
		bool mpayclick = false;
		Timer showButtonPressed;
		Timer doubleClickTimer;
	};

