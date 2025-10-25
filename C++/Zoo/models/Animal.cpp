#include <string>
#include <iostream>

class Animal {
    public:
        int id;
        std::string nom;
        std::string espece;
        int age;
        double poids;
        int id_enclos;

        // Constructeur
        Animal(int id, const std::string& nom, const std::string& espece, int age, double poids, int id_enclos)
            : id(id), nom(nom), espece(espece), age(age), poids(poids), id_enclos(id_enclos) {};
        
        
        // Getters/Setters
        int getId() const {
            return id;
        }
        std::string getNom() const {
            return nom;
        }
        std::string getEspece() const {
            return espece;
        }
        int getAge() const {
            return age;
        }
        double getPoids() const {
            return poids;
        }
        int getIdEnclos() const {
            return id_enclos;
        }
        void setId(int id) {
            this->id = id;
        }

        void setNom(const std::string& nom) {
            this->nom = nom;
        }

        void setEspece(const std::string& espace) {
            this->espece = espace;
        }

        void setAge(int age) {
            this->age = age;
        }

        void setPoids(double poids) {
            this->poids = poids;
        }

        void setIdEnclos(int id_enclos) {
            this->id_enclos = id_enclos;
        }

        // Méthodes virtuelles
        virtual void afficherInfos() const {
            // Affichage des informations de l'animal
            std::cout << "ID: "      << id        << "\n"
                      << "Nom: "     << nom       << "\n"
                      << "Espèce: "  << espece    << "\n"
                      << "Age: "     << age       << "\n"
                      << "Poids: "   << poids     << "\n"
                      << "ID Enclos" << id_enclos << "\n";
        }

        virtual void nourrir() const {
            // Comportement de nourrissage par défaut
            printf("%s est nourri.\n", nom.c_str());
        }

        virtual void emettreSon() const {
            // Comportement sonore par défaut
            printf("%s fait un son.\n", nom.c_str());
        }

};