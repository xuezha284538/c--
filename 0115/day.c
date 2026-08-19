#include <stdio.h>
// int fib(int n)
// {
//     int a[n];
//     a[0] = 1;
//     a[1] = 1;

//     for (int i = 2; i < n; i++)
//     {

//         a[i] = a[i - 2] + a[i - 1];
//     }
//     return a[n - 1];
// }
int days(int year, int month, int day)
{
    int today, temp = 0;
    int month2[month];
    // int month3[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    for (int i = month - 1; i > 0; i--)
    {
        switch (i)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            month2[i] = 31;
            break;

        case 4:
        case 6:
        case 9:
        case 11:
            month2[i] = 30;
            break;

        case 2:
            if ((year % 4 == 0) && (year % 100 == 0) || year % 400 == 0)
            {
                month2[i] = 29;
            }
            else
            {
                month2[i] = 28;
            }
            break;
        default:
            break;
        }
        temp += month2[i];
    }
    if (month == 1)
    {
        month2[month - 1] = 0;
    }

    today = temp + day;
    return today;
}

int main(int argc, char const *argv[])
{
    int year;
    int month;
    int day;
    printf("请输入年份月份和天数(2006,1,3): ");
    scanf("%d,%d,%d", &year, &month, &day);
    printf("天数:%d", days(year, month, day));
    return 0;
}
