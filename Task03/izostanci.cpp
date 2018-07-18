#include <iostream>
#include <fstream>
using namespace std;

const char * crt = "\n----------------------------------------------------\n";
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}
};
struct Izostanak {
	char * _razlogIzostanka;
	Datum _datumIzostanka;
	int _brojSati;
	bool _opravdano;//pravdanje zahtijeva da se evidentira i razlog izostanka	

	void Unos(Datum datumIzostanka, int brojSati) {
		_datumIzostanka.Unos(*datumIzostanka._dan, *datumIzostanka._mjesec, *datumIzostanka._godina);
		_brojSati = brojSati;
		_opravdano = false;
		_razlogIzostanka = new char;
		_razlogIzostanka[0] = '\0';
	}
	void Dealociraj() { 
		_datumIzostanka.Dealociraj();
		delete[]_razlogIzostanka; 
		_razlogIzostanka = nullptr;
	}
	void Ispis() {
		_datumIzostanka.Ispis(); 
		cout << _razlogIzostanka << "\n" << _brojSati << "\n"; 
	}
	void Opravdaj(const char * razlogIzostanka) {
		if (_razlogIzostanka != nullptr)
			delete _razlogIzostanka;
		int vel = strlen(razlogIzostanka) + 1;
		_razlogIzostanka = new char[vel];
		strcpy_s(_razlogIzostanka, vel, razlogIzostanka);
		_opravdano = true;
	}
};
struct Student {
	//format broja indeksa: IB150051
	//osigurati automatsko generisanje broja indeksa pocevsi od IB150001
	char _brojIndeksa[9];
	char * _imePrezime;
	Izostanak * _izostanci;
	int _brojIzostanaka;
	//Potpisi funkcija trebaju odgovarati onima u main-u
	//1. Unos
	//2. Dealociraj
	/*3. DodajIzostanak - dodaje novi izostanak u listu izostanaka.	Onemoguciti ponavljanje izostanaka na nacin da se uvecava samo broj sati ukoliko
	korisnik (profesor) vise puta pokusa dodati izostanak na isti dan.*/
	//4. Ispis - ispisuje izostanke u formatu prikazanom na slici (nije obavezno da ispis bude identican, ali je svakako pozeljno). Prilikom svakog ispisa, izostanke je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj indeksa.
	//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu datuma, pronalazi i vraca pokazivac na izostanak. Ukoliko trazeni izostanak ne postoji funkcija vraca nullptr.

	void Unos(char* brojI, const char *ime) {
		int vel = strlen(ime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime,vel, ime);
		strcpy_s(_brojIndeksa, brojI);
		_brojIzostanaka = 0;
		_izostanci = new Izostanak[10];
	}

	void Dealociraj() {
		delete[]_imePrezime;
		_imePrezime = NULL;
		for (int i = 0; i < _brojIzostanaka; i++)
			_izostanci[i].Dealociraj();
		delete[]_izostanci;
		_izostanci = nullptr;
	}

	void DodajIzostanak(Izostanak izostanak) {
		for (int i = 0; i < _brojIzostanaka; i++){
			if (*izostanak._datumIzostanka._dan == *this->_izostanci[i]._datumIzostanka._dan && *izostanak._datumIzostanka._mjesec == *this->_izostanci[i]._datumIzostanka._mjesec){
				_izostanci[i]._brojSati += izostanak._brojSati;
				_izostanci[i].Opravdaj(izostanak._razlogIzostanka);
				return;
			}
		}
		_izostanci[_brojIzostanaka].Unos(izostanak._datumIzostanka, izostanak._brojSati);
		_izostanci[_brojIzostanaka].Opravdaj(izostanak._razlogIzostanka);
		++_brojIzostanaka;
	}

	void Ispis() {
		cout << "Broj indexa je: " << _brojIndeksa << "\n";
		cout << "Ime i prezime je: " << _imePrezime << "\n";
		cout << "Izostanci : \n";
		for (int i = 0; i < _brojIzostanaka; i++){
			_izostanci[i].Ispis();
			cout << "\n==============\n";
		}
		char ime[12];
		strcpy_s(ime, _brojIndeksa);
		strcpy_s(ime, ".txt");
		ofstream upis(ime, ios::out);
		upis.write((char*)this, sizeof(Student));
		upis.close();
	}

