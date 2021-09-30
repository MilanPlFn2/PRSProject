all: client_udp serveur_udp

client: client_udp.o
	gcc client_udp.o -o client_udp 

serveur: serveur_udp.o
	gcc serveur_udp.o -o serveur_udp 

client.o: client.c 
	gcc -c client_udp.c -o client_udp.o

serveur.o: serveur.c 
	gcc -c serveur_udp.c -o serveur_udp.o

clean:
	rm -f client_udp serveur_udp *.o
