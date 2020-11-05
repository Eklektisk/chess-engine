#!/usr/bin/python
# Generate piece moves along rays

def generate():
    all_moves = [list([list([list([]) for k in range(8)]) for j in range(6)]) for i in range(64)]

    for y in range(8):
        for x in range(8):
            pos = y * 8 + x

            alt_pos = pos
            for i in range(y):
                alt_pos -= 8
                for piece in [4, 5]:
                    all_moves[pos][piece][0].append(alt_pos)

            alt_pos = pos
            for i in range(min(7 - x, y)):
                alt_pos -= 7
                for piece in [0, 4]:
                    all_moves[pos][piece][1].append(alt_pos)

            alt_pos = pos
            for i in range(7 - x):
                alt_pos += 1
                for piece in [4, 5]:
                    all_moves[pos][piece][2].append(alt_pos)

            alt_pos = pos
            for i in range(min(7 - x, 7 - y)):
                alt_pos += 9
                for piece in [0, 4]:
                    all_moves[pos][piece][3].append(alt_pos)

            alt_pos = pos
            for i in range(7 - y):
                alt_pos += 8
                for piece in [4, 5]:
                    all_moves[pos][piece][4].append(alt_pos)

            alt_pos = pos
            for i in range(min(x, 7 - y)):
                alt_pos += 7
                for piece in [0, 4]:
                    all_moves[pos][piece][5].append(alt_pos)

            alt_pos = pos
            for i in range(x):
                alt_pos -= 1
                for piece in [4, 5]:
                    all_moves[pos][piece][6].append(alt_pos)

            alt_pos = pos
            for i in range(min(x, y)):
                alt_pos -= 9
                for piece in [0, 4]:
                    all_moves[pos][piece][7].append(alt_pos)

    return all_moves

def format(all_moves):
    formatted = '{\n'
    for i in range(64):
        formatted += '\t{\n'

        for j in range(6):
            formatted += '\t\t{\n'

            for k in range(8):
                squares = ','.join(['%2d' % (square) for square in all_moves[i][j][k]] + ['-1' for n in range(7 - len(all_moves[i][j][k]))])
                size = len(all_moves[i][j][k])
                formatted += f'\t\t\t{{ pos: {{{squares}}}, size: {size} }},\n'

            formatted += '\t\t},\n'

        formatted += '\t},\n'

    formatted += '}'

    var = 'struct Ray rayMoves[64][6][8]'

    return f'{var} = {formatted};'

if __name__ == '__main__':
    print('No debug function found')

