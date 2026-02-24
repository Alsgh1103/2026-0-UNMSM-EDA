// btree.h

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <mutex>
#include <vector>
#include <utility>
#include "BTreePage.h"
#include "BTreeIterator.h"
#include "../variadic-util.h"

template <typename Traits>
class CBTree
{
       using BTPage     = CBTreePage<Traits>;
       using ObjectInfo = CBTreeEntry<Traits>;

public:
       using value_type        = typename Traits::value_type;
       using ObjIDType         = typename Traits::ObjIDType;
       using ForwardIterator          = CBTreeForwardIterator<Traits>;
       using BackwardIterator  = CBTreeBackwardIterator<Traits>;

public:
        CBTree(int order = Traits::order, bool unique = true);
        virtual ~CBTree() = default;

        bool         Insert(const value_type& key, ObjIDType ObjID);
        bool         Remove(const value_type& key, ObjIDType ObjID);
        ObjectInfo*  Search(const value_type& key);

        long size()     const { return m_NumKeys; }
        long height()   const { return m_Height;  }
        long GetOrder() const { return m_Order;   }

        void Print(ostream& os);

        template<typename Func, typename... Args>
        void Inorden(Func fn, Args... args);

        template<typename Func, typename... Args>
        void Preorden(Func fn, Args... args);

        template<typename Func, typename... Args>
        void Postorden(Func fn, Args... args);

        template<typename Func, typename... Args>
        void Foreach(Func fn, Args... args);

        template<typename Func, typename... Args>
        ObjectInfo* FirstThat(Func fn, Args... args);

        //Iteradores
        ForwardIterator begin() {
            std::vector<ObjectInfo*> entries;
            FillEntries(&m_Root, entries);
            return ForwardIterator(entries);
        }
        ForwardIterator end() { return ForwardIterator(); }

        BackwardIterator rbegin() {
            std::vector<ObjectInfo*> entries;
            FillEntries(&m_Root, entries);
            return BackwardIterator(entries);
        }
        BackwardIterator rend() { return BackwardIterator(); }

        friend ostream& operator<<(ostream& os, CBTree<Traits>& BTree) {
                BTree.Print(os);
                return os;
        }

        friend istream& operator>>(istream& is, CBTree<Traits>& BTree) {
                std::lock_guard<std::recursive_mutex> lock(BTree.m_mtx);
                int nElements;
                is >> nElements;
                for (auto i = 0; i < nElements ; ++i){
                        value_type key;
                        ObjIDType ID;
                        is >> key >> ID;
                        BTree.Insert(key, ID);
                }
                return is;
        }

protected:
        BTPage  m_Root;
        long    m_NumKeys;
        bool    m_Unique;
        int     m_Order;
        int     m_Height;
        mutable std::recursive_mutex m_mtx;

private:
        //LLENAR VECTOR EN INORDEN
        void FillEntries(BTPage* pPage, std::vector<ObjectInfo*>& entries){
            if (!pPage || pPage->m_KeyCount == 0) return;
            for (int i = 0; i < pPage->m_KeyCount; i++){
                if (pPage->m_SubPages[i])
                    FillEntries(pPage->m_SubPages[i], entries);
                entries.push_back(&pPage->m_Keys[i]);
            }
            if (pPage->m_SubPages[pPage->m_KeyCount])
                FillEntries(pPage->m_SubPages[pPage->m_KeyCount], entries);
        }
};

template <typename Entry, typename ...Args>
void SumaVariadic(Entry& elem, int level, Args ...args){
        elem.key += Test(args...);
        std::cout << (int)elem.key << " ";
}

template <typename T>
void Add7(T& e, int level) { 
    e.key += 7; 
    std::cout << (int)e.key << " "; 
}

template <typename Traits>
CBTree<Traits>::CBTree(int order, bool unique)
        : m_Root(2 * order + 1, unique),
          m_NumKeys(0),
          m_Unique(unique),
          m_Order(order),
          m_Height(1)
{
        m_Root.SetMaxKeysForChilds(order);
}

template <typename Traits>
bool CBTree<Traits>::Insert(const value_type& key, ObjIDType ObjID)
{
        std::lock_guard<std::recursive_mutex> lock(m_mtx);
        bt_ErrorCode error = m_Root.Insert(key, ObjID);
        if( error == bt_duplicate ) 
              return false;
        m_NumKeys++;
        if( error == bt_overflow ) {
                m_Root.SplitRoot();
                m_Height++;
        }
        return true;
}

template <typename Traits>
bool CBTree<Traits>::Remove(const value_type& key, ObjIDType ObjID)
{
        std::lock_guard<std::recursive_mutex> lock(m_mtx);
        bt_ErrorCode error = m_Root.Remove(key, ObjID);
       if( error == bt_duplicate || error == bt_nofound )
               return false;
        m_NumKeys--;

       if( error == bt_rootmerged )
               m_Height--;
        return true;
}

template <typename Traits>
typename CBTree<Traits>::ObjectInfo*
CBTree<Traits>::Search(const value_type& key)
{
        std::lock_guard<std::recursive_mutex> lock(m_mtx);
        return m_Root.Search(key);
}

template <typename Traits>
void CBTree<Traits>::Print(ostream& os)
{
        std::lock_guard<std::recursive_mutex> lock(m_mtx);
        m_Root.Print(os);
}

template <typename Traits>
template <typename Func, typename... Args>
void CBTree<Traits>::Inorden(Func fn, Args... args){
       std::lock_guard<std::recursive_mutex> lock(m_mtx);
       m_Root.Inorden(fn, 0, args...);
}

template <typename Traits>
template <typename Func, typename... Args>
void CBTree<Traits>::Preorden(Func fn, Args... args)
{
        std::lock_guard<std::recursive_mutex> lock(m_mtx);
        m_Root.Preorden(fn, 0, args...);
}

template <typename Traits>
template <typename Func, typename... Args>
void CBTree<Traits>::Postorden(Func fn, Args... args){
       std::lock_guard<std::recursive_mutex> lock(m_mtx);
       m_Root.Postorden(fn, 0, args...);
}

template <typename Traits>
template <typename Func, typename... Args>
void CBTree<Traits>::Foreach(Func fn, Args... args)
{
        std::lock_guard<std::recursive_mutex> lock(m_mtx);
        m_Root.Inorden(fn, 0, args...);
}

template <typename Traits>
template <typename Func, typename... Args>
typename CBTree<Traits>::ObjectInfo*
CBTree<Traits>::FirstThat(Func fn, Args... args)
{
        std::lock_guard<std::recursive_mutex> lock(m_mtx);
        return m_Root.FirstThat(fn, 0, args...);
}

void DemoBTree();

#endif