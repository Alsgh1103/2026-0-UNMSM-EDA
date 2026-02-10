#ifndef __DOUBLE_LINKED_LIST_H__
#define __DOUBLE_LINKED_LIST_H__

#include <iostream>
#include "../general/types.h"
#include "../util.h"
#include "linkedlist.h" 

using namespace std;

template <typename Traits>
class NodeDoubleLinkedList : public NodeLinkedList<Traits> {
    using value_type = typename Traits::value_type;
    using Node       = NodeDoubleLinkedList<Traits>;
    using BaseNode   = NodeLinkedList<Traits>;

private:
    Node *m_pPrev = nullptr;

public:
    NodeDoubleLinkedList(value_type _value, ref_type _ref = -1, BaseNode* _next = nullptr, Node* _prev = nullptr)
        : BaseNode(_value, _ref, _next), m_pPrev(_prev) {}

    Node* GetPrev() const { return m_pPrev; }
    Node*& GetPrevRef() { return m_pPrev; }
};


template <typename Traits>
class DoubleLinkedListBackwardIterator {
    using Node       = NodeDoubleLinkedList<Traits>;
    using BaseNode   = NodeLinkedList<Traits>;
    using value_type = typename Traits::value_type;

private:
    Node* m_pCurrent;

public:
    DoubleLinkedListBackwardIterator(Node* node) 
        : m_pCurrent(node) {}

    DoubleLinkedListBackwardIterator& operator++() {
        assert( m_pCurrent != nullptr);
            m_pCurrent = m_pCurrent->GetPrev();
        return *this;
    }

    value_type& operator*() {
        return m_pCurrent->GetValueRef();
    }

    bool operator!=(const DoubleLinkedListBackwardIterator& other) const {
        return m_pCurrent != other.m_pCurrent;
    }
};

template <typename Traits>
class CDoubleLinkedList : public CLinkedList<Traits> {
    using value_type = typename Traits::value_type;
    using Node       = NodeDoubleLinkedList<Traits>;
    using BaseNode   = NodeLinkedList<Traits>;
    using ForwardIterator = LinkedListForwardIterator<Traits>;
    using BackwardIterator = DoubleLinkedListBackwardIterator<Traits>;

public:
    CDoubleLinkedList() : CLinkedList<Traits>() {}
    CDoubleLinkedList(const CDoubleLinkedList<Traits>& other);
    CDoubleLinkedList(CDoubleLinkedList<Traits>&& other) noexcept;
    CDoubleLinkedList<Traits>& operator=(const CDoubleLinkedList<Traits>& other);
    CDoubleLinkedList<Traits>& operator=(CDoubleLinkedList<Traits>&& other) noexcept;
    virtual ~CDoubleLinkedList() {}
    void Insert(const value_type &val, ref_type ref);
    void push_back(const value_type &val, ref_type ref);
    ForwardIterator begin() { return ForwardIterator(this->m_pRoot); }
    ForwardIterator end()   { return ForwardIterator(nullptr); }
    BackwardIterator rbegin() { return BackwardIterator((Node*)(this->m_pLast)); }
    BackwardIterator rend()   { return BackwardIterator(nullptr); }
    
    friend ostream &operator<<(ostream &os, CDoubleLinkedList<Traits> &container) {
        os << "CDoubleLinkedList: size = " << container.m_nElements << endl;
        BaseNode *currentNode = container.m_pRoot;
        while (currentNode != nullptr) {
            os << "[" << currentNode->GetValue() << "," << currentNode->GetRef() << "]";
            if (currentNode->GetNext() != nullptr)
                os << "<->"; 
            currentNode = currentNode->GetNext();
        }
        return os;
    }


    friend istream &operator>>(istream& is, CDoubleLinkedList<Traits> &container) {
        size_t nElements;
        is >> nElements;
        for (size_t i = 0; i < nElements; ++i) {
            value_type val;
            ref_type ref;
            is >> val >> ref;
            container.Insert(val, ref);
        }
        return is;
    }

private:
    void InternalInsert(BaseNode *&rParent, BaseNode *pPrev, const value_type &val, ref_type ref);
};


