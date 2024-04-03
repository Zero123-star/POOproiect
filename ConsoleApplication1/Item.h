#ifndef ITEM_H
#define ITEM_H
#include <iostream>
#include <string>
#include "brother.h"
class item {
protected:
	std::string name;
	int uses;
	int cooldown;
public:
	//item 
	item(std::string nume)
	{
		this->name = nume;
		if (this->name.compare("Generator") == 0)
		{
			uses = 1;
			cooldown = 2;
		}
		else
		{
			uses = 3;
			cooldown = 0;
		}

	}
	std::string GetName() const
	{
		return name;
	};
	item operator +=(const item& it)
	{
		this->uses += it.uses;
		return *this;
	}
	int FolosesteItem()
	{
		if (this->cooldown > 0)
		{
			std::cout << "Eroare! Cooldown";
			return -1;
		}
		else
			if (this->name.compare("Generator") == 0)
			{
				this->cooldown = 2;
				this->uses -= 1;
				return 10;

			}
			else
			{
				this->uses -= 1;
				return 40;

			}
	};
	void ScadeCooldown()
	{
		if (this->cooldown > 0)
			this->cooldown -= 1;
	}
	int GetCooldown()const
	{
		return this->cooldown;
	}
	int GetUses()const
	{
		return this->uses;
	}

	~item()
	{};
};
#endif