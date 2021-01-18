// Copyright [2021] <Alyona Dorodnyaya>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/thread.hpp>
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

void accept_thread();
void handle_clients_thread();

#endif  // INCLUDE_HEADER_HPP_
