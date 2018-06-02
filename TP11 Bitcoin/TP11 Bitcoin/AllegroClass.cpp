#include "AllegroClass.h"
#include <iostream>
using namespace std;
allegroClass::allegroClass(unsigned quantity_) : quantity(quantity_)
{

	bool goodInit = false;

	if (al_init()) {
		if (al_install_audio()) {
			if (al_init_acodec_addon()) {
				if (al_init_image_addon()) {
					if (al_init_primitives_addon()) {
						if (al_init_font_addon()) {
							if (al_init_ttf_addon()) {
								if (al_install_keyboard()) {
									if ((ev_queue = al_create_event_queue())) {
										if ((display = al_create_display(SCREEN_W, SCREEN_H))) {
											this->smallfont = al_load_font("font.ttf", -SMALLFONT_SIZE, NULL);
											this->bigfont = al_load_font("font.ttf", -BIGFONT_SIZE, NULL);
											al_register_event_source(this->ev_queue, al_get_keyboard_event_source());
											al_register_event_source(this->ev_queue, al_get_display_event_source(this->display));
											al_set_window_title(display, "EDAcoin Mining Facility.encd");
											ALLEGRO_BITMAP * icon = al_load_bitmap("icon.png");
											al_set_display_icon(display, icon);
											goodInit = true;
										}
									}
								}
							}
							else
								std::cout << "ERROR: Failed to load ttf addon\n" << std::endl;
						}
						else
							std::cout << "ERROR: Failed to load font addon\n" << std::endl;
					}
					else
						std::cout << "ERROR: Failed to load primitives addon\n" << std::endl;
				}
				else
					std::cout << "ERROR: Failed to load Image addon\n" << std::endl;
			}
			else
				std::cout << "ERROR: Failed to install acodec addon\n" << std::endl;
		}
		else
			std::cout << "ERROR: Failed to install audio\n" << std::endl;
	}
	else
		std::cout << "ERROR: Failed to initialize allegro system\n" << std::endl;

	if (goodInit) {
		calculateVertices();
	}

}
void allegroClass::newBitmap(ALLEGRO_BITMAP * bitmap_)
{

}

void allegroClass::drawButton(unsigned x1, unsigned y1, unsigned x2, unsigned y2, ALLEGRO_COLOR color, std::string msg)
{
	al_draw_filled_rounded_rectangle(x1 + 1, y1 + 1, x2 + 2, y2 + 2, 3, 3, AL_BLACK);
	al_draw_filled_rounded_rectangle(x1, y1, x2, y2, 3, 3, color);
	al_draw_text(smallfont, AL_BLACK, (x1 + x2) / 2, (y1 + y2) / 2 - SMALLFONT_SIZE / 2, ALLEGRO_ALIGN_CENTER, msg.c_str());
}

void allegroClass::infoWindow(Nodo* nodito) {
	al_draw_filled_rounded_rectangle(INFO_WINDOW_X1 + 1, INFO_WINDOW_Y1 + 1, INFO_WINDOW_X2 + 2, INFO_WINDOW_Y2 + 2, 3, 3, AL_BLACK);
	al_draw_filled_rounded_rectangle(INFO_WINDOW_X1, INFO_WINDOW_Y1, INFO_WINDOW_X2, INFO_WINDOW_Y2, 3, 3, AL_DARK_ORANGE);
	std::string auxstring = "Node " + std::to_string(nodito->getID()) + " info:";
	al_draw_text(smallfont, AL_BLACK, INFO_WINDOW_TITLE_X, INFO_WINDOW_TITLE_Y, ALLEGRO_ALIGN_CENTER, auxstring.c_str());
	auxstring = "Balance: " + std::to_string(nodito->getBalance());
	al_draw_text(smallfont, AL_BLACK, INFO_WINDOW_TEXT_X, INFO_WINDOW_TEXT_Y1, ALLEGRO_ALIGN_LEFT, auxstring.c_str());
	if (nodito->getIsMiner()) {
		auxstring = "Miner";
	}
	else {
		auxstring = "Full service";
	}
	al_draw_text(smallfont, AL_BLACK, INFO_WINDOW_TEXT_X, INFO_WINDOW_TEXT_Y2, ALLEGRO_ALIGN_LEFT, auxstring.c_str());
	auxstring = ""; //provisorio
	al_draw_text(smallfont, AL_BLACK, INFO_WINDOW_TEXT_X, INFO_WINDOW_TEXT_Y3, ALLEGRO_ALIGN_CENTER, auxstring.c_str());
	al_draw_text(smallfont, AL_BLACK, INFO_WINDOW_TEXT_X, INFO_WINDOW_TEXT_Y4, ALLEGRO_ALIGN_CENTER, auxstring.c_str());
	al_draw_text(smallfont, AL_BLACK, INFO_WINDOW_TEXT_X, INFO_WINDOW_TEXT_Y5, ALLEGRO_ALIGN_CENTER, auxstring.c_str());
}

