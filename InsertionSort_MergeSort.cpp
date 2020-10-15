// Tema: insertion sort a merge sort

#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

// ------------------------------------------------------------------------------------------------
// Insertion sort
// ------------------------------------------------------------------------------------------------

// Funkcia usporiada pole 'data' algoritmom insertion sort.
// Parametre:
//  'data' - pole, ktoreho obsah funkcia usporiada
//  'length' - pocet prvkov pola 'data'
void insertionSort(int *data, const size_t length) {
    for(size_t i = 1; i < length; ++i ) { // 'i' oznacuje hranicu usporiadanej casti pola
        int current = data[i];
        size_t j = i;
        while(j > 0 && data[j-1] > current) {
            data[j] = data[j-1];
            --j;
        }
        data[j] = current;
    }
}

//------------------------------------------------------------------------------
// Merge sort
//------------------------------------------------------------------------------

// Funkcia kombinuje dve usporiadane casti v poli 'input' do jednej usporiadanej casti v poli 'output'
// Parametre:
//  'output' - vystupne pole, ktoreho cast output[low]...output[high-1] bude po vykonani funkcie usporiadana
//             (vratane output[low] aj output [high-1])
//  'input'  - vstupne pole, ktoreho casti input[low]...input[middle-1] a input[middle]...input[high-1] musia byt pri volani funkcie usporiadane!
//  'low'    - index 1. prvku lavej usporiadavanej casti pola 'input'
//  'middle' - index 1. prvku pravej usporiadanej casti pola 'input' a zaroven index prvku za poslednym prvkom lavej usporiadanej casti
//  'high'   - index za poslednym prvkom pravej usporiadanej casti pola 'input'
void merge(int* output, const int* input, const size_t low, const size_t middle, const size_t high)
{
    size_t first = low;     // index prvku v lavej  casti vstupneho pola, ktory sa bude porovnavat
    size_t second = middle; // index prvku v pravej casti vstupneho pola, ktory sa bude porovnavat
    size_t out = low;       // index do vystupneho pola kam sa bude kopirovat mensi z prvkov input[first] a input[second]

    // do casti vystupneho pola sa skopiruju prvky s casti vstupneho pola tak, aby boli usporiadane
    while (first < middle && second < high) {
        if (input[first] <= input[second]) { // stabilne triedenie
            output[out] = input[first];
            ++first;
        }
        else {
            output[out] = input[second];
            ++second;
        }
        ++out;
    }
    while (first < middle) {
        output[out] = input[first];
        ++first;
        ++out;
    }
    while (second < high) {
        output[out] = input[second];
        ++second;
        ++out;
    }
}

// Funkcia usporiada cast pola algoritmom merge sort (verzia top-down), vratane prvkov s indexami 'low' a 'high-1'.
// Pri volani funkcie musi byt obsah poli 'arrayA' a 'arrayB' rovnaky!
// Usporiadane prvky budu po vykonani funkcie v 'arrayA'.
// Parametre:
//  'arrayA' - pole, ktoreho cast arrayA[low]...arrayA[high-1] funkcia usporiada. Po vykonani funkcie bude toto pole usporiadane
//  'arrayB' - vstupne pole. Po vykonani toto pole nebude usporiadane, ale poradie jeho prvkov bude pravdepodobne zmenene
//  'low'    - index 1. prvku casti pola, ktoru funkcia usporiada
//  'high'   - index za poslednym prvkom casti pola, ktoru funkcia usporiada
void mergeSortRecursive(int* arrayA, int* arrayB, const size_t low, const size_t high)
{
    // ak cast pola obsahuje maximalne jeden prvok, tak je tato cast pola usporiadana
    if (low + 1 >= high) {
        return;
    }

    int middle = (high + low) / 2; // index prvku, ktory je v polovici usporiadavanej casti pola

    // V rekurzivnych volaniach usporiadame kazdu polovicu samostatne.
    // Na zaciatku vykonavania funkcie musia byt obidve polia rovnake, pretoze nevieme v akej hlbke sa rekurzia prestane vnarat a zacne usporiadavat prvky
    mergeSortRecursive(arrayB, arrayA, low, middle);
    mergeSortRecursive(arrayB, arrayA, middle, high);

    // usporiadane polovice v 'arrayB' usporiadame do jedneho celku v 'arrayA'
    merge(arrayA, arrayB, low, middle, high);
}

