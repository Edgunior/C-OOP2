
#include <iostream> 
#include <fstream> 
#include <algorithm>
#include <vector> 
#include <string> 

using namespace std; 


class Osoba {

private: 
	string ime, prezime, jmbg; 

public: 
	Osoba(string jmbg, string ime = "Petar", string prezime = "Petrovic") {
		
		this->jmbg = (jmbg.length() == 13 && all_of(jmbg.begin(), jmbg.end(),
			[](char c) {return isdigit(c); })) ? jmbg : "0000000000000";
		this->ime = ime; 
		this->prezime = prezime;
	}

	virtual void ispisiPodatke(ostream& out) const {
		out << "[" << ime << ", " << prezime << ", " << jmbg << " ]" << endl;
	 
	} 

	friend ostream& operator << (ostream& out, const Osoba& o) {
		o.ispisiPodatke(out); 
		return out;
	}

	string getJmbg() { return this->jmbg; }

}; 

class Razredni; 
class Ucenik : public Osoba {

	Razredni* razredni = nullptr; 

public: 
	using Osoba::Osoba; 

	void postaviRazrednog(Razredni* razredni) {
		this->razredni = razredni;
	}

}; 

class Razredni : public Osoba {

	vector <Ucenik*> ucenici;

public:
	using Osoba::Osoba;

	Razredni& operator += (Ucenik* ucenik) {
	
		ucenik->postaviRazrednog(this); 
		ucenici.push_back(ucenik); 

		return *this;
	}

	virtual void ispisiPodatke(ostream& out) const override {
	
		Osoba::ispisiPodatke(out); 
		out << endl; 

		for (auto itr = ucenici.begin(); itr != ucenici.end(); itr++) {
			Ucenik* ucenik = *itr; 
			out << *ucenik;
		
		}
	} 

	int brojValidnih() {
		return count_if(ucenici.begin(), ucenici.end(), [](Ucenik* u)
			{return u->getJmbg() != "0000000000000"; });
	} 

	void particionisanje() {
		partition(ucenici.begin(), ucenici.end(), [](Ucenik* u)
			{return u->getJmbg() != "0000000000000"; });
			
	}

};

int main() { 

	Razredni razredni("1234567890123", "Razredni", "Razrednic"); 

	ifstream in("input.txt"); 
	ofstream out("output.txt"); 

	if (!in.is_open()) return EXIT_FAILURE; 

	while (!in.eof()) {
		string jmbg, ime, prezime; 
		in >> jmbg >> ime >> prezime;
		 
		if (ime == "$DEF$" || prezime == "$DEF$") razredni += new Ucenik(jmbg);
		else razredni += new Ucenik(ime, prezime, jmbg);

	} 

	razredni.particionisanje(); 
	Osoba* osoba = &razredni; 
	out << *osoba << "Broj ucenika sa validnim jmbg je " << razredni.brojValidnih() << endl;

	return 0;
}