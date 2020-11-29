all: random insrand twoplayer

random: random.c
	gcc -Wall -std=c99 random.c -L.. -lchessengine -I.. -o random
	echo 'LD_LIBRARY_PATH=.. ./random $$@' > random.sh
	chmod 755 random.sh

insrand: insrand.c
	gcc -Wall -std=c99 insrand.c -L.. -lchessengine -I.. -o insrand
	echo 'LD_LIBRARY_PATH=.. ./insrand $$@' > insrand.sh
	chmod 755 insrand.sh

twoplayer: twoplayer.c
	gcc -Wall -std=c99 twoplayer.c -L.. -lchessengine -lncurses -I.. -o twoplayer
	echo 'LD_LIBRARY_PATH=.. ./twoplayer $$@' > twoplayer.sh
	chmod 755 twoplayer.sh

clean: clean-random clean-twoplayer clean-insrand

clean-random:
	@rm -vf random random.sh

clean-twoplayer:
	@rm -vf twoplayer twoplayer.sh

clean-insrand:
	@rm -vf insrand insrand.sh

