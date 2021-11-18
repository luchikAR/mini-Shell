#include "../includes/minishell.h"

void	similar_var(t_env *env, char **arguments)
{
	int	i;
	int	j;

	i = 0;
	while (env->var[i])
	{
		j = 1;
		while (arguments[j])
		{
			if (ft_strncmp(env->var[i], arguments[j],
					ft_strlen(env->var[i])) == 0)
			{
				del_similar_var(env, i);
				i--;
			}
			j++;
		}
		i++;
	}
}

char	**add_envp(char **envp, char **arguments, int words)
{
	char	**new_envp;
	int		i;
	int		j;

	new_envp = malloc(sizeof(char *) * (words + 1));
	i = 0;
	while (*(envp + i))
	{
		new_envp[i] = envp[i];
		i++;
	}
	j = 1;
	while (*(arguments + j))
	{
		new_envp[i] = ft_strdup(arguments[j]);
		i++;
		j++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

char	*val_var_create(char *src, int index)
{
	char	**arr;
	char	*ret;

	if (ft_strchr(src, '='))
	{
		arr = ft_split(src, '=');
		ret = ft_strdup(arr[index]);
		free_arr(arr);
	}
	else
	{
		if (index == 0)
			ret = ft_strdup(src);
		else
			ret = ft_strdup("");
	}
	return (ret);
}

char	**add_var_val(char **arr, char **arguments, int words, int index)
{
	char	**var_val;
	int		i;
	int		j;

	var_val = malloc(sizeof(char *) * (words + 1));
	i = 0;
	while (*(arr + i))
	{
		var_val[i] = arr[i];
		i++;
	}
	j = 1;
	while (*(arguments + j))
	{
		var_val[i] = val_var_create(arguments[j], index);
		i++;
		j++;
	}
	var_val[i] = NULL;
	return (var_val);
}

int	*add_f_equal(char **envp, int words)
{
	int	*arr;
	int	i;

	i = 0;
	arr = malloc(sizeof(int) * (words + 1));
	while (envp[i] != '\0')
	{
		if (ft_strchr(envp[i], '='))
			arr[i] = 2;
		else
			arr[i] = 1;
		i++;
	}
	arr[i] = '\0';
	return (arr);
}
