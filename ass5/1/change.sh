#!/bin/bash
sed -i "/#define PORT/c\#define PORT $1" ./client_tcp.c
sed -i "/#define PORT/c\#define PORT $1" ./server_iter.c
sed -i "/#define PORT/c\#define PORT $1" ./server_conc.c
make
