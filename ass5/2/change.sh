#!/bin/bash
sed -i "/#define PORT/c\#define PORT $1" ./client.c
sed -i "/#define PORT/c\#define PORT $1" ./server1.c
sed -i "/#define PORT/c\#define PORT $1" ./server2.c
make
