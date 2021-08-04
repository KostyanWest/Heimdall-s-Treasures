#include <boost/asio.hpp>
#include "client.hpp"



using namespace boost::asio;
using namespace heimdall::engine::network;

inline io_service boost_asio_service;
#define SERVICE boost_asio_service
