#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#include <iostream>
#include <utility>
#include <cassert>
#include <mutex>
#include "../general/types.h"
#include "../util.h"
#include "../foreach.h"

using namespace std;

// TODO: Traits para listas enlazadas
template <typename T, typename _Func>
struct ListTrait{
    using value_type = T;
    using Func       = _Func;
};

template <typename T>
struct AscendingTrait : 
    public ListTrait<T, std::greater<T> >{
};

template <typename T>
struct DescendingTrait : 
    public ListTrait<T, std::less<T> >{
};

// Iterators para listas enlazadas

template <typename Traits>
class NodeLinkedList{

    using  value_type  = typename Traits::value_type;
    using  Node        = NodeLinkedList<Traits>;
private:
    
    value_type m_data;
    ref_type   m_ref;
    Node *m_pNext     = nullptr;

public:
    NodeLinkedList(){}
    NodeLinkedList( value_type _value, ref_type _ref = -1, Node* _next=nullptr)
        : m_data(_value), m_ref(_ref), m_pNext(_next) {   }
    value_type  GetValue   () const { return m_data; }
    value_type &GetValueRef() { return m_data; }

    ref_type    GetRef     () const { return m_ref;   }
    ref_type   &GetRefRef  () { return m_ref;   }

    Node      * GetNext     () const { return m_pNext;   }
    Node      *&GetNextRef  () { return m_pNext;   }

    Node &operator=(const Node &another){
        m_data = another.GetValue();
        m_ref   = another.GetRef();
        return *this;
    }
    bool operator==(const Node &another) const
    { return m_data == another.GetValue();   }
    bool operator<(const Node &another) const
    { return m_data < another.GetValue();   }
};


//iterator
template <typename Traits>
class LinkedListForwardIterator{
    using Node       = NodeLinkedList<Traits>;
    using value_type = typename Traits::value_type;
private:
    Node* m_pCurrent;
    Node* m_pStart;
    bool  m_bFirstPass;
public:
    LinkedListForwardIterator(Node* node) 
        : m_pCurrent(node),
          m_pStart (node),
          m_bFirstPass(true)
        {}
    LinkedListForwardIterator& operator++();
    value_type& operator*();
    bool operator!=(const LinkedListForwardIterator& other) const;
    bool operator==(const LinkedListForwardIterator& other) const;
};

//Operador ++
template <typename Traits>
LinkedListForwardIterator<Traits>& LinkedListForwardIterator<Traits>::operator++(){
    assert( m_pCurrent != nullptr);
    m_pCurrent = m_pCurrent->GetNext();

    if(m_pCurrent == m_pStart && !m_bFirstPass)
        m_pCurrent = nullptr;

    m_bFirstPass = false;
    return *this;
}

//Operador *
template <typename Traits>
typename LinkedListForwardIterator<Traits>::value_type& LinkedListForwardIterator<Traits>::operator*(){
    return m_pCurrent->GetValueRef();

}

//Operador !=
template <typename Traits>
bool LinkedListForwardIterator<Traits>::operator!=(const LinkedListForwardIterator& other) const {
    return m_pCurrent != other.m_pCurrent;
}
//Operador ==
template <typename Traits>
bool LinkedListForwardIterator<Traits>::operator==(const LinkedListForwardIterator& other) const{
    return m_pCurrent == other.m_pCurrent;
}



template <typename Traits>
class CLinkedList {
    using  value_type      = typename Traits::value_type;;
    using  Node            = NodeLinkedList<Traits>;
    using  forwardIterator = LinkedListForwardIterator<Traits>;
protected:
    Node *m_pRoot      = nullptr;
    Node *m_pLast      = nullptr;
    size_t m_nElements = 0;
    mutable std::recursive_mutex m_mtx;
public:
    CLinkedList(){}
    // TODO: Constructor copia
    // TODO: Move Constructor
    // TODO: Destructor seguro y virtual
    // TODO: Concurrencia (mutex)
    // TODO: Iterators begin() end()
    // TODO: Operadores de acceso []
    CLinkedList(const CLinkedList<Traits>& other);
    CLinkedList(CLinkedList<Traits>&& other) noexcept 
        : m_pRoot     (std::exchange (other.m_pRoot,nullptr)), 
          m_pLast     (std::exchange (other.m_pLast,nullptr)),
          m_nElements (std::exchange (other.m_nElements,0))
        {}
    virtual ~CLinkedList();

    CLinkedList& operator=(const CLinkedList<Traits>& other);
    CLinkedList& operator=(CLinkedList<Traits>&& other) noexcept;
    value_type& operator[](size_t index);
    const value_type& operator[](size_t index) const;
    void push_back(value_type const &val, ref_type ref);
    void Insert(const value_type &val, ref_type ref);
    size_t getSize(){  
        return m_nElements;  
    }
    forwardIterator begin(){ return forwardIterator(m_pRoot); }
    forwardIterator end(){ return forwardIterator(nullptr); }

    //Foreach
    template <typename ObjFunc, typename ...Args>
    void Foreach (ObjFunc of, Args ...args){
        ::Foreach(*this,of,args...);
    }
    //Firsthat
    template <typename ObjFunc, typename ...Args>
    auto FirstThat(ObjFunc of, Args ...args){
        return ::FirstThat(*this,of,args...);
    }
private:
    void InternalInsert(Node *&rParent, const value_type &val, ref_type ref);

