#include <iostream> 
#include <Windows.h>
using namespace std;
char * crt = "\n========================================\n";


enum NacinStudiranja { REDOVAN, DL };
enum GodinaStudija { PRVA = 1, DRUGA, TRECA, CETVRTA };
enum Odgovori { TACNO, NETACNO };
const int brojPitanja = 105;
const int pitanja = 100;

struct Pitanja_admin {
	char *_pitanja[pitanja];
	int brojac_p = 0;
	Odgovori odgovori[pitanja];
	int brojac_o = 0;
};

struct Student {
	int _brojIndeksa;
	char _imePrezime[30];
	bool _radioTest;
	NacinStudiranja _nacin;
	GodinaStudija _godina;
	Odgovori _odgovori[brojPitanja];
	int _brojac_o = 0;
};

void PrikaziPitanje(int rbPitanja) {
	if (rbPitanja == 1)
		cout << crt << "Enumeracije i unije su sinonimi ?" << crt;
	else if (rbPitanja == 2)
		cout << crt << "Moguce je kreirati anonimnu uniju ?" << crt;
	else if (rbPitanja == 3)
		cout << crt << "Svi clanovi unije koriste istu memorijsku lokaciju?" << crt;
	else if (rbPitanja == 4)
		cout << crt << "Svi clanovi strukture koriste istu memorijsku lokaciju ?" << crt;
	else
		cout << crt << "Unija moze biti deklarisana unutar strukture ?" << crt;
}

Odgovori GetTacanOdgovor(int rbPitanja) {
	if (rbPitanja == 1)
		return NETACNO;
	if (rbPitanja == 2)
		return TACNO;
	if (rbPitanja == 3)
		return TACNO;
	if (rbPitanja == 4)
		return NETACNO;
	return TACNO;
}

int GetUspjehStudenta(Odgovori odgovori[], Pitanja_admin pitanja) {
	int brojacTacnih = 0;
	int i = 0;
	for (; i<5; i++)
		if (odgovori[i] == GetTacanOdgovor(i + 1))
			brojacTacnih++;
	if (pitanja.brojac_o != 0)
	{
		for (int j = 0; j < pitanja.brojac_o; i++, j++) {
			if (odgovori[i] == pitanja.odgovori[j]) {
				brojacTacnih++;
			}
		}
	}
	return brojacTacnih;
}

void PokreniTest(Student & student, Pitanja_admin &pitanja) {
	int unos;
	int i = 0;
	Odgovori temp;
	for (; i < 5; i++) {
		do {
			PrikaziPitanje(i + 1);
			cout << "Odgovor (1.Tacno,2.Netacno): ";
			cin >> unos;
			unos--;
			temp = (Odgovori)unos;
		} while (temp != TACNO && temp != NETACNO);
		student._odgovori[i] = temp;
	}
	if (pitanja.brojac_p != 0) {
		for (int j = 0; j < pitanja.brojac_p; i++, j++) {
			do {
				cout << crt <<  pitanja._pitanja[j] << " ?" << crt;
				cout << "Odgovor (1.Tacno,2.Netacno): ";
				cin >> unos;
				unos--;
				temp = (Odgovori)unos;
			} while (temp != TACNO && temp != NETACNO);
			student._odgovori[i] = temp;
		}
	}
	student._radioTest = true;
}

void UnosPodataka(Student * niz, int max) {
	int unos;
	for (int i = 0; i<max; i++) {
		cout << crt << "\t::STUDENT " << i + 1 << "::" << crt;
		cout << "Unesite ime i prezime: ";
		cin.getline(niz[i]._imePrezime, 30);
		cout << "Unesite broj indeksa: ";
		cin >> niz[i]._brojIndeksa;
		cout << "Nacin studiranja (1.Redovan, 2.DL): ";
		cin >> unos;
		niz[i]._nacin = (NacinStudiranja)--unos;
		cout << "Nacin godinu studija (1,2,3,4): ";
		cin >> unos;
		niz[i]._godina = (GodinaStudija)unos;
		cin.ignore();
		niz[i]._radioTest = false;
		//tek smo kreirali studenta        
		//pa mozemo biti sigurni da test nije radjen  
	}
}

void ispis(Student a) {
	cout << "Ime i prezime: " << a._imePrezime << "\n";
	cout << "Broj indexa: " << a._brojIndeksa << "\n";
	Sleep(1000);
}

void Pretraga(Student * niz, int max) {
	//definisati funkciju(e) koja(e) ce omoguciti pretragu po:
	int unos;
	do {
		cout << "Vrsta pretrage: \n";
		cout << "1. Godina studiranja\n";
		cout << "2. Nacin studiranja\n";
		cin >> unos;
		cin.ignore();
	} while (unos < 1 || unos > 2);
	//1. godini studija
	if (unos == 1) {
		cout << "Godina: ";
		cin >> unos;
		cin.ignore();
		GodinaStudija temp = (GodinaStudija)unos;
		for (int i = 0; i < max; i++) {
			if (niz[i]._godina == temp) {
				ispis(niz[i]);
			}
		}
	}
	//2. nacinu studiranja 
	else {
		cout << "Nacin studiranja: <1> Redovan <2> DL\n";
		cin >> unos;
		cin.ignore();
		--unos;
		NacinStudiranja temp = (NacinStudiranja)unos;
		for (int i = 0; i < max; i++) {
			if (niz[i]._nacin == temp) {
				ispis(niz[i]);
			}
		}
	}
	//3. rezultatima ostvarenim na testu ----- To vec ima -------
	//Nove funkcionalnosti dodati u funkciju PrikaziMeni
}

