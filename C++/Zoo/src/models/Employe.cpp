#include <string>
#include <iostream>
#include "../../include/models/Employe.hpp"

// Constructeur
Employe::Employe() : id(0), nom(""), poste(""), salaire(0.0), id_enclos(0) {};
Employe::Employe(int id, const std::string &nom, const std::string &poste, double salaire, int id_enclos)
    : id(id), nom(nom), poste(poste), salaire(salaire), id_enclos(id_enclos) {};


// Getters
int Employe::getID() const {
    return id;
}
const std::string& Employe::getNom() const {
    return nom;
}
const std::string& Employe::getPoste() const {
    return poste;
}
double Employe::getSalaire() const {
    return salaire;
}
int Employe::getIDEnclos() const {
    return id_enclos;
}

// Setters
void Employe::setID(int id) {
    this->id = id;
}
void Employe::setNom(const std::string &nom) {
    this->nom = nom;
}
void Employe::setPoste(const std::string &poste) {
    this->poste = poste;
}
void Employe::setSalaire(double salaire) {
    this->salaire = salaire;
}
void Employe::setIDEnclos(int id_enclos) {
    this->id_enclos = id_enclos;
}

// méthodes
void Employe::afficherInfos() const {
    std::cout << "============= Informations Employé =============\n" 
              << "ID: "        << getID()       << "\n"
              << "Nom: "       << getNom()      << "\n"
              << "Poste: "     << getPoste()    << "\n"
              << "Salaire: "   << getSalaire()  << "\n"
              << "ID Enclos: " << getIDEnclos() << std::endl;
}