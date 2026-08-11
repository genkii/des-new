use std::{collections::HashSet, net::SocketAddr};

use crate::protocol::{hello::Hello, packet::Packet};

pub struct PacketHandler {
    pub valid_clients: HashSet<SocketAddr>,
    pub password: String,
}

impl PacketHandler {
    pub fn handle(&mut self, id: u8, payload: &[u8], sender: SocketAddr) -> anyhow::Result<()> {
        match id {
            Hello::ID => {
                let packet = Hello::decode(payload)?;

                println!(
                    "user: {}, password: {}",
                    packet.player_name, packet.password
                );

                if packet.password == self.password {
                    self.valid_clients.insert(sender);
                } else {
                    println!(
                        "Player: {} tried to connect with wrong password: {} from ip: {}",
                        packet.player_name, packet.password, sender
                    );

                    println!("Check if this was intentional, else try to ip ban the person");
                }
            }

            _ => {
                println!("Unknown packet ID: {id}");
            }
        }

        Ok(())
    }
}
