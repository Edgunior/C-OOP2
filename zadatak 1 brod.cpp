#include <iostream>     // Include standard input/output stream library
#include <string>       // Include string library
#include <vector>       // Include vector library
#include <algorithm>    // Include algorithm library for functions like stable_sort
#include <fstream>      // Include file stream library

using namespace std;    // Use the standard namespace to avoid prefixing standard library names with std::

class Brod {
private:
    string marka, model;    // Private members for the boat's brand and model
    float snagaMotora;      // Private member for the boat's engine power

protected:
    string serijskiBroj;    // Protected member for the boat's serial number

public:
    // Constructor to initialize brand, model, serial number, and optionally engine power
    Brod(string marka, string model, string serijskiBroj, float snagaMotora = 100.00)
        : marka(marka), model(model), serijskiBroj(serijskiBroj), snagaMotora(snagaMotora) {}

    // Pure virtual function to check if the serial number is valid (must be overridden in derived classes)
    virtual bool serijskiBrojValidan() const = 0;

    // Overload the << operator to print boat details
    friend ostream& operator<<(ostream& out, const Brod& b) {
        return out << b.marka << ", " << b.model << ", " << b.serijskiBroj << ", " << b.snagaMotora << endl;
    }

    // Virtual function to print boat details
    virtual void ispisiPodatke(ostream& out) const {
        out << "[" << marka << ", " << model << ", " << serijskiBroj << ", " << snagaMotora << " ]" << endl;
    }

    // Getter for engine power
    float getSnagaMotora() const {
        return this->snagaMotora;
    }
};

class Camac : public Brod {
public:
    using Brod::Brod;  // Inherit constructors from the base class

    // Override function to check if the serial number contains only digits or lowercase letters
    bool serijskiBrojValidan() const override {
        string s = this->serijskiBroj;
        return all_of(s.begin(), s.end(), [](char c) { return isdigit(c) || islower(c); });
    }

    // Override function to print boat details using the base class implementation
    void ispisiPodatke(ostream& out) const override {
        Brod::ispisiPodatke(out);
    }
};

class Jedrilica : public Brod {
public:
    using Brod::Brod;  // Inherit constructors from the base class

    // Override function to check if the serial number contains only digits or uppercase letters
    bool serijskiBrojValidan() const override {
        string s = this->serijskiBroj;
        return all_of(s.begin(), s.end(), [](char c) { return isdigit(c) || isupper(c); });
    }

    // Override function to print boat details using the base class implementation
    void ispisiPodatke(ostream& out) const override {
        Brod::ispisiPodatke(out);
    }
};

int main() {
    ifstream in("input.txt");     // Create an input file stream object to read from "input.txt"
    ofstream out("output.txt");   // Create an output file stream object to write to "output.txt"

    if (!in.is_open()) return EXIT_FAILURE;   // Check if the input file is open. If not, return failure status.

    vector<Brod*> brodovi;    // Vector to store pointers to Brod objects
    char oznaka;              // Variable to store the boat type ('J' for Jedrilica, any other for Camac)
    string marka, model, serijskiBroj;  // Variables to store the boat's brand, model, and serial number
    float snagaMotora;        // Variable to store the engine power

    // Read the boat details from the input file
    while (in >> oznaka >> marka >> model >> serijskiBroj >> snagaMotora) {
        if (!in.good()) break;  // Check for any reading errors

        if (snagaMotora == -1) snagaMotora = 100.00;  // If engine power is -1, set it to the default value of 100.00

        Brod* brod = nullptr;   // Pointer to a Brod object
        if (oznaka == 'J') {    // Check the type of boat based on oznaka
            brod = new Jedrilica(marka, model, serijskiBroj, snagaMotora);  // Create a new Jedrilica object if oznaka is 'J'
        }
        else {
            brod = new Camac(marka, model, serijskiBroj, snagaMotora);  // Create a new Camac object for any other oznaka
        }

        brodovi.push_back(brod);  // Add the created boat object to the brodovi vector
    }

    // Check if the brodovi vector is not empty
    if (!brodovi.empty()) {
        // Sort the boats in descending order of engine power using stable_sort
        stable_sort(brodovi.begin(), brodovi.end(), [](Brod* b1, Brod* b2) {
            return b1->getSnagaMotora() > b2->getSnagaMotora();
            });

        Brod* maxBrod = brodovi.front();  // Get the boat with the maximum engine power (the first boat after sorting)

        // Check if the serial number of the boat with the maximum engine power is valid
        if (maxBrod->serijskiBrojValidan()) {
            maxBrod->ispisiPodatke(out);  // Print the details of the boat with the maximum engine power to the output file if the serial number is valid
        }
        else {
            cout << "Brod sa najvecom snagom motora ima nevalidan serijski broj" << endl;  // Print an error message to the console if the serial number is invalid
        }

        // Delete all dynamically allocated boat objects to free memory
        for (Brod* brod : brodovi) {
            delete brod;
        }
    }

    in.close();  // Close the input file
    out.close(); // Close the output file

    return 0;  // Return 0 to indicate successful execution of the program
}
