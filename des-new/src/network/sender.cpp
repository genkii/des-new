#include <msgpack.hpp>
#include "./packets/hello.hpp"
#include <asio.hpp>

class PacketSender {
private:
    asio::io_context io;
    asio::ip::udp::socket socket{io};
    asio::ip::udp::endpoint server{
        asio::ip::make_address("127.0.0.1"),
        12345
    };

    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> packer;

    msgpack::sbuffer payload;
    msgpack::packer<msgpack::sbuffer> payload_packer;

public:
    PacketSender()
        : packer(buffer), payload_packer(payload) {
        socket.open(asio::ip::udp::v4());
    }

    void send_packet(const Hello& packet) {
        payload.clear();
        buffer.clear();

        // Create the Hello payload

        payload_packer.pack_map(2);

        payload_packer.pack("player_name");
        payload_packer.pack(packet.player_name);

        payload_packer.pack("password");
        payload_packer.pack(packet.password);

        // Create the packet envelope
        packer.pack_array(2);

        // Packet ID
        packer.pack(Hello::ID);

        // Payload as MessagePack binary
        packer.pack_bin(static_cast<uint32_t>(payload.size()));
        packer.pack_bin_body(payload.data(), payload.size());

        socket.send_to(
            asio::buffer(buffer.data(), buffer.size()),
            server
        );
    }
};
