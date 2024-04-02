#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "brother.h"
#include "generator_random.h"
using namespace std;
class SmallMeteorite
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
	};*/
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
class item {
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
class entity
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
};
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
	vector <item> inventar;
public:
	player(point c = { 0,0 }, int hp = 100, int speed = 30) : lower_right_corner(c), hp(hp), speed(speed) {};
	~player()
	{
		//cout << b[0].x << ' ' << b[0].y << ' ' << hp << ' ' << speed;
	}
	friend std::istream& operator>>(std::istream& is, player& pl)
	{
		cout << "SELECTEAZA COORDONATELE TALE XY. VALORILE POT FI INTRE (10,10) SI (90,40)";
		int ok = 1;
		int x, y;
		while (ok)
		{
			//int x, y;
			cin >> x >> y;
			if (x < 10 || x>90 || y < 10 || y>40)
				cout << "EROARE! TE ROG RESPECTA LIMITELE!";
			else
			{
				cout << "AU FOST PUSE COORDONATELE: X=" << x << " Y=" << y;
				ok = 0;
			};
		};
		pl.lower_right_corner = { x,y };
		cout <<'\n'<< "SELECTEAZA HP SI SPEED:"<<'\n'<<"HP:";
		cin >> pl.hp;
		cout << " SPEED:";
		cin >> pl.speed;

		return is;
	}
	friend std::ostream& operator<<(std::ostream& os, const player& pl)
	{
		cout << "NAVA TA SE GASESTE LA COORDONATELE X=" << pl.lower_right_corner.x << " Y=" << pl.lower_right_corner.y;
		cout << '\n' << "NAVA TA ARE IN PREZENT " << pl.hp << " HP RAMAS SI VITEZA MAXIMA " << pl.speed;
		cout << '\n' << "NAVA TA ARE URMATORUL INVENTAR: ";
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
		string p2 = it.GetName();
		for (auto i = inventar.begin(); i != inventar.end(); i++)
		{
			//Daca am itemul in inventar ii maresc nr de folosiri
			string p1 = i->GetName();
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
	player& Foloseste_Item(const string Tip)
	{
		if (!(Tip.compare("Generator") == 0 || Tip.compare("Scut") == 0))
		{
			cout << "Eroare! Nu exista un asemenea obiect";
			return *this;
		}
		else
		{
			for (auto i = inventar.begin(); i != inventar.end(); i++)
			{
				string p = i->GetName();
				if (Tip.compare(p) == 0)
				{
					int val;
					val=i->FolosesteItem();
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
		cout << "Eroare! Nu am avut nici un obiect de tipul cerut"; 
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
	vector <item>& GetInventory()
	{
		return inventar;
	}
	void ShowInventory()const
	{
		if (inventar.empty())
			cout << "NU AVEM NIMIC IN INVENTAR" << '\n';
		else
			for (auto i = inventar.begin(); i != inventar.end(); i++)
			{
				cout << i->GetName() << ":"<<'\n';
				cout <<"Folosiri:"<<i->GetUses()<<'\n'<< "Cooldown:"<<i->GetCooldown()<<'\n';

			}


	}
	player& Set_Health(int hp)
	{
		this->hp = hp;
		return *this;
	}
};
int check_collission(point m, point n)
{
	// Returneaza 1 daca este o coliziune intre 2 elemente de tip patrat, m si n reprezinta coordonatele de la coltul stanga jos 
	// Returneaza 0 daca nu gaseste o coliziune

	if (n.y == m.y && (n.x == m.x || m.x == n.x - 1 || m.x == n.x + 1))
		return 1;
	else
		if ((n.x == m.x || m.x == n.x - 1 || m.x == n.x + 1) && (m.y == n.y - 1 || m.y == n.y + 1))
			return 1;
	return 0;
}
int generate(int l, int r) 
{
	int RNG = rand() % (r - l + 1) + l;
	return RNG;
}
class time_and_space: public player
{
	//Scop clasa:
	//In principal o sa gestioneze "jocul", o sa verifice coliziunile, o sa simuleze miscarile etc
private:
	player player1 = { {25,25},20,200 };
	//cout << player1.speed;
	vector <SmallMeteorite> v;
	vector <entity> entitati;
	int p[101][101] = { 0 };
	void inserare_patrat(const point& c, int forma)
	{
		//Insereaza un patrat in matricea data
		//Forma va indica tipul patratului
		//1-> meteorit
		//2-> nava player
		//3-> entitate(adica inventar)
		//pointul reprezinta punctul din dreapta jos
		//**
		//*p 
		//point c-> are coordonatele lui p
		p[c.y][c.x] = forma;
		p[c.y - 1][c.x] = forma;
		p[c.y][c.x - 1] = forma;
		p[c.y - 1][c.x - 1] = forma;
	}
	int Verificare_Coliziune_Player_Patrat(point& CoordPlayer, point CoordPatrat)
	{
		//Nimica interesant legat de codul asta
		// Verifica coliziunea intre player si un element ce are forma de patrat
		// In prezent e folosita doar de coliziunile meteorit-player si entitate(item)-player
		// Scriu functia asta doar ca sa mai scap de cod-bloating
		// return 1 -> am coliziune. Return 0 nu am 
		if (check_collission(CoordPlayer += point{ 0, 0 }, CoordPatrat) == 1)
			return 1;
		else
		if (check_collission(CoordPlayer += point{ -2, 0 }, CoordPatrat) == 1)
			return 1;
		else
		if (check_collission(CoordPlayer += point{ 0,2 }, CoordPatrat) == 1)
			return 1;
		else
		if (check_collission(CoordPlayer += point{ 2,0 }, CoordPatrat) == 1)
			return 1;
		else
		if (check_collission(CoordPlayer += point{ 2,-1 }, CoordPatrat) == 1)
			return 1;
		else
		if (check_collission(CoordPlayer += point{ -6,0 }, CoordPatrat) == 1)
			return 1;
		return 0;
	}
	void updatare_matrice()
	{
		//cout << player1.speed;
		//Inserare cordonate meteoriti
		for (auto i = v.begin(); i != v.end(); i++)
		{
			inserare_patrat(i->GiveCoordinates(), 1);
		}

		//Inserare coordonate player
		point z = player1.GetCoordinates();
		inserare_patrat(z, 2);
		inserare_patrat(z += point{ -2, 0 }, 2);
		inserare_patrat(z += point{ 0,2 }, 2);
		inserare_patrat(z += point{ 2,0 }, 2);
		inserare_patrat(z += point{ 2,-1 }, 2);
		inserare_patrat(z += point{ -6,0 }, 2);
		//Inserare coordonate player 


		//Inserare coordonate entitati 
		for (auto i = entitati.begin(); i != entitati.end(); i++)
		{
			inserare_patrat(i->GiveCoordinates(), 3);
		}
	}
	void afisarematrice()
	{
		//Afiseaza matricea
		//Am decis momentan sa o am de 51 linii si 100 de coloane, ca sa fie vizibila

		for (int i = 51; i >=1; i--)
		{
			for (int j = 1; j <= 100; j++)
			{
				cout << p[i][j];
				p[i][j] = 0;
			}
			//cout << "100";
			cout << '\n';
		}
	};
	void Turn_Has_Passed()
	{
		//Simulez o tura(meteoritii o sa evolueze respectand traiectoria, entitatile care si au consumat toate turele dispar, cooldown-ul de la iteme scade) etc
		//Elimin elementele care au iesit din mapa(au coordonate negative/>100x100
		for (auto i = v.begin(); i != v.end(); i++)
		{
			i->evolve();
			point aux = i->GiveCoordinates();
			if (aux.x < 0 || aux.y < 0 || aux.x>100 || aux.y>100)
			{
				cout << "UN METEORIT A IESIT DE PE HARTA SI A FOST STERS" << '\n';
				i = v.erase(i);
				if (i == v.end())
					break;
			}
		}
		for (auto i = entitati.begin(); i != entitati.end(); i++)
		{
			i->evolve();
			if (i->GiveTurn() <= -1)
			{
				cout << "O entitate a expirat" << '\n';
				i = entitati.erase(i);
				if (i == entitati.end())
					break;
			}
		}
		//cout << "NDS";
		for (auto i = player1.GetInventory().begin(); i != player1.GetInventory().end(); i++)
		{
			i->ScadeCooldown();
		}
	}
	void eliminate_metorite_collissions()
	{
		//In codul acesta calculez toate coliziunile
		//Prioritate vor avea coliziunile intre meteoriti
		//Apoi se verifica coliziunile cu nava player
		//Si in final coliziunile player entitati(adica daca playerul poate colecta entitatea)
		//TODO: Separeaza coliziunile in diferite functii
		//Prea multe linii de cod 
		if(!v.empty())
		for (auto i = v.begin(); i != v.end() - 1; )
		{
			point c = i->GiveCoordinates();
			auto j = i + 1;
			while (j != v.end()) {
				point b = j->GiveCoordinates();
				if (check_collission(c, b) == 1)
				{
					cout << "COLIZIUNE DETECTATA INTRE METEORITUL " << c.x << ' ' << c.y << " SI METEORITUL " << b.x << ' ' << b.y << " IN FINAL ";
					if (*i - *j == 1)
					{
						cout << "METEORITUL " << b.x << ' ' << b.y << " A FOST DISTRUS" << '\n';
						j = v.erase(j);
					}
					else if (*i - *j == 0)
					{
						cout << "METEORITUL " << c.x << ' ' << c.y << " A FOST DISTRUS" << '\n';
						i = v.erase(i);
						break;
					}
					else {
						cout << "AMANDOI METEORITI AU FOST DISTRUSI" << '\n';
						i = v.erase(i);
						j = v.erase(j);
						if (j == v.end())
							break;
					}
				}
				else
				{
					j++;
				}
			}
			if (i != v.end())
				++i;
		}
		/*
		Cod vechi de verificare coliziuni meteoriti
		Prea buguit, memory leakuri, nu facea corect treaba
		Codul nou ar trebui sa fie corect

		for (auto i = v.begin(); i != v.end() - 1; i++)
		{
			//int nr2 = nr+1;
			point c = i->GiveCoordinates();
			for (auto j = i+1; j != v.end();j++)
			{
				point b = j->GiveCoordinates();
				if (check_collission(c, b) == 1)
				{
					cout << "COLIZIUNE DETECTATA INTRE " << c.x << ' ' << c.y << " SI " << b.x << ' ' << b.y << '\n';
					if (*i - *j == 1)
					{
						j = v.erase(j);
						//point xc = j->GiveCoordinates();
						//cout << xc.x << ' ' << xc.y<<"SI";
						//xc = (j - 1)->GiveCoordinates();
						//cout << xc.x << ' ' << xc.y;
						j = j - 1;
						cout << "A FOST DISTRUS METEORITUL CU COORDONATELE " << b.x << ' ' << b.y << '\n';
						if (j == v.end())
							break;

						//j = i+1;
					}
					else
						if (*i - *j == 0)
						{
							i = v.erase(i);
							cout << "A FOST DISTRUS METEORITUL CU COORDONATELE " << c.x << ' ' << c.y << '\n';
						}
						else
						{

							i = v.erase(i);
							j = v.erase(j);
							cout << "AU FOST DISTRUSI AMANDOI METEORITI" << '\n';
							if (j == v.end())
								break;
						}
					//nr2++;
				}
			}
			//nr++;
		}*/
		if(!v.empty())
		for (auto i = v.begin(); i != v.end();)
		{
			/// <summary>
			/// Arata oribil
			/// Probabil o sa fac o functie separata
			/// Problema eficienta. Poate sa fac o verificare separata cu un fel de colizion box masiv.
			/// </summary>
			point z = this->player1.GetCoordinates();
			if (Verificare_Coliziune_Player_Patrat(z, i->GiveCoordinates()) == 1)
			{
				this->player1.meteorite_colission(*i);
				cout << "COLIZIUNE PLAYER SI METEORIT!!!" << '\n';
				i = v.erase(i);
				if (i == v.end())
					break;
			}
			else
			{
				i++;
			}
		}
		if(!entitati.empty())
		for (auto i = entitati.begin(); i != entitati.end();)
		{
			point z = this->player1.GetCoordinates();
			if (Verificare_Coliziune_Player_Patrat(z, i->GiveCoordinates()) == 1)
			{
				cout << "Am adaugat o entitate de tip " << i->GiveName() << '\n';
				item p{ i->GiveName() };
				this->player1.adauga_item(p);
				i = entitati.erase(i);
				if (i == entitati.end())
					break;
			}
			else
			{
				++i;
			}
		}
	}
	void gimme_coordinates()
	{
		//Afiseaza toate coordonatele entitatilor/meteoritilor/playerului de pe mapa
		cout << "COORDONATE METEORITI:" << '\n';
		for (auto i = v.begin(); i != v.end(); i++)
		{
			point c = i->GiveCoordinates();
			cout << c.x << ' ' << c.y << '\n';
		}
	}
	void aux()
	{
		player1.ShowInventory();
		player1.Foloseste_Item("Generator");
	}
public:
	void start()
	{
		//"Jocul" propriu zis, gestioneaza cam totul
		cout << "THE COMBAT SIMULATION HAS BEGUN...." << '\n' << "ANALYZING..." << "CHANGING DEFAULT LANGUAGE TO ROMANIAN" << '\n';
		cout<<"TE ROG SELECTEAZA DACA VREI SA PILOTEZI O NAVA CUSTOM SAU O NAVA OBISNUITA:" << '\n';
		cout << "APASA 1 PENTRU CUSTOM... ALTFEL ORICE CHEIE PENTRU NAVA OBISNUITA"<<'\n';
		char p;
		cin >> p;
		if (p == '1')
		{
			cout << "AI ALES NAVA CUSTOM...";
			cin >> player1;
		}
		else
		{
			cout << "AI ALES NAVA OBISNUITA... ";
				//cout << player1;
		}
		cout << player1;
		int movement_action=1;
		int simulare_ended_by_player = 0;
		//updatare_matrice();
		while (player1.GetHealthStats()>0)//Jocul continua cat timp playerul este in viata 
		{

			cout << '\n' << "HP=" << player1.GetHealthStats();
			if (movement_action == 1)
				cout << " INCA POTI SA TE MISTI";
			else
				cout << " TREBUIE SA TERMINI TURA PENTRU A TE PUTEA MISCA DIN NOU!";
			cout << '\n' << "CE DORESTI SA FACI?"<<'\n';
			cout << "TASTEAZA 1 PENTRU A AFISA HARTA, TASTEAZA 2 PENTRU A TE MISCA INTR-O DIRECTIE, TASTEAZA 3 PENTRU A AFISA STATUTUL NAVEI" << '\n';
			cout << "TASTEAZA 4 PENTRU A FOLOSI UN ITEM DIN INVENTAR, TASTEAZA 5 PENTRU A TRECE TURA"<<'\n'<<"TASTEAZA 6 PENTRU A SPAWNA ENTITATI, TASTEAZA 7 PENTRU A FINALIZA SIMULAREA";
			cout << '\n' << '\n';
			char p;
			cin >> p;
			if (p == '1')
			{
				updatare_matrice();
				afisarematrice();
			}
			else 
				if (p == '2')
				{
					if (movement_action == 1)
					{
						int ok;
						cout << "IN CE DIRECTIE VREI SA MERGI?" << '\n';
						cout << "TASTEAZA W PENTRU SUS. TASTEAZA D PENTRU DREAPTA. TASTEAZA A PENTRU STANGA. TASTEAZA S PENTRU JOS." << '\n';
						//cout << "TASTEAZA WA PENTRU STANGA SUS. TASTEAZA WD PENTRU DREAPTA SUS. TASTEAZA SD PENTRU DREAPTA JOS. TASTEAZA WA PENTRU STANGA JOS";
						string directie;

						point sens = { 0,0 };
						ok = 1;
						while (ok)
						{
							cin >> directie;
							ok = 0;
							if (directie.compare("w") == 0)
							{
								sens.x = 0;
								sens.y = 1;
							}
							else if (directie.compare("d") == 0)
							{
								sens.x = 1;
								sens.y = 0;
							}
							else if (directie.compare("s") == 0)
							{
								sens.x = 0;
								sens.y = -1;

							}
							else if (directie.compare("a") == 0)
							{
								sens.x = -1;
								sens.y = 0;
							}
							else
							{
								ok = 1;
								cout << "EROARE! TE ROG TASTEAZA CU LITERE MICI DIRECTIA!" << '\n';
							}
						}
						cout << "CATE SPATII VREI SA MERGI?" << '\n';
						cout << "TINE MINTE CA POTI SA MERGI DOAR " << player1.GetSpeedStats() / 10 << " SPATII";
						cout << '\n' << "SPATII DE PARCURS:";
						int xc;
						ok = 1;
						while (ok)
						{
							cin >> xc;
							if (xc<0 || xc>player1.GetSpeedStats() / 10)
								cout << "EROARE! POTI SA MERGI CEL PUTIN 0 SI CEL MULT" << player1.GetSpeedStats() / 10 << " SPATII" << '\n';
							else
							{
								sens.x *= xc;
								sens.y *= xc;
								ok = 0;
							}
						}
						cout << sens.x << ' ' << sens.y << ' ';
						player1.movement(sens);
						eliminate_metorite_collissions();
						movement_action = 0;
						//Turn_Has_Passed();
						cout << "PERFECT, AM TERMINAT DE MUTAT NAVA";
					}
					else
						cout << "NU TE MAI POTI MISCA IN TURA ACEASTA. TREBUIE SA TRECI TURA INTAI.";
				}
			else 
				if(p=='3')
				{
					cout << player1;
				}
			else
				if(p=='4')
				{
					vector<item>p = player1.GetInventory();
					if (p.empty())
						cout << "EROARE! NU AI NIMICA IN INVENTAR";
					else
						cout << "INVENTARUL TAU CONTINE URMATOARELE LUCRURI:" << '\n';
					player1.ShowInventory();
					cout << '\n' << "CE DORESTI SA FOLOSESTI?";
						string val;
					cin >> val;
					player1.Foloseste_Item(val);
				}
			else
				if (p == '5')
					{
						movement_action = 1;
						Turn_Has_Passed();
						eliminate_metorite_collissions();
				    }
			else
				if (p == '6')
				{
					//string confirmare;
					cout << "VOI GENERA ENTITATI ACUM SI LE VOI PLASA PE HARTA. INTAI VOI CALCULA CATE ENTITATI SA PUN..."<<'\n';
						int random = generate(1, 30);
						cout << "VOI PUNE..."<<random <<" ENTITATI!"<<'\n';
						int goodguys=generate(1, random / 8 + 1);
						cout << "DIN ACESTEA VOR FI " << goodguys << " ENTITATI DE TIP INVENTAR SI " << random - goodguys << " ENTITATI DE TIP METEORIT!" << '\n';
						cout << "VOI GENERA ACUM ELEMENTELE...   ";
						
						point punct = { 0, 0 };
						punct = player1.GetCoordinates();
						int delimitarex = 50;
						int delimitarey = 25;
						if (punct.x > delimitarex)
							swap(punct.x, delimitarex);
						if (punct.y > delimitarey)
							swap(punct.y, delimitarey);
						if (punct.x < 10)
							punct.x + 5;
						if (punct.y < 10)
							punct.y + 5;
						for (int i = 1; i < (random - goodguys) / 2; i++)
						{
							int x = generate(1, punct.x - 5);
							int y = generate(1, punct.y - 5);
							int trajectoryx = generate(0, 1);
							int negative = generate(0, 1);
							int health = generate(10, 30);
							int dmg = generate(5, 10);
							int speed = generate(10, 30);
							if (negative == 1)
								trajectoryx *= -1;
							//dmg speed health coord trajectory
							//cout << dmg << ' ' << speed << ' ' << health << ' ' << x << ' ' << y << ' ' << trajectoryx << '\n';
							v.push_back({ dmg, speed, health, { x,y }, { trajectoryx,-1 } });
						}
						for (int i = 1; i < (random - goodguys) / 2; i++)
						{
							int x = generate(delimitarex, 90);
							int y = generate(delimitarey, 45);
							int trajectoryx = generate(0, 1);
							int negative = generate(0, 1);
							int health = generate(10, 30);
							int dmg = generate(5, 10);
								if (negative == 1)
									trajectoryx *= -1;
							v.push_back({ dmg, speed, health, { x,y }, { trajectoryx,-1 } });
						}
						cout << "AM TERMINAT DE PUS METEORITII!   ";
						for (int i = 1; i <= goodguys; i++)
						{
							//name cord ture
							int x = generate(1, 100);
							int y = generate(1, 50);
							int type = generate(0, 1);
							string m;
							int ture=generate(3,10);
							if (type == 0)
								m = "Generator";
							else
								m = "Scut";
							entitati.push_back({ m,{x,y},ture });
						}
						cout << "AM TERMINAT DE PUS ENTITATILE!";
				}
			else
				if (p == '7')
				{
					player1.Set_Health(0);
					simulare_ended_by_player = 1;
				}
		};
		if (simulare_ended_by_player == 1)
			cout << "SIMULAREA A FOST INCHISA CU SUCCES!";
		else
			cout << "BETTER LUCK NEXT TIME!";
	}
}zx;


int main()
{
	int l = 3;
	int r = 40;
	srand(static_cast<unsigned int>(time(nullptr)));
	zx.start();
	return 0;
}
