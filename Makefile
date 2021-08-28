server:
	clang -o server server.c request.c response.c router.c utils.c

run:
	./server

clean:
	rm server

all: server run clean
