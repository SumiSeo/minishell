/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sumseo <sumseo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:59:43 by sumseo            #+#    #+#             */
/*   Updated: 2024/10/05 16:59:22 by sumseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_path(char **cmds, char *path)
{
	(void)cmds;
	if (!path || access(path, X_OK | F_OK) != 0)
		return (0);
	else
		return (1);
}

int	getfile(t_exec **cmds_list, t_data *data)
{
	int	flags;
	int	infile_exist;

	if ((*cmds_list)->files_info->infile_info->final_name)
	{
		infile_exist = open((*cmds_list)->files_info->infile_info->final_name,
				O_RDONLY);
		if (infile_exist < 0)
			final_infile_problem(data, cmds_list);
		else
			(*cmds_list)->infile = infile_exist;
	}
	if ((*cmds_list)->files_info->infile_info->final_name == NULL
		&& (*cmds_list)->files_info->infile_info->name)
	{
		return (control_normal_infile(data, cmds_list));
	}
	if ((*cmds_list)->files_info->outfile_info->type == APPENDREDIR)
		flags = O_RDWR | O_APPEND | O_CREAT;
	else if ((*cmds_list)->files_info->outfile_info->type == OUTREDIR)
		flags = O_WRONLY | O_TRUNC | O_CREAT;
	else
		return (1);
	return (control_outfile(cmds_list, flags));
	return (1);
}

void	close_pipe_files(t_exec *cmds_list)
{
	while (cmds_list != NULL)
	{
		if (cmds_list && cmds_list->prev)
		{
			if (cmds_list->prev->pipe_fdi >= 0)
				close(cmds_list->prev->pipe_fdi);
			if (cmds_list->prev->pipe_fdo >= 0)
				close(cmds_list->prev->pipe_fdo);
		}
		cmds_list = cmds_list->next;
	}
}

void	pipe_init(t_data *data, t_exec *cmds_list, int i)
{
	int	fd[2];

	if (data->num_pipe < 1)
		return ;
	else
	{
		if (i < data->total_cmds - 1)
		{
			if (pipe(fd) == -1)
				perror("Pipe Init");
			else
			{
				cmds_list->pipe_fdi = fd[0];
				cmds_list->pipe_fdo = fd[1];
			}
		}
		else
			return ;
	}
}

void	redirection(t_exec *cmds_list, t_data *pipe_info, int i)
{
	if (!cmds_list || !pipe_info)
		perror("cmds_list or pipe_info is NULL\n");
	if (pipe_info->total_cmds == 1)
	{
		only_redirection(&cmds_list);
		return ;
	}
	else if (i == 0)
		first_cmd(cmds_list);
	else if (i == pipe_info->total_cmds - 1)
		last_cmd(cmds_list);
	else
		middle_cmd(cmds_list);
}
