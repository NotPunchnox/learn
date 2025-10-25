L'objectif de cette mini application est de simuler un Zoo ( oui c'est assez abstrait dit comme ça )
Il sert juste à me remettre dans la programmation orienté Objet en cpp









Diagramme UML:
```uml
Simulation de Zoo

'=======================
' === CLASSES MÉTIER ===
'=======================

class Animal {
  - int id
  - string nom
  - string espece
  - int age
  - double poids
  - int id_enclos

  + Animal()
  + Animal(int, string, string, int, double, int)
  + virtual void afficherInfos() const
  + virtual void nourrir()
  + virtual void emettreSon() const = 0
}

class Mammifere extends Animal {
  - string typeFourrure
  + void emettreSon() const
}

class Oiseau extends Animal {
  - double envergure
  + void emettreSon() const
}

class Reptile extends Animal {
  - bool estVenimeux
  + void emettreSon() const
}

class Enclos {
  - int id
  - string type
  - double taille
  - int capacite
  - vector<Animal*> animaux

  + Enclos()
  + Enclos(int, string, double, int)
  + void ajouterAnimal(Animal*)
  + void afficherAnimaux() const
  + int getNbAnimaux() const
}

class Employe {
  - int id
  - string nom
  - string poste
  - double salaire
  - int id_enclos

  + Employe()
  + Employe(int, string, string, double, int)
  + void afficherInfos() const
}

class Zoo {
  - vector<Enclos> enclosList
  - vector<Employe> employeList

  + void chargerDepuisBD()
  + void sauvegarderDansBD()
  + void simulerJournee()
  + void afficherZoo() const
}

'=============================
' === CLASSES D’ACCÈS BD ====
'=============================

class DatabaseManager {
  - sql::Connection* connexion
  + DatabaseManager(string host, string user, string password, string db)
  + bool connecter()
  + sql::Connection* getConnexion()
}

class AnimalDAO {
  - DatabaseManager* db
  + AnimalDAO(DatabaseManager*)
  + vector<Animal*> getAll()
  + void insert(Animal*)
  + void update(Animal*)
  + void remove(int id)
}

class EnclosDAO {
  - DatabaseManager* db
  + EnclosDAO(DatabaseManager*)
  + vector<Enclos> getAll()
  + void insert(Enclos&)
}

class EmployeDAO {
  - DatabaseManager* db
  + EmployeDAO(DatabaseManager*)
  + vector<Employe> getAll()
  + void insert(Employe&)
}

'=========================
' === RELATIONS ENTRE CLASSES ===
'=========================

Animal "0..*" --> "1" Enclos : appartient à >
Enclos "1" --> "0..*" Animal : contient >
Employe "1" --> "1" Enclos : s'occupe de >
Zoo "1" --> "0..*" Enclos
Zoo "1" --> "0..*" Employe
Zoo "1" --> "0..*" Animal

DatabaseManager <.. AnimalDAO
DatabaseManager <.. EnclosDAO
DatabaseManager <.. EmployeDAO
```