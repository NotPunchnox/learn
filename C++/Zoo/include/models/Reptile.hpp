#pragma once
#include "Animal.hpp"
#include <string>

// Déclaration de l'héritage mammifère
class Reptile: public Animal {

public:
    // Constructeur
    Reptile(int id, const std::string &nom, const std::string& espece, int age, double poids, int id_enclos, bool estVenimeux);

    // Destructeur
    virtual ~Reptile() = default;

    // Getters
    bool getIsVenomous() const;

    // Setters
    void setIsVenomous(bool estVenimeux);

    // Méthodes
    void emettreSon() const override;
    std::string Reptile::getType() const { return "REPTILE"; }

private:
    bool estVenimeux;
    
};