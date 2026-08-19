#include <stdio.h>
int days2(int year, int month, int day)
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
struct Date
{
    int year;
    int month;
    int day;
};
int diff(struct Date *p, struct Date *p1)
{
    int days,temp=0,temp1=0;
    if (p->year == p1->year)
    {

        if (p->month == p1->month)
        {
            if (p->day > p1->day)
            {
                days = p->day - p1->day;
            }
            else
                days = p1->day - p->day;
        }
        else
        {
            if (p->month > p1->month)
            {
                days = days2(p->year, p->month, p->day) - days2(p1->year, p1->month, p1->day);
            }
            else
            {
                days = days2(p1->year, p1->month, p1->day) - days2(p->year, p->month, p->day);
            }
        }
    }
    else if (p->year > p1->year)
    {
        if (p->month == p1->month)
        {
            if (p->day > p1->day)
            {
                days = p->day - p1->day;
            }
            else
                days = p1->day - p->day;
        }
        else
        {
            if (p->month > p1->month)
            {
                days = days2(p->year, p->month, p->day) - days2(p1->year, p1->month, p1->day);
            }
            else
            {
                days = days2(p1->year, p1->month, p1->day) - days2(p->year, p->month, p->day);
            }
        }
        for ( int i = p->year; i >p1->year; i--)
        {
             if ((i % 4 == 0) && (i % 100 != 0) || i % 400 == 0)
            {
               temp = 366;
            }
            else
            {
                temp = 365;
            }
            temp1+=temp;
            
        }
        
        days+=temp1;
    }
    return days;
}

int main(int argc, char const *argv[])
{
    struct Date d1 = {2025, 12, 5}, d2 = {2024, 11, 5};
    printf("%d", diff(&d1, &d2));
    return 0;
}
