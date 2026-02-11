#ifndef __STACK_H__
#define __STACK_H__

#include <iostream>
#include <mutex>
#include <cassert>
#include <utility>
#include "../general/types.h"
#include "../util.h"

using namespace std;

template  <typename T>
class NodeStack{
    using value_type = T;
private:
    value_type m_data;
    NodeStack* m_pNext = nullptr;
public:
    NodeStack(){};
    NodeStack(value_type _value, NodeStack* _next )
        : m_data(_value) , m_pNext(_next)
    {}
    value_type  GetValue    () const { return m_data;  }
    value_type &GetValueRef ()       { return m_data;  }
    NodeStack*   GetNext     () const { return m_pNext; }
    NodeStack*  &GetNextRef  ()       { return m_pNext; }
    
};

template <typename T>
class CStack{
    using NodeStack  = NodeStack<T>;
    using value_type = T;
private:
    NodeStack* m_pTop  = nullptr;
    size_t m_nElements = 0;
    mutable std::recursive_mutex m_mtx;

public:
    void Push(const value_type& value);
    void Pop();
    value_type& Top();
    const value_type& Top() const;
    CStack(){}
    //Copy constructor
    CStack(const CStack<T>& other);
    //Move constructor
    CStack(CStack<T>&& other) noexcept 
        : m_pTop      (std::exchange(other.m_pTop, nullptr)),
          m_nElements (std::exchange(other.m_nElements, 0))
        {}
    //Destructor Seguro
    virtual ~CStack();
    //Operador '=' copia
    CStack& operator=(const CStack<T>& other);
    //Operador '=' robado
    CStack& operator=(CStack<T>&& other) noexcept;

private:
    //Clear para limpiar pilas
    void Clear();
    void CopyFrom(const CStack<T>& other);
    friend ostream& operator<<(ostream& os , CStack<T>& stack){
        std::lock_guard<std::recursive_mutex> lock(stack.m_mtx);
        os << "CStack: size = " << stack.m_nElements << std::endl;
        NodeStack* p_CurrentNode = stack.m_pTop;
        while(p_CurrentNode){
            os<< "[" << p_CurrentNode->GetValue() << "]";
            p_CurrentNode = p_CurrentNode->GetNext();
            if (p_CurrentNode)
                os << "->";
        }
        return os;
    }
    friend istream& operator>>(istream& is , CStack<T>& stack){
        size_t nElements;
        is >> nElements;
        for (size_t i=0 ; i < nElements ; ++i){
            value_type val;
            is >> val;
            stack.Push(val);
        }
        return is;
    }
};

template <typename T>
void CStack<T>::Clear(){
    NodeStack* p_CurrentNode = m_pTop;
    while (p_CurrentNode){
        NodeStack* p_nextNode = p_CurrentNode->GetNext();
        delete p_CurrentNode;
        p_CurrentNode = p_nextNode;
    }
    m_pTop      = nullptr;
    m_nElements = 0; 
}

template <typename T>
void CStack<T>::CopyFrom(const CStack<T>& other){
    NodeStack* p_CurrentNode = other.m_pTop;
    NodeStack* p_LastCreated = nullptr;
    while(p_CurrentNode){
        NodeStack* pNew = new NodeStack(p_CurrentNode->GetValue(),nullptr);
        if(!m_pTop)
            m_pTop = pNew;
        else
            p_LastCreated->GetNextRef() = pNew;

        p_LastCreated = pNew;
        ++m_nElements;
        p_CurrentNode = p_CurrentNode->GetNext();
    }
}


//Inserción de elemento - Push
template <typename T>
void CStack<T>::Push(const typename CStack::value_type& value){
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    NodeStack* pNew = new NodeStack ( value, m_pTop );
    m_pTop             = pNew;
    ++m_nElements;
}

//Eliminación de último elemento - Pop
template <typename T>
void CStack<T>::Pop(){
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    assert(m_pTop);
    NodeStack* p_nextNode = m_pTop->GetNext();
    delete m_pTop;
    m_pTop = p_nextNode;
    --m_nElements;
}

//Mostrar último elemento con posibilidad de modificar
template <typename T>
typename CStack<T>::value_type& CStack<T>::Top(){
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    assert(m_pTop);
    return m_pTop->GetValueRef(); 
} 
//Solo leer último elemento de la pila
template <typename T>
const typename CStack<T>::value_type& CStack<T>::Top() const{
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    assert(m_pTop);
    return m_pTop->GetValue(); 
} 

// Copy constructor
template <typename T>
CStack<T>::CStack(const CStack<T>& other){
    std::lock_guard<std::recursive_mutex> lock(other.m_mtx);
    CopyFrom(other);
}

//Destructor seguro
template <typename T>
CStack<T>::~CStack(){
   Clear();
}

//Operador '=' - Copy  
template <typename T>
CStack<T>& CStack<T>::operator=(const CStack<T>& other){
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
CStack<T>& CStack<T>::operator=(CStack<T>&& other) noexcept{
    if (this == &other)
        return *this;
    std::lock_guard<std::recursive_mutex> lock1(m_mtx);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx);
    Clear();
    m_pTop      = std::exchange (other.m_pTop,nullptr);
    m_nElements = std::exchange (other.m_nElements,0);
    return *this;
}
#endif // __STACK_H__