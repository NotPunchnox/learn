#include "include/models/Mammifere.hpp"

int main(void)
{
    Reptile lion(1, "Simba", "Panthera leo", 5, 190.5, 101, "dorée");
    
    lion.afficherInfos();
    lion.emettreSon();
}