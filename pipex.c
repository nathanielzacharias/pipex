/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzachari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:39:44 by nzachari          #+#    #+#             */
/*   Updated: 2024/02/16 16:09:03 by nzachari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*access_ok(char *cmd)
{
	const char	*pathvar;
	char		**indv_paths;
	char		*pathwcmd;
	int			access_check;
	ssize_t		i;

	pathvar = get_pathvar();
	indv_paths = ft_split(pathvar, ':');
	i = -1;
	while (indv_paths[++i])
	{
		pathwcmd = ft_strjoin(ft_strjoin(indv_paths[i], "/"), cmd);
		access_check = access(pathwcmd, X_OK);
		if (access_check == 0)
		{
			free(indv_paths);
			return (pathwcmd);
		}
		free(pathwcmd);
	}
	free(indv_paths);
	return (NULL);
}

int	parse_pipex(char *cmd, int pid, int pipe_end)
{
	char	**cmd_args;
	int		checkdupchild;
	int		checkdupparent;
	char	*binpath;

	cmd_args = ft_split(cmd, ' ');
	binpath = access_ok(cmd_args[0]);
	if (!binpath)
		return (errno = EACCES, perror("access() denied"), 1);
	if (pid == 0)
	{
		checkdupchild = dup2(pipe_end, STDOUT_FILENO);
		if (checkdupchild < 0)
			return (perror("dup2() failed in child"), 1);
		execve(binpath, cmd_args, (char *const *)environ);
	}
	else
	{
		checkdupparent = dup2(pipe_end, STDIN_FILENO);
		if (checkdupparent < 0)
			return (perror("dup2() failed in parent"), 1);
		execve(binpath, cmd_args, (char *const *)environ);
		perror("In parent second child: ");
	}
	return (0);
}

int	has_errors(int ac, char *av[], t_fdstruct *fds)
{
	int	pipe_success;

	if (ac != 5)
		return (errno = EINVAL, perror("Program requires 5 args"), 1);
	pipe_success = pipe(fds->pipefd);
	if (pipe_success < 0)
		return (errno = EPIPE, perror("pipe_success < 0"), 1);
	fds->in_fd = open(av[1], O_RDONLY);
	fds->out_fd = open(av[ac -1], O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (fds->in_fd < 0 || fds->out_fd < 0)
		return (errno = EBADFD, perror("open() returns -1"), 1);
	return (0);
}

void	dup_close_andparse(t_fdstruct *fds, int pipein, char *av[])
{
	if (pipein == 1)
	{
		dup2(fds->in_fd, STDIN_FILENO);
		close(fds->in_fd);
		close(fds->pipefd[0]);
		parse_pipex(av[2], 0, fds->pipefd[1]);
	}
	else
	{
		dup2(fds->out_fd, STDOUT_FILENO);
		close(fds->out_fd);
		close(fds->pipefd[1]);
		parse_pipex(av[3], 1, fds->pipefd[0]);
	}
}

int	main(int ac, char *av[])
{
	int			pid;
	t_fdstruct	fds;

	if (has_errors(ac, av, &fds))
		return (1);
	pid = fork();
	if (pid < 0)
		return (errno = ESRCH, perror("pid < 0"), 1);
	if (pid == 0)
		dup_close_andparse(&fds, 1, av);
	else
	{
		waitpid(pid, NULL, WNOHANG);
		pid = fork();
		if (pid < 0)
			return (errno = ESRCH, perror("pid < 0"), 1);
		if (pid == 0)
			dup_close_andparse(&fds, 0, av);
	}
	return (0);
}
