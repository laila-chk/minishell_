/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waboutzo <waboutzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 18:48:37 by waboutzo          #+#    #+#             */
/*   Updated: 2022/06/08 12:18:17 by waboutzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

t_node	*handler(char *str)
{
	t_lexer *lexer;
	t_token *token;
	t_node	*node = NULL;
	t_node	*tmp;
	
	tmp = NULL;
	lexer = init_lexer(str);
	token = lexer_get_next_token(lexer);
	while (token->e_type != TOKEN_EOF)
	{
		if(token->e_type == TOKEN_ERROR)
		{
			free(lexer);
			free(token->value);
			free(token);
			return (NULL);
		}
		//printf("%p \n", node);
		node = ft_lstnew((void *) init_cmd(lexer, &token));
		if(!node)
		{
			free(token->value);
			free(token);
			free(lexer);
			return (NULL);
		}
		ft_lstadd_back(&tmp, node);
		free(token->value);
		free(token);
		token = lexer_get_next_token(lexer);
	}
	// free(token);
	node = tmp;
	printf_node(node);
	free(token->value);
	free(lexer);
	free(token);
	return (node);
}


//pipe handling nsito
int	main(int argc, char **argv, char **envp)
{
	char	*str;
	t_node	*cmd;
	// int counter = 0;
	(void) envp;

	if (argc == 1)
	{
		while (1)
		{
			str = readline("\033[0;35mminishell$ \033[0;37m");
			add_history (str);
			cmd = handler(str);
			free_node(&cmd);
			// if(counter == 1)
			// 	while(1);
			// counter++;
			// printf("%p\n", cmd);
			free(str);
		}
	}
	return (0);
}
