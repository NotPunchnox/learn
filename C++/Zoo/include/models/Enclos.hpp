#include <string>
#include <vector>
#include "Animal.hpp"

class Enclos {

public:
    // Constructeur
    Enclos();
    Enclos(int id, const std::string &type, double taille, int capacite);

    // Destructeur
    ~Enclos();

    // Getters
    int getID() const;
    const std::string& getType() const;
    double getTaille() const;
    int getCapacite() const;
    const std::vector<Animal*>& getAnimaux() const;

    // Setters
    void setID(int id);
    void setType(const std::string &type);
    void setTaille(double taille);
    void setCapacite(int capacite);

    // Méthodes
    void ajouterAnimal(Animal* animal);
    void afficherAnimaux();
    int getNbAnimaux() const;
    void nettoyerAnimaux();

private:
    int id;
    std::string type;
    double taille;
    int capacite;
    std::vector<Animal*> animaux;
};
