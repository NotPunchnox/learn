#pragma once
#include "Animal.hpp"
#include <string>

// Déclaration de l'héritage mammifère
class Mammifere: public Animal {

public:
    // Constructeur
    Mammifere(int id, const std::string &nom, const std::string& espece, int age, double poids, int id_enclos, const std::string &typeFourrure);

    // Destructeur
    virtual ~Mammifere() = default;

    // Getters
    const std::string& getTypeFourrure() const;

    // Setters
    void setTypeFourrure(const std::string &typeFourrure);

    // Méthodes
    void emettreSon() const override;
    std::string getType() const;

private:
    std::string typeFourrure;
    
};
