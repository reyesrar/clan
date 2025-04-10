#include "node.h"
#include "../models/ClanMember.h"
#include <fstream>
#include <sstream>
enum Order{PREORDER, INORDER, POSTORDER};

template<class T>
class Tree{
    private:
        Node<T>* root = nullptr;
        Node<T>* insert(Node<T>*, T);
        Node<T>* remove(Node<T>*, T);
        Node<T>* leftmost(Node<T>*);
        bool searchPreOrder(Node<T>*, int);
        bool searchInOrder(Node<T>*, int);
        bool searchPostOrder(Node<T>*, int);
        void successionLinePreOrder(Node<T>*);
        void successionLineInOrder(Node<T>*);
        void successionLinePostOrder(Node<T>*);
        T findMember(Node<T>*);
        T findMember(Node<T>*, int);
        void updateFile(const string&, const T&);
        void saveNode(Node<T>*, ofstream&);
        int height(Node<T>*);
        int getBalanceFactor(Node<T>*);
        Node<T>* rotateRight(Node<T>*);
        Node<T>* rotateLeft(Node<T>*);
        Node<T>* balanceTree(Node<T>*);
    public:
        Tree(Node<T>*);
        void insert(T);
        void remove(T);
        bool search(int, int);
        void update(T, T);
        void printSuccessionLine(int);
        void assignNewLeader();
        void modifyClanMember(const string&);
};

using namespace std;

template<class T>
Tree<T>::Tree(Node<T>* root){
    this->root = root;
}

template<class T>
void Tree<T>::insert(T data){
    this->root = insert(this->root, data);
}

template<class T>
Node<T>* Tree<T>::insert(Node<T>* root, T data){
    if (root == nullptr) return new Node<T>(data);
    if (data < root->getData()) root->setLeft(insert(root->getLeft(), data));
    else root->setRight(insert(root->getRight(), data));

    root->setHeight(1 + max(height(root->getLeft()), height(root->getRight())));

    return balanceTree(root);
}

template<class T>
void Tree<T>::remove(T data){
    if (this->root == nullptr) return;
    else this->root = remove(this->root, data);
}

template<class T>
Node<T>* Tree<T>::remove(Node<T>* root, T data){
    if (root == nullptr) return root;
    if (data < root->getData()) root->setLeft(remove(root->getLeft(), data));
    else if (data > root->getData()) root->setRight(remove(root->getRight(), data));
    else {
        if (root->getLeft() == nullptr) {
            Node<T>* temp = root->getRight();
            delete root;
            return temp;
        } else if (root->getRight() == nullptr) {
            Node<T>* temp = root->getLeft();
            delete root;
            return temp;
        }
        Node<T>* temp = leftmost(root->getRight());
        root->setData(temp->getData());
        root->setRight(remove(root->getRight(), temp->getData()));
    }

    if (root == nullptr) return root;

    root->setHeight(1 + max(height(root->getLeft()), height(root->getRight())));

    return balanceTree(root);
}

template<class T>
Node<T>* Tree<T>::leftmost(Node<T>* node) {
    while(node->getLeft() != nullptr) node = node->getLeft();
    return node;
}

template<class T>
bool Tree<T>::search(int id, int order) {
    bool found;
    switch(order) {
        case PREORDER:
            found = searchPreOrder(this->root, id);
            break;
        case INORDER:
            found = searchInOrder(this->root, id);
            break;
        case POSTORDER:
            found = searchPostOrder(this->root, id);
            break;
        default:
            found = false;
    }
    if (!found) {
        cout << "Nodo con ID '" << id << "' no existe en el arbol" << endl;
    }
    return found;
}

template<class T>
bool Tree<T>::searchPreOrder(Node<T>* node, int id) {
    if (node == nullptr) return false;
    if (node->getData().id == id) {
        return true;
    }
    if (searchPreOrder(node->getLeft(), id)) return true;
    return searchPreOrder(node->getRight(), id);
}

template<class T>
bool Tree<T>::searchInOrder(Node<T>* node, int id) {
    if (node == nullptr) return false;
    if (searchInOrder(node->getLeft(), id)) return true;
    if (node->getData().id == id) {
        return true;
    }
    return searchInOrder(node->getRight(), id);
}

