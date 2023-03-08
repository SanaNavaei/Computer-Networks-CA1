CC = g++ -g -std=c++11

all: server.out client.out

client.out: client.o readJson.o user.o admin.o rooms.o
	$(CC) $^ -o $@

server.out: server.o readJson.o user.o admin.o rooms.o
	$(CC) $^ -o $@
	
client.o: source/client.cpp library/client.hpp library/readJson.hpp
	$(CC) -c $< -o $@

server.o: source/server.cpp library/server.hpp library/readJson.hpp
	$(CC) -c $< -o $@

user.o: source/user.cpp library/user.hpp
	$(CC) -c $< -o $@

admin.o: source/admin.cpp library/admin.hpp
	$(CC) -c $< -o $@

readJson.o: source/readJson.cpp library/readJson.hpp library/json.hpp library/user.hpp library/admin.hpp
	$(CC) -c $< -o $@

rooms.o: source/rooms.cpp library/rooms.hpp library/readJson.hpp
	$(CC) -c $< -o $@

.PHONY: clean

clean:
	rm *.o *.out
