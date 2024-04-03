//#pragma once
//Aici o sa arunc cod nefolositor
/*class item {
protected:
	string name;
	int uses;
	int cooldown;
public:
	//item
	item(string nume)
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
	string GetName() const
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
			cout << "Eroare! Cooldown";
			return -1;
		}
		else
			if (this->name.compare("Generator")==0)
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
		if(this->cooldown>0)
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
*/
/* class SmallMeteorite
{
	//Memoreaza in lower_left_extremity coordonatele la punctul din dreapta jos(Da ar fi trebuit sa fie lower_right_extremity,dar cand am realizat asta aveam multe linii scrise)
	//Astfel forma unui meteorit va fi urmatoarea:
	//	**
	//	*p
	// Cu coordonatele punctului p fiind puse in lower_left_extremity
	//Trajectory: Formata din puncte i1,j1 -> La fiecare pas de timp meteoritul(x,y) o sa evolueze in (x+i1,y+j1)
	//Idee evolutie cu speed? Evolutia va putea fi facuta impreuna cu speed. Daca speed <10, nu se face evolutie dar speed creste cu speed.
	//Altfel o sa mearga [x*speed/10]+[y*speed/10]. Asa pot include si variabile intregi, si o traiectorie mai mult focusata pe x decat pe y si altele
	//De gandit: Mai fac o alta variabila numita distanta parcursa poate? Evident daca speed<10 nu o sa se faca nici o miscare.
	//Note: In prezent distance_Travelled nu foloseste la nimic
	int dmg;
	int speed;
	int health;
	point lower_left_extremity;
	point trajectory; // = { 0,0 };
public:
	SmallMeteorite(int dmg = 5, int speed = 10, int health = 20, point c = { 0,89 }, point trajectory = { 0,0 }) : dmg(dmg), speed(speed), health(health), lower_left_extremity(c), trajectory(trajectory) {};
	int operator-(const SmallMeteorite& z)//Coliziunea intre 2 meteoriti. Returneaza 1 daca primul meteorit supravietuieste, 2 daca al doilea, 0 daca amandoi se autodistrug
	{
		//NOTE: CODUL ASTA !!NU!! VERIFICA DACA E COLIZIUNE, DOAR CALCULEAZA CUM AR FI COLIZIUNEA!!!
		//cout << this->health << " " << z.dmg << ' ';
		int h1 = this->health / z.dmg;
		int h2 = z.health / this->dmg;
		//cout << h1 << " " << h2 << " ";
		if (h1 > h2)
			return 1;
		else
			if (h1 < h1)
				return 2;
		return 0;
	}
	/*int colizion(const SmallMeteorite& z) const
	{
		//RETURNEAZA TRUE DACA EXISTA O COLIZIUNE CU METEORITUL Z
		//ASUM CA METEORITUL Z ESTE TOT DE TIPUL SMALL METEORITE
		//!!NU!! VERIFICA COLIZIUNEA INTRE METEORIT SI PLAYER
				//cout << z.lower_left_extremity.x;
		if (this->lower_left_extremity.x)
			return true;
		return false;
	};
	void evolve()
	{
		//this->distance_travelled += this->speed;
		//O sa trebuiasca de revizuit putin logica codului, aici sau la coliziuni, ~candva
		//De ce?
		//Pentru ca daca avem 2 meteoriti fix unul langa altul, care daca ar avea amandoi speed 10 s ar intersecta, dar daca unu ar avea speed 50 nu ar mai fi nici o intersectie
		//Cum fixez asta?
		//Inca nu m am decis, poate ar fi posibilitatea de a face intai evolutiile cu meteoritii cei mai rapizi, in sensul in care
		// Daca un meteorit ar avea speed 30, as face 2 evolutii de speed 10
		// Si apoi evolutia finala de speed 10 e facuta impreuna cu toti meteoritii
		// Dar inca nu sunt sigur
		cout << this->lower_left_extremity.y << "+" << this->trajectory.y;
		this->lower_left_extremity.x = this->lower_left_extremity.x + this->trajectory.x * this->speed / 10;
		this->lower_left_extremity.y = this->lower_left_extremity.y + this->trajectory.y * this->speed / 10;
		cout << "=" << this->lower_left_extremity.y << '\n';

	}
	point GiveCoordinates() const
	{
		return  { lower_left_extremity.x, lower_left_extremity.y };
	}
	int GiveDamagePlease() const
	{
		return dmg;
	};
};
/*class entity
{
	//Scop?
	//Momentan o sa fie facut doar pt a spawna iteme pe harta. Poate in viitor o sa aiba alte folosinte
	//Asum din nou forma basic de patrat, coordonatele reprezinta punctul din stanga jos.
protected:
	string name;
	point coord;
	int ture; //Cat timp se va afla pe harta, o tura este consumata la fiecare evolutie de timp din matrice
public:
	entity(string m = "MSD", point c = { 3,4 }, int ture = 2) :name(m), coord(c), ture(ture) {};
	point GiveCoordinates()
	{
		return { coord.x, coord.y };

	}
	string GiveName() const
	{
		return name;
	}
	int GiveTurn()const
	{
		return ture;
	}
	friend std::ostream& operator<<(std::ostream& os, const entity& en)
	{
		cout << "Itemul de tip " << en.name << " se gaseste la coordonatele " << en.coord.x << ' ' << en.coord.y << " si mai are timp ramas " << en.ture;

	};
	friend std::istream& operator>>(std::istream& is, entity& en)
	{
		cout << "Itemul tau este de tip? (Generator/Scut) ";
		int ok = 0;
		string nume;
		while (ok == 0)
		{

			cin >> nume;
			if (nume.compare("Generator") == 0 || nume.compare("Scut") == 0)
				ok = 1;
			if (ok == 0)
				cout << "Eroare! Scrie 'Generator' sau 'Scut' ";
		};

		en.name.assign(nume);
		if (en.name.compare("Generator") == 0)
			en.ture = 3;
		else
			en.ture = 4;

		cout << '\n' << "Care sunt coordonatele sale? (XY) ";
		cin >> en.coord.x >> en.coord.y;
		return is;
	};
	entity& evolve()
	{
		this->ture--;
		return *this;
	}
};*/
*/
#ifndef GENERATOR_RANDOM_H
#define GENERATOR_RANDOM_H
#endif