template<class T>
bool Tree<T>::searchPostOrder(Node<T>* node, int id) {
    if (node == nullptr) return false;
    if (searchPostOrder(node->getLeft(), id)) return true;
    if (searchPostOrder(node->getRight(), id)) return true;
    if (node->getData().id == id) {
        return true;
    }
    return false;
}

template<class T>
void Tree<T>::update(T oldData, T newData) {
    this->root = remove(this->root, oldData);
    this->root = insert(this->root, newData);
}

template<class T>
void Tree<T>::printSuccessionLine(int order) {
    switch (order) {
        case PREORDER:
            successionLinePreOrder(this->root);
            break;
        case INORDER:
            successionLineInOrder(this->root);
            break;
        case POSTORDER:
            successionLinePostOrder(this->root);
            break;
        default:
            successionLineInOrder(this->root);
            break;
    }
}

template<class T>
void Tree<T>::successionLinePreOrder(Node<T>* node) {
    if (node == nullptr) return;
    if (node->getData().isDead == 0) {
        cout << "ID: " << node->getData().id 
             << ", Nombre: " << node->getData().name 
             << ", Apellido: " << node->getData().lastName 
             << ", Edad: " << node->getData().age;
        if (node->getData().isChief == 1) {
            cout << " (Lider Actual)";
        }
        cout << endl;
    }
    successionLinePreOrder(node->getLeft());
    successionLinePreOrder(node->getRight());
}

template<class T>
void Tree<T>::successionLineInOrder(Node<T>* node) {
    if (node == nullptr) return;
    successionLineInOrder(node->getLeft());
    if (node->getData().isDead == 0) {
        cout << "ID: " << node->getData().id 
             << ", Nombre: " << node->getData().name 
             << ", Apellido: " << node->getData().lastName 
             << ", Edad: " << node->getData().age;
        if (node->getData().isChief == 1) {
            cout << " (Lider Actual)";
        }
        cout << endl;
    }
    successionLineInOrder(node->getRight());
}

template<class T>
void Tree<T>::successionLinePostOrder(Node<T>* node) {
    if (node == nullptr) return;
    successionLinePostOrder(node->getLeft());
    successionLinePostOrder(node->getRight());
    if (node->getData().isDead == 0) {
        cout << "ID: " << node->getData().id 
             << ", Nombre: " << node->getData().name 
             << ", Apellido: " << node->getData().lastName 
             << ", Edad: " << node->getData().age;
        if (node->getData().isChief == 1) {
            cout << " (Lider Actual)";
        }
        cout << endl;
    }
}

template<class T>
void Tree<T>::assignNewLeader() {
    if (this->root == nullptr) return;

    T currentLeader = this->root->getData();
    if (currentLeader.isDead || currentLeader.age >= 70 || currentLeader.isChief == 0) {
        T newLeader = findNewLeader(this->root, currentLeader);
        if (newLeader.id != 0) {
            cout << "Nuevo Lider Asignado: ID " << newLeader.id 
                 << ", Nombre: " << newLeader.name 
                 << ", Apellido: " << newLeader.lastName << endl;

            currentLeader.isChief = 0;
            newLeader.isChief = 1;

            update(currentLeader, newLeader);

            updateFile("bin/data.csv", currentLeader);
            updateFile("bin/data.csv", newLeader);
        } else {
            cout << "No se ha encontrado un nuevo lider para asignar." << endl;
        }
    }
}

template<class T>
T Tree<T>::findMember(Node<T>* node, int id) {
    if (node == nullptr) return T();

    if (node->getData().id == id) {
        return node->getData();
    }

    T leftResult = findMember(node->getLeft(), id);
    if (leftResult.id != 0) return leftResult;

    return findMember(node->getRight(), id);
}

template<class T>
T Tree<T>::findMember(Node<T>* node) {
    if (node == nullptr) return T();

    if (!node->getData().isDead) {
        return node->getData();
    }

    T leftResult = findMember(node->getLeft());
    if (leftResult.id != 0) return leftResult;

    return findMember(node->getRight());
}

