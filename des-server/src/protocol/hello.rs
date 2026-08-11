use crate::protocol::packet::Packet;
use anyhow::{Ok, Result};
use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize)]
pub struct Hello {
    pub player_name: String,
    pub password: String,
}

impl Packet for Hello {
    const ID: u8 = 0;
    fn encode(&self) -> Result<Vec<u8>> {
        let bytes = rmp_serde::to_vec(&self)?;

        Ok(bytes)
    }

    fn decode(data: &[u8]) -> Result<Self> {
        Ok(rmp_serde::from_slice(data)?)
    }
}
