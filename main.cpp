#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <functional>

#include "Hotel.h"
#include "Camera.h"
#include "Client.h"
#include "Receptioner.h"
#include "exceptions.h"
#include "utils.h"

void afiseazaMeniu();
void gestioneazaCamere(Hotel<std::string>& hotel);
void gestioneazaClienti(Hotel<std::string>& hotel);
void gestioneazaReceptioneri(Hotel<std::string>& hotel);
void gestioneazaRezervari(Hotel<std::string>& hotel);
void afiseazaRapoarte(Hotel<std::string>& hotel);
void demonstreazaFunctionalitati(Hotel<std::string>& hotel);

void demonstreazaSTL(Hotel<std::string>& hotel);
void demonstreazaExceptii(Hotel<std::string>& hotel);
void demonstreazaPolimorfism(Hotel<std::string>& hotel);

int main() {
    try {
        std::cout << "=== SISTEM DE MANAGEMENT HOTEL ===" << std::endl;
        std::cout << "Autor: Student C++" << std::endl;
        std::cout << "Proiect demonstrativ pentru conceptele POO" << std::endl << std::endl;

        Hotel<std::string>& hotel = Hotel<std::string>::getInstance(
            "Hotel Imperial Palace",
            "Strada Unirii nr. 15, București",
            5
        );

        demonstreazaFunctionalitati(hotel);

        int optiune;
        bool continua = true;

        while (continua) {
            afiseazaMeniu();
            std::cout << "Alegeti o optiune: ";

            if (!(std::cin >> optiune)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Optiune invalida! Va rugam introduceti un numar." << std::endl;
                continue;
            }

            std::cin.ignore();

            try {
                switch (optiune) {
                    case 1:
                        gestioneazaCamere(hotel);
                        break;
                    case 2:
                        gestioneazaClienti(hotel);
                        break;
                    case 3:
                        gestioneazaReceptioneri(hotel);
                        break;
                    case 4:
                        gestioneazaRezervari(hotel);
                        break;
                    case 5:
                        afiseazaRapoarte(hotel);
                        break;
                    case 6:
                        demonstreazaSTL(hotel);
                        break;
                    case 7:
                        demonstreazaExceptii(hotel);
                        break;
                    case 8:
                        demonstreazaPolimorfism(hotel);
                        break;
                    case 9:
                        std::cout << hotel << std::endl;
                        break;
                    case 0:
                        continua = false;
                        std::cout << "La revedere!" << std::endl;
                        break;
                    default:
                        std::cout << "Optiune invalida!" << std::endl;
                        break;
                }
            } catch (const HotelException& e) {
                std::cout << "EROARE: " << e.what() << std::endl;
            } catch (const std::exception& e) {
                std::cout << "EROARE GENERICA: " << e.what() << std::endl;
            }

            if (continua) {
                std::cout << "\nApasati Enter pentru a continua...";
                std::cin.get();
            }
        }

    } catch (const HotelException& e) {
        std::cerr << "EROARE CRITICA: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "EROARE NECUNOSCUTA: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

void afiseazaMeniu() {
    std::cout << "\n========== MENIU PRINCIPAL ==========\n";
    std::cout << "1. Gestionare Camere\n";
    std::cout << "2. Gestionare Clienti\n";
    std::cout << "3. Gestionare Receptioneri\n";
    std::cout << "4. Gestionare Rezervari\n";
    std::cout << "5. Rapoarte si Statistici\n";
    std::cout << "6. Demonstrare STL si Lambda\n";
    std::cout << "7. Demonstrare Exceptii\n";
    std::cout << "8. Demonstrare Polimorfism\n";
    std::cout << "9. Afisare Informatii Hotel\n";
    std::cout << "0. Iesire\n";
    std::cout << "=====================================\n";
}

void gestioneazaCamere(Hotel<std::string>& hotel) {
    std::cout << "\n=== GESTIONARE CAMERE ===" << std::endl;
    std::cout << "1. Adauga camera noua\n";
    std::cout << "2. Afiseaza toate camerele\n";
    std::cout << "3. Cauta camera dupa numar\n";
    std::cout << "4. Afiseaza camere disponibile\n";
    std::cout << "5. Sortare camere dupa pret\n";

    int optiune;
    std::cout << "Optiune: ";
    std::cin >> optiune;
    std::cin.ignore();

    switch (optiune) {
        case 1: {
            std::cout << "Introduceti datele pentru camera noua:\n";
            auto camera = std::make_unique<Camera>();
            std::cin >> *camera;
            hotel.adaugaCamera(std::move(camera));
            std::cout << "Camera a fost adaugata cu succes!" << std::endl;
            break;
        }
        case 2: {
            std::cout << "\n=== TOATE CAMERELE ===" << std::endl;
            for (size_t i = 0; i < hotel.getNumarCamere(); ++i) {
                std::cout << "Camera " << (i + 1) << " din " << hotel.getNumarCamere() << std::endl;
            }
            break;
        }
        case 3: {
            int numar;
            std::cout << "Introduceti numarul camerei: ";
            std::cin >> numar;
            Camera* camera = hotel.gasesteCamera(numar);
            if (camera) {
                std::cout << *camera << std::endl;
            } else {
                std::cout << "Camera nu a fost gasita!" << std::endl;
            }
            break;
        }
        case 4: {
            auto camereDisponibile = hotel.getCamereDisponibile();
            std::cout << "\n=== CAMERE DISPONIBILE (" << camereDisponibile.size() << ") ===" << std::endl;
            for (const auto& camera : camereDisponibile) {
                std::cout << *camera << std::endl << "---" << std::endl;
            }
            break;
        }
        case 5: {
            hotel.sorteazaCamereByPret();
            std::cout << "Camerele au fost sortate dupa pret!" << std::endl;
            break;
        }
        default:
            std::cout << "Optiune invalida!" << std::endl;
    }
}

void gestioneazaClienti(Hotel<std::string>& hotel) {
    std::cout << "\n=== GESTIONARE CLIENTI ===" << std::endl;
    std::cout << "1. Adauga client nou\n";
    std::cout << "2. Afiseaza toti clientii\n";
    std::cout << "3. Cauta client\n";
    std::cout << "4. Afiseaza clienti VIP\n";
    std::cout << "5. Promoveaza client la VIP\n";

    int optiune;
    std::cout << "Optiune: ";
    std::cin >> optiune;
    std::cin.ignore();

    switch (optiune) {
        case 1: {
            std::cout << "Introduceti datele pentru clientul nou:\n";
            auto client = std::make_unique<Client>();
            std::cin >> *client;
            hotel.adaugaClient(std::move(client));
            std::cout << "Clientul a fost adaugat cu succes!" << std::endl;
            break;
        }
        case 2: {
            std::cout << "\n=== TOTI CLIENTII ===" << std::endl;
            std::cout << "Numar total clienti: " << hotel.getNumarClienti() << std::endl;
            std::cout << "Clienti VIP: " << Client::getNumarClientiVIP() << std::endl;
            break;
        }
        case 3: {
            std::string nume, prenume;
            std::cout << "Introduceti numele: ";
            std::getline(std::cin, nume);
            std::cout << "Introduceti prenumele: ";
            std::getline(std::cin, prenume);

            Client* client = hotel.gasesteClient(nume, prenume);
            if (client) {
                client->afiseazaDetalii();
            } else {
                std::cout << "Clientul nu a fost gasit!" << std::endl;
            }
            break;
        }
        case 4: {
            auto clientiVIP = hotel.getClientiVIP();
            std::cout << "\n=== CLIENTI VIP (" << clientiVIP.size() << ") ===" << std::endl;
            for (const auto& client : clientiVIP) {
                client->afiseazaDetalii();
                std::cout << "---" << std::endl;
            }
            break;
        }
        case 5: {
            std::string nume, prenume;
            std::cout << "Introduceti numele clientului: ";
            std::getline(std::cin, nume);
            std::cout << "Introduceti prenumele clientului: ";
            std::getline(std::cin, prenume);

            Client* client = hotel.gasesteClient(nume, prenume);
            if (client) {
                client->setEsteVIP(true);
                std::cout << "Clientul a fost promovat la VIP!" << std::endl;
            } else {
                std::cout << "Clientul nu a fost gasit!" << std::endl;
            }
            break;
        }
        default:
            std::cout << "Optiune invalida!" << std::endl;
    }
}

void gestioneazaReceptioneri(Hotel<std::string>& hotel) {
    std::cout << "\n=== GESTIONARE RECEPTIONERI ===" << std::endl;
    std::cout << "1. Adauga receptioner nou\n";
    std::cout << "2. Afiseaza toti receptionerii\n";
    std::cout << "3. Cauta receptioner dupa ID\n";
    std::cout << "4. Mareste salarii (++)\n";
    std::cout << "5. Afiseaza receptioneri dupa program\n";

    int optiune;
    std::cout << "Optiune: ";
    std::cin >> optiune;
    std::cin.ignore();

    switch (optiune) {
        case 1: {
            std::cout << "Introduceti datele pentru receptionerul nou:\n";
            auto receptioner = std::make_unique<Receptioner>();
            std::cin >> *receptioner;
            hotel.adaugaReceptioner(std::move(receptioner));
            std::cout << "Receptionerul a fost adaugat cu succes!" << std::endl;
            break;
        }
        case 2: {
            std::cout << "\n=== TOTI RECEPTIONERII ===" << std::endl;
            std::cout << "Numar total receptioneri: " << hotel.getNumarReceptioneri() << std::endl;
            std::cout << "Salariu minim: " << utils::formatPrice(Receptioner::calculeazaSalariuMinim()) << std::endl;
            break;
        }
        case 3: {
            std::string id;
            std::cout << "Introduceti ID-ul angajatului: ";
            std::getline(std::cin, id);

            Receptioner* receptioner = hotel.gasesteReceptioner(id);
            if (receptioner) {
                receptioner->afiseazaDetalii();
            } else {
                std::cout << "Receptionerul nu a fost gasit!" << std::endl;
            }
            break;
        }
        case 4: {
            std::string id;
            std::cout << "Introduceti ID-ul angajatului: ";
            std::getline(std::cin, id);

            Receptioner* receptioner = hotel.gasesteReceptioner(id);
            if (receptioner) {
                double salariuVechi = receptioner->getSalariu();
                ++(*receptioner);
                std::cout << "Salariul a fost marit de la " << utils::formatPrice(salariuVechi)
                         << " la " << utils::formatPrice(receptioner->getSalariu()) << std::endl;
            } else {
                std::cout << "Receptionerul nu a fost gasit!" << std::endl;
            }
            break;
        }
        case 5: {
            std::string program;
            std::cout << "Introduceti programul (ziua/noaptea/mixt): ";
            std::getline(std::cin, program);

            auto receptioneri = hotel.getReceptioneriByProgram(program);
            std::cout << "\n=== RECEPTIONERI PROGRAM " << program << " (" << receptioneri.size() << ") ===" << std::endl;
            for (const auto& receptioner : receptioneri) {
                receptioner->afiseazaDetalii();
                std::cout << "---" << std::endl;
            }
            break;
        }
        default:
            std::cout << "Optiune invalida!" << std::endl;
    }
}

void gestioneazaRezervari(Hotel<std::string>& hotel) {
    std::cout << "\n=== GESTIONARE REZERVARI ===" << std::endl;
    std::cout << "1. Creeaza rezervare noua\n";
    std::cout << "2. Anuleaza rezervare\n";
    std::cout << "3. Verifica status rezervare\n";
    std::cout << "4. Afiseaza toate rezervarile\n";

    int optiune;
    std::cout << "Optiune: ";
    std::cin >> optiune;
    std::cin.ignore();

    switch (optiune) {
        case 1: {
            int numarCamera;
            std::string nume, prenume;

            std::cout << "Introduceti numarul camerei: ";
            std::cin >> numarCamera;
            std::cin.ignore();

            std::cout << "Introduceti numele clientului: ";
            std::getline(std::cin, nume);

            std::cout << "Introduceti prenumele clientului: ";
            std::getline(std::cin, prenume);

            if (hotel.facereRezervare(numarCamera, nume, prenume)) {
                std::cout << "Rezervarea a fost efectuata cu succes!" << std::endl;
            } else {
                std::cout << "Rezervarea nu a putut fi efectuata!" << std::endl;
            }
            break;
        }
        case 2: {
            int numarCamera;
            std::cout << "Introduceti numarul camerei pentru anulare: ";
            std::cin >> numarCamera;

            if (hotel.anulareRezervare(numarCamera)) {
                std::cout << "Rezervarea a fost anulata cu succes!" << std::endl;
            } else {
                std::cout << "Nu s-a gasit rezervare pentru aceasta camera!" << std::endl;
            }
            break;
        }
        case 3: {
            int numarCamera;
            std::cout << "Introduceti numarul camerei: ";
            std::cin >> numarCamera;

            if (hotel.esteRezervata(numarCamera)) {
                Client* client = hotel.getClientRezervare(numarCamera);
                std::cout << "Camera " << numarCamera << " este rezervata";
                if (client) {
                    std::cout << " de " << client->getNume() << " " << client->getPrenume();
                }
                std::cout << std::endl;
            } else {
                std::cout << "Camera " << numarCamera << " nu este rezervata." << std::endl;
            }
            break;
        }
        case 4: {
            std::cout << "\n=== TOATE REZERVARILE ===" << std::endl;
            std::cout << "Implementarea completa a listarii rezervarilor..." << std::endl;
            break;
        }
        default:
            std::cout << "Optiune invalida!" << std::endl;
    }
}

void afiseazaRapoarte(Hotel<std::string>& hotel) {
    std::cout << "\n=== RAPOARTE SI STATISTICI ===" << std::endl;

    std::cout << "Informatii generale:" << std::endl;
    std::cout << hotel << std::endl;

    std::cout << "\nStatistici detaliate:" << std::endl;
    std::cout << "- Persoane totale create: " << Persoana::getNumarTotalPersone() << std::endl;
    std::cout << "- Camere totale create: " << Camera::getNumarTotalCamere() << std::endl;
    std::cout << "- Clienti VIP: " << Client::getNumarClientiVIP() << std::endl;
    std::cout << "- Receptioneri totali: " << Receptioner::getNumarTotalReceptioneri() << std::endl;
    std::cout << "- Hoteluri create: " << Hotel<std::string>::getNumarTotalHoteluri() << std::endl;

    auto statisticiCamere = hotel.getStatisticiCamere();
    std::cout << "\nDistributia camerelor pe tipuri:" << std::endl;
    for (const auto& pair : statisticiCamere) {
        std::cout << "- " << Camera::tipCameraToString(pair.first) << ": " << pair.second << " camere" << std::endl;
    }
}

void demonstreazaSTL(Hotel<std::string>& hotel) {
    std::cout << "\n=== DEMONSTRARE STL SI LAMBDA ===" << std::endl;

    std::cout << "1. Filtrare camere cu pret > 200 RON:" << std::endl;
    auto camereScumpe = hotel.filtreazaCamere([](const Camera* camera) {
        return camera->getPretPeNoapte() > 200.0;
    });
    std::cout << "Gasit " << camereScumpe.size() << " camere scumpe." << std::endl;

    std::cout << "\n2. Filtrare clienti VIP:" << std::endl;
    auto clientiVIP = hotel.filtreazaClienti([](const Client* client) {
        return client->getEsteVIP();
    });
    std::cout << "Gasit " << clientiVIP.size() << " clienti VIP." << std::endl;

    std::cout << "\n3. Sortare automata:" << std::endl;
    hotel.sorteazaCamereByPret();
    hotel.sorteazaClientiByNume();
    hotel.sorteazaReceptioneriByExperienta();
    std::cout << "Sortarile au fost efectuate!" << std::endl;

    std::cout << "\n4. Demonstrare vector si algoritmi:" << std::endl;
    std::vector<int> numereCamere = {101, 102, 203, 304, 105};

    std::sort(numereCamere.begin(), numereCamere.end());
    std::cout << "Numere camere sortate: ";
    for (int nr : numereCamere) {
        std::cout << nr << " ";
    }
    std::cout << std::endl;

    std::vector<int> etaje;
    std::transform(numereCamere.begin(), numereCamere.end(), std::back_inserter(etaje),
        [](int numarCamera) { return numarCamera / 100; });

    std::cout << "Etajele corespunzatoare: ";
    for (int etaj : etaje) {
        std::cout << etaj << " ";
    }
    std::cout << std::endl;
}

void demonstreazaExceptii(Hotel<std::string>& hotel) {
    std::cout << "\n=== DEMONSTRARE EXCEPTII ===" << std::endl;

    std::cout << "1. Testare exceptie camera indisponibila:" << std::endl;
    try {
        Camera* camera = hotel.gasesteCamera(9999);
        if (!camera) {
            throw CameraIndisponibilaException(9999);
        }
    } catch (const CameraIndisponibilaException& e) {
        std::cout << "Capturata exceptie specifica: " << e.what() << std::endl;
    } catch (const HotelException& e) {
        std::cout << "Capturata exceptie de baza (upcasting): " << e.what() << std::endl;
    }

    std::cout << "\n2. Testare exceptie date invalide:" << std::endl;
    try {
        auto client = std::make_unique<Client>("", "Prenume", "123456789", 25, "email@test.com", "Adresa");
    } catch (const DateInvalideException& e) {
        std::cout << "Capturata exceptie date invalide: " << e.what() << std::endl;
    } catch (const HotelException& e) {
        std::cout << "Capturata exceptie de baza: " << e.what() << std::endl;
    }

    std::cout << "\n3. Testare propagare exceptii:" << std::endl;
    try {
        auto camera = std::make_unique<Camera>(-1, TipCamera::STANDARD, -100, 0, false, 0);
    } catch (const DateInvalideException& e) {
        std::cout << "Exceptie propagata din constructor: " << e.what() << std::endl;
    }

    std::cout << "\nTeste exceptii finalizate cu succes!" << std::endl;
}

void demonstreazaPolimorfism(Hotel<std::string>& /* hotel */) {
    std::cout << "\n=== DEMONSTRARE POLIMORFISM ===" << std::endl;

    std::vector<std::unique_ptr<Persoana>> persoane;

    persoane.push_back(std::make_unique<Client>("Ion", "Popescu", "0741234567", 35,
                                               "ion.popescu@email.com", "Str. Florilor nr. 10", true));
    persoane.push_back(std::make_unique<Receptioner>("Maria", "Ionescu", "0742345678", 28,
                                                    "REC001", 3500.0, "ziua", 3));
    persoane.push_back(std::make_unique<Client>("Ana", "Marinescu", "0743456789", 42,
                                               "ana.marinescu@email.com", "Bdul Unirii nr. 15", false));

    std::cout << "Demonstrare apelare metode virtuale (polimorfism):" << std::endl;
    for (const auto& persoana : persoane) {
        std::cout << "\n--- " << persoana->getTipPersoana() << " ---" << std::endl;
        persoana->afiseazaDetalii();
    }

    std::cout << "\nDemonstrare downcast sigur:" << std::endl;
    for (const auto& persoana : persoane) {
        if (Client* client = dynamic_cast<Client*>(persoana.get())) {
            std::cout << "Downcast la Client reusit pentru: " << client->getNume()
                     << " (VIP: " << (client->getEsteVIP() ? "Da" : "Nu") << ")" << std::endl;
        }
        else if (Receptioner* receptioner = dynamic_cast<Receptioner*>(persoana.get())) {
            std::cout << "Downcast la Receptioner reusit pentru: " << receptioner->getNume()
                     << " (ID: " << receptioner->getIdAngajat() << ")" << std::endl;
        }
    }

    std::cout << "\nDemonstrare clonare (Virtual Constructor Pattern):" << std::endl;
    auto primeaPersoana = persoane[0]->clone();
    std::cout << "Persoana clonata: " << primeaPersoana->getNume() << " "
              << primeaPersoana->getPrenume() << " (" << primeaPersoana->getTipPersoana() << ")" << std::endl;
}

void demonstreazaFunctionalitati(Hotel<std::string>& hotel) {
    std::cout << "\n=== INITIALIZARE CU DATE DE TEST ===" << std::endl;

    try {
        std::cout << "Adaugare camere de test..." << std::endl;

        hotel.adaugaCamera(std::make_unique<Camera>(101, TipCamera::STANDARD, 150.0, 2, false, 1));
        hotel.adaugaCamera(std::make_unique<Camera>(102, TipCamera::DELUXE, 220.0, 2, true, 1));
        hotel.adaugaCamera(std::make_unique<Camera>(201, TipCamera::SUITE, 350.0, 4, true, 2));
        hotel.adaugaCamera(std::make_unique<Camera>(301, TipCamera::APARTAMENT, 500.0, 6, true, 3));

        std::cout << "Adaugare clienti de test..." << std::endl;

        hotel.adaugaClient(std::make_unique<Client>("Ion", "Popescu", "0741234567", 35,
                                                   "ion.popescu@email.com", "Str. Florilor nr. 10", false));
        hotel.adaugaClient(std::make_unique<Client>("Maria", "Vasilescu", "0742345678", 42,
                                                   "maria.vasilescu@email.com", "Bdul Libertății nr. 25", true));
        hotel.adaugaClient(std::make_unique<Client>("Andrei", "Marin", "0743456789", 28,
                                                   "andrei.marin@email.com", "Str. Mihai Eminescu nr. 5", false));

        std::cout << "Adaugare receptioneri de test..." << std::endl;

        auto receptioner1 = std::make_unique<Receptioner>("Elena", "Constantinescu", "0744567890", 30,
                                                         "REC001", 3200.0, "ziua", 5);
        receptioner1->adaugaLimba("Romana");
        receptioner1->adaugaLimba("Engleza");
        receptioner1->adaugaLimba("Franceza");
        hotel.adaugaReceptioner(std::move(receptioner1));

        auto receptioner2 = std::make_unique<Receptioner>("Cristian", "Dumitrescu", "0745678901", 25,
                                                         "REC002", 2800.0, "noaptea", 2);
        receptioner2->adaugaLimba("Romana");
        receptioner2->adaugaLimba("Engleza");
        hotel.adaugaReceptioner(std::move(receptioner2));

        std::cout << "Adaugare servicii speciale..." << std::endl;
        hotel.adaugaServiciuSpecial(std::string("Spa & Wellness"));
        hotel.adaugaServiciuSpecial(std::string("Transport aeroport"));
        hotel.adaugaServiciuSpecial(std::string("Room service 24/7"));
        hotel.adaugaServiciuSpecial(std::string("Concierge premium"));

        std::cout << "Creare rezervari de test..." << std::endl;
        hotel.facereRezervare(101, "Ion", "Popescu");
        hotel.facereRezervare(201, "Maria", "Vasilescu");

        std::cout << "Initializarea a fost completata cu succes!" << std::endl;

        std::cout << "\n=== DEMONSTRARE OPERATORI SUPRAINCARCAT ===" << std::endl;

        Client* client = hotel.gasesteClient("Maria", "Vasilescu");
        if (client) {
            std::cout << "Client VIP gasit: " << client->getNume() << std::endl;
            *client += 1500.0;
            std::cout << "Adaugat bonus pentru client VIP" << std::endl;

            double pretOriginal = 300.0;
            double pretCuDiscount = pretOriginal * (*client);
            std::cout << "Pret original: " << utils::formatPrice(pretOriginal) << std::endl;
            std::cout << "Pret cu discount VIP: " << utils::formatPrice(pretCuDiscount) << std::endl;
        }

        Camera* camera = hotel.gasesteCamera(102);
        if (camera) {
            std::cout << "\nCamera gasita: " << camera->getNumar() << std::endl;
            std::cout << "Pret initial: " << utils::formatPrice(camera->getPretPeNoapte()) << std::endl;

            *camera += 50.0;
            std::cout << "Pret dupa marire: " << utils::formatPrice(camera->getPretPeNoapte()) << std::endl;

            Camera cameraNou = *camera + 100.0;
            std::cout << "Pret camera noua (operator +): " << utils::formatPrice(cameraNou.getPretPeNoapte()) << std::endl;
        }

        Receptioner* receptioner = hotel.gasesteReceptioner("REC001");
        if (receptioner) {
            std::cout << "\nReceptioner gasit: " << receptioner->getNume() << std::endl;
            std::cout << "Salariu initial: " << utils::formatPrice(receptioner->getSalariu()) << std::endl;

            ++(*receptioner);
            std::cout << "Salariu dupa marire (++): " << utils::formatPrice(receptioner->getSalariu()) << std::endl;
        }

    } catch (const HotelException& e) {
        std::cout << "EROARE la initializare: " << e.what() << std::endl;
    }
}

void demonstreazaTemplate() {
    std::cout << "\n=== DEMONSTRARE TEMPLATE CU TIP DIFERIT ===" << std::endl;

    Hotel<int>& hotelNumeric = Hotel<int>::getInstance("Hotel Numeric", "Adresa Test", 4);

    hotelNumeric.adaugaServiciuSpecial(101);
    hotelNumeric.adaugaServiciuSpecial(102);
    hotelNumeric.adaugaServiciuSpecial(103);

    std::cout << "Hotel cu template <int> creat cu "
              << hotelNumeric.getNumarServiciiSpeciale() << " servicii numerice." << std::endl;

    std::cout << "Demonstrare template completata!" << std::endl;
}