#include <string>
#include <iostream>
#include "../../include/models/Mammifere.hpp"
#include "../../include/models/Animal.hpp"


// Constructeur
Mammifere::Mammifere(int id, const std::string &nom, const std::string& espece, int age, double poids, int id_enclos, const std::string &typeFourrure)
    : Animal(id, nom, espece, age, poids, id_enclos), typeFourrure(typeFourrure) {};


// Getters
const std::string& Mammifere::getTypeFourrure() const {
    return typeFourrure;
}

// Setters
void Mammifere::setTypeFourrure(const std::string& nouvelleFourrure) {
    this->typeFourrure = nouvelleFourrure;
}

void Mammifere::emettreSon() const {
    std::cout << "Le mammifère " << nom << " Grogne..." << std::endl;
}