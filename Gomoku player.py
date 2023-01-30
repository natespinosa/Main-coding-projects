"""Gomoku starter code: the first section of this code was not created by me (author can be found below), this was my first programming project.
My own code can be found from line 311.
Author(s): Michael Guerzhoy with tests contributed by Siavash Kazemian.  Last modified: Oct. 30, 2021

My code plays gomoku against a user. It looks for the next most convenient position on the board given vertain conditions such as bounding, how 
full the board is, and possible outcomes of the current play.

"""

# def is_bounded(board, y_end, x_end, length, d_y, d_x):
#     pass

#def detect_row(board, col, y_start, x_start, length, d_y, d_x):
#    return open_seq_count, semi_open_seq_count

#def detect_rows(board, col, length):
    ####CHANGE ME
#    open_seq_count, semi_open_seq_count = 0, 0
#    return open_seq_count, semi_open_seq_count

def score(board):
    MAX_SCORE = 100000

    open_b = {}
    semi_open_b = {}
    open_w = {}
    semi_open_w = {}

    for i in range(2, 6):
        open_b[i], semi_open_b[i] = detect_rows(board, "b", i)
        open_w[i], semi_open_w[i] = detect_rows(board, "w", i)


    if open_b[5] >= 1 or semi_open_b[5] >= 1:
        return MAX_SCORE

    elif open_w[5] >= 1 or semi_open_w[5] >= 1:
        return -MAX_SCORE

    return (-10000 * (open_w[4] + semi_open_w[4])+
            500  * open_b[4]                     +
            50   * semi_open_b[4]                +
            -100  * open_w[3]                    +
            -30   * semi_open_w[3]               +
            50   * open_b[3]                     +
            10   * semi_open_b[3]                +
            open_b[2] + semi_open_b[2] - open_w[2] - semi_open_w[2])


def print_board(board):

    s = "*"
    for i in range(len(board[0])-1):
        s += str(i%10) + "|"
    s += str((len(board[0])-1)%10)
    s += "*\n"

    for i in range(len(board)):
        s += str(i%10)
        for j in range(len(board[0])-1):
            s += str(board[i][j]) + "|"
        s += str(board[i][len(board[0])-1])

        s += "*\n"
    s += (len(board[0])*2 + 1)*"*"

    print(s)


def make_empty_board(sz):
    board = []
    for i in range(sz):
        board.append([" "]*sz)
    return board



def analysis(board):
    for c, full_name in [["b", "Black"], ["w", "White"]]:
        print("%s stones" % (full_name))
        for i in range(2, 6):
            open, semi_open = detect_rows(board, c, i);
            print("Open rows of length %d: %d" % (i, open))
            print("Semi-open rows of length %d: %d" % (i, semi_open))






def play_gomoku(board_size):
    board = make_empty_board(board_size)
    board_height = len(board)
    board_width = len(board[0])

    while True:
        print_board(board)
        if is_empty(board):
            move_y = board_height // 2
            move_x = board_width // 2
        else:
            move_y, move_x = search_max(board)

        print("Computer move: (%d, %d)" % (move_y, move_x))
        board[move_y][move_x] = "b"
        print_board(board)
        analysis(board)

        game_res = is_win(board)
        if game_res in ["White won", "Black won", "Draw"]:
            return game_res


        print("Your move:")
        move_y = int(input("y coord: "))
        move_x = int(input("x coord: "))
        board[move_y][move_x] = "w"
        print_board(board)
        analysis(board)

        game_res = is_win(board)
        if game_res in ["White won", "Black won", "Draw"]:
            return game_res



def put_seq_on_board(board, y, x, d_y, d_x, length, col):
    for i in range(length):
        board[y][x] = col
        y += d_y
        x += d_x


def test_is_empty():
    board  = make_empty_board(8)
    if is_empty(board):
        print("TEST CASE for is_empty PASSED")
    else:
        print("TEST CASE for is_empty FAILED")

def test_is_bounded():
    board = make_empty_board(8)
    x = 4; y = 4; d_x = 1; d_y = 1; length = 4
    put_seq_on_board(board, y, x, d_y, d_x, length, "w")
    print_board(board)

    y_end = 7
    x_end = 7

    if is_bounded(board, y_end, x_end, length, d_y, d_x) == 'SEMIOPEN':
        print("TEST CASE for is_bounded PASSED")
    else:
        print("TEST CASE for is_bounded FAILED")


