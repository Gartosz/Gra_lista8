#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <stdlib.h> 
#include "plansza.h"
#include "main.h"

const int w = 800, h = 600;
struct pozycja
{
	float x;
	float y;
	float x0;
	float y0;
	float vx;
	float vy;
	int zwrot;
	pozycja() { x0 = 0; y0 = 0; x = 0.0; y = 0.0; vx = 0.0; vy = 0.0; zwrot = 0; }
	pozycja(int _x, int _y) { this->x = _x; this->y = _y; }
};

void ruch(sf::Event event, pozycja& gracz, float& czas, float sekundy, int& strzal,bool &bok);
void stop(sf::Event event, pozycja& gracz, bool & bok);
void nowa_gra(pozycja& gracz, pozycja* przeciwnicy, sf::CircleShape* wrogowie, sf::RectangleShape& postac,int &t,sf::Clock &zegar_gry, pozycja* pocisk_p);

int okno()
{
	int strzal[50];
	int a=0,b;
	float x;
	bool bok = false;
	int t = 0;
	srand(time(NULL));
	pozycja gracz(400, h - 0.05 * h - 30);;
	pozycja przeciwnicy[10];
	pozycja pocisk_p[50];
	sf::RectangleShape pocisk[50];
	sf::CircleShape wrogowie[10];
	sf::RenderWindow plansza(sf::VideoMode(w, h), "Gra");
	sf::RectangleShape postac(sf::Vector2f(10, 30));
	sf::RectangleShape platforma(sf::Vector2f(100, 15));
	platforma.setFillColor(sf::Color::Blue);
	platforma.setPosition(250,500);
	sf::Clock podskok;
	sf::Time czas = podskok.getElapsedTime();
	float sekundy = czas.asSeconds();
	float czas_skoku = 0;
	sf::Clock na_boki;
	sf::Time czas_boki = na_boki.getElapsedTime();
	float sekundy_boki = czas_boki.asSeconds();
	float czas_ruchu = 0;
	sf::RectangleShape podloga(sf::Vector2f(w, h - 0.95 * h));
	podloga.setFillColor(sf::Color::Green);
	podloga.setOrigin(0, h - 0.95 * h);
	sf::Clock zegar_p;
	sf::Time czas_p;
	float sekundy_p[50];
	sf::Clock zegar_gry;
	sf::Time czas_gry = zegar_gry.getElapsedTime();
	float sekundy_gry = czas_gry.asSeconds();
	sf::Font font;
	if (!font.loadFromFile("BitPap.ttf"))
	{
		// error...
	}
	sf::Text komunikat;
	komunikat.setFont(font);
	komunikat.setPosition(100,100);
	komunikat.setCharacterSize(40);
	komunikat.setString("Wygrales!\nAby zagrac ponownie wcisnij F1");
	sf::Text komunikat_p;
	komunikat_p.setFont(font);
	komunikat_p.setPosition(100, 100);
	komunikat_p.setCharacterSize(40);
	komunikat_p.setString("Skonczyl ci sie czas!\nAby zagrac ponownie wcisnij F1");
	sf::Text licznik;
	licznik.setFont(font);
	licznik.setPosition(650, 10);
	licznik.setCharacterSize(50);
	std::string tekst = std::to_string(20-sekundy_gry);
	tekst.resize(tekst.size()-4);
	licznik.setString(tekst);
	nowa_gra(gracz, przeciwnicy, wrogowie, postac, t,zegar_gry, pocisk_p);
	for (int i = 0; i < 50; i++)
	{
		strzal[i] = 0;
		sekundy_p[i] = 0;
		pocisk[i].setSize(sf::Vector2f(15, 3));
		pocisk[i].setPosition(0, 0);
		pocisk[i].setFillColor(sf::Color::White);
		pocisk[i].setOrigin(7.5,0);
	}
	while (plansza.isOpen())
	{
		plansza.clear(sf::Color::Black);
		czas_gry = zegar_gry.getElapsedTime();
		sekundy_gry = czas_gry.asSeconds();
		if (20 - sekundy_gry > 0 && t!=10)
		{
			tekst = std::to_string(20 - sekundy_gry);
			tekst.resize(tekst.size() - 4);
			licznik.setString(tekst);
		}
		else if(t!=10)
			plansza.draw(komunikat_p);
		czas = podskok.getElapsedTime();
		sekundy = czas.asSeconds();
		czas_boki = na_boki.getElapsedTime();
		sekundy_boki = czas_boki.asSeconds();
		postac.setPosition(gracz.x, gracz.y);
		podloga.setPosition(0,h);
		sf::Event event;
		while (plansza.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				plansza.close();
				return main();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape) {
					plansza.close();
					return main();
				}
				else if (event.key.code == sf::Keyboard::F1)
					nowa_gra(gracz,przeciwnicy,wrogowie,postac,t,zegar_gry, pocisk_p);
				else if (20 - sekundy_gry > 0)
				{
					b = a - 1;
					if (b == -1)
						b = 49;
					if (czas_p.asSeconds() - sekundy_p[b] > 0.2)
						ruch(event, gracz, czas_skoku, sekundy, strzal[a], bok);
					else if(event.key.code != sf::Keyboard::Space)
						ruch(event, gracz, czas_skoku, sekundy, strzal[a], bok);
				}
			}
			else if (event.type == sf::Event::KeyReleased)
				stop(event,gracz,bok);

		}
		if ((gracz.x < 240 || gracz.x > 350) && !gracz.vy && gracz.y != h - 0.05 * h - 30)
		{
			czas_skoku = sekundy;
			gracz.vy = -0.01;
		}

		else if (gracz.vy && gracz.y - (gracz.vy - 2 * (sekundy - czas_skoku)) < 500 && gracz.x>240 && gracz.x<350)
			gracz.y -= gracz.vy- 2 * (sekundy - czas_skoku);
			
		else if (gracz.vy && gracz.x > 240 && gracz.x < 350 && gracz.y<500)
		{
			gracz.y = 470;
			podskok.restart();
			czas = podskok.getElapsedTime();
			sekundy = czas.asSeconds();
			czas_skoku = 0;
			gracz.vy = 0;
		}

		else if(gracz.vy && gracz.y - (gracz.vy - 2 * (sekundy - czas_skoku)) < h - 0.05 * h - 30)
			gracz.y -= gracz.vy - 2 * (sekundy - czas_skoku);

		else if(gracz.vy)
		{
			gracz.y = h - 0.05 * h - 30;
			podskok.restart();
			czas = podskok.getElapsedTime();
			sekundy= czas.asSeconds();
			czas_skoku = 0;
			gracz.vy = 0;
		}
		if (bok)
		{
			na_boki.restart();
			czas_boki = na_boki.getElapsedTime();
			sekundy_boki = czas_boki.asSeconds();
			bok = false;
			x = gracz.x;
		}
		if (gracz.vx!=0)
		{
			czas_boki = na_boki.getElapsedTime();
			gracz.x= x+czas_boki.asSeconds()*gracz.vx;
		}
		plansza.draw(postac);
		plansza.draw(podloga);
		plansza.draw(platforma);
		if (strzal[a])
		{
			pocisk_p[a].zwrot = gracz.zwrot;
			pocisk_p[a].vx = 300*pocisk_p[a].zwrot;
			pocisk_p[a].x0 = gracz.x+gracz.zwrot*10;
			pocisk_p[a].y0 = gracz.y;
			czas_p = zegar_p.getElapsedTime();
			sekundy_p[a] = czas_p.asSeconds();
			pocisk[a].setPosition(pocisk_p[a].x0, pocisk_p[a].y0);
			strzal[a] = 0;
			a++;
			if (a == 50)
				a = 0;
		}
		czas_p = zegar_p.getElapsedTime();
		for (int i = 0; i < 50; i++)
		{
			pocisk_p[i].x = pocisk_p[i].x0 + (czas_p.asSeconds() - sekundy_p[i]) * pocisk_p[i].vx;
			pocisk[i].setPosition(pocisk_p[i].x, pocisk_p[i].y0);
			if (pocisk_p[i].x> w - 15)
				pocisk_p[i].vx = 0;
			for (int j = 0; j < 10; j++)
			{
				
				if (przeciwnicy[j].x == static_cast<int>(pocisk_p[i].x)+15*pocisk_p[i].zwrot && pocisk_p[i].y0 >= przeciwnicy[j].y-13 && pocisk_p[i].y0 <= przeciwnicy[j].y + 10 )
				{
					przeciwnicy[j].vy = 1;
					przeciwnicy[j].x = -100;
					przeciwnicy[j].y = -100;
					pocisk_p[i].vx = 0;
					pocisk_p[i].x = 0;
					pocisk_p[i].y = 0;
				}
			}
			if (pocisk_p[i].vx!=0)
				plansza.draw(pocisk[i]);
				
		}
		for (int i = 0; i < 10; i++)
		{
			if (!przeciwnicy[i].vy)
				plansza.draw(wrogowie[i]);
			else if(przeciwnicy[i].x!=-15)
			{
				przeciwnicy[i].x = -15;
				przeciwnicy[i].y = -15;
				t++;
			}
				
		}
		if(t==10)
			plansza.draw(komunikat);
		plansza.draw(licznik);
		plansza.display();
	}
}

