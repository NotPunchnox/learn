#include <iostream>


class afficherAnimal {

public:

  // Constructeur
  afficherAnimal();
  afficherAnimal(std::string color);


  // Getters
  std::string getColor() const;

  // Setters
  void setColor(const std::string& color);
// Méthodes
  void menu() const;

private:
  std::string color;
};
