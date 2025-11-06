#include <iostream>
#include "../../include/vue/afficherAnimal.hpp"
#include "../../include/vue/afficher.hpp"
#include "../../include/dao/AnimalDao.hpp"
#include "../../include/models/Mammifere.hpp"
#include "../../include/models/Oiseau.hpp"
#include "../../include/models/Reptile.hpp"

// Constructeur
afficherAnimal::afficherAnimal(DatabaseManager *db)
  : db(db), color("default") {};

afficherAnimal::afficherAnimal(DatabaseManager *db, std::string color)
  : db(db), color(color) {};

// Getters
std::string afficherAnimal::getColor() const {
  return color;
}

// Setters
void afficherAnimal::setColor(const std::string& color) {
  this->color = color;
}

// Méthodes
void afficherAnimal::menu() {
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

  AnimalDao animalDao(db);

  // Traiter le choix de l'utilisateur
  switch (choix) {
    
    case 1: {
      Animal* nouvelAnimal;
      std::string typeString;
      std::string nom;
      std::string espece;
      std::string typeFourrure = "";
      std::string envergure = "";
      std::string estVenimeuxString = "";
      double poids;
      bool estVenimeux;
      int age;
      int id_enclos;
      int type;

      std::cout << "Veuillez entrer les informations de l'animal à ajouter." << std::endl;
      
      // Récupérer le type
      std::cout << "==== Choix du type de l'animal ====\n"
                << "1. Mammifère  \n"
                << "2. Oiseau     \n"
                << "3. Réptile    \n"
                << "Votre choix : ";
      std::cin >> type;

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

      switch (type) {
        case 1: {
          typeString = "MAMMIFERE";
          std::cout << "Type de fourrure : ";
          std::cin  >> typeFourrure;
          
          Mammifere m(0, nom, espece, age, poids, id_enclos, typeFourrure);

          animalDao.insert(&m);
          break;
        }
        case 2: {
          typeString = "OSIEAU";
          std::cout << "Envergure (cm): ";
          std::cin >> envergure;
          break;
        }
        case 3: {
          typeString = "REPTILE";
          std::cout << "est vénimeux (o/n) :";
          std::cin >> estVenimeuxString;
          if (estVenimeuxString == "o") {
            estVenimeux = true;
          } else {
            estVenimeux = false;
          }
          break;
        }

        default:
          std::cout << "Type invalide. Retour au menu des animaux." << std::endl;
          this->menu();
          return;
      };

      system("clear");
      std::cout << "L'Animal " << nom << "ajouté avec succès !" << std::endl;
      
      this->menu();
      break;
    }

    case 2: {
      std::cout << "Supprimer un animal sélectionné.";
      int idASupprimer;
      std::cout << "Veuillez entrer l'ID de l'animal à supprimer: ";
      std::cin >> idASupprimer;

      AnimalDao *animalDaoSuppr = new AnimalDao(db);
      animalDaoSuppr->remove(idASupprimer);
      
      system("clear");
      std::cout << "L'Animal avec l'ID " << idASupprimer << " a été supprimé avec succès !" << std::endl;

      this->menu();
      break;
    }

    case 3: {
      std::cout << "Afficher les animaux sélectionné.";
      break;
    }

    case 4: {
      system("clear");
      std::cout << "Retour au menu principal sélectionné.";
      
      Afficher *afficher = new Afficher(db);
      afficher->menu();

      break;
    }

    default:
      std::cout << "Choix invalide. Veuillez réessayer." << std::endl;
      system("clear");
      break;
  };

};
