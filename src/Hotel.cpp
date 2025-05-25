#include "Hotel.h"
#include <iostream>
#include <numeric>

template<typename T>
std::unique_ptr<Hotel<T>> Hotel<T>::instance = nullptr;

template<typename T>
int Hotel<T>::numarTotalHoteluri = 0;

template<typename T>
Hotel<T>::Hotel(const std::string& nume, const std::string& adresa, int numarStele)
    : nume(nume), adresa(adresa), numarStele(numarStele) {

    if (!utils::isValidString(nume)) {
        throw DateInvalideException("Numele hotelului nu poate fi vid");
    }

    if (numarStele < 1 || numarStele > 5) {
        throw DateInvalideException("Numarul de stele trebuie sa fie intre 1 si 5");
    }

    ++numarTotalHoteluri;
}

template<typename T>
Hotel<T>& Hotel<T>::getInstance(const std::string& nume, const std::string& adresa, int numarStele) {
    if (!instance) {
        instance = std::unique_ptr<Hotel<T>>(new Hotel<T>(nume, adresa, numarStele));
    }
    return *instance;
}

template<typename T>
Hotel<T>::~Hotel() {
    --numarTotalHoteluri;
}

template<typename T>
void Hotel<T>::setNume(const std::string& nume) {
    if (!utils::isValidString(nume)) {
        throw DateInvalideException("Numele hotelului nu poate fi vid");
    }
    this->nume = utils::trim(nume);
}

template<typename T>
void Hotel<T>::setAdresa(const std::string& adresa) {
    this->adresa = utils::trim(adresa);
}

template<typename T>
void Hotel<T>::setNumarStele(int stele) {
    if (stele < 1 || stele > 5) {
        throw DateInvalideException("Numarul de stele trebuie sa fie intre 1 si 5");
    }
    this->numarStele = stele;
}

template<typename T>
bool Hotel<T>::proceseazaRezervare(int numarCamera, const std::string& numeClient) {
    try {
        if (!verificaDisponibilitate(numarCamera)) {
            return false;
        }

        if (!validareClient(numeClient)) {
            return false;
        }

        Client* client = gasesteClient(numeClient, "");
        if (client) {
            int clientId = std::hash<std::string>{}(client->getNume() + client->getPrenume());
            confirmaRezervare(numarCamera, clientId);
            return true;
        }

        return false;
    } catch (const HotelException& e) {
        std::cerr << "Eroare la procesarea rezervarii: " << e.what() << std::endl;
        return false;
    }
}

template<typename T>
bool Hotel<T>::verificaDisponibilitate(int numarCamera) const {
    Camera* camera = gasesteCamera(numarCamera);
    return camera && camera->esteDisponibila();
}

template<typename T>
bool Hotel<T>::validareClient(const std::string& numeClient) const {
    return utils::isValidString(numeClient);
}

template<typename T>
void Hotel<T>::confirmaRezervare(int numarCamera, int idClient) {
    Camera* camera = gasesteCamera(numarCamera);
    if (camera) {
        camera->setStatus(StatusCamera::REZERVATA);
        rezervari[numarCamera] = idClient;
    }
}

template<typename T>
void Hotel<T>::adaugaCamera(std::unique_ptr<Camera> camera) {
    if (!camera) {
        throw DateInvalideException("Camera nu poate fi null");
    }

    if (gasesteCamera(camera->getNumar())) {
        throw OperatieInvalidaException("Camera cu acest numar exista deja");
    }

    camere.push_back(std::move(camera));
}

template<typename T>
void Hotel<T>::eliminaCamera(int numarCamera) {
    auto it = std::find_if(camere.begin(), camere.end(),
        [numarCamera](const std::unique_ptr<Camera>& camera) {
            return camera->getNumar() == numarCamera;
        });

    if (it != camere.end()) {
        if ((*it)->getStatus() == StatusCamera::OCUPATA ||
            (*it)->getStatus() == StatusCamera::REZERVATA) {
            throw OperatieInvalidaException("Nu se poate elimina o camera ocupata sau rezervata");
        }

        rezervari.erase(numarCamera);
        camere.erase(it);
    } else {
        throw CameraIndisponibilaException(numarCamera);
    }
}

template<typename T>
Camera* Hotel<T>::gasesteCamera(int numarCamera) const {
    auto it = std::find_if(camere.begin(), camere.end(),
        [numarCamera](const std::unique_ptr<Camera>& camera) {
            return camera->getNumar() == numarCamera;
        });

    return (it != camere.end()) ? it->get() : nullptr;
}

template<typename T>
std::vector<Camera*> Hotel<T>::getCamereDisponibile() const {
    std::vector<Camera*> result;

    for (const auto& camera : camere) {
        if (camera->esteDisponibila()) {
            result.push_back(camera.get());
        }
    }

    return result;
}

template<typename T>
std::vector<Camera*> Hotel<T>::getCamereByTip(TipCamera tip) const {
    std::vector<Camera*> result;

    for (const auto& camera : camere) {
        if (camera->getTip() == tip) {
            result.push_back(camera.get());
        }
    }

    return result;
}

