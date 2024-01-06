/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATarget.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 14:35:56 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 14:36:49 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ATARGET_HPP
# define ATARGET_HPP

// INCLUDES

// MACROS

// FORWARD DECLARATIONS

class ATarget
{
	public:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		ATarget(void);

		ATarget(const ATarget &other);
		ATarget &operator=(const ATarget &other);
		ATarget(ATarget &&other) noexcept;
		ATarget &operator=(ATarget &&other) noexcept;

		virtual ~ATarget(void);

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

	protected:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */

	private:
		/* TYPEDEFS */

		/* ATTRIBUTES */

		/* CONSTRUCTORS & DESTRUCTORS */

		/* GETTERS */

		/* SETTERS */

		/* MEMBER FUNCTIONS */
};

#endif // ATARGET_HPP
