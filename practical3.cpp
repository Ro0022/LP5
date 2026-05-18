#include<iostream>
#include<omp.h>
#include<climits>

using namespace std;

int main(){

    int a[] = {10,5,40,4,1};
    int n = sizeof(a)/sizeof(a[0]);

    // find Minimum
    int min_value = INT_MAX;
    #pragma omp parallel for reduction(min:min_value)
    for(int i:a){
        if(i<min_value){
            min_value = i;
        }
    }
    cout<<"Minimum value is : "<<min_value<<endl;

    // find Maximum
    int max_value = INT_MIN;
    #pragma omp parallel for reduction(max:max_value)
    for(int i:a){
        if(i>max_value){
            max_value = i;
        }
    }
    cout<<"Maximum value is :"<<max_value<<endl;

    // Sum of the array elements
    int sum = 0;
    double start = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for(int i:a){
        sum+=i;
    }
    double end = omp_get_wtime();
    cout<<"SUM is :"<<sum<<endl;
    cout<<"Execution Time is: "<<(end-start)<<" Miliseconds"<<endl;

    // Average of the array
   
    // #pragma omp parallel for reduction(+:sum)
    // for(int i:a){
    //     sum+=i;
    // }
    double average = (double)(sum/n);
    cout<<"Average is : "<<average<<endl;

    
    return 0;
}