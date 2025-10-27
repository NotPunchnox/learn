#include <string>
#include <vector>
#include "iostream"
#include "../../include/models/Enclos.hpp"

// Constructeur
Enclos::Enclos() : id(0), type(""), taille(0.0), capacite(0), animaux() {};
Enclos::Enclos(int id, const std::string &type, double taille, int capacite)
    : id(id), type(type), taille(taille), capacite(capacite), animaux() {};

// Getters
int Enclos::getID() const {
    return id;
}
const std::string& Enclos::getType() const {
    return type;
}
double Enclos::getTaille() const {
    return taille;
}
int Enclos::getCapacite() const {
    return capacite;
}
const std::vector<Animal>& Enclos::getAnimaux() const {
    return animaux;
}


// Setters
void Enclos::setID(int id) {
    this->id = id;
}
void Enclos::setType(const std::string &type) {
    this->type = type;
}
void Enclos::setTaille(double taille) {
    this->taille = taille;
}
void Enclos::setCapacite(int capacite) {
    this->capacite = capacite;
}
void Enclos::setAnimaux(const std::vector<Animal> animaux) {
    this->animaux = animaux;
}


// Méthodes
void Enclos::ajouterAnimal(const Animal animal) {
    animaux.push_back(animal);
}
void Enclos::afficherAnimaux() const {
    std::cout << "Liste des Animaux présent dans l'Enclos : " << getID() << "\n"
              << "=====================================================" << std::endl;

    for (Animal a : animaux) {
        std::cout << "Nom: " << a.getNom() << "\n" << std::endl;
    }
}
int Enclos::getNbAnimaux() const {
    return animaux.size();
}