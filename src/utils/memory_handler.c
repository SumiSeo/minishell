/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:02:58 by sokaraku          #+#    #+#             */
/*   Updated: 2024/10/05 19:28:42 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees the t_data structure.
 * @param data A pointer to the data structure.
 * @returns void.
 */
static void	free_data_ptr(t_data *data)
{
	if (data)
	{
		free_multiple_pointers(2, data->fd_hd, data->pids);
		free_arrs((void **)data->hd_files);
		free(data);
	}
}

/**
 * @brief Frees the environment list.
 * @param env_list The head of the environment list.
 * @returns void.
 */
void	free_env_list(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list;
		env_list = env_list->next;
		free(tmp->variable);
		free(tmp);
	}
}

/**
 * @brief Stores or frees the line, the execution list and the environment list.
 * @param line The line to store or free.
 * @param exec The head of the execution list.
 * @param store A boolean being true (1) if the line, the execution list and the
 * environment list must be stored, and false (0) otherwise.
 * @param free_env A boolean being true (1) if the environment list must be
 * freed, and false (0) otherwise.
 * @returns void.
 */
void	store_or_free(char *line, t_exec *exec, bool store, bool free_env)
{
	static char		*line_store = NULL;
	static t_exec	*exec_store = NULL;
	static t_env	*env_list_store = NULL;
	static t_data	*data_store = NULL;

	if (store)
	{
		line_store = line;
		exec_store = exec;
		env_list_store = exec->data->env_list;
		data_store = exec->data;
		return ;
	}
	free_all(line_store, exec_store, env_list_store, free_env);
}

/**
 * @brief Frees a list of tokens.
 * @param head The head of the tokens' list.
 * @returns void.
 */
void	free_tokens(t_tokens *tokens_head, bool all)
{
	t_tokens	*tmp;
	__int8_t	id_cmd;

	if (!tokens_head)
		return ;
	id_cmd = 0;
	while (tokens_head)
	{
		tmp = tokens_head;
		tokens_head = tokens_head->next;
		if (all)
			free(tmp->word);
		else if (all == false && tmp->type > APPENDREDIR)
			free(tmp->word);
		if (all && id_cmd == tmp->id_cmd)
		{
			if (tmp->cmd_array)
			{
				free_arrs((void **)tmp->cmd_array);
				id_cmd++;
			}
		}
		free(tmp);
	}
}

/**
 * @brief Frees a list of executions.
 * @param exec_head The head of the executions' list.
 * @param all A boolean being true (1) if all the memory must be freed, and
 * false (0) otherwise.
 * @returns void.
 */
void	free_exec(t_exec *exec_head, bool all)
{
	t_exec	*tmp;
	t_data	*data;
	t_files	*files;

	data = exec_head->data;
	while (exec_head)
	{
		tmp = exec_head;
		exec_head = exec_head->next;
		files = tmp->files_info;
		if (all)
		{
			if (data && is_diff(files->infile_info->name, data->hd_files))
				free(files->infile_info->name);
			if (files->infile_info->is_heredoc)
				free(files->infile_info->del);
			free(files->outfile_info->name);
			free_arrs((void **)tmp->cmd_array);
		}
		free_multiple_pointers(5, files->infile_info, files->outfile_info,
			files, tmp->path, tmp);
	}
	free_data_ptr(data);
}
