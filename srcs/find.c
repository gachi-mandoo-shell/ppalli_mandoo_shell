#include "minishell.h"

char	*find_env_value(char *env_name, char **envp)
{
	int	i;
	int	env_len;

	i = -1;
	env_len = ft_strlen(env_name);
	while (envp[++i])
	{
		if (!ft_strncmp(env_name, envp[i], env_len))
		{
			if (envp[i][env_len] == '=')
				return (ft_strchr(envp[i], '=') + 1);
		}
	}
	return (NULL);
}

int		find_cmd_path(t_cmd *cmd_list, char **envp)
{
	char		*env_value;
	char		**path;
	int			i;
	struct stat	buf;

	env_value = find_env_value("PATH", envp);
	path = ft_split(env_value, ':');
	i = -1;
	while (path[++i])
	{
		path[i] = ft_strjoin(ft_strjoin(path[i], "/", 1), cmd_list->cmd_name, 1);
		if (!stat(path[i], &buf))
		{
			cmd_list->cmd_name = ft_strdup(path[i]);
			break ;
		}
	}
	// if (!path[i])
		// command not found
	i = -1;
	while (path[++i])
		free(path[i]);
	free(path);
	return (run_process(cmd_list, envp));
}

char	*get_env_value(char *arg, char **envp)
{
	char	*env_name;
	char	*result;
	int		flag;
	int		i;

	flag = 0;
	i = 0;
	result = NULL;
	env_name = NULL;
	while (arg[i])
	{
		if (arg[i] == '\'')
		{
			if (!(flag & D_QUOTE))				// if d_quote off
				flag |= S_QUOTE;				// s_quote on
			i++;
		}
		else if (arg[i] == '\"')
		{
			if (!(flag & S_QUOTE))				// if s_quote off
				flag |= D_QUOTE;				// d_quote on
			i++;
		}
		else
		{
			if (flag & S_QUOTE)
			{
				while (flag & S_QUOTE)
				{
					result = ft_str_char_join(result, arg[i++]);
					if (arg[i] == '\'')
						flag |= S_QUOTE;
				}
				i++;
			}
			else if (flag & D_QUOTE)
			{
				while (flag & D_QUOTE)
				{
					if (arg[i] == '\\')
					{
						if (arg[i + 1] == '\"' || arg[i + 1] == '\\' || arg[i + 1] == '`' || arg[i + 1] == '$')
							result = ft_str_char_join(result, arg[++i]);
						else
							result = ft_str_char_join(result, arg[i]);
						i++;
					}
					else if (arg[i] == '$')
					{
						// if (arg[i - 1] == '\\')
						// 	result = ft_str_char_join(result, arg[i++]);
						// else
						// {
						while (!ft_strchr(" \t\n$\"\'\\", arg[++i]))		// $HOME -> /Users/jkeum
							env_name = ft_str_char_join(env_name, arg[i]);
						result = ft_strjoin(result, find_env_value(env_name, envp));
						free(env_name);
						// }
					}
					else if (arg[i] == '\"')
					{
						flag ^= D_QUOTE;
						i++;
					}
				}
			}
			else
			{
				if (arg[i] == '$')
				{
					while (!ft_strchr(" \t\n$\"\'\\", arg[++i]))		// $HOME -> /Users/jkeum
						env_name = ft_str_char_join(env_name, arg[i]);
					result = ft_strjoin(result, find_env_value(env_name, envp));
					free(env_name);
				}
				else
					result = ft_str_char_join(result, arg[i++]);
			}
		}
	}
	return (result);
}

// int		main(int argc, char *argv[], char **envp)
// {
// 	char	arg[] = "hello\"  hi   $HOME\\$HOME\"world$PWD\\$HOME";

// 	printf("%s\n", get_env_value(arg, envp));
// 	return (0);
// }