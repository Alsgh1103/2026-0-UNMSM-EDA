#include <iostream>
#include <fstream>
#include "containers/doublecircularlist.h" 
#include "variadic-util.h"

using namespace std;

bool MayorQue30_DCLL(T1 &elem);

void DemoDoubleCircularList(){
    cout << " Lista Circular doble enlazada" << endl;
    CDoubleCircularLinkedList < AscendingTrait<T1> > l1;

    l1.Insert(20, 5);
    l1.Insert(30, 3);
    l1.Insert(40, 6);
    l1.Insert(10, 1);
    l1.Insert(8, 8);
    l1.Insert(50, 11);
    cout << l1 << endl;

    //PROBANDO CONSTRUCTOR COPIA
    CDoubleCircularLinkedList < AscendingTrait<T1> > l2 = l1;
    cout << l2 << endl;
    cout << endl;

    //PROBANDO OPERADOR '='
    CDoubleCircularLinkedList < AscendingTrait<T1> > l3;
    l3.Insert(50,6);
    l3.Insert(30,2); 
    cout << endl;
    cout << "Lista 3" << endl;
    cout << l3 << endl;
    cout << endl;
    l3 = l1;
    cout << "Lista 3 modificada" << endl;
    cout << l3 << endl;
    cout << endl;

    //MOVE CONSTRUCTOR
    cout << endl;
    cout << "MOVE CONSTRUCTOR - lista 4" << endl;
    CDoubleCircularLinkedList< AscendingTrait<T1> > l4 = std::move(l3);
    cout << l4 << endl;

    //OPERADOR '=' CON move
    cout << endl;
    cout << "Operador '=' en move - lista 5" << endl;
    CDoubleCircularLinkedList< AscendingTrait<T1> > l5;
    l5 = std::move(l2);
    cout << l5 << endl; 
    cout << endl;

    //Probando iteradores
    cout << "Probando iteradores "<< endl;
    cout << endl << "Forward" << endl;
    for(auto it = l5.begin(); it != l5.end() ; ++it)
        cout << *it << "; ";
    cout << endl;

    cout << endl << "Backward" << endl;
    auto itBack = l5.rbegin();
    size_t count = 0;
    size_t totalToShow = l5.getSize() * 2; 
    while(count < totalToShow) {
        cout << *itBack << "; ";
        ++itBack;
        ++count;
    }
    cout << "..." << endl << endl;

    //Probando Foreach
    cout << endl << endl << "Probando Foreach con Suma" << endl;
    cout << "Lista antes: " << l5 << endl;
    l5.Foreach(Suma<T1>, 10);
    cout << "Lista despues: " << l5 << endl;

    //Probando Firsthat
    auto iter = l5.FirstThat(&MayorQue30_DCLL);
    if(iter != l5.end())
    {   cout << "El primer elemento mayor a 30 es: " << *iter << endl;   }

    //Probando Istream
    cout << endl << "Probando operator>> input stream " << endl;
    cout << "Leyendo de archivo 'DoubleCircularList.txt' " << endl;

    ifstream list("DoubleCircularList.txt");
    CDoubleCircularLinkedList<AscendingTrait<T1>> l6;
    list >> l6;
    list.close();
    cout << "Lista leida: " << l6 << endl;
    
}

bool MayorQue30_DCLL(T1 &elem){
    return elem > 30;
}
