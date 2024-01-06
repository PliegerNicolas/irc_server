/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:05:51 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 13:44:17 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sockets/SocketInfo.hpp"

/* ************************************************************************** */
/* *                                 PUBLIC                                 * */
/* ************************************************************************** */

/* CONSTRUCTORS & DESTRUCTORS */

SocketInfo::SocketInfo(void):
	fd(-1),
	domain(0),
	type(0),
	protocol(0)
{
	std::memset(&addr, 0, sizeof(addr));
	addrSize = 0;
}

SocketInfo::SocketInfo(const int &domain, const int &type, const int &protocol,
	const std::string &endPoint, const std::string &routingPoint):
	fd(-1),
	domain(domain),
	type(type),
	protocol(protocol),
	endPoint(endPoint),
	routingPoint(routingPoint)
{
	std::memset(&addr, 0, sizeof(addr));
	addrSize = 0;

	setAddress();
}

SocketInfo::SocketInfo(const int &newFd):
	fd(newFd),
	domain(0),
	type(0),
	protocol(0)
{
	std::memset(&addr, 0, sizeof(addr));
	addrSize = 0;

	if (fd == -1)
		throw std::runtime_error("Error: SocketInfo - invalid fd passed.");

	getAddress(fd);
}

SocketInfo::SocketInfo(const SocketInfo &other):
	fd(dup(other.fd)),
	domain(other.domain),
	type(other.type),
	protocol(other.protocol),
	endPoint(other.endPoint),
	routingPoint(other.routingPoint),
	hostname(other.hostname),
	addr(other.addr),
	addrSize(other.addrSize)
{
	return ;
}

SocketInfo &SocketInfo::operator=(const SocketInfo &other)
{
	if (this != &other)
	{
		fd = dup(other.fd);
		domain = other.domain;
		type = other.type;
		protocol = other.protocol;
		endPoint = other.endPoint;
		routingPoint = other.routingPoint;
		hostname = other.hostname;
		addr = other.addr;
		addrSize = other.addrSize;
	}

	return (*this);
}

SocketInfo::SocketInfo(SocketInfo &&other) noexcept:
	fd(other.fd),
	domain(other.domain),
	type(other.type),
	protocol(other.protocol),
	endPoint(other.endPoint),
	routingPoint(other.routingPoint),
	hostname(other.hostname),
	addr(other.addr),
	addrSize(other.addrSize)
{
	other.fd = -1;
	std::memset(&other.addr, 0, sizeof(other.addr));
	other.addrSize = 0;
}

SocketInfo &SocketInfo::operator=(SocketInfo &&other) noexcept
{
	if (this != &other)
	{
		fd = other.fd;
		domain = other.domain;
		type = other.type;
		protocol = other.protocol;
		endPoint = other.endPoint;
		routingPoint = other.routingPoint;
		hostname = other.hostname;
		addr = other.addr;
		addrSize = other.addrSize;

		other.fd = -1;
		std::memset(&other.addr, 0, sizeof(other.addr));
		other.addrSize = 0;
	}

	return (*this);
}

SocketInfo::~SocketInfo(void)
{
	if (fd != -1)
		close(fd);
	if (domain == AF_UNIX)
		unlink(endPoint.c_str());
}

/* GETTERS */

/* SETTERS */

void	SocketInfo::setFd(const int &newFd)
{
	if (fd != -1)
		close(fd);
	fd = newFd;
}

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

void	SocketInfo::setAddress(void)
{

	AddressFunc	f;

	switch (domain)
	{
		case AF_UNIX:
			f = &SocketInfo::setAddress_AF_UNIX;
			break ;
		case AF_INET:
			f = &SocketInfo::setAddress_AF_INET;
			break ;
		case AF_INET6:
			f = &SocketInfo::setAddress_AF_INET6;
			break ;
		case 0:
			throw std::runtime_error("Error: SocketInfo - no socket domain passed.");
		default:
			throw std::runtime_error("Error: SocketInfo - unrecognized socket family.");
	}

	(this->*f)();
}

void	SocketInfo::setAddress_AF_UNIX(void)
{
	if (!endPoint.empty() || !routingPoint.empty())
		throw std::runtime_error("Error: SocketInfo - no endpoint nor routingpoint expected.");

	struct sockaddr_un	*unix_addr = reinterpret_cast<struct sockaddr_un *>(&addr);
	std::string			socketPath;

	{
		std::ostringstream	oss;

		oss << "unix_" << getpid() << "_" << time(nullptr);
		socketPath = "/tmp/" + oss.str();
	}

	unix_addr->sun_family = AF_UNIX;
	strncpy(unix_addr->sun_path, socketPath.c_str(), sizeof(unix_addr->sun_path) - 1);
	unix_addr->sun_path[sizeof(unix_addr->sun_path) - 1] = '\0';
	addrSize = sizeof(*unix_addr);

	endPoint = unix_addr->sun_path;
	hostname = unix_addr->sun_path;
}

