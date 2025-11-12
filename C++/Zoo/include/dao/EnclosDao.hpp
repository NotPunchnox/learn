#include <vector>
#include "../models/Enclos.hpp"
#include "../utils/DatabaseManager.hpp"

class EnclosDao {

public:
    // Constructeur
    EnclosDao(DatabaseManager*);

    // Méthodes
    std::vector<Enclos> getAll();
    void insert(Enclos&);
    void afficherEnclos();
private:
    DatabaseManager* db;
};
