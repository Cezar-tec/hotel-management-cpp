#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>

enum class TipCamera {
    STANDARD,
    DELUXE,
    SUITE,
    APARTAMENT
};

enum class StatusCamera {
    LIBERA,
    OCUPATA,
    MENTENANTA,
    REZERVATA
};

class Camera {
private:
    int numar;
    TipCamera tip;
    StatusCamera status;
    double pretPeNoapte;
    int capacitateMaxima;
    std::vector<std::string> facilitati;
    bool areBalcon;
    int etaj;
    
    static int numarTotalCamere;
    static double pretBaza;
    
public:
    Camera();
    Camera(int numar, TipCamera tip, double pretPeNoapte, 
           int capacitateMaxima, bool areBalcon, int etaj);
    
    Camera(int numar, TipCamera tip, double pretPeNoapte, 
           int capacitateMaxima, bool areBalcon, int etaj,
           const std::vector<std::string>& facilitati);
    
    Camera(const Camera& other);
    Camera& operator=(const Camera& other);
    ~Camera();
    
    int getNumar() const { return numar; }
    TipCamera getTip() const { return tip; }
    StatusCamera getStatus() const { return status; }
    double getPretPeNoapte() const { return pretPeNoapte; }
    int getCapacitateMaxima() const { return capacitateMaxima; }
    const std::vector<std::string>& getFacilitati() const { return facilitati; }
    bool getAreBalcon() const { return areBalcon; }
    int getEtaj() const { return etaj; }
    
    void setStatus(StatusCamera status);
    void setPretPeNoapte(double pret);
    void setAreBalcon(bool balcon);
    
    void adaugaFacilitate(const std::string& facilitate);
    bool areFacilitate(const std::string& facilitate) const;
    void eliminaFacilitate(const std::string& facilitate);
    int getNumarFacilitati() const { return static_cast<int>(facilitati.size()); }
    
    static int getNumarTotalCamere() { return numarTotalCamere; }
    static double getPretBaza() { return pretBaza; }
    static void setPretBaza(double pret) { pretBaza = pret; }
    static std::string tipCameraToString(TipCamera tip);
    static std::string statusCameraToString(StatusCamera status);
    
    bool esteDisponibila() const;
    double calculeazaPret(int numarNopti) const;
    double calculeazaPretCuFacilitati() const;
    
    friend std::ostream& operator<<(std::ostream& os, const Camera& camera);
    friend std::istream& operator>>(std::istream& is, Camera& camera);
    
    Camera& operator+=(double marire);
    bool operator<(const Camera& other) const;
    bool operator==(const Camera& other) const;
    
    friend bool operator>(const Camera& a, const Camera& b);
    friend Camera operator+(const Camera& camera, double marire);
    
private:
    void initializareFacilitatiDefault();
    double calculeazaFactorTip() const;
};