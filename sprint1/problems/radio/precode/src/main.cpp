#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <string_view>

#include "audio.h"

namespace net = boost::asio;
using net::ip::udp;

using namespace std::literals;

void StartServer(uint16_t port);
void StartClient(uint16_t port);

int main(int argc, char** argv) {

    if (argc != 3) {
        std::cout << "Usage: "sv << argv[0] << " <client | server> <port> "sv << std::endl;
        return 1;
    }
    
    std::string_view type = argv[1];
    const int port = std::stoi(argv[2]);
    
    if (type == "client") {
        StartClient(port);        
    }
    else if (type == "server") {
        StartServer(port);
    }    
    
    return 0;
}

void StartServer(uint16_t port) {
    net::io_context io_context;
    udp::socket socket(io_context, udp::v4());
    std::array<char, 1024> recv_buf;
    udp::endpoint remote_endpoint;
    Player player(ma_format_u8, 1);
    
    while (true) {
        //player.PlayBuffer(rec_result.data.data(), rec_result.frames, 1.5s);
        auto size = socket.receive_from(net::buffer(recv_buf), remote_endpoint);

        std::cout << "Client said "sv << std::string_view(recv_buf.data(), size) << std::endl;
        std::cout << "Playing done" << std::endl;
    }
}

void StartClient(uint16_t port) {
    Recorder recorder(ma_format_u8, 1);
    net::io_context io_context;
    udp::socket socket(io_context, udp::v4());
    boost::system::error_code ec;

    while (true) {
        std::string str;

        std::cout << "Press IP to send message..." << std::endl;
        std::getline(std::cin, str);
        
        auto rec_result = recorder.Record(65000, 1.5s);
        std::cout << "Recording done" << std::endl;

        uint32_t buffer_size = 65000 * recorder.GetFrameSize();

        auto endpoint = udp::endpoint(net::ip::address_v4::from_string(str, ec), port);

        //socket.send_to(net::buffer(rec_result.data.data(), buffer_size), endpoint);
        //DEBUG
        socket.send_to(net::buffer("Hello from client"sv), endpoint);
    }
}