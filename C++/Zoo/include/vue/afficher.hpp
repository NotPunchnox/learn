#include <iostream>
#include "../utils/DatabaseManager.hpp"

class Afficher {

public:
  // Constructeur
  Afficher(DatabaseManager *db);


  // Destructeur
  virtual ~Afficher() = default;


  // Getters
  const std::string getColor() const;

  // Setters
  void setColor(const std::string color);

  // Méthodes
  void menu();


private:
  std::string color;
  DatabaseManager *db;
};