template<class T>
void Tree<T>::modifyClanMember(const string& filePath) {
    int id;
    cout << "Ingrese el ID del miembro a modificar: ";
    cin >> id;

    if (!search(id, INORDER)) {
        cout << "Miembro no encontrado." << endl;
        return;
    }

    T oldMember = findMember(this->root, id);
    if (oldMember.id == 0) {
        cout << "Error: No se pudo encontrar el miembro con ID " << id << "." << endl;
        return;
    }

    T newMember = oldMember;

    auto displayMemberInfo = [](const T& member) {
        cout << "\nInformacion del miembro actual:\n";
        cout << "ID: " << member.id << "\n"
             << "Nombre: " << member.name << "\n"
             << "Apellido: " << member.lastName << "\n"
             << "Genero: " << member.gender << "\n"
             << "Edad: " << member.age << "\n"
             << "ID Padre: " << member.idFather << "\n"
             << "Esta muerto: " << (member.isDead ? "Si" : "No") << "\n"
             << "Fue lider: " << (member.wasChief ? "Si" : "No") << "\n"
             << "Es lider actual: " << (member.isChief ? "Si" : "No") << "\n";
    };

    displayMemberInfo(newMember);

    int choice;
    do {
        cout << "\n--- Modificar Miembro ---\n";
        cout << "1. Nombre\n";
        cout << "2. Apellido\n";
        cout << "3. Genero\n";
        cout << "4. Edad\n";
        cout << "5. Esta muerto\n";
        cout << "6. Fue lider\n";
        cout << "7. Es lider actual\n";
        cout << "0. Guardar y salir\n";
        cout << "Seleccione una opcion: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Ingrese nuevo nombre: ";
                cin >> newMember.name;
                displayMemberInfo(newMember);
                break;
            case 2:
                cout << "Ingrese nuevo apellido: ";
                cin >> newMember.lastName;
                displayMemberInfo(newMember);
                break;
            case 3:
                cout << "Ingrese nuevo genero (H/M): ";
                cin >> newMember.gender;
                if (newMember.gender != 'H' && newMember.gender != 'M') {
                    cout << "Genero invalido. Debe ser 'H' o 'M'.\n";
                    newMember.gender = oldMember.gender;
                }
                displayMemberInfo(newMember);
                break;
            case 4:
                cout << "Ingrese nueva edad: ";
                cin >> newMember.age;
                if (newMember.age >= 70 && oldMember.isChief == 1) {
                    char confirm;
                    cout << "El miembro es el lider actual y su edad sera mayor o igual a 70. Desea remover su liderazgo y asignar un nuevo lider? (Y/N): ";
                    cin >> confirm;
                    if (confirm == 'Y' || confirm == 'y') {
                        newMember.isChief = 0;
                        assignNewLeader();
                        cout << "El lider actual sera removido debido a su edad y se asignara un nuevo lider.\n";
                    } else if (confirm == 'N' || confirm == 'n') {
                        cout << "Operacion cancelada.\n";
                        newMember.age = oldMember.age;
                    } else {
                        cout << "Entrada invalida. Operacion cancelada.\n";
                        newMember.age = oldMember.age;
                    }
                }
                displayMemberInfo(newMember);
                break;
            case 5:
                if (oldMember.isChief) {
                    char confirm;
                    cout << "El miembro es el lider actual. Desea marcarlo como muerto y asignar un nuevo lider? (Y/N): ";
                    cin >> confirm;
                    if (confirm == 'Y' || confirm == 'y') {
                        newMember.isDead = 1;
                        newMember.isChief = 0;
                        assignNewLeader();
                        cout << "El lider actual ha sido marcado como muerto y se ha asignado un nuevo lider.\n";
                    } else if (confirm == 'N' || confirm == 'n') {
                        cout << "Operacion cancelada.\n";
                    } else {
                        cout << "Entrada invalida. Operacion cancelada.\n";
                    }
                } else {
                    cout << "Esta muerto? (1: Si, 0: No): ";
                    cin >> newMember.isDead;
                }
                displayMemberInfo(newMember);
                break;
            case 6:
                cout << "Fue lider? (1: Si, 0: No): ";
                cin >> newMember.wasChief;
                displayMemberInfo(newMember);
                break;
            case 7:
                cout << "Es lider actual? (1: Si, 0: No): ";
                cin >> newMember.isChief;
                displayMemberInfo(newMember);
                break;
            case 0:
                cout << "Guardando cambios...\n";
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (choice != 0);

    update(oldMember, newMember);
    updateFile(filePath, newMember);

    cout << "Datos del miembro actualizados correctamente." << endl;
}

template<class T>
void Tree<T>::updateFile(const string& filePath, const T& updatedNode) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        cerr << "Error abriendo archivo para lectura: " << filePath << endl;
        return;
    }

    stringstream buffer;
    string line;
    bool updated = false;

    getline(inFile, line);
    buffer << line << "\n"; 

    while (getline(inFile, line)) {
        stringstream ss(line);
        string id;
        getline(ss, id, ',');

        if (stoi(id) == updatedNode.id) {
            buffer << updatedNode.id << "," << updatedNode.name << "," << updatedNode.lastName << ","
                   << updatedNode.gender << "," << updatedNode.age << "," << updatedNode.idFather << ","
                   << updatedNode.isDead << "," << updatedNode.wasChief << "," << updatedNode.isChief << "\n";
            updated = true;
        } else buffer << line << "\n";
    }

    inFile.close();

    if (!updated) {
        cerr << "Error: No se encontró el nodo con ID " << updatedNode.id << " en el archivo." << endl;
        return;
    }

    ofstream outFile(filePath, ios::trunc);
    if (!outFile.is_open()) {
        cerr << "Error abriendo archivo para escritura: " << filePath << endl;
        return;
    }

    outFile << buffer.str();
    outFile.close();
}

