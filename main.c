/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 00:43:23 by sboudcha          #+#    #+#             */
/*   Updated: 2026/08/05 17:59:09 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int initializer(t_data *data){

	if (!init_dongles(data)
	|| !init_coders(data)
	|| !init_heap(data))
		return 1;
	return 0;
}
int	main(int ac, char *av[])
{
	t_data data;
	
	if (ac != 9)
		return (error_message(1, NULL));
	if (validate_args(av + 1, &data.args) == -1)
		return (1);
	initializer(&data);
	return (0);
}
