#include <vector>
#include "../models/Employe.hpp"
#include "../utils/DatabaseManager.hpp"

class EmployeDao {
    
public:
    // Constructeur
    EmployeDao(DatabaseManager*);

    // Méthodes
    std::vector<Employe> getAll();
    void insert(Employe&);

private:
    DatabaseManager* db;
};