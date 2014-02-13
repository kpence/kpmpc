/* list.h */
/////////////////////////////////////////////////////////////////////
//
// Functions required by class to have list:
//
// getNode()
// getHead()
// setAsHead()
// rmHead()
//
/////////////////////////////////////////////////////////////////////

template <typename T> class Node {
private:
    T next;
    T ref;
    void reset();
protected:
public:
    Node(T _ref);
    ~Node();

    T getRef();
    void setRef(T _ref);
    void setNext(T _next);

    Node<T> *getNodeNext();
    Node<T> *getNodePrev(T _ref = NULL);
    Node<T> *getNodeLast();
    Node<T> *getNodeHead();

    T getNext();
    T getPrev(T _ref = NULL);
    T getLast(bool head = false);
    T getHead();

    bool isHead();
    bool isLast();

    void swap(T _ref);

    void clean();

    void detach();
    void attach();

    void setAsHead();
    void unsetAsHead();

};
