#include <iostream>
#include "../../include/vue/afficherAnimal.hpp"
#include "../../include/vue/afficher.hpp"

// Constructeur
afficherAnimal::afficherAnimal() : id_menu(0) {};
afficherAnimal::afficherAnimal(int id_menu) : id_menu(id_menu) {};


// Getters
int afficherAnimal::getIdMenu() const {
  return id_menu;
}

// Setters
void afficherAnimal::setIdMenu(int id_menu) {
  this->id_menu = id_menu;
}

// Méthodes
void afficherAnimal::menu() const {
  int choix;

  // Afficher le menu des animaux
  std::cout << "=== MENU ANIMAUX ===\n
            << "1. Ajouter un animal  \n"
            << "2. Supprimer un animal \n"
            << "3. Afficher les animaux\n"
            << "4. Retour au menu principal\n"
            << "======================\n" << std::endl;

  std::cout << "Veuillez entrer votre choix: ";

  // Récupérer l'entrée de l'utilisateur
  std::cin >> choix,
  
  // Traiter le choix de l'utilisateur
  switch (choix) {
    
    case 1:
      std::cout << "Ajouter un animal sélectionné."
      break;
    
    case 2:
      std::cout << "Supprimer un animal sélectionné."
      break;
    
    case 3:
      std::cout << "Afficher les animaux sélectionné."
      break;

    case 4:
      system("clear");
      std::cout << "Retour au menu principal sélectionné."
      
      afficher *afficher;
      afficher->menu();

      break;
  }

}