template<typename T>
void Hotel<T>::adaugaClient(std::unique_ptr<Client> client) {
    if (!client) {
        throw DateInvalideException("Clientul nu poate fi null");
    }

    if (gasesteClient(client->getNume(), client->getPrenume())) {
        throw OperatieInvalidaException("Client cu acest nume si prenume exista deja");
    }

    clienti.push_back(std::move(client));
}

template<typename T>
void Hotel<T>::eliminaClient(const std::string& nume, const std::string& prenume) {
    auto it = std::find_if(clienti.begin(), clienti.end(),
        [&nume, &prenume](const std::unique_ptr<Client>& client) {
            return client->getNume() == nume && client->getPrenume() == prenume;
        });

    if (it != clienti.end()) {
        if ((*it)->areRezervari()) {
            throw OperatieInvalidaException("Nu se poate elimina un client cu rezervari active");
        }

        clienti.erase(it);
    } else {
        throw ClientInexistentException(nume + " " + prenume);
    }
}

template<typename T>
Client* Hotel<T>::gasesteClient(const std::string& nume, const std::string& prenume) const {
    auto it = std::find_if(clienti.begin(), clienti.end(),
        [&nume, &prenume](const std::unique_ptr<Client>& client) {
            return client->getNume() == nume &&
                   (prenume.empty() || client->getPrenume() == prenume);
        });

    return (it != clienti.end()) ? it->get() : nullptr;
}

template<typename T>
std::vector<Client*> Hotel<T>::getClientiVIP() const {
    std::vector<Client*> result;

    for (const auto& client : clienti) {
        if (client->getEsteVIP()) {
            result.push_back(client.get());
        }
    }

    return result;
}

template<typename T>
void Hotel<T>::adaugaReceptioner(std::unique_ptr<Receptioner> receptioner) {
    if (!receptioner) {
        throw DateInvalideException("Receptionerul nu poate fi null");
    }

    if (gasesteReceptioner(receptioner->getIdAngajat())) {
        throw OperatieInvalidaException("Receptioner cu acest ID exista deja");
    }

    receptioneri.push_back(std::move(receptioner));
}

template<typename T>
void Hotel<T>::eliminaReceptioner(const std::string& idAngajat) {
    auto it = std::find_if(receptioneri.begin(), receptioneri.end(),
        [&idAngajat](const std::unique_ptr<Receptioner>& receptioner) {
            return receptioner->getIdAngajat() == idAngajat;
        });

    if (it != receptioneri.end()) {
        receptioneri.erase(it);
    } else {
        throw ClientInexistentException("Receptioner cu ID: " + idAngajat);
    }
}

template<typename T>
Receptioner* Hotel<T>::gasesteReceptioner(const std::string& idAngajat) const {
    auto it = std::find_if(receptioneri.begin(), receptioneri.end(),
        [&idAngajat](const std::unique_ptr<Receptioner>& receptioner) {
            return receptioner->getIdAngajat() == idAngajat;
        });

    return (it != receptioneri.end()) ? it->get() : nullptr;
}

template<typename T>
std::vector<Receptioner*> Hotel<T>::getReceptioneriByProgram(const std::string& program) const {
    std::vector<Receptioner*> result;

    for (const auto& receptioner : receptioneri) {
        if (receptioner->getProgram() == program) {
            result.push_back(receptioner.get());
        }
    }

    return result;
}

template<typename T>
void Hotel<T>::adaugaServiciuSpecial(const T& serviciu) {
    auto it = std::find(serviciiSpeciale.begin(), serviciiSpeciale.end(), serviciu);
    if (it == serviciiSpeciale.end()) {
        serviciiSpeciale.push_back(serviciu);
    }
}

template<typename T>
void Hotel<T>::eliminaServiciuSpecial(const T& serviciu) {
    auto it = std::find(serviciiSpeciale.begin(), serviciiSpeciale.end(), serviciu);
    if (it != serviciiSpeciale.end()) {
        serviciiSpeciale.erase(it);
    }
}

template<typename T>
bool Hotel<T>::areServiciuSpecial(const T& serviciu) const {
    return std::find(serviciiSpeciale.begin(), serviciiSpeciale.end(), serviciu) != serviciiSpeciale.end();
}

template<typename T>
bool Hotel<T>::facereRezervare(int numarCamera, const std::string& numeClient, const std::string& prenumeClient) {
    try {
        Camera* camera = gasesteCamera(numarCamera);
        if (!camera) {
            throw CameraIndisponibilaException(numarCamera);
        }

        if (!camera->esteDisponibila()) {
            throw CameraIndisponibilaException(numarCamera);
        }

        Client* client = gasesteClient(numeClient, prenumeClient);
        if (!client) {
            throw ClientInexistentException(numeClient + " " + prenumeClient);
        }

        camera->setStatus(StatusCamera::REZERVATA);
        client->adaugaRezervare(numarCamera);
        rezervari[numarCamera] = std::hash<std::string>{}(numeClient + prenumeClient);

        return true;
    } catch (const HotelException& e) {
        std::cerr << "Eroare la rezervare: " << e.what() << std::endl;
        return false;
    }
}

