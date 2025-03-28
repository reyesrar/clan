#pragma once
template<class T>

class Node{
    private:
        T data;
        Node<T>* left = nullptr;
        Node<T>* right = nullptr;
        int height; // New property
    public:
        Node(T, Node<T>*, Node<T>*);
        Node(T);
        ~Node();
        Node<T>* getLeft();
        Node<T>* getRight();
        void setLeft(Node<T>*);
        void setRight(Node<T>*);
        T getData();
        void setData(T);
        int getHeight(); // New method
        void setHeight(int); // New method
        void print();
};

using namespace std;

template<class T>
Node<T>::Node(T data, Node<T>* left, Node<T>* right){
    this->data = data;
    this->left = left;
    this->right = right;
    this->height = 1; // Initialize height
}

template<class T>
Node<T>::Node(T data){
    this->data = data;
    this->height = 1; // Initialize height
}

template<class T>
Node<T>::~Node(){
    this->left = nullptr;
    this->right = nullptr;
}

template<class T>
Node<T>* Node<T>::getLeft(){
    return this->left;
}

template<class T>
Node<T>* Node<T>::getRight(){
    return this->right;
}

template<class T>
void Node<T>::setLeft(Node<T>* left){
    this->left = left;
}

template<class T>
void Node<T>::setRight(Node<T>* right){
    this->right = right;
}

template<class T>
T Node<T>::getData(){
    return this->data;
}

template<class T>
void Node<T>::setData(T data){
    this->data = data;
}

template<class T>
int Node<T>::getHeight(){
    return this->height;
}

template<class T>
void Node<T>::setHeight(int height){
    this->height = height;
}

template<class T>
void Node<T>::print(){
    cout<<this->data<<endl;
}