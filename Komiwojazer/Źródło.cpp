#include <iostream>
#include <ctime>



using namespace std;
using std::cout;


int poczatkowa_najlepsza_funkcja_celu = INT_MAX; //zmienna do zapamietania wartosci funkcji celu rozwiazania powstalego w sposob losowy o najbardziej optymalnej (najmniejszej) jej wartosci
int koncowa_najlepsza_funkcja_celu = INT_MAX; //zmnienna do zapamietania wartosci funckji celu rozwiazania znalezionego przez algorytm genetyczny o najbardziej optymalnej (najmniejszej) jej wartosci


int * poczatkowe_rozwiazanie; //tablica do zapamietania rozwiazania znalezionego na poczatku dzialania programu w sposob losowy
int * koncowe_rozwiazanie; //tablica do zapamietania rozwiazania znalezionego przez algorytm genetyczny

class Graf //Klasa sluzaca do reprezentacji grafu pelnego
{

	int wielkosc_tablicy = 0;
	


	int** odleglosc; //macierz do zapamietania wartosci krawedzi pomiedzy dwoma wierzcholkami



	public:
	Graf(int a, int b); 

	void przypisz(int liczba_wierzcholkow)

	{
		for (int i = 0;i < liczba_wierzcholkow;i++)
		{



			for (int j = 0; j < liczba_wierzcholkow;j++)
			{
				int liczba = rand() % 50 + 1;
				

				if (i == j)
					liczba = -1;

				if (odleglosc[i][j] == -2)
				{
				
				odleglosc[i][j] = liczba;
				odleglosc[j][i] = liczba;

				}

		}

		}

	}


	
	void wyswietl() // wyswietla wartosci krawedzi dla danych dwoch wierzcholkow w grafie, -1 oznacza brak krawedzi

	{

		

		cout << "liczba wierzcholkow: " << wielkosc_tablicy << endl << endl;

		cout << "Ponizej po dwukropku przedstawione sa odleglosci pomiedzy poszczegolnymi wierzcholkami polaczonymi krawedziami, -1 oznacza brak krawedzi" << endl << endl;

		

		for (int i = 0;i < wielkosc_tablicy;i++)
		{
			cout << endl;
			for (int j = 0; j < wielkosc_tablicy;j++)
			{
				cout << i + 1 << "--" << j + 1 << ": ";
					cout << odleglosc[i][j]<<endl;

			}

		}


	}

	int oblicz_funkcje(int *tablica, int wielkosc_tab) //oblicza wartosc funkcji celu

	{

		int wartosc = 0;

		
		for (int i = 0;i < wielkosc_tab-1;i++)
		{

			wartosc = wartosc + odleglosc[tablica[i]][tablica[i+1]];

		}

		wartosc = wartosc + odleglosc[tablica[wielkosc_tab-1]][tablica[0]];
		
		return wartosc;


	}
	
};

Graf* poczatek = NULL;

Graf::Graf(int a, int b) //konstruktor sluzacy do zainicjowania macierzy dynamicznej o wielkosci b i wypelnienienia jej wartosciami poczatkowymi

{

	
	odleglosc = new int* [b];
	for (int i = 0;i < b;i++) {
		odleglosc[i] = new int[b];
	}


	for (int i = 0;i < b;i++)
	{



		for (int j = 0; j < b;j++)
		{
	

				odleglosc[i][j] = -2;
				


		}

	}


	wielkosc_tablicy = b;

}




class Rozwiazania // Klasa sluzaca do reprezentacji uzyskanych rozwiazan
{



	int wielkosc_tab = 0;
	int* uzyte;
	

public:
	int* rozwiazanie;
	int funkcja_celu = 0;
	
	Rozwiazania();
	Rozwiazania(int b);


	void przypisz() //funkcja sluzaca do tworzenia losowych rozwiazan poczatkowych
	{
		for (int i = 0;i < wielkosc_tab;i++)
		{
			int liczba = -10;
			while(uzyte[liczba]>=0 || liczba==-10)
			liczba = rand() % wielkosc_tab;

			rozwiazanie[i] = liczba;
		
			uzyte[liczba] = i;

		}

		Graf* grof = poczatek;

		int wynik = grof->oblicz_funkcje(rozwiazanie, wielkosc_tab);

		funkcja_celu = wynik;


		if (poczatkowa_najlepsza_funkcja_celu > funkcja_celu) //szukanie rozwiazania o najmniejszej funkcji celu zeby zapamietac to rozwiazanie i wartosc jego funkcji celu do porownania pod koniec dzialania programu
		{
			poczatkowa_najlepsza_funkcja_celu = funkcja_celu;

			for (int i = 0;i < wielkosc_tab;i++)
			{
				poczatkowe_rozwiazanie[i] = rozwiazanie[i];
			}

		}

	}




	void przelicz_ocene() //funkcja sluzaca do obliczania funkcji celu dla rozwiazan powstajacych w trakcie dzialania algorytmu genetycznego

