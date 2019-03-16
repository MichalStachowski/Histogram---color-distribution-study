//POP_2018_01_22_projekt_2_Stachowski_Michal_AIR_1_171570
//Projekt polega na wykonaniu histogramu jasnosci oraz barw podstawowych RGB.
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<iomanip>
using namespace std;

struct obraz
{
	int rozmiar;
	int szerokosc;
	int wysokosc;
	int offset;
	unsigned long long int R_ogolem;	
	unsigned long long int G_ogolem;
	unsigned long long int B_ogolem;
	unsigned long long int wymiar;	
	float tab_jasnosci[256];
	double suma_RGB;
	float procenty[3];
};

//-------Podstawowe informacje wczytanego zdjeecia--------------------
void odczyt_podstawowych_informacji(char nazwa_plik[20], obraz *tab);
void zapis_podstawowych_informacji(obraz *tab);
//--------------------------------------------------------------------

//--------Funkcje odpowiadajace za rozklad RGB-------
void analiza_RGB(char nazwa_plik[20], obraz *tab);
void obliczenia_RGB(obraz *tab);
void histogram_RGB(obraz *tab);
void zapis_histogram_RGB(obraz *tab);
void wykres_RGB(obraz *tab);
void zapis_wykres_RGB(obraz *tab);
//---------------------------------------------------

//--------Funckje odpowiadajace za histogram jasnosci-----------
void czestosc(obraz *tab);
void analiza_jasnosc(char nazwa_plik[20], obraz *tab);
void histogram_jasnosc(char nazwa_plik[20], obraz *tab);
void zapis_histogram_jasnosc(char nazwa_plik[20], obraz *tab);
void wykres_jasnosc(obraz *tab);
void zapis_wykres_jasnosc(obraz *tab);
//--------------------------------------------------------------

//--------Funckje odpowiadajace za histogram skladowych koloru---
void analiza_skladowa(int a, char nazwa_plik[20], obraz* tab);
void wybor_skladowej(obraz* tab, char nazwa_plik[20]);
//---------------------------------------------------------------

//----------Funkcje przedstawiaj¹ce dostepne opcje w programie-------------------------------------------------
void RGB(char nazwa_plik[20], obraz *tab);
void jasnosc(char nazwa_plik[20], obraz *tab);
void skladowa(int a, char nazwa_plik[20], obraz* tab, void histogram_jasnosc(char nazwa_plik[20], obraz *tab));
//--------------------------------------------------------------------------------------------------------------

//--------Funkcje zabezpieczajace program przed bledami wynikacjacymi ze zlych typow wprowadzonych danych-----
void zly_plik();
void zly_wybor_trybu(char nazwa_plik[20], obraz *tab, void wybor_trybu(char nazwa_plik[20], obraz *tab));
//------------------------------------------------------------------------------------------------------------

void wybor_trybu(char nazwa_plik[20], obraz *tab);


int main()
{		
	obraz *tab;
	tab = new obraz[1];	
	
	while (true)
	{
		char nazwa_plik[20];
		cout << "Podaj nazwe pliku, ktory ma zostac odczytany: ";
		cin >> nazwa_plik;
		
		ifstream plik_odczyt;	
		plik_odczyt.open(nazwa_plik, ios::binary);
		
		if(!plik_odczyt)
		{
			while(!plik_odczyt)
			{
				zly_plik();
				break;			
			}	
		}
		else
		{
			odczyt_podstawowych_informacji(nazwa_plik, tab);
			zapis_podstawowych_informacji(tab);
			wybor_trybu(nazwa_plik, tab);
			break;																
		}		
		plik_odczyt.close();
	}
			
	cout << "\nWyniki analizy zostaly zapisane do pliku tekstowego.\nDziekuje za skorzystanie z programu i zachecam do dalszego uzytkowania.";
	system("PAUSE");
	return 0;
}


