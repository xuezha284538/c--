#include<stdio.h>
void swap( int a[],int i,int j)
{
    int t=a[i];
    a[i]=a[j];
    a[j]=t;
}
void quick(int a[],int l,int r)
{
    if(l>=r) return ;
    int i=l,j=r;
    int p=a[l];
    while(i<j)
    {
        while(i<j&&a[j]>=p)
        {
            j--;
        }
        while(i<j&&a[i]<=p)
        {
            i++;
        }
        if(i<j)
        {
                swap(a,i,j);
        }
    }
    swap(a,i,l);
    quick(a,l,i-1);
    quick(a,i+1,r);
}



int main()
{
    int a[]={12,23,5,45,2,7,8,3,78};
    int len=sizeof(a)/sizeof(a[0]);
    quick(a,0,len - 1 );
    for(int i=0;i<len;i++)
    {
        printf("%d,",a[i]);
    }
}