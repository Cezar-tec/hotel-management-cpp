#include "Camera.h"
#include "exceptions.h"
#include "utils.h"
#include <algorithm>
#include <sstream>

int Camera::numarTotalCamere = 0;
double Camera::pretBaza = 150.0;

Camera::Camera() : numar(0), tip(TipCamera::STANDARD), status(StatusCamera::LIBERA),
                   pretPeNoapte(pretBaza), capacitateMaxima(1), areBalcon(false), etaj(1) {
    ++numarTotalCamere;
    initializareFacilitatiDefault();
}

Camera::Camera(int numar, TipCamera tip, double pretPeNoapte, 
               int capacitateMaxima, bool areBalcon, int etaj)
    : numar(numar), tip(tip), status(StatusCamera::LIBERA), pretPeNoapte(pretPeNoapte),
      capacitateMaxima(capacitateMaxima), areBalcon(areBalcon), etaj(etaj) {
    
    if (numar <= 0) {
        throw DateInvalideException("Numarul camerei trebuie sa fie pozitiv");
    }
    if (pretPeNoapte <= 0) {
        throw DateInvalideException("Pretul pe noapte trebuie sa fie pozitiv");
    }
    if (capacitateMaxima <= 0 || capacitateMaxima > 10) {
        throw DateInvalideException("Capacitatea maxima trebuie sa fie intre 1 si 10");
    }
    if (etaj <= 0 || etaj > 50) {
        throw DateInvalideException("Etajul trebuie sa fie intre 1 si 50");
    }
    
    ++numarTotalCamere;
    initializareFacilitatiDefault();
}

Camera::Camera(int numar, TipCamera tip, double pretPeNoapte, 
               int capacitateMaxima, bool areBalcon, int etaj,
               const std::vector<std::string>& facilitati)
    : Camera(numar, tip, pretPeNoapte, capacitateMaxima, areBalcon, etaj) {
    this->facilitati = facilitati;
}

Camera::Camera(const Camera& other) 
    : numar(other.numar), tip(other.tip), status(other.status),
      pretPeNoapte(other.pretPeNoapte), capacitateMaxima(other.capacitateMaxima),
      facilitati(other.facilitati), areBalcon(other.areBalcon), etaj(other.etaj) {
    ++numarTotalCamere;
}

Camera& Camera::operator=(const Camera& other) {
    if (this != &other) {
        numar = other.numar;
        tip = other.tip;
        status = other.status;
        pretPeNoapte = other.pretPeNoapte;
        capacitateMaxima = other.capacitateMaxima;
        facilitati = other.facilitati;
        areBalcon = other.areBalcon;
        etaj = other.etaj;
    }
    return *this;
}

Camera::~Camera() {
    --numarTotalCamere;
}

void Camera::setStatus(StatusCamera status) {
    this->status = status;
}

void Camera::setPretPeNoapte(double pret) {
    if (pret <= 0) {
        throw DateInvalideException("Pretul pe noapte trebuie sa fie pozitiv");
    }
    this->pretPeNoapte = pret;
}

void Camera::setAreBalcon(bool balcon) {
    this->areBalcon = balcon;
}

void Camera::adaugaFacilitate(const std::string& facilitate) {
    std::string facilitateTrimmed = utils::trim(facilitate);
    if (!utils::isValidString(facilitateTrimmed)) {
        throw DateInvalideException("Facilitatea nu poate fi vida");
    }
    if (!areFacilitate(facilitateTrimmed)) {
        facilitati.push_back(facilitateTrimmed);
    }
}

bool Camera::areFacilitate(const std::string& facilitate) const {
    std::string facilitateTrimmed = utils::trim(facilitate);
    return std::find(facilitati.begin(), facilitati.end(), facilitateTrimmed) != facilitati.end();
}

void Camera::eliminaFacilitate(const std::string& facilitate) {
    std::string facilitateTrimmed = utils::trim(facilitate);
    auto it = std::find(facilitati.begin(), facilitati.end(), facilitateTrimmed);
    if (it != facilitati.end()) {
        facilitati.erase(it);
    }
}

std::string Camera::tipCameraToString(TipCamera tip) {
    switch (tip) {
        case TipCamera::STANDARD: return "Standard";
        case TipCamera::DELUXE: return "Deluxe";
        case TipCamera::SUITE: return "Suite";
        case TipCamera::APARTAMENT: return "Apartament";
        default: return "Unknown";
    }
}

std::string Camera::statusCameraToString(StatusCamera status) {
    switch (status) {
        case StatusCamera::LIBERA: return "Libera";
        case StatusCamera::OCUPATA: return "Ocupata";
        case StatusCamera::MENTENANTA: return "Mentenanta";
        case StatusCamera::REZERVATA: return "Rezervata";
        default: return "Unknown";
    }
}

bool Camera::esteDisponibila() const {
    return status == StatusCamera::LIBERA;
}

double Camera::calculeazaPret(int numarNopti) const {
    if (numarNopti <= 0) {
        throw DateInvalideException("Numarul de nopti trebuie sa fie pozitiv");
    }
    
    double pretTotal = pretPeNoapte * numarNopti * calculeazaFactorTip();
    
    if (numarNopti >= 7) {
        pretTotal *= 0.9;
    } else if (numarNopti >= 3) {
        pretTotal *= 0.95;
    }
    
    return pretTotal;
}

