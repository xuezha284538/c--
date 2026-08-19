#include<stdio.h>
void swap(int a[],int i,int j)
{
    int t=a[i];
    a[i]=a[j];
    a[j]=t;
}

void quick(int a[],int l,int r)
{   int i=l,j=r;
    if(i>=j) return ;
    
    while(i<j)
    {
        while(i<j&&a[j]>=a[l])
        j--;
        while(i<j&&a[i]<=a[l])
        i++;
        if (i<j)
        {
            swap(a,i,j);
        }
        
    }
    swap(a,l,j);
    quick(a,l,j-1);
    quick(a,j+1,r);

}

void show(int a[],int len)
{

    printf("{");
    for (int i = 0; i < len; i++)
    {
        printf("%d,",a[i]);
    }
    printf("\b}"); 
}

int main()
{
int a[]={31,23,434,534,123,3445,1213,35,2};
int n=sizeof(a)/sizeof(a[0]);
quick(a,0,n-1);
show(a,n);
    return 0;
}