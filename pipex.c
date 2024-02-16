/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzachari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:39:44 by nzachari          #+#    #+#             */
/*   Updated: 2024/02/16 14:19:59 by nzachari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>

int	main(int argc, char *av[])
{
	if (argc != 5)
	{
		printf("must have 5 args");
		return (0);
	}
	printf("small win");
}
