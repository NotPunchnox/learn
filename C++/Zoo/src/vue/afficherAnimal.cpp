#include <iostream>
#include "../../include/vue/afficherAnimal.hpp"
#include "../../include/vue/afficher.hpp"
#include "../../include/dao/AnimalDao.hpp"

// Constructeur
afficherAnimal::afficherAnimal() : color("default") {};
afficherAnimal::afficherAnimal(std::string color) : color(color) {};

// Getters
std::string afficherAnimal::getColor() const {
  return color;
}

// Setters
void afficherAnimal::setColor(const std::string& color) {
  this->color = color;
}

// Méthodes
void afficherAnimal::menu() const {
  int choix;

  // Afficher le menu des animaux
  std::cout << "=== MENU ANIMAUX ===\n"
            << "1. Ajouter un animal  \n"
            << "2. Supprimer un animal \n"
            << "3. Afficher les animaux\n"
            << "4. Retour au menu principal\n"
            << "======================\n" << std::endl;

  // Récupérer le choix de l'utilisateur
  std::cout << "Veuillez entrer votre choix: ";
  std::cin >> choix;
 
  // Traiter le choix de l'utilisateur
  switch (choix) {
    
    case 1:
      std::string nom;
      std::string espece;
      int age;
      double poids;
      int id_enclos;


      std::cout << "Ajouter un animal sélectionné." << std::endl;
      std::cout << "Veuillez entrer les informations de l'animal à ajouter." << std::endl;
      std:: cout << "Nom: ";
      std::cin >> nom;
      std::cout << "Espèce: ";
      std::cin >> espece;
      std::cout << "Âge: ";
      std::cin >> age;
      std::cout << "Poids: ";
      std::cin >> poids;
      std::cout << "ID Enclos: ";
      std::cin >> id_enclos;

      Animal nouvelAnimal(0, nom, espece, age, poids, id_enclos);
      //Animal *nouvelAnimal = new Animal(0, nom, espece, age, poids, id_enclos);
      //AnimalDao animalDao;
      animalDao.insert(nouvelAnimal);

      system("clear");
      std::cout << "L'Animal " << nom << "ajouté avec succès !" << std::endl;
      
      this->menu();
      break;
    
    case 2:
      std::cout << "Supprimer un animal sélectionné.";
      int idASupprimer;
      std::cout << "Veuillez entrer l'ID de l'animal à supprimer: ";
      std::cin >> idASupprimer;

      AnimalDao animalDaoSuppr;
      animalDaoSuppr.remove(idASupprimer);
      
      system("clear");
      std::cout << "L'Animal avec l'ID " << idASupprimer << " a été supprimé avec succès !" << std::endl;

      this->menu();
      break;
    
    case 3:
      std::cout << "Afficher les animaux sélectionné.";
      break;

    case 4:
      system("clear");
      std::cout << "Retour au menu principal sélectionné.";
      
      afficher *afficher;
      afficher->menu();

      break;

    default:
      std::cout << "Choix invalide. Veuillez réessayer." << std::endl;
      system("clear");
      break;
  };

};