def test_detect_row():
    board = make_empty_board(8)
    x = 5; y = 3; d_x = 1; d_y = -1; length = 2
    put_seq_on_board(board, y, x, d_y, d_x, length, "w")
    print_board(board)
    if detect_row(board, "w",y,x,length,d_y,d_x) == (1,0):
        print("TEST CASE for detect_row PASSED")
    else:
        print("TEST CASE for detect_row FAILED")

def test_detect_rows():
    board = make_empty_board(8)
    x = 5; y = 1; d_x = -1; d_y = 1; length = 3; col = 'b'
    put_seq_on_board(board, y, x, d_y, d_x, length, "b")
    print_board(board)
    if detect_rows(board, col,length) == (1,0):
        print("TEST CASE for detect_rows PASSED")
    else:
        print("TEST CASE for detect_rows FAILED")

def test_search_max():
    board = make_empty_board(8)
    x = 5; y = 0; d_x = 0; d_y = 1; length = 4; col = 'w'
    put_seq_on_board(board, y, x, d_y, d_x, length, col)
    x = 6; y = 0; d_x = 0; d_y = 1; length = 4; col = 'b'
    put_seq_on_board(board, y, x, d_y, d_x, length, col)
    print_board(board)
    if search_max(board) == (4,6):
        print("TEST CASE for search_max PASSED")
    else:
        print("TEST CASE for search_max FAILED")

def easy_testset_for_main_functions():
    test_is_empty()
    test_is_bounded()
    test_detect_row()
    test_detect_rows()
    test_search_max()

def some_tests():
    board = make_empty_board(8)

    board[0][5] = "w"
    board[0][6] = "b"
    y = 5; x = 2; d_x = 0; d_y = 1; length = 3
    put_seq_on_board(board, y, x, d_y, d_x, length, "w")
    y = 0; x = 0; d_x = 1; d_y = 1; length = 5
    put_seq_on_board(board, y, x, d_y, d_x, length, "w")
    print_board(board)
    analysis(board)

    # Expected output:
    #       *0|1|2|3|4|5|6|7*
    #       0w| | | | |w|b| *
    #       1 |w| | | | | | *
    #       2 | |w| | | | | *
    #       3 | | |w| | | | *
    #       4 | | | |w| | | *
    #       5 | |w| | | | | *
    #       6 | |w| | | | | *
    #       7 | |w| | | | | *
    #       *****************
    #       Black stones:
    #       Open rows of length 2: 0
    #       Semi-open rows of length 2: 0
    #       Open rows of length 3: 0
    #       Semi-open rows of length 3: 0
    #       Open rows of length 4: 0
    #       Semi-open rows of length 4: 0
    #       Open rows of length 5: 0
    #       Semi-open rows of length 5: 0
    #       White stones:
    #       Open rows of length 2: 0
    #       Semi-open rows of length 2: 0
    #       Open rows of length 3: 0
    #       Semi-open rows of length 3: 1
    #       Open rows of length 4: 0
    #       Semi-open rows of length 4: 0
    #       Open rows of length 5: 0
    #       Semi-open rows of length 5: 0

    y = 3; x = 5; d_x = -1; d_y = 1; length = 2

    put_seq_on_board(board, y, x, d_y, d_x, length, "b")
    print_board(board)
    analysis(board)

    # Expected output:
    #        *0|1|2|3|4|5|6|7*
    #        0 | | | | |w|b| *
    #        1 | | | | | | | *
    #        2 | | | | | | | *
    #        3 | | | | |b| | *
    #        4 | | | |b| | | *
    #        5 | |w| | | | | *
    #        6 | |w| | | | | *
    #        7 | |w| | | | | *
    #        *****************
    #
    #         Black stones:
    #         Open rows of length 2: 1
    #         Semi-open rows of length 2: 0
    #         Open rows of length 3: 0
    #         Semi-open rows of length 3: 0
    #         Open rows of length 4: 0
    #         Semi-open rows of length 4: 0
    #         Open rows of length 5: 0
    #         Semi-open rows of length 5: 0
    #         White stones:
    #         Open rows of length 2: 0
    #         Semi-open rows of length 2: 0
    #         Open rows of length 3: 0
    #         Semi-open rows of length 3: 1
    #         Open rows of length 4: 0
    #         Semi-open rows of length 4: 0
    #         Open rows of length 5: 0
    #         Semi-open rows of length 5: 0
    #

    y = 5; x = 3; d_x = -1; d_y = 1; length = 1
    put_seq_on_board(board, y, x, d_y, d_x, length, "b");
    print_board(board);
    analysis(board);

    #        Expected output:
    #           *0|1|2|3|4|5|6|7*
    #           0 | | | | |w|b| *
    #           1 | | | | | | | *
    #           2 | | | | | | | *
    #           3 | | | | |b| | *
    #           4 | | | |b| | | *
    #           5 | |w|b| | | | *
    #           6 | |w| | | | | *
    #           7 | |w| | | | | *
    #           *****************
    #
    #
    #        Black stones:
    #        Open rows of length 2: 0
    #        Semi-open rows of length 2: 0
    #        Open rows of length 3: 0
    #        Semi-open rows of length 3: 1
    #        Open rows of length 4: 0
    #        Semi-open rows of length 4: 0
    #        Open rows of length 5: 0
    #        Semi-open rows of length 5: 0
    #        White stones:
    #        Open rows of length 2: 0
    #        Semi-open rows of length 2: 0
    #        Open rows of length 3: 0
    #        Semi-open rows of length 3: 1
    #        Open rows of length 4: 0
    #        Semi-open rows of length 4: 0
    #        Open rows of length 5: 0
    #        Semi-open rows of length 5: 0



