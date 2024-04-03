#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "brother.h"
#include "SmallMeteorite.h"
#include "Item.h"
#include "Entity.h"
#include "Player.h"

using namespace std;
class time_and_space
{
	//Scop clasa:
	//In principal o sa gestioneze "jocul", o sa verifice coliziunile, o sa simuleze miscarile etc
private:
	player player1 = { {25,25},20,200 };
	//cout << player1.speed;
	vector <SmallMeteorite> v;
	vector <entity> entitati;
    int p[101][101] = { 0 };
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
							punct.x += 5;
						if (punct.y < 10)
							punct.y += 5;
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
							int speed = generate(10, 30);
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
	//int l = 3;
	//int r = 40;
	srand(static_cast<unsigned int>(time(nullptr)));
	zx.start();
	return 0;
}
