#include "node.h"
#include "../models/ClanMember.h"
#include <fstream>
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
        int height(Node<T>*);
        int getBalanceFactor(Node<T>*);
        Node<T>* rotateRight(Node<T>*);
        Node<T>* rotateLeft(Node<T>*);
        Node<T>* balanceTree(Node<T>*);
        void successionLinePreOrder(Node<T>*);
        void successionLineInOrder(Node<T>*);
        void successionLinePostOrder(Node<T>*);
        T findNewLeader(Node<T>*);
        void updateLeaderInFile(const T&, const T&);
    public:
        Tree(Node<T>*);
        void insert(T);
        void remove(T);
        bool search(int, int);
        void update(T, T);
        void printSuccessionLine(int);
        void assignNewLeader();
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
    if(root == nullptr) return new Node<T>(data);
    if(data < root->getData()) root->setLeft(insert(root->getLeft(), data));
    else root->setRight(insert(root->getRight(), data));

    root->setHeight(1 + max(height(root->getLeft()), height(root->getRight())));

    return balanceTree(root);
}

template<class T>
void Tree<T>::remove(T data){
    this->root = remove(this->root, data);
}

template<class T>
Node<T>* Tree<T>::remove(Node<T>* root, T data){
    if(root == nullptr) return root;
    if(data < root->getData()) root->setLeft(remove(root->getLeft(), data));
    else if(data > root->getData()) root->setRight(remove(root->getRight(), data));
    else {
        if(root->getLeft() == nullptr) {
            Node<T>* temp = root->getRight();
            delete root;
            return temp;
        }
        else if(root->getRight() == nullptr) {
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
    cout << "Recorriendo nodo con ID '" << node->getData().id 
         << "', Nombre: '" << node->getData().name 
         << "', Apellido: '" << node->getData().lastName << "'" << endl;
    if (node->getData().id == id) {
        cout << "Nodo con ID '" << id << "' encontrado" << endl;
        return true;
    }
    if (searchPreOrder(node->getLeft(), id)) return true;
    return searchPreOrder(node->getRight(), id);
}

template<class T>
bool Tree<T>::searchInOrder(Node<T>* node, int id) {
    if (node == nullptr) return false;
    if (searchInOrder(node->getLeft(), id)) return true;
    cout << "Recorriendo nodo con ID '" << node->getData().id 
         << "', Nombre: '" << node->getData().name 
         << "', Apellido: '" << node->getData().lastName << "'" << endl;
    if (node->getData().id == id) {
        cout << "Nodo con ID '" << id << "' encontrado" << endl;
        return true;
    }
    return searchInOrder(node->getRight(), id);
}

template<class T>
bool Tree<T>::searchPostOrder(Node<T>* node, int id) {
    if (node == nullptr) return false;
    if (searchPostOrder(node->getLeft(), id)) return true;
    if (searchPostOrder(node->getRight(), id)) return true;
    cout << "Recorriendo nodo con ID '" << node->getData().id 
         << "', Nombre: '" << node->getData().name 
         << "', Apellido: '" << node->getData().lastName << "'" << endl;
    if (node->getData().id == id) {
        cout << "Nodo con ID '" << id << "' encontrado" << endl;
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
    if (!node->getData().isDead) {
        cout << "ID: " << node->getData().id 
             << ", Nombre: " << node->getData().name 
             << ", Apellido: " << node->getData().lastName 
             << ", Edad: " << node->getData().age;
        if (node->getData().isChief) {
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
    if (!node->getData().isDead) {
        cout << "ID: " << node->getData().id 
             << ", Nombre: " << node->getData().name 
             << ", Apellido: " << node->getData().lastName 
             << ", Edad: " << node->getData().age;
        if (node->getData().isChief) {
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
    if (!node->getData().isDead) {
        cout << "ID: " << node->getData().id 
             << ", Nombre: " << node->getData().name 
             << ", Apellido: " << node->getData().lastName 
             << ", Edad: " << node->getData().age;
        if (node->getData().isChief) {
            cout << " (Lider Actual)";
        }
        cout << endl;
    }
}

template<class T>
void Tree<T>::assignNewLeader() {
    if (this->root == nullptr) return;

    T currentLeader = this->root->getData();
    if (currentLeader.isDead || currentLeader.age >= 70) {
        T newLeader = findNewLeader(this->root);
        if (newLeader.id != 0) {
            cout << "Nuevo Lider Asignado: ID " << newLeader.id 
                 << ", Nombre: " << newLeader.name 
                 << ", Apellido: " << newLeader.lastName << endl;

            currentLeader.isChief = 0;
            newLeader.isChief = 1;

            update(currentLeader, newLeader);
            updateLeaderInFile(currentLeader, newLeader);
        } else {
            cout << "No se ha encontrado un nuevo lider para asignar." << endl;
        }
    } else if (currentLeader.isChief && currentLeader.isDead) {
        currentLeader.isChief = 0;
        updateLeaderInFile(currentLeader, currentLeader);
    }
}

template<class T>
void Tree<T>::updateLeaderInFile(const T& oldLeader, const T& newLeader) {
    const string filePath = "bin/data.csv";
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error abriendo archivo: " << filePath << endl;
        return;
    }

    string line, updatedContent;
    getline(file, line);
    updatedContent += line + "\n";

    while (getline(file, line)) {
        stringstream ss(line);
        string id;
        getline(ss, id, ',');

        if (stoi(id) == oldLeader.id) {
            line = to_string(oldLeader.id) + ", " + oldLeader.name + ", " + oldLeader.lastName + ", " +
                   oldLeader.gender + ", " + to_string(oldLeader.age) + ", " + to_string(oldLeader.idFather) + ", " +
                   to_string(oldLeader.isDead ? 1 : 0) + ", " + to_string(oldLeader.wasChief ? 1 : 0) + ", 0";
        } else if (stoi(id) == newLeader.id) {
            line = to_string(newLeader.id) + ", " + newLeader.name + ", " + newLeader.lastName + ", " +
                   newLeader.gender + ", " + to_string(newLeader.age) + ", " + to_string(newLeader.idFather) + ", " +
                   to_string(newLeader.isDead ? 1 : 0) + ", " + to_string(newLeader.wasChief ? 1 : 0) + ", 1";
        }

        updatedContent += line + "\n";
    }

    file.close();

    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        cerr << "Error de escritura: " << filePath << endl;
        return;
    }

    outFile << updatedContent;
    outFile.close();
}

template<class T>
T Tree<T>::findNewLeader(Node<T>* node) {
    if (node == nullptr) return T();

    Node<T>* left = node->getLeft();
    Node<T>* right = node->getRight();

    if (left != nullptr && !left->getData().isDead && left->getData().gender == 'H') {
        return left->getData();
    }
    if (right != nullptr && !right->getData().isDead && right->getData().gender == 'H') {
        return right->getData();
    }

    T leftResult = findNewLeader(left);
    if (leftResult.id != 0) return leftResult;

    T rightResult = findNewLeader(right);
    if (rightResult.id != 0) return rightResult;

    if (left != nullptr && !left->getData().isDead && left->getData().gender == 'M') {
        return left->getData();
    }
    if (right != nullptr && !right->getData().isDead && right->getData().gender == 'M') {
        return right->getData();
    }

    return T();
}

