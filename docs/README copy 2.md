# LLM Message Protocol

The following message protocol refers to the segmentation of a packet.

The first 6 bytes refers to the destination ID
The next 6 bytes refers to teh src ID
and the next 7 bytes are data (0 if it is a controller requesting data from a sensor)
and finnaly the last 2 bytes are the CRC.

In total the length of a valid packet is 21.


| Byte Num | Purpose                   |
| ---------| --------------------------| 
| 0-5      | dest ID                   |
| 6-12     | src ID                    |
| 13-19    | DATA                      |
| 20-21    | CALC CRC16 CCITT FALSE    |