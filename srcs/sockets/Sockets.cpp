/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:18:14 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 14:01:28 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets/Sockets.hpp"

/* ************************************************************************** */
/* *                                 PUBLIC                                 * */
/* ************************************************************************** */

/* CONSTRUCTORS & DESTRUCTORS */

Sockets::Sockets(void)
{
	return ;
}

Sockets::Sockets(const Sockets &other):
	_mySockets(other._mySockets),
	_peerSockets(other._peerSockets)
{
	return ;
}

Sockets &Sockets::operator=(const Sockets &other)
{
	if (this != &other)
	{
		_mySockets = other._mySockets;
		_peerSockets = other._peerSockets;
	}

	return (*this);
}

Sockets::Sockets(Sockets &&other) noexcept:
	_mySockets(std::move(other._mySockets)),
	_peerSockets(std::move(other._peerSockets))
{
	return ;
}

Sockets &Sockets::operator=(Sockets &&other) noexcept
{
	if (this != &other)
	{
		_mySockets = std::move(other._mySockets);
		_peerSockets = std::move(other._peerSockets);
	}

	return (*this);
}

Sockets::~Sockets(void)
{
	return ;
}

/* GETTERS */

/* SETTERS */

/* MEMBER FUNCTIONS */

void	Sockets::initialize(const SocketInfo::Params &params)
{
	for (const auto &p : params)
	{
		const int socketFd = socket(p.domain, p.type, p.protocol);

		if (socketFd == -1)
			throw std::runtime_error("Error: Sockets - couldn't initialize socket. "
				+ std::string(strerror(errno)));

		_mySockets.emplace(socketFd, SocketInfo(p));
		_mySockets[socketFd].setFd(socketFd);
	}
}

void		Sockets::bindToRoutingPoint(void)
{
	for (auto &socketInfoPair : _mySockets)
	{
		auto &socketInfo = socketInfoPair.second;

		if (bind(socketInfo.fd,
			reinterpret_cast<struct sockaddr *>(&socketInfo.addr),
			socketInfo.addrSize) == -1)
			throw std::runtime_error("Error: Sockets - couldn't bind endpoint to routingpoint. "
				+ std::string(strerror(errno)));
	}
}

void		Sockets::listenForConnections(const int &backlog)
{
	for (const auto &socketInfoPair : _mySockets)
	{
		const auto &socketInfo = socketInfoPair.second;

		if (listen(socketInfo.fd, backlog) == -1)
			throw std::runtime_error("Error: Sockets - couldn't listen to Socket. "
				+ std::string(strerror(errno)));
	}
}

/*
SocketInfo	*Sockets::acceptConnection(void)
{
	for (auto &socketInfoPair : _mySockets)
	{
		auto &socketInfo = socketInfoPair.second;

		const int socketFd = accept(socketInfo.fd,
			reinterpret_cast<struct sockaddr *>(&socketInfo.addr),
			&socketInfo.addrSize);

		if (socketFd != -1)
		{
			_peerSockets.emplace(socketFd, SocketInfo(socketFd));
			return (&_peerSockets[socketFd]);
		}
	}

	throw std::runtime_error("Error: Sockets - couldn't accept connection. "
		+ std::string(strerror(errno)));
}
*/

void	Sockets::setSocketsOption(int level, int option, int value)
{
	for (const auto &socketInfoPair : _mySockets)
	{
		const SocketInfo	&socketInfo = socketInfoPair.second;

		if ((level != socketInfo.type)
			|| ((socketInfo.addr.ss_family != AF_INET6) && (level == IPPROTO_IPV6)))
			return ;

		if (setsockopt(socketInfo.fd, level, option, &value, sizeof(value)) != 0)
			throw std::runtime_error("Error: Sockets - couldn't set socket option. "
				+ std::string(strerror(errno)));
	}
}

void	Sockets::setFileDescriptorsOption(int level, int option)
{
	for (const auto &socketInfoPair : _mySockets)
	{
		const SocketInfo	&socketInfo = socketInfoPair.second;

		if (fcntl(socketInfo.fd, level, option) != 0)
			throw std::runtime_error("Error: Sockets - couldn't set file descriptor option. "
				+ std::string(strerror(errno)));
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
