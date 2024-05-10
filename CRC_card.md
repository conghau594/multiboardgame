
### Class: Transceiver
+-----------------------------------------------------+-------------------+
|   Reponsibilities                                   |   Collaborators   |
+-----------------------------------------------------+-------------------+
|Trao đổi dữ liệu với client qua mạng                 |  <undefined>      |
|Trao đổi dữ liệu nhận từ mạng cho đối tượng Parser   |Parser             |
+-----------------------------------------------------+-------------------+



### Class: Parser
+----------------------------------------------------------+--------------------------------+
|   Reponsibilities                                        |   Collaborators                |
+----------------------------------------------------------+--------------------------------+
|Trao đổi dữ liệu với đối tượng Transceiver                |Transceiver                     |
|Đọc hiểu dữ liệu Player nhận từ Transceiver               |Player, Encryptor, Compressor   |
|Uỷ quyền xác minh đối tượng Player nếu nó vừa đăng nhập   |Authorizer                      |
|Trao đổi dữ liệu Player với đối tượng quản lý Player      |Transceiver                     |
|Uỷ quyền mã hoá, nén dữ liệu để gửi cho Transceiver       |Player, Encryptor, Compressor   |
+----------------------------------------------------------+--------------------------------+


### Class: <template>
+----------------------------------------+-------------------+
|   Reponsibilities                      |   Collaborators   |
+----------------------------------------+-------------------+
|Knows ...
+----------------------------------------+-------------------+



### Class: Board
+----------------------------------------|-------------------+
|   Reponsibilities                   |   Collaborators   |
|-------------------------------------|-------------------|
|Knows players and their own pieces   |Player, Piece      |
|Knows its mode                       |Mode               |
|-------------------------------------|-------------------|
|Manage players       |Player             |
|Set position for pieces   |Piece              |
|Choose mode game     |Mode               |



### Class: Player

|   Reponsibilities            |   Collaborators   |
|------------------------------|-------------------|
|Knows board, its own pieces   |Board, Piece       |
|------------------------------|-------------------|
|Chooses type of board         |Board              |
|Chooses mode game             |Mode               |
|Moves the pieces              |Piece              |



### Class: Piece

|   Reponsibilities     |   Collaborators   |
|-----------------------|-------------------|
|Know its position      |Position         |
|




### Class: Mode

|   Reponsibilities     |   Collaborators   |
|-----------------------|-------------------|
|Knows its Rule
|-----------------------|-------------------|
|Does ....




### Class: <template>

|   Reponsibilities            |   Collaborators   |
|------------------------------|-------------------|
|Knows ...
|------------------------------|-------------------|
|Ghép cặp và thiết lập bàn chơi   |



### Class: <template>
+----------------------------------------+-------------------+
|   Reponsibilities                      |   Collaborators   |
|----------------------------------------|-------------------|
|Knows ...
+----------------------------------------+-------------------+
