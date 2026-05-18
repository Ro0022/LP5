#include<iostream>
#include<omp.h>
using namespace std;


void merge(int a[] , int low, int mid, int high){
    int temp[high-low+1];
    int i = low;
    int j = mid+1;
    int index = 0;
    while(i<=mid && j<=high){
        if(a[i]<=a[j]){
            temp[index++]=a[i++];
        }
        else{
            temp[index++]=a[j++];
        }
    }
    while(i<=mid){
        temp[index++]=a[i++];
    }
    while(j<=high){
        temp[index++]=a[j++];
    }
    for(int ind =low;ind<=high;ind++){
        a[ind]=temp[ind-low];
    }
}

void mergeSort(int a[],int low,int high){
    if(low>=high) return;
    int mid = low + (high-low)/2;
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            mergeSort(a,low,mid);
        }
        #pragma omp section
        {
            mergeSort(a,mid+1,high);
        }
    }
    merge(a,low,mid,high);
}

void bubble_sort(int a[],int length){
    for(int i=0;i<length;i++){
        int start = i%2;
        #pragma omp parallel for
        for(int j=start;j<length-1;j+=2){
            if(a[j]>a[j+1]){
                int temp = a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
    }
}

int main(){

   
    int a[] = {10,9,8,7,6,5,4,3,2,1};
    int length = sizeof(a)/sizeof(a[0]);
    // double start = omp_get_wtime();
    // bubble_sort(a,length);
    // double end = omp_get_wtime();
    // cout<<"Sorted Array..."<<endl;
    // for(int i=0;i<length;i++){
    //     cout<<a[i]<<" ";
    // }
    // cout<<"\nExecution Time : "<<(end-start)*1000<<" miliseconds";

    double startM = omp_get_wtime();
    mergeSort(a,0,length-1);
    double endM = omp_get_wtime();
    cout<<"Sorted Array..."<<endl;
    for(int i:a){
        cout<<i<<" ";
    }    
    cout<<"Exection Time : "<<(endM-startM)<<" Miliseconds";
    return 0;
}
