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
  
  int choix;

  // Afficher le menu principal
  std::cout << "=== MENU PRINCIPAL ===\n"
            << "1. Gérer les animaux  \n"
            << "2. Gérer les enclos   \n"
            << "4. Simuler une journée\n"
            << "5. Quitter            \n"
            << "======================\n" << std::endl;

  // Récupérer l'entrée de l'utilisateur
  std::cin >> choix;

  // Traiter le choix de l'utilisateur
  switch (choix) {
    case 1:
      std::cout << "Gérer les animaux sélectionné." << std::endl;
      break;
    case 2:
      std::cout << "Gérer les enclos sélectionné." << std::endl;
      break;
    case 4:
      std::cout << "Simuler une journée sélectionné." << std::endl;
      break;
    case 5:
      std::cout << "Quitter sélectionné." << std::endl;
      exit(0);
      break;
    default:
      std::cout << "Choix invalide. Veuillez réessayer." << std::endl;
      system("clear");
      break;
  }

}
