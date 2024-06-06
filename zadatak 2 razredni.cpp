#include <iostream> // Include the iostream library for input and output operations
#include <fstream>  // Include the fstream library for file input and output operations
#include <algorithm> // Include the algorithm library for standard algorithms like all_of and partition
#include <vector>  // Include the vector library for using the vector container
#include <string>  // Include the string library for using the string class
using namespace std; // Use the standard namespace

// Forward declaration of the Ucenik class
class Ucenik;

// Definition of the Osoba (Person) class
class Osoba {
private:
    string ime, prezime, jmbg; // Private member variables: first name, last name, and JMBG

public:
    // Constructor with default values for ime (name) and prezime (surname)
    Osoba(string jmbg, string ime = "Petar", string prezime = "Petrovic") {
        // Check if jmbg is 13 digits long and contains only digits, otherwise set to "0000000000000"
        this->jmbg = (jmbg.length() == 13 && all_of(jmbg.begin(), jmbg.end(),
            [](char c) {return isdigit(c);})) ? jmbg : "0000000000000";
        this->ime = ime; // Set the first name
        this->prezime = prezime; // Set the last name
    }

    // Virtual function to print object data
    virtual void ispisiPodatke(ostream& out) const {
        out << "[ " << ime << ", " << prezime << ", " << jmbg << " ]" << endl;
    }

    // Overloaded << operator for printing Osoba objects
    friend ostream& operator << (ostream& out, const Osoba& o) {
        o.ispisiPodatke(out); // Call the virtual function to print data
        return out; // Return the output stream
    }

    // Getter function for jmbg
    string getJmbg() const {
        return this->jmbg;
    }
};

// Definition of the Razredni (Class Teacher) class, inheriting from Osoba
class Razredni : public Osoba {
    vector<Ucenik*> ucenici; // Vector to store pointers to Ucenik objects

public:
    using Osoba::Osoba; // Inherit the constructors from Osoba

    // Overloaded += operator to add a Ucenik to the vector
    Razredni& operator += (Ucenik* ucenik) {
        ucenik->postaviRazrednog(this); // Set this Razredni as the class teacher for the Ucenik
        ucenici.push_back(ucenik); // Add the Ucenik to the vector
        return *this; // Return the current object to allow chaining
    }

    // Override the virtual function to print Razredni and its Ucenici
    virtual void ispisiPodatke(ostream& out) const override {
        Osoba::ispisiPodatke(out); // Print Razredni data
        out << endl; // Print a newline

        // Iterate through the vector and print each Ucenik
        for (auto itr = ucenici.begin(); itr != ucenici.end(); itr++) {
            Ucenik* ucenik = *itr;
            out << *ucenik;
        }
    }

    // Function to count the number of valid JMBG in Ucenici
    int brojValidnih() const {
        return count_if(ucenici.begin(), ucenici.end(), [](Ucenik* u) {
            return u->getJmbg() != "0000000000000";
            });
    }

    // Function to partition Ucenici based on valid JMBG
    void particionisanje() {
        partition(ucenici.begin(), ucenici.end(), [](Ucenik* u) {
            return u->getJmbg() != "0000000000000";
            });
    }

    // Destructor to delete dynamically allocated Ucenik objects
    ~Razredni() {
        for (Ucenik* ucenik : ucenici) {
            delete ucenik; // Free the memory allocated for each Ucenik
        }
    }
};

// Definition of the Ucenik (Student) class, inheriting from Osoba
class Ucenik : public Osoba {
    Razredni* razredni = nullptr; // Pointer to the class teacher (Razredni)

public:
    using Osoba::Osoba; // Inherit the constructors from Osoba

    // Function to set the class teacher for this Ucenik
    void postaviRazrednog(Razredni* razredni) {
        this->razredni = razredni;
    }

    // Override the virtual function to print Ucenik data
    virtual void ispisiPodatke(ostream& out) const override {
        Osoba::ispisiPodatke(out); // Call the base class function to print data
    }
};

int main() {
    Razredni razredni("1234567890123", "Razredni", "Razrednic"); // Create a Razredni object

    ifstream in("input.txt"); // Open the input file
    ofstream out("output.txt"); // Open the output file

    // Check if the input file is open
    if (!in.is_open()) {
        cerr << "Failed to open input file" << endl; // Print an error message
        return EXIT_FAILURE; // Exit the program with a failure status
    }

    // Read data from the input file
    while (!in.eof()) {
        string jmbg, ime, prezime;
        in >> jmbg >> ime >> prezime; // Read jmbg, ime, and prezime from the file

        // Check if default values should be used
        if (ime == "$DEF$" || prezime == "$DEF$") {
            razredni += new Ucenik(jmbg); // Add a new Ucenik with default values
        }
        else {
            razredni += new Ucenik(jmbg, ime, prezime); // Add a new Ucenik with provided values
        }
    }

    razredni.particionisanje(); // Partition the Ucenici based on valid JMBG
    Osoba* osoba = &razredni; // Create a pointer to the Razredni as an Osoba
    out << *osoba << "Broj ucenika sa validnim jmbg je " << razredni.brojValidnih() << endl; // Print the data

    return 0; // Exit the program successfully
}
