/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 14:37:28 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 14:38:17 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

// INCLUDES

# include "target/Atarget.hpp"

// MACROS

// FORWARD DECLARATIONS

class Channel: public ATarget
{
	public:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		Channel(void);

		Channel(const Channel &other);
		Channel &operator=(const Channel &other);
		Channel(Channel &&other);
		Channel &operator=(Channel &&other);

		virtual ~Channel(void);

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

#endif // CHANNEL_HPP

