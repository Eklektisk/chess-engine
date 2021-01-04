all: random insrand twoplayer

random: random.c
	gcc -Wall -std=c99 random.c -L.. -static -lchessengine -I.. -o random

insrand: insrand.c
	gcc -Wall -std=c99 insrand.c -L.. -static -lchessengine -I.. -o insrand

twoplayer: twoplayer.c
	gcc -Wall -std=c99 twoplayer.c ../libchessengine.a -lncurses -I.. -o twoplayer

clean: clean-random clean-twoplayer clean-insrand

clean-random:
	@rm -vf random random.sh

clean-twoplayer:
	@rm -vf twoplayer twoplayer.sh

clean-insrand:
	@rm -vf insrand insrand.sh