template<class T>
void Tree<T>::saveNode(Node<T>* node, ofstream& outFile) {
    if (node == nullptr) return;

    saveNode(node->getLeft(), outFile);

    T data = node->getData();
    outFile << data.id << "," << data.name << "," << data.lastName << ","
            << data.gender << "," << data.age << "," << data.idFather << ","
            << data.isDead << "," << data.wasChief << "," << data.isChief << "\n";

    saveNode(node->getRight(), outFile);
}

template<class T>
int Tree<T>::height(Node<T>* node) {
    if (node == nullptr) return 0;
    return node->getHeight();
}

template<class T>
int Tree<T>::getBalanceFactor(Node<T>* node) {
    if (node == nullptr) return 0;
    return height(node->getLeft()) - height(node->getRight());
}

template<class T>
Node<T>* Tree<T>::rotateRight(Node<T>* node) {
    Node<T>* subtreeRoot = node->getLeft();
    Node<T>* rightChild = subtreeRoot->getRight();

    subtreeRoot->setRight(node);
    node->setLeft(rightChild);

    node->setHeight(max(height(node->getLeft()), height(node->getRight())) + 1);
    subtreeRoot->setHeight(max(height(subtreeRoot->getLeft()), height(subtreeRoot->getRight())) + 1);

    return subtreeRoot;
}

template<class T>
Node<T>* Tree<T>::rotateLeft(Node<T>* node) {
    Node<T>* subtreeRoot = node->getRight();
    Node<T>* leftChild = subtreeRoot->getLeft();

    subtreeRoot->setLeft(node);
    node->setRight(leftChild);

    node->setHeight(max(height(node->getLeft()), height(node->getRight())) + 1);
    subtreeRoot->setHeight(max(height(subtreeRoot->getLeft()), height(subtreeRoot->getRight())) + 1);

    return subtreeRoot;
}

template<class T>
Node<T>* Tree<T>::balanceTree(Node<T>* node) {
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1 && getBalanceFactor(node->getLeft()) >= 0)
        return rotateRight(node);

    if (balanceFactor > 1 && getBalanceFactor(node->getLeft()) < 0) {
        node->setLeft(rotateLeft(node->getLeft()));
        return rotateRight(node);
    }

    if (balanceFactor < -1 && getBalanceFactor(node->getRight()) <= 0)
        return rotateLeft(node);

    if (balanceFactor < -1 && getBalanceFactor(node->getRight()) > 0) {
        node->setRight(rotateRight(node->getRight()));
        return rotateLeft(node);
    }

    return node;
}