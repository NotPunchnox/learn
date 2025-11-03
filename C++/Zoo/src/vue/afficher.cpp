#include <iostream>
#include "../../include/vue/afficher.cpp"


// Constructeur
Afficher::Afficher();

Afficher::Afficher(const std::string color) :
  color(color) {};


// Getters
const std::string Afficher::getColor() {
  return color;
}

// Setters
void Afficher::setColor(const std::string color) {
  this->color = color;
}


// Méthodes
void Afficher::menu() {
  std::cout << "===================================="
            << "1. Animaux (afficher, modifier, supprimer...)"
            << "2. Employé (afficher, ajouter..)"
            << "3. Enclos (afficher, modifier, supprimer...)"
            << std::endl;
}
