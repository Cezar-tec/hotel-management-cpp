#pragma once
#include "Camera.h"
#include "Client.h"
#include "Receptioner.h"
#include "exceptions.h"
#include "utils.h"
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>

template<typename T>
class Hotel {
private:
    std::string nume;
    std::string adresa;
    int numarStele;
    std::vector<std::unique_ptr<Camera>> camere;
    std::vector<std::unique_ptr<Client>> clienti;
    std::vector<std::unique_ptr<Receptioner>> receptioneri;
    std::map<int, int> rezervari;
    std::vector<T> serviciiSpeciale;

    static std::unique_ptr<Hotel<T>> instance;

    Hotel(const std::string& nume, const std::string& adresa, int numarStele);

    static int numarTotalHoteluri;

public:
    static Hotel<T>& getInstance(const std::string& nume = "Hotel Imperial",
                                 const std::string& adresa = "Str. Centrala nr. 1",
                                 int numarStele = 4);

    ~Hotel();

    Hotel(const Hotel&) = delete;
    Hotel& operator=(const Hotel&) = delete;

    const std::string& getNume() const { return nume; }
    const std::string& getAdresa() const { return adresa; }
    int getNumarStele() const { return numarStele; }
    size_t getNumarCamere() const { return camere.size(); }
    size_t getNumarClienti() const { return clienti.size(); }
    size_t getNumarReceptioneri() const { return receptioneri.size(); }
    const std::vector<T>& getServiciiSpeciale() const { return serviciiSpeciale; }

    void setNume(const std::string& nume);
    void setAdresa(const std::string& adresa);
    void setNumarStele(int stele);

    static int getNumarTotalHoteluri() { return numarTotalHoteluri; }

    bool proceseazaRezervare(int numarCamera, const std::string& numeClient);

protected:
    virtual bool verificaDisponibilitate(int numarCamera) const;
    virtual bool validareClient(const std::string& numeClient) const;
    virtual void confirmaRezervare(int numarCamera, int idClient);

public:
    void adaugaCamera(std::unique_ptr<Camera> camera);
    void eliminaCamera(int numarCamera);
    Camera* gasesteCamera(int numarCamera) const;
    std::vector<Camera*> getCamereDisponibile() const;
    std::vector<Camera*> getCamereByTip(TipCamera tip) const;

    void adaugaClient(std::unique_ptr<Client> client);
    void eliminaClient(const std::string& nume, const std::string& prenume);
    Client* gasesteClient(const std::string& nume, const std::string& prenume) const;
    std::vector<Client*> getClientiVIP() const;

    void adaugaReceptioner(std::unique_ptr<Receptioner> receptioner);
    void eliminaReceptioner(const std::string& idAngajat);
    Receptioner* gasesteReceptioner(const std::string& idAngajat) const;
    std::vector<Receptioner*> getReceptioneriByProgram(const std::string& program) const;

    void adaugaServiciuSpecial(const T& serviciu);
    void eliminaServiciuSpecial(const T& serviciu);
    bool areServiciuSpecial(const T& serviciu) const;
    size_t getNumarServiciiSpeciale() const { return serviciiSpeciale.size(); }

    bool facereRezervare(int numarCamera, const std::string& numeClient, const std::string& prenumeClient);
    bool anulareRezervare(int numarCamera);
    bool esteRezervata(int numarCamera) const;
    Client* getClientRezervare(int numarCamera) const;

    std::vector<Camera*> filtreazaCamere(std::function<bool(const Camera*)> predicat) const;
    std::vector<Client*> filtreazaClienti(std::function<bool(const Client*)> predicat) const;

    void sorteazaCamereByPret();
    void sorteazaClientiByNume();
    void sorteazaReceptioneriByExperienta();

    double calculeazaVenitTotal() const;
    double calculeazaOcupare() const;
    std::map<TipCamera, int> getStatisticiCamere() const;

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Hotel<U>& hotel);

    bool operator==(const Hotel<T>& other) const;

private:
    int getNextClientId() const;
    void actualizeazaStatusCamere();
    bool validareNumarCamera(int numar) const;
};