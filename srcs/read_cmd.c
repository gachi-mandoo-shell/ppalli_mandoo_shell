#include "minishell.h"

int	read_cmd(char **input_string)
{
	int	c;

	*input_string = NULL;
	while (read(0, &c, sizeof(c)) > 0)
	{
		if (c == '\n')
			break;
		*input_string = ft_str_char_join(*input_string, c);
		c = 0;
	}
	return (1);
}
