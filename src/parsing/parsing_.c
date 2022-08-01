/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waboutzo <waboutzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 19:21:35 by waboutzo          #+#    #+#             */
/*   Updated: 2022/07/31 22:37:26 by waboutzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*string_cases(t_lexer *lexer, char **envp)
{
	char	*str;

	if (lexer->c == '"')
		str = quotes_handler(lexer, envp, '"');
	else if (lexer->c == '\'')
		str = quotes_handler(lexer, envp, '\'');
	else if (lexer->c == '$')
	{
		str = handle_env_var(lexer, envp);
		int i= 0;
		while(str[i])
		{
			if(str[i] == '\'')
				str[i] = -1;
			if(str[i] == '"')
				str[i] = -2;
			i++;
		}
	}
	else
	{
		str = lexer_get_current_char_as_string(lexer);
		lexer_advance(lexer);
	}
	return (str);
}

char	*pure_arg(char *str, char **envp)
{
	char	*value;
	char	*s;
	t_lexer	*lexer;
	char	*tmp;

	value = (char *) malloc(sizeof(char));
	value[0] = '\0';
	lexer = init_lexer(str);
	while (lexer->c != '\0')
	{
		s = string_cases(lexer, envp);
		tmp = value;
		value = ft_strjoin(value, s);
		free(tmp);
		free(s);
	}
	free(str);
	free(lexer);
	return (value);
}

char	*remove_qoutes(char *str)
{
	char	*value;
	char	*s;
	t_lexer	*lexer;
	char	*tmp;

	lexer = init_lexer(str);
	value = (char *) malloc(sizeof(char));
	value[0] = '\0';
	while (lexer->c != '\0')
	{
		if (lexer->c == '"' || lexer->c == '\'')
			s = quotes(lexer,lexer->c);
		else
		{
			s = lexer_get_current_char_as_string(lexer);
			lexer_advance(lexer);
		}
		tmp = value;
		value = ft_strjoin(value, s);
		free(tmp);
		free(s);
	}
	free(lexer);
	return (value);
}

void	pure_after_expand(char **str)
{
	int	i;
	char	*tmp;

	i = 0;
	while (str[i])
	{
		tmp = str[i];
		str[i] = remove_qoutes(str[i]);
		int j = 0;
		while(str[i][j])
		{
			if(str[i][j] == -1)
				str[i][j] = '\'';
			if(str[i][j] == -2)
				str[i][j] = '"';
			j++;
		}
		free(tmp);
		i++;
	}
}

int	ft_counte_novide(char **str)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (str[i])
	{
		if (str[i][0] != '\0')
			counter++;
		i++;
	}
	return (counter);
}
