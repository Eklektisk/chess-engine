#!/usr/bin/python
# Generate piece moves along rays

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
    for i in range(len(all_moves[pos][piece])):
        for j in range(64):
            if all_moves[pos][piece][i][j]:
                dummy_board[j] = '%d' % i

    print(f'{pos}, {["BISHOP", "KING", "KNIGHT", "PAWN", "QUEEN", "ROOK"][piece]}')

    for i in range(8):
        print(''.join(dummy_board[i*8:(i+1)*8]))
    
    print()

    return

def generate():
    all_moves = [list([list([list([False for l in range(64)]) for k in range(8)]) for j in range(6)]) for i in range(64)]

    for y in range(8):
        for x in range(8):
            pos = y * 8 + x

            alt_pos = pos
            for i in range(y):
                alt_pos -= 8
                for piece in [4, 5]:
                    all_moves[pos][piece][0][alt_pos] = True

            alt_pos = pos
            for i in range(min(7 - x, y)):
                alt_pos -= 7
                for piece in [0, 4]:
                    all_moves[pos][piece][1][alt_pos] = True

            alt_pos = pos
            for i in range(7 - x):
                alt_pos += 1
                for piece in [4, 5]:
                    all_moves[pos][piece][2][alt_pos] = True

            alt_pos = pos
            for i in range(min(7 - x, 7 - y)):
                alt_pos += 9
                for piece in [0, 4]:
                    all_moves[pos][piece][3][alt_pos] = True

            alt_pos = pos
            for i in range(7 - y):
                alt_pos += 8
                for piece in [4, 5]:
                    all_moves[pos][piece][4][alt_pos] = True

            alt_pos = pos
            for i in range(min(x, 7 - y)):
                alt_pos += 7
                for piece in [0, 4]:
                    all_moves[pos][piece][5][alt_pos] = True

            alt_pos = pos
            for i in range(x):
                alt_pos -= 1
                for piece in [4, 5]:
                    all_moves[pos][piece][6][alt_pos] = True

            alt_pos = pos
            for i in range(min(x, y)):
                alt_pos -= 9
                for piece in [0, 4]:
                    all_moves[pos][piece][7][alt_pos] = True

    return all_moves

def format(all_moves):
    formatted = '{\n'
    for i in range(64):
        formatted += '\t{\n'

        for j in range(6):
            formatted += '\t\t{\n'

            for k in range(8):
                can_move = ','.join(list(map(lambda x: ['0','1'][x], all_moves[i][j][k])))
                formatted += f'\t\t\t{{{can_move}}},\n'

            formatted += '\t\t},\n'

        formatted += '\t},\n'

    formatted += '}'

    var = 'const bool rayMoves[64][6][8][64]'

    return f'{var} = {formatted};'

if __name__ == '__main__':
    all_moves = generate()
    _debug(all_moves,  0, 0)
    _debug(all_moves,  7, 0)
    _debug(all_moves, 27, 0)
    _debug(all_moves, 56, 0)
    _debug(all_moves, 63, 0)
    _debug(all_moves,  0, 5)
    _debug(all_moves,  7, 5)
    _debug(all_moves, 27, 5)
    _debug(all_moves, 56, 5)
    _debug(all_moves, 63, 5)
    _debug(all_moves, 27, 4)
    _debug(all_moves,  0, 2)
