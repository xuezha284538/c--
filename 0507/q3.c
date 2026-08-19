#include <stdio.h>

void swap(int a[], int i, int j)
{
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}
void quick(int a[], int l, int r)
{
    if (l >= r)
    {
        return ;//结束  
    }
    int i=l,j=r;
    while(i<j) 
    {
        while(i<j&&a[j]>=a[l]) //找到比基准小的就不动
       {   
        j--;
        }
        while(i<j&&a[i]<=a[l]) //找到比基准大的就不动
       {    i++;
        }
      
        swap(a,i,j);
    }
    swap(a,l,j); //相遇的前一个位置是基准的位置   

    quick(a,l,j-1); //递归排序 左边  
    quick(a,j+1,r); //递归排序 右边

}

void show(int a[], int len)
{
    printf("{");
    for (int i = 0; i < len; i++)
    {
        printf("%d,", a[i]);
    }
    printf("\b}\n");
}
int main()
{
    int a[]={1,3,2,5,4};
    int n=sizeof(a)/sizeof(a[0]);
        quick(a,0,n-1);
    
    show(a,n);
    return 0;
}