/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waboutzo <waboutzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 22:13:02 by lchokri           #+#    #+#             */
/*   Updated: 2022/07/27 03:16:34 by waboutzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

char **sorted_env(char **env)
{
	int		i[2];
	int		j;
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
	printf("sssssss\n");
	envp = sorted_env(ft_strdup_double(env));
	printf("sssssss\n");
	while(envp[i])
	{
		line = ft_split(envp[i], '=');
		if(line[1])
		{
			free(line[1]);
			line[1] = ft_substr(envp[i], ft_strlen(line[0]) + 1, ft_strlen(envp[i]));
		}
		printf("declare -x %s=", line[0]);
		if(!line[1])
			printf("\"\"\n");
		else if(line[1] && strcmp(line[1], "''"))
			printf("\"%s\"\n", line[1]);
		free_double_char(line, 0);
		i++;
	}
	free_double_char(envp, 0);
}

int get_name_index(char *s)
{
	int index;

	index = 1;
	while(s[index] && s[index]!= '+' && s[index] != '=')	
		index++;
	return (index);
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
void get_data(char *new_var, t_env_var **var)
{
	int	index;

	index = get_name_index(new_var);
	(*var)->name = ft_substr(new_var, 0, index);
	(*var)->key = get_key(new_var, index);
	if(index + (*var)->key != ft_strlen(new_var))
		(*var)->value = ft_substr(new_var, index + (*var)->key, ft_strlen(new_var));
}
void	edit_excited_variable(char ***env, t_env_var *var, int index)
{
	char	*last_value;
	char	*tmp;

	if(var->key == 2 && (*env)[index])
	{
		last_value = ft_strchr((*env)[index], '=');
		tmp = var->value;
		var->value = ft_strjoin((last_value + 1) , var->value);
		free(tmp);
	}
	if((*env)[index])
		free((*env)[index]);
	(*env)[index] = ft_strjoin(var->name, "=");
	if(var->key != 0)
	{
		tmp = (*env)[index];
		(*env)[index] = ft_strjoin((*env)[index] , var->value);
		free(tmp);
	}
}

void	add_var(char	***env, t_env_var *var, int index)
{
	char	**new_env;
	int		len;
	int		i;
	int		size;


	len = double_pointer_len(*env);
	size = (((index == -1) * (len + 1)) + ((index != -1) * len));
	if(index == -1)
		index = len;
	new_env = (char **) malloc((size + 1) * sizeof(char *));
	i = 0;
	while(i < len)
	{
		new_env[i] = ft_strdup((*env)[i]);
		i++;
	}
	while(i <= size)
	{
		new_env[i] = NULL;
		i++;
	}
	edit_excited_variable(&new_env , var, index);
	free_double_char(*env, 0);
	*env = new_env;
}

void	my_export(char ***env, char **new_var)
{
	int		j;
	t_env_var	*var;
	int		index;

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
			get_data(new_var[j], &var);
			index = get_index_of_double_char(*env, var->name);
			if(!ft_check_var_validition(var->name))
			{
				ft_setter(1);
				printf_error("minishell : export: `': not a valid identifier\n", NULL, NULL);
			}
			else
			{
				if(index != -1 && var->key == 0)
						return ;
				else
					add_var(env, var, index);
			}
			free(var->name);
			if(var->key != 0)
				free(var->value);
			free(var);
		}
	}
}
