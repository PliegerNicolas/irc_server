/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 11:24:11 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 14:01:48 by nicolas          ###   ########.fr       */
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
}

Server::Server(const Server &other):
	Sockets(other),
	_epollFd(epoll_create1(EPOLL_CLOEXEC)),
	_threadPool(other._threadPool)
{
	if (_epollFd == -1)
		throw std::runtime_error("Error: Server - couldn't initialize _epollFd.");
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
	return ;
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
