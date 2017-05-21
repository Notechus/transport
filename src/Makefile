# Sebastian Paulus 266446

Flags = -std=c++11 -Wextra -g -Wall


transport : Makefile transport.o utils.o udpsocket.o packet.o packetbuffer.o
	g++ $(Flags) -o transport transport.o utils.o udpsocket.o packet.o packetbuffer.o

transport.o : Makefile main.cpp utils.o udpsocket.o packet.o
	g++ -c $(Flags) main.cpp -o  transport.o

udpsocket.o : Makefile udpsocket.cpp udpsocket.h utils.o packet.o
	g++ -c $(Flags) udpsocket.cpp -o udpsocket.o

packetbuffer.o : Makefile packetbuffer.cpp packetbuffer.h utils.o packet.o
	g++ -c $(Flags) packetbuffer.cpp -o packetbuffer.o

utils.o : Makefile utils.cpp utils.h
	g++ -c $(Flags) utils.cpp -o utils.o

packet.o : Makefile packet.cpp packet.h
	g++ -c $(Flags) packet.cpp -o packet.o

clean:
		rm -rf *.o

distclean:
		rm -rf *.o transport .*.swp *.h.gch