void ruch(sf::Event event, pozycja& gracz,float &czas,float sekundy,int &strzal,bool &bok)
{
	if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
	{
		if (gracz.vy == 0)
		{
			gracz.vy = 0.35;
			czas = sekundy;
		}
	}
	else if (event.key.code == sf::Keyboard::Space)
	{
		strzal = 1;
	}
	else if ((event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A))
	{
		if (gracz.vx!=-200)
			bok = true;
		gracz.zwrot = -1;
		gracz.vx = -200;
	}
	else if ((event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D))
	{
		if (gracz.vx != 200)
			bok = true;
		gracz.zwrot = 1;
		gracz.vx = 200;
	}
}

void stop(sf::Event event, pozycja& gracz,bool &bok)
{
	if ((event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) && gracz.vx <= 0)
		gracz.vx = 0;
	else if ((event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) && gracz.vx >= 0)
		gracz.vx = 0;
}

void nowa_gra(pozycja &gracz, pozycja* przeciwnicy, sf::CircleShape *wrogowie, sf::RectangleShape &postac,int &t, sf::Clock& zegar_gry,pozycja *pocisk_p)
{
	zegar_gry.restart();
	gracz.x = 400;
	gracz.y=h - 0.05 * h - 30;
	gracz.zwrot = 1;
	postac.setPosition(gracz.x, gracz.y);
	gracz.vy = 0;
	gracz.vx = 0;
	t = 0;
	int o = 50;
	for (int i = 0; i < 10; i++)
	{
		if (i == 5)
			o += 400;
		przeciwnicy[i].vy = 0;
		przeciwnicy[i].x = rand() % 250 + o;
		przeciwnicy[i].y = rand() % 200 + 320;
		if (przeciwnicy[i].x > 230 && przeciwnicy[i].x < 360 && przeciwnicy[i].y>485 && przeciwnicy[i].y < 520)
		{
			i--;
			continue;
		}
		for (int j = 0; j < i; j++)
		{
			if (przeciwnicy[i].x> przeciwnicy[j].x-20 && przeciwnicy[i].x< przeciwnicy[j].x+20 && przeciwnicy[i].y > przeciwnicy[j].y-20 && przeciwnicy[i].y < przeciwnicy[j].y+20)
			{
				i--;
				break;
			}
		}
		wrogowie[i].setPosition(przeciwnicy[i].x, przeciwnicy[i].y);
		wrogowie[i].setFillColor(sf::Color::Red);
		wrogowie[i].setRadius(10);
		wrogowie[i].setOrigin(10, 10);
	}
	for (int i = 0; i < 50; i++)
	{
		pocisk_p[i].vx = 0;
	}
}