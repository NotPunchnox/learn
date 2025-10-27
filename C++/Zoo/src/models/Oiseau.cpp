#include <string>
#include <iostream>
#include "../../include/models/Oiseau.hpp"

Oiseau::Oiseau(int id, const std::string &nom, const std::string &espece, int age, double poids, int id_enclos, const std::string &envergure)
    : Animal(id, nom, espece, age, poids, id_enclos), envergure(envergure) {};


const std::string& Oiseau::getEnvergure() const {
    return envergure;
}

void Oiseau::setEnvergure(const std::string &envergure) {
    this->envergure = envergure;
}

void Oiseau::emettreSon() const {
    std::cout << "L'oiseau " << getNom() << "chante" << std::endl;
}