	{

		Graf* grof = poczatek;

		int wynik = grof->oblicz_funkcje(rozwiazanie, wielkosc_tab);

		funkcja_celu = wynik;

		if (koncowa_najlepsza_funkcja_celu > funkcja_celu)
		{
			koncowa_najlepsza_funkcja_celu = funkcja_celu;

			for (int i = 0;i < wielkosc_tab;i++)
			{
				koncowe_rozwiazanie[i] = rozwiazanie[i];
			}

		}

	}


};

Rozwiazania* tab_R;


Rozwiazania::Rozwiazania()

{



}

Rozwiazania::Rozwiazania(int b) //konstruktor sluzacy do zainicjowania dynamicznych tablic o wielkosci b i wypelnienia ich wartosciami poczatkowymi

{

	rozwiazanie = new int [b];
	uzyte = new int[b];
	
	for (int i = 0;i < b;i++)
	{

			rozwiazanie[i] = -2;
			uzyte[i] = -2;

	}

	wielkosc_tab = b;

}


//////////////////////////////////////////////////////////////////////////


class Turniej // Klasa sluzaca do przeprowadzania Turnieju i przechowywania rozwiazan zwycieskich w turniejach
{

int wielkosc_tab = 0;
	
public:
	int* rozwiazanie_turniej;
	int funkcja_celu = 0;
	
	Turniej();
	Turniej(int b, int zasieg, int rozmiar_populacji);

};

Turniej * tab_T;


Turniej::Turniej()

{


}


Turniej::Turniej(int liczba_wierzcholkow, int rozmiar_turnieju, int rozmiar_populacji) //Konstruktor sluzacy do zainicjalizowania tablic dynamicznych o wielkosci liczba_wierzcholkow i przeprowadzenia turnieju

{

	rozwiazanie_turniej = new int[liczba_wierzcholkow];
	
	for (int i = 0;i < liczba_wierzcholkow;i++)
	{

		rozwiazanie_turniej[i] = -2;

	}


	wielkosc_tab = liczba_wierzcholkow;

	int liczba = 0;
	int ocena = INT_MAX;
	int licznik = 0;

	Rozwiazania wskaznik_R = NULL;

	for (int i = 0;i < rozmiar_turnieju;i++)
	{

		liczba = rand() % rozmiar_populacji;


		if (tab_R[liczba].funkcja_celu < ocena)
		{

			ocena = tab_R[liczba].funkcja_celu;
		
			wskaznik_R = tab_R[liczba];
		
		}

		
		for (int i = 0;i < wielkosc_tab;i++)
		{
			
			rozwiazanie_turniej[i] = wskaznik_R.rozwiazanie[i];
			
		}

		funkcja_celu = wskaznik_R.funkcja_celu;


	}

}

//////////////////////////////////////////////////////////////////////////

void Krzyzowanie(int rozmiar_pop, int wielkosc_tab, int dziecko_numer) //Funkcja sluzaca do tworzenia nowych rozwiazan na podstawie rozwiazan, ktore wygraly w turniejach
{
	
	int* uzyte_1;
	int* uzyte_2;

	uzyte_1 = new int[wielkosc_tab];
	uzyte_2 = new int[wielkosc_tab];

	int pierwszy_rodzic = 0;
	int drugi_rodzic = 0;
	int punkt_krzyzowania = 0;
	int punkt_krzyz = 0;

	rozmiar_pop = rozmiar_pop - 1;

	while (pierwszy_rodzic == drugi_rodzic)
	{
		pierwszy_rodzic = rand() % rozmiar_pop;
		drugi_rodzic = rand() % rozmiar_pop;
	}


	int dziecko_pierwsze = dziecko_numer;
	int dziecko_drugie = dziecko_numer+1;


	for (int i = 0;i < wielkosc_tab;i++)
	{

		uzyte_1[i] = -2;
		uzyte_2[i] = -2;

		tab_R[dziecko_pierwsze].rozwiazanie[i] = -1;
		tab_R[dziecko_drugie].rozwiazanie[i] = -1;





	}

	punkt_krzyzowania = rand() % wielkosc_tab;

	tab_T[pierwszy_rodzic].rozwiazanie_turniej;
	tab_T[drugi_rodzic].rozwiazanie_turniej;


	if(punkt_krzyzowania==wielkosc_tab)
	punkt_krzyz = punkt_krzyzowania - 1;

	else if (punkt_krzyzowania == 0)
		punkt_krzyz = punkt_krzyzowania + 1;


		for (int j = punkt_krzyz;j < wielkosc_tab;j++)
		{

			tab_R[dziecko_pierwsze].rozwiazanie[j] = tab_T[drugi_rodzic].rozwiazanie_turniej[j];
			uzyte_1[tab_T[drugi_rodzic].rozwiazanie_turniej[j]] = 1;

			
			tab_R[dziecko_drugie].rozwiazanie[j] = tab_T[pierwszy_rodzic].rozwiazanie_turniej[j];
			uzyte_2[tab_T[pierwszy_rodzic].rozwiazanie_turniej[j]] = 1;


		}


		for (int j = punkt_krzyz-1;j >= 0;j--)
		{

			for (int y = 0; y < wielkosc_tab;y++)
			{

				if (uzyte_1[tab_T[pierwszy_rodzic].rozwiazanie_turniej[y]] < 0)
				{
					tab_R[dziecko_pierwsze].rozwiazanie[j] = tab_T[pierwszy_rodzic].rozwiazanie_turniej[y];
				}

				if (uzyte_2[tab_T[drugi_rodzic].rozwiazanie_turniej[y]] < 0)
				{
					tab_R[dziecko_drugie].rozwiazanie[j] = tab_T[drugi_rodzic].rozwiazanie_turniej[y];
				}

			}

		}


}


