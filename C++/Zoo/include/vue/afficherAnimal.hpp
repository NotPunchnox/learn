#include <iostream>
#include "../utils/DatabaseManager.hpp"


class afficherAnimal {

public:

  // Constructeur
  afficherAnimal(DatabaseManager *db);
  afficherAnimal(DatabaseManager *db, std::string color);


  // Getters
  std::string getColor() const;

  // Setters
  void setColor(const std::string& color);
// Méthodes
  void menu();

private:
  std::string color;
  DatabaseManager *db;
};