double Camera::calculeazaPretCuFacilitati() const {
    double pretBaza = pretPeNoapte * calculeazaFactorTip();
    double adaosFacilitati = facilitati.size() * 25.0;
    
    if (areBalcon) {
        adaosFacilitati += 50.0;
    }
    
    return pretBaza + adaosFacilitati;
}

Camera& Camera::operator+=(double marire) {
    if (marire < 0) {
        throw DateInvalideException("Marirea pretului nu poate fi negativa");
    }
    pretPeNoapte += marire;
    return *this;
}

bool Camera::operator<(const Camera& other) const {
    return pretPeNoapte < other.pretPeNoapte;
}

bool Camera::operator==(const Camera& other) const {
    return numar == other.numar;
}

void Camera::initializareFacilitatiDefault() {
    facilitati.clear();
    
    facilitati.push_back("Wi-Fi gratuit");
    facilitati.push_back("TV LED");
    facilitati.push_back("Aer conditionat");
    
    switch (tip) {
        case TipCamera::STANDARD:
            facilitati.push_back("Minibar");
            break;
        case TipCamera::DELUXE:
            facilitati.push_back("Minibar");
            facilitati.push_back("Seif");
            facilitati.push_back("Halat si papuci");
            break;
        case TipCamera::SUITE:
            facilitati.push_back("Minibar");
            facilitati.push_back("Seif");
            facilitati.push_back("Halat si papuci");
            facilitati.push_back("Living separat");
            facilitati.push_back("Jacuzzi");
            break;
        case TipCamera::APARTAMENT:
            facilitati.push_back("Bucatarie complet echipata");
            facilitati.push_back("Masina de spalat");
            facilitati.push_back("Living separat");
            facilitati.push_back("2 dormitoare");
            break;
    }
}

double Camera::calculeazaFactorTip() const {
    switch (tip) {
        case TipCamera::STANDARD: return 1.0;
        case TipCamera::DELUXE: return 1.5;
        case TipCamera::SUITE: return 2.0;
        case TipCamera::APARTAMENT: return 2.5;
        default: return 1.0;
    }
}

std::ostream& operator<<(std::ostream& os, const Camera& camera) {
    os << "=== CAMERA " << camera.numar << " ===" << std::endl;
    os << "Tip: " << Camera::tipCameraToString(camera.tip) << std::endl;
    os << "Status: " << Camera::statusCameraToString(camera.status) << std::endl;
    os << "Pret pe noapte: " << utils::formatPrice(camera.pretPeNoapte) << std::endl;
    os << "Capacitate maxima: " << camera.capacitateMaxima << " persoane" << std::endl;
    os << "Etaj: " << camera.etaj << std::endl;
    os << "Balcon: " << (camera.areBalcon ? "Da" : "Nu") << std::endl;
    os << "Pret cu facilitati: " << utils::formatPrice(camera.calculeazaPretCuFacilitati()) << std::endl;
    
    os << "Facilitati (" << camera.facilitati.size() << "): ";
    for (size_t i = 0; i < camera.facilitati.size(); ++i) {
        os << camera.facilitati[i];
        if (i < camera.facilitati.size() - 1) os << ", ";
    }
    os << std::endl;
    
    return os;
}

std::istream& operator>>(std::istream& is, Camera& camera) {
    std::cout << "Introduceti numarul camerei: ";
    is >> camera.numar;
    
    std::cout << "Introduceti tipul camerei (0-Standard, 1-Deluxe, 2-Suite, 3-Apartament): ";
    int tipInt;
    is >> tipInt;
    camera.tip = static_cast<TipCamera>(tipInt);
    
    std::cout << "Introduceti pretul pe noapte: ";
    is >> camera.pretPeNoapte;
    
    std::cout << "Introduceti capacitatea maxima: ";
    is >> camera.capacitateMaxima;
    
    std::cout << "Are balcon? (1-Da, 0-Nu): ";
    int balconInt;
    is >> balconInt;
    camera.areBalcon = (balconInt == 1);
    
    std::cout << "Introduceti etajul: ";
    is >> camera.etaj;
    
    is.ignore();
    
    if (camera.numar <= 0) {
        throw DateInvalideException("Numarul camerei trebuie sa fie pozitiv");
    }
    if (camera.pretPeNoapte <= 0) {
        throw DateInvalideException("Pretul pe noapte trebuie sa fie pozitiv");
    }
    if (camera.capacitateMaxima <= 0 || camera.capacitateMaxima > 10) {
        throw DateInvalideException("Capacitatea maxima trebuie sa fie intre 1 si 10");
    }
    if (camera.etaj <= 0 || camera.etaj > 50) {
        throw DateInvalideException("Etajul trebuie sa fie intre 1 si 50");
    }
    
    std::cout << "Introduceti numarul de facilitati: ";
    int numarFacilitati;
    is >> numarFacilitati;
    is.ignore();
    
    camera.facilitati.clear();
    for (int i = 0; i < numarFacilitati; ++i) {
        std::cout << "Facilitatea " << (i + 1) << ": ";
        std::string facilitate;
        std::getline(is, facilitate);
        camera.adaugaFacilitate(facilitate);
    }
    
    return is;
}

bool operator>(const Camera& a, const Camera& b) {
    return a.getPretPeNoapte() > b.getPretPeNoapte();
}

Camera operator+(const Camera& camera, double marire) {
    Camera result = camera;
    result += marire;
    return result;
}