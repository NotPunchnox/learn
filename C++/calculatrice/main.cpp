#include "iostream"
#include "./utils/console.cpp"

using namespace std;


// Programme de calculatrice assez basique
int main() {

    float x, y, resultat;
    int operateur;
    string operateurStr;

    Print("===== Calculatrice Simple en C++ =====");
    x = InputFloat("Premier nombre: ");
    
    Print("1) +");
    Print("2) -");
    Print("3) /");
    Print("4) *");
    Print("5) %");
    operateur = InputFloat("Opérateur: ");

    y = InputFloat("Second nombre: ");

    switch (operateur) {

        case 1:
            resultat = x + y;
            operateurStr = "+";
            break;
        
        case 2:
            resultat = x - y;
            operateurStr = "-";
            break;
        
        case 3:
            resultat = x / y;
            operateurStr = "/";
            break;
        
        case 4:
            resultat = x * y;
            operateurStr = "*";
            break;

        case 5:
            resultat = (int)x % (int)y;
            operateurStr = "%";
            break;

        default:
            Print("L'opérateur doit être inclus parmis les options suivantes: (1, 2, 3, 4, 5).");
            x = y = 0;
            break;
    }

    Print(
        "\nCalcul: " + to_string((int)x) + " " + operateurStr + " " + to_string((int)y) + " = " + to_string(resultat)
    );


    return 0;
}