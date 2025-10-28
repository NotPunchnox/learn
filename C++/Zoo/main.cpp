#include <iostream>
#include "include/dao/AnimalDao.hpp"


int main(void)
{

   DatabaseManager *db = new DatabaseManager("localhost", "root", "password", "zoo_db");

   AnimalDao animalDao(db);
   std::vector<Animal> listeAnimaux = animalDao.getAll();

   std::cout << "Nombre d'animaux dans le Zoo : " << listeAnimaux.size() << std::endl;

   for (Animal animal : listeAnimaux)
   {
      animal.afficherInfos();
   }
   
}