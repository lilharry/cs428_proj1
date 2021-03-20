make:
	g++ server.cpp -o server
	g++ client.cpp -o client

run:
	./server &
	./client
