/*
* Search Algorithms Comparison
* Author: Kosta Matijevic
* Date: Oct 2019
*/

#pragma once
#include<iostream>
#include<fstream>
#include<chrono>
#include<algorithm>
#include<iomanip>

#define INPUT "in.txt"

using namespace std;
struct Result {
    int index;
    int comparisons;
    double time;
    Result():index(-1),comparisons(0),time(0){}
};

struct Table {
    int* arr_;
    int size_;
    Table() :arr_(nullptr), size_(0){}
    void Set(int* a, int s) { arr_ = a; size_ = s; }
    void Sort(int order) { if (order) sort(arr_, arr_ + size_); else sort(arr_, arr_ + size_, greater<int>()); }
    int isSorted() { for (int i = 1; i < size_; ++i) { if (arr_[i - 1] > arr_[i]) return 0; } return 1; }
};

struct BST{

    struct Node {
        int k;
        Node* left, * right;
        Node(int val) { k = val; left = right = nullptr; }
    };

    static Node* CreateBst(int* arr, int start, int end) {
        if (start > end) return nullptr;
        int mid = (start + end) / 2;
        Node* new_root = new Node(arr[mid]);
        new_root->left = CreateBst(arr, start, mid - 1);
        new_root->right = CreateBst(arr, mid+1, end);

        return new_root;
    }

    BST(Table *K) {
        root = CreateBst(K->arr_,0, K->size_-1);
    }
    
    Node* root;
};

BST* tree; //global tree to avoid building new tree for every BST search

typedef Result* (*SearchFunction)(int*, int, int);

typedef struct Method {
    string name;
    SearchFunction search_function;
    Method(string n, SearchFunction sf) { name = n; search_function = sf; }
    Method():name(" "),search_function(nullptr){}
};

Method *methods[5]; //global array of all supported search methods

Result* BinarySearch(int*arr, int size, int key)
{
    Result* r = new Result;


    int low = 0, high = size-1 , mid;

    while (low <= high)
    {
        mid = (low + high) / 2;
        r->comparisons++;
        if (arr[mid] == key)
        {
            r->index = mid;
            break;
        }
        
        else if (arr[mid] > key)
        {
            r->comparisons++;
            high = mid - 1;
        }
        else
            low = mid + 1;
    }
    
    return r;
}

Result* TernarySearch(int* arr, int size, int key)
{
    Result* r = new Result;

    int low = 0, high = size-1, mid1, mid2;

    while (low <= high)
    {
        mid1 = low + (high - low) / 3;
        mid2 = high - (high - low) / 3;

        r->comparisons++;
        if (arr[mid1] == key)
        {
            r->index = mid1;
            break;
        }
        r->comparisons++;
        if (arr[mid2] == key)
        {
            r->index = mid2;
            break;
        }

        r->comparisons++;
        if (key < arr[mid1])
        {
            high = mid1 - 1;
        }
        else if (key > arr[mid2])
        {
            r->comparisons++;
            low = mid2 + 1;
        }
        else
        {
            low = mid1 + 1;
            high = mid2 - 1;
        }
    }

    return r;
}

Result* SequentialSearch(int* arr, int size, int key)
{
    Result* r = new Result;

    arr[size] = key;
    int i = 0;
    while (arr[i] != key)
    {
        r->comparisons++;
        i++;
    }

    if (i == size) r->index = -1;
    else r->index = i;

    return r;
}

Result* InterpolationSearch(int* arr, int size, int key) 
{
    Result* r = new Result;

    int low = 0, high = size - 1, mid;

    while (low < high)
    {
        mid = low + (key - arr[low])*(high - low) / (arr[high] - arr[low]);
        r->comparisons++;
        if (arr[mid] == key)
        {
            r->index = mid;
            break;
        }

        else if (arr[mid] > key)
        {
            r->comparisons++;
            high = mid - 1;
        }
        else
            low = mid + 1;
    }

    return r;
}

Result* BinarySearchTree(int* tr, int size, int key)
{
    
    Result* r = new Result;
    BST::Node* tmp = tree->root;
    while (tmp != nullptr)
    {
        r->comparisons++;
        if (tmp->k == key) 
        {
            r->index = 0;
            break;
        }
        else if (tmp->k > key)
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }

    return r;
}

void SetTable(Table *K)
{
    int size;
    cout << "\nTable size: ";
    cin >> size;
    int* arr = new int[size];
    cout << "Keys:\n";
    for (int i = 0; i < size; i++)
        cin >> arr[i];
    K->Set(arr, size);
    cout << "Table successfully created.\n\n";
}

