#pragma once
#include <string>

// Déclaration de la classe Animal
class Animal {
public:
    // Constructeur
    Animal(int id, const std::string& nom, const std::string& espece, int age, double poids, int id_enclos);

    // Destructeur virtuel pour permettre l'héritage
    virtual ~Animal() = default;

    // Getters
    int getId() const;
    const std::string& getNom() const;
    const std::string& getEspece() const;
    int getAge() const;
    double getPoids() const;
    int getIdEnclos() const;

    // Setters
    void setId(int id);
    void setNom(const std::string& nom);
    void setEspece(const std::string& espece);
    void setAge(int age);
    void setPoids(double poids);
    void setIdEnclos(int id_enclos);

    // Méthodes virtuelles
    virtual void afficherInfos() const;
    virtual void nourrir() const;
    virtual void emettreSon() const;
    virtual std::string getType() const = 0;

protected:
    int id{0};
    std::string nom;
    std::string espece;
    int age{0};
    double poids{0.0};
    int id_enclos{0};
};
