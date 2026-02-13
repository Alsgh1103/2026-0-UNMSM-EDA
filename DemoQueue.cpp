#include <iostream>
#include <fstream>
#include "containers/queue.h"
#include "variadic-util.h"

using namespace std;

void DemoQueue(){
    CQueue<T1> Q1;

    Q1.Push(4);
    Q1.Push(1);
    Q1.Push(8);
    Q1.Push(3);
    cout << "Cola 1 " << endl;
    cout << Q1 << endl;

    cout << endl << "Pop - Cola 1 " << endl;
    cout << "Elemento extraido: " << Q1.Pop() << endl;
    cout << "Despues de Pop: " << Q1 << endl;
    cout << "Siguiente elemento extraido: " << Q1.Pop() << endl;
    cout << "Cola 1 - estado final " << Q1 << endl;

    ifstream inf("queue.txt");
    CQueue<T1> Q2;
    inf >> Q2;
    inf.close();
    cout << endl;
    cout << "Cola leida desde archivo:" << endl;
    cout << Q2 << endl;

    cout << endl << "PROBANDO CONSTRUCTOR COPIA" << endl;
    CQueue<T1> Q3 = Q1;
    cout << Q3 << endl;
    cout << endl;

    cout << "PROBANDO OPERADOR '='" << endl;
    CQueue<T1> Q4;
    Q4.Push(99);
    cout << endl;
    cout << "Cola 4" << endl;
    cout << Q4 << endl;
    cout << endl;
    Q4 = Q1;
    cout << "Cola 4 modificada" << endl;
    cout << Q4 << endl;

    cout << endl;
    cout << "MOVE CONSTRUCTOR - cola 5" << endl;
    CQueue<T1> Q5 = std::move(Q3);
    cout << Q5 << endl;

    cout << endl;
    cout << "Operador '=' en move - cola 6" << endl;
    CQueue<T1> Q6;
    Q6 = std::move(Q4);
    cout << Q6 << endl;
}
