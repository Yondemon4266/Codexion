
#include "data.h"

static int ft_isspace(char c)
{
    if ((c >= 9 && c <= 13) || c == 32)
        return EXIT_SUCCESS;
    return EXIT_FAILURE;
}

static int ft_is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return EXIT_SUCCESS;
    return EXIT_FAILURE;
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
        if (ft_is_digit(s[i]) == EXIT_FAILURE)
            return 
        i++;
        return (result * sign);
    }
    return (result * sign);
}