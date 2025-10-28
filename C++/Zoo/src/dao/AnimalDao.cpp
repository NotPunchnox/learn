#include <vector>
#include <iostream>
#include <mysqlx/xdevapi.h>
#include "../../include/models/Animal.hpp"
#include "../../include/utils/DatabaseManager.hpp"
#include "../../include/dao/AnimalDao.hpp"

AnimalDao::AnimalDao(DatabaseManager* db) : db(db) {};

std::vector<Animal> AnimalDao::getAll() {
    std::vector<Animal> result;

    try {
        mysqlx::Session* session = db->getSession();
        mysqlx::SqlResult res = session->sql("SELECT * from animal").execute();

        for (mysqlx::Row row : res) {
            
            Animal animal(
                static_cast<int>(row[0]),
                std::string(row[1]),
                std::string(row[2]),
                static_cast<int>(row[3]),
                static_cast<double>(row[4]),
                static_cast<int>(row[5])
            );
            result.push_back(animal);

            // std::string type;
            // type = std::string(row[6]);
            // if (type == "MAMMIFERE") {
            // } else if (type == "OISEAU") {
            // } else if (type == "REPTILE") {
            // } else {
            //     throw "Le type : " + std::string(row[6]) + " n'existe pas !";
            // }

        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur SQL : " << e.what() << std::endl;
    }

    return result;
}