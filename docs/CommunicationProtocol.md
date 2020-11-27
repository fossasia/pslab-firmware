## Types and sizes

| type | size    |
| ---- | ------- |
| byte | 1 byte  |
| Int  | 2 bytes |   
| Long | 4 bytes |      

## Commands

The acknowledge byte is `254`.

| Function Descriptor   | Main Cmd | Sub Cmd | Arguments             | Arg Type     | Return Data Stream     | Type        | Number of bytes    |
| --------------------- | -------- | ------- | --------------------- | ------------ | ---------------------- | ----------- | ------------------ |
| `READ_BULK_FLASH`     | `1`      | `4`     | no. of bytes to read  | Int (0-2048) | values stored in flash | byte stream | N                  |
|                       |          |         | page number           | byte (0-19)  | acknowledge            | byte        | 1                  |
|                       |          |         |                       |              |                        |             |                    |
| `WRITE_BULK_FLASH`    | `1`      | `3`     | no. of bytes to write | Int (0-2048) |                        | byte stream | N                  |
|                       |          |         | page number           | byte (0-19)  | acknowledge            | byte        | 1                  |
|                       |          |         |                       |              |                        |             |                    |
| `get_version`         | `11`     | `5`     |                       |              | version string         | string      | terminated by `\n` |

## Example `read_bulk_flash`

1) Send `chr(1),chr(4)` (i.e., `0x14`?)
2) Send number of bytes to read `chr(N&0xFF),chr((N>>8)&0xFF)`
   (so `N`'s low byte's compliment to `0xFF`, followed by high byte's compliment
   to `0xFF`, i.e. binary compliment of swapped bytes?)
3) Send Page Number `chr(0-19)` (i.e. `0x01`..`0x13`?)
4) read `N` bytes (TODO: how?)
5) Read 1 byte and check if equals `254` (so that is a parity byte?)

## Example `get version`

1) Send `chr(11)`,`chr(5)` (`0xB5`?)
2) Read until newline           
