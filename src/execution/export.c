/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waboutzo <waboutzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 22:13:02 by lchokri           #+#    #+#             */
/*   Updated: 2022/07/30 11:00:31 by waboutzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **sorted_env(char **env)
{
	int		i[2];
	char	*temp;
	
	i[0] = 0;
	i[1] = 0;
	while (env[i[0]])
	{
		i[1] = 0;
		while (env[i[1] + i[0]])
		{
			if(ft_strcmp(env[i[0]], env[i[1] + i[0]]) > 0)
			{
				temp = env[i[0]];
				env[i[0]] = env[i[1] + i[0]];
				env[i[1] + i[0]] = temp;
			}
			i[1]++;
		}
		i[0]++;
	}
	env[i[0]] = NULL;
	return (env);
}

void	print_export(char **env)
{
	int		i;
	char	**line;
	char	**envp;
	
	i = 0;
	envp = sorted_env(ft_strdup_double(env));
	while(envp[i])
	{
		line = ft_split(envp[i], '=');
		if(line[1])
		{
			free(line[1]);
			line[1] = ft_substr(envp[i], ft_strlen(line[0]) + 1, ft_strlen(envp[i]));
		}
		printf("declare -x %s", line[0]);
		if(!ft_strchr(envp[i], '='))
		{
			i++;
			printf("\n");
			free_double_char(line, 0);
			continue;
		}
		printf("=");
		if(!line[1])
			printf("\"\"\n");
		else if(line[1] && strcmp(line[1], "''"))
			printf("\"%s\"\n", line[1]);
		free_double_char(line, 0);
		i++;
	}
	free_double_char(envp, 0);
}

int	get_key(char *s, int index)
{
	int key;

	if (!s[index])
		key = 0;
	if (s[index] == '=')
		key = 1;
	if (s[index] == '+')
		key = 2;
	return (key);
}
void init_var(char *new_var, t_env_var **var, char **env)
{
	t_env_var	*tmp;
	int			index;
	int			value_start;

	
	tmp = *var;
	index = 0;
	tmp->t_stat = EXISTED;
	while(new_var[index] && new_var[index]!= '+'
		&& new_var[index] != '=')	
		index++;
	tmp->e_type = SET_VALUE;
	tmp->name = ft_substr(new_var, 0, index);
	tmp->index = get_index_of_double_char(env, tmp->name);
	tmp->key = get_key(new_var, index);
	value_start = index + tmp->key;
	tmp->value = ft_substr(new_var, value_start, ft_strlen(new_var));
	if(tmp->index == -1)
		tmp->t_stat = NOT;
	else if (value_start >= ft_strlen(new_var) && tmp->key != 1)
			tmp->e_type = NOTHING;
	else if(tmp->key == 2)
		tmp->e_type = CONCATENATE;
}
void	edit_exsited_variable(char ***env, t_env_var *var)
{
	char	*value;
	char	*tmp;
	
	tmp = (*env)[var->index];
	(*env)[var->index] = ft_strdup(var->name);
	if (var->key != 0)
	{
		value = ft_strdup(var->value);
		if(var->e_type == CONCATENATE)
			value = ft_strjoin(ft_strchr(tmp, '=') + 1, var->value);
		free(tmp);
		free((*env)[var->index]);
		(*env)[var->index] = ft_strjoin(var->name, "=");
		tmp = (*env)[var->index];
		(*env)[var->index] =  ft_strjoin((*env)[var->index], value);
		free(tmp);
		free(value);
	}
}

void	add_var(char	***env, t_env_var *var)
{
	char	**new_env;
	int		len;
	int		i;
	int		size;


	len = double_pointer_len(*env);
	size = (((var->t_stat == NOT) * (len + 1)) + ((var->t_stat != NOT) * len));
	if(var->t_stat == NOT)
		var->index = len;
	new_env = (char **) malloc((size + 1) * sizeof(char *));
	i = -1;
	while(++i < len)
		new_env[i] = ft_strdup((*env)[i]);
	if(var->t_stat == NOT)
		new_env[i++] = NULL;
	new_env[i++] = NULL;
	edit_exsited_variable(&new_env , var);
	free_double_char(*env, 0);
	*env = new_env;
}

void	my_export(char ***env, char **new_var)
{
	int		j;
	t_env_var	*var;

	j = 0;
	ft_setter(0);
	if (new_var[1] == NULL)
		print_export(*env);
	else
	{
		while (new_var[++j])
		{
			var = (t_env_var *) malloc(sizeof(t_env_var ));
			var->value = NULL;
			init_var(new_var[j], &var, *env);
			if(!ft_check_var_validition(var->name))
			{
				ft_setter(1);
				printf_error("minishell : export: `': not a valid identifier\n", NULL, NULL);
			}
			else if(var->e_type != NOTHING)
				add_var(env, var);
			free(var->name);
			free(var->value);
			free(var);
		}
	}
}