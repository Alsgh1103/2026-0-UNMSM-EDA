#include <iostream>
#include <fstream>
#include <string>
#include "containers/heap.h"

using namespace std;

void DemoHeap(){
    
    //Creación de Minheap
    cout << "MinHeap 1" << endl;
    CHeap<MinHeapTrait<T1>> MinHeap1(2);
    //Uso Pop para insertar elementos y ordenar
    MinHeap1.Push(3);
    MinHeap1.Push(4);
    MinHeap1.Push(0);
    MinHeap1.Push(1);

    cout << MinHeap1 << endl;

    //Creación de MaxHeap
    cout << endl << "MaxHeap" << endl;
    CHeap<MaxHeapTrait<T1>> MaxHeap1(1);
    MaxHeap1.Push(12);
    MaxHeap1.Push(10);
    MaxHeap1.Push(7);
    MaxHeap1.Push(24);
    cout << MaxHeap1 << endl;

    //Probando constructor copia
    cout << endl << "Copy constructor" << endl;
    cout << "MinHeap 2" << endl;
    CHeap<MinHeapTrait<T1>> Minheap2 = MinHeap1;
    cout << Minheap2 << endl;

    // Probando operador asignación '=' en copia
    CHeap<MinHeapTrait<T1>> MinHeap3(2);
    MinHeap3.Push(50);
    MinHeap3.Push(34);

    cout << endl << "MinHeap 3 antes de copiar" << endl << MinHeap3 << endl;

    MinHeap3 = MinHeap1;
    cout << "MinHeap 3 despues de copiar de MinHeap 1 "<< endl << MinHeap3 << endl; 

    //Probando Move Constructor
    cout << endl << "MinHeap 4 robando datos de MinHeap 3 " << endl;
    CHeap<MinHeapTrait<T1>> MinHeap4 = std::move(MinHeap3);
    cout << "Quitando datos de MinHeap 3" << endl;
    cout << MinHeap4 << endl;
    cout << "MinHeap 3 actual" << endl;
    cout << MinHeap3 << endl;
    
    //Probando operador asignación '=' en move
    cout << endl << "MinHeap 3 robando datos de MinHeap 1" << endl;
    MinHeap3 = std::move(MinHeap1);
    cout << MinHeap3 << endl;
    cout << "MinHeap 1 actual " << endl;
    cout << MinHeap1 << endl << endl;

    //Probando Pop
    cout << "Extrayendo los menores elementos en MinHeap 3" << endl;
    cout << "Menor elemento 1 : " << MinHeap3.Pop() << endl;
    cout << "Menor elemento 2 : " << MinHeap3.Pop() << endl;
    cout << "MinHeap 3 despues de Pop" << endl;
    cout << MinHeap3 << endl << endl;

    //Probando istream - operator>>
    ifstream Heap("Heap.txt");
    CHeap<MinHeapTrait<T1>> MinHeap5;
    Heap >> MinHeap5;
    Heap.close();
    cout << "Lectura de archivo y ordenado en un MinHeap" << endl;
    cout << MinHeap5 << endl;
    
    Heap.open("Heap.txt");
    CHeap<MaxHeapTrait<T1>> MaxHeap2;
    Heap >> MaxHeap2;
    Heap.close();
    cout << "Lectura de archivo y ordenado en un MaxHeap" << endl;
    cout << MaxHeap2 << endl << endl;

    //Probando con Strings
    cout << "Probando Heap con Strings (MaxHeap)" << endl;
    CHeap<MaxHeapTrait<string>> StringHeap(5);
    StringHeap.Push("Zanahoria");
    StringHeap.Push("Arbol");
    StringHeap.Push("Casa");
    StringHeap.Push("Dedo");
    StringHeap.Push("Bebida");
    cout << "MaxHeap antes de la extraccion de elementos" << endl;    
    cout << StringHeap << endl << endl;
    
    cout << "Extrayendo del StringHeap: " << endl;
    cout << "1: " << StringHeap.Pop() << endl;
    cout << "2: " << StringHeap.Pop() << endl;
    cout << "Resto: " << StringHeap << endl;
}