void LoadTable(Table *K)
{
    int size=0;
    
    ifstream iFile;
    iFile.open(INPUT,ios::in);
    if (iFile.is_open())
    {
        iFile >> size;
        int* arr = new int[size];
        for (int i = 0; i < size; i++)
            iFile >> arr[i];
        K->Set(arr, size);
        iFile.close();
        cout << "Loaded a table with " << size << " keys.\n";
    }
    else
        cout << "Cannot open input file.\n";
}

void SortTable(Table* K) {
    /*
    /////// for future algorithms with backwards sorting
    cout << "\n\n1.Ascending sort\n";
    cout << "2.Descending sort\n";

    int ctrl;
    cin >> ctrl;
    switch (ctrl) {
        case 1:
            K->Sort(1);
            break;
        case 2:
            K->Sort(0);
            break;
    }
    */
    K->Sort(1);
    cout << "Array successfully sorted.\n";
}
void RandomizeTable(Table* K){}
void PrintTable(Table *K){
    for (int i = 0; i < K->size_; i++)
        cout << K->arr_[i] << " ";
    cout << "\n\n";
}
void SearchKey(Table *K)
{
    if (K->arr_ == nullptr)
    {
        cout << endl;
        return;
    }

    tree = new BST(K); 

    int ctrl, key, count;
    Result* res = new Result;
    int* arr = K->arr_;
    int size = K->size_;
    Method *currentMethod=nullptr;

    if (!K->isSorted())
        cout << "WARNING: array is not sorted\n\n";

    cout << "\nSelect search method:\n";
    cout << "1.Sequential search {  O(n) }\n";
    cout << "2.Binary search {  O(logn) } !must be sorted\n";
    cout << "3.Interpolation search{ O(loglogn) - O(n) } !must be sorted\n";
    cout << "4.Ternary search {  O(logn) } !must be sorted\n";
    cout << "5.Fibonacci search {  O(logn) } !must be sorted\n";
    cout << "6.Binary Search Tree{ O(logn) } !must be sorted\n";
    cout << "9.Compare All\n";

    cin >> ctrl;

    cout << "\nEnter search key: ";
    cin >> key; cout << "\n";

    count = 0;
    switch (ctrl)
    {
    case 1:
        currentMethod = methods[0];
        break;
    case 2:
        currentMethod = methods[1];

        break;
    case 3:
        currentMethod = methods[2];

        break;
    case 4:
        currentMethod = methods[3];

        break;
    case 6:
        currentMethod = methods[4];
        break;
    case 9:
        count = 4;
        currentMethod = methods[count];
        break;
    }


    while (count >= 0)
    {
        auto start = chrono::steady_clock::now();
        res = currentMethod->search_function(arr, size, key);
        auto end = chrono::steady_clock::now();
        double time = chrono::duration_cast<chrono::microseconds>(end - start).count();

        cout << currentMethod->name << endl<<"----------------------"<< endl;
        if (res->index == -1) cout << "The key was not found.\n";
        else cout << "The key was found.\n";

    
        cout << "Number of comparisons: " << res->comparisons << ".\n";
        cout << "Processor time (Microseconds): " << setprecision(10) << time << ".\n\n";
        count--;
        if (count >= 0) currentMethod = methods[count];
    }
}

int main()
{
    Table* K = new Table;
    methods[0] = new Method("Sequential search",&SequentialSearch);
    methods[1] = new Method("Binary search", &BinarySearch);
    methods[3] = new Method("Ternary search", &TernarySearch);
    methods[2] = new Method("Interpolation search", &InterpolationSearch);
    methods[4] = new Method("Binary search tree", &BinarySearchTree);
    cout << "Kosta Matijevic - SEARCH ALGORITHMS\n--------------------------------";
    while (1)
    {
        int ctrl; 
        cout << "\n\n\n------------------------------------\n";
        cout << "1.Set keys table\n";
        cout << "2.Load table from file\n";
        cout << "3.Sort table\n";
        cout << "4.Randomize table\n";
        cout << "5.Print table\n\n";
        cout << "6.Search single key\n";
        cout << "7.Search multiple keys (COMING SOON) \n\n";
        cout << "0.Exit\n";
        cout << "------------------------------------\n";

        cin >> ctrl;
        switch (ctrl)
        {
        case 1:
            SetTable(K);
            break;
        case 2:
            LoadTable(K);
            break;
        case 3:
            SortTable(K);
            break;
        case 4:
            RandomizeTable(K);
            break;
        case 5:
            PrintTable(K);
            break;
        case 6:
            SearchKey(K);
            break;
        case 0:
            exit(0);
            break;
        }
    }

}