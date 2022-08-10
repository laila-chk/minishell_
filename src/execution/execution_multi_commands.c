/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_multi_commands.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waboutzo <waboutzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 12:37:23 by waboutzo          #+#    #+#             */
/*   Updated: 2022/08/11 00:34:51 by waboutzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char *cwd_saver;

// char	*check_cmd(char *cmd, char **envp)
// {

// }

void	dup_norm(int fildes1, int fildes2)
{
	dup2(fildes1, fildes2);
	close(fildes1);
}

void	child_work(t_node *head, char **env, int *pipe_fd, int last_fd)
{
	t_cmd		*cmd;
	char		*tmp;
	char		*path;

	cmd = (t_cmd *)head->content;
	if (head->next != NULL)
		dup_norm(pipe_fd[1], 1);
	if (cmd->output != NULL)
		dup_norm(cmd->output->fd, 1);
	if (last_fd != -1)
		dup_norm(last_fd, 0);
	if (cmd->input != NULL)
		dup_norm(cmd->input->fd, 0);
	close(pipe_fd[0]);
	if (cmd->after_expand && cmd->after_expand[0] && cmd->e_rtype == VALID)
	{
		tmp = getcwd(NULL, 0);
		if(ft_strcmp(cmd->after_expand[0], "pwd") && tmp)
		{
			free(cwd_saver);
			cwd_saver = tmp;
		}
		if (!execute(cmd->after_expand, &env, 0))
		{
			error_handling(cmd->after_expand[0]);
			path = check_cmd(cmd->after_expand[0], env);
			execve(path, cmd->after_expand, env);
			perror("minishell");
			ft_setter(127);
		}
	}
	else
		ft_setter(0);
	exit(ft_getter());
}

void	execution_multi_cmds(t_node *head, char **env)
{
	t_cmd		*cmd;
	int			last_fd;
	int			pipe_fd[2];
	int			id;
	int			res;
	int			status;

	last_fd = -1;
	res = 0;
	while (head != NULL)
	{
		pipe(pipe_fd);
		id = fork();
		if (id == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			child_work(head, env, pipe_fd, last_fd);
		}
		else
		{
			cmd = (t_cmd *)head->content;
			ft_setter(0);
			if (last_fd != -1)
				close(last_fd);
			if (!head->next)
				close(pipe_fd[0]);
			if (cmd->output != NULL)
				close(cmd->output->fd);
			if (cmd->input != NULL)
				close(cmd->input->fd);
			close(pipe_fd[1]);
			last_fd = pipe_fd[0];
		}
		head = head->next;
	}
	signal(SIGINT, SIG_IGN);
	while (res != -1)
	{
		res = waitpid(-1, &status, 0);
		if (res == id)
		{
			ft_setter(WEXITSTATUS(status));
			if (WIFSIGNALED(status))
				ft_setter(WTERMSIG(status) + 128);
		}
	}
	signal(SIGINT, sig_handler);
}
