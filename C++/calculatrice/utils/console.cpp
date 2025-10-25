#include "iostream"

using namespace std;

void Print(string message) {
    cout << message << "\n";
}

string InputString(string message) {

    string result;

    cout << message;
    cin >> result;

    return result;
}

float InputFloat(string message) {
    float result;

    cout << message;

    try {
        cin >> result;
    
    } catch (int e) {
        Print("Error: " + to_string(e));
    }
    return result;
}