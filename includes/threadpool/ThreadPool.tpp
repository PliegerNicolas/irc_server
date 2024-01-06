/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.tpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 14:49:37 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/05 15:03:37 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threadpool/ThreadPool.hpp"

/* ************************************************************************** */
/* *                                 PUBLIC                                 * */
/* ************************************************************************** */

/* CONSTRUCTORS & DESTRUCTORS */

/* GETTERS */

/* SETTERS */

/* MEMBER FUNCTIONS */

template<class F, class... Args>
void	ThreadPool::enqueue(F &&f, Args &&... args)
{
	{
		std::unique_lock<std::mutex>	lock(_queueMutex);

		_tasks.emplace([f, args...]() { f(args...); });
		_queueCondition.notify_one();
	}
}

/* ************************************************************************** */
/* *                                PROTECTED                               * */
/* ************************************************************************** */

/* CONSTRUCTORS & DESTRUCTORS */

/* GETTERS */

/* SETTERS */

/* MEMBER FUNCTIONS */

/* ************************************************************************** */
/* *                                 PRIVATE                                * */
/* ************************************************************************** */

/* CONSTRUCTORS & DESTRUCTORS */

/* GETTERS */

/* SETTERS */

/* MEMBER FUNCTIONS */
