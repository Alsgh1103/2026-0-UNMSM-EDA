// BTreeIterator.h
#ifndef BTREEITERATOR_H
#define BTREEITERATOR_H

#include <vector>
#include "BTreePage.h"

//Forward declarations
template<typename Traits> class CBTreeForwardIterator;
template<typename Traits> class CBTreeBackwardIterator;

//Forward Iterator
template<typename Traits>
class CBTreeForwardIterator{
    using ForwardIterator = CBTreeForwardIterator<Traits>;
    using ObjectInfo      = CBTreeEntry<Traits>;
public:
    using value_type = typename Traits::value_type;
private:
    std::vector<ObjectInfo*> m_entries;
    int m_pos;
public:
    CBTreeForwardIterator() : m_pos(0) {}
    CBTreeForwardIterator(std::vector<ObjectInfo*> entries)
        : m_entries(entries), m_pos(0) {}
    ObjectInfo& operator*();
    bool operator!=(const ForwardIterator& other) const;
    bool operator==(const ForwardIterator& other) const;
    ForwardIterator& operator++();
};

//Funciones ForwardIterator
template<typename Traits>
typename CBTreeForwardIterator<Traits>::ObjectInfo&
CBTreeForwardIterator<Traits>::operator*(){
    return *m_entries[m_pos];
}

template<typename Traits>
CBTreeForwardIterator<Traits>& CBTreeForwardIterator<Traits>::operator++(){
    m_pos++;
    return *this;
}

template<typename Traits>
bool CBTreeForwardIterator<Traits>::operator!=(const CBTreeForwardIterator<Traits>& other) const {
    bool thisEnd  = m_pos >= (int)m_entries.size();
    bool otherEnd = other.m_pos >= (int)other.m_entries.size();
    if (thisEnd && otherEnd) return false;
    if (thisEnd || otherEnd) return true;
    return m_pos != other.m_pos;
}

template<typename Traits>
bool CBTreeForwardIterator<Traits>::operator==(const CBTreeForwardIterator<Traits>& other) const {
    return !(*this != other);
}

//Backward Iterator
template<typename Traits>
class CBTreeBackwardIterator{
    using BackwardIterator = CBTreeBackwardIterator<Traits>;
    using ObjectInfo       = CBTreeEntry<Traits>;
public:
    using value_type = typename Traits::value_type;
private:
    std::vector<ObjectInfo*> m_entries;
    int m_pos;
public:
    CBTreeBackwardIterator() : m_pos(-1) {}
    CBTreeBackwardIterator(std::vector<ObjectInfo*> entries)
        : m_entries(entries), m_pos((int)entries.size() - 1) {}
    ObjectInfo& operator*();
    bool operator!=(const BackwardIterator& other) const;
    bool operator==(const BackwardIterator& other) const;
    BackwardIterator& operator++();
};

//Funciones BackwardIterator
template<typename Traits>
typename CBTreeBackwardIterator<Traits>::ObjectInfo&
CBTreeBackwardIterator<Traits>::operator*(){
    return *m_entries[m_pos];
}

template<typename Traits>
CBTreeBackwardIterator<Traits>& CBTreeBackwardIterator<Traits>::operator++(){
    m_pos--;
    return *this;
}

template<typename Traits>
bool CBTreeBackwardIterator<Traits>::operator!=(const CBTreeBackwardIterator<Traits>& other) const {
    bool thisEnd  = m_pos < 0;
    bool otherEnd = other.m_pos < 0;
    if (thisEnd && otherEnd) return false;
    if (thisEnd || otherEnd) return true;
    return m_pos != other.m_pos;
}

template<typename Traits>
bool CBTreeBackwardIterator<Traits>::operator==(const CBTreeBackwardIterator<Traits>& other) const {
    return !(*this != other);
}

#endif
