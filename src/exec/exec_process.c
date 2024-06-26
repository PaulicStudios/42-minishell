/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 16:23:17 by vscode            #+#    #+#             */
/*   Updated: 2024/05/12 10:53:08 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	ft_buildin_dup(t_process *process, int *dup_stdin, int *dup_stdout)
{
	if (ft_get_fd(process->file_in) != -1)
	{
		*dup_stdin = dup(STDIN_FILENO);
		dup2(ft_get_fd(process->file_in), STDIN_FILENO);
	}
	if (ft_get_fd(process->file_out) != -1)
	{
		*dup_stdout = dup(STDOUT_FILENO);
		dup2(ft_get_fd(process->file_out), STDOUT_FILENO);
	}
}

static void	ft_exec_buildin(t_process *process, t_global *global)
{
	int	dup_stdin;
	int	dup_stdout;

	dup_stdin = -1;
	dup_stdout = -1;
	ft_buildin_dup(process, &dup_stdin, &dup_stdout);
	ft_exec_buildins(process, global);
	if (dup_stdin != -1)
		dup2(dup_stdin, STDIN_FILENO);
	ft_close_fd(&dup_stdin);
	if (dup_stdout != -1)
		dup2(dup_stdout, STDOUT_FILENO);
	ft_close_fd(&dup_stdout);
}

void	ft_execute_process(t_process *process, t_global *global)
{
	if (!ft_exec_buildin_in_fork(process))
	{
		ft_exec_buildin(process, global);
		return ;
	}
	if (process->type == PROCESS_BUILDIN)
		process->type = PROCESS_BUILDIN_FORK;
	process->pid = fork();
	if (process->pid == -1)
	{
		ft_print_error(strerror(errno), process->cmd);
		return ;
	}
	if (process->pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		ft_set_underscore_env(process->cmd, global);
		ft_execute_child_process(process, global);
	}
}

void	ft_wait_for_processes(t_ast_node *node, t_global *global)
{
	if (!node)
		return ;
	ft_wait_for_processes(node->right, global);
	if (node->process)
	{
		if (ft_is_debug())
			printf("waiting for %s...\n", node->process->cmd);
		if ((node->process->type == PROCESS_FORK
				|| node->process->type == PROCESS_BUILDIN_FORK)
			&& node->exit_status == -1)
			node->exit_status = ft_wait_pid(node->process->pid);
	}
	if (node->exit_status != -1)
		global->exit_status = node->exit_status;
	ft_wait_for_processes(node->left, global);
}

void	ft_set_right_exit_code(t_ast_node *node, t_global *global)
{
	if (!node)
		return ;
	if (node->process || node->exit_status != -1)
		global->exit_status = node->exit_status;
	ft_set_right_exit_code(node->left, global);
	ft_set_right_exit_code(node->right, global);
}
