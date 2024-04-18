#ifndef EXEC_H
# define EXEC_H

# include <sys/types.h>

# define PIPE_READ 0
# define PIPE_WRITE 1

typedef struct s_process
{
	bool	is_buildin;
	int		file_in;
	int		pipe_fd_in[2];
	int		pipe_fd_out[2];
	int		file_out;
	char	*cmd;
	char	**args;
	pid_t	pid;
	int		exit_status;
}			t_process;

#endif