void odczyt_podstawowych_informacji(char nazwa_plik[20], obraz *tab)
{
	ifstream plik_odczyt;	
	plik_odczyt.open(nazwa_plik, ios::binary);
		
	plik_odczyt.seekg(2);
	plik_odczyt.read(reinterpret_cast<char*>(&tab->rozmiar), 4);
		
	plik_odczyt.seekg(10);
	plik_odczyt.read(reinterpret_cast<char*>(&tab->offset), 4);
				
	plik_odczyt.seekg(18);
	plik_odczyt.read(reinterpret_cast<char*>(&tab->szerokosc), 4);
	plik_odczyt.read(reinterpret_cast<char*>(&tab->wysokosc), 4);
		
	cout << "Podstawowe informacje analizowanego zdjecia: \n";
	cout << " rozmiar: " << tab->rozmiar << " bajtow.\n";
	cout << " szerokosc: " << tab->szerokosc << " pikseli." << endl;
	cout << " wysokosc: " << tab->wysokosc << " pikseli.\n" << endl;
	plik_odczyt.close();	
}


void zapis_podstawowych_informacji(obraz *tab)
{
	ofstream plik_zapis;
	plik_zapis.open("plik.txt", ios::out);
	plik_zapis << "Informacje ogolne zdjecia:\n";
	plik_zapis << " rozmiar: " << tab->rozmiar << " bajtow." << endl;
	plik_zapis << " szerokosc: " << tab->szerokosc << " pikseli." << endl;
	plik_zapis << " wysokosc: " << tab->wysokosc << " pikseli.\n" << endl;
	plik_zapis.close();
}


void analiza_RGB(char nazwa_plik[20], obraz *tab)
{
	unsigned long long int B=0, G=0, R=0;
	ifstream plik_odczyt;
	plik_odczyt.open(nazwa_plik, ios::binary);	
	plik_odczyt.seekg(tab->offset);

	for(int i=0; i<tab->wysokosc; i++)
	{
		for (int j=0; j<tab->szerokosc; j++)
		{
			for (int k=0; k<3; k++)
			{
				if (k==0)
				{
					plik_odczyt.read(reinterpret_cast<char*>(&B), 1);
					tab->B_ogolem = B+tab->B_ogolem;
				}
				else if (k==1)
				{
					plik_odczyt.read(reinterpret_cast<char*>(&G), 1);
					tab->G_ogolem = G+tab->G_ogolem;
				}
				else if (k==2)
				{
					plik_odczyt.read(reinterpret_cast<char*>(&R), 1);
					tab->R_ogolem = R+tab->R_ogolem;
				} 
			}
		}
		if(tab->szerokosc%4==3) plik_odczyt.seekg(3, ios::cur);
		else if(tab->szerokosc%4==2) plik_odczyt.seekg(2, ios::cur);
		else if(tab->szerokosc%4==1) plik_odczyt.seekg(1, ios::cur);
	}

	plik_odczyt.close();
}


void obliczenia_RGB(obraz *tab)
{
	tab->suma_RGB = tab->R_ogolem + tab->G_ogolem + tab->B_ogolem;
	tab->procenty[0] = (tab->R_ogolem/tab->suma_RGB)*100;	//R
	tab->procenty[1] = (tab->G_ogolem/tab->suma_RGB)*100;	//G
	tab->procenty[2] = (tab->B_ogolem/tab->suma_RGB)*100;	//B
}


void histogram_RGB(obraz *tab)
{		
	obliczenia_RGB(tab);
	int max=0;
	for(int i=0; i<3; i++)
	{
		if(tab->procenty[i]>max) max=tab->procenty[i];
	}
	
	cout << "Histogram rozkladu barw podstawowych: \n\n";
	
	for(int i=25; i>=0; i--)
	{		
		for(int j=0; j<3; j++)
		{
			if (tab->procenty[j]==0) cout << "            ";
			else if (((tab->procenty[j]/max)*100)/4>=i) cout << setw(15) << "*********** " << setw(4);
			else cout << setw(15) << "            " << setw(4);
		}
	cout << endl;	
	}
	cout << setw(9) << "R"<< setw(15) << "G" << setw(15) << "B" << "\n\n";	
	cout << "Wcisnij dowolny przycisk aby zobaczyc wykres: \n";
	system("pause");
	cout << endl;
}


