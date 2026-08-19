# include<stdio.h>
# include<string.h>

int main(int argc, char const *argv[])


{
    char a[]={'h','e','l','l','o'};
    char b[]={"hello"};
    char d[]={"bye"};
    char c[]={'h','e','l','l','o','\0'};
    char *j="hello";
    j="bye";

    b[0]=d[0];
    b[1]=d[1];
    b[2]=d[2];
    b[3]=d[3];

    printf("%s\n",a);
    printf("%s\n",b);
    printf("%s\n",c);
    printf("%s\n",j);
    return 0;
}
