build:
	gcc simple_server.c -o simple_server

run:
	./simple_server

.PHONY : clean
clean :
	-rm -f simple_server
