#include <string>
#include <iostream>
#include "../../include/models/Reptile.hpp"

// Constructeur
Reptile::Reptile(int id, const std::string &nom, const std::string &espece, int age, double poids, int id_enclos, bool estVenimeux)
    : Animal(id, nom, espece, age, poids, id_enclos), estVenimeux(estVenimeux) {};


bool Reptile::getIsVenomous() const {
    return estVenimeux;
}

void Reptile::setIsVenomous(bool estVenimeux) {
    this->estVenimeux = estVenimeux;
}

void Reptile::emettreSon() const {
    std::cout << "le serpent " << getNom() << " siffle..." << std::endl;
}

std::string Reptile::getType() const {
  return "REPTILE";
}
