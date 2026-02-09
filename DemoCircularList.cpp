#include <iostream>
#include <fstream>
#include "containers/circularlist.h" 
#include "variadic-util.h"

using namespace std;

bool MayorQue30Circular(T1 &elem);

void DemoCircularList(){
    cout << "Lista Circular" << endl;
    CCircularList < AscendingTrait<T1> > l1;

    l1.Insert(20, 5);
    l1.Insert(30, 3);
    l1.Insert(40, 6);
    l1.Insert(10, 1);
    l1.Insert(8, 8);
    l1.Insert(50, 11);
    cout << l1 << endl;

    //PROBANDO CONSTRUCTOR COPIA
    cout << endl << "Constructor copia" << endl;
    CCircularList < AscendingTrait<T1> > l2 = l1;
    cout << l2 << endl;
    cout << endl;

    //PROBANDO OPERADOR '='
    cout << endl << "Operator '='" << endl;
    CCircularList < AscendingTrait<T1> > l3;
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
    CCircularList< AscendingTrait<T1> > l4 = move(l3);
    cout << l4 << endl;

    //OPERADOR '=' CON move
    cout << endl;
    cout << "Operador '=' en move - lista 5" << endl;
    CCircularList< AscendingTrait<T1> > l5;
    l5 = move(l2);
    cout << l5 << endl; 
    cout << endl;

    //Probando operador '[]'
    cout << "Operador [] - lista 5" << endl;
    l5[0] = 15;
    cout << l5 << endl ;

    //Probando iteradores
    cout << endl << "Iteradores " << endl;
    for(auto it = l5.begin(); it != l5.end() ; ++it)
        cout << *it << "; ";

    //Probando Foreach con suma 
    cout << endl << endl << "Probando Foreach con Suma" << endl;
    cout << "Lista antes: " << l5 << endl;
    l5.Foreach(Suma<T1>, 10);
    cout << "Lista despues: " << l5 << endl;

    //Probando Firsthat 
    cout << endl << "Probando FirstThat" << endl;
    auto iter = l5.FirstThat(&MayorQue30Circular);
    if(iter != l5.end())
    {   cout << "El primer elemento mayor a 30 es: " << *iter << endl;   }
    
    //Probando Istream con ifstream
    cout << endl << "Probando operator>> " << endl;
    ifstream list("CircularList.txt");
    CCircularList<AscendingTrait<T1>> l6;
    list >> l6;
    list.close();
    cout << "Lista leida: " << l6 << endl;

}

bool MayorQue30Circular(T1 &elem){
    return elem > 30;
}