def is_in_range(board, y, x):
    if (x < 0) or (x >= len(board)) or (y < 0) or (y >= len(board[0])):
        return False
    else:
        return True


def is_empty(board):
    count = 0
    for i in range(len(board)):
        for j in range(len(board[i])):
            if board[i][j] != " ":
                return False
            else:
                count += 1
    if count == 64:
        return True
    else:
        return False

def is_bounded(board, y_end, x_end, length, d_y, d_x):
    beg_open = False
    end_open = False
    x_start = x_end - (length - 1) * d_x
    y_start = y_end - (length - 1) * d_y

    if is_in_range(board, y_start - d_y, x_start - d_x):
        if board[y_start - d_y][x_start - d_x] == " ":
            beg_open = True
    if is_in_range(board, y_end + d_y, x_end + d_x):
        if board[y_end + d_y][x_end + d_x] == " ":
            end_open = True

    if beg_open and end_open:
        return "OPEN"
    elif beg_open == False and end_open == False:
        return "CLOSED"
    elif beg_open or end_open:
        return "SEMIOPEN"


def detect_row(board, col, y_start, x_start, length, d_y, d_x):
    open = 0
    semi_open = 0
    x = x_start
    y = y_start
    while is_in_range(board, y, x):
        if board[y][x] == col:
            seq_len = 1
            seq_x_end = x
            seq_y_end = y
            while is_in_range(board, seq_y_end + d_y, seq_x_end + d_x) and board[seq_y_end + d_y][seq_x_end + d_x] == col:
                seq_len += 1
                seq_x_end += d_x
                seq_y_end += d_y
            if seq_len == length:
                if is_bounded(board, seq_y_end, seq_x_end, seq_len, d_y, d_x) == "OPEN":
                    open += 1
                elif is_bounded(board, seq_y_end, seq_x_end, seq_len, d_y, d_x) == "SEMIOPEN":
                    semi_open += 1
            x = seq_x_end + d_x
            y = seq_y_end + d_y
        else:
            x += d_x
            y += d_y
    return (open, semi_open)


def detect_row_all(board, col, y_start, x_start, length, d_y, d_x):
    open = 0
    semi_open = 0
    closed = 0
    x = x_start
    y = y_start
    while is_in_range(board, y + length * d_y, x + length * d_x):
        if board[y][x] == col:
            seq_len = 1
            seq_x_end = x
            seq_y_end = y
            while is_in_range(board, seq_y_end + d_y, seq_x_end + d_x) and board[seq_y_end + d_y][seq_x_end + d_x] == col:
                seq_len += 1
                seq_x_end += d_x
                seq_y_end += d_y
            if seq_len == length:
                if is_bounded(board, seq_y_end, seq_x_end, seq_len, d_y, d_x) == "OPEN":
                    open += 1
                elif is_bounded(board, seq_y_end, seq_x_end, seq_len, d_y, d_x) == "SEMIOPEN":
                    semi_open += 1
                elif is_bounded(board, seq_y_end, seq_x_end, seq_len, d_y, d_x) == "CLOSED":
                    closed += 1
            x = seq_x_end + d_x
            y = seq_y_end + d_y
        x += d_x
        y += d_y
    return (open, semi_open, closed)


