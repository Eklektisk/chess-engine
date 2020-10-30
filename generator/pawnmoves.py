#!/usr/bin/python
# Generate piece moves to specific points

def generate():
    all_moves = [list([list([list([list([False for m in range(64)]) for l in range(2)]) for k in range(2)]) for j in range(2)]) for i in range(64)]

    for x in range(8):
        all_moves[8 + x][1][0][1][16 + x] = True
        all_moves[8 + x][1][0][1][24 + x] = True

        all_moves[48 + x][0][0][1][40 + x] = True
        all_moves[48 + x][0][0][1][32 + x] = True

        if x > 0:
            all_moves[8 + x][1][0][0][15 + x] = True
            all_moves[8 + x][1][0][0][23 + x] = True

            all_moves[48 + x][0][0][0][39 + x] = True
            all_moves[48 + x][0][0][0][31 + x] = True

        if x < 7:
            all_moves[8 + x][1][0][0][17 + x] = True
            all_moves[8 + x][1][0][0][25 + x] = True

            all_moves[48 + x][0][0][0][41 + x] = True
            all_moves[48 + x][0][0][0][33 + x] = True

        for y in range(2,7):
            init_b_pos = y * 8 + x
            init_w_pos = (7 - y) * 8 + x

            all_moves[init_b_pos][1][1][1][init_b_pos + 8] = True
            all_moves[init_w_pos][0][1][1][init_w_pos - 8] = True

            if x > 0:
                all_moves[init_b_pos][1][1][0][init_b_pos + 7] = True
                all_moves[init_w_pos][0][1][0][init_w_pos - 9] = True

            if x < 7:
                all_moves[init_b_pos][1][1][0][init_b_pos + 9] = True
                all_moves[init_w_pos][0][1][0][init_w_pos - 7] = True

    return all_moves

def format(all_moves):
    formatted = '{\n'
    for i in range(64):
        formatted += '\t{\n'

        for j in range(2):
            formatted += '\t\t{\n'

            for k in range(2):
                formatted += '\t\t\t{\n'

                for l in range(2):
                    can_move = ','.join(list(map(lambda x: ['0', '1'][x], all_moves[i][j][k][l])))
                    formatted += f'\t\t\t\t{{{can_move}}},\n'

                formatted += '\t\t\t},\n'

            formatted += '\t\t},\n'

        formatted += '\t},\n'

    formatted += '}'

    var = 'const bool pawnMoves[64][2][2][2][64]'


    return f'{var} = {formatted};'

if __name__ == '__main__':
    print('No generator found')