template<typename T>
bool Hotel<T>::anulareRezervare(int numarCamera) {
    auto it = rezervari.find(numarCamera);
    if (it != rezervari.end()) {
        Camera* camera = gasesteCamera(numarCamera);
        if (camera) {
            camera->setStatus(StatusCamera::LIBERA);
        }

        for (const auto& client : clienti) {
            if (client->eliminaRezervare(numarCamera)) {
                break;
            }
        }

        rezervari.erase(it);
        return true;
    }
    return false;
}

template<typename T>
bool Hotel<T>::esteRezervata(int numarCamera) const {
    return rezervari.find(numarCamera) != rezervari.end();
}

template<typename T>
Client* Hotel<T>::getClientRezervare(int numarCamera) const {
    auto it = rezervari.find(numarCamera);
    if (it != rezervari.end()) {
        for (const auto& client : clienti) {
            auto& rezervariClient = client->getRezervari();
            if (std::find(rezervariClient.begin(), rezervariClient.end(), numarCamera) != rezervariClient.end()) {
                return client.get();
            }
        }
    }
    return nullptr;
}

template<typename T>
std::vector<Camera*> Hotel<T>::filtreazaCamere(std::function<bool(const Camera*)> predicat) const {
    std::vector<Camera*> result;

    for (const auto& camera : camere) {
        if (predicat(camera.get())) {
            result.push_back(camera.get());
        }
    }

    return result;
}

template<typename T>
std::vector<Client*> Hotel<T>::filtreazaClienti(std::function<bool(const Client*)> predicat) const {
    std::vector<Client*> result;

    for (const auto& client : clienti) {
        if (predicat(client.get())) {
            result.push_back(client.get());
        }
    }

    return result;
}

template<typename T>
void Hotel<T>::sorteazaCamereByPret() {
    std::sort(camere.begin(), camere.end(),
        [](const std::unique_ptr<Camera>& a, const std::unique_ptr<Camera>& b) {
            return *a < *b;
        });
}

template<typename T>
void Hotel<T>::sorteazaClientiByNume() {
    std::sort(clienti.begin(), clienti.end(),
        [](const std::unique_ptr<Client>& a, const std::unique_ptr<Client>& b) {
            return *a < *b;
        });
}

template<typename T>
void Hotel<T>::sorteazaReceptioneriByExperienta() {
    std::sort(receptioneri.begin(), receptioneri.end(),
        [](const std::unique_ptr<Receptioner>& a, const std::unique_ptr<Receptioner>& b) {
            return *a < *b;
        });
}

template<typename T>
double Hotel<T>::calculeazaVenitTotal() const {
    double venitTotal = 0.0;

    for (const auto& camera : camere) {
        if (camera->getStatus() == StatusCamera::OCUPATA ||
            camera->getStatus() == StatusCamera::REZERVATA) {
            venitTotal += camera->calculeazaPretCuFacilitati();
        }
    }

    return venitTotal;
}

template<typename T>
double Hotel<T>::calculeazaOcupare() const {
    if (camere.empty()) {
        return 0.0;
    }

    int camereOcupate = std::count_if(camere.begin(), camere.end(),
        [](const std::unique_ptr<Camera>& camera) {
            return camera->getStatus() == StatusCamera::OCUPATA ||
                   camera->getStatus() == StatusCamera::REZERVATA;
        });

    return (static_cast<double>(camereOcupate) / camere.size()) * 100.0;
}

template<typename T>
std::map<TipCamera, int> Hotel<T>::getStatisticiCamere() const {
    std::map<TipCamera, int> statistici;

    for (const auto& camera : camere) {
        statistici[camera->getTip()]++;
    }

    return statistici;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Hotel<T>& hotel) {
    os << "====== " << hotel.nume << " ======" << std::endl;
    os << "Adresa: " << hotel.adresa << std::endl;
    os << "Numar stele: " << hotel.numarStele << " stele" << std::endl;
    os << "Numar camere: " << hotel.camere.size() << std::endl;
    os << "Numar clienti: " << hotel.clienti.size() << std::endl;
    os << "Numar receptioneri: " << hotel.receptioneri.size() << std::endl;
    os << "Procent ocupare: " << hotel.calculeazaOcupare() << "%" << std::endl;
    os << "Venit total estimat: " << utils::formatPrice(hotel.calculeazaVenitTotal()) << std::endl;
    os << "Servicii speciale: " << hotel.serviciiSpeciale.size() << std::endl;

    return os;
}

template<typename T>
bool Hotel<T>::operator==(const Hotel<T>& other) const {
    return nume == other.nume && adresa == other.adresa && numarStele == other.numarStele;
}

template<typename T>
int Hotel<T>::getNextClientId() const {
    return static_cast<int>(clienti.size()) + 1;
}

template<typename T>
void Hotel<T>::actualizeazaStatusCamere() {
    for (const auto& camera : camere) {
        if (camera->getStatus() == StatusCamera::MENTENANTA) {
        }
    }
}

template<typename T>
bool Hotel<T>::validareNumarCamera(int numar) const {
    return numar > 0 && numar <= 9999;
}

template class Hotel<std::string>;
template class Hotel<int>;