char *dodavanje_pitanja() {
	char *pok = new char[100];
	cout << "Unesite pitanje:\n";
	cin.getline(pok, 100);
	return pok;
}

Odgovori dodavanje_odgovora() {
	int a;
	cout << "Dodajte odgovor za pitanje: <1> Tacno <2> Netacno\n";
	cin >> a;
	cin.ignore();
	return ((Odgovori)--a);
}

int PrikaziMeni() {
	int izbor;
	do {
		cout << crt << "\t::MENI::" << crt;
		cout << "1. Pokreni test" << endl;
		cout << "2. Prikazi rezultate" << endl;
		cout << "3. Kraj rada" << crt;
		cout << "Unesite izbor: ";
		cin >> izbor;
	} while (izbor<1 || izbor>3);
	return izbor;
}

int PrikaziMeni_admin() {
	int izbor;
	do {
		cout << crt << "\t::MENI - ADMIN::" << crt;
		cout << "1. Pretraga\n";
		cout << "2. Ponovo polaganje testa\n";
		cout << "3. Dodavanje pitanja\n";
		cout << "4. Kraj\n";
		cout << "Unesite izbor: ";
		cin >> izbor;
		cin.ignore();
	} while (izbor < 1 || izbor > 4);
	return izbor;
}

int PrikaziStudente(Student * niz, int max) {
	int rb;
	do {
		for (int i = 0; i<max; i++)
			cout << i + 1 << ". " << niz[i]._imePrezime << endl;
		cout << crt << "Unesite redni broj studenta: ";
		cin >> rb;
	} while (rb<1 || rb>max);
	return rb - 1;
}

//dodati sljedece funkcionalnosti:
//1.administratoru omoguciti da unosi pitanja te definise tacne odgovore  ##################
//2.administratoru dati prava da studentu ponovo omoguci polaganje testa ###############
//3.uvesti username/password na osnovu koga ce se studenti logirati #########
//4.odvojiti administratorske funkcije (meni) od korisnickih #####################




void main() {
	int max, izbor, odabraniStudent;
	Pitanja_admin _pitanja;
	int admin_pass = 1234, student_pass = 0000;
	int unos1;
	cout << "Unesite broj studenata: ";
	cin >> max;
	cin.ignore();
	Student * niz = new Student[max];
	UnosPodataka(niz, max);
	char * imePrezime = nullptr;
	do {
		cout << "<1> Admin  <2> Student\n";
		cin >> unos1;
		if (unos1 == 2 && student_pass == []() ->int {
			int pass;
			cout << "Unesi pass: ";
			cin >> pass;
			cin.ignore();
			return pass;
		}()) {
			system("cls");
			odabraniStudent = PrikaziStudente(niz, max);
			imePrezime = niz[odabraniStudent]._imePrezime;
			do {
				izbor = PrikaziMeni();
				if (izbor == 3)
					break;
				//pokusajte pojednostaviti naredni dio koda, te smanjiti broj nepotrebnih provjera   
				if (izbor == 1) {
					if (niz[odabraniStudent]._radioTest == false)
						PokreniTest(niz[odabraniStudent], _pitanja);
					else
						cout << "Student " << imePrezime << " je vec radio test!" << endl;
				}
				//da li nam je else if mozda bio bolja opcija 
				else {
					if (niz[odabraniStudent]._radioTest == true) {
						int brojTacnih = GetUspjehStudenta(niz[odabraniStudent]._odgovori, _pitanja);
						cout << crt << "Uspjeh studenta " << imePrezime << " je " << brojTacnih << "/" << 5 + _pitanja.brojac_o << " ili " << (brojTacnih * 100.0) / (5 + _pitanja.brojac_o) << "%" << crt;
					}
					else
						cout << "Student " << imePrezime << " jos uvijek nije radio test!" << endl;
				}
				system("pause>0");
				system("cls");
			} while (izbor != 3);
		}
		else if (unos1 == 1 && admin_pass == []()->int {
			int pass;
			cout << "Unesi pass: ";
			cin >> pass;
			cin.ignore();
			return pass;
		}()) {
			system("cls");
			izbor = PrikaziMeni_admin();
			if (izbor == 4) {
				break;
			}
			else if (izbor == 1) {
				Pretraga(niz, max);
			}
			else if (izbor == 2) {
				odabraniStudent = PrikaziStudente(niz, max);
				niz[odabraniStudent]._radioTest = false;
				cout << "Student moze ponovo raditi test!";
			}
			else {
				_pitanja._pitanja[_pitanja.brojac_p] = dodavanje_pitanja();
				++_pitanja.brojac_p;
				_pitanja.odgovori[_pitanja.brojac_o] = dodavanje_odgovora();
				++_pitanja.brojac_o;
			}
			system("pause>0");
			system("cls");
		}
	} while (true);
	for (int i = 0; i < _pitanja.brojac_p; i++) {
		delete[] _pitanja._pitanja[i];
	}
	delete[] niz;
	niz = nullptr;

	system("pause");
}