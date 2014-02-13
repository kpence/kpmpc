/* list.cpp */
#include "list.h"

// Functions required by class to have list:
//
// getNode()
// getHead()
// setAsHead()
// rmHead()
//
/////////////////////////////////////////////////////////////////////

/////////////////////////
/* Other functions (get, set, is) */

// Last
template <typename T> bool Node<T>::isLast() {
    if (getLast() == getRef())
        return true;
    return false;
}

template <typename T> T Node<T>::getLast(bool head) {
    if (getHead() == NULL)
        return NULL;
    if (getNodeHead() == NULL)
        return NULL;
    if (head == false)
        return (getNodeHead()->getLast(true));
    if (getNext() != NULL)
        return (getNodeNext()->getLast(true));
    return getRef();
}

template <typename T> Node<T> *Node<T>::getNodeLast() {
    if (getLast() == NULL)
        return NULL;
    return getLast()->getNode();
}

// Head
template <typename T> T Node<T>::getHead() {
    return getRef()->getHead();
}

template <typename T> Node<T> *Node<T>::getNodeHead() {
    if (getHead() == NULL)
        return NULL;
    return getHead()->getNode();
}

template <typename T> bool Node<T>::isHead() {
    if (getHead() == getRef())
        return true;
    return false;
}

template <typename T> void Node<T>::setAsHead() {
    getRef()->setAsHead();
}

template <typename T> void Node<T>::unsetAsHead() {
    if (isHead()) {
        if (getNext() == NULL)
            getRef()->rmHead();
        else
            getNext()->setAsHead();
    }
}

// Ref -- This is the pointer to the owner of this node object
template <typename T> T Node<T>::getRef() {
    return ref;
}

template <typename T> void Node<T>::setRef(T _ref) {
    ref = _ref;
}

// Next
template <typename T> T Node<T>::getNext() {
    return next;
}

template <typename T> void Node<T>::setNext(T _next) {
    if (_next != getRef())
        next = _next;
}

template <typename T> Node<T> *Node<T>::getNodeNext() {
    return getNext()->getNode();
}

// Prev
template <typename T> Node<T> *Node<T>::getNodePrev(T _list) {
    if (getPrev() == NULL)
        return NULL;
    return getPrev()->getNode();
}

template <typename T> T Node<T>::getPrev(T _list) {
    if (_list == NULL) {
        if (isHead())
            return NULL;
        return (getNodeHead()->getPrev(getRef()));
    }
    if (getNext() != NULL && getNext() != _list)
        return (getNodeNext()->getPrev(_list));
    return getRef();
}

/////////////////////////
/* Node functions */
template <typename T> void Node<T>::clean() {
    detach();
}

template <typename T> void Node<T>::detach() {
    if (!isHead())
        getNodePrev()->setNext(getNext());
    unsetAsHead();
}

template <typename T> void Node<T>::attach() {
    bool cnt = true;
    if (getHead() == NULL)
        setAsHead();
    else {
        if (!isLast()) {
            getNodeLast()->setNext(getRef());
            cnt = false;
        }
        else if (!isHead()) {
            getNodePrev()->setNext(getRef());
            cnt = false;
        }
    }

    if (getPrev() != NULL && cnt)
        getNodePrev()->setNext(getRef());

}

template <typename T> void Node<T>::swap(T _ref) {
    if (_ref != NULL) {
        T _pRef = _ref->getPrev();
        T pRef = getPrev();
        if (!isHead())
            _pRef->getNode()->setNext(getRef());
        if (!_ref->getNode()->isHead())
            pRef->getNode()->setNext(_ref);
        if (_ref->getNode()->isHead())
            setAsHead();
        if (isHead())
            _ref->setAsHead();
    }
}

/////////////////////////
/* Main functions */
template <typename T> Node<T>::Node(T _ref) {
    reset();
    setRef(_ref);
}

template <typename T> Node<T>::~Node() {
    reset();
}

template <typename T> void Node<T>::reset() {
    next = NULL;
    ref = NULL;
}
