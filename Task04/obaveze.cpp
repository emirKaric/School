#include <iostream>
#include <fstream>
using namespace std;

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };

struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; _dan = NULL; delete _mjesec; _mjesec = NULL; delete _godina; _godina = NULL; }
};
struct ObavezeNaPredmetu {
	VrstaObaveze * _vrstaObaveze;
	Datum * _datumIzvrsenja;
	char * _napomena;
	int _ocjena; // 5 - 10 
	void Unos(VrstaObaveze vrsta, Datum * datum, int ocjena, const char * napomena) {
		_vrstaObaveze = new VrstaObaveze(vrsta);
		_datumIzvrsenja = new Datum;
		_datumIzvrsenja->Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(napomena) + 1;
		_napomena = new char[size];
		strcpy_s(_napomena, size, napomena);
	}
	void Ispis() {
		cout << *_vrstaObaveze << " " << _ocjena << " " << _napomena;
		_datumIzvrsenja->Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja->Dealociraj();
		delete _vrstaObaveze; _vrstaObaveze = nullptr;
		delete _datumIzvrsenja; _datumIzvrsenja = nullptr;
		delete[] _napomena; _napomena = nullptr;
	}
};
struct PolozeniPredmet {
	Datum _datumPolaganja; //datum koji ce se evidentirati kao datum kada je predmet polozen tj. kada je formirana konacna ocjena
	char * _nazivPredmeta;
	ObavezeNaPredmetu * _listaIzvrsenihObaveza[10];
	int _trenutnoIzvrsenihObaveza;
	int _konacnaOcjena;
	//formira se na osnovu ocjena izvrsenih obaveza

						/*
						1. Unos - na osnovu vrijednosti primljenog parametra izvrsiti inicijalizaciju odgovarajucih atributa
						2. DodajIzvrsenuObavezu � na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu. Potrebno je onemoguciti dodavanje identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci najmanje 7 dana. Identicna vrsta obaveze se moze dodati samo u slucaju da se radi o Seminarskom ili je prethodno dodana obaveza (identicne vrste)
						imala ocjenu 5. Ukoliko je moguce, osigurati prosirenje niza na nacin da prihvati vise od 10 obaveza.
						3. FormirajKonacnuOcjenu - konacna ocjene predstavlja prosjecnu ocjenu na predmetu, a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita. Ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena najmanje dva seminarska rada. U slucaju da bilo koji od navedenih uslova nije zadovoljen konacna ocjena treba biti postavljena na vrijednost 5. Konacna ocjena, takodjer, ne smije biti formirana u slucaju da u napomeni od dvije obaveze stoji rijec 'prepisivao' ili 'izbacen'. Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, kao datum polaganja je potrebno postaviti na vrijednost datuma posljednje izvrsene obaveze koja je usla u formiranje ocjene.
						4. Ispis � ispsuje vrijednosti svih atributa strukture
						*/

	void Unos(const char* naziv) {
		int vel = strlen(naziv) + 1;
		_nazivPredmeta = new char[vel];
		strcpy_s(_nazivPredmeta, vel, naziv);
		_trenutnoIzvrsenihObaveza = 0;
		_konacnaOcjena = 5;
	}

	bool DodajIzvrsenuObavezu(VrstaObaveze vrsta, Datum datum, int ocjena, const char *napomena) {
		if (vrsta == Seminarski){
			_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza] = new ObavezeNaPredmetu;
			_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza]->Unos(vrsta, &datum, ocjena, napomena);
			++_trenutnoIzvrsenihObaveza;
			return true;
		}
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++){
			if (vrsta == *_listaIzvrsenihObaveza[i]->_vrstaObaveze && _listaIzvrsenihObaveza[i]->_ocjena > 5) {
				return false;
			}
		}
		_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza] = new ObavezeNaPredmetu;
		_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza]->Unos(vrsta, &datum, ocjena, napomena);
		++_trenutnoIzvrsenihObaveza;
		return true;
	}
	int getOcjenaByObaveza(VrstaObaveze o, bool sum=false) {
		int suma = 0, cnt = 0;
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++) 
			if (*_listaIzvrsenihObaveza[i]->_vrstaObaveze == o && _listaIzvrsenihObaveza[i]->_ocjena > 5) {
				if(!sum)return _listaIzvrsenihObaveza[i]->_ocjena;
				suma += _listaIzvrsenihObaveza[i]->_ocjena;
				++cnt;
			}
		return (sum && cnt > 1) ? suma / cnt : 0;
	}
	int Okoncaj(int ocjenu,int plus) {
		if (ocjenu)
			if (int seminarski = getOcjenaByObaveza(Seminarski, true)) {
				_konacnaOcjena = (ocjenu + seminarski)/plus;
				_datumPolaganja.Unos(*_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza - 1]->_datumIzvrsenja->_dan, *_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza - 1]->_datumIzvrsenja->_mjesec, *_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza - 1]->_datumIzvrsenja->_godina);
			}
		return _konacnaOcjena;
	}

	int FormirajKonacnuOcjenu(int ocjena = 0) {
		return ((ocjena = getOcjenaByObaveza(Parcijalni1) + getOcjenaByObaveza(Parcijalni2)) > 11) ? Okoncaj(ocjena, 3) : Okoncaj(getOcjenaByObaveza(Integralni), 2);
	}

		void Ispis() {
		cout << "Naziv predmeta: " << _nazivPredmeta << "\n";
		cout << "Datum polaganja je: "; _datumPolaganja.Ispis();
		cout << "Konacna ocjena: " << _konacnaOcjena << "\n";
		cout << "Izvrseno obaveza: " << _trenutnoIzvrsenihObaveza << "\n";
		cout << "Obaveze su: \n\n";
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++){
			_listaIzvrsenihObaveza[i]->Ispis();
			cout << "\n=====================\n";
		}
	}
};

