#pragma once
#include "Animal.hpp"
#include <string>

// Déclaration de l'héritage mammifère
class Mammifere: public Animal {

public:
    // Constructeur
    Mammifere(int id, const std::string &nom, const std::string& espece, int age, double poids, int id_enclos, std::string& typeFourrure);

    // Destructeur
    virtual ~Mammifere() = default;

    // Getters
    std::string& getTypeFourrure() const;

    // Setters
    void setTypeFourrure(std::string&) const;

    // Méthodes
    void emettreSon() const;

protected:
    std::string& typeFourrure;
};