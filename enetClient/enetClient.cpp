// enetClient.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <enet/enet.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "enet.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char **argv)
{
	ENetAddress address;
	ENetHost *client;
	ENetPeer *peer;
	char message[1024];
	ENetEvent event;
	int eventStatus;

	// a. Initialize enet
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	atexit(enet_deinitialize);

	// b. Create a host using enet_host_create
	client = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);

	if (client == NULL) {
		fprintf(stderr, "An error occured while trying to create an ENet server host\n");
		exit(EXIT_FAILURE);
	}

	enet_address_set_host(&address, "localhost");
	address.port = 1234;

	// c. Connect and user service
	peer = enet_host_connect(client, &address, 2, 0);

	if (peer == NULL) {
		fprintf(stderr, "No available peers for initializing an ENet connection");
		exit(EXIT_FAILURE);
	}

	eventStatus = 1;

	while (1) {
		eventStatus = enet_host_service(client, &event, 50000);

		// If we had some event that interested us
		if (eventStatus > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				printf("(Client) We got a new connection from %x\n", event.peer->address.host);
					break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf("(Client) Message from server : %s\n", event.packet->data);
				// Lets broadcast this message to all
				// enet_host_broadcast(client, 0, event.packet);
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("(Client) %s disconnected.\n", event.peer->data);
					// Reset client's information
					event.peer->data= NULL;
				break;
			}
		}

		printf("Say > ");
		gets(message);

		if (strlen(message) > 0) {
			ENetPacket *packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet);
		}
	}
}