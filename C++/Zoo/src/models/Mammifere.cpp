#include <string>
#include <iostream>
#include "../../include/models/Mammifere.hpp"
#include "../../include/models/Animal.hpp"


// Constructeur
Mammifere::Mammifere(int id, const std::string &nom, const std::string& espece, int age, double poids, int id_enclos, std::string& typeFourrure)
    : Animal(id, nom, espece, age, poids, id_enclos), typeFourrure(typeFourrure) {};


// Getters
std::string& Mammifere::getTypeFourrure() const {
    return typeFourrure;
}

// Setters
void Mammifere::setTypeFourrure(std::string& nouvelleFourrure) const {
    this->typeFourrure = nouvelleFourrure;
}