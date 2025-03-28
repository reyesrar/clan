#include "node.h"
enum Order{PREORDER, INORDER, POSTORDER};

template<class T>
class Tree{
    private:
        Node<T>* root = nullptr;
        Node<T>* insert(Node<T>*, T);
        Node<T>* remove(Node<T>*, T);
        Node<T>* leftmost(Node<T>*);
        bool searchPreOrder(Node<T>*, T);
        bool searchInOrder(Node<T>*, T);
        bool searchPostOrder(Node<T>*, T);
        int height(Node<T>*);
        int getBalanceFactor(Node<T>*);
        Node<T>* rotateRight(Node<T>*);
        Node<T>* rotateLeft(Node<T>*);
        Node<T>* balanceTree(Node<T>*);
    public:
        Tree(Node<T>*);
        void insert(T);
        void remove(T);
        bool search(T, int);
        void update(T, T);
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
bool Tree<T>::search(T data, int order){
    bool found;
    switch(order) {
        case PREORDER:
            found = searchPreOrder(this->root, data);
            break;
        case INORDER:
            found = searchInOrder(this->root, data);
            break;
        case POSTORDER:
            found = searchPostOrder(this->root, data);
            break;
        default:
            found = false;
    }
    if (!found) {
        cout << "Nodo '" << data << "' no existe en el arbol" << endl;
    }
    return found;
}

template<class T>
bool Tree<T>::searchPreOrder(Node<T>* node, T data) {
    if (node == nullptr) return false;
    cout << "Recorriendo nodo '" << node->getData() << "'" << endl;
    if (node->getData() == data) {
        cout << "Nodo '"<< data << "' encontrado" << endl;
        return true;
    }
    if (searchPreOrder(node->getLeft(), data)) return true;
    return searchPreOrder(node->getRight(), data);
}

template<class T>
bool Tree<T>::searchInOrder(Node<T>* node, T data) {
    if (node == nullptr) return false;
    if (searchInOrder(node->getLeft(), data)) return true;
    cout << "Recorriendo nodo '" << node->getData() << "'" << endl;
    if (node->getData() == data) {
        cout << "Nodo '"<< data << "' encontrado" << endl;
        return true;
    }
    return searchInOrder(node->getRight(), data);
}

template<class T>
bool Tree<T>::searchPostOrder(Node<T>* node, T data) {
    if (node == nullptr) return false;
    if (searchPostOrder(node->getLeft(), data)) return true;
    if (searchPostOrder(node->getRight(), data)) return true;
    cout << "Recorriendo nodo '" << node->getData() << "'" << endl;
    if (node->getData() == data) {
        cout << "Nodo '"<< data << "' encontrado" << endl;
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

