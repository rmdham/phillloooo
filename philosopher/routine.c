/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamimsa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 21:16:41 by ahamimsa          #+#    #+#             */
/*   Updated: 2024/12/29 21:16:43 by ahamimsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print(philo, "has taken a fork");
	if (philo->philo_nbr == 1)
	{
		usleep(philo->time_to_die * 1000);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	pthread_mutex_lock(philo->left_fork);
	print(philo, "has taken a fork");
	philo->eating = 1;
	print(philo, "is eating");
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal_eaten = get_time();
	philo->meals_counter++;
	pthread_mutex_unlock(philo->meal_lock);
	usleep(philo->time_to_eat * 1000);
	philo->eating = 0;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	sleeping(t_philo *philo)
{
	print(philo, "sleeping");
	usleep(philo->time_to_sleep * 1000);
}

void	thinking(t_philo *philo)
{
	print(philo, "thinking");
}

int	dead_monitor(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_lock);
	if (*philo->dead == 1)
		return (pthread_mutex_unlock(philo->dead_lock), 1);
	pthread_mutex_unlock(philo->dead_lock);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
	{
		precise_usleep(1);
	}
	while (!dead_monitor(philo))
	{
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}
