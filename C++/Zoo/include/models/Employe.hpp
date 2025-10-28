#include <string>

class Employe {

public:
    // Constructeur
    Employe();
    Employe(int id, const std::string &nom, const std::string &poste, double salaire, int id_enclos);

    // Getters
    int getID() const;
    const std::string &getNom() const;
    const std::string &getPoste() const;
    double getSalaire() const;
    int getIDEnclos() const;

    // Setters
    void setID(int id);
    void setNom(const std::string &nom);
    void setPoste(const std::string &poste);
    void setSalaire(double salaire);
    void setIDEnclos(int id_enclos);

    // Méthodes
    void afficherInfos() const;


private:
    int id;
    std::string nom;
    std::string poste;
    double salaire;
    int id_enclos;

};