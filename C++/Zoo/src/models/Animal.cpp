#include <string>
#include <iostream>

#include "../../include/models/Animal.hpp"

// Constructeur
Animal::Animal(int id, const std::string &nom, const std::string& espece, int age, double poids, int id_enclos)
    : id(id), nom(nom), espece(espece), age(age), poids(poids), id_enclos(id_enclos) {};

// Getters
int Animal::getId() const {
    return id;
}
const std::string& Animal::getNom() const {
    return nom;
}
const std::string& Animal::getEspece() const {
    return espece;
}
int Animal::getAge() const {
    return age;
}
int Animal::getIdEnclos() const {
    return id_enclos;
}

// Setters
void Animal::setId(int id) {
    this->id = id;
}
void Animal::setNom(const std::string& nom) {
    this->nom = nom;
}
void Animal::setEspece(const std::string& espece) {
    this->espece = espece;
}
void Animal::setAge(int age) {
    this->age = age;
}
void Animal::setPoids(double poids) {
    this->poids = poids;
}
void Animal::setIdEnclos(int id_enclos) {
    this->id_enclos = id_enclos;
}


// Méthodes virtuelles
void Animal::afficherInfos() const {
    std::cout << "ID: "      << id        << "\n"
              << "Nom: "     << nom       << "\n"
              << "Espèce: "  << espece    << "\n"
              << "Age: "     << age       << "\n"
              << "Poids: "   << poids     << "\n"
              << "ID Enclos: "<< id_enclos << "\n"
              << std::endl;
}

void Animal::nourrir() const {
    std::cout << nom << " est nourri." << std::endl;
}

void Animal::emettreSon() const {
    std::cout << nom << " fait un son." << std::endl;
}