void zapis_histogram_RGB(obraz *tab)
{
	ofstream plik_zapis;
	plik_zapis.open("plik.txt", ios::out|ios::app);
		
	int max=0;
	for(int i=0; i<3; i++)
	{
		if(tab->procenty[i]>max) max=tab->procenty[i];
	}
	
	plik_zapis << "Histogram rozkladu barw podstawowych: \n\n";
	
	for(int i=25; i>=0; i--)
	{		
		for(int j=0; j<3; j++)
		{
			if (tab->procenty[j]==0) plik_zapis << "            ";
			else if (((tab->procenty[j]/max)*100)/4>=i) plik_zapis << setw(15) << "*********** " << setw(4);
			else plik_zapis << setw(15) << "            " << setw(4);
		}
	plik_zapis << endl;	
	}
	plik_zapis << setw(9) << "R"<< setw(14) << "G" << setw(15) << "B" << "\n\n\n";			
	plik_zapis.close();
}


void wykres_RGB(obraz *tab)
{
  	cout << setiosflags(ios::fixed);
	cout << setprecision(2);
	cout << "  Kolor   | Procentowa zawartosc " << endl;
  	cout << "----------------------------------" << endl;
  	
	for (int i=0; i<3; i++)
	{
		if(i==0) cout << "Czerwony" << "\t" << tab->procenty[0] << " %" << endl;
		else if (i==1) cout << "Zielony" << "\t\t" << tab->procenty[1] << " %" << endl;
		else if (i==2) cout << "Niebieski" << "\t" << tab->procenty[2] << " %" << endl;
	}
	
	cout << "----------------------------------\n" << endl;
}


void zapis_wykres_RGB(obraz *tab)
{
	ofstream plik_zapis;
	plik_zapis.open("plik.txt", ios::out|ios::app);
	
	plik_zapis << setiosflags(ios::fixed);
	plik_zapis << setprecision(2);
	
	plik_zapis << "  Kolor   | Procentowa zawartosc " << endl;
  	plik_zapis << "----------------------------------" << endl; 	
	for (int i=0; i<3; i++)
	{
		if(i==0) plik_zapis << "Czerwony" << "\t" << tab->procenty[0] << " %" << endl;
		else if (i==1) plik_zapis << "Zielony" << "\t\t" << tab->procenty[1] << " %" << endl;
		else if (i==2) plik_zapis << "Niebieski" << "\t" << tab->procenty[2] << " %" << endl;
	}	
	plik_zapis << "----------------------------------\n" << endl;
	plik_zapis.close();
}


void czestosc(obraz *tab)
{
	ofstream plik_zapis;
	plik_zapis.open("plik.txt", ios::out|ios::app);
	int max=tab->tab_jasnosci[0];
	int min=tab->tab_jasnosci[0];
	int cz=0, rz=0;
	for (int i=0; i<256; i++)
	{
		if (tab->tab_jasnosci[i]>max) 
		{
			max=tab->tab_jasnosci[i];
			cz=i;
		}
		
		if (tab->tab_jasnosci[i]<min) 
		{
			min=tab->tab_jasnosci[i];
			rz=i;
		}
	}	
	cout << "Najrzadziej wystapila wartosc " << rz << " - " << min << " razy.\n";	
	cout << "Najczesciej wystapila wartosc " << cz << " - " << max << " razy!\n";
	plik_zapis << "Najrzadziej wystapila wartosc " << rz << " - " << min << " razy.\n";	
	plik_zapis << "Najczesciej wystapila wartosc " << cz << " - " << max << " razy!\n";
	plik_zapis.close();
}


void analiza_jasnosc(char nazwa_plik[20], obraz *tab)
{
	cout << setiosflags(ios::fixed);
	cout << setprecision(0);
	cout << "Trwa ladowanie: \n";
	
	tab->wymiar = tab->rozmiar - tab->offset + 1;
	int* wartosci = new int[tab->wymiar];
	for (int i=0; i<tab->wymiar; i++) wartosci[i]=0;
	
	ifstream plik_odczyt;	
	plik_odczyt.open(nazwa_plik, ios::binary);
	
	plik_odczyt.seekg(tab->offset);

	for (int i=0; i<tab->wymiar; i++)
	{
		plik_odczyt.read(reinterpret_cast<char*>(&wartosci[i]), 1);
	}
	
	for (int szukana=0; szukana<256; szukana++)
	{
		for (int i=0; i<tab->wymiar; i++)
		{
			if(wartosci[i]==szukana) tab->tab_jasnosci[szukana]++;
		}
	cout << ".";
	}
	cout << endl << endl;
	
	plik_odczyt.close();
	delete [] wartosci;
}


