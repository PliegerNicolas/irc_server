/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 11:24:11 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 14:32:27 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

/* ************************************************************************** */
/* *                                 PUBLIC                                 * */
/* ************************************************************************** */

/* CONSTRUCTORS & DESTRUCTORS */

Server::Server(const SocketInfo::Params &params):
	Sockets(),
	_epollFd(epoll_create1(EPOLL_CLOEXEC)),
	_threadPool(NUM_THREADS)
{
	if (_epollFd == -1)
		throw std::runtime_error("Error: Server - couldn't initialize _epollFd.");

	Sockets::initialize(params);

	Sockets::setSocketsOption(SOL_SOCKET, SO_REUSEADDR, 1);
	Sockets::setSocketsOption(SOL_SOCKET, SO_KEEPALIVE, 1);
	Sockets::setSocketsOption(IPPROTO_IPV6, IPV6_V6ONLY, 0);
	Sockets::setSocketsOption(IPPROTO_TCP, TCP_QUICKACK, 1);
	Sockets::setSocketsOption(IPPROTO_TCP, TCP_NODELAY, 1);
	Sockets::setFileDescriptorsOption(F_SETFL, O_NONBLOCK);

	Sockets::bindToRoutingPoint();
	Sockets::listenForConnections(SOMAXCONN);

	for (const auto &socket: _mySockets)
		addToEpoll(socket.second.fd, EPOLLIN | EPOLLHUP
			| EPOLLRDHUP | EPOLLERR | EPOLLET);
}

Server::Server(const Server &other):
	Sockets(other),
	_epollFd(epoll_create1(EPOLL_CLOEXEC)),
	_threadPool(other._threadPool)
{
	if (_epollFd == -1)
		throw std::runtime_error("Error: Server - couldn't initialize _epollFd.");

	for (const auto &socket: _mySockets)
		addToEpoll(socket.second.fd, EPOLLIN | EPOLLOUT | EPOLLHUP
			| EPOLLRDHUP | EPOLLERR | EPOLLET);
}

Server	&Server::operator=(const Server &other)
{
	if (this != &other)
	{
		Sockets::operator=(other);

		if (_epollFd != -1)
			close(_epollFd);

		_epollFd = epoll_create1(EPOLL_CLOEXEC);
		if (_epollFd == -1)
			throw std::runtime_error("Error: Server - couldn't initialize _epollFd.");

		// close all previous addToEpolls() ?

		for (const auto &socket: _mySockets)
			addToEpoll(socket.second.fd, EPOLLIN | EPOLLOUT | EPOLLHUP
				| EPOLLRDHUP | EPOLLERR | EPOLLET);

		_threadPool = other._threadPool;
	}

	return (*this);
}

Server::Server(Server &&other) noexcept:
	Sockets(std::move(other)),
	_epollFd(other._epollFd),
	_threadPool(std::move(other._threadPool))
{
	other._epollFd = -1;
}

Server &Server::operator=(Server &&other) noexcept
{
	if (this != &other)
	{
		Sockets::operator=(std::move(other));

		_epollFd = other._epollFd;
		_threadPool = std::move(other._threadPool);

		other._epollFd = -1;
	}

	return (*this);
}

Server::~Server(void)
{
	if (_epollFd != -1)
	{
		close(_epollFd);
		_epollFd = -1;
	}
}

/* GETTERS */

/* SETTERS */

/* MEMBER FUNCTIONS */

void	Server::launch(void)
{
	struct epoll_event	events[MAX_EVENTS];

	while (true) // set signal value
	{
		int	eventsCount = epoll_wait(_epollFd, events, MAX_EVENTS, -1);

		if (eventsCount == -1)
			throw std::runtime_error("Error: Server - epoll_wait failed. "
				+ std::string(strerror(errno)));

		processEvents(events, eventsCount);
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

void	Server::processEvents(struct epoll_event *events, const int &numEvents)
{
	for (int i = 0; i < numEvents; ++i)
	{
		struct epoll_event	&event = events[i];

		if (_mySockets.find(event.data.fd) != _mySockets.end())
		{
			if (event.events & EPOLLIN)
				_threadPool.enqueue([this]() { this->addClient(); });
			if (event.events & EPOLLHUP || event.events & EPOLLRDHUP || event.events & EPOLLERR)
				(void)event;
		}
		else
		{
			if (event.events & EPOLLIN)
				std::cout << "A client sent data (" << event.data.fd << ")" << std::endl;
			if (event.events & EPOLLOUT)
				std::cout << "A client is waiting for data (" << event.data.fd << ")" << std::endl;
			if ((event.events & EPOLLHUP) || (event.events & EPOLLRDHUP))
			{
				int	clientFd = event.data.fd;
				_threadPool.enqueue([this, clientFd]() { this->removeClient(clientFd); });
			}
			if (event.events & EPOLLERR)
				(void)event;
		}
	}
}

void	Server::addToEpoll(const int &fd, const uint32_t &events)
{
	if (fd == -1)
		throw std::runtime_error("Error: Server - trying to add an invalid fd to epoll. "
			+ std::string(strerror(errno)));

	struct epoll_event	event;
	event.events = events;
	event.data.fd = fd;

	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
		throw std::runtime_error("Error: Server - couldn't add fd to epoll. "
			+ std::string(strerror(errno)));
}

void	Server::updateInEpoll(const int &fd, const uint32_t &events)
{
	if (fd == -1)
		throw std::runtime_error("Error: Server - trying to update an invalid fd to epoll. "
			+ std::string(strerror(errno)));

	struct epoll_event	event;
	event.events = events;
	event.data.fd = fd;

	if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event) == -1)
		throw std::runtime_error("Error: Server - couldn't update fd in epoll. "
			+ std::string(strerror(errno)));
}

void	Server::deleteFromEpoll(const int &fd)
{
	if (fd == -1)
		throw std::runtime_error("Error: Server - trying to remove an invalid fd to epoll. "
			+ std::string(strerror(errno)));

	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr) == -1)
		throw std::runtime_error("Error: Server - couldn't remove fd from epoll. "
			+ std::string(strerror(errno)));
}

void	Server::addClient(void)
{
	std::cout << "Added client" << std::endl;
}

void	Server::removeClient(const int &clientFd)
{
	std::cout << "Removed client" << std::endl;
	(void)clientFd;
}
