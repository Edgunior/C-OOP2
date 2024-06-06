

#include <iostream> 
#include <string> 
#include <vector> 
#include <algorithm> 
#include <fstream> 

using namespace std; 

class Vozilo {

private: 
	string marka, model; 
	float snagaMotora; 
protected: 
	string brojSasije; 

public: 
	Vozilo(string marka, string model, string brojSasije, float snagaMotora = 150.00) {
		this->marka = marka; 
		this->model = model; 
		this->brojSasije = brojSasije; 
		this->snagaMotora = snagaMotora;
	
	} 

	virtual bool proveriBrojSasije() const = 0; 

	friend ostream& operator << (ostream& out, const Vozilo& v) {
	
		return out << v.marka << ", " << v.model << ", " << v.brojSasije 
			<< ", " << v.snagaMotora << endl;
	}

	float getSnagaMotora() { return this->snagaMotora; }

}; 

class Dvotockas : public Vozilo {

public: 
	using Vozilo::Vozilo; 

	bool proveriBrojSasije() const override {
	
		int n = brojSasije.length(); 

		if (n < 4) return false; 
		if (any_of(brojSasije.begin(), brojSasije.end(), [](char c) {return !isdigit(c) && !islower(c); }))
			return false;
	}


};

class Cetvorotockas : public Vozilo {

public:
	using Vozilo::Vozilo;

	bool proveriBrojSasije() const override {

		int n = brojSasije.length();

		if (n < 4) return false;
		if (any_of(brojSasije.begin(), brojSasije.end(), 
			[](char c) {return !isdigit(c) && !isupper(c); }))
			return false;
	}

};

int main() { 


	ifstream in("input.txt"); 
	ofstream out("output.txt"); 

	if (!in.is_open()) return EXIT_FAILURE; 

	vector<Vozilo*> vozila;  

	/*
		To Do 
		citanje iz fajla
	*/


	sort(vozila.begin(), vozila.end(),
		[](Vozilo* v1, Vozilo* v2) {return v1->getSnagaMotora() > v2->getSnagaMotora(); });




}