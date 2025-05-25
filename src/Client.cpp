#include "Client.h"
#include "exceptions.h"
#include "utils.h"
#include <algorithm>
#include <sstream>

int Client::numarClientiVIP = 0;

Client::Client() : Persoana(), email(""), adresa(""), esteVIP(false), discountVIP(0.0) {
}

Client::Client(const std::string& nume, const std::string& prenume, 
               const std::string& telefon, int varsta, const std::string& email, 
               const std::string& adresa, bool esteVIP)
    : Persoana(nume, prenume, telefon, varsta), email(email), adresa(adresa), 
      esteVIP(esteVIP), discountVIP(esteVIP ? calculeazaDiscountDefault() : 0.0) {
    
    if (!utils::isValidString(email)) {
        throw DateInvalideException("Email-ul nu poate fi vid");
    }
    
    if (esteVIP) {
        ++numarClientiVIP;
    }
}

Client::Client(const Client& other) 
    : Persoana(other), email(other.email), adresa(other.adresa), 
      rezervari(other.rezervari), esteVIP(other.esteVIP), discountVIP(other.discountVIP) {
    
    if (esteVIP) {
        ++numarClientiVIP;
    }
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        bool eraVIP = this->esteVIP;
        
        Persoana::operator=(other);
        email = other.email;
        adresa = other.adresa;
        rezervari = other.rezervari;
        esteVIP = other.esteVIP;
        discountVIP = other.discountVIP;
        
        if (eraVIP && !esteVIP) {
            --numarClientiVIP;
        } else if (!eraVIP && esteVIP) {
            ++numarClientiVIP;
        }
    }
    return *this;
}

Client::~Client() {
    if (esteVIP) {
        --numarClientiVIP;
    }
}

void Client::setEmail(const std::string& email) {
    if (!utils::isValidString(email)) {
        throw DateInvalideException("Email-ul nu poate fi vid");
    }
    this->email = utils::trim(email);
}

void Client::setAdresa(const std::string& adresa) {
    this->adresa = utils::trim(adresa);
}

void Client::setEsteVIP(bool vip) {
    actualizeazaNumarVIP(vip);
    this->esteVIP = vip;
    this->discountVIP = vip ? calculeazaDiscountDefault() : 0.0;
}

void Client::adaugaRezervare(int idCamera) {
    auto it = std::find(rezervari.begin(), rezervari.end(), idCamera);
    if (it == rezervari.end()) {
        rezervari.push_back(idCamera);
    } else {
        throw OperatieInvalidaException("Camera este deja rezervata de acest client");
    }
}

bool Client::eliminaRezervare(int idCamera) {
    auto it = std::find(rezervari.begin(), rezervari.end(), idCamera);
    if (it != rezervari.end()) {
        rezervari.erase(it);
        return true;
    }
    return false;
}

void Client::afiseazaDetalii() const {
    std::cout << "=== DETALII CLIENT ===" << std::endl;
    std::cout << "Nume: " << nume << " " << prenume << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Adresa: " << adresa << std::endl;
    std::cout << "Telefon: " << telefon << std::endl;
    std::cout << "Varsta: " << varsta << " ani" << std::endl;
    std::cout << "Status VIP: " << (esteVIP ? "Da" : "Nu") << std::endl;
    if (esteVIP) {
        std::cout << "Discount VIP: " << (discountVIP * 100) << "%" << std::endl;
    }
    std::cout << "Numar rezervari: " << rezervari.size() << std::endl;

    if (!rezervari.empty()) {
        std::cout << "Camere rezervate: ";
        for (size_t i = 0; i < rezervari.size(); ++i) {
            std::cout << rezervari[i];
            if (i < rezervari.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

std::unique_ptr<Persoana> Client::clone() const {
    return std::make_unique<Client>(*this);
}

Client& Client::operator+=(double suma) {
    if (suma > 0 && suma >= 1000.0) {
        if (!esteVIP) {
            setEsteVIP(true);
        }
    }
    return *this;
}

bool Client::operator>(const Client& other) const {
    return rezervari.size() > other.rezervari.size();
}

void Client::citireSpecifica(std::istream& is) {
    std::cout << "Introduceti email-ul: ";
    std::getline(is, email);
    email = utils::trim(email);

    std::cout << "Introduceti adresa: ";
    std::getline(is, adresa);
    adresa = utils::trim(adresa);

    std::cout << "Este client VIP? (1-Da, 0-Nu): ";
    int vipInput;
    is >> vipInput;
    is.ignore();

    setEsteVIP(vipInput == 1);

    if (!utils::isValidString(email)) {
        throw DateInvalideException("Email-ul nu poate fi vid");
    }
}

void Client::afisareSpecifica(std::ostream& os) const {
    os << "Email: " << email << std::endl;
    os << "Adresa: " << adresa << std::endl;
    os << "Status VIP: " << (esteVIP ? "Da" : "Nu") << std::endl;
    if (esteVIP) {
        os << "Discount VIP: " << (discountVIP * 100) << "%" << std::endl;
    }
    os << "Numar rezervari: " << rezervari.size() << std::endl;
}

void Client::actualizeazaNumarVIP(bool nouValoare) {
    if (esteVIP && !nouValoare) {
        --numarClientiVIP;
    } else if (!esteVIP && nouValoare) {
        ++numarClientiVIP;
    }
}

double operator*(double pret, const Client& client) {
    if (client.getEsteVIP()) {
        return pret * (1.0 - client.getDiscountVIP());
    }
    return pret;
}