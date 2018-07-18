#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

char crt[] = "\n=======================================\n";

enum VrstaVozila { PUTNICKO, TERETNO }; 

struct Datum {
	int _dan, _mjesec, _godina; 
	void Unos(); 
	void Prikazi();
};

struct Vozilo {
	char _regOznaka[10]; 
	char * _markaTip;
	int _godinaProizvodnje;  
	Datum _registrovanDO; //objekat druge strukture
	VrstaVozila _vrsta; //enumeracija  
	void Unos();  
	void Prikazi(); 
	void Dealociraj(); 
};

struct Vlasnik{ 
	char * _imePrezime; 
	char _JMBG[14]; 
	Datum _datumRodjenja; //objekat druge strukture  
	 //niz od 10 pokazivaca na objekte tipa Vozilo
	Vozilo * _vozila[10];//predpostavimo da po zakonu vlasnik ne smije imati vise   
	//od 10 vozila 
	int _brojacVozila; //brojac nam govori koliko vozila vlasnik posjeduje 
	void Unos(); 
	void Prikazi(); 
	void DodajVozilo();//dodaje podatke o novom vozilu,	onemoguciti dupliranje
	void UkloniVozilo();//uklanja vozilo sa liste 
	Vozilo * GetVozilo(char * regOznaka);//vraca pokazivac na vozilo koje ima  
	//trazenu registarsku oznaku. Ukoliko vozilo ne postoji, funkcija vraca nullptr 
	void Dealociraj();  
};

void Datum::Unos() {
	cout << "Unesi dan: ";
	cin >> _dan;
	cout << "Unesite mjesec: ";
	cin >> _mjesec;
	cout << "Unesite godinu: ";
	cin >> _godina;
	cin.ignore();
}

void Datum::Prikazi() {
	cout << _dan << "." << _mjesec << "." << _godina << "\n";
}


void Vozilo::Unos() {
	cout << crt << "\t\t::UNOS::" << crt;
	cout << "Unesite registracijske oznake: ";
	cin.getline(_regOznaka, 10);
	char tekst[20];
	cout << "Unesite marku i tip automobila: ";
	cin.getline(tekst, 20);
	int velicina = strlen(tekst) + 1;
	_markaTip = new char[velicina];
	strcpy_s(_markaTip, velicina, tekst);
	cout << "Unesite godinu proizvodnje: \n";
	cin >> _godinaProizvodnje;
	cin.ignore();
	_registrovanDO.Unos();
	int izbor;
	cout << "Vrsta vozila <1> Putnicko <2> Teretno \nIzbor: ";
	cin >> izbor;
	cin.ignore();
	_vrsta = (VrstaVozila)--izbor;
}

void Vozilo::Prikazi() {
	cout << crt << "\t\t::ISPIS::" << crt;
	cout << "Registracijske oznake su: " << _regOznaka << "\n";
	cout << "Marka i tip automobila je: " << _markaTip << "\n";
	cout << "Godina proizvodnje: " << _godinaProizvodnje << "\n";
	cout << "Datum registracije: ";
	_registrovanDO.Prikazi();
	if (_vrsta == PUTNICKO){
		cout << "Vrsta vozila: Putnicko\n";
	}
	else{
		cout << "Vrsta vozila: Teretno\n";
	}
}

void Vozilo::Dealociraj() {
	delete[]_markaTip;
	_markaTip = nullptr;
}


void Vlasnik::Unos() {
	char tekst[30];
	cout << crt << "\t\t::UNOS::" << crt;
	cout << "Unesite ime i prezime vlasnika: ";
	cin.getline(tekst, 30);
	int velicina = strlen(tekst) + 1;
	_imePrezime = new char[velicina];
	strcpy_s(_imePrezime, velicina, tekst);
	cout << "Unesite JMBG: ";
	cin.getline(_JMBG, 14);
	cout << "Unesite datum rodjenja: \n";
	_datumRodjenja.Unos();
	if (_brojacVozila == 0){
	_brojacVozila = 0;
	_vozila[_brojacVozila]->Unos();
	++_brojacVozila;
	}
}

void Vlasnik::Prikazi() {
	cout << crt << "\t\t::ISPIS::" << crt;
	cout << "Ime i prezime: " << _imePrezime << "\n";
	cout << "JMBG: " << _JMBG << "\n";
	cout << "Datum rodjenja: ";
	_datumRodjenja.Prikazi();
	for (int i = 0; i < _brojacVozila; i++){
		_vozila[i]->Prikazi();
	}
}

void Vlasnik::DodajVozilo() {
	_vozila[_brojacVozila] = new Vozilo;
	_vozila[_brojacVozila]->Unos();
	_brojacVozila++;
	for (int i = 0; i < _brojacVozila-1; i++) {
		if (strcmp(_vozila[i]->_regOznaka, _vozila[_brojacVozila-1]->_regOznaka) == 0) {
			UkloniVozilo();
			cout << "Error nije moguce duplicirati!!\n";
		}
		
	}
}

void Vlasnik::UkloniVozilo() {
	--_brojacVozila;
	_vozila[_brojacVozila]->Dealociraj();
	delete _vozila[_brojacVozila];
	_vozila[_brojacVozila] = nullptr;
}

Vozilo *Vlasnik::GetVozilo(char *regOznaka) {
	for (int i = 0; i < _brojacVozila; i++){
		if (strcmp(_vozila[i]->_regOznaka,regOznaka) == 0){
			return _vozila[i];
		}
	}
	return nullptr;
}

