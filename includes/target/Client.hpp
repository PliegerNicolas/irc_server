/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 14:36:55 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 14:39:09 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

// INCLUDES

# include "target/ATarget.hpp"
# include "sockets/SocketInfo.hpp"

// MACROS

// FORWARD DECLARATIONS

class Client: public ATarget
{
	public:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		Client(void);

		Client(const Client &other);
		Client &operator=(const Client &other);
		Client(Client &&other);
		Client &operator=(Client &&other);

		virtual ~Client(void);

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

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

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

		/* STATIC MEMBER FUNCTIONS */
};

#endif // CLIENT_HPP
