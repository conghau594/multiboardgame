
## Package: DataTransportLayer

### Class: Parser
|   Reponsibilities                                                                       |   Collaborators                |
|-----------------------------------------------------------------------------------------|--------------------------------|
|Sends data to Transceiver object                                                         |Transceiver                     |
|Parses the data from Transceiver object to Player information                            |Player, Encryptor, Compressor   |
|Sends login information to Authorizer object for authorization                           |Authorizer                      |
|Sends the authorized player data to PlayerManager object                                 |Transceiver                     |
|Processes data to send to Transceiver object                                             |Player, Encryptor, Compressor   |



# Package: DataTransportLayer

### Class: Transceiver
|   Reponsibilities                                                                       |   Collaborators                |
|-----------------------------------------------------------------------------------------|--------------------------------|
|Sends/receives data with client over network                                             |<undefined>                     |
|Sends the data to Parser object                                                          |Parser                          |



### Class: Authorizer
|   Reponsibilities                                                                       |   Collaborators                |
|-----------------------------------------------------------------------------------------|--------------------------------|
|Authorizes player information                                                            |Parser, DatabaseConnector       |



### Class: Encryptor
|   Reponsibilities                                                                       |   Collaborators                |
|-----------------------------------------------------------------------------------------|--------------------------------|
|Encrypts data                                                                            |<undefined>                     |



### Class: Compressor
|   Reponsibilities                                                                       |   Collaborators                |
|-----------------------------------------------------------------------------------------|--------------------------------|
|Compresses data                                                                          |<undefined>                     |



### Class: Piece

|   Reponsibilities     |   Collaborators   |
|-----------------------|-------------------|
|Does ...



### Class: Mode

|   Reponsibilities     |   Collaborators   |
|-----------------------|-------------------|
|Does ....




### Class: _class_name_
|   Reponsibilities                                                                       |   Collaborators                |
|-----------------------------------------------------------------------------------------|--------------------------------|
|does something....