void histogram_jasnosc(char nazwa_plik[20], obraz *tab)
{
	int max=0;
	for(int i=0; i<256; i++)
	{
		if(tab->tab_jasnosci[i]>max) max=tab->tab_jasnosci[i];
	}
	
	for(int i=50; i>=0; i--)
	{		
		if(i==50) cout << "   ";
		for(int j=0; j<256; j++)
		{
			if(tab->tab_jasnosci[j]==0) cout << " " << setw(4);
			else if (((tab->tab_jasnosci[j]/max)*100)/2>=i) cout << "*" << setw(4);
			else cout << " " << setw(4);
		}
	cout << endl;	
	}	

	for(int i=0; i<256; i++)
	{
		cout << i << setw(4);
	}	
	cout << endl << endl;	
	system("pause");
}


void zapis_histogram_jasnosc(char nazwa_plik[20], obraz *tab)
{
	ofstream plik_zapis;
	plik_zapis.open("plik.txt", ios::out|ios::app);

	int max=0;
	for(int i=0; i<256; i++)
	{
		if(tab->tab_jasnosci[i]>max) max=tab->tab_jasnosci[i];
	}
	
	for(int i=50; i>=0; i--)
	{		
		if(i==50) plik_zapis << "   ";
		for(int j=0; j<256; j++)
		{
			if (tab->tab_jasnosci[j]==0) plik_zapis << " " << setw(4);
			else if (((tab->tab_jasnosci[j]/max)*100)/2>=i) plik_zapis << "*" << setw(4);
			else plik_zapis << " " << setw(4);
		}
	plik_zapis << endl;	
	}	

	for(int i=0; i<256; i++)
	{
		plik_zapis << i << setw(4);
	}
	plik_zapis.close();	
}


void wykres_jasnosc(obraz *tab)
{
	cout << "  Wartosc   | Ilosc wystapien " << endl;
  	cout << "----------------------------------" << endl;
  	
  	for (int i=0; i<256; i++)
  	{
  		cout << setw(6) << i << setw(16) << tab->tab_jasnosci[i] << endl;
	}
		
	cout << "----------------------------------" << endl;
	czestosc(tab);
}


void zapis_wykres_jasnosc(obraz *tab)
{
	ofstream plik_zapis;
	plik_zapis << setiosflags(ios::fixed);
	plik_zapis << setprecision(0);
	plik_zapis.open("plik.txt", ios::out|ios::app);
	plik_zapis << "\n\n\n Wykres dla histogramu jasnosci: \n";
	plik_zapis << "  Wartosc   | Ilosc wystapien " << endl;
  	plik_zapis << "----------------------------------" << endl;
  	
  	for (int i=0; i<256; i++)
  	{
  		plik_zapis << setw(6) << i << setw(16) << tab->tab_jasnosci[i] << endl;
	}
		
	plik_zapis << "----------------------------------" << endl;
	plik_zapis.close();
}


void RGB(char nazwa_plik[20], obraz *tab)
{
	cout << endl;
	analiza_RGB(nazwa_plik, tab);
	histogram_RGB(tab);
	zapis_histogram_RGB(tab);
	wykres_RGB(tab);
	zapis_wykres_RGB(tab);
}

void jasnosc(char nazwa_plik[20], obraz *tab)
{
	analiza_jasnosc(nazwa_plik, tab);
	histogram_jasnosc(nazwa_plik, tab);
	zapis_histogram_jasnosc(nazwa_plik, tab);
	wykres_jasnosc(tab);
	zapis_wykres_jasnosc(tab);
}


