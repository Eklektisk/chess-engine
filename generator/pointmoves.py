#!/usr/bin/python
# Generate piece moves to specific points

def _debug(all_moves, pos, piece):
    dummy_board = [
        '.','.','.','.','.','.','.','.',
        '.','.','.','.','.','.','.','.',
        '.','.','.','.','.','.','.','.',
        '.','.','.','.','.','.','.','.',
        '.','.','.','.','.','.','.','.',
        '.','.','.','.','.','.','.','.',
        '.','.','.','.','.','.','.','.',
        '.','.','.','.','.','.','.','.',
    ]

    dummy_board[pos] = '#'
    for i in range(64):
        if all_moves[pos][piece][i] and all_moves[pos][piece][i]:
            dummy_board[i] = '&'
        elif all_moves[pos][piece][i]:
            dummy_board[i] = 'W'
        elif all_moves[pos][piece][i]:
            dummy_board[i] = 'B'

    print(f'{pos}, {["BISHOP", "KING", "KNIGHT", "PAWN", "QUEEN", "ROOK"][piece]}')

    for i in range(8):
        print(''.join(dummy_board[i*8:(i+1)*8]))
    
    print()

    return

def generate():
    all_moves = [list([list([False for k in range(64)]) for j in range(6)]) for i in range(64)]

    up_one    = set()
    left_one  = set()
    down_one  = set()
    right_one = set()
    up_two    = set()
    left_two  = set()
    down_two  = set()
    right_two = set()

    for y in range(8):
        for x in range(8):
            pos = y * 8 + x

            if y > 0:
                up_one.add(pos)

                if y > 1:
                    up_two.add(pos)

            if y < 7:
                down_one.add(pos)

                if y < 6:
                    down_two.add(pos)

            if x > 0:
                left_one.add(pos)

                if x > 1:
                    left_two.add(pos)

            if x < 7:
                right_one.add(pos)

                if x < 6:
                    right_two.add(pos)

    for pos in range(64):
        if pos in up_one:
            all_moves[pos][1][pos - 8] = True

            if pos in left_one:
                all_moves[pos][1][pos - 9] = True

            if pos in right_one:
                all_moves[pos][1][pos - 7] = True

            if pos in left_two:
                all_moves[pos][2][pos - 10] = True

            if pos in right_two:
                all_moves[pos][2][pos - 6] = True

        if pos in down_one:
            all_moves[pos][1][pos + 8] = True

            if pos in left_one:
                all_moves[pos][1][pos + 7] = True

            if pos in right_one:
                all_moves[pos][1][pos + 9] = True

            if pos in left_two:
                all_moves[pos][2][pos + 6] = True

            if pos in right_two:
                all_moves[pos][2][pos + 10] = True

        if pos in left_one:
            all_moves[pos][1][pos - 1] = True

            if pos in up_two:
                all_moves[pos][2][pos - 17] = True

            if pos in down_two:
                all_moves[pos][2][pos + 15] = True

        if pos in right_one:
            all_moves[pos][1][pos + 1] = True

            if pos in up_two:
                all_moves[pos][2][pos - 15] = True

            if pos in down_two:
                all_moves[pos][2][pos + 17] = True

    return all_moves

def format(all_moves):
    formatted = '{\n'
    for i in range(64):
        formatted += '\t{\n'

        for j in range(6):
            can_move = ','.join(list(map(lambda x: ['0', '1'][x], all_moves[i][j])))
            formatted += f'\t\t{{{can_move}}},\n'

        formatted += '\t},\n'

    formatted += '}'

    var = 'const bool pointMoves[64][6][64]'

    return f'{var} = {formatted};'

if __name__ == '__main__':
    all_moves = generate()
    _debug(all_moves,  0, 1)
    _debug(all_moves,  7, 1)
    _debug(all_moves, 27, 1)
    _debug(all_moves, 56, 1)
    _debug(all_moves, 63, 1)
    _debug(all_moves,  0, 2)
    _debug(all_moves,  7, 2)
    _debug(all_moves, 27, 2)
    _debug(all_moves, 56, 2)
    _debug(all_moves, 63, 2)
    _debug(all_moves, 27, 0)
