/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waboutzo <waboutzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 12:52:38 by waboutzo          #+#    #+#             */
/*   Updated: 2022/06/14 15:47:24 by waboutzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void	lexer_previous(t_lexer	*lexer)
{
	if (lexer->i != 0)
	{
		lexer->i -= 1;
		lexer->c = lexer->contents[lexer->i];
	}
}

int	get_index(t_lexer lexer, char c)
{
	while (lexer.c != '\0')
	{
		if (lexer.c == c)
			return (lexer.i);
		lexer_advance(&lexer);
	}
	return (-1);
}

char	*hard_code(t_lexer *lexer, char **envp, int c, int next_qoutes)
{
	char *str;

	if(lexer->c == '$' || lexer->i == next_qoutes)
		return ft_strdup("$");
	if (!ft_isalnum(lexer->c) && lexer->c != '_')
	{
		str = lexer_get_current_char_as_string(lexer);
		lexer_advance(lexer);
		return (ft_strjoin("$", str));
	}
	if (ft_isdigit(lexer->c))
	{
		lexer_advance(lexer);
		return ft_strdup("");
	}
	return (NULL);
}

char	*env_var_inside_qoutes(t_lexer *lexer, char **envp, int c)
{
	char	*value;
	char	*s;
	char	*str;
	int		next_qoutes;

	lexer_advance(lexer);
	next_qoutes = get_index(*lexer, c);
	str = hard_code(lexer, envp, c, next_qoutes);
	if (str)
		return (str);
	value = ft_calloc(1, sizeof(char));
	while (lexer->i != next_qoutes
		&& (ft_isalnum(lexer->c) || lexer->c == '_'))
	{
		s = lexer_get_current_char_as_string(lexer);
		value = ft_realloc(value, (ft_strlen(value)
					+ ft_strlen(s) + 1) * sizeof(char));
		ft_strcat(value, s);
		free(s);
		lexer_advance(lexer);
	}
	return (dollar_value(envp, value));
}

char	*quotes_cases(t_lexer *lexer, char **envp, int c)
{
	char *str;

	if (lexer->c == '$' && c != 39)
		str = env_var_inside_qoutes(lexer, envp, c);
	else
	{
		str = lexer_get_current_char_as_string(lexer);
		lexer_advance(lexer);
	}
	return (str);
}
char	*quotes_handler(t_lexer *lexer, char **envp, int c)
{
	char	*value;
	char	*s;

	lexer_advance(lexer);
	value = ft_calloc(1, sizeof(char));
	while (lexer->c != c)
	{
		s = quotes_cases(lexer, envp, c);
		value = ft_realloc(value, (ft_strlen(value)
					+ ft_strlen(s) + 1) * sizeof(char));
		ft_strcat(value, s);
		free(s);
	}
	lexer_advance(lexer);
	return (value);
}