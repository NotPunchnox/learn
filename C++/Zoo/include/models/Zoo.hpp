#include <vector>
#include "./Employe.hpp"
#include "./Enclos.hpp"

class Zoo {

public:

    // Constructeur
    Zoo();
    Zoo(const std::vector<Enclos> enclosList, const std::vector<Employe> employeList);

    // Destructeur
    virtual ~Zoo() = default;

    // Getters
    const std::vector<Enclos> getEnclos() const;
    const std::vector<Employe> getEmploye() const;

    // Setters
    void setEnclos(const std::vector<Enclos> enclosList);
    void setEmploye(const std::vector<Employe> employeList);


    // Méthodes
    void chargerDepuisDB();
    void sauvegarderDansDB();
    void simulerJournee();
    void afficherZoo() const;

private:
    std::vector<Enclos> enclosList;
    std::vector<Employe> employeList;

};