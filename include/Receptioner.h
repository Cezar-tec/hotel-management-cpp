#pragma once
#include "Persoana.h"
#include <string>
#include <vector>

class Receptioner : public Persoana {
private:
    std::string idAngajat;
    double salariu;
    std::string program;
    int experienta;
    std::vector<std::string> limbiVorbite;
    
    static int numarTotalReceptioneri;
    
public:
    Receptioner();
    Receptioner(const std::string& nume, const std::string& prenume,
                const std::string& telefon, int varsta, const std::string& idAngajat,
                double salariu, const std::string& program, int experienta);
    
    Receptioner(const Receptioner& other);
    Receptioner& operator=(const Receptioner& other);
    ~Receptioner() override;
    
    const std::string& getIdAngajat() const { return idAngajat; }
    double getSalariu() const { return salariu; }
    const std::string& getProgram() const { return program; }
    int getExperienta() const { return experienta; }
    const std::vector<std::string>& getLimbiVorbite() const { return limbiVorbite; }
    
    void setIdAngajat(const std::string& id);
    void setSalariu(double salariu);
    void setProgram(const std::string& program);
    void setExperienta(int experienta);
    
    void adaugaLimba(const std::string& limba);
    bool vorbeLimba(const std::string& limba) const;
    int getNumarLimbi() const { return static_cast<int>(limbiVorbite.size()); }
    
    static int getNumarTotalReceptioneri() { return numarTotalReceptioneri; }
    static double calculeazaSalariuMinim() { return 2500.0; }
    static bool validareProgramLucru(const std::string& program);
    
    bool poatePreluaRezervari() const;
    double calculeazaBonusExperienta() const;
    
    void afiseazaDetalii() const override;
    std::string getTipPersoana() const override { return "Receptioner"; }
    std::unique_ptr<Persoana> clone() const override;
    
    Receptioner& operator++();
    Receptioner operator++(int);
    bool operator<(const Receptioner& other) const;
    
protected:
    void citireSpecifica(std::istream& is) override;
    void afisareSpecifica(std::ostream& os) const override;
    
private:
    void actualizeazaNumarReceptioneri(bool adauga);
};

bool operator>(const Receptioner& a, const Receptioner& b);