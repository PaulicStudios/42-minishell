/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:39:45 by vscode            #+#    #+#             */
/*   Updated: 2024/05/12 10:54:28 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	ft_set_current_pwd(t_global *global)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return ;
	ft_set_env_env("PWD", cwd, &global->envv);
	ft_set_env_export("PWD", cwd, &global->env_export);
	free(cwd);
}

void	process_input(char *input, t_global *global)
{
	t_token		*tokens;
	t_ast_node	*ast;

	ast = NULL;
	tokens = NULL;
	ft_set_current_pwd(global);
	tokenize(input, &tokens, 0);
	print_tokens(tokens);
	ft_expand_tokens(tokens, global);
	print_tokens(tokens);
	retokenize(&tokens);
	print_tokens(tokens);
	rearrange_tokens(&tokens);
	print_tokens(tokens);
	if (input_validation(&tokens))
		return (global->exit_status = 2, free_tokens(tokens), (void)0);
	gen_ast(&ast, tokens);
	print_ast(&ast, 0, global);
	ft_org_tokens_for_exec(ast);
	if (ft_is_debug())
		printf("-----\n");
	print_ast(&ast, 0, global);
	ft_exec_all(ast, global);
	ft_free_nodes(ast);
}

static void	ft_execute_input(char *input, t_global *global)
{
	char	**lines;
	size_t	ind;

	lines = ft_split(input, '\n');
	if (lines == NULL)
		return ;
	ind = 0;
	while (lines[ind] && global->should_exit == false)
	{
		global->old_exit_status = global->exit_status;
		add_history(lines[ind]);
		if (validator(lines[ind]))
		{
			ft_print_error("syntax error", NULL);
			global->exit_status = 2;
		}
		else
			process_input(lines[ind], global);
		ind++;
	}
	ft_arr_free((void **)lines);
}

static char	*ft_read_input(t_global *global)
{
	char	*input;
	char	*prompt;
	char	*tmp;

	if (global->isatty)
	{
		prompt = ft_build_prompt();
		if (prompt == NULL)
			return (NULL);
		input = readline(prompt);
		free(prompt);
	}
	else
	{
		tmp = get_next_line(STDIN_FILENO);
		input = ft_strtrim(tmp, "\n");
		free(tmp);
	}
	return (input);
}

int	show_prompt(t_global *global)
{
	char	*input;

	input = ft_read_input(global);
	if (input == NULL)
	{
		if (global->isatty)
			ft_putstr_fd("exit\n", 1);
		return (1);
	}
	ft_execute_input(input, global);
	free(input);
	return (0);
}
