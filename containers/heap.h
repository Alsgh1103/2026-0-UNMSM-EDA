#ifndef __HEAP_H__
#define __HEAP_H__

#include <iostream>
#include <utility>
#include <mutex>
#include <cassert>
#include "../general/types.h"
#include "../util.h"


template <typename _T>
struct MinHeapTrait{
    using T = _T;
    static bool Compare(const T& parent, const T& child){
        return parent < child;
    }
};

template <typename _T>
struct MaxHeapTrait{
    using T = _T;
    static bool Compare(const T& parent, const T& child){
        return parent > child;
        }
};

template <typename Traits>
class CHeap{
    using value_type = typename Traits::T ;
private:
Size        m_capacity = 0;
value_type* m_data     = nullptr;
Size        m_last     = 0;


mutable     std::recursive_mutex  m_mtx;

public:
    CHeap(){}
    CHeap(Size capacity);
    //COPY CONSTRUCTOR
    CHeap(const CHeap<Traits>& other);
    //MOVE CONSRRUCTOR
    CHeap(CHeap<Traits>&& other) noexcept
        : m_capacity(std::exchange(other.m_capacity,0)), 
          m_data    (std::exchange(other.m_data,nullptr)),
          m_last    (std::exchange(other.m_last,0))
    {}
    // Size GetSize() { return m_last; }
    virtual ~CHeap();
    value_type Pop();
    void Push(const value_type& valor);
    void resize();
    CHeap<Traits>& operator=(const CHeap<Traits>& other);
    CHeap<Traits>& operator=(CHeap<Traits>&& other) noexcept;

    friend std::ostream &operator<<(std::ostream& os, const CHeap<Traits>& Heap){
        std::lock_guard<std::recursive_mutex> lock(Heap.m_mtx);
        os << "CHeap: size = " << Heap.m_last << std::endl;
        os << "[" ;
        for (Size i = 0; i < Heap.m_last ; ++i ){
            os << Heap.m_data[i];
            if (i != Heap.m_last-1)
                os << ",";
        }
        os << "]";
        return os;
    }
    friend std::istream &operator>>(std::istream& is, CHeap<Traits>& Heap){
        std::lock_guard<std::recursive_mutex> lock(Heap.m_mtx);
        Size size = 0;
        is >> size;
        value_type valor;
        for (Size i = 0; i < size ; ++i){
            is >> valor;
            Heap.Push(valor);
        }
        return is;
    }

private:
    void HeapifyUp();
    void HeapifyDown();
    void HeapifyUpRecursive(Size index);
    void HeapifyDownRecursive(Size index);

};

template <typename Traits>
CHeap<Traits>::CHeap(Size capacity)
    : m_capacity(capacity), 
      m_data(new value_type[capacity]),
      m_last(0)
{}

//Copy constructor
template <typename Traits>
CHeap<Traits>::CHeap(const CHeap<Traits>& other){
    std::lock_guard<std::recursive_mutex> lock(other.m_mtx);
    assert(other.m_data);
    m_capacity = other.m_capacity;
    m_data     = new value_type[m_capacity]; 
    m_last     = other.m_last;
    for (Size i = 0; i < m_last ; ++i){
        m_data[i] = other.m_data[i];
    }
}

//destructor
template <typename Traits>
CHeap<Traits>::~CHeap(){ 
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    delete[] m_data; 
}

//Insertar elemento - Push
template <typename Traits>
void CHeap<Traits>::Push(const value_type& value){
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    if (m_last == m_capacity)
        resize();
    m_data[m_last++] = value;
    HeapifyUp();
}

template <typename Traits>
void CHeap<Traits>::HeapifyUp(){
    HeapifyUpRecursive(m_last-1);
}
//Escalado en el Heap - Agregando último elemento
template <typename Traits>
void CHeap<Traits>::HeapifyUpRecursive(Size index){
    if (index == 0)
        return;
    Size parent = (index - 1) / 2;
    if(!Traits::Compare(m_data[parent],m_data[index])){
        value_type temp = std::move(m_data[parent]);
        m_data[parent]  = std::move(m_data[index]);
        m_data[index]   = std::move(temp);
        HeapifyUpRecursive(parent);
    }
}

//Obtener el último elemento y eliminarlo
template <typename Traits>
typename CHeap<Traits>::value_type CHeap<Traits>::Pop(){
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    assert (m_last != 0);
    value_type value = std::move(m_data[0]);
    m_data[0]        = std::move(m_data[--m_last]);
    HeapifyDown();
    return value;
}

//Escalado en el Heap - Eliminando el menor elemento
template <typename Traits>
void CHeap<Traits>::HeapifyDown(){
    HeapifyDownRecursive(0);
}

template <typename Traits>
void CHeap<Traits>::HeapifyDownRecursive(Size index){
    Size hijo_izq = 2 * index + 1;
    Size hijo_der = 2 * index + 2;

    if (hijo_izq >= m_last)
        return;
    
    Size ganador = hijo_izq;
    if (hijo_der < m_last) {
        if (!Traits::Compare(m_data[hijo_izq], m_data[hijo_der]))
            ganador = hijo_der;
    }
    
    if (Traits::Compare(m_data[index], m_data[ganador]))
        return;
    
    value_type temp = std::move(m_data[index]);
    m_data[index]   = std::move(m_data[ganador]);
    m_data[ganador] = std::move(temp);

    HeapifyDownRecursive(ganador);
}

//Incrementar espacio
template <typename Traits>
void CHeap<Traits>::resize(){ 
    Size new_capacity = 0;
    if (m_capacity == 0)
        new_capacity = 5;
    else
        new_capacity = 2*m_capacity;
    value_type* new_data = new value_type[new_capacity];
    for (Size i = 0; i < m_last ; ++i)
        new_data[i] = m_data[i];
    delete[] m_data;
    m_data     = new_data;
    m_capacity = new_capacity; 
}

//operador '=' para el copia
template <typename Traits>
CHeap<Traits>& CHeap<Traits>::operator=(const CHeap<Traits>& other){
    if (this == &other)
        return *this;
    std::lock(m_mtx,other.m_mtx);
    std::lock_guard<std::recursive_mutex> lock1(m_mtx, std::adopt_lock);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx, std::adopt_lock);
    delete[] m_data;
    m_capacity = other.m_capacity;
    m_data = new value_type[m_capacity]; 
    m_last = other.m_last;
    for (Size i = 0; i < m_last ; ++i){
        m_data[i] = other.m_data[i];
    }
    return *this;
}
//operador '=' para el move
template <typename Traits>
CHeap<Traits>& CHeap<Traits>::operator=(CHeap<Traits>&& other) noexcept{
    if (this == &other)
        return *this;
    std::lock(m_mtx,other.m_mtx);
    std::lock_guard<std::recursive_mutex> lock1(m_mtx, std::adopt_lock);
    std::lock_guard<std::recursive_mutex> lock2(other.m_mtx, std::adopt_lock);
    delete[] m_data;
    m_capacity = std::exchange(other.m_capacity,0);
    m_data     = std::exchange(other.m_data,nullptr);
    m_last     = std::exchange(other.m_last,0);
    
    return *this;
}


#endif // __HEAP_H__