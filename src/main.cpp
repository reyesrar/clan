#include <iostream>
#include <fstream>
#include <sstream>
#include "binaryTree/binaryTree.h"
#include "models/ClanMember.h"
using namespace std;

void loadClanData(Tree<ClanMember>& tree, const string& filePath);

int main() {
    Tree<ClanMember> clanTree(nullptr);
    loadClanData(clanTree, "bin/data.csv");

    int choice;
    do {
        cout << "\n--- Menu ---\n";
        cout << "1. Mostrar linea de sucesion\n";
        cout << "2. Modificar datos de un miembro\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int order;
                cout << "Seleccionar recorrido (0: PREORDER, 1: INORDER, 2: POSTORDER): ";
                cin >> order;
                cout << "Linea de Sucesion Actual:" << endl;
                clanTree.printSuccessionLine(order);
                break;
            }
            case 2:
                clanTree.modifyClanMember("bin/data.csv");
                clanTree.saveTreeToFile("bin/data.csv");
                break;
            case 0:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
    } while (choice != 0);

    return 0;
}

void loadClanData(Tree<ClanMember>& tree, const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error abriendo archivo: " << filePath << endl;
        return;
    }

    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, lastName, gender, age, idFather, isDead, wasChief, isChief;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, lastName, ',');
        getline(ss, gender, ',');
        getline(ss, age, ',');
        getline(ss, idFather, ',');
        getline(ss, isDead, ',');
        getline(ss, wasChief, ',');
        getline(ss, isChief, ',');

        ClanMember member(
            stoi(id), 
            name, 
            lastName, 
            gender[0], 
            stoi(age), 
            stoi(idFather), 
            stoi(isDead), 
            stoi(wasChief), 
            stoi(isChief)
        );

        tree.insert(member);
    }

    file.close();
}