// Funkcia usporiada pole 'array' algoritmom merge sort (verzia top-down).
// Parametre:
//  'array' - pole, ktore funkcia usporiada (pole bude po vykonani funkcie usporiadane)
//  'length' - pocet prvkov pola 'array'
// Poznamka: tato funkcia sluzi najma ako rozhranie pre zakrytie detailov rekurzivnej implementacie merge sort-u pri volani z main-u
void mergeSortTopDown(int* array, const size_t length)
{
    int* tmpArray = new int[length]; // pomocne pole
    std::memcpy(tmpArray, array, length * sizeof(int)); // prekopirujeme, pretoze chceme vysledok (usporiadane prvky) v originalnom poli
    // std::copy(array, array+length, tmpArray);
    mergeSortRecursive(array, tmpArray, 0, length);
    delete[] tmpArray;
}

// Vymeni obsahy smernikov (adresy v smernikoch)
// Vstupy: 'a' - adresa smernika, ktoreho obsah bude prekopirovany do smernika, ktoreho adresa je v 'b'
//         'b' - adresa smernika, ktoreho obsah bude prekopirovany do smernika, ktoreho adresa je v 'a'
void swapPointers(int **a, int **b)
{
    int *c = (*a);
    (*a) = (*b);
    (*b) = c;
}

// Funkcia usporiada pole 'array' algoritmom merge sort (verzia bottom-up).
// Parametre:
//  'array' - pole, ktore funkcia usporiada (pole bude po vykonani funkcie usporiadane)
//  'length' - pocet prvkov pola 'array'
void mergeSortBottomUp(int * array, const size_t length)
{
    int* tmpArray = new int [length];

    int * input = array;
    int * output = tmpArray;
    bool needCopyToOrigin = false;
    for (size_t step = 1; step < length; step *= 2) {
        for (size_t i = 0; i < length; i += 2 * step) {
            size_t low = i;
            size_t middle = min(i + step, length);
            size_t high = min(i + 2 * step, length);
            merge(output, input, low, middle, high);
        }
        swapPointers(&input, &output);
        needCopyToOrigin = ! needCopyToOrigin;
    }

    if (needCopyToOrigin) {
        std::memcpy(array, tmpArray, length * sizeof(int));
        //copy
    }

    delete[] tmpArray;
}

//-------------------------------------------------------------------------------------------------
// TESTOVANIE
//-------------------------------------------------------------------------------------------------

#define LENGTH(array) (sizeof(array)/sizeof(array[0])) // len pre staticky definovane polia

// Funkcia vytlaci titulok 'label', obsah pola 'data' a odriadkuje
// Parametre:
//  'label' - textovy retaze oznacujuci pole
//  'data' - pole, ktoreho obsah sa vytlaci
//  'length' - pocet prvkov pola 'data'
void println(const char * label, const int *data, const size_t length) {
    cout << label << ": ";
    for(size_t i = 0; i < length; ++i) {
        cout << setw(3) << data[i] << ' ';
    }
    cout << endl;
}

void test_insertionSort(){
    cout << "insertion sort" << endl;

    int d1[] = {10, 5, 2, 1, 8, 7, 5};
    insertionSort(d1, LENGTH(d1));
    println("d1", d1, LENGTH(d1));

    int d2[] = {100, -5, 1, 2 };
    insertionSort(d2, LENGTH(d2));
    println("d2", d2, LENGTH(d2));

    insertionSort(d2, 1);
    println("d2", d2, LENGTH(d2));

    insertionSort(d2, 0);
    println("d2", d2, LENGTH(d2));
}

