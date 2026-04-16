#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


int ft_isspace(char c)
{
    if ((c >= 9 && c <= 13) || c == 32)
        return (0);
    return 1;
}

long    ft_atoi(char *s)
{
    long result;
    int sign;
    int i;

    result = 0;
    sign = 1;
    i = 0;
    while (s[i])
    {
        while (ft_isspace(s[i]) == 0)
            i++;
        
        if (s[i] == '+' || s[i] == '-')
        {
            if (s[i] == '-')
                sign = -1;
            i++;
        }
        while (s[i] >= '0' && s[i] <= '9')
        {
            result = (result * 10) + (s[i] - '0');
            i++;
        }
        return (result * sign);
    }
    return (result * sign);
}

int main(int ac, char **av)
{
    if (ac != 2)
        return (1);
    
    long number;

    number = ft_atoi(av[1]);
    if (number <= INT_MAX && number >= INT_MIN)
    {
        printf("%ld", number);
    }
    else
    {
        printf("NOT OK");
    }
}