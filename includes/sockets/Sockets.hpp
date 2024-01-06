/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 12:58:52 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 14:01:11 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETS_HPP
# define SOCKETS_HPP

// INCLUDES

# include "sockets/SocketInfo.hpp"

# include <vector>
# include <map>

// MACROS

// FORWARD DECLARATIONS

class Sockets
{
	public:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		Sockets(void);

		Sockets(const Sockets &other);
		Sockets &operator=(const Sockets &other);
		Sockets(Sockets &&other) noexcept;
		Sockets &operator=(Sockets &&other) noexcept;

		virtual ~Sockets(void);

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

	protected:
		/* TYPEDEFS */

		typedef std::map<int, SocketInfo>	SocketsList;

		/* ATTRIBUTES */

		SocketsList		_mySockets;
		SocketsList		_peerSockets;

		/* CONSTRUCTORS & DESTRUCTORS */

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

		void		initialize(const SocketInfo::Params &params);
		void		bindToRoutingPoint(void);
		void		listenForConnections(const int &backlog);
		//SocketInfo	*acceptConnection(void);

		void		setSocketsOption(int level, int option, int value);
		void		setFileDescriptorsOption(int level, int option);

	private:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */
};

#endif // SOCKETS_HPP
