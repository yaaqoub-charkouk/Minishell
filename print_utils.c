#include "parsing/parsing.h"

void print_tokens(t_list *tokens)
{
	while (tokens)
	{
		printf("TOKEN: %-10s \n", tokens->content);
		switch (tokens->type)
		{
			case CMD:
				printf("  TYPE: CMD\n");
				break;
			case PIPE:
				printf("  TYPE: PIPE\n");
				break;
			case REDIRECTION_IN:
				printf("  TYPE: REDIRECTION_IN\n");
				break;
			case REDIRECTION_OUT:
				printf("  TYPE: REDIRECTION_OUT\n");
				break;
			case APPEND:
				printf("  TYPE: APPEND\n");
				break;
			case HEREDOC:
				printf("  TYPE: HEREDOC\n");
				break;
			case AND:
				printf("  TYPE: AND\n");
				break;
			case OR:
				printf("  TYPE: OR\n");
				break;
			case P_OPEN:
				printf("  TYPE: (\n");
				break;
			case P_CLOSE:
				printf("  TYPE: )\n");
				break;
			default:
				printf("  TYPE: Unknown\n");
				break;
		}
		tokens = tokens->next;
	}
}