void test_merge() {
    cout << "merge" << endl;

    int d1[]  = {100, 1, 2, 5, 6, 3, 4, 7, 8, 100, 100 };
    int d2[]  = {100, 3, 4, 7, 8, 1, 2, 5, 6, 100, 100 };
    int d3[]  = {100, 1, 2, 3, 4, 5, 6, 7, 8, 100, 100 };
    int d4[]  = {100, 5, 6, 7, 8, 1, 2, 3, 4, 100, 100 };
    int d5[]  = {100, 3, 4, 5, 1, 2, 6, 7, 100, 100 };
    int d6[]  = {100, 3, 4, 5, 6, 1, 2, 7, 100, 100 };
    int d7[]  = {100, 1, 2, 3, 100, 100 };
    int d8[]  = {100, 2, 1, 3, 100, 100 };
    int d9[]  = {100, 1, 2, 100, 100 };
    int d10[] = {100, 2, 1, 100, 100 };
    int output[11] = {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10};

    merge(output, d1, 1, 5, 9);
    println("d1 ", output, LENGTH(output));

    merge(output, d2, 1, 5, 9);
    println("d2 ", output, LENGTH(output));

    merge(output, d3, 1, 5, 9);
    println("d3 ", output, LENGTH(output));

    merge(output, d4, 1, 5, 9);
    println("d4 ", output, LENGTH(output));

    merge(output, d5, 1, 4, 8);
    println("d5 ", output, LENGTH(output));

    merge(output, d6, 1, 5, 8);
    println("d6 ", output, LENGTH(output));

    merge(output, d7, 1, 3, 4);
    println("d7 ", output, LENGTH(output));

    merge(output, d8, 1, 2, 4);
    println("d8 ", output, LENGTH(output));

    merge(output, d9, 1, 2, 3);
    println("d9 ", output, LENGTH(output));

    merge(output, d10, 1, 2, 3);
    println("d10", output, LENGTH(output));
}

void test_mergeSortTopDown() {
    cout << "merge sort (top-down)"<< endl;

    int d1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int d2[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int d3[] = {1,1,1,1,1};
    int d4[] = {2, 4, 1, 1, -1, 1, 1};
    int d5[] = {3, 2, 1};
    int d6[] = {2, 1};
    int d7[] = {1};
    int d8[] = {};

    mergeSortTopDown(d1, LENGTH(d1));
    println("d1", d1, LENGTH(d1));

    mergeSortTopDown(d2, LENGTH(d2));
    println("d2", d2, LENGTH(d2));

    mergeSortTopDown(d3, LENGTH(d3));
    println("d3", d3, LENGTH(d3));

    mergeSortTopDown(d4, LENGTH(d4));
    println("d4", d4, LENGTH(d4));

    mergeSortTopDown(d5, LENGTH(d5));
    println("d5", d1, LENGTH(d5));

    mergeSortTopDown(d6, LENGTH(d6));
    println("d6", d6, LENGTH(d6));

    mergeSortTopDown(d7, LENGTH(d7));
    println("d7", d7, LENGTH(d7));

    mergeSortTopDown(d8, LENGTH(d8));
    println("d8", d8, LENGTH(d8));
}

void test_mergeSortBottomUp() {
    cout << "merge sort (bottom-up)"<< endl;

    int d1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int d2[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int d3[] = {1,1,1,1,1};
    int d4[] = {2, 4, 1, 1, -1, 1, 1};
    int d5[] = {3, 2, 1};
    int d6[] = {2, 1};
    int d7[] = {1};
    int d8[] = {};

    mergeSortBottomUp(d1, LENGTH(d1));
    println("d1", d1, LENGTH(d1));

    mergeSortBottomUp(d2, LENGTH(d2));
    println("d2", d2, LENGTH(d2));

    mergeSortBottomUp(d3, LENGTH(d3));
    println("d3", d3, LENGTH(d3));

    mergeSortBottomUp(d4, LENGTH(d4));
    println("d4", d4, LENGTH(d4));

    mergeSortBottomUp(d5, LENGTH(d5));
    println("d5", d1, LENGTH(d5));

    mergeSortBottomUp(d6, LENGTH(d6));
    println("d6", d6, LENGTH(d6));

    mergeSortBottomUp(d7, LENGTH(d7));
    println("d7", d7, LENGTH(d7));

    mergeSortBottomUp(d8, LENGTH(d8));
    println("d8", d8, LENGTH(d8));
}

int main() {
    test_insertionSort();
    test_merge();
    test_mergeSortTopDown();
    test_mergeSortBottomUp();
    return 0;
}