	void Sortiraj() {
		for (int i = 0; i < _brojIzostanaka - 1; i++){
			for (int j = i + 1; j < _brojIzostanaka; j++){
				if (*_izostanci[i]._datumIzostanka._dan > *_izostanci[j]._datumIzostanka._dan){
					int dan = *_izostanci[i]._datumIzostanka._dan;
					*_izostanci[i]._datumIzostanka._dan = *_izostanci[j]._datumIzostanka._dan;
					*_izostanci[j]._datumIzostanka._dan = dan;
				}
			}
		}
	}

	Izostanak * BinarnaPretraga(Datum trazeni) {
		Sortiraj();
		int prvi = 1;
		int zadnji = _brojIzostanaka;
		bool nadjen = false;
		while ((nadjen == false) && (prvi <= zadnji)) {
			int srednji = (prvi + zadnji) / 2;
			if (*trazeni._dan < *_izostanci[srednji]._datumIzostanka._dan){
				zadnji = srednji + 1;
			}
			else if(*trazeni._dan == *_izostanci[srednji]._datumIzostanka._dan){
				nadjen = true;
				return &_izostanci[srednji];
			}
			else{
				prvi = srednji + 1;
			}
		}
	}
};

//1. PronadjiNajveciNeopravdaniIzostanak - rekurzivna funkcija koja vraca pokazivac na neopravdani izostanak koji ima najveci broj sati
Izostanak *PronadjiNajveciNeopravdaniIzostanak(Izostanak * izostanci, int trenutno , int max, int najvece = 0) {
	if (trenutno == max){
		return &izostanci[najvece];
	}
	return (izostanci[trenutno]._brojSati > izostanci[najvece]._brojSati) ? PronadjiNajveciNeopravdaniIzostanak(izostanci,trenutno + 1 ,max,trenutno): PronadjiNajveciNeopravdaniIzostanak(izostanci, trenutno + 1 , max, najvece);
}

//2. GenerisiSljedeciBrojIndeksa - generise i vraca naredni broj indeksa
int brojIn = 150001;
char *GenerisiSljedeciBrojIndeksa() {
	//IB150051
	char * generisi = new char[9];
	char inta[7];
	++brojIn;
	_itoa_s(brojIn, inta, 10);
	strcpy_s(generisi, 9, "IB");
	strcpy_s(generisi, 9, inta);
	return generisi;
}

int main() {
	Datum jucer, prije5Dana, prije10Dana;
	jucer.Unos(12, 6, 2016);
	prije5Dana.Unos(8, 6, 2016);
	prije10Dana.Unos(3, 6, 2016);

	Student denis;
	denis.Unos(GenerisiSljedeciBrojIndeksa(), "Denis Music");

	Izostanak izostanakJucer, izostanakPrije5Dana, izostanakPrije10Dana;

	izostanakJucer.Unos(jucer, 5);
	denis.DodajIzostanak(izostanakJucer);

	izostanakPrije5Dana.Unos(prije5Dana, 3);
	denis.DodajIzostanak(izostanakPrije5Dana);
	izostanakPrije5Dana.Unos(prije5Dana, 2);
	izostanakPrije5Dana.Opravdaj("Odsutan zbog bolesti - gripa");
	denis.DodajIzostanak(izostanakPrije5Dana);


	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);
	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);

	denis.Ispis();
	Izostanak * p = denis.BinarnaPretraga(jucer);
	cout << "Binarnom pretragom pronadjen izostanak -> ";
	p->Ispis();
	denis.Ispis();
	Izostanak * p2 = PronadjiNajveciNeopravdaniIzostanak(denis._izostanci,1 ,denis._brojIzostanaka);
	cout << "Rekurzivno pronadjen najveci neopravdani izostanak -> ";
	p2->Ispis();
	cout << endl;
	////dealocirati zauzetu memoriju
	denis.Dealociraj();
	izostanakJucer.Dealociraj(); izostanakPrije5Dana.Dealociraj(); izostanakPrije10Dana.Dealociraj();
	jucer.Dealociraj(); prije5Dana.Dealociraj(); prije10Dana.Dealociraj();
	return 0;
}