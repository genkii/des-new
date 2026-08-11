use std::{collections::HashSet, net::SocketAddr};

use crate::protocol::{envelope::PacketEnvelope, handler::PacketHandler};

mod protocol;

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let mut handler = PacketHandler {
        valid_clients: HashSet::new(),
        password: "1234".to_string(),
    };
    let mut interval = tokio::time::interval(std::time::Duration::from_millis(1000 / 60));

    let socket = tokio::net::UdpSocket::bind("0.0.0.0:12345").await?;

    let mut buffer = [0u8; 1024];

    loop {
        tokio::select! {
            result = socket.recv_from(&mut buffer) => {
                let (size, sender) = result?;

                let packet: PacketEnvelope = PacketEnvelope::decode(&buffer[..size])?;

                handler.handle(packet.id, &packet.payload, sender)?;
            }

            _ = interval.tick() => {

            }
        }
    }
}