void Vlasnik::Dealociraj() {
	delete[]_imePrezime;
	_imePrezime = nullptr;
	for (int i = 0; i < _brojacVozila; i++){
		_vozila[i]->Dealociraj();
		delete _vozila[i];
		_vozila[i] = nullptr;
	}
}

void Alokacija(Vlasnik *&, int&);
void Dealokacija(Vlasnik *&, int);
int PrikazMeni();
int PrikazVlasnikaRB(Vlasnik*, int);
void UnosVlasnik(Vlasnik*&, int, int&);
void UnosVozila(Vlasnik *&, int);
void PrikazVlasnika(Vlasnik*, int);
void PretragaIiP(Vlasnik*, int);
void PretragaIiliP(Vlasnik*, int);
void PretragaRegO(Vlasnik*, int);
void Prepravka(Vlasnik*, int);

int main()
{
	Vlasnik *niz;
	int max = 0, uneseno = 0, izbor;
	Alokacija(niz, max);
	system("cls");
	do{
		izbor = PrikazMeni();
		switch (izbor){
		case 1:
			UnosVlasnik(niz, max, uneseno);
			break;
		case 2:
			UnosVozila(niz, uneseno);
			break;
		case 3:
			PrikazVlasnika(niz, uneseno);
			break;
		case 4:
			PretragaIiP(niz, uneseno);
			break;
		case 5:
			PretragaIiliP(niz, uneseno);
			break;
		case 6:
			PretragaRegO(niz, uneseno);
			break;
		case 7:
			Prepravka(niz, uneseno);
			break;
		}
	} while (izbor != 8);
	Dealokacija(niz, uneseno);
	return 0;
}

void Alokacija(Vlasnik *&vlasnik, int &max) {
	do{
		cout << "Za koloko osoba unosite podatke: \n";
		cin >> max;
		cin.ignore();
	} while (max <= 0);
	vlasnik = new Vlasnik[max];
}
void Dealokacija(Vlasnik *&niz, int uneseno) {
	for (int i = 0; i < uneseno; i++){
		niz[i].Dealociraj();
	}
	delete[]niz;
	niz = nullptr;
}

int PrikazMeni() {
	int unos;
	do {
		cout << crt << "\t\t::MENI::" << crt;
		cout << "1. Dodaj osobu\n";
		cout << "2. Dodaj vozilo\n";
		cout << "3. Prikazi osobe\n";
		cout << "4. Pretraga po imenu i prezimenu\n";
		cout << "5. Pretraga po imenu ili prezimenu\n";
		cout << "6. Pretraga po registracijskim oznakama\n";
		cout << "7. Izmjena podataka\n";
		cout << "8. Kraj\n";
		cout << "Izbor: ";
		cin >> unos;
		cin.ignore();
	} while (unos < 1 || unos > 8);
	return unos;
}

int PrikazVlasnikaRB(Vlasnik *niz, int uneseno) {///
	int izbor;
	do{
	for (int i = 0; i < uneseno; i++){
		cout << i + 1 << ". " << niz[i]._imePrezime << ", vozila " << niz[i]._brojacVozila << "\n";
	}
	cout << "Unesite izbor: ";
	cin >> izbor;
	cin.ignore();
	} while (izbor < 1 || izbor > uneseno);
	izbor--;
	return izbor;
}

void UnosVlasnik(Vlasnik *&niz, int max, int &ukupno) {
	char nastavak = 'D';
	niz[ukupno]._brojacVozila = 0;
	while (ukupno < max && toupper(nastavak) == 'D'){
		niz[ukupno]._vozila[niz[ukupno]._brojacVozila] = new Vozilo;
		niz[ukupno].Unos();
		++ukupno;
	}
}

void UnosVozila(Vlasnik *&niz,int uneseno) {
	int izbor = PrikazVlasnikaRB(niz, uneseno);
	niz[izbor].DodajVozilo();
}

void PrikazVlasnika(Vlasnik *niz, int ukupno) {
	for (int i = 0; i < ukupno; i++){
		niz[i].Prikazi();
	}
}

void PretragaIiP(Vlasnik *niz, int uneseno) {
	char inicijali[30];
	bool pronadjen = false;
	cout << "Unesite inicijale vlasnika: \n";
	cin.getline(inicijali, 30);
	for (int i = 0; i < uneseno; i++){
		if (strcmp(niz[i]._imePrezime,inicijali) == 0){
			niz[i].Prikazi();
			pronadjen = true;
		}
	}
	if(pronadjen == false)
		cout << "Ne postoji vlasnik sa unesenim inicijalima\n";
}

void PretragaIiliP(Vlasnik *niz, int uneseno) {
	char inicijali[30];
	bool pronadjen = false;
	cout << "Unesite inicijale vlasnika: \n";
	cin.getline(inicijali, 30);
	for (int i = 0; i < uneseno; i++) {
		if (strstr(niz[i]._imePrezime, inicijali) != 0) {
			niz[i].Prikazi();
			pronadjen = true;
		}
	}
	if (pronadjen == false)
		cout << "Ne postoji vlasnik sa unesenim inicijalima\n";
}

void PretragaRegO(Vlasnik *niz, int uneseno) {
	int izbor = PrikazVlasnikaRB(niz, uneseno);
	char reg[10];
	cout << "Unesite registracijske oznake: \n";
	cin.getline(reg, 10);
	Vozilo *pok = niz[izbor].GetVozilo(reg);
	if (pok == nullptr){
		cout << "Auto sa tim registracisjkim onakama ne postoji\n";
	}
	else{
		pok->Prikazi();
	}
}

void Prepravka(Vlasnik*niz, int uneseno) {
	int izbor = PrikazVlasnikaRB(niz, uneseno);
	niz[izbor].Unos();
}