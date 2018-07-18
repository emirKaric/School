#include <iostream>
#include <fstream>
using namespace std;

enum Opstina { Opstina1, Opstina2, Opstina3, Opstina4 };
const char * OpstineChar[] = { "Opstina1", "Opstina2", "Opstina3", "Opstina4" };
enum Kandidati { Kandidat1, Kandidat2, Kandidat3, Kandidat4 };
const char * KandidatiChar[] = { "Kandidat1", "Kandidat2", "Kandidat3", "Kandidat4" };

struct Datum {
	int _dan, _mjesec, _godina;
	void Unos(int d, int m, int g) { _dan = d; _mjesec = m; _godina = g; }
	void Ispis() { cout << _dan << "/" << _mjesec << "/" << _godina << endl; }
	
	bool getGodine(Datum d) {
		int g = d._godina - _godina;
		if (g > 18){
			return true;
		}
		else if (g == 18 && _mjesec > d._mjesec) {
			return true;
		}
		return false;
	}

	bool uslov(Datum d, int x) {
		if (x == 1){
			if (d._godina > _godina)
				return true;
			else if (d._godina == _godina && d._mjesec > _mjesec)
				return true;
			else if (d._godina == _godina && d._mjesec == _mjesec && d._dan >= _dan)
				return true;
			return false;
		}
		else{
			if (d._godina < _godina)
				return true;
			else if (d._godina == _godina && d._mjesec < _mjesec)
				return true;
			else if (d._godina == _godina && d._mjesec == _mjesec && d._dan <= _dan)
				return true;
			return false;
		}
	}

	/*bool VeciDatum(Datum d){
		if (d._godina < _godina)
				return true;
		else if (d._godina == _godina && d._mjesec < _mjesec)
				return true;
		else if (d._godina == _godina && d._mjesec == _mjesec && d._dan <= _dan)
				return true;
		return false;
	}
       bool ManjiDatum(Datum d){
	return !VeciDatum(d);
	}*/
};

Datum d;
struct Osoba {
	Datum _datumRodjenja;
	char * _imePrezime;
	char _JMBG[14];
	Opstina _Prebivaliste;
	void unos(Datum d, const  char * ip, const char jmbg[], Opstina p) {
		_datumRodjenja = d;
		_imePrezime = new char[strlen(ip) + 1];
		strcpy_s(_imePrezime, strlen(ip) + 1, ip);
		strcpy_s(_JMBG, jmbg);
		_Prebivaliste = p;
	}
	void Dealociraj() { delete[] _imePrezime; _imePrezime = nullptr; }
	void Info() {
		_datumRodjenja.Ispis();
		cout << _imePrezime << " " << _JMBG << " " <<
			OpstineChar[_Prebivaliste] << endl;
	}
};
struct Glas {
	Osoba * _glasac;
	Kandidati * _glasZa;
	void Unos(Osoba o, Kandidati k) {
		_glasac = new Osoba;
		_glasac->unos(o._datumRodjenja, o._imePrezime, o._JMBG, o._Prebivaliste);
		_glasZa = new Kandidati(k);
	}
	void Ispis() {
		_glasac->Info();
		cout << KandidatiChar[*_glasZa] << endl;
	}
	void Dealociraj() { _glasac->Dealociraj(); delete _glasac; delete _glasZa;}
};
struct Izbori2016 {
	Opstina * _glasackoMjesto;
	Glas * _glasovi[1000];
	int _doSadaGlasalo;
	/*
	1. Unos - na osnovu vrijednosti primljenih parametara, inicijalizovati vrijednosti atributa strukture.
	2. DodajGlas - funkcija treba da omoguci dodavanje informacija o novom glasu i tom prilikom onemoguciti: da ista osoba glasa vise puta, glasanje osobama mladjim od 18 godina (uzeti u obzir samo mjesec i godinu rodjenja), glasanje osobama iz drugih opstina. U zavisnosti od uspjesnosti operacije funkcija vraca true ili false
	3. Ispis - u zavisnosti od postignutog uspjeha funkcija treba ispisati listu kandidata sortiranu opadajucim redoslijedom, a uz svakog kandidata je potrebno ispisati i osvojeni procenat glasova.
	Funkcija vraca opstinu u kojoj je pobjednicki kandidat ostvario najveci broj glasova, a ukoliko je pobjednicki kandidat ostvario podjednak broj glasova u vise opstina, funkcija vraca prvu koja se nalazi u nizu na koji pokazuje pokazivac izbornaMjesta
	4. BrojGlasacaPoRodjenju - funkcija vraca broj glasaca koji su do tog momenta glasali, a koji su rodjeni u parametrima definisanom periodu (parametri su tipa datum, period OD i DO)
	*/

	void Unos(Opstina opstina) {
		_glasackoMjesto = new Opstina(opstina);
		_doSadaGlasalo = 0;
	}

