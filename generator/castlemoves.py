#!/usr/bin/python
# Generate piece moves to specific points

def generate():
    all_moves = [list([list([list([list([list([False for n in range(64)]) for m in range(2)]) for l in range(2)]) for k in range(2)]) for j in range(2)]) for i in range(64)]

    all_moves[3][0][0][0][0][1] = True
    all_moves[3][0][0][0][1][2] = True

    all_moves[3][0][1][0][0][5] = True
    all_moves[3][0][1][0][1][4] = True

    all_moves[60][0][0][0][0][58] = True
    all_moves[60][0][0][0][1][59] = True

    all_moves[60][0][1][0][0][62] = True
    all_moves[60][0][1][0][1][61] = True

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
                    formatted += '\t\t\t\t{\n'

                    for m in range(2):
                        can_move = ','.join(list(map(lambda x: ['0', '1'][x], all_moves[i][j][k][l][m])))
                        formatted += f'\t\t\t\t\t{{{can_move}}},\n'

                    formatted += '\t\t\t\t},\n'

                formatted += '\t\t\t},\n'

            formatted += '\t\t},\n'

        formatted += '\t},\n'

    formatted += '}'

    var = 'const bool castleMoves[64][2][2][2][2][64]'

    return f'{var} = {formatted};'

if __name__ == '__main__':
    print("No debug function found")
