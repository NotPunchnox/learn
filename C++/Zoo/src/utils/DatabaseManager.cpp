#include <string>
#include <iostream>
#include <mysqlx/xdevapi.h>
#include "../../include/utils/DatabaseManager.hpp"

// Constructeur
DatabaseManager::DatabaseManager() : host(""), user(""), password(""), database(""), port(33060) {};
DatabaseManager::DatabaseManager(const std::string &host, const std::string &user, const std::string &password, const std::string &database, int port)
    : host(host), user(user), password(password), database(database), port(port) {};

// Destructeur
DatabaseManager::~DatabaseManager() {
    deconnecter();
}

// Méthodes
bool DatabaseManager::connecter() {
    try {
        // Création d’une session MySQL
        session = std::make_unique<mysqlx::Session>(host, port, user, password);
        session->sql("USE " + database).execute();
        std::cout << "✅ Connexion MySQL réussie !" << std::endl;
        return true;
    } catch (const mysqlx::Error &err) {
        std::cerr << "❌ Erreur MySQL : " << err.what() << std::endl;
        return false;
    } catch (std::exception &ex) {
        std::cerr << "❌ Exception : " << ex.what() << std::endl;
        return false;
    }
}

void DatabaseManager::deconnecter() {
    if (session) {
        session->close();
        std::cout << "🔒 Connexion MySQL fermée." << std::endl;
    }
}

mysqlx::Session* DatabaseManager::getSession() {
    return session.get();
}