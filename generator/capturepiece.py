#!/usr/bin/python
# Generate list of postions where one piece can capture another piece

def generate():
    capture_piece = [list([list([list([False for l in range(64)]) for k in range(2)]) for j in range(6)]) for i in range(64)]

    # RAY MOVES
    for y in range(8):
        for x in range(8):
            pos = y * 8 + x

            alt_pos = pos
            for i in range(y):
                alt_pos -= 8
                for piece in [4, 5]:
                    capture_piece[pos][piece][0][alt_pos] = True
                    capture_piece[pos][piece][1][alt_pos] = True

            alt_pos = pos
            for i in range(min(7 - x, y)):
                alt_pos -= 7
                for piece in [0, 4]:
                    capture_piece[pos][piece][0][alt_pos] = True
                    capture_piece[pos][piece][1][alt_pos] = True

            alt_pos = pos
            for i in range(7 - x):
                alt_pos += 1
                for piece in [4, 5]:
                    capture_piece[pos][piece][0][alt_pos] = True
                    capture_piece[pos][piece][1][alt_pos] = True

            alt_pos = pos
            for i in range(min(7 - x, 7 - y)):
                alt_pos += 9
                for piece in [0, 4]:
                    capture_piece[pos][piece][0][alt_pos] = True
                    capture_piece[pos][piece][1][alt_pos] = True

            alt_pos = pos
            for i in range(7 - y):
                alt_pos += 8
                for piece in [4, 5]:
                    capture_piece[pos][piece][0][alt_pos] = True
                    capture_piece[pos][piece][1][alt_pos] = True

            alt_pos = pos
            for i in range(min(x, 7 - y)):
                alt_pos += 7
                for piece in [0, 4]:
                    capture_piece[pos][piece][0][alt_pos] = True
                    capture_piece[pos][piece][1][alt_pos] = True

            alt_pos = pos
            for i in range(x):
                alt_pos -= 1
                for piece in [4, 5]:
                    capture_piece[pos][piece][0][alt_pos] = True
                    capture_piece[pos][piece][1][alt_pos] = True

            alt_pos = pos
            for i in range(min(x, y)):
                alt_pos -= 9
                for piece in [0, 4]:
                    capture_piece[pos][piece][0][alt_pos] = True
                    capture_piece[pos][piece][1][alt_pos] = True

    # POINT MOVES
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
            capture_piece[pos][1][0][pos - 8] = True
            capture_piece[pos][1][1][pos - 8] = True

            if pos in left_one:
                capture_piece[pos][1][0][pos - 9] = True
                capture_piece[pos][1][1][pos - 9] = True

            if pos in right_one:
                capture_piece[pos][1][0][pos - 7] = True
                capture_piece[pos][1][1][pos - 7] = True

            if pos in left_two:
                capture_piece[pos][2][0][pos - 10] = True
                capture_piece[pos][2][1][pos - 10] = True

            if pos in right_two:
                capture_piece[pos][2][0][pos - 6] = True
                capture_piece[pos][2][1][pos - 6] = True

        if pos in down_one:
            capture_piece[pos][1][0][pos + 8] = True
            capture_piece[pos][1][1][pos + 8] = True

            if pos in left_one:
                capture_piece[pos][1][0][pos + 7] = True
                capture_piece[pos][1][1][pos + 7] = True

            if pos in right_one:
                capture_piece[pos][1][0][pos + 9] = True
                capture_piece[pos][1][1][pos + 9] = True

            if pos in left_two:
                capture_piece[pos][2][0][pos + 6] = True
                capture_piece[pos][2][1][pos + 6] = True

            if pos in right_two:
                capture_piece[pos][2][0][pos + 10] = True
                capture_piece[pos][2][1][pos + 10] = True

        if pos in left_one:
            capture_piece[pos][1][0][pos - 1] = True
            capture_piece[pos][1][1][pos - 1] = True

            if pos in up_two:
                capture_piece[pos][2][0][pos - 17] = True
                capture_piece[pos][2][1][pos - 17] = True

            if pos in down_two:
                capture_piece[pos][2][0][pos + 15] = True
                capture_piece[pos][2][1][pos + 15] = True

        if pos in right_one:
            capture_piece[pos][1][0][pos + 1] = True
            capture_piece[pos][1][1][pos + 1] = True

            if pos in up_two:
                capture_piece[pos][2][0][pos - 15] = True
                capture_piece[pos][2][1][pos - 15] = True

            if pos in down_two:
                capture_piece[pos][2][0][pos + 17] = True
                capture_piece[pos][2][1][pos + 17] = True

    # PAWN MOVES
    for x in range(8):
        for y in range(1,7):
            init_b_pos = y * 8 + x
            init_w_pos = (7 - y) * 8 + x

            if x > 0:
                capture_piece[init_b_pos][3][1][init_b_pos + 7] = True
                capture_piece[init_w_pos][3][0][init_w_pos - 9] = True

            if x < 7:
                capture_piece[init_b_pos][3][1][init_b_pos + 9] = True
                capture_piece[init_w_pos][3][0][init_w_pos - 7] = True

    return capture_piece

def format(capture_piece):
    formatted = '{\n'
    for i in range(64):
        formatted += '\t{\n'

        for j in range(6):
            formatted += '\t\t{\n'

            for k in range(2):
                can_move = ','.join(list(map(lambda x: ['0','1'][x], capture_piece[i][j][k])))
                formatted += f'\t\t\t{{{can_move}}},\n'

            formatted += '\t\t},\n'

        formatted += '\t},\n'

    formatted += '}'

    var = 'bool capturePiece[64][6][2][64]'

    return f'{var} = {formatted};'

if __name__ == '__main__':
    print('No debug function found')

