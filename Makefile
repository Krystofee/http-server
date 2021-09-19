server:
	clang -o server server.c request.c response.c router.c utils.c debug.c status_codes.c mimetypes.c views/serve_static.c views/index.c -fsanitize=undefined -iquote .

run:
	./server

clean:
	rm server

all: server run clean
