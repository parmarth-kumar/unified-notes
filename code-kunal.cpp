#include <vector>
#include <iostream>
#include <utility>
using namespace std;

void bubbleSort(vector<int>& arr){
    int n = arr.size();
    if(n <= 1){
        return;
    }

    // outer loop
    for(int i=0; i<n-1; ++i){
        bool swapped = false;

        // inner loop
        for(int j=0; j<n-1-i; ++j){
            if(arr[j] > arr[j+1]){
                swap(arr[j] , arr[j+1]);
                swapped = true;
            }
        }

        // already sorted array
        if(!swapped){
            break;
        }
    }
        

}

int main(){
    vector<int> arr = {5,2,4,1,3};

    cout << "original arr : ";
    for(int x: arr){
        cout << x << " ";
    }
    cout << "\n";

    bubbleSort(arr);

    cout << "Sorted Array:   ";
    for(int x : arr){
        cout << x << " ";
    }
    cout << "\n";

    return 0;
}