/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sokaraku <sokaraku@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:25:38 by sokaraku          #+#    #+#             */
/*   Updated: 2024/10/05 19:04:19 by sokaraku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts the number of a given char c inside a string str.
 * @param str A pointer to a string.
 * @param c The character to count.
 * @returns The number of times a character appeared inside the string.
 */
short int	count_char(char *str, char c)
{
	short int	n;

	if (!str)
		return (0);
	n = 0;
	while (*str)
	{
		if (*str == c)
			n++;
		str++;
	}
	return (n);
}

/**
 * @brief A simple strjoin, but it also puts a space between the two strings,
 to facilitate the split afterwards.
 * @param s1 A pointer to the first string to copy.
 * @param s2 a pointer to the second string to copy.
 * @param c The char to put in the middle of s1 and s2.
 * @returns The merged strings, with a char c in the middle.
 * Returns NULL if the allocation failed or if the two strings are NULL.
 */
char	*merge_strings(char *s1, char *s2, char c)
{
	size_t	i;
	size_t	j;
	char	*new;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	new = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 2);
	if (!new)
		return (NULL);
	while (s1 && s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	if (s1)
		new[i++] = c;
	while (s2 && s2[j])
		new[i++] = s2[j++];
	new[i] = 0;
	free(s1);
	return (new);
}
/**
 * @brief Checks if a command is a builtin or not.
 * @param cmd The string to check.
 * @returns True (1) if the command is a builtin and false (0) otherwise.
 */

bool	is_builtin(char *cmd)
{
	static char	*builtins[] = {"echo", "cd", "pwd", "export", "unset", "env",
		"exit"};
	__int8_t	i;

	i = 0;
	while (i < 7)
	{
		if (!ft_strcmp(cmd, builtins[i]))
			return (true);
		i++;
	}
	return (false);
}

/**
 * @brief Frees multiple pointers using variadic arguments.
 * @param size The number of pointers to free.
 * @param ... The pointer(s).
 * @returns void.
 */
void	free_multiple_pointers(int size, ...)
{
	va_list	arg;

	va_start(arg, size);
	while (size--)
		free(va_arg(arg, void *));
	va_end(arg);
}

/**
 * @brief Used for handling memory. Checks if a malloc'd string can be freed
 * without generting a double free in the allocated array of strings strs.
 * @param s1 The malloc'd string.
 * @param strs An array of malloc'd strings, which possibly contains s1.
 * @returns true if s1 isn't stored in strs, and false otherwise.
 */
bool	is_diff(char *s1, char **strs)
{
	unsigned short int	i;

	if (!strs || !s1)
		return (true);
	i = -1;
	while (strs[++i])
	{
		if (s1 == strs[i])
			return (false);
	}
	return (true);
}
