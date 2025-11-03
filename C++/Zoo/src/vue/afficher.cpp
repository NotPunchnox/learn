#include <iostream>
#include "../../include/vue/afficher.hpp"


// Constructeur
Afficher::Afficher() : color("") {};


// Getters
const std::string Afficher::getColor() const {
  return color;
}

// Setters
void Afficher::setColor(const std::string color) {
  this->color = color;
}


// Méthodes
void Afficher::menu() const {
  std::cout << "=============================================\n"
            << "1. Animaux (afficher, modifier, supprimer...)\n"
            << "2. Employé (afficher, ajouter..)             \n"
            << "3. Enclos (afficher, modifier, supprimer...) \n"
            << std::endl;
}