void zly_plik()
{
	cin.clear();												//czyszczenie flagi bledu strumienia
	cin.sync();													//czysczenie bufora strumienia
	cout << "Nie istnieje podany plik. Sprobuj ponownie.\n\n";
	system("pause");
	system("cls");
}

void zly_wybor_trybu(char nazwa_plik[20], obraz *tab, void wybor_trybu(char nazwa_plik[20], obraz *tab))
{
	cin.clear();		
	cin.sync();
	cout << "Wprowadzono nieprawidlowa opcje!\n\n";
	system("pause");
	system("cls");
	wybor_trybu(nazwa_plik, tab);
}


void analiza_skladowa(int a, char nazwa_plik[20], obraz* tab)
{
	cout << setiosflags(ios::fixed);
	cout << setprecision(0);
	ifstream plik_odczyt;	
	plik_odczyt.open(nazwa_plik, ios::binary);
	cout << "Trwa ladowanie: \n";
	
	tab->wymiar = (tab->rozmiar-tab->offset)/3;
	int* wartosci = new int[tab->wymiar];
	for (int i=0; i<tab->wymiar; i++) wartosci[i]=0;
	
	plik_odczyt.seekg((tab->offset) + a);

	for (int i=0; i<tab->wymiar; i++)
	{
		plik_odczyt.read(reinterpret_cast<char*>(&wartosci[i]), 1);
		plik_odczyt.seekg(2, ios::cur);
	}
	
	for (int szukana=0; szukana<256; szukana++)
	{
		for (int i=0; i<tab->wymiar; i++)
		{
			if(wartosci[i]==szukana) tab->tab_jasnosci[szukana]++;
		}
	cout << ".";
	}
	cout << endl << endl;
	
	plik_odczyt.close();
	delete [] wartosci;
}

void skladowa(int a, char nazwa_plik[20], obraz* tab, void histogram_jasnosc(char nazwa_plik[20], obraz *tab))
{
	analiza_skladowa(a, nazwa_plik, tab);
	histogram_jasnosc(nazwa_plik, tab);
	zapis_histogram_jasnosc(nazwa_plik, tab);
	wykres_jasnosc(tab);
	zapis_wykres_jasnosc(tab);
}

void wybor_skladowej(obraz* tab, char nazwa_plik[20])
{
	char wybor[2];
	while(true)
	{
		cout << "Dla jakiej skladowej wyznaczyc histogram? (R-czerwony, G-zielony, B-niebieski): ";
		cin >> wybor;
		
		if((strcmp(wybor, "R")!=0) && (strcmp(wybor, "G")!=0) && (strcmp(wybor, "B")!=0))
		{
			while ( (strcmp(wybor, "R")!=0) && (strcmp(wybor, "G")!=0) && (strcmp(wybor, "B")!=0) )
			{
				cout << "Wprowadzono nieprawidlowa opcje! (R-czerwony, G-zielony, B-niebieski)\n\n";
				break;
			}
		}
		else if((strcmp(wybor, "B")==0)) 
		{
			skladowa(0, nazwa_plik, tab, histogram_jasnosc);
			break;
		}
		else if((strcmp(wybor, "G")==0))
		{
			skladowa(1, nazwa_plik, tab, histogram_jasnosc);
			break;
		} 
		else if((strcmp(wybor, "R")==0))
		{
			 skladowa(2, nazwa_plik, tab, histogram_jasnosc);
			 break;
		}
	}
}


void wybor_trybu(char nazwa_plik[20], obraz *tab)
{
	int wybor;
	cout << "Jaki histogram utworzyc? (1 - histogram barwy, 2 - histogram jasnosci, 3 - histogram dla skladowej koloru): ";
	cin >> wybor;
	switch(wybor)
	{
		case 1:
		{
			RGB(nazwa_plik, tab);
			break;	
		}
		
		case 2:
		{
			jasnosc(nazwa_plik, tab);
			break;				
		}
		
		case 3:
		{
			wybor_skladowej(tab, nazwa_plik);
			break;				
		}
		
		default:
		{
			zly_wybor_trybu(nazwa_plik, tab, wybor_trybu);
			break;
		}	
	}
}
