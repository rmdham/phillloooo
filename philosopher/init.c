/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamimsa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 21:14:17 by ahamimsa          #+#    #+#             */
/*   Updated: 2024/12/29 21:14:22 by ahamimsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_args(t_args *args, int argc, char **argv)
{
	if (argc > 6 || argc < 5)
		exit(1);
	args->philo_nbr = ft_atoi(argv[1]);
	args->philo_death_time = ft_atoi(argv[2]);
	args->philo_meal_time = ft_atoi(argv[3]);
	args->philo_sleep_time = ft_atoi(argv[4]);
	if (argv[5])
		args->nbr_of_meals = ft_atoi(argv[5]);
	else
		args->nbr_of_meals = -1;
	if (args->philo_nbr > 200 || args->philo_nbr < 1)
		exit_error("the number of philosophers is incorrect");
	if (args->philo_death_time < 60 || args->philo_meal_time < 60
		|| args->philo_sleep_time < 60)
		exit_error("the other inputs are not correct!");
	return (1);
}

void	init_forks(pthread_mutex_t forks[], t_args *args)
{
	int	i;

	i = 0;
	while (i < args->philo_nbr)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

int	init_philo(t_args *args, t_prog *prog, pthread_mutex_t *forks,
		t_philo *philo)
{
	int	i;

	i = -1;
	prog_init(philo, prog);
	while (i++ < args->philo_nbr)
	{
		philo[i].philo_nbr = args->philo_nbr;
		philo[i].time_to_die = args->philo_death_time;
		philo[i].time_to_eat = args->philo_meal_time;
		philo[i].time_to_sleep = args->philo_sleep_time;
		philo[i].meals_eaten = 0;
		philo[i].start_time = get_time();
		philo[i].id = i + 1;
		philo[i].last_meal_eaten = get_time();
		philo[i].dead_lock = &prog->dead_lock;
		philo[i].meal_lock = &prog->meal_lock;
		philo[i].write_lock = &prog->write_lock;
		philo[i].eating = 0;
		philo[i].nbr_of_meals = args->nbr_of_meals;
		philo[i].dead = &prog->death_flag;
		philo[i].left_fork = &forks[i];
		philo[i].right_fork = &forks[(i - 1 + philo->philo_nbr)
			% philo->philo_nbr];
	}
	return (1);
}

int	init_threads(t_philo *philo, pthread_mutex_t *forks, t_prog *prog)
{
	pthread_t	thread;
	int			i;

	pthread_mutex_init(forks, NULL);
	i = 0;
	if (pthread_create(&thread, NULL, &monitor, prog->philo) != 0)
		exit_error("Error creating monitor thread");
	while (i < philo->philo_nbr)
	{
		pthread_create(&philo[i].thread, NULL, &routine, &philo[i]);
		i++;
	}
	if (pthread_join(thread, NULL) != 0)
		exit_error("Error joining monitor thread");
	i = 0;
	while (i < philo->philo_nbr)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			exit_error("Error joining routine thread");
		i++;
	}
	return (0);
}
