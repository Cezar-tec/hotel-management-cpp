#include "Persoana.h"
#include "exceptions.h"
#include "utils.h"
#include <algorithm>

int Persoana::numarTotalPersone = 0;

Persoana::Persoana() : nume(""), prenume(""), telefon(""), varsta(0) {
    ++numarTotalPersone;
}

Persoana::Persoana(const std::string& nume, const std::string& prenume, 
                   const std::string& telefon, int varsta) 
    : nume(nume), prenume(prenume), telefon(telefon), varsta(varsta) {
    
    if (!utils::isValidString(nume) || !utils::isValidString(prenume)) {
        throw DateInvalideException("Numele si prenumele nu pot fi vide");
    }
    
    if (varsta < 0 || varsta > 150) {
        throw DateInvalideException("Varsta trebuie sa fie intre 0 si 150 de ani");
    }
    
    ++numarTotalPersone;
}

Persoana::Persoana(const Persoana& other) 
    : nume(other.nume), prenume(other.prenume), 
      telefon(other.telefon), varsta(other.varsta) {
    ++numarTotalPersone;
}

Persoana& Persoana::operator=(const Persoana& other) {
    if (this != &other) {
        nume = other.nume;
        prenume = other.prenume;
        telefon = other.telefon;
        varsta = other.varsta;
    }
    return *this;
}

Persoana::~Persoana() {
    --numarTotalPersone;
}

void Persoana::setNume(const std::string& nume) {
    if (!utils::isValidString(nume)) {
        throw DateInvalideException("Numele nu poate fi vid");
    }
    this->nume = utils::trim(nume);
}

void Persoana::setPrenume(const std::string& prenume) {
    if (!utils::isValidString(prenume)) {
        throw DateInvalideException("Prenumele nu poate fi vid");
    }
    this->prenume = utils::trim(prenume);
}

void Persoana::setTelefon(const std::string& telefon) {
    this->telefon = utils::trim(telefon);
}

void Persoana::setVarsta(int varsta) {
    if (varsta < 0 || varsta > 150) {
        throw DateInvalideException("Varsta trebuie sa fie intre 0 si 150 de ani");
    }
    this->varsta = varsta;
}

bool Persoana::operator==(const Persoana& other) const {
    return nume == other.nume && prenume == other.prenume && 
           telefon == other.telefon && varsta == other.varsta;
}

std::ostream& operator<<(std::ostream& os, const Persoana& p) {
    os << "=== " << p.getTipPersoana() << " ===" << std::endl;
    os << "Nume: " << p.nume << " " << p.prenume << std::endl;
    os << "Telefon: " << p.telefon << std::endl;
    os << "Varsta: " << p.varsta << " ani" << std::endl;
    
    p.afisareSpecifica(os);
    
    return os;
}

std::istream& operator>>(std::istream& is, Persoana& p) {
    std::cout << "Introduceti numele: ";
    std::getline(is, p.nume);
    p.nume = utils::trim(p.nume);
    
    std::cout << "Introduceti prenumele: ";
    std::getline(is, p.prenume);
    p.prenume = utils::trim(p.prenume);
    
    std::cout << "Introduceti telefonul: ";
    std::getline(is, p.telefon);
    p.telefon = utils::trim(p.telefon);
    
    std::cout << "Introduceti varsta: ";
    is >> p.varsta;
    is.ignore();
    
    if (!utils::isValidString(p.nume) || !utils::isValidString(p.prenume)) {
        throw DateInvalideException("Numele si prenumele nu pot fi vide");
    }
    
    if (p.varsta < 0 || p.varsta > 150) {
        throw DateInvalideException("Varsta trebuie sa fie intre 0 si 150 de ani");
    }
    
    p.citireSpecifica(is);
    
    return is;
}

bool operator<(const Persoana& a, const Persoana& b) {
    if (a.nume != b.nume) {
        return a.nume < b.nume;
    }
    return a.prenume < b.prenume;
}