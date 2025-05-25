#pragma once
#include <string>
#include <iostream>
#include <memory>

class Persoana {
protected:
    std::string nume;
    std::string prenume;
    std::string telefon;
    int varsta;
    
    static int numarTotalPersone;
    
public:
    Persoana();
    Persoana(const std::string& nume, const std::string& prenume, 
             const std::string& telefon, int varsta);
    Persoana(const Persoana& other);
    
    Persoana& operator=(const Persoana& other);
    virtual ~Persoana();
    
    const std::string& getNume() const { return nume; }
    const std::string& getPrenume() const { return prenume; }
    const std::string& getTelefon() const { return telefon; }
    int getVarsta() const { return varsta; }
    
    void setNume(const std::string& nume);
    void setPrenume(const std::string& prenume);
    void setTelefon(const std::string& telefon);
    void setVarsta(int varsta);
    
    static int getNumarTotalPersone() { return numarTotalPersone; }
    static void resetNumarPersone() { numarTotalPersone = 0; }
    
    virtual void afiseazaDetalii() const = 0;
    virtual std::string getTipPersoana() const = 0;
    virtual std::unique_ptr<Persoana> clone() const = 0;
    
    friend std::ostream& operator<<(std::ostream& os, const Persoana& p);
    friend std::istream& operator>>(std::istream& is, Persoana& p);
    
    bool operator==(const Persoana& other) const;
    friend bool operator<(const Persoana& a, const Persoana& b);
    
protected:
    virtual void citireSpecifica(std::istream& is) = 0;
    virtual void afisareSpecifica(std::ostream& os) const = 0;
};