#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

#define MSGLEN 100
#define SERPORT 60050

using namespace std;

int main(int argc, char **argv) {

	if (argc != 2) {
		printf("%s ip\n", argv[0]);
		return -1;
	}
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (WSAStartup(socketVersion, &wsadata) != 0) {
		perror("WSAStartup:");
		return -1;
	}
	printf("WSAStartup succeed\n");
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket == INVALID_SOCKET) {
		perror("socket:");
		return -1;
	}
	printf("socket succeed\n");
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(SERPORT);
	sockAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	if (connect(client_socket, (sockaddr *)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
		perror("connect:");
		return - 1;
	}
	printf("connect succeed\n");
	char msg[MSGLEN];
	while (1) {
		memset(msg, 0, MSGLEN);
		printf("input message:");
		gets_s(msg, MSGLEN);
		if (0 == strcmp(msg, "q")) {
			send(client_socket, msg, strlen(msg), 0);
			break;
		}
		int msglen = send(client_socket, msg, strlen(msg), 0);
		if (msglen <= 0) {
			perror("send:");
			return -1;
		}
		printf("send succeed(%d):%s\n", msglen, msg);
	}
	
	printf("close connect\n");
	closesocket(client_socket);
	WSACleanup();
	return 0;
}