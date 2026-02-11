#ifndef __DOUBLE_CIRCULAR_LIST_H__
#define __DOUBLE_CIRCULAR_LIST_H__

#include "doublelinkedlist.h"

template <typename Traits>
class CDoubleCircularLinkedList : public CDoubleLinkedList<Traits> {
    using value_type = typename Traits::value_type;
    using Node       = NodeDoubleLinkedList<Traits>;
    using BaseNode   = NodeLinkedList<Traits>;

public:
    CDoubleCircularLinkedList() : CDoubleLinkedList<Traits>() {}
    ~CDoubleCircularLinkedList();
    CDoubleCircularLinkedList(const CDoubleCircularLinkedList<Traits>& other);
    CDoubleCircularLinkedList(CDoubleCircularLinkedList<Traits>&& other) noexcept;
    CDoubleCircularLinkedList<Traits>& operator=(const CDoubleCircularLinkedList<Traits>& other);
    CDoubleCircularLinkedList<Traits>& operator=(CDoubleCircularLinkedList<Traits>&& other) noexcept;
    void push_back(const value_type &val, ref_type ref) ;
    void Insert(const value_type &val, ref_type ref) ;
    friend ostream &operator<<(ostream &os, CDoubleCircularLinkedList<Traits> &container) {
        std::lock_guard<std::recursive_mutex> lock(container.m_mtx);
        os << "CDoubleCircularLinkedList: size = " << container.m_nElements << endl;
        if (!container.m_pRoot) 
            return os;
        BaseNode *currentNode = container.m_pRoot;
        do {
            os << "[" << currentNode->GetValue() << "," << currentNode->GetRef() << "]";
            if (currentNode->GetNext() != container.m_pRoot)
                os << "<->";
            currentNode = currentNode->GetNext();  
        } while (currentNode != container.m_pRoot); 
        return os;
    }

    friend istream &operator>>(istream& is, CDoubleCircularLinkedList<Traits> &container) {
        size_t nElements;
        is >> nElements;
        using value_type = typename Traits::value_type; 
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

template <typename Traits>
CDoubleCircularLinkedList<Traits>::~CDoubleCircularLinkedList() {
    if (!this->m_pRoot)
        return;
    if (this->m_pLast) {
        this->m_pLast->GetNextRef() = nullptr;
    }
}

template <typename Traits>
void CDoubleCircularLinkedList<Traits>::push_back(const value_type &val, ref_type ref) {
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
    Node *pNewNode = new Node(val, ref, (BaseNode*)this->m_pRoot, (Node*)this->m_pLast);
    
    if (!this->m_pRoot) {
        this->m_pRoot = pNewNode;
        this->m_pLast = pNewNode;
        pNewNode->GetNextRef() = pNewNode; 
        pNewNode->GetPrevRef() = pNewNode; 
    } else {
        this->m_pLast->GetNextRef() = pNewNode;
        ((Node*)this->m_pRoot)->GetPrevRef() = pNewNode;
        this->m_pLast = pNewNode;
    }
    ++this->m_nElements;
}

template <typename Traits>
void CDoubleCircularLinkedList<Traits>::Insert(const value_type &val, ref_type ref) {
    std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
    if (!this->m_pRoot) {
        push_back(val, ref); 
        return;
    }
    InternalInsert(this->m_pRoot, (BaseNode*)this->m_pLast, val, ref);
}

template <typename Traits>
void CDoubleCircularLinkedList<Traits>::InternalInsert(BaseNode *&rParent, BaseNode *pPrev, const value_type &val, ref_type ref) {
    if (rParent->GetValue() > val) {
        Node *pNew = new Node(val, ref, rParent, (Node*)pPrev);
        ((Node*)rParent)->GetPrevRef() = pNew;
        rParent = pNew; 
        if (pPrev == this->m_pLast)
            this->m_pLast->GetNextRef() = pNew;
        ++this->m_nElements;
        return;
    }
    if (rParent->GetNext() == this->m_pRoot) {
        push_back(val, ref);
        return;
    }
    InternalInsert(rParent->GetNextRef(), rParent, val, ref);
}

// Constructor de copia
template <typename Traits>
CDoubleCircularLinkedList<Traits>::CDoubleCircularLinkedList(const CDoubleCircularLinkedList<Traits>& other) 
    : CDoubleLinkedList<Traits>() {
    std::lock_guard<std::recursive_mutex> lock(other.m_mtx);
    if (!other.m_pRoot) return;
    
    BaseNode* currentNode = other.m_pRoot;
    do {
        push_back(currentNode->GetValue(), currentNode->GetRef());
        currentNode = currentNode->GetNext();
    } while (currentNode != other.m_pRoot);
}

// Move constructor
template <typename Traits>
CDoubleCircularLinkedList<Traits>::CDoubleCircularLinkedList(CDoubleCircularLinkedList<Traits>&& other) noexcept 
    : CDoubleLinkedList<Traits>() {
    std::lock_guard<std::recursive_mutex> lock(other.m_mtx);
    this->m_pRoot     = std::exchange(other.m_pRoot, nullptr);
    this->m_pLast     = std::exchange(other.m_pLast, nullptr);
    this->m_nElements = std::exchange(other.m_nElements, 0);
}

// Operator '=' constuctor copia
template <typename Traits>
CDoubleCircularLinkedList<Traits>& CDoubleCircularLinkedList<Traits>::operator=(const CDoubleCircularLinkedList<Traits>& other) {
    if (this == &other) return *this;
    
    std::lock(this->m_mtx, other.m_mtx);
    std::lock_guard<std::recursive_mutex> lock1(this->m_mtx, std::adopt_lock);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx, std::adopt_lock);


    if (this->m_pRoot) {
        this->m_pLast->GetNextRef() = nullptr;
        BaseNode* current = this->m_pRoot;
        while (current != nullptr) {
            BaseNode* next = current->GetNext();
            delete current;
            current = next;
        }
    }
    this->m_pRoot = nullptr;
    this->m_pLast = nullptr;
    this->m_nElements = 0;

    if (!other.m_pRoot) 
        return *this;
    BaseNode* currentNode = other.m_pRoot;
    do {
        this->push_back(currentNode->GetValue(), currentNode->GetRef());
        currentNode = currentNode->GetNext();
    } while (currentNode != other.m_pRoot);
    return *this;
}

// Operator '=' move ocnstructor
template <typename Traits>
CDoubleCircularLinkedList<Traits>& CDoubleCircularLinkedList<Traits>::operator=(CDoubleCircularLinkedList<Traits>&& other) noexcept {
    if (this == &other) return *this;

    std::lock(this->m_mtx, other.m_mtx);
    std::lock_guard<std::recursive_mutex> lock1(this->m_mtx, std::adopt_lock);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx, std::adopt_lock);
    if (this->m_pRoot) {
        this->m_pLast->GetNextRef() = nullptr; 
        BaseNode* current = this->m_pRoot;
        while (current != nullptr) {
            BaseNode* next = current->GetNext();
            delete current;
            current = next;
        }
    }
    this->m_pRoot     = std::exchange(other.m_pRoot, nullptr);
    this->m_pLast     = std::exchange(other.m_pLast, nullptr);
    this->m_nElements = std::exchange(other.m_nElements, 0);
    return *this;
}

#endif // __DOUBLE_CIRCULAR_LIST_H__
