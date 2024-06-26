/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_buildin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vscode <vscode@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 16:21:51 by vscode            #+#    #+#             */
/*   Updated: 2024/05/11 16:21:52 by vscode           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*ft_cd_dir_rel(t_process *process)
{
	char	*cwd;
	char	*new_path;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_error_buildin(strerror(errno), process->args[1], process, 1);
		return (NULL);
	}
	new_path = ft_strjoin(cwd, "/");
	free(cwd);
	if (!new_path)
		return (NULL);
	cwd = ft_strjoin(new_path, process->args[1]);
	free(new_path);
	return (cwd);
}

static char	*ft_cd_env(const char *env, t_process *process, t_global *global)
{
	char	*new_pwd;
	char	*err_msg;

	new_pwd = ft_get_env((char *)env, global->envv);
	if (!new_pwd)
	{
		err_msg = ft_strjoin(env, " not set");
		ft_error_buildin(err_msg, NULL, process, 1);
		free(err_msg);
	}
	return (new_pwd);
}

static void	ft_set_oldpwd(t_global *global)
{
	char	*value;

	value = ft_get_env("PWD", global->envv);
	if (value)
	{
		ft_set_env_export("OLDPWD", value, &global->env_export);
		ft_set_env_env("OLDPWD", value, &global->envv);
		free(value);
	}
}

void	ft_cd_buildin(t_process *process, t_global *global)
{
	char	*temp;

	if (!process->args[1])
		temp = ft_cd_env("HOME", process, global);
	else if (ft_strncmp(process->args[1], "--", 3) == 0)
		temp = ft_cd_env("HOME", process, global);
	else if (process->args[2] != NULL)
		return (ft_error_buildin("too many arguments", NULL, process, 1));
	else if (ft_strncmp(process->args[1], "-", 2) == 0)
	{
		temp = ft_cd_env("OLDPWD", process, global);
		if (temp)
			printf("%s\n", temp);
	}
	else if (process->args[1][0] == '/' || process->args[1][0] == '.')
		temp = ft_strdup(process->args[1]);
	else
		temp = ft_cd_dir_rel(process);
	ft_set_oldpwd(global);
	if (!temp)
		return ;
	if (chdir(temp) != 0)
		ft_error_buildin(strerror(errno), process->args[1], process, 1);
	free(temp);
}
