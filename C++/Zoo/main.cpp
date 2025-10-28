#include "include/models/Mammifere.hpp"
#include "include/models/Oiseau.hpp"
#include "include/models/Reptile.hpp"
#include "include/models/Enclos.hpp"
#include "include/models/Employe.hpp"

int main(void)
{
    // Test de la classe Mammifere
    Mammifere lion(1, "Simba", "Panthera leo", 5, 190.5, 101, "dorée");
    
    lion.afficherInfos();
    lion.emettreSon();

    // Test de la classe Oiseau
    Oiseau perroquet(2, "Polly", "Amazona aestiva", 2, 1.5, 102, "30 cm");

    // Test de la classe Reptile
    Reptile serpent(3, "Kaa", "Python regius", 4, 12.3, 103, true);


    // Test de la classe Enclos
    Enclos petitEnclo(1, "petit", 20.5, 2);
    petitEnclo.ajouterAnimal(serpent);
    petitEnclo.ajouterAnimal(perroquet);

    petitEnclo.afficherAnimaux();
    petitEnclo.getNbAnimaux();


    // Test de la classe Employe
    Employe unEmploye(1, "Jhon", "Garde", 1.5, petitEnclo.getID());
    unEmploye.afficherInfos();


    return 0;
}