#include <iostream>

class Afficher {

public:
  // Constructeur
  Afficher();
  Afficher(const std::string color);

  // Destructeur
  virtual ~Afficher() = default;


  // Getters
  const std::string getColor() const;

  // Setters
  void setColor(const std::string color);

  // Méthodes
  void menu() const;

private:
  std::string color;
}