    // TODO: Persistencia (write)
    //Operator <<
    friend ostream &operator<<(ostream &os, CLinkedList<Traits> &container){
        std::lock_guard<std::recursive_mutex> lock(container.m_mtx);
        os << "ClinkedList: size = " << container.m_nElements << endl;
        Node *currentNode = container.m_pRoot;
        while ( currentNode != nullptr){
            os << "[" << currentNode->GetValue() << "," << currentNode->GetRef() << "]";
            
            if ( currentNode->GetNext() != nullptr)
                os << "->";
            currentNode = currentNode->GetNext();
        }
        return os;
    }
    // TODO: Persistencia (read)
    //Operator >>
    friend istream &operator>>(istream& is, CLinkedList<Traits> &container){
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

};




//Destructor seguro
template <typename Traits>
CLinkedList<Traits>::~CLinkedList(){
    Node *currentNode = m_pRoot;
    while( currentNode != nullptr){
        Node* nextNode = currentNode->GetNext();
        delete currentNode;
        currentNode = nextNode;
    }
    m_nElements = 0;
    m_pRoot     = nullptr;
    m_pLast     = nullptr;
} 

//Constructor copia
template <typename Traits>
CLinkedList<Traits>::CLinkedList(const CLinkedList<Traits>& other){
    std::lock_guard<std::recursive_mutex> lock(other.m_mtx);
    Node *currentNode = other.m_pRoot;
    while (currentNode != nullptr){
        Node *pNewNode = new Node(currentNode->GetValue(), currentNode->GetRef());
        if (!m_pRoot)
            m_pRoot = pNewNode;
        else
            m_pLast->GetNextRef() = pNewNode;
        m_pLast = pNewNode;
        ++m_nElements;
        currentNode = currentNode->GetNext();
    }
}

//Operador de asignación
template <typename Traits>
CLinkedList<Traits>& CLinkedList<Traits>::operator=(const CLinkedList<Traits>& other){
    if( this == &other )
        return *this;
    std::lock(m_mtx, other.m_mtx);
    std::lock_guard<std::recursive_mutex> lock1(m_mtx, std::adopt_lock);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx, std::adopt_lock);
    Node *currentNode = m_pRoot;
    while ( currentNode != nullptr){
        Node* nextNode = currentNode->GetNext();
        delete currentNode;
        currentNode = nextNode;
    }

    m_pRoot     = nullptr;
    m_pLast     = nullptr;
    m_nElements = 0;

    currentNode = other.m_pRoot;
    while (currentNode != nullptr){
        Insert(currentNode->GetValue(),currentNode->GetRef());
        currentNode = currentNode->GetNext();
    }
    
    return *this;
}

//operador de asignación (robado)
template <typename Traits>
CLinkedList<Traits>& CLinkedList<Traits>::operator=(CLinkedList<Traits>&& other) noexcept{
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    std::lock_guard<std::recursive_mutex> lockOther(other.m_mtx);
    if( this != &other ){
        Node *currentNode = m_pRoot;
        while ( currentNode != nullptr){
            Node* nextNode = currentNode->GetNext();
            delete currentNode;
            currentNode = nextNode;
        }

        m_pRoot     = std::exchange (other.m_pRoot,nullptr);
        m_pLast     = std::exchange (other.m_pLast,nullptr);
        m_nElements = std::exchange (other.m_nElements,0);
    }
    return *this;
}

//operador [] asignar valor
template <typename Traits>
typename CLinkedList<Traits>::value_type& CLinkedList<Traits>::operator[](size_t index){
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    assert (index < m_nElements);
    Node *currentNode = m_pRoot;
    for (size_t i=0; i < index; ++i)
        currentNode = currentNode->GetNext();
    return currentNode->GetValueRef(); 
}

//operador [] mostrar valor
template <typename Traits>
const typename CLinkedList<Traits>::value_type& CLinkedList<Traits>::operator[](size_t index) const {
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    assert (index < m_nElements);
    Node *currentNode = m_pRoot;
    for (size_t i=0; i < index; ++i)
        currentNode = currentNode->GetNext();
    return currentNode->GetValueRef();
}


template <typename Traits>
void CLinkedList<Traits>::push_back(const value_type &val, ref_type ref){
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    Node *pNewNode = new Node(val, ref);
    if   ( !m_pRoot )
        m_pRoot = pNewNode;
    else
        m_pLast->GetNextRef()=pNewNode;
    m_pLast = pNewNode;
    ++m_nElements;
}

template <typename Traits>
void CLinkedList<Traits>::InternalInsert(Node *&rParent, const value_type &val, ref_type ref){
    // TODO: Agregar algo para el caso de circular
    if( !rParent || rParent->GetValue() > val ){
        Node *pNew = new Node(val, ref, rParent);
        rParent = pNew;
        ++m_nElements;
        return;
    }
    InternalInsert(rParent->GetNextRef(), val, ref);
}

template <typename Traits>
void CLinkedList<Traits>::Insert(const value_type &val, ref_type ref){
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    InternalInsert(m_pRoot, val, ref);
}

#endif // __LINKEDLIST_H__
