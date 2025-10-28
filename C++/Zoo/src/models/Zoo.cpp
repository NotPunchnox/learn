#include <vector>
#include "../../include/utils/DatabaseManager.hpp"
#include "../../include/models/Zoo.hpp"

// Constructeur
Zoo::Zoo() : enclosList(), employeList() {};
Zoo::Zoo(const std::vector<Enclos> enclosList, const std::vector<Employe> employeList)
    : enclosList(enclosList), employeList(employeList) {};


// Getters
const std::vector<Enclos> Zoo::getEnclos() const {
    return enclosList;
}
const std::vector<Employe> Zoo::getEmploye() const {
    return employeList;
}

// Setters
void Zoo::setEnclos(const std::vector<Enclos> enclos) {
    this->enclosList = enclos;
}
void Zoo::setEmploye(const std::vector<Employe> empl) {
    this->employeList = empl;
}

// Méthodes