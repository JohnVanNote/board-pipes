build : Fork.c board.c
	gcc -lpthread -o Fork ./Fork.c ./board.c ./csapp.c
	
test : build
	./Fork $(N) $(M)


clean:
	\rm ./Fork
