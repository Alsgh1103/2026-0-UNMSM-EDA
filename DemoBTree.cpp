    #include <iostream>
    #include <fstream>
    #include "containers/BTree.h"

    using namespace std;

    using TraitAsc  = CBTreeTraitsAsc<char>;
    using TraitDesc = CBTreeTraitsDesc<char>;
    using EntryAsc  = CBTreeEntry<TraitAsc>;
    using EntryDesc = CBTreeEntry<TraitDesc>;
    using TraitInt  = CBTreeTraitsAsc<int>;
    using EntryInt  = CBTreeEntry<TraitInt>;

    const char* keys1 = "D1XJ2xTg8zKL9AhijOPQcEowRSp0NbW567BUfCqrs4FdtYZakHIuvGV3eMylmn";
    const char* keys2 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const char* keys3 = "DYZakHIUwxVJ203ejOP9Qc8AdtuEop1XvTRghSNbW567BfiCqrs4FGMyzKLlmn";
    const char* keys4 = "0123456789";

    void PrintEntry  (EntryAsc& , int );
    bool EsMayorQueM (EntryAsc& , int );
    void DemoBTree(){

        cout << "----------DEMO B-TREE-------------" << endl;

        //Creación e inserción en BTree Ascendente
        cout << endl << "BTree 1 - Ascending" << endl;
        CBTree<TraitAsc> Tree1;
        for (int i = 0; keys1[i]; ++i)
            Tree1.Insert(keys1[i], i);
        cout << "Tamaño : " << Tree1.size()    << endl;
        cout << "Altura : " << Tree1.height()  << endl;
        cout << "Orden  : " << Tree1.GetOrder()<< endl;
        cout << Tree1 << endl;

        //Probando recorridos
        cout << endl << "Inorden" << endl;
        Tree1.Inorden(PrintEntry);
        cout << endl;

        cout << endl << "Preorden" << endl;
        Tree1.Preorden(PrintEntry);
        cout << endl;

        cout << endl << "Postorden" << endl;
        Tree1.Postorden(PrintEntry);
        cout << endl;

        //Probando Foreach
        cout << endl << "Foreach" << endl;
        CBTree<TraitInt> TreeForeach;
        for (int i = 1; i <= 10; ++i)
            TreeForeach.Insert(i, i*10);
        TreeForeach.Foreach(Add7<EntryInt>);
        cout << endl;

        cout << "Foreach Variadic (sumando 1+2+3=6 a cada elemento)" << endl;
        TreeForeach.Foreach(SumaVariadic<EntryInt, int, int, int>, 1, 2, 3);
        cout << endl;


        //Probando FirstThat
        cout << endl << "FirstThat (primer valor > 'M')" << endl;
        EntryAsc* pFound = Tree1.FirstThat(EsMayorQueM);
        if (pFound)
            cout << "Encontrado: " << pFound->key << " ObjID=" << pFound->ObjID << endl;
        else
            cout << "No encontrado" << endl;
        
        //Probando operator>>
        cout << endl << "Probando operator>>" << endl;
        ifstream file("BTree.txt");
        CBTree<TraitAsc> Tree2;
        file >> Tree2;
        file.close();
        cout << "Lectura de archivo" << endl;
        cout << Tree2 << endl;

        //Probando iterador forward begin() / end()
        cout << endl << "ForwardIterator" << endl;
        CBTree<TraitAsc> Tree3;
        for (int i = 0; keys2[i]; i++)
            Tree3.Insert(keys2[i], i);
        for (auto it = Tree3.begin(); it != Tree3.end(); ++it)
            cout << (*it).key << " ";
        cout << endl;

        //Probando iterador backward rbegin() / rend()
        cout << endl << "BackwardIterator" << endl;
        for (auto it = Tree3.rbegin(); it != Tree3.rend(); ++it)
            cout << (*it).key << " ";
        cout << endl;

        //Probando Search
        cout << endl << "Search" << endl;
        EntryAsc* pSearch = Tree1.Search('Z');
        if (pSearch)
            cout << "Encontrado: " << pSearch->key << " ObjID=" << pSearch->ObjID << endl;
        else
            cout << "No encontrado" << endl;

        pSearch = Tree1.Search('?');
        if (pSearch)
            cout << "Encontrado: " << pSearch->key << endl;
        else
            cout << "No encontrado '?'" << endl;

        //Probando Remove + iterador
        cout << endl << "Remove" << endl;
        CBTree<TraitAsc> Tree4;
        const char* small = "ABCDEFG";
        for (int i = 0; small[i]; i++)
            Tree4.Insert(small[i], i);

        cout << "Antes  : ";
        for (auto it = Tree4.begin(); it != Tree4.end(); ++it)
            cout << (*it).key << " ";
        cout << endl;

        Tree4.Remove('D', 3);
        cout << "Despues (sin D): ";
        for (auto it = Tree4.begin(); it != Tree4.end(); ++it)
            cout << (*it).key << " ";
        cout << endl;

        //Probando BTree Descendente
        cout << endl << "BTree Descendente" << endl;
        CBTree<TraitDesc> TreeDesc;
        for (int i = 0; keys2[i]; i++)
            TreeDesc.Insert(keys2[i], i);
        cout << TreeDesc << endl;

}

void PrintEntry  (EntryAsc& e, int level) { cout << e.key << " "; }
bool EsMayorQueM (EntryAsc& e, int level) { return e.key > 'M'; }
