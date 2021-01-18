// Copyright [2021] <Alyona Dorodnyaya>

#include <server.hpp>

using boost::asio::io_service;
using boost::asio::ip::tcp;
using std::chrono::system_clock;

static io_service service;

// user defined class
class talk_to_client {
 private:
  system_clock::time_point time_;
  std::string username_;
  tcp::socket socket_;

 public:
  talk_to_client() : time_(system_clock::now()), socket_(service) {}

  void set_username(const std::string& name) { username_ = name; }
  std::string username() { return username_; }

  bool timed_out() { return (system_clock::now() - time_).count() >= 5; }

  void answer_to_client(std::string message) {
    socket_.write_some(boost::asio::buffer(message));
  }

  tcp::socket& socket() { return socket_; }
};

std::vector<boost::shared_ptr<talk_to_client>> clients;
[[maybe_unused]] boost::recursive_mutex mut;

void accept_thread() {
  tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 8001));
  while (true) {
    BOOST_LOG_TRIVIAL(info)
        << "\nSTART\nThread id: " << std::this_thread::get_id() << "\n";

    boost::shared_ptr<talk_to_client> new_(new talk_to_client());
    acceptor.accept(new_->socket());
    boost::recursive_mutex::scoped_lock lock(mut);

    std::string username;
    new_->socket().read_some(boost::asio::buffer(username, 15));
    if (username.empty()) {
      new_->answer_to_client("\"Login\" is empty");
      BOOST_LOG_TRIVIAL(info) << "\nLogging failed\nThread id: \n"
                              << std::this_thread::get_id() << "\n";
    } else {
      new_->set_username(username);
      clients.push_back(new_);
      new_->answer_to_client("Login OK");
      BOOST_LOG_TRIVIAL(info) << "\nLogging ok\nUsername: " << username << "\n";
    }
  }
}

std::string get_users() {
  std::string users;
  for (const auto& client : clients) users += client->username() + " ";
  return users;
}

void handle_clients_thread() {
  while (true) {
    boost::this_thread::sleep(boost::posix_time::millisec(1));
    boost::recursive_mutex::scoped_lock lock(mut);
    for (auto& client : clients) {
      std::string command;
      client->socket().read_some(boost::asio::buffer(command, 1024));
      if (command == "Clients") client->answer_to_client(get_users());
      if (command == "Ping") {
        if (clients.at(clients.size()) == client)
          client->answer_to_client("Ping OK");
        else
          client->answer_to_client(get_users());
      }
      BOOST_LOG_TRIVIAL(info)
          << "\nAnswered to client. Username: " << client->username() << "\n";
    }
    clients.erase(std::remove_if(clients.begin(), clients.end(),
                                 boost::bind(&talk_to_client::timed_out, _1)),
                  clients.end());
  }
}

int main() {
  boost::thread_group threads;
  threads.create_thread(accept_thread);
  threads.create_thread(handle_clients_thread);
  threads.join_all();
}