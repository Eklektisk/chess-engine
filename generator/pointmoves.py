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
    all_moves = [list([list() for j in range(6)]) for i in range(64)]

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
            all_moves[pos][1].append(pos - 8)

            if pos in left_one:
                all_moves[pos][1].append(pos - 9)

            if pos in right_one:
                all_moves[pos][1].append(pos - 7)

            if pos in left_two:
                all_moves[pos][2].append(pos - 10)

            if pos in right_two:
                all_moves[pos][2].append(pos - 6)

        if pos in down_one:
            all_moves[pos][1].append(pos + 8)

            if pos in left_one:
                all_moves[pos][1].append(pos + 7)

            if pos in right_one:
                all_moves[pos][1].append(pos + 9)

            if pos in left_two:
                all_moves[pos][2].append(pos + 6)

            if pos in right_two:
                all_moves[pos][2].append(pos + 10)

        if pos in left_one:
            all_moves[pos][1].append(pos - 1)

            if pos in up_two:
                all_moves[pos][2].append(pos - 17)

            if pos in down_two:
                all_moves[pos][2].append(pos + 15)

        if pos in right_one:
            all_moves[pos][1].append(pos + 1)

            if pos in up_two:
                all_moves[pos][2].append(pos - 15)

            if pos in down_two:
                all_moves[pos][2].append(pos + 17)

        all_moves[pos][1].sort()
        all_moves[pos][2].sort()

    return all_moves

def format(all_moves):
    formatted = '{\n'
    for i in range(64):
        formatted += '\t{\n'

        for j in range(6):
            squares = ','.join(['%2d' % (square) for square in all_moves[i][j]] + ['-1' for n in range(8 - len(all_moves[i][j]))])
            size = len(all_moves[i][j])
            formatted += f'\t\t{{ .pos = {{{squares}}}, .size = {size} }},\n'

        formatted += '\t},\n'

    formatted += '}'

    var = 'struct Vector8 pointMoves[64][6]'

    return f'{var} = {formatted};'

if __name__ == '__main__':
    print('No debug function found')

