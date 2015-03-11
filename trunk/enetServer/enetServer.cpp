// enetServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <enet/enet.h>
#include <stdio.h>

#pragma comment(lib, "enet.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char **argv)
{
	ENetAddress address;
	ENetHost *server;
	ENetEvent event;
	int eventStatus;

	// a. Initialize enet
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	atexit(enet_deinitialize);

	// b. Create a host using enet_host_create
	address.host = ENET_HOST_ANY;
	address.port = 1234;

	server = enet_host_create(&address, 32, 2, 0, 0);

	if (server == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENet server host\n");
		exit(EXIT_FAILURE);
	}

	// c. Connect and user service
	eventStatus = 1;

	while (1) {
		eventStatus = enet_host_service(server, &event, 50000);

		// If we had some event that interested us
		if (eventStatus > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u\n",event.peer->address.host, event.peer->address.port);
				printf("(Server) We got a new connection from %x\n", event.peer->address.host);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf("(Server) Message from client : %s\n", event.packet->data);
				// Lets broadcast this message to all
				enet_host_broadcast(server, 0, event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
					// Reset client's information
					event.peer->data = NULL;
				break;

			}
		}
	}

}