Risk/Issues:

Issue:

- Corrupted EEPROM - causes bad config 
1. Bad has_activated means it could be set to false
2. And keeps activating each time it boots

Severity:

- Low (The only thing that worsens is the battery) and more load on the server

Fix: Check if eeprom has corrupted, if it has then restore to def config



- Unable to connect to wifi, due to hardware component issue - never activates, etc
1. env casing breaks causes wifi module to break
2. Readings are never sent to the netlink

- Severity:

- Very High


Fix: And a retry counter for the wifi connection + timeout between each retry. Also add some physical notification to notify the user that the device's wifi is broken.



- Collecting readings, readings are corrupted
1. data is read into the chip, chip processes the data but it is invalid, the device still sends the coap packet, but the coap server drops it. reading packets are never sent and stoted in the DB.

Severity: Very High

Fix: Push a notification to the user if the current data set has at least 50% faults.


