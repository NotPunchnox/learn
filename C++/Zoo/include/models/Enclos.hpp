#include <string>
#include <vector>
#include "Animal.hpp"

class Enclos {

public:
    // Constructeur
    Enclos();
    Enclos(int id, std::string &type, double taille, int capacite);

    // Destructeur
    virtual ~Enclos() = default;

    // Getters
    int getID() const;
    const std::string& getType() const;
    double getTaille() const;
    int getCapacite() const;
    const std::vector<Animal>& getAnimaux() const;

    // Setters
    void setID(int id);
    void setType(const std::string& type);
    void setTaille(double taille);
    void setCapacite(int capacite);
    void setAnimaux(const std::vector<Animal>);

    // Méthodes
    void ajouterAnimal(const Animal animal);
    void afficherAnimaux() const;
    int getNbAnimaux() const;

private:
    int id;
    std::string type;
    double taille;
    int capacite;
    std::vector<Animal> animaux;
};