#include <iostream>
#include "../utils/DatabaseManager.hpp"

using namespace std;

class afficherEnclos {

public:

  // Constructeur
  afficherEnclos(DatabaseManager *db);
  afficherEnclos(DatabaseManager *db, string color);

  // Getters
  string getColor() const;

  // Setters
  void setColor(const string& color);

  // Méthodes
  void menu();


private:
  string color;
  DatabaseManager *db;

}
