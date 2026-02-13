#include <iostream>
#include <fstream>
#include "containers/stack.h"
#include "variadic-util.h"

using namespace std;

void DemoStack(){
    CStack<T1> S1;

    S1.Push(4);
    S1.Push(1);
    S1.Push(8);
    S1.Push(3);
    cout << "Stack 1 " << endl << S1 << endl;
    cout << endl << " Stack 1 - Pop" << endl;
    T1 valor = S1.Pop();
    cout << "Ultimo valor en la pila = " << valor << endl;
    cout << "Despues de Pop: " << S1 << endl;

    ifstream inf("stack.txt");
    CStack<T1> S2;
    inf >> S2;
    inf.close();
    cout << endl << "Stack 2 - leida desde archivo:" << endl;
    cout << S2 << endl;



    cout << endl << "PROBANDO CONSTRUCTOR COPIA" << endl;
    CStack<T1> S3 = S1;
    cout << S3 << endl;
    cout << endl;

    cout << "PROBANDO OPERADOR '='" << endl;
    CStack<T1> S4;
    S4.Push(99);
    cout << "Stack 4" << endl;
    cout << S4 << endl;
    cout << endl;
    S4 = S1;
    cout << "Stack 4 modificado" << endl;
    cout << S4 << endl;
    cout << endl;

    cout << endl;
    cout << "MOVE CONSTRUCTOR - stack 5" << endl;
    CStack<T1> S5 = std::move(S3);
    cout << S5 << endl;

    cout << endl;
    cout << "Operador '=' en move - stack 6" << endl;
    CStack<T1> S6;
    S6 = std::move(S4);
    cout << S6 << endl;
}