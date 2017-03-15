##
# makefile
##
# John Van Note
# 2016-10-11
##

fork='./Fork'
main='./board'
cc='gcc'
flags='-lpthread'

run: compile
	$(fork).out $(N) $(M)

compile:
	$(cc) $(fork).c $(main).c $(flags) -o $(fork).out
	
clean:
	\rm -f *.out
