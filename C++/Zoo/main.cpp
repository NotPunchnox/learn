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

   AnimalDao animalDao(db);
   std::vector<Animal*> listeAnimaux = animalDao.getAll();

   std::cout << "Nombre d'animaux dans le Zoo : " << listeAnimaux.size() << std::endl;

   for (Animal* animal : listeAnimaux)
   {
      animal->afficherInfos();
   }

   // Déclaration d'un mammifère
   Mammifere lion(0, "Simba", "Lion", 5, 190.5, 1, "Court");
   animalDao.insert(&lion);
   std::cout << "Type animal: " << lion.getType() << std::endl;

   // Nettoyage de la mémoire
   for (Animal* animal : listeAnimaux) {
      delete animal;
   }
   listeAnimaux.clear();

   db->deconnecter();
   delete db;
   return 0;
}