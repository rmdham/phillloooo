/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamimsa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 21:15:01 by ahamimsa          #+#    #+#             */
/*   Updated: 2024/12/29 21:15:02 by ahamimsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	prog_init(t_philo *philos, t_prog *prog)
{
	prog->death_flag = 0;
	prog->philo = philos;
	pthread_mutex_init(&prog->dead_lock, NULL);
	pthread_mutex_init(&prog->meal_lock, NULL);
	pthread_mutex_init(&prog->write_lock, NULL);
}

int	main(int argc, char **argv)
{
	t_philo			philo[ALL_PHILOS];
	t_args			args;
	t_prog			prog;
	pthread_mutex_t	forks[ALL_PHILOS];

	// Check the number of command-line arguments.
	if (argc == 5 || argc == 6)
	{
		// Initialize arguments.
		if (!init_args(&args, argc, argv))
			exit_error("erreuuur");
		// Initialize forks.
		init_forks(forks, &args);
		// Initialize philosophers.
		if (!init_philo(&args, &prog, forks, philo))
			exit_error("hmm check again");
		// Initialize threads.
		if (init_threads(philo, forks, &prog))
			exit_error("suiiii");
	}
	else
		// Handle errors and exit if necessary.
		exit_error("what are you doing");
}
