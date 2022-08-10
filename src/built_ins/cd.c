/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waboutzo <waboutzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 12:57:29 by waboutzo          #+#    #+#             */
/*   Updated: 2022/08/10 22:27:10 by waboutzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char *cwd_saver;

void	update_pwd(char ***env)
{
	int		index;
	char	*str;

	index = get_index_of_double_char(*env, "PWD");
	if (index != -1)
	{
		free((*env)[index]);
		str = getcwd(NULL, 0);
		(*env)[index] = ft_strjoin("PWD=", str);
		free(str);
	}
}

void	update_oldpwd(char ***env, char *get_cwd,char *cwd_env)
{
	int		index;
	char	*str;
	char	*tmp;

	str = NULL;
	if (cwd_env)
		str = cwd_env;
	else if (get_cwd)
		str = get_cwd;
	index = get_index_of_double_char(*env, "OLDPWD");
	if (index != -1)
	{
		free((*env)[index]);
		(*env)[index] = ft_strdup("OLDPWD");
		if(str && !access(cwd_saver, X_OK))
		{
			tmp = (*env)[index];
			(*env)[index] = ft_strjoin((*env)[index], "=");
			free(tmp);
			tmp = (*env)[index];
			(*env)[index] = ft_strjoin((*env)[index], str);
			free(tmp);
		}
	}
}

void	cd(char *path, char ***env)
{
	char	*cwd_env;
	char	*get_cwd;
	char	*str;

	
	ft_setter(0);
	if (!path)
	{
		path = getenv("HOME");
		if(!path)
		{
			str = dollar_value(*env, ft_strdup("USER"));
			path = ft_strjoin("/Users/", str);
			free(str);
		}
	}
	get_cwd = getcwd(NULL, 0);
	cwd_env = getpwd(*env);
	if (!(chdir(path) == -1))
	{
		update_pwd(env);
		update_oldpwd(env, get_cwd,cwd_env);
	}
	else
	{
		ft_setter(1);
		str = ": No such file or directory\n";
		if(!access(path, F_OK) && access(path, X_OK) != 0)
			str  = ": Permission denied\n";
		printf_error("minishell: ", path, str);
	}
	free (cwd_env);
	free (get_cwd);
}
