/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 11:13:26 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 14:03:45 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include <iostream>

int	main(void)
{
	try
	{
		SocketInfo::Params	params;

		params.reserve(3);
		params.emplace_back(SocketInfo(AF_INET6, SOCK_STREAM, IPPROTO_TCP, "::", "5000"));
		params.emplace_back(SocketInfo(AF_INET, SOCK_STREAM, IPPROTO_TCP, "192.168.1.11", "5001"));
		params.emplace_back(SocketInfo(AF_UNIX, SOCK_STREAM, 0, "", ""));

		Server	server(params);

		server.launch();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return (0);
}
