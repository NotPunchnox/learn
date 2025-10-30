#include <iostream>
#include <memory>
#include <vector>
#include "include/dao/AnimalDao.hpp"
#include "include/models/Mammifere.hpp"
#include "include/utils/DatabaseManager.hpp"

int main(void)
{

   std::cout << "Bienvenue dans le gestionnaire de Zoo !" << std::endl;
   DatabaseManager *db = new DatabaseManager("localhost", "root", "password", "zoo_db");

   // Connexion à la base de données
   if (!db->connecter()) {
      std::cerr << "Échec de la connexion à la base de données." << std::endl;
      delete db;
      return 1;
   }

   // Récupérer les animeaux présents dans la db
   AnimalDao animalDao(db);
   std::vector<Animal*> listeAnimaux = animalDao.getAll();

   // Afficher le nombre d'animaux et leurs informations
   std::cout << "Nombre d'animaux dans le Zoo : " << listeAnimaux.size() << std::endl;
   for (Animal* animal : listeAnimaux)
   {
      animal->afficherInfos();
   }

   // Déclaration d'un mammifère
   Mammifere lion(0, "Simba", "Lion", 5, 190.5, 1, "Court");
   // animalDao.insert(&lion);
   std::cout << "Type animal: " << lion.getType() << std::endl;

   // Nettoyage de la mémoire
   for (Animal* animal : listeAnimaux) {
      delete animal;
   }
   listeAnimaux.clear();

   // se déconnecter de la base de données et nettoyer la mémoire
   db->deconnecter();
   delete db;

   return 0;
}