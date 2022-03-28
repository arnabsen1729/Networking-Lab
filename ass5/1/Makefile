all: client iter conc

client: client_tcp.c
	gcc client_tcp.c -o client_tcp

iter: server_iter.c
	gcc server_iter.c -o server_iter

conc: server_conc.c
	gcc server_conc.c -o server_conc

clean:
	rm client_tcp server_iter server_conc
