#include "../../inc/minishell.h"

bool	ft_execute_process(t_process *process, char **envp)
{
	process->pid = fork();
	if (process->pid == -1)
		return (false);
	if (process->pid == 0)
	{
		if (process->pipe_fd_in[PIPE_READ] != -1)
			dup2(process->pipe_fd_in[PIPE_READ], STDIN_FILENO);
		ft_close_fd(&process->pipe_fd_in[PIPE_READ]);
		ft_close_fd(&process->pipe_fd_in[PIPE_WRITE]);
		if (process->pipe_fd_out[PIPE_WRITE] != -1)
			dup2(process->pipe_fd_out[PIPE_WRITE], STDOUT_FILENO);
		ft_close_fd(&process->pipe_fd_out[PIPE_READ]);
		ft_close_fd(&process->pipe_fd_out[PIPE_WRITE]);
		if (ft_is_buildin_cmd(process->cmd))
		{
			exit(ft_exec_buildins(process, envp));
			//Free
		}
		else
		{
			execve(process->cmd, process->args, envp);
			exit(1);
		}
		//ToDo: Handle Error
	}
	ft_close_fd(&process->pipe_fd_in[PIPE_READ]);
	ft_close_fd(&process->pipe_fd_in[PIPE_WRITE]);
	return (true);
}