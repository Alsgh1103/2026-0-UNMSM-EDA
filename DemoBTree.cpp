#include <time.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "containers/BTree.h"

using TraitAsc  = CBTreeTraitsAsc<char>;
using TraitDesc = CBTreeTraitsDesc<char>;
using EntryAsc  = CBTreeEntry<TraitAsc>;
using EntryDesc = CBTreeEntry<TraitDesc>;

const char* keys1 = "D1XJ2xTg8zKL9AhijOPQcEowRSp0NbW567BUfCqrs4FdtYZakHIuvGV3eMylmn";
const char* keys2 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char* keys3 = "DYZakHIUwxVJ203ejOP9Qc8AdtuEop1XvTRghSNbW567BfiCqrs4FGMyzKLlmn";

