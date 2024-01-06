/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 13:02:53 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 13:23:23 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETINFO_HPP
# define SOCKETINFO_HPP

// INCLUDES

# include <vector>

# include <string>
# include <sstream>
# include <cstring>
# include <stdexcept>

# include <unistd.h>

# include <sys/socket.h>
# include <fcntl.h>

# include <sys/un.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/tcp.h>

// MACROS

// FORWARD DECLARATIONS

class SocketInfo
{
	public:
		/* TYPEDEFS */

		typedef std::vector<SocketInfo>	Params;

		/* ATTRIBUTES */

		int						fd;
		int						domain;
		int						type;
		int						protocol;
		std::string				endPoint;
		std::string				routingPoint;
		std::string				hostname;
		struct sockaddr_storage	addr;
		socklen_t				addrSize;

		/* CONSTRUCTORS & DESTRUCTORS */

		SocketInfo(void);
		SocketInfo(const int &fd);
		SocketInfo(const int &domain, const int &type, const int &protocol,
			const std::string &endPoint, const std::string &routingPoint);

		SocketInfo(const SocketInfo &other);
		SocketInfo &operator=(const SocketInfo &other);
		SocketInfo(SocketInfo &&other) noexcept;
		SocketInfo &operator=(SocketInfo &&other) noexcept;

		virtual ~SocketInfo(void);

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

		void	setFd(const int &newFd);

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

		typedef void (SocketInfo::*AddressFunc)(void);

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

		void	setAddress(void);

		void	setAddress_AF_UNIX(void);
		void	setAddress_AF_INET(void);
		void	setAddress_AF_INET6(void);

		void	getAddress(const int &fd);

		void	getAddress_AF_UNIX(void);
		void	getAddress_AF_INET(void);
		void	getAddress_AF_INET6(void);

		/* STATIC MEMBER FUNCTIONS */
};

#endif // SOCKETINFO_HPP
