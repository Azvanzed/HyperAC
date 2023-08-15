#include <server.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

bool server::sendServer(request_header_t* request, response_header_t** response) {
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);

	tcp::socket socket(io_service);
	boost::asio::connect(socket, resolver.resolve({ SERVER_HOST, SERVER_PORT }));

	boost::asio::write(socket, boost::asio::buffer(request, request->size));
	if (response) {
		response_header_t header;
		boost::asio::read(socket, boost::asio::buffer(&header, sizeof(response_header_t)));

		*response = (response_header_t*)malloc(header.size);
		**response = header;

		void* next_buffer = (void*)((uint64_t)*response + sizeof(response_header_t));
		size_t next_size = header.size - sizeof(response_header_t);
		boost::asio::read(socket, boost::asio::buffer(next_buffer, next_size));
	}

	socket.close();
	return true;
}