/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 11:22:09 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 15:01:17 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

// INCLUDES

# include "sockets/Sockets.hpp"
# include "threadpool/ThreadPool.hpp"

# include <iostream>

# include <unistd.h>
# include <sys/epoll.h>

// MACROS

# define MAX_EVENTS 150
# define NUM_THREADS 5

// FORWARD DECLARATIONS

class Server: public Sockets
{
	public:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		Server(const SocketInfo::Params &params);

		Server(const Server &other);
		Server &operator=(const Server &other);
		Server(Server &&other) noexcept;
		Server &operator=(Server &&other) noexcept;

		virtual ~Server(void);

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

		void	launch(void);

	protected:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

	private:
		/* TYPEDEFS */

		typedef std::map<int, SocketInfo>	Clients;

		/* ATTRIBUTES */

		int				_epollFd;
		ThreadPool		_threadPool;

		Clients			_clients;

		/* CONSTRUCTORS & DESTRUCTORS */

		Server(void) = delete;

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

		void	processEvents(struct epoll_event *events, const int &numEvents);

		void	addClient(void);
		void	removeClient(const int &clientFd);

		void	addToEpoll(const int &fd, const uint32_t &events);
		void	updateInEpoll(const int &fd, const uint32_t &events);
		void	deleteFromEpoll(const int &fd);
};

#endif // SERVER_HPP
