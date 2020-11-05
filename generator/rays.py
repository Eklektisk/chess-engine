#!/usr/bin/python
# Generate a list of spaces along a ray

def _debug(all_rays, pos):
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
    for i, ray in enumerate(all_rays[pos]):
        for square in ray:
            dummy_board[square] = '%d' % i

    print(pos)

    for i in range(8):
        print(''.join(dummy_board[i*8:(i+1)*8]))
    
    print()

    return

def generate():
    all_rays = [list([list() for j in range(8)]) for i in range(64)]

    for y in range(8):
        for x in range(8):
            pos = y * 8 + x

            alt_pos = pos
            for i in range(y):
                alt_pos -= 8
                all_rays[pos][0].append(alt_pos)

            alt_pos = pos
            for i in range(min(7 - x, y)):
                alt_pos -= 7
                all_rays[pos][1].append(alt_pos)

            alt_pos = pos
            for i in range(7 - x):
                alt_pos += 1
                all_rays[pos][2].append(alt_pos)

            alt_pos = pos
            for i in range(min(7 - x, 7 - y)):
                alt_pos += 9
                all_rays[pos][3].append(alt_pos)

            alt_pos = pos
            for i in range(7 - y):
                alt_pos += 8
                all_rays[pos][4].append(alt_pos)

            alt_pos = pos
            for i in range(min(x, 7 - y)):
                alt_pos += 7
                all_rays[pos][5].append(alt_pos)

            alt_pos = pos
            for i in range(x):
                alt_pos -= 1
                all_rays[pos][6].append(alt_pos)

            alt_pos = pos
            for i in range(min(x, y)):
                alt_pos -= 9
                all_rays[pos][7].append(alt_pos)

    return all_rays

def format(all_rays):
    formattedOne = '{\n'
    formattedTwo = '{\n'
    for i in range(64):
        ray_contains = ['-1' for i in range(64)]

        formattedOne += '\t{\n'

        for j in range(8):
            squares = ','.join(['%2d' % (square) for square in all_rays[i][j]] + ['-1' for n in range(7 - len(all_rays[i][j]))])
            size = len(all_rays[i][j])
            formattedOne += f'\t\t{{ pos: {{{squares}}}, size: {size} }},\n'

            for square in all_rays[i][j]:
                ray_contains[square] = f'{j}'

        formattedOne += '\t},\n'
        formattedTwo += f'\t{{{",".join(ray_contains)}}},\n'

    formattedOne += '}'
    formattedTwo += '}'

    varOne = 'struct Ray allRays[64][8]'
    varTwo = 'int8_t rayContains[64][64]'

    return f'{varOne} = {formattedOne};\n\n{varTwo} = {formattedTwo};'

if __name__ == '__main__':
    all_rays = generate()
    _debug(all_rays,  0)
    _debug(all_rays,  7)
    _debug(all_rays, 27)
    _debug(all_rays, 56)
    _debug(all_rays, 63)

