#include <vector>
#include <iostream>
#include <mysqlx/xdevapi.h>
#include "../../include/models/Animal.hpp"
#include "../../include/utils/DatabaseManager.hpp"
#include "../../include/dao/AnimalDao.hpp"

AnimalDao::AnimalDao(DatabaseManager* db) : db(db) {};

/*
    === Mini doc SQL ===
    sql(std::string(query)).execute();

    mysqlx::SqlStatement req = session->sql("INSERT INTO exemple (key) VALUES (?)");
    req.bind(value_param, ...).execute();

    === Schéma animal ===
    +--------------+--------------------------------------+------+-----+---------+----------------+
    | Field        | Type                                 | Null | Key | Default | Extra          |
    +--------------+--------------------------------------+------+-----+---------+----------------+
    | id           | int                                  | NO   | PRI | NULL    | auto_increment |
    | nom          | varchar(100)                         | NO   |     | NULL    |                |
    | espece       | varchar(100)                         | NO   |     | NULL    |                |
    | age          | int                                  | NO   |     | NULL    |                |
    | poids        | double                               | NO   |     | NULL    |                |
    | id_enclos    | int                                  | NO   | MUL | NULL    |                |
    | type_animal  | enum('MAMMIFERE','OISEAU','REPTILE') | NO   |     | NULL    |                |
    | typeFourrure | varchar(100)                         | YES  |     | NULL    |                |
    | envergure    | double                               | YES  |     | NULL    |                |
    | estVenimeux  | tinyint(1)                           | YES  |     | NULL    |                |
    +--------------+--------------------------------------+------+-----+---------+----------------+

*/

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

void AnimalDao::insert(Animal* animal) {
    try {
        mysqlx::Session* session = db->getSession();
        // mysqlx::SqlStatement req = session->sql("INSERT INTO animal (id, nom, espece, age, poids, id_enclos, type_animal, typeFourrure, envergure, estVenimeux) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        // mysqlx::SqlResult res = req.bind().execute();

    } catch (const std::exception& e) {
        std::cerr << "Erreur SQL : " << e.what() << std::endl;
    }

}