#include <iostream>
#include <memory>
#include <vector>
#include "include/dao/AnimalDao.hpp"
#include "include/utils/DatabaseManager.hpp"

int main(void)
{

   std::cout << "Bienvenue dans le gestionnaire de Zoo !" << std::endl;

   DatabaseManager *db = new DatabaseManager("localhost", "root", "password", "zoo_db");

   // Connexion à la base de données
   db->connecter();

   AnimalDao animalDao(db);
   std::vector<Animal> listeAnimaux = animalDao.getAll();

   std::cout << "Nombre d'animaux dans le Zoo : " << listeAnimaux.size() << std::endl;

   for (Animal animal : listeAnimaux)
   {
      animal.afficherInfos();
   }

   db->deconnecter();
   delete db;
   return 0;
}