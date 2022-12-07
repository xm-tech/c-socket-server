build:
	gcc simple_server.c -o simple_server
run:
	./simple_server

.PHONY : clean
clean :
	-rm -f main && rm -f simple_server && rm -rf ./*.dSYM
