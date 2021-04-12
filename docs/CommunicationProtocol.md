## Types and sizes

| type | size    |
| ---- | ------- |
| byte | 1 byte  |
| Int  | 2 bytes |   
| Long | 4 bytes |      

## Commands

| Function Descriptor   | Main Cmd | Sub Cmd | Arguments              | Arg Type       | Return Data Stream     | Type        | Number of bytes    |
| --------------------- | -------- | ------- | ---------------------- | -------------- | ---------------------- | ----------- | ------------------ |
|                       |          |         |                        |                |                        |             |                    |
| `GET_VERSION`         | `11`     | `5`     |                        |                | version string         | string      | terminated by `\n` |
|                       |          |         |                        |                |                        |             |                    |
| `CAPTURE_ONE`         | `2`      | `1`     | channel mux no.        | byte (0-136)   |                        |             |                    |
|                       |          |         | no. samples to capture | Int (1-10000)  |                        |             |                    |
|                       |          |         | timegap                | Int (6-65535)  | acknowledge            | byte        | 1                  |
|                       |          |         |                        |                |                        |             |                    |
| `CAPTURE_TWO`         | `2`      | `2`     | channel mux no.        | byte (0-136)   |                        |             |                    |
|                       |          |         | no. samples to capture | Int (1-5000)   |                        |             |                    |
|                       |          |         | timegap                | Int (7-65535)  | acknowledge            | byte        | 1                  |
|                       |          |         |                        |                |                        |             |                    |
| `CAPTURE_DMASPEED`    | `2`      | `3`     | channel mux no.        | byte (0-136)   |                        |             |                    |
|                       |          |         | no. samples to capture | Int (1-10000)  |                        |             |                    |
|                       |          |         | timegap                | Int (4-65535)  | acknowledge            | byte        | 1                  |
|                       |          |         |                        |                |                        |             |                    |
| `CAPTURE_FOUR`        | `2`      | `4`     | channel mux no.        | byte (0-136)   |                        |             |                    |
|                       |          |         | no. samples to capture | Int (1-2500)   |                        |             |                    |
|                       |          |         | timegap                | Int (14-65535) | acknowledge            | byte        | 1                  |
|                       |          |         |                        |                |                        |             |                    |
| `CONFIGURE_TRIGGER`   | `2`      | `5`     | trigger channel        | byte (0-3)     |                        |             |                    |
|                       |          |         | trigger voltage        | Int (0-4095)   | acknowledge            | byte        | 1                  |
|                       |          |         |                        |                |                        |             |                    |
| `GET_CAPTURE_STATUS`  | `2`      | `6`     |                        |                | conversion done        | byte        | 1                  |
|                       |          |         |                        |                | no. of samples         | Int         | 2                  |
|                       |          |         |                        |                | acknowledge            | byte        | 1                  |
|                       |          |         |                        |                |                        |             |                    |
| `SET_PGA_GAIN`        | `2`      | `8`     | pga                    | byte (1-2)     |                        |             |                    |
|                       |          |         | gain index             | byte (0-7)     | acknowledge            | byte        | 1                  |
|                       |          |         |                        |                |                        |             |                    |
| `GET_VOLTAGE_SUMMED`  | `2`      | `10`    | channel mux no.        | byte (0-8)     | sum of 16 samples      | Int         | 2                  |
|                       |          |         |                        |                | acknowledge            | byte        | 1                  |
|                       |          |         |                        |                |                        |             |                    |
| `SET_PGA_GAIN`        | `2`      | `7`     | pga                    | byte (1-2)     |                        |             |                    |
|                       |          |         | gain index             | byte (0-7)     | acknowledge            | byte        | 1                  |
|                       |          |         |                        |                |                        |             |                    |
| `SET_CAP`             | `2`      | `21`    | state                  | byte (0-1)     |                        |             |                    |
|                       |          |         | charge time            | Int (0-65535)  | acknowledge            | byte        | 1                  |
|                       |          |         |                        |                |                        |             |                    |


## Example `get version`

1) Send `chr(11)`,`chr(5)` (`0xB5`?)
2) Read until newline           
