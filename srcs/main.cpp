/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 11:13:26 by nicolas           #+#    #+#             */
/*   Updated: 2024/01/06 11:16:26 by nicolas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threadpool/ThreadPool.hpp"

#include <iostream>

std::mutex	writeMutex;

class MyClass
{
	public:
		MyClass(void)
		{
			std::unique_lock<std::mutex>	lock(writeMutex);
			std::cout << "Constructor called" << std::endl;
		}
		~MyClass(void)
		{
			std::unique_lock<std::mutex>	lock(writeMutex);
			std::cout << "Destructor called" << std::endl;
		}

		void memberFunction(const int &nbr)
		{
			std::unique_lock<std::mutex>	lock(writeMutex);
			std::cout << "MemberFunction: " << nbr << std::endl;
		}
};

void	staticFunction(const int &nbr)
{
	std::unique_lock<std::mutex>	lock(writeMutex);
	std::cout << "StaticFunction: " << nbr << std::endl;
}

int	main(void)
{
	ThreadPool	pool1(5);
	ThreadPool	pool2(pool1);
	MyClass		obj;

	pool1.enqueue([&obj]() { obj.memberFunction(0); });
	pool1.enqueue([]() { staticFunction(-10); });

	pool1 = pool2;

	pool2.enqueue([&obj]() { obj.memberFunction(0); });
	pool2.enqueue([]() { staticFunction(-10); });

	return (0);
}
