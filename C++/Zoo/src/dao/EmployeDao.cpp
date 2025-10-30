#include <vector>
#include <iostream>
#include <mysqlx/xdevapi.h>
#include "../../include/dao/EmployeDao.hpp"
#include "../../include/utils/DatabaseManager.hpp"

// Constructeur
EmployeDao::EmployeDao(DatabaseManager* db) : db(db) {};

// Méthodes
std::vector<Employe> EmployeDao::getAll() {

    std::vector<Employe> result;

    try {
        mysqlx::Session* session = db->getSession();
        mysqlx::SqlResult res = session->sql("SELECT * from employe").execute();

        for (mysqlx::Row row : res) {
            int id = static_cast<int>(row[0]);
            std::string nom = std::string(row[1]);
            std::string poste = std::string(row[2]);
            double salaire = static_cast<double>(row[3]);
            int id_enclos = static_cast<int>(row[4]);

            Employe empl = Employe(id, nom, poste, salaire, id_enclos);
            result.push_back(empl);
        }
    } catch (std::exception& e) {
        std::cerr << "Erreur SQL (EmployeDao) : " << e.what() << std::endl;
    } catch (std::string& e) {
        std::cerr << "Erreur (EmployeDao) : " << e << std::endl;
    }
    
    return result;
    
}