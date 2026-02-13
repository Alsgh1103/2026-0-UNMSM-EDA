#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <iostream>
#include <mutex>
#include <cassert>
#include <utility>
#include "../general/types.h"
#include "../util.h"

using namespace std;

template  <typename T>
class NodeQueue{
    using value_type = T;
private:
    value_type m_data;
    NodeQueue* m_pNext = nullptr;
public:
    NodeQueue(){};
    NodeQueue(value_type _value, NodeQueue* _next )
        : m_data(_value) , m_pNext(_next)
    {}
    value_type  GetValue    () const { return m_data;  }
    value_type &GetValueRef ()       { return m_data;  }
    NodeQueue*   GetNext     () const { return m_pNext; }
    NodeQueue*  &GetNextRef  ()       { return m_pNext; }
    
};

template <typename T>
class CQueue{
    using NodeQueue  = NodeQueue<T>;
    using value_type = T;
private:
    NodeQueue* m_pFront = nullptr;
    NodeQueue* m_pRear  = nullptr;
    size_t m_nElements = 0;
    mutable std::recursive_mutex m_mtx;

public:
    void Push(const value_type& value);
    value_type Pop();
    CQueue(){}
    //Copy constructor
    CQueue(const CQueue<T>& other);
    //Move constructor
    CQueue(CQueue<T>&& other) noexcept 
        : m_pFront    (std::exchange(other.m_pFront, nullptr)),
          m_pRear     (std::exchange(other.m_pRear, nullptr)),
          m_nElements (std::exchange(other.m_nElements, 0))
        {}
    //Destructor Seguro
    virtual ~CQueue();
    //Operador '=' copia
    CQueue& operator=(const CQueue<T>& other);
    //Operador '=' robado
    CQueue& operator=(CQueue<T>&& other) noexcept;

private:
    //Clear para limpiar colas
    void Clear();
    void CopyFrom(const CQueue<T>& other);
    friend ostream& operator<<(ostream& os , CQueue<T>& queue){
        std::lock_guard<std::recursive_mutex> lock(queue.m_mtx);
        os << "CQueue: size = " << queue.m_nElements << std::endl;
        NodeQueue* p_CurrentNode = queue.m_pFront;
        while(p_CurrentNode){
            os<< "[" << p_CurrentNode->GetValue() << "]";
            p_CurrentNode = p_CurrentNode->GetNext();
            if (p_CurrentNode)
                os << "->";
        }
        return os;
    }
    friend istream& operator>>(istream& is , CQueue<T>& queue){
        size_t nElements;
        is >> nElements;
        for (size_t i=0 ; i < nElements ; ++i){
            value_type val;
            is >> val;
            queue.Push(val);
        }
        return is;
    }
};

template <typename T>
void CQueue<T>::Clear(){
    NodeQueue* p_CurrentNode = m_pFront;
    while (p_CurrentNode){
        NodeQueue* p_nextNode = p_CurrentNode->GetNext();
        delete p_CurrentNode;
        p_CurrentNode = p_nextNode;
    }
    m_pFront    = nullptr;
    m_pRear     = nullptr;
    m_nElements = 0; 
}

template <typename T>
void CQueue<T>::CopyFrom(const CQueue<T>& other){
    NodeQueue* p_CurrentNode = other.m_pFront;
    while(p_CurrentNode){
        NodeQueue* pNew = new NodeQueue(p_CurrentNode->GetValue(), nullptr);
        if(!m_pFront)
            m_pFront = pNew;
        else
            m_pRear->GetNextRef() = pNew;

        m_pRear = pNew;
        ++m_nElements;
        p_CurrentNode = p_CurrentNode->GetNext();
    }
}


//agregar al final de la cola
template <typename T>
void CQueue<T>::Push(const typename CQueue::value_type& value){
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    NodeQueue* pNew = new NodeQueue ( value, nullptr );
    if(!m_pFront){
        m_pFront = pNew;
        m_pRear  = pNew;
    } else {
        m_pRear->GetNextRef() = pNew;
        m_pRear = pNew;
    }
    ++m_nElements;
}

//quitar del inicio de la cola
template <typename T>
typename CQueue<T>::value_type CQueue<T>::Pop(){
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    assert(m_pFront);
    NodeQueue* p_nextNode = m_pFront->GetNext();
    value_type valor = std::move(m_pFront->GetValueRef());
    delete m_pFront;
    m_pFront = p_nextNode;
    if(!m_pFront)
        m_pRear = nullptr;
    --m_nElements;
    return valor;
}

// Copy constructor
template <typename T>
CQueue<T>::CQueue(const CQueue<T>& other){
    std::lock_guard<std::recursive_mutex> lock(other.m_mtx);
    CopyFrom(other);
}

//Destructor seguro
template <typename T>
CQueue<T>::~CQueue(){
   Clear();
}

//Operador '=' - Copy  
template <typename T>
CQueue<T>& CQueue<T>::operator=(const CQueue<T>& other){
    if (this == &other)
        return *this;
    std::lock(m_mtx,other.m_mtx);
    std::lock_guard<std::recursive_mutex> lock1(m_mtx, std::adopt_lock);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx, std::adopt_lock);
    Clear();
    CopyFrom(other);
    return *this;
}

//Operador '=' - Move 
template <typename T>
CQueue<T>& CQueue<T>::operator=(CQueue<T>&& other) noexcept{
    if (this == &other)
        return *this;
    std::lock_guard<std::recursive_mutex> lock1(m_mtx);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx);
    Clear();
    m_pFront    = std::exchange (other.m_pFront,nullptr);
    m_pRear     = std::exchange (other.m_pRear,nullptr);
    m_nElements = std::exchange (other.m_nElements,0);
    return *this;
}
#endif // __QUEUE_H__