build:
	# gcc simple_server.c -o simple_server
	gcc daytimetcpsrv.c -o daytimetcpsrv

tag:
	ctags * .

run:
	# ./simple_server
	./daytimetcpsrv

.PHONY : clean
clean :
	-rm -f simple_server tags
	-rm -f daytimetcpsrv tags
