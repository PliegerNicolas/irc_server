/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 14:53:34 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 11:15:37 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threadpool/ThreadPool.hpp"

/* ************************************************************************** */
/* *                                 PUBLIC                                 * */
/* ************************************************************************** */

/* CONSTRUCTORS & DESTRUCTORS */

ThreadPool::ThreadPool(const size_t	&numThreads):
	_stop(false)
{
	_threads.reserve(numThreads);

	for (size_t i = 0; i < numThreads; ++i)
		_threads.emplace_back([this] { workerThread(); });
}

ThreadPool::ThreadPool(const ThreadPool &other):
	_stop(false)
{
	const size_t	numThreads = other._threads.size();

	_threads.reserve(numThreads);

	for (size_t i = 0; i < numThreads; ++i)
		_threads.emplace_back([this] { workerThread(); });
}

ThreadPool &ThreadPool::operator=(const ThreadPool &other)
{
	if (this != &other)
	{
		{
			std::unique_lock<std::mutex>	lock(_queueMutex);
			_stop = true;
			_queueCondition.notify_all();
		}

		for (auto &thread : _threads)
			if (thread.joinable())
				thread.join();

		const size_t	numThreads = other._threads.size();

		{
			std::unique_lock<std::mutex>	lock(_queueMutex);
			_threads.clear();
			_threads.reserve(numThreads);

			for (size_t i = 0; i < numThreads; ++i)
				_threads.emplace_back([this] { workerThread(); });

			_stop = false;
		}
	}

	return (*this);
}

ThreadPool::ThreadPool(ThreadPool &&other) noexcept:
	_threads(std::move(other._threads)),
	_tasks(std::move(other._tasks)),
	_stop(other._stop)
{
	other._stop = true;
}

ThreadPool &ThreadPool::operator=(ThreadPool &&other) noexcept
{
	if (this != &other)
	{
		_threads = std::move(other._threads);
		_tasks = std::move(other._tasks);
		_stop = other._stop;

		other._stop = true;
	}

	return (*this);
}

ThreadPool::~ThreadPool(void)
{
	{
		std::unique_lock<std::mutex>	lock(_queueMutex);
		_stop = true;
		_queueCondition.notify_all();
	}

	for (auto &thread : _threads)
		if (thread.joinable())
			thread.join();
}

/* GETTERS */

/* SETTERS */

/* MEMBER FUNCTIONS */

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

void	ThreadPool::workerThread(void)
{
	while (true)
	{
		Task	task;

		{
			std::unique_lock<std::mutex>	lock(_queueMutex);
			_queueCondition.wait(lock, [this] { return (_stop || !_tasks.empty()); });

			if (_stop && _tasks.empty())
				return ;

			task = std::move(_tasks.front());
			_tasks.pop();
		}

		task();
	}
}
