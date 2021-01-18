// Copyright [2020] <Alyona Dorodnyaya>

#include <server.hpp>

int main() {
  boost::thread_group threads;
  threads.create_thread(accept_thread);
  threads.create_thread(handle_clients_thread);
  threads.join_all();
}