def detect_rows(board, col, length):
    rows = [0, 0]
    for y in range(len(board)):
        rows[0] += detect_row(board, col, y, 0, length, 0, 1)[0]
        rows[1] += detect_row(board, col, y, 0, length, 0, 1)[1]
    for x in range(len(board[0])):
        rows[0] += detect_row(board, col, 0, x, length, 1, 0)[0]
        rows[1] += detect_row(board, col, 0, x, length, 1, 0)[1]
    for y in range(len(board)):
        rows[0] += detect_row(board, col, y, 0, length, 1, 1)[0]
        rows[1] += detect_row(board, col, y, 0, length, 1, 1)[1]
    for x in range(1, len(board[0])):
        rows[0] += detect_row(board, col, len(board)-1, x, length, 1, 1)[0]
        rows[1] += detect_row(board, col, len(board)-1, x, length, 1, 1)[1]
    for y in range(len(board)):
        rows[0] += detect_row(board, col, y, 0, length, 1, -1)[0]
        rows[1] += detect_row(board, col, y, 0, length, 1, -1)[1]
    for x in range(1, len(board[0])):
        rows[0] += detect_row(board, col, 0, x, length, 1, -1)[0]
        rows[1] += detect_row(board, col, 0, x, length, 1, -1)[1]
    return tuple(rows)


def detect_rows_all(board, col, length):
    rows = [0, 0, 0]
    for y in range(len(board)):
        rows[0] += detect_row_all(board, col, y, 0, length, 0, 1)[0]
        rows[1] += detect_row_all(board, col, y, 0, length, 0, 1)[1]
        rows[2] += detect_row_all(board, col, y, 0, length, 0, 1)[2]
    for x in range(len(board[0])):
        rows[0] += detect_row_all(board, col, 0, x, length, 1, 0)[0]
        rows[1] += detect_row_all(board, col, 0, x, length, 1, 0)[1]
        rows[2] += detect_row_all(board, col, 0, x, length, 1, 0)[2]
    for x in range(len(board)):
        rows[0] += detect_row_all(board, col, 0, x, length, 1, 1)[0]
        rows[1] += detect_row_all(board, col, 0, x, length, 1, 1)[1]
        rows[2] += detect_row_all(board, col, 0, x, length, 1, 1)[2]
    for y in range(len(board)):
        rows[0] += detect_row_all(board, col, y, 0, length, 1, 1)[0]
        rows[1] += detect_row_all(board, col, y, 0, length, 1, 1)[1]
        rows[2] += detect_row_all(board, col, y, 0, length, 1, 1)[2]
    for x in range(len(board[0])):
        rows[0] += detect_row_all(board, col, 0, x, length, 1, -1)[0]
        rows[1] += detect_row_all(board, col, 0, x, length, 1, -1)[1]
        rows[2] += detect_row_all(board, col, 0, x, length, 1, -1)[2]
    for y in range(len(board)):
        rows[0] += detect_row_all(board, col, y, len(board)-1, length, 1, -1)[0]
        rows[1] += detect_row_all(board, col, y, len(board)-1, length, 1, -1)[1]
        rows[2] += detect_row_all(board, col, y, len(board)-1, length, 1, -1)[2]
    return tuple(rows)


def is_win(board):
    for i in range(3):
        if detect_rows_all(board, "b", 5)[i] > 0:
            print("Black won")
            return "Black won"
        elif detect_rows_all(board, "w", 5)[i] > 0:
            print("White won")
            return "White won"
        elif is_full(board):
            return "Draw"
    return "Continue playing"

# def is_win(board):
#     count = 0
#     for i in range(3):
#         if detect_rows_all(board, "b", 5)[i] > 0:
#             print("Black won")
#             return "Black won"
#             count += 1
#         elif detect_rows_all(board, "w", 5)[i] > 0:
#             print("White won")
#             return "White won"
#             count += 1
#         elif is_full(board):
#             print("Draw")
#             return "Draw"
#             count += 1
#     if count == 0:
#         print("Continue playing")
#         return "Continue playing"

def search_max(board):
    max_score = 0
    max_y = 0
    max_x = 0
    for y in range(len(board)):
        for x in range(len(board[0])):
            if board[y][x] == " ":
                board[y][x] = "b"
                if score(board) > max_score:
                    max_x = x
                    max_y = y
                    max_score = score(board)
                board[y][x] = " "
    return max_y, max_x


def is_full(board):
    for x in range(len(board[0])):
        for y in range(len(board)):
            if board[y][x] == " ":
                return False
    return True


if __name__ == '__main__':
#    some_tests()
#    test_is_bounded()
#    test_detect_row()
#    test_detect_rows()
#    test_search_max()
    play_gomoku(8)
    #test_is_win()
