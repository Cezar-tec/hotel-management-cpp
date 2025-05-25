#pragma once
#include <stdexcept>
#include <string>

class HotelException : public std::exception {
protected:
    std::string message;
    
public:
    explicit HotelException(const std::string& msg) : message(msg) {}
    virtual ~HotelException() noexcept = default;
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class CameraIndisponibilaException : public HotelException {
public:
    explicit CameraIndisponibilaException(int numarCamera) 
        : HotelException("Camera " + std::to_string(numarCamera) + " este indisponibila") {}
};

class ClientInexistentException : public HotelException {
public:
    explicit ClientInexistentException(const std::string& nume) 
        : HotelException("Clientul " + nume + " nu a fost gasit") {}
};

class DateInvalideException : public HotelException {
public:
    explicit DateInvalideException(const std::string& detalii) 
        : HotelException("Date invalide: " + detalii) {}
};

class CapacitateDepasitaException : public HotelException {
public:
    explicit CapacitateDepasitaException() 
        : HotelException("Capacitatea hotelului a fost depasita") {}
};

class OperatieInvalidaException : public HotelException {
public:
    explicit OperatieInvalidaException(const std::string& operatie) 
        : HotelException("Operatie invalida: " + operatie) {}
};