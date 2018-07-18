#include<iostream>
#include<assert.h>
using namespace std;

struct Datum {
	int *_dan, *_mjesec, *_godina;

	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}

	void Dealociraj() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}

	void Ispis() {
		cout << _dan << "." << _mjesec << "." << _godina << "\n";
	}
};
struct Autor {
	char *_ime;
	char *_prezime;
	char *_mjestoRodjenja;
	Datum datumRodjenja;

	void Unos(const char *ime, const char *prezime, const char *mjesto, Datum datum) {
		_ime = new char[strlen(ime) + 1];
		strcpy_s(_ime, strlen(ime) + 1, ime);
		_prezime = new char[strlen(prezime) + 1];
		strcpy_s(_prezime, strlen(prezime) + 1, prezime);
		_mjestoRodjenja = new char[strlen(mjesto) + 1];
		strcpy_s(_mjestoRodjenja, strlen(mjesto) + 1, mjesto);
		datumRodjenja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
	}

	void Dealociraj(){
		delete[]_ime; _ime = nullptr;
		delete[]_prezime; _prezime = nullptr;
		delete[]_mjestoRodjenja; _mjestoRodjenja = nullptr;
		datumRodjenja.Dealociraj();
	}

	void Ispis() {
		cout << "Ime autora: " << _ime << "\n";
		cout << "Prezime autora: " << _prezime << "\n";
		cout << "Mjesto rodjenja autora: " << _mjestoRodjenja << "\n";
		cout << "Datum rodjenja autora: "; datumRodjenja.Ispis();
	}
};
struct Knjiga {
	char *_naziv;
	char *_opis;
	Datum datumIzdavanja;
	Autor _autor;

	void Unos(const char *naziv, const char *opis, Datum izdavanje, Autor autor) {
		_naziv = new char[strlen(naziv) + 1];
		strcpy_s(_naziv, strlen(naziv) + 1, naziv);
		_opis = new char[strlen(opis) + 1];
		strcpy_s(_opis, strlen(opis) + 1, opis);
		datumIzdavanja.Unos(*izdavanje._dan, *izdavanje._mjesec, *izdavanje._godina);
		_autor.Unos(autor._ime, autor._prezime, autor._mjestoRodjenja, autor.datumRodjenja);
	}

	void Unos(Knjiga knjiga) {
		Unos(knjiga._naziv,knjiga._opis,knjiga.datumIzdavanja, knjiga._autor);
	}
	
	void Dealociraj() {
		delete[]_naziv; _naziv = nullptr;
		delete[]_opis; _opis = nullptr;
		datumIzdavanja.Dealociraj();
		_autor.Dealociraj();
	}

	void Ispis() {
		cout << "Ime knjige: " << _naziv << "\n";
		cout << "Opis knjge: " << _opis << "\n";
		cout << "Datum izdavanja: "; datumIzdavanja.Ispis();
		cout << "Autor:\n";
		_autor.Ispis();
	}
};

struct Biblioteka {
	char *_grad;
	Knjiga *_knjige;
	int _trenutnoKnjiga;
	int _maxKnjiga;

	void Unos(const char*grad, const int max) {
		_grad = new char[strlen(grad) + 1];
		strcpy_s(_grad, strlen(grad) + 1, grad);
		_maxKnjiga = max;
		_trenutnoKnjiga = 0;
		_knjige = new Knjiga[max];
	}

	void Dealociraj() {
		delete[]_grad;
		_grad = nullptr;
		_knjige->Dealociraj();
		delete[]_knjige;
		_knjige = nullptr;
	}

	void DodajKnjigu(Knjiga knjiga) {
		for (int i = 0; i < _trenutnoKnjiga; i++){
			if (strcmp(knjiga._naziv, _knjige[i]._naziv) == 0){
				return;
			}
		}
		if (_trenutnoKnjiga < _maxKnjiga) {
			_knjige[_trenutnoKnjiga].Unos(knjiga);
			++_trenutnoKnjiga;
		}
	}

	Knjiga *PretragaPoNazivu(const char* naziv) {
		Knjiga *nazivK = nullptr;
		for (int i = 0; i < _trenutnoKnjiga; i++){
			if (strcmp(naziv, _knjige[i]._naziv) == 0){
				nazivK = &_knjige[i];
			}
		}
		if (naziv != nullptr){
			return nazivK;
		}
		return nullptr;
	}

	bool Ukloni(int redniBroj) {
		int index = redniBroj - 1;
		if (index >= 0 && index < _trenutnoKnjiga){
			_knjige[index].Dealociraj();
			_knjige[index];
			for (int i = index; i < _trenutnoKnjiga -1; i++){
				_knjige[i] = _knjige[i + 1];
			}
			++_trenutnoKnjiga;
			return true;
		}
		return false;
	}
};

int main() {
	Datum datum;
	datum.Unos(11, 12, 1234);
	datum.Ispis();

	Autor autor;
	autor.Unos("Cola", "Colic", "Capina", datum);
	autor.Ispis();
	Autor autor2;
	autor2.Unos("Murat", "Muratic", "Tinja", datum);
	autor2.Ispis();

	Knjiga knjiga, knjiga1, knjiga2;
	knjiga.Unos("Alisa u Zemlji cuda", " darica ", datum, autor);
	knjiga.Ispis();
	knjiga1.Unos("Cola darica", "daretina", datum, autor);
	knjiga.Ispis();
	knjiga2.Unos(knjiga1);
	knjiga2.Ispis();

	Biblioteka StaraBiblioteka;
	StaraBiblioteka.Unos("Mostar", 1000);
	StaraBiblioteka.DodajKnjigu(knjiga1);
	StaraBiblioteka.DodajKnjigu(knjiga2);
	Knjiga * k = StaraBiblioteka.PretragaPoNazivu("Col");
	if (k != nullptr) {
		cout << "Pretragom po nazivu,pronadjena knjiga -> ";
		k->Ispis();
	}
	else
		cout << "Knjiga nije pronadjena!!\n";
	//ostalo testiranje funkcije ukloni


	//dealociraj zauzetu memoriju
	datum.Dealociraj();
	autor.Dealociraj();
	autor2.Dealociraj();
	knjiga.Dealociraj();
	knjiga1.Dealociraj();
	knjiga2.Dealociraj();
	StaraBiblioteka.Dealociraj();
	system("Pause");
	return 0;
}