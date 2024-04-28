#include "../../inc/minishell.h"

static void	ft_read_here_doc(char *limiter, int fd_pipe[2])
{
	char	*line;

	limiter = ft_strjoin(limiter, "\n");
	if (limiter == NULL)
		return ;
	while (true)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			get_next_line(-1);
			break ;
		}
		ft_putstr_fd(line, fd_pipe[PIPE_WRITE]);
		free(line);
	}
	free(limiter);
}

void	ft_exec_here_doc(t_ast_node *node, t_global *global)
{
	char	*limiter;
	int		fd_pipe[2];
	t_fd	*fd;

	limiter = ft_get_file_name(node);
	if (pipe(fd_pipe) != 0)
		return (ft_print_error(strerror(errno), NULL));
	ft_read_here_doc(limiter, fd_pipe);
	close(fd_pipe[PIPE_WRITE]);
	fd = ft_add_t_fd(global);
	if (!fd)
		return ;
	fd->fd_pipe[PIPE_READ] = fd_pipe[PIPE_READ];
	if (node->left)
	{
		if (!node->left->fd_in[PIPE_READ])
		{
			node->left->fd_in[PIPE_READ] = &fd->fd_pipe[PIPE_READ];
			return ;
		}
	}
	ft_close_fd(&fd->fd_pipe[PIPE_READ]);
}
