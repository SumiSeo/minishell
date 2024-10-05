/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumseo <sumseo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:30:11 by sumseo            #+#    #+#             */
/*   Updated: 2024/10/05 16:54:45 by sumseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quit_child(t_exec **cmds_list)
{
	if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((*cmds_list)->cmd_array[0], STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		store_or_free(NULL, NULL, false, true);
		exit(126);
	}
	if (errno == ENOENT)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((*cmds_list)->cmd_array[0], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		store_or_free(NULL, NULL, false, true);
		exit(127);
	}
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((*cmds_list)->cmd_array[0], STDERR_FILENO);
	ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	store_or_free(NULL, NULL, false, true);
	exit(127);
}

void	init_child(t_exec **cmds_list, char **env_copy, t_data *data)
{
	signal(SIGINT, sig_handler_forks);
	signal(SIGQUIT, sig_handler_quit);
	if (getfile(cmds_list, data))
	{
		only_redirection(cmds_list);
		if (!(*cmds_list)->cmd_array)
		{
			store_or_free(NULL, NULL, false, true);
			exit(0);
		}
		if ((*cmds_list)->cmd_array && parse_path((*cmds_list)->cmd_array,
				(*cmds_list)->path))
		{
			execve((*cmds_list)->path, (*cmds_list)->cmd_array, env_copy);
		}
		if (errno == EISDIR)
		{
			store_or_free(NULL, NULL, false, true);
			exit(126);
		}
		else
			quit_child(cmds_list);
	}
	store_or_free(NULL, NULL, false, true);
	exit(1);
}

void	exec_shell_builtin(t_exec **cmds_list, int builtin_check,
		t_env **env_list, t_data *data)
{
	if (getfile(cmds_list, data))
	{
		only_redirection(cmds_list);
		exec_builtin(builtin_check, cmds_list, env_list);
	}
}

void	exec_shell_init(t_exec **exec_list)
{
	dup2((*exec_list)->old_stdout, STDOUT_FILENO);
	dup2((*exec_list)->old_stdin, STDIN_FILENO);
	close((*exec_list)->old_stdout);
	close((*exec_list)->old_stdin);
}

void	exec_shell(t_exec **exec_list, t_env **env_list, char **env_copy,
		t_data *data)
{
	int		builtin_check;
	int		status;
	pid_t	fork_id;

	status = 0;
	builtin_check = which_builtin(*exec_list);
	launch_heredoc(exec_list, data);
	if (builtin_check > 0)
	{
		(*exec_list)->old_stdin = dup(STDIN_FILENO);
		(*exec_list)->old_stdout = dup(STDOUT_FILENO);
		exec_shell_builtin(exec_list, builtin_check, env_list, data);
		exec_shell_init(exec_list);
	}
	else
	{
		fork_id = fork();
		if (fork_id == 0)
			init_child(exec_list, env_copy, data);
		else
			get_status(fork_id, status, data);
	}
}
