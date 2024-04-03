#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include <iostream>
#include <string>
#include "brother.h"
#include "Item.h"
class entity
{
	//Scop?
	//Momentan o sa fie facut doar pt a spawna iteme pe harta. Poate in viitor o sa aiba alte folosinte
	//Asum din nou forma basic de patrat, coordonatele reprezinta punctul din stanga jos. 
protected:
	std::string name;
	point coord;
	int ture; //Cat timp se va afla pe harta, o tura este consumata la fiecare evolutie de timp din matrice
public:
	entity(std::string m = "MSD", point c = { 3,4 }, int ture = 2) :name(m), coord(c), ture(ture) {};
	point GiveCoordinates()
	{
		return { coord.x, coord.y };

	}
	std::string GiveName() const
	{
		return name;
	}
	int GiveTurn()const
	{
		return ture;
	}
	friend std::ostream& operator<<(std::ostream& os, const entity& en)
	{
		std::cout << "Itemul de tip " << en.name << " se gaseste la coordonatele " << en.coord.x << ' ' << en.coord.y << " si mai are timp ramas " << en.ture;

	};
	friend std::istream& operator>>(std::istream& is, entity& en)
	{
		std::cout << "Itemul tau este de tip? (Generator/Scut) ";
		int ok = 0;
		std::string nume;
		while (ok == 0)
		{

			std::cin >> nume;
			if (nume.compare("Generator") == 0 || nume.compare("Scut") == 0)
				ok = 1;
			if (ok == 0)
				std::cout << "Eroare! Scrie 'Generator' sau 'Scut' ";
		};

		en.name.assign(nume);
		if (en.name.compare("Generator") == 0)
			en.ture = 3;
		else
			en.ture = 4;

		std::cout << '\n' << "Care sunt coordonatele sale? (XY) ";
		std::cin >> en.coord.x >> en.coord.y;
		return is;
	};
	entity& evolve()
	{
		this->ture--;
		return *this;
	}
};
#endif
