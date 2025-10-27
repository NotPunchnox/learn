#pragma once
#include "Animal.hpp"
#include <string>

// Déclaration de l'héritage mammifère
class Reptile: public Animal {

public:
    // Constructeur
    Reptile(int id, const std::string &nom, const std::string &espece, int age, double poids, int id_enclos, const std::string &envergure);

    // Destructeur
    virtual ~Reptile() = default;

    // Getters
    const std::string& getEnvergure() const;

    // Setters
    void setEnvergure(const std::string &envergure);

    // Méthodes
    void emettreSon() const override;

private:
    std::string envergure;
    
};