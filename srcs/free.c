#include "../includes/minishell.h"

void	free_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

void	ft_all_status(t_all *all)
{
	all->pipe = malloc(sizeof(t_lst_pipe));
	all->pipe->next = NULL;
	all->pipe->command = NULL;
	all->pipe->prev = NULL;
	all->pipe->num = -1;
	all->pipe->count_red_pip = -1;
}

void	free_main(char **m_str, char **arguments, t_all *all)
{
	int			i;
	t_lst_pipe	*tmp;

	while (all->pipe->next)
	{
		i = 0;
		tmp = all->pipe;
		all->pipe = all->pipe->next;
		while (tmp->command && tmp->command[i])
		{
			free(tmp->command[i]);
			i++;
		}
		free(tmp->command);
		free(tmp);
	}
	free(all->pipe);
	ft_all_status(all);
	if (m_str[0] != NULL)
	{
		free(m_str[0]);
		(void)arguments;
	}
}
