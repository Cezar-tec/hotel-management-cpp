#include "Receptioner.h"
#include "exceptions.h"
#include "utils.h"
#include <algorithm>
#include <sstream>

int Receptioner::numarTotalReceptioneri = 0;

Receptioner::Receptioner() 
    : Persoana(), idAngajat(""), salariu(0.0), program("ziua"), experienta(0) {
    actualizeazaNumarReceptioneri(true);
}

Receptioner::Receptioner(const std::string& nume, const std::string& prenume,
                         const std::string& telefon, int varsta, const std::string& idAngajat,
                         double salariu, const std::string& program, int experienta)
    : Persoana(nume, prenume, telefon, varsta), idAngajat(idAngajat), 
      salariu(salariu), program(program), experienta(experienta) {
    
    if (!utils::isValidString(idAngajat)) {
        throw DateInvalideException("ID-ul angajatului nu poate fi vid");
    }
    
    if (salariu < calculeazaSalariuMinim()) {
        throw DateInvalideException("Salariul nu poate fi sub minimul legal");
    }
    
    if (!validareProgramLucru(program)) {
        throw DateInvalideException("Programul de lucru este invalid");
    }
    
    if (experienta < 0) {
        throw DateInvalideException("Experienta nu poate fi negativa");
    }
    
    actualizeazaNumarReceptioneri(true);
}

Receptioner::Receptioner(const Receptioner& other)
    : Persoana(other), idAngajat(other.idAngajat), salariu(other.salariu),
      program(other.program), experienta(other.experienta), limbiVorbite(other.limbiVorbite) {
    actualizeazaNumarReceptioneri(true);
}

Receptioner& Receptioner::operator=(const Receptioner& other) {
    if (this != &other) {
        Persoana::operator=(other);
        idAngajat = other.idAngajat;
        salariu = other.salariu;
        program = other.program;
        experienta = other.experienta;
        limbiVorbite = other.limbiVorbite;
    }
    return *this;
}

Receptioner::~Receptioner() {
    actualizeazaNumarReceptioneri(false);
}

void Receptioner::setIdAngajat(const std::string& id) {
    if (!utils::isValidString(id)) {
        throw DateInvalideException("ID-ul angajatului nu poate fi vid");
    }
    this->idAngajat = utils::trim(id);
}

void Receptioner::setSalariu(double salariu) {
    if (salariu < calculeazaSalariuMinim()) {
        throw DateInvalideException("Salariul nu poate fi sub minimul legal");
    }
    this->salariu = salariu;
}

void Receptioner::setProgram(const std::string& program) {
    if (!validareProgramLucru(program)) {
        throw DateInvalideException("Programul de lucru este invalid");
    }
    this->program = program;
}

void Receptioner::setExperienta(int experienta) {
    if (experienta < 0) {
        throw DateInvalideException("Experienta nu poate fi negativa");
    }
    this->experienta = experienta;
}

void Receptioner::adaugaLimba(const std::string& limba) {
    std::string limbaTrimmed = utils::trim(limba);
    if (!utils::isValidString(limbaTrimmed)) {
        throw DateInvalideException("Limba nu poate fi vida");
    }
    
    if (!vorbeLimba(limbaTrimmed)) {
        limbiVorbite.push_back(limbaTrimmed);
    }
}

bool Receptioner::vorbeLimba(const std::string& limba) const {
    std::string limbaTrimmed = utils::trim(limba);
    return std::find(limbiVorbite.begin(), limbiVorbite.end(), limbaTrimmed) != limbiVorbite.end();
}

bool Receptioner::validareProgramLucru(const std::string& program) {
    std::string programTrimmed = utils::trim(program);
    return programTrimmed == "ziua" || programTrimmed == "noaptea" || programTrimmed == "mixt";
}

bool Receptioner::poatePreluaRezervari() const {
    return experienta >= 1;
}

double Receptioner::calculeazaBonusExperienta() const {
    return salariu * (experienta * 0.05);
}

