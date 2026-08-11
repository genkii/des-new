use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize)]
struct RawEnvelope(u8, Vec<u8>);

#[derive(Debug, Serialize, Deserialize)]
pub struct PacketEnvelope {
    pub id: u8,
    pub payload: Vec<u8>,
}

impl PacketEnvelope {
    pub fn decode(data: &[u8]) -> anyhow::Result<Self> {
        let envelope: RawEnvelope = rmp_serde::from_slice(data)?;

        Ok(PacketEnvelope {
            id: envelope.0,
            payload: envelope.1,
        })
    }
}
