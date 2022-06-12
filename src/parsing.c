/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waboutzo <waboutzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 15:19:13 by waboutzo          #+#    #+#             */
/*   Updated: 2022/06/12 16:42:34 by waboutzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

char	*string_cases(t_lexer *lexer, char **envp)
{
	if(lexer->c == '$')
		return (handle_env_var(lexer, envp));
	if(lexer->c == '"')
		return (quotes_handler(lexer, envp, '"'));
	if(lexer->c == '\'')
		return (quotes_handler(lexer, envp, '\''));
	else
		return (lexer_get_current_char_as_string(lexer));
}
char	*pure_arg(char *str, char **envp)
{
	char	*value;
	char	*s;
	t_lexer	*lexer;

	value = ft_calloc(1, sizeof(char));
	lexer = init_lexer(str);
	while (lexer->c != '\0')
	{
		s = string_cases(lexer, envp);
		value = ft_realloc(value, (ft_strlen(value)
					+ ft_strlen(s) + 1) * sizeof(char));
		ft_strcat(value, s);
		free(s);
		lexer_advance(lexer);
	}
	free(str);
	free(lexer);
	return (value);
}

void	parsing_args(t_node **head, char **envp)
{
	t_node		*temporary;
	t_args		*args;

	temporary = *head;
	while (temporary != NULL)
	{
		args = (t_args *) temporary->content;
		args->value = pure_arg(args->value, envp);
		temporary = temporary->next;
	}
}

// void parsing_redrection(t_node **head)
// {
// 	t_node		*temporary;

// 	temporary = *head;
// 	while (temporary != NULL)
// 	{
// 		temporary = temporary->next;
// 	}
// }
void	parsing(t_node **command, char **envp)
{
	t_node	*temporary;

	temporary = *command;
	while (temporary != NULL)
	{
		parsing_args(&((t_cmd *)temporary->content)->args, envp);
		// parsing_redrection(&((t_cmd *)temporary->content)->redrec);
		temporary = temporary->next;
	}
}

// int		i;
// 	char	*tmp;
// 	char	*expd;

// 	tmp = *str;
// 	i = 0;
// 	while (tmp[i])
// 	{
// 		if (tmp[i] == '$')
// 		{
// 			expand_dollar(tmp, &expd, i);
// 			free(expd);
// 		}
// 		i++;
// 	}
