server:
	clang -o server server.c request.c response.c router.c utils.c debug.c status_codes.c mimetypes.c -fsanitize=undefined

run:
	./server

clean:
	rm server

all: server run clean
