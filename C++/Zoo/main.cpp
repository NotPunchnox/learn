#include <iostream>
#include <vector>
#include <memory>
#include "include/utils/DatabaseManager.hpp"
#include "include/dao/AnimalDao.hpp"
#include "include/models/Animal.hpp"

int main(void)
{
   std::cout << "Démarrage du programme" << std::endl;

   DatabaseManager *db = new DatabaseManager("localhost", "root", "password", "zoo_db");

   // Tester la connexion avant d'utiliser la db
   std::cout << "Tentative de connexion à la base de données..." << std::endl;
   if (!db->connecter())
   {
      std::cerr << "Erreur: Impossible de se connecter à la base de données!" << std::endl;
      std::cerr << "Vérifiez que:" << std::endl;
      std::cerr << "  - MySQL est en cours d'exécution" << std::endl;
      std::cerr << "  - La base 'zoo_db' existe" << std::endl;
      std::cerr << "  - Les identifiants sont corrects" << std::endl;
      delete db;
      return 1;
   }

   // Si la tentative de connexion à fonctionné alors utiliser la db
   AnimalDao animalDao(db);
   std::vector<Animal> listeAnimaux = animalDao.getAll();

   std::cout << "Nombre d'animaux dans le Zoo : " << listeAnimaux.size() << std::endl;

   for (Animal animal : listeAnimaux)
   {
      animal.afficherInfos();
   }

   // Se déconnecter et supprimer le pointeur
   db->deconnecter();
   delete db;
   return 0;
}