#include <vector>
#include "../models/Animal.hpp"
#include "../utils/DatabaseManager.hpp"

class AnimalDao {

public:
    // Constructeur
    AnimalDao(DatabaseManager* db);

    // Méthodes CRUD (create, read, update, delete)
    std::vector<Animal> getAll();
    void insert(Animal*);
    void update(Animal*);
    void remove(Animal*);

private:
    DatabaseManager* db;
};