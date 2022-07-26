/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waboutzo <waboutzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 13:12:20 by waboutzo          #+#    #+#             */
/*   Updated: 2022/07/25 09:52:12 by waboutzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

char	*quotes(t_lexer *lexer, char **envp, int c)
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

char	*ft_norm(t_lexer *lexer)
{
	char	*str;
	char	*tmp;

	if (!ft_isalnum(lexer->c) && lexer->c != '_')
	{
		if(lexer->c == '?')
			tmp = ft_itoa(ft_getter());
		else
		{
			str = lexer_get_current_char_as_string(lexer);
			tmp = ft_strjoin("$", str);
			free(str);
		}
		lexer_advance(lexer);
		return (tmp);
	}
	if (ft_isdigit(lexer->c))
	{
		if (lexer->c == '0')
		{
			lexer_advance(lexer);
			return (ft_strdup("minishell"));
		}
		lexer_advance(lexer);
		return (ft_strdup(""));
	}
	return (NULL);
}

char	*hard_code_norm(t_lexer *lexer, char **envp)
{
	if (lexer->c == '$')
		return (ft_strdup("$"));
	if (lexer->c == 34)
		return (quotes(lexer, envp, 34));
	if (lexer->c == 39)
		return (quotes(lexer, envp, 39));
	else
		return (ft_norm(lexer));
	return (NULL);
}

char	*handle_env_var(t_lexer *lexer, char **envp)
{
	char	*value;
	char	*s;
	char	*str;

	lexer_advance(lexer);
	str = hard_code_norm(lexer, envp);
	if (str)
		return (str);
	value = ft_calloc(1, sizeof(char));
	while (ft_isalnum(lexer->c) || lexer->c == '_')
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

char	*dollar_value(char **envp, char *var)
{
	char	*str;
	char	**tmp;

	if (!(ft_strchr(var, '=')) && *var)
	{
		while (*envp)
		{
			tmp = ft_split(*envp, '=');
			free(tmp[1]);
			tmp[1] = ft_substr(*envp, ft_strlen(*tmp) + 1, ft_strlen(*envp));
			if (!ft_strcmp(tmp[0], var))
			{
				if (!tmp[1])
					str = ft_strdup("");
				else
					str = ft_strdup(tmp[1]);
				free(var);
				free_double_char(tmp, 0);
				return (str);
			}
			free_double_char(tmp, 0);
			envp++;
		}
	}
	free(var);
	return (ft_strdup(""));
}
