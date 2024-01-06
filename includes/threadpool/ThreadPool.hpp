/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 14:49:37 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 10:46:39 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADPOOL_HPP
# define THREADPOOL_HPP

// INCLUDES

# include <thread>
# include <mutex>
# include <condition_variable>

# include <vector>
# include <queue>

# include <functional>

// MACROS

// FORWARD DECLARATIONS

class ThreadPool
{
	public:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		explicit ThreadPool(const size_t &numThreads);

		ThreadPool(const ThreadPool &other);
		ThreadPool &operator=(const ThreadPool &other);
		ThreadPool(ThreadPool &&other) noexcept;
		ThreadPool &operator=(ThreadPool &&other) noexcept;

		virtual ~ThreadPool(void);

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

		template <class F, class... Args>
		void	enqueue(F &&f, Args &&...args);

		/* STATIC MEMBER FUNCTIONS */

	protected:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

		/* STATIC MEMBER FUNCTIONS */

	private:
		/* TYPEDEFS */

		typedef std::vector<std::thread>	Threads;
		typedef std::function<void()>		Task;
		typedef std::queue<Task>			Tasks;

		/* ATTRIBUTES */

		Threads						_threads;
		Tasks						_tasks;

		std::mutex					_queueMutex;
		std::condition_variable		_queueCondition;
		bool						_stop;

		/* CONSTRUCTORS & DESTRUCTORS */

		ThreadPool(void) = delete;

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

		void	workerThread(void);

		/* STATIC MEMBER FUNCTIONS */
};

# include "threadpool/ThreadPool.tpp"

#endif // THREADPOOL_HPP
