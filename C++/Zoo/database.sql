-- =====================================================
--  SCRIPT DE CRÉATION DE LA BASE DE DONNÉES : ZOO_DB
-- =====================================================

-- Création de la base
DROP DATABASE IF EXISTS zoo_db;
CREATE DATABASE zoo_db CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
USE zoo_db;

-- TABLE : ENCLOS
CREATE TABLE Enclos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    type VARCHAR(100) NOT NULL,
    taille DOUBLE NOT NULL,
    capacite INT NOT NULL
);

-- TABLE : EMPLOYE
CREATE TABLE Employe (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(100) NOT NULL,
    poste VARCHAR(100) NOT NULL,
    salaire DOUBLE NOT NULL,
    id_enclos INT,
    CONSTRAINT fk_employe_enclos FOREIGN KEY (id_enclos)
        REFERENCES Enclos(id)
        ON DELETE SET NULL
        ON UPDATE CASCADE
);

-- TABLE : ANIMAL
CREATE TABLE Animal (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(100) NOT NULL,
    espece VARCHAR(100) NOT NULL,
    age INT NOT NULL,
    poids DOUBLE NOT NULL,
    id_enclos INT NOT NULL,
    type_animal ENUM('MAMMIFERE', 'OISEAU', 'REPTILE') NOT NULL,

    typeFourrure VARCHAR(100) DEFAULT NULL,
    envergure VARCHAR(100) DEFAULT NULL,
    estVenimeux BOOLEAN DEFAULT NULL,

    CONSTRAINT fk_animal_enclos FOREIGN KEY (id_enclos)
        REFERENCES Enclos(id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);



-- DONNÉES DE TEST

-- Enclos
INSERT INTO Enclos (type, taille, capacite)
VALUES
('Savane', 500.0, 10),
('Volière', 200.0, 20),
('Terrarium', 150.0, 15);

-- Employés
INSERT INTO Employe (nom, poste, salaire, id_enclos)
VALUES
('Alice Dupont', 'Soigneuse Mammifères', 2200.00, 1),
('Marc Lefevre', 'Soigneur Oiseaux', 2100.00, 2),
('Sophie Durant', 'Soigneuse Reptiles', 2000.00, 3);

-- Animaux
INSERT INTO Animal (nom, espece, age, poids, id_enclos, type_animal, typeFourrure)
VALUES
('Simba', 'Lion', 5, 190.5, 1, 'MAMMIFERE', 'Courte');

INSERT INTO Animal (nom, espece, age, poids, id_enclos, type_animal, envergure)
VALUES
('Coco', 'Perroquet', 2, 1.1, 2, 'OISEAU', "45 cm");

INSERT INTO Animal (nom, espece, age, poids, id_enclos, type_animal, estVenimeux)
VALUES
('Kaa', 'Serpent', 4, 12.3, 3, 'REPTILE', TRUE);