void Receptioner::afiseazaDetalii() const {
    std::cout << "=== DETALII RECEPTIONER ===" << std::endl;
    std::cout << "Nume: " << nume << " " << prenume << std::endl;
    std::cout << "ID Angajat: " << idAngajat << std::endl;
    std::cout << "Telefon: " << telefon << std::endl;
    std::cout << "Varsta: " << varsta << " ani" << std::endl;
    std::cout << "Salariu: " << utils::formatPrice(salariu) << std::endl;
    std::cout << "Program: " << program << std::endl;
    std::cout << "Experienta: " << experienta << " ani" << std::endl;
    std::cout << "Bonus experienta: " << utils::formatPrice(calculeazaBonusExperienta()) << std::endl;
    std::cout << "Poate prelua rezervari: " << (poatePreluaRezervari() ? "Da" : "Nu") << std::endl;
    
    std::cout << "Limbi vorbite (" << limbiVorbite.size() << "): ";
    for (size_t i = 0; i < limbiVorbite.size(); ++i) {
        std::cout << limbiVorbite[i];
        if (i < limbiVorbite.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}

std::unique_ptr<Persoana> Receptioner::clone() const {
    return std::make_unique<Receptioner>(*this);
}

Receptioner& Receptioner::operator++() {
    salariu *= 1.05;
    return *this;
}

Receptioner Receptioner::operator++(int) {
    Receptioner temp(*this);
    ++(*this);
    return temp;
}

bool Receptioner::operator<(const Receptioner& other) const {
    return experienta < other.experienta;
}

void Receptioner::citireSpecifica(std::istream& is) {
    std::cout << "Introduceti ID-ul angajatului: ";
    std::getline(is, idAngajat);
    idAngajat = utils::trim(idAngajat);
    
    std::cout << "Introduceti salariul: ";
    is >> salariu;
    is.ignore();
    
    std::cout << "Introduceti programul (ziua/noaptea/mixt): ";
    std::getline(is, program);
    program = utils::trim(program);
    
    std::cout << "Introduceti experienta (ani): ";
    is >> experienta;
    is.ignore();
    
    if (!utils::isValidString(idAngajat)) {
        throw DateInvalideException("ID-ul angajatului nu poate fi vid");
    }
    
    if (salariu < calculeazaSalariuMinim()) {
        throw DateInvalideException("Salariul nu poate fi sub minimul legal");
    }
    
    if (!validareProgramLucru(program)) {
        throw DateInvalideException("Programul de lucru este invalid");
    }
    
    if (experienta < 0) {
        throw DateInvalideException("Experienta nu poate fi negativa");
    }
    
    std::cout << "Introduceti numarul de limbi vorbite: ";
    int numarLimbi;
    is >> numarLimbi;
    is.ignore();
    
    for (int i = 0; i < numarLimbi; ++i) {
        std::cout << "Limba " << (i + 1) << ": ";
        std::string limba;
        std::getline(is, limba);
        adaugaLimba(limba);
    }
}

void Receptioner::afisareSpecifica(std::ostream& os) const {
    os << "ID Angajat: " << idAngajat << std::endl;
    os << "Salariu: " << utils::formatPrice(salariu) << std::endl;
    os << "Program: " << program << std::endl;
    os << "Experienta: " << experienta << " ani" << std::endl;
    os << "Bonus experienta: " << utils::formatPrice(calculeazaBonusExperienta()) << std::endl;
    
    os << "Limbi vorbite: ";
    for (size_t i = 0; i < limbiVorbite.size(); ++i) {
        os << limbiVorbite[i];
        if (i < limbiVorbite.size() - 1) os << ", ";
    }
    os << std::endl;
}

void Receptioner::actualizeazaNumarReceptioneri(bool adauga) {
    if (adauga) {
        ++numarTotalReceptioneri;
    } else {
        --numarTotalReceptioneri;
    }
}

bool operator>(const Receptioner& a, const Receptioner& b) {
    return a.getSalariu() > b.getSalariu();
}