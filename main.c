#include "minishel.h"

int main(int    ac, char **av, char **env)
{
    char    *line;

    line = readline("minishell-1.0 ");
    printf("the command is : %s", line);

}