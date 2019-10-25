#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

#define MSGLEN 100
#define SERPORT 60050

using namespace std;

int main(int argc, char **argv) {
	
	//初始化socket库
	WORD  sockVersion = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (WSAStartup(sockVersion, &wsadata) != 0) { //向系统申请使用sockVersion版本的socket，0为成功
		perror("WSAStartup:");
		return -1;
	}
	printf("WSAStartup succeed\n");
	//初始化socket
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //tcp/ip协议族，tcp通信类型，tcp协议
	if (server_socket == INVALID_SOCKET) { //-1?
		perror("socket:");
		return -1;
	}
	printf("socket succeed\n");
	//绑定ip和端口
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET; //协议族，在socket编程中只能是AF_INET
	sockAddr.sin_port = htons(SERPORT); //端口号（使用网络字节顺序），端口号的范围0~65535,同时0~1024范围的端口号已经被系统使用或保留。
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY; //按照网络字节顺序存储IP地址  INADDR_ANY:#define INADDR_ANY ((in_addr_t) 0x00000000) 代表所有地址
	if (bind(server_socket, (sockaddr *)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) { //成功返回 0，SOCKET_ERROR: -1
		perror("bind:");
		return -1;
	}
	printf("bind succeed\n");
	//监听
	if (listen(server_socket, 10) == SOCKET_ERROR) {//监听队列最大长度10 //成功返回 0，SOCKET_ERROR: -1
		perror("listen:");
		return -1;
	}
	printf("listen succeed\n");

	SOCKET client_socket;
	sockaddr_in client_addr;
	int len = sizeof(client_addr);
	client_socket = accept(server_socket, (sockaddr *)&client_addr, &len); //通过参数返回客户端信息
	if (client_socket == INVALID_SOCKET) {
		perror("accept:");
		return -1;
	}
	printf("accept succeed\n");
	char msg[MSGLEN];
	int msglen;
	while (1) {
		msglen = recv(client_socket, msg, MSGLEN, 0);
		if (msglen <= 0) {
			perror("recv:");
			return -1;
		}
		msg[msglen] = '\0';
		if (0 == strcmp(msg, "q")) {
			break;
		}
		printf("recv succeed\n");
		printf("copy message(%d) from client: %s\n", msglen, msg);
	}
	
	printf("close connect\n");
	closesocket(client_socket);
	closesocket(server_socket);
	WSACleanup();
	return 0;
} 