void	SocketInfo::setAddress_AF_INET(void)
{
	if (endPoint.empty() || routingPoint.empty())
		throw std::runtime_error("Error: SocketInfo - endpoint and routingpoint expected.");

	struct sockaddr_in *ipv4_addr = reinterpret_cast<struct sockaddr_in *>(&addr);

	ipv4_addr->sin_family = AF_INET;
	if (inet_pton(AF_INET, endPoint.c_str(), &ipv4_addr->sin_addr) <= 0)
		throw std::runtime_error("Error: SocketInfo - invalid IP address (protocol V4).");

	{
		int					port = 0;
		std::istringstream	iss(routingPoint);

		if (!(iss >> port))
			throw std::runtime_error("Error: SocketInfo - failed std::string conversion to int.");

		ipv4_addr->sin_port = htons(port);
	}

	{
		char	buffer[NI_MAXHOST];
		if (getnameinfo(reinterpret_cast<struct sockaddr *>(ipv4_addr),
			sizeof(*ipv4_addr), buffer, sizeof(buffer), NULL, 0, 0) == 0)
			hostname = buffer;
	}

	addrSize = sizeof(*ipv4_addr);
}

void	SocketInfo::setAddress_AF_INET6(void)
{
	if (endPoint.empty() || routingPoint.empty())
		throw std::runtime_error("Error: SocketInfo - endpoint and routingpoint expected.");

	struct sockaddr_in6 *ipv6_addr = reinterpret_cast<struct sockaddr_in6 *>(&addr);

	ipv6_addr->sin6_family = AF_INET6;
	if (inet_pton(AF_INET6, endPoint.c_str(), &ipv6_addr->sin6_addr) <= 0)
		throw std::runtime_error("Error: SocketInfo - invalid IP address (protocol V6).");

	{
		int					port = 0;
		std::istringstream	iss(routingPoint);

		if (!(iss >> port))
			throw std::runtime_error("Error: SocketInfo - failed std::string conversion to int.");

		ipv6_addr->sin6_port = htons(port);
	}

	{
		char	buffer[NI_MAXHOST];
		if (getnameinfo(reinterpret_cast<struct sockaddr *>(ipv6_addr),
			sizeof(*ipv6_addr), buffer, sizeof(buffer), NULL, 0, 0) == 0)
			hostname = buffer;
	}

	addrSize = sizeof(*ipv6_addr);
}

void	SocketInfo::getAddress(const int &fd)
{
	addrSize = sizeof(addr);

	if (getsockname(fd, reinterpret_cast<struct sockaddr *>(&addr), &addrSize) == -1)
		throw std::runtime_error("Error: SocketInfo - couldn't get socket information. "
			+ std::string(strerror(errno)));
	domain = addr.ss_family;

	{
		socklen_t	typeLen = sizeof(type);

		if (getsockopt(fd, SOL_SOCKET, SO_TYPE, &type, &typeLen) == -1)
			throw std::runtime_error("Error: SocketInfo - couldn't get socket type. "
				+ std::string(strerror(errno)));
	}

	{
		socklen_t		protocolLen = sizeof(protocol);

		if (getsockopt(fd, SOL_SOCKET, SO_PROTOCOL, &protocol, &protocolLen) == -1)
			throw std::runtime_error("Error: SocketInfo - couldn't get socket protocol. "
				+ std::string(strerror(errno)));
	}

	AddressFunc	f;

	switch (domain)
	{
		case AF_UNIX:
			f = &SocketInfo::getAddress_AF_UNIX;
			break ;
		case AF_INET:
			f = &SocketInfo::getAddress_AF_INET;
			break ;
		case AF_INET6:
			f = &SocketInfo::getAddress_AF_INET6;
			break ;
		default:
			throw std::runtime_error("Error: SocketInfo - unrecognized socket family.");
	}

	(this->*f)();
}

void	SocketInfo::getAddress_AF_UNIX(void)
{
	struct sockaddr_un	*unix_addr = reinterpret_cast<struct sockaddr_un *>(&addr);

	endPoint = unix_addr->sun_path;
	hostname = unix_addr->sun_path;
}

void	SocketInfo::getAddress_AF_INET(void)
{
	struct sockaddr_in	*ipv4_addr = reinterpret_cast<struct sockaddr_in *>(&addr);

	{
		char	ip[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &(ipv4_addr->sin_addr), ip, sizeof(ip));
		endPoint = ip;
	}

	{
		const int			port = ntohs(ipv4_addr->sin_port);
		std::stringstream	ss;

		ss << port;

		routingPoint = ss.str();
	}

	{
		char	hname[NI_MAXHOST];

		if (getnameinfo(reinterpret_cast<struct sockaddr *>(&addr),
			addrSize, hname, sizeof(hname), nullptr, 0, 0) != 0)
			throw std::runtime_error("Error: SocketInfo - failed to get hostname from socket. "
				+ std::string(strerror(errno)));

		hostname = hname;
	}
}

void	SocketInfo::getAddress_AF_INET6(void)
{
	struct sockaddr_in6	*ipv6_addr = reinterpret_cast<struct sockaddr_in6 *>(&addr);

	{
		char	ip[INET6_ADDRSTRLEN];

		inet_ntop(AF_INET6, &(ipv6_addr->sin6_addr), ip, sizeof(ip));
		endPoint = ip;
	}

	{
		const int			port = ntohs(ipv6_addr->sin6_port);
		std::stringstream	ss;

		ss << port;

		routingPoint = ss.str();
	}

	{
		char	hname[NI_MAXHOST];

		if (getnameinfo(reinterpret_cast<struct sockaddr *>(&addr),
			addrSize, hname, sizeof(hname), nullptr, 0, 0) != 0)
			throw std::runtime_error("Error: SocketInfo - failed to get hostname from socket. "
				+ std::string(strerror(errno)));

		hostname = hname;
	}
}
