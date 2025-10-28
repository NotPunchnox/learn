#include <iostream>
#include <mysqlx/xdevapi.h>

using ::std::cout;
using ::std::endl;
using namespace::mysqlx;


class DatabaseManager {

public:
    // Constructeur
    DatabaseManager(const std::string &host, const std::string &user, const std::string &password, const std::string &database, int port = 33060);

    // Destructeur
    virtual ~DatabaseManager() = default;

    // Méthodes
    bool connecter();
    void deconnercter();

    mysqlx::Session* getSession();


private:
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    int port;
    std::unique_ptr<mysqlx::Session> session;

};