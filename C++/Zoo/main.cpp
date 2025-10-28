#include <iostream>
#include <vector>
#include <memory>
#include "include/utils/DatabaseManager.hpp"
#include "include/dao/AnimalDao.hpp"
#include "include/models/Animal.hpp"


int main(void)
{
   std::cout << "test" << std::endl;

   DatabaseManager *db = new DatabaseManager("localhost", "root", "password", "zoo_db");

   AnimalDao animalDao(db);
   std::vector<Animal> listeAnimaux = animalDao.getAll();

   std::cout << "Nombre d'animaux dans le Zoo : " << listeAnimaux.size() << std::endl;

   for (Animal animal : listeAnimaux)
   {
      animal.afficherInfos();
   }

   return 0;
}