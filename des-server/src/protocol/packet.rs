use anyhow::Result;

pub trait Packet: Sized {
    const ID: u8;
    fn encode(&self) -> Result<Vec<u8>>;
    fn decode(data: &[u8]) -> Result<Self>;
}