	bool DodajGlas(Glas glas) {
		if (*_glasackoMjesto != glas._glasac->_Prebivaliste || glas._glasac->_datumRodjenja.getGodine(d)  == false){
			return false;
		}
		for (int i = 0; i < _doSadaGlasalo; i++){
			if (strcmp(glas._glasac->_imePrezime, _glasovi[i]->_glasac->_imePrezime) == 0){
				return false;
			}
		}
		_glasovi[_doSadaGlasalo] = new Glas;
		_glasovi[_doSadaGlasalo]->Unos(*glas._glasac, *glas._glasZa);
		++_doSadaGlasalo;
		cout << "Dodano!!!\n";
		return true;
	}

	void Ispis() {
		cout << "Glasacko mjesto: " << (char)*_glasackoMjesto << "\n";
		for (int i = 0; i < _doSadaGlasalo; i++){
			_glasovi[i]->Ispis();
		}
	}

	int BrojGlasacaPoRodjenju(Datum OD, Datum DO) {
		int brojac = 0;
		for (int i = 0; i < _doSadaGlasalo; i++){
			if (_glasovi[i]->_glasac->_datumRodjenja.uslov(OD,1) == true && _glasovi[i]->_glasac->_datumRodjenja.uslov(DO, 2) == true) {
				++brojac;
			}
		}
		return brojac;
	}
	void Dealociraj() {
		delete _glasackoMjesto;
		_glasackoMjesto = nullptr;
		for (int i = 0; i < _doSadaGlasalo; i++){
			_glasovi[i]->Dealociraj();
			delete _glasovi[i];
			_glasovi[i] = nullptr;
		}
	}
};
/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti identican nazivu opstine u kojoj se odrzavaju izbori
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj glasova koje je na izborima ostvario odredjeni kandidat (kandidat se proslijedjuje kao parametar)
*/

int PretragaRekurzivno(Izbori2016 izbori, Kandidati kandidat, int trenutno = 0) {
	if (trenutno == izbori._doSadaGlasalo){
		return 0;
	}
	return (*izbori._glasovi[trenutno]->_glasZa == kandidat) ? 1 + PretragaRekurzivno(izbori, kandidat,trenutno + 1) : 0 + PretragaRekurzivno(izbori, kandidat, trenutno + 1);
}

void UpisiUBinarniFajl(Izbori2016 &izbori) {
	ofstream upis("Opstina1", ios::out | ios::binary);
	upis.write((char*)&izbori, sizeof(Izbori2016));
	upis.close();
}
void UcitajIzBinarnogFajla(Izbori2016 &izbori, const char*ime) {
	ifstream ispis(ime, ios::in | ios::binary);
	ispis.read((char*)&izbori, sizeof(Izbori2016));
	ispis.close();
}


int main() {
	/*
	1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
	2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
	4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
	5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
	6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datumGlasac1, datumGlasac2, datumGlasac3, datumGlasac4, datumIzbora;
	datumIzbora.Unos(14, 7, 2016);
	d = datumIzbora;
	datumGlasac1.Unos(18, 5, 1990);
	datumGlasac2.Unos(5, 3, 1982);
	datumGlasac3.Unos(12, 8, 1958);
	datumGlasac4.Unos(22, 6, 2000);

	Osoba a, b, c, d, e;
	a.unos(datumGlasac1, "Denis Music", "1111111111111", Opstina1);
	b.unos(datumGlasac2, "Jasmin Azemovic", "1111111111112", Opstina1);
	c.unos(datumGlasac3, "Indira Hamulic", "1111111111113", Opstina1);
	d.unos(datumGlasac4, "Adel Handzic", "1111111111114", Opstina2);
	e.unos(datumGlasac4, "Zanin Vejzovic", "1111111111115", Opstina1);


	Glas g1, g2, g3, g4, g5;
	g1.Unos(a, Kandidat1);
	g2.Unos(b, Kandidat1);
	g3.Unos(c, Kandidat2);
	g4.Unos(d, Kandidat1);
	g5.Unos(e, Kandidat2);

	Izbori2016 izbori, izbori_FromFile;
	izbori.Unos(Opstina1);

	izbori.DodajGlas(g1);
	izbori.DodajGlas(g2);
	izbori.DodajGlas(g3);
	izbori.DodajGlas(g4);
	izbori.DodajGlas(g5);
	izbori.Ispis();

	Datum OD, DO;
	OD.Unos(17, 7, 1998);
	DO.Unos(17, 7, 1986);
	cout << "Glasaca u starost od 18 - 30 godina->" << izbori.BrojGlasacaPoRodjenju(OD, DO) << endl;
	cout << "Broj glasova za Kandidat1->" << PretragaRekurzivno(izbori,Kandidat1) << endl;
	UpisiUBinarniFajl(izbori);
	UcitajIzBinarnogFajla(izbori_FromFile, "Opstina1");
	izbori_FromFile.Ispis();
	a.Dealociraj(), b.Dealociraj(), c.Dealociraj(), d.Dealociraj(), e.Dealociraj();
	g1.Dealociraj(), g2.Dealociraj(), g3.Dealociraj(), g4.Dealociraj(), g5.Dealociraj();
	izbori.Dealociraj();
	return 0;
}