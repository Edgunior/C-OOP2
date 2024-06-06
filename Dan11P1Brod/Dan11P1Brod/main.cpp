#include <iostream> 
#include <string> 
#include <fstream> 
#include <algorithm> 
#include <vector> 

using namespace std; 

class Brod {
private:
	string marka, model; 
	float snagaMotora; 

protected: 
	string serijskiBroj; 

public: 
	Brod(string marka, string model, string serijskiBroj, float snagaMotora = 100.00) {
		this->marka = marka; 
		this->model = model; 
		this->serijskiBroj = serijskiBroj;
		this->snagaMotora = snagaMotora;
	} 

	virtual bool serijskiBrojValidan() const = 0;  

	friend ostream& operator <<(ostream& out, const Brod& b) {
		return out << b.marka << " " << b.model << " " << b.serijskiBroj << " " << b.snagaMotora << endl;
	}

	virtual void ispisiPodatke(ostream& out) const {
		out << "[" << marka << " " << model << " " << serijskiBroj << " " << snagaMotora << endl;

	}


	float getSnagaMotora() { return this->snagaMotora; }
}; 


class Camac : public Brod {
public: 
	using Brod::Brod; 

	bool serijskiBrojValidan() const override {
		
		string s = this->serijskiBroj; 

		if (any_of(s.begin(), s.end(), [](char c) {return !isdigit(c) && !islower(c); }))
			return false; 
		return true;
	} 

	void ispisiPodatke(ostream& out) const { 
		
		Brod::ispisiPodatke(out);
	}
}; 

class Jedrilica : public Brod {

public: 
	using Brod::Brod; 

	bool serijskiBrojValidan() const override {
		
		string s = this->serijskiBroj; 

		if (!none_of(s.begin(), s.end(), [](char c) {return !isdigit(c) && !isupper(c); }))
			return false; 

		return true;
	} 

	void ispisiPodatke(ostream& out) const {

		Brod::ispisiPodatke(out);
	}

};

int main() { 

	ifstream in("input.txt");
	ofstream out("output.txt");

	if (!in.is_open()) return EXIT_FAILURE;

	vector<Brod*> brodovi;
	while (!in.eof()) {
		char oznaka;
		string marka, model, serijskiBroj;
		float snagaMotora;

		in >> oznaka >> marka >> model >> serijskiBroj >> snagaMotora;

		Brod* brod;

		if (oznaka == 'J') {
			if (snagaMotora == -1) brod = new Jedrilica(marka, model, serijskiBroj);
			else brod = new Jedrilica(marka, model, serijskiBroj, snagaMotora);
		}
		else {
			if (snagaMotora == -1) brod = new Camac(marka, model, serijskiBroj);
			else brod = new Camac(marka, model, serijskiBroj, snagaMotora);

		}
		brodovi.push_back(brod);

	}

	stable_sort(brodovi.begin(), brodovi.end(), [](Brod* b1, Brod* b2)
		{return b1->getSnagaMotora() > b2->getSnagaMotora(); });

	Brod* bb = *max_element(brodovi.begin(), brodovi.end(), [](Brod* b1, Brod* b2)
		{return b1->getSnagaMotora() < b2->getSnagaMotora(); });

	if (bb->serijskiBrojValidan())
		bb->ispisiPodatke(out);
	else
		cout << "Brod sa najvecom snagom motora ima nevalidan serijski broj" << endl;


	return 0;
}