void Mutacja(int dziecko_numer, int wielkosc_tab) //funkcja do wprowadzania mutacji - losowej zmiany w rozwiazaniach

{

	int dziecko_pierwsze = dziecko_numer;

	int liczba1 = rand() % wielkosc_tab;

	int liczba2 = rand() % wielkosc_tab;

	int zamiana[1];

	zamiana[0] = tab_R[dziecko_numer].rozwiazanie[liczba1];

	tab_R[dziecko_numer].rozwiazanie[liczba1] = tab_R[dziecko_numer].rozwiazanie[liczba2];

	tab_R[dziecko_numer].rozwiazanie[liczba2] = zamiana[0];

}


int main()

{
	
	srand(time(NULL));

	int liczba_wierzcholkow = 0;
	int decyzja = -1;

	while (decyzja != 0 && decyzja != 1)
	{
		cout << "Czy uzyc domyslna liczbe wierzcholkow=15? 1-tak 0-nie"<<endl;
		cin >> decyzja;

	}

	if (decyzja == 1)
	{
		liczba_wierzcholkow = 15;
	}

	else
	{
		cout << "Podaj liczbê wierzcho³kow"<<endl;
		cin >> liczba_wierzcholkow;
	}
	

	poczatkowe_rozwiazanie = new int[liczba_wierzcholkow];
	koncowe_rozwiazanie = new int[liczba_wierzcholkow];


	Graf* grof = new Graf(liczba_wierzcholkow, liczba_wierzcholkow);

	grof->przypisz(liczba_wierzcholkow);
		
	poczatek = grof;

	int rozmiar_populacji = 4000;
	int liczba_cykli = 2000;
	int rozmiar_turnieju = 8;
	float procent_mutacji = 0.250;
	float procent_zastepowalnosci = 0.5;

	
	cout << "Rozmiar populacji: " << rozmiar_populacji << endl;
	cout << "Liczba cykli: " << liczba_cykli << endl;
	cout << "Rozmiar turnieju: " << rozmiar_turnieju << endl;
	cout << "procent mutacji: " << procent_mutacji << endl;
	cout << "procent zastepowalnosci pokolen: " << procent_zastepowalnosci << endl;

	tab_R = new Rozwiazania[rozmiar_populacji];

	for (int i = 0;i < rozmiar_populacji;i++)
	{
		tab_R[i] = Rozwiazania(liczba_wierzcholkow);

		tab_R[i].przypisz();

		int liczba = rand() % 2;
	
	}


	///////////////////////////////////////////////////////////////////////////////////////////

	tab_T = new Turniej[rozmiar_populacji];

	cout << endl << "Trwaja obliczenia:" << endl;

	for (int j = 0;j < liczba_cykli;j++)

	{
		if (j % 100 == 0) 
		cout << "=";

		for (int i = 0;i < rozmiar_populacji;i++) 
		{

			tab_T[i] = Turniej(liczba_wierzcholkow, rozmiar_turnieju, rozmiar_populacji);

		}

		for (int i = 0;i < rozmiar_populacji * procent_zastepowalnosci;i++)
	    Krzyzowanie(rozmiar_populacji, liczba_wierzcholkow, i);



		for (int i = 0;i < rozmiar_populacji * procent_mutacji;i++)
			Mutacja(i, liczba_wierzcholkow);


		for (int i = 0;i < rozmiar_populacji;i++)
			tab_R[i].przelicz_ocene();


	
	}



	cout << "=======================================================" << endl;
	cout << "WYPISUJE ODLEGLOSCI POMIEDZY WIERZCHOLKAMI:" << endl << endl;


	poczatek->wyswietl();


	cout << "=======================================================" << endl;
	cout << "WYPISUJE ROZWIAZANIA:" << endl << endl;

	cout << "Rozwiazanie uzyskane w sposob losowy na poczatku dzialania programu: " << endl;


	for (int i = 0;i < liczba_wierzcholkow;i++)

	{
		cout << poczatkowe_rozwiazanie[i] + 1 << "--";



	}

	cout << poczatkowe_rozwiazanie[0] + 1;

	cout << endl;

	cout << "Wartosc funkcji celu: " << poczatkowa_najlepsza_funkcja_celu << endl << endl;



	cout << "Rozwiazanie uzyskane w wyniku dzialania algorytmu genetycznego: " << endl;


	for (int i = 0;i < liczba_wierzcholkow;i++)

	{
		cout << koncowe_rozwiazanie[i] + 1 << "--";



	}

	cout << koncowe_rozwiazanie[0] + 1;

	cout << endl;

	cout << "Wartosc funkcji celu: " << koncowa_najlepsza_funkcja_celu << endl;
	
	return 12;

}