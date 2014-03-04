## CeBIT demo network setup

The CeBIT demo consists of 7 nodes.

## Node configuration

| Device   | Address    | Ignore    | RPL   | Bord                  |
|:-------- |:---------- |:--------- |:----- |:--------------------- |
| Portal   | 23         | -         | r     | msba2 + raspberryPi   |
| Router 1 | 31         | -         | n     | msba2                 |
| Router 2 | 32         | -         | n     | msba2                 |
| Router 3 | 33         | -         | n     | msba2                 |
| Sensor   | 41         | 23        | n     | avsextrem             |
| Light    | 51         | 23        | n     | msba2                 |
| Watch    | 61         | -         | n/a   | chronos               |


## Application protocol

| byte 2 | byte 1 | byte 0 |
| ------ | ------ | ------ |
| ID     | DATA   | SEQU   |

**ID:** the event id (or type), see events.h for available IDs

**DATA:** one byte of data, context sensitive to the ID

**SEQU:** a sequence number used in the network to enable transmission of multiple UDP packets for reliability improvements