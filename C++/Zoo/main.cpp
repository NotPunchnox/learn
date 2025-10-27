#include "include/models/Mammifere.hpp"
#include "include/models/Oiseau.hpp"
#include "include/models/Reptile.hpp"


int main(void)
{
    // Test de la classe Mammifere
    Mammifere lion(1, "Simba", "Panthera leo", 5, 190.5, 101, "dorée");
    
    lion.afficherInfos();
    lion.emettreSon();


    // Test de la classe Oiseau
    Oiseau perroquet(2, "Polly", "Amazona aestiva", 2, 1.5, 102, "30 cm");
    perroquet.afficherInfos();
    perroquet.emettreSon();

    // Test de la classe Reptile
    Reptile serpent(3, "Kaa", "Python regius", 4, 12.3, 103, true);
    serpent.afficherInfos();
    serpent.emettreSon();


    return 0;
}