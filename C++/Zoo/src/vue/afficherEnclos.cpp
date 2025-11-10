#include <iostream>
#include "../../include/utils/DatabaseManager.hpp"
#include "../../include/vue/afficherEnclos.hpp"

using namespace std;

// Constructeur
afficherEnclos::afficherEnclos(DatabaseManager *db)
  : db(db), color("default") {};
afficherEnclos::afficherEnclos(DatabaseManager *db, string color)
  : db(db), color(color);


// Getters
string afficherEnclos::getColor() const {
  return color;
}

// Setters
void afficherEnclos::setColor(const string& color) {
  this->color = color;
}

// Méthodes
void afficherEnclos::menu() {

  int choix;

  cout << "=== GESTION DES ENCLOS ===\n"
       << "1. Afficher les enclos\n"
       << "2. Créer un nouvel enclos\n"
       << "3. Voir les animaux d’un enclos\n"
       << "4. Supprimer un enclos\n"
       << "5. Retour\n"
       << "\nVotre choix: ";

  cin >> choix;

  switch (choix) {

    case 1:
      break;

    case 2:
      break;

    case 3:
      break;

    case 4:
      break;

    case 5:
      break;

  }

};
