#include <iostream>
#include <memory>
#include <vector>
#include "include/dao/AnimalDao.hpp"
#include "include/models/Mammifere.hpp"
#include "include/utils/DatabaseManager.hpp"
#include "include/dao/EmployeDao.hpp"
#include "include/vue/afficher.hpp"

using namespace std;

int main(void)
{
   cout << "Bienvenue dans le gestionnaire de Zoo !" << endl;
   DatabaseManager *db = new DatabaseManager("localhost", "admin", "password", "zoo_db", 3306);

   // Connexion à la base de données

  if (!db->connecter()) {
      cerr << "Échec de la connexion à la base de données." << endl;
      delete db;
      return 1;
  }


   Afficher *afficher = new Afficher(db);
   afficher->menu();

  /*

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



   EmployeDao employeDao(db);
   std::vector<Employe> listeEmployes = employeDao.getAll();
   
   // boucle pour afficher l'intégralité des employés présent dans la db
   for (Employe empl : listeEmployes) {
      empl.afficherInfos();
   }
  */
   // Création d'un nouvel employé pour tester: fonctionnel
   // Employe nouvelEmpl = Employe(0, "Samuel", "développeur", 2200, 1);
   // employeDao.insert(nouvelEmpl);

   // se déconnecter de la base de données et nettoyer la mémoire
   db->deconnecter();
   delete db;

   return 0;
}
