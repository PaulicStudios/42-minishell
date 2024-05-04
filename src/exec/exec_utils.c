#include "../../inc/minishell.h"

char	*ft_get_file_name(t_ast_node *node)
{
	t_token	*token;

	if (node->right)
	{
		if (node->right->token)
		{
			token = node->right->token;
			while (token)
			{
				if (token->type == TOKEN_ARG)
					return (token->value);
				token = token->next;
			}
		}
	}
	return (NULL);
}

static char	**ft_init_args_arr(char *cmd)
{
	char	**args;

	args = ft_arr_create_len(2);
	if (args == NULL)
		return (NULL);
	args[0] = ft_strdup(cmd);
	if (args[0] == NULL)
	{
		ft_arr_free((void **) args);
		return (NULL);
	}
	return (args);
}

char	**ft_fill_args(t_token *token, char *cmd)
{
	char	**args;
	char	*tmp;

	args = ft_init_args_arr(cmd);
	if (args == NULL)
		return (NULL);
	token = token->next;
	while (token)
	{
		if (token->type == TOKEN_ARG)
		{
			tmp = ft_strdup(token->value);
			if (tmp == NULL)
				return (ft_arr_free((void **) args), NULL);
			if (!ft_arr_add(tmp, &args))
			{
				ft_arr_free((void **) args);
				return (NULL);
			}
		}
		token = token->next;
	}
	return (args);
}

void	ft_handle_verify_process_error(t_process *process)
{
	if (process->cmd && process->cmd[0] == '\0')
	{
		process->exit_status = 0;
		return ;
	}
	if (errno != 0)
	{
		ft_print_error(strerror(errno), process->cmd);
		if (errno != 21 && errno != 13)
			process->exit_status = 127;
		else
			process->exit_status = 126;
	}
	else
	{
		ft_print_error("command not found", process->cmd);
		process->exit_status = 127;
	}
}

bool	ft_exec_buildin_in_fork(t_process *process)
{
	return (!(process->type == PROCESS_BUILDIN && process->fd_in[0] == NULL && process->fd_in[1] == NULL && process->fd_out[0] == NULL && process->fd_out[1] == NULL));
}
