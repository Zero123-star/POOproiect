//#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include "brother.h"
#include "Item.h"
#include "SmallMeteorite.h"
class player
{
	//Cum va functiona lower_right_corner?
	//Practic forma navei spatiale va avea mereu forma aceasta:
	//		****
	//	  ********
	//	  ********
	//		***p
	// Unde coordonatele punctului p vor fi puse lower_right_corner(si evident p face parte din nava)
	//De ce forma aceasta? In principal ca poate fi vazuta ca si 6 meteoriti uniti, asa ca se poate repeta codul de verificare coliziuni
	//Momentan voi abandona ideea in care utilizatorul poate sa dea o forma proprie navei
	//De ce?
	//Mult mai usor de scris codul, verificari de coliziuni, memorare, etc 
	//In schimb voi pune mai multe forme predefinite in viitor, pe care playerul le va putea alege 
protected:
	point lower_right_corner;
	int hp;
	int speed;
	std::vector <item> inventar;
public:
	player(point c = { 0,0 }, int hp = 100, int speed = 30) : lower_right_corner(c), hp(hp), speed(speed) {};
	~player()
	{
		//cout << b[0].x << ' ' << b[0].y << ' ' << hp << ' ' << speed;
	}
	friend std::istream& operator>>(std::istream& is, player& pl)
	{
		std::cout << "SELECTEAZA COORDONATELE TALE XY. VALORILE POT FI INTRE (10,10) SI (90,40)";
		int ok = 1;
		int x, y;
		while (ok)
		{
			//int x, y;
			std::cin >> x >> y;
			if (x < 10 || x>90 || y < 10 || y>40)
				std::cout << "EROARE! TE ROG RESPECTA LIMITELE!";
			else
			{
				std::cout << "AU FOST PUSE COORDONATELE: X=" << x << " Y=" << y;
				ok = 0;
			};
		};
		pl.lower_right_corner = { x,y };
		std::cout << '\n' << "SELECTEAZA HP SI SPEED:" << '\n' << "HP:";
		std::cin >> pl.hp;
		std::cout << " SPEED:";
		std::cin >> pl.speed;

		return is;
	}
	friend std::ostream& operator<<(std::ostream& os, const player& pl)
	{
		std::cout << "NAVA TA SE GASESTE LA COORDONATELE X=" << pl.lower_right_corner.x << " Y=" << pl.lower_right_corner.y;
		std::cout << '\n' << "NAVA TA ARE IN PREZENT " << pl.hp << " HP RAMAS SI VITEZA MAXIMA " << pl.speed;
		std::cout << '\n' << "NAVA TA ARE URMATORUL INVENTAR: ";
		pl.ShowInventory();

		return os;
	}
	player& movement(const point direction)
	{
		this->lower_right_corner.x = this->lower_right_corner.x + direction.x;
		this->lower_right_corner.y = this->lower_right_corner.y + direction.y;
		return *this;
	}
	int GetHealthStats() const
	{
		return hp;
	}
	int GetSpeedStats() const
	{
		return speed;
	}
	point GetCoordinates() const
	{
		return lower_right_corner;
	}
	player& adauga_item(const item& it)
	{
		std::string p2 = it.GetName();
		for (auto i = inventar.begin(); i != inventar.end(); i++)
		{
			//Daca am itemul in inventar ii maresc nr de folosiri
			std::string p1 = i->GetName();
			if (p1.compare(p2) == 0)
			{
				*i += it;
				return *this;
			}

		}
		//Daca inca nu am itemul in inventar il adaug
		inventar.push_back(it);
		return *this;

	}
	player& Foloseste_Item(const std::string Tip)
	{
		if (!(Tip.compare("Generator") == 0 || Tip.compare("Scut") == 0))
		{
			std::cout << "Eroare! Nu exista un asemenea obiect";
			return *this;
		}
		else
		{
			for (auto i = inventar.begin(); i != inventar.end(); i++)
			{
				std::string p = i->GetName();
				if (Tip.compare(p) == 0)
				{
					int val;
					val = i->FolosesteItem();
					if (val != -1)
					{
						if (Tip.compare("Generator") == 0)
							this->speed += val;
						else
							this->hp += val;
					}
					return *this;
					break;
				};
			}
		}
		std::cout << "Eroare! Nu am avut nici un obiect de tipul cerut";
		return *this;
	}
	player& meteorite_colission(const SmallMeteorite& c)
	{
		//Nu merge const la getterul de damage 
		// Nvm fixed it, adauga const dupa functie
			 //std::cout << c.GiveDamagePlease();
		int damage = c.GiveDamagePlease();
		this->hp -= damage;
		return *this;
	};
	std::vector <item>& GetInventory()
	{
		return inventar;
	}
	void ShowInventory()const
	{
		if (inventar.empty())
			std::cout << "NU AVEM NIMIC IN INVENTAR" << '\n';
		else
			for (auto i = inventar.begin(); i != inventar.end(); i++)
			{
				std::cout << i->GetName() << ":" << '\n';
				std::cout << "Folosiri:" << i->GetUses() << '\n' << "Cooldown:" << i->GetCooldown() << '\n';

			}


	}
	player& Set_Health(int hp)
	{
		this->hp = hp;
		return *this;
	}
};
#endif