allegroClass::~allegroClass()
{
	for (ALLEGRO_BITMAP* bitmop : bitmaps) {
		al_destroy_bitmap(bitmop);
	}
	al_destroy_event_queue(ev_queue);
	al_destroy_display(display);
}

void allegroClass::updateDisplay(std::vector<Nodo*>& nodos)
{
	al_draw_filled_rectangle(START_X, START_Y, INFO_GRAPH_LIMIT_X, END_Y, AL_ORANGE);
	al_draw_text(bigfont, AL_BLACK, TITLE_POS_X, TITLE_POS_Y, ALLEGRO_ALIGN_CENTER, "EDAcoin Mining Facility");
	for (Nodo* nodito : nodos) {
		if (nodito->selected) {
			drawButton(PAY_BUTTON_X1, PAY_BUTTON_Y1, PAY_BUTTON_X2, PAY_BUTTON_Y2, AL_DARK_ORANGE, PAY_BUTTON_TEXT);
			drawButton(MALICIOUS_PAY_BUTTON_X1, MALICIOUS_PAY_BUTTON_Y1, MALICIOUS_PAY_BUTTON_X2, MALICIOUS_PAY_BUTTON_Y2, AL_DARK_ORANGE, MALICIOUS_PAY_BUTTON_TEXT);
			infoWindow(nodito);
		}
	}
	drawConnection(nodos);
	drawNodes(nodos);
	al_flip_display();
}

void allegroClass::drawNodes(std::vector<Nodo*>& nodos)
{

	int j;

	for (int i = 0, j = 0; i < quantity * 2; i += 2) {
		if (nodos[j]->selected)
			al_draw_filled_circle(vert[i], vert[i + 1], CIRCLE_RADIUS - (quantity / 2), AL_DARK_ORANGE);
		else
			al_draw_filled_circle(vert[i], vert[i + 1], CIRCLE_RADIUS - (quantity / 2), AL_ORANGE);

		al_draw_text(smallfont, AL_BLACK, vert[i], vert[i + 1] - SMALLFONT_SIZE / 2, ALLEGRO_ALIGN_CENTER, std::to_string(j).c_str());
		j++;
	}

}

void allegroClass::calculateVertices() {

	for (int i = 0; i < quantity * 2; i++)
		vert.push_back(0);

	float alpha = (2 * M_PI / quantity); //angulo entre nodos

	for (int i = 0, h = 1; i < 2 * quantity; i += 2)
	{
		//Por cada vertice, se rota a partir del centro de masa en funcion del angulo calculado anteriormente
		vert[i] = (((SCREEN_W - INFO_GRAPH_LIMIT_X) / 2) + INFO_GRAPH_LIMIT_X) + (GRAPH_RADIUS)* cos(alpha * h);
		h++;
	}

	for (int i = 1, h = 1; i < 2 * quantity; i += 2)
	{
		//Idem para coordinadas en Y
		vert[i] = SCREEN_H / 2 + (GRAPH_RADIUS)* sin(alpha*h);
		h++;
	}
}

void allegroClass::drawConnection(std::vector<Nodo*>& nodos)
{
	for (int i = 0; i < quantity; i++) {
		for (int j = 0; j < nodos[i]->connectedNodes.size(); j++) {
			Nodo* auxNodo = nodos[i]->connectedNodes[j];
			if (auxNodo != nullptr) {
				al_draw_line(vert[2 * i], vert[(2 * i) + 1], vert[2 * (auxNodo->getID())], vert[(2 * (auxNodo->getID())) + 1], AL_DARK_ORANGE, 3);
			}
		}
	}
}

ALLEGRO_EVENT_QUEUE * allegroClass::getEventQueue()
{
	return ev_queue;
}