/*
1. UpisiUBinarniFajl - objekat koji je primljen kao parametar upisuje u binarni fajl. Naziv fajla treba biti sastavljen od naziva predmeta i konacne ocjena npr. "Programiranje II_6"
2. UcitajIzBinarnogFajla - u objekat koji je primljen kao parametar ucitava sadrzaj binarnog fajla (naziv fajla je takodjer proslijedjen kao parametar)
3. PretragaRekurzivno - rekurzivna funkcija koja treba da vrati broj obaveza kod koji je student ostvario negativnu ocjenu
*/

int PretragaRekurzivno(const PolozeniPredmet &pr, int trenutno, int ukupno) {
	if (trenutno == ukupno){
		return 0;
	}
	return (pr._listaIzvrsenihObaveza[trenutno]->_ocjena <= 5) ? 1 + PretragaRekurzivno(pr, ++trenutno, ukupno) : 0 + PretragaRekurzivno(pr, ++trenutno, ukupno);
}

void UpisiUBinarniFajl(PolozeniPredmet prII) {
	char ime[13];
	strcpy_s(ime, "file");
	ofstream upis(ime,ios::out | ios::binary);
	upis.write((char*)&prII, sizeof(PolozeniPredmet));
	upis.close();
}

void UcitajIzBinarnogFajla(PolozeniPredmet &prII,const char *ime) {
	ifstream ispis(ime, ios::in | ios::binary);
	ispis.read((char*)&prII, sizeof(PolozeniPredmet));
	ispis.close();
}



int main() {
	/*
	1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
	2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	3. KREIRAJTE .DOC FAJL SA VA�IM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJE�ENJA VA�IH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
	4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORI�TENJE HELP-A
	5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VA�A RJE�ENJA)
	6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URA�ENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
	*/

	Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2, datumIntegralni;
	datumSeminarski1.Unos(10, 6, 2016);
	datumSeminarski2.Unos(18, 6, 2016);
	datumParcijalni1.Unos(22, 3, 2016);
	datumParcijalni2.Unos(22, 6, 2016);

	//polozeni predmet
	PolozeniPredmet prII, prII_FromFile;
	prII.Unos("Programiranje II");
	if (prII.DodajIzvrsenuObavezu(Parcijalni1, datumParcijalni1, 6, "uslovno polozen"))
		cout << "Parcijalni 1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 5, "previse gresaka"))
		cout << "Parcijalni 2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski1, 8, "implementirani svi algoritmi"))
		cout << "Seminarski1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski2, 6, "rad slican kao kod studenta IB150388"))
		cout << "Seminarski2...dodan" << endl;

	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;

	datumParcijalni2.Unos(22, 7, 2016);
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 6, ""))
		cout << "Parcijalni 2...dodan" << endl;
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;
	//rekurzija
	cout << "Broj negativnih ocjena je: " << PretragaRekurzivno(prII, 0,prII._trenutnoIzvrsenihObaveza) << endl;
	////manipulacija fajlovima
	UpisiUBinarniFajl(prII);
	prII.Ispis();
	UcitajIzBinarnogFajla(prII_FromFile, "file");
	prII_FromFile.Ispis();
	datumSeminarski1.Dealociraj();  datumSeminarski2.Dealociraj(); datumParcijalni1.Dealociraj(); datumParcijalni2.Dealociraj(); datumIntegralni.Dealociraj();
	
	return 0;
}s