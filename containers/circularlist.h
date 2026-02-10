#ifndef __CIRCULARLIST_H__
#define __CIRCULARLIST_H__
#include "linkedlist.h"

using namespace std;

template <typename Traits>
class CCircularList : public CLinkedList<Traits> {
    using value_type      = typename Traits::value_type;
    using Node            = NodeLinkedList<Traits>;
    using forwardIterator = LinkedListForwardIterator<Traits>;

public:
    CCircularList() : CLinkedList<Traits>() {}
    CCircularList(const CCircularList<Traits>& other);
    CCircularList(CCircularList<Traits>&& other) noexcept  
    : CLinkedList<Traits>(std::move(other)) 
    {}
    virtual ~CCircularList();
    void push_back(const value_type &val, ref_type ref);
    void Insert(const value_type &val, ref_type ref);
    CCircularList& operator=(const CCircularList<Traits>& other);
    CCircularList& operator=(CCircularList<Traits>&& other) noexcept; 
    friend ostream &operator<<(ostream &os, CCircularList<Traits> &container){
        std::lock_guard<std::recursive_mutex> lock(container.m_mtx);
        os << "CCircularList: size = " << container.m_nElements << endl;
        if (!container.m_pRoot) 
            return os;
        Node *currentNode = container.m_pRoot;
        do {
            os << "[" << currentNode->GetValue() << "," << currentNode->GetRef() << "]";
            if (currentNode->GetNext() != container.m_pRoot)
                os << "->";
            currentNode = currentNode->GetNext();  
        } while (currentNode != container.m_pRoot); 
        return os;
    }
    
    friend istream &operator>>(istream& is, CCircularList<Traits> &container){
        size_t nElements;
        is >> nElements;
        for (size_t i=0 ; i < nElements ; ++i ){
            value_type val;
            ref_type ref;
            is >> val >> ref;
            container.Insert(val,ref);
        }
        return is;
    }

private:
    void InternalInsert(Node *&rParent, const value_type &val, ref_type ref);
};

template <typename Traits>
CCircularList<Traits>::~CCircularList(){
    if (!this->m_pRoot)
        return;
    this->m_pLast->GetNextRef() = nullptr;
}

template <typename Traits>
void CCircularList<Traits>::push_back(const value_type &val, ref_type ref){
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
    Node *pNewNode = new Node(val, ref);
    if (!this->m_pRoot) {
        this->m_pRoot = pNewNode;
        this->m_pLast = pNewNode;
        pNewNode->GetNextRef() = pNewNode;
    } 
    else {
        this->m_pLast->GetNextRef() = pNewNode;
        this->m_pLast = pNewNode;
        pNewNode->GetNextRef() = this->m_pRoot;
    }
    ++this->m_nElements;
}

template <typename Traits>
void CCircularList<Traits>::Insert(const value_type &val, ref_type ref){
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
    InternalInsert(this->m_pRoot, val, ref);
    if (this->m_pLast)
        this->m_pLast->GetNextRef() = this->m_pRoot;
}

template <typename Traits>
void CCircularList<Traits>::InternalInsert(Node *&rParent, const value_type &val, ref_type ref){
   if( !rParent || rParent->GetValue() > val ){
        Node *pNew = new Node(val, ref, rParent);
        rParent = pNew;
        if ( !pNew->GetNext())
            this->m_pLast = pNew;
        ++this->m_nElements;
        return;
    }
    if ( !rParent->GetNext() || rParent->GetNext() == this->m_pRoot){
        Node *pNew = new Node(val, ref, this->m_pRoot);
        rParent->GetNextRef() = pNew;
        ++this->m_nElements;
        this->m_pLast = pNew;
        return;
    }
    InternalInsert(rParent->GetNextRef(), val, ref);
}

//constructor copia
template <typename Traits>
CCircularList<Traits>::CCircularList(const CCircularList<Traits>& other)
    : CLinkedList<Traits>() {
        std::lock_guard<std::recursive_mutex> lock(other.m_mtx);
        if (!other.m_pRoot) 
            return;
    Node *currentNode = other.m_pRoot;
    do{
        push_back(currentNode->GetValue(), currentNode->GetRef());
        currentNode = currentNode->GetNext();
    }while (currentNode != other.m_pRoot);
}

//operador '=' para constructor copia
template <typename Traits>
CCircularList<Traits>& CCircularList<Traits>::operator=(const CCircularList<Traits>& other){
    if (this == &other)
        return *this;
    std::lock(this->m_mtx, other.m_mtx);
    std::lock_guard<std::recursive_mutex> lock1(this->m_mtx, std::adopt_lock);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx, std::adopt_lock);
    if (this->m_pRoot) {
        this->m_pLast->GetNextRef() = nullptr;
        Node *currentNode = this->m_pRoot;
        while (currentNode != nullptr) {
            Node* nextNode = currentNode->GetNext();
            delete currentNode;
            currentNode = nextNode;
        }
    }
    this->m_pRoot     = nullptr;
    this->m_pLast     = nullptr;
    this->m_nElements = 0;
    if (!other.m_pRoot)
        return *this;
    Node *currentNode = other.m_pRoot;
    do {
        push_back(currentNode->GetValue(), currentNode->GetRef());
        currentNode = currentNode->GetNext();
    } while (currentNode != other.m_pRoot);
    return *this;
}

//operador = para constructor move
template <typename Traits>
CCircularList<Traits>& CCircularList<Traits>::operator=(CCircularList<Traits>&& other) noexcept{
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
    std::lock_guard<std::recursive_mutex> lockOther(other.m_mtx);
    if( this != &other ){
        if (this->m_pRoot) {
            this->m_pLast->GetNextRef() = nullptr;
            Node *currentNode = this->m_pRoot;
            while ( currentNode != nullptr){
                Node* nextNode = currentNode->GetNext();
                delete currentNode;
                currentNode = nextNode;
            }
        }
        this->m_pRoot     = std::exchange(other.m_pRoot, nullptr);
        this->m_pLast     = std::exchange(other.m_pLast, nullptr);
        this->m_nElements = std::exchange(other.m_nElements, 0);
    }
    return *this;
}



#endif //__CIRCULARLIST_H__
