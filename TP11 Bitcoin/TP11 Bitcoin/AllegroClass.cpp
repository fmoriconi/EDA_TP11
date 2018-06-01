#include "AllegroClass.h"

#include <iostream>
using namespace std;
allegroClass::allegroClass()
{
	if (al_init())
	{
		if (al_install_audio())
		{
			if (al_init_acodec_addon())
			{
				if ((al_reserve_samples(1)))
				{
					if (al_init_image_addon())
					{
						if (al_init_primitives_addon()) {
							if (al_install_keyboard())
							{
								if ((ev_queue = al_create_event_queue()))
								{
									if ((display = al_create_display(SCREEN_W, SCREEN_H)))
									{
										al_register_event_source(this->ev_queue, al_get_keyboard_event_source());
										al_register_event_source(this->ev_queue, al_get_display_event_source(this->display));
										al_set_window_title(display, "EDAcoin Mining Facility.encd");
										ALLEGRO_BITMAP * icon = al_load_bitmap("icon.png");
										al_set_display_icon(display, icon);
									}
								}
							}
						}
						else
							std::cout << "ERROR: Failed to load primitives addon\n" << std::endl;
					}
					else
						std::cout << "ERROR: Failed to load Image addon\n" << std::endl;
				}
				else
					std::cout << "ERROR: Failed to reserve sample\n" << std::endl;
			}
			else
				std::cout << "ERROR: Failed to install acodec addon\n" << std::endl;
		}
		else
			std::cout << "ERROR: Failed to install audio\n" << std::endl;
	}
	else
		std::cout << "ERROR: Failed to initialize allegro system\n" << std::endl;
}
void allegroClass::newBitmap(ALLEGRO_BITMAP * bitmap_)
{

}

allegroClass::~allegroClass()
{
	for (ALLEGRO_BITMAP* bitmop : bitmaps) {
		al_destroy_bitmap(bitmop);
	}
	al_destroy_event_queue(ev_queue);
	al_destroy_display(display);
}

void allegroClass::updateDisplay()
{
	al_flip_display();
}

ALLEGRO_EVENT_QUEUE * allegroClass::getEventQueue()
{
	return ev_queue;
}