//Constructor copia
template <typename Traits>
CDoubleLinkedList<Traits>::CDoubleLinkedList(const CDoubleLinkedList<Traits>& other) : CLinkedList<Traits>() {
    std::lock_guard<std::recursive_mutex> lock(other.m_mtx);
    BaseNode* currentNode = other.m_pRoot;
    while (currentNode != nullptr) {
        this->push_back(currentNode->GetValue(), currentNode->GetRef());
        currentNode = currentNode->GetNext();
    }
}

//Move Constructor
template <typename Traits>
CDoubleLinkedList<Traits>::CDoubleLinkedList(CDoubleLinkedList<Traits>&& other) noexcept : CLinkedList<Traits>() {
    std::lock_guard<std::recursive_mutex> lock(other.m_mtx);
    this->m_pRoot     = std::exchange(other.m_pRoot, nullptr);
    this->m_pLast     = std::exchange(other.m_pLast, nullptr);
    this->m_nElements = std::exchange(other.m_nElements, 0);
}

//Operador de asignación '=' copia
template <typename Traits>
CDoubleLinkedList<Traits>& CDoubleLinkedList<Traits>::operator=(const CDoubleLinkedList<Traits>& other) {
    if (this == &other) 
        return *this;
    std::lock(this->m_mtx, other.m_mtx);
    std::lock_guard<std::recursive_mutex> lock1(this->m_mtx, std::adopt_lock);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx, std::adopt_lock);
    BaseNode* current = this->m_pRoot;
    while (current != nullptr) {
        BaseNode* next = current->GetNext();
        delete current;
        current = next;
    }
    this->m_pRoot = nullptr;
    this->m_pLast = nullptr;
    this->m_nElements = 0;

    BaseNode* otherNode = other.m_pRoot;
    while (otherNode != nullptr) {
        this->push_back(otherNode->GetValue(), otherNode->GetRef());
        otherNode = otherNode->GetNext();
    }
    return *this;
}

//Operador de asignación '=' - move constructor
template <typename Traits>
CDoubleLinkedList<Traits>& CDoubleLinkedList<Traits>::operator=(CDoubleLinkedList<Traits>&& other) noexcept {
    if (this == &other) return *this;

    std::lock(this->m_mtx, other.m_mtx);
    std::lock_guard<std::recursive_mutex> lock1(this->m_mtx, std::adopt_lock);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx, std::adopt_lock);

    BaseNode* current = this->m_pRoot;
    while (current != nullptr) {
        BaseNode* next = current->GetNext();
        delete current;
        current = next;
    }

    this->m_pRoot     = std::exchange(other.m_pRoot, nullptr);
    this->m_pLast     = std::exchange(other.m_pLast, nullptr);
    this->m_nElements = std::exchange(other.m_nElements, 0);

    return *this;
}

template <typename Traits>
void CDoubleLinkedList<Traits>::push_back(const value_type &val, ref_type ref) {
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
    Node *pNewNode = new Node(val, ref, nullptr, (Node*)(this->m_pLast));
    if (!this->m_pRoot)
        this->m_pRoot = pNewNode;
     else 
        this->m_pLast->GetNextRef() = pNewNode;
    this->m_pLast = pNewNode;
    ++this->m_nElements;
}

template <typename Traits>
void CDoubleLinkedList<Traits>::Insert(const value_type &val, ref_type ref) {
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
    InternalInsert(this->m_pRoot, nullptr, val, ref);
}

template <typename Traits>
void CDoubleLinkedList<Traits>::InternalInsert(BaseNode *&rParent, BaseNode *pPrev, const value_type &val, ref_type ref) {
    if (!rParent || rParent->GetValue() > val) {
        Node *pNew = new Node(val, ref, rParent, (Node*)(pPrev));
        if (rParent)
            ((Node*)rParent)->GetPrevRef() = pNew;
        else 
            this->m_pLast = pNew;
        rParent = pNew;
        ++this->m_nElements;
        return;
    }
    InternalInsert(rParent->GetNextRef(), rParent, val, ref);
}

#endif // __DOUBLE_LINKED_LIST_H__