#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // for strdup

// === AST Node Creator ===
t_tree *create_tree_node(t_type_node type, t_tree *left, t_tree *right, char **cmd)
{
    t_tree *node = malloc(sizeof(t_tree));
    if (!node)
        return NULL;
    node->type = type;
    node->left_node = left;
    node->right_node = right;
    node->cmd = cmd;
    return node;
}

// === Group Extractor ===
t_list *extract_group(t_list **tokens)
{
    t_list *start = *tokens;
    t_list *curr = *tokens;
    int depth = 0;

    if (!curr || curr->type != PAREN_OPEN)
        return NULL;

    depth++;
    curr = curr->next;

    while (curr)
    {
        if (curr->type == PAREN_OPEN)
            depth++;
        else if (curr->type == PAREN_CLOSE)
            depth--;

        if (depth == 0)
            break;

        curr = curr->next;
    }

    if (!curr || depth != 0)
    {
        printf("syntax error: unbalanced parentheses\n");
        return NULL;
    }

    t_list *after = curr->next;
    curr->next = NULL;
    *tokens = after;
    return start->next;
}

// === Recursive Parser Forward Declarations ===
t_tree *parse_logical_or(t_list **tokens);
t_tree *parse_logical_and(t_list **tokens);
t_tree *parse_pipe(t_list **tokens);
t_tree *parse_redirection(t_list **tokens);
t_tree *parse_command(t_list **tokens);
t_tree *parse_group(t_list **tokens);

// === Entry Point ===
t_tree *parse_input(t_list *tokens)
{
    return parse_logical_or(&tokens);
}

// === Logical OR ===
t_tree *parse_logical_or(t_list **tokens)
{
    t_list *curr = *tokens, *prev = NULL;
    while (curr)
    {
        if (curr->type == OR)
        {
            t_list *right = curr->next;
            if (prev) prev->next = NULL;
            return create_tree_node(OR,
                parse_logical_or(tokens),
                parse_logical_or(&right), NULL);
        }
        prev = curr;
        curr = curr->next;
    }
    return parse_logical_and(tokens);
}

// === Logical AND ===
t_tree *parse_logical_and(t_list **tokens)
{
    t_list *curr = *tokens, *prev = NULL;
    while (curr)
    {
        if (curr->type == AND)
        {
            t_list *right = curr->next;
            if (prev) prev->next = NULL;
            return create_tree_node(AND,
                parse_logical_and(tokens),
                parse_logical_and(&right), NULL);
        }
        prev = curr;
        curr = curr->next;
    }
    return parse_pipe(tokens);
}

// === Pipe ===
t_tree *parse_pipe(t_list **tokens)
{
    t_list *curr = *tokens, *prev = NULL;
    while (curr)
    {
        if (curr->type == PIPE)
        {
            t_list *right = curr->next;
            if (prev) prev->next = NULL;
            return create_tree_node(PIPE,
                parse_pipe(tokens),
                parse_pipe(&right), NULL);
        }
        prev = curr;
        curr = curr->next;
    }
    return parse_redirection(tokens);
}

// === Redirection ===
t_tree *parse_redirection(t_list **tokens)
{
    t_list *curr = *tokens, *prev = NULL;
    while (curr)
    {
        if (curr->type == REDIRECTION_OUT || curr->type == REDIRECTION_IN ||
            curr->type == APPEND || curr->type == HEREDOC)
        {
            t_type_node type = curr->type;
            t_list *file_token = curr->next;
            if (!file_token || file_token->type != CMD)
                return NULL;
            if (prev) prev->next = NULL;
            return create_tree_node(type,
                parse_redirection(tokens),
                parse_command(&file_token), NULL);
        }
        prev = curr;
        curr = curr->next;
    }
    return parse_command(tokens);
}

// === Group (Parentheses) ===
t_tree *parse_group(t_list **tokens)
{
    t_list *inside = extract_group(tokens);
    if (!inside)
        return NULL;
    t_tree *subtree = parse_logical_or(&inside);
    return create_tree_node(GROUP, subtree, NULL, NULL);
}

// === Command ===
t_tree *parse_command(t_list **tokens)
{
    if (*tokens && (*tokens)->type == PAREN_OPEN)
        return parse_group(tokens);

    t_list *curr = *tokens;
    int count = 0;
    while (curr && curr->type == CMD)
    {
        count++;
        curr = curr->next;
    }
    if (count == 0)
        return NULL;

    char **cmd = malloc(sizeof(char *) * (count + 1));
    curr = *tokens;
    for (int i = 0; i < count; i++) {
        cmd[i] = strdup(curr->content);
        curr = curr->next;
    }
    cmd[count] = NULL;

    t_tree *node = create_tree_node(CMD, NULL, NULL, cmd);
    *tokens = curr;
    return node;
}