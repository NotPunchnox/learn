#include <vector>
#include <iostream>
#include <mysqlx/xdevapi.h>
#include "../../include/models/Animal.hpp"
#include "../../include/models/Mammifere.hpp"
#include "../../include/models/Oiseau.hpp"
#include "../../include/models/Reptile.hpp"
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

std::vector<Animal*> AnimalDao::getAll() {
    std::vector<Animal*> result;

    try {
        mysqlx::Session* session = db->getSession();
        mysqlx::SqlResult res = session->sql("SELECT * from animal").execute();

        for (mysqlx::Row row : res) {
            int id = static_cast<int>(row[0]);
            std::string nom = std::string(row[1]);
            std::string espece = std::string(row[2]);
            int age = static_cast<int>(row[3]);
            double poids = static_cast<double>(row[4]);
            int id_enclos = static_cast<int>(row[5]);
            std::string type = std::string(row[6]);

            Animal* animal = nullptr;

            if (type == "MAMMIFERE") {
                std::string typeFourrure = std::string(row[7]);
                animal = new Mammifere(id, nom, espece, age, poids, id_enclos, typeFourrure);
            } else if (type == "OISEAU") {
                std::string envergure = std::string(row[8]);
                animal = new Oiseau(id, nom, espece, age, poids, id_enclos, envergure);
            } else if (type == "REPTILE") {
                bool estVenimeux = static_cast<bool>(row[9]);
                animal = new Reptile(id, nom, espece, age, poids, id_enclos, estVenimeux);
            } else {
                throw std::string("Le type : " + type + " n'existe pas !");
            }

            if (animal != nullptr) {
                result.push_back(animal);
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur SQL : " << e.what() << std::endl;
    }
    catch (const std::string& e) {
        std::cerr << "Erreur : " << e << std::endl;
    }

    return result;
}

void AnimalDao::insert(Animal* animal) {
    try {

        std::string type = animal->getType();
        mysqlx::Session* session = db->getSession();
        std::string query = "INSERT INTO animal (id, nom, espece, age, poids, id_enclos, type_animal, typeFourrure, envergure, estVenimeux) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        mysqlx::SqlStatement req = session->sql(query);

        if (type == "MAMMIFERE") {
            Mammifere* mamm = static_cast<Mammifere*>(animal);
            req.bind(animal->getId(), animal->getNom(), animal->getEspece(), animal->getAge(), animal->getPoids(), animal->getIdEnclos(), animal->getType(), mamm->getTypeFourrure(), nullptr, nullptr);
        } else if (type == "OISEAU") {
            Oiseau* ois = static_cast<Oiseau*>(animal);
            req.bind(animal->getId(), animal->getNom(), animal->getEspece(), animal->getAge(), animal->getPoids(), animal->getIdEnclos(), animal->getType(), nullptr, ois->getEnvergure(), nullptr);
        } else if (type == "REPTILE") {
            Reptile* rept = static_cast<Reptile*>(animal);
            req.bind(animal->getId(), animal->getNom(), animal->getEspece(), animal->getAge(), animal->getPoids(), animal->getIdEnclos(), animal->getType(), nullptr, nullptr, rept->getIsVenomous());
        } else {
            req.bind(animal->getId(), animal->getNom(), animal->getEspece(), animal->getAge(), animal->getPoids(), animal->getIdEnclos(), nullptr, nullptr, nullptr, nullptr);
        }

        mysqlx::SqlResult res = req.execute();

    } catch (const std::exception& e) {
        std::cerr << "Erreur SQL : " << e.what() << std::endl;
    }
}

void AnimalDao::remove(Animal* animal) {
    try {

        mysqlx::Session* session = db->getSession();

        mysqlx::SqlStatement req = session->sql("DELETE FROM animal WHERE id = ?").bind(animal->getId());

        mysqlx::SqlResult res = req.execute();

    } catch (const std::exception& e) {
        std::cerr << "Erreur SQL : " << e.what() << std::endl;
    }
}