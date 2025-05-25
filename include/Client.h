#pragma once
#include "Persoana.h"
#include <vector>
#include <string>

class Client : public Persoana {
private:
    std::string email;
    std::string adresa;
    std::vector<int> rezervari;
    bool esteVIP;
    double discountVIP;
    
    static int numarClientiVIP;
    
public:
    Client();
    Client(const std::string& nume, const std::string& prenume, 
           const std::string& telefon, int varsta, const std::string& email, 
           const std::string& adresa, bool esteVIP = false);
    
    Client(const Client& other);
    Client& operator=(const Client& other);
    ~Client() override;
    
    const std::string& getEmail() const { return email; }
    const std::string& getAdresa() const { return adresa; }
    bool getEsteVIP() const { return esteVIP; }
    double getDiscountVIP() const { return discountVIP; }
    const std::vector<int>& getRezervari() const { return rezervari; }
    
    void setEmail(const std::string& email);
    void setAdresa(const std::string& adresa);
    void setEsteVIP(bool vip);
    
    void adaugaRezervare(int idCamera);
    bool eliminaRezervare(int idCamera);
    bool areRezervari() const { return !rezervari.empty(); }
    int getNumarRezervari() const { return static_cast<int>(rezervari.size()); }
    
    static int getNumarClientiVIP() { return numarClientiVIP; }
    static double calculeazaDiscountDefault() { return 0.15; }
    
    void afiseazaDetalii() const override;
    std::string getTipPersoana() const override { return "Client"; }
    std::unique_ptr<Persoana> clone() const override;
    
    Client& operator+=(double suma);
    bool operator>(const Client& other) const;
    
protected:
    void citireSpecifica(std::istream& is) override;
    void afisareSpecifica(std::ostream& os) const override;
    
private:
    void actualizeazaNumarVIP(bool nouValoare);
};

double operator*(double pret, const Client& client);