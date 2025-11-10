#include <vector>
#include <iostream>
#include <mysqlx/xdevapi.h>
#include "../../include/dao/EnclosDao.hpp"
#include "../../include/utils/DatabaseManager.hpp"
#include "../../include/dao/EnclosDao.hpp"

// Constructeur
EnclosDao::EnclosDao(DatabaseManager* db) : db(db) {};

// Méthodes
std::vector<Enclos> EnclosDao::getAll() {
  
  std::vector<Enclos> listeEnclos;

  try {
    
    mysqlx::Session* session = db->getSession();
    mysqlx::SqlResult res = session->sql("SELECT * FROM enclos").execute();

    for (mysqlx::Row row : res) {
      int id = static_cast<int>(row[0]);
      std::string type = std::string(row[1]);
      int taille = static_cast<int>(row[2]);
      int capacite = static_cast<int>(row[3]);

      Enclos encl = Enclos(id, type, taille, capacite);
      listeEnclos.push_back(encl);
    }

  } catch (std::exception& e) {
    std::cerr << "Erreur SQL (EnclosDao) : " << e.what() << std::endl;
  } catch (std::string& e) {
    std::cerr << "Erreur (EnclosDao) : " << e << std::endl;
  }

  return listeEnclos;

}
