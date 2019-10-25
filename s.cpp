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
	
	//��ʼ��socket��
	WORD  sockVersion = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (WSAStartup(sockVersion, &wsadata) != 0) { //��ϵͳ����ʹ��sockVersion�汾��socket��0Ϊ�ɹ�
		perror("WSAStartup:");
		return -1;
	}
	printf("WSAStartup succeed\n");
	//��ʼ��socket
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //tcp/ipЭ���壬tcpͨ�����ͣ�tcpЭ��
	if (server_socket == INVALID_SOCKET) { //-1?
		perror("socket:");
		return -1;
	}
	printf("socket succeed\n");
	//��ip�Ͷ˿�
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET; //Э���壬��socket�����ֻ����AF_INET
	sockAddr.sin_port = htons(SERPORT); //�˿ںţ�ʹ�������ֽ�˳�򣩣��˿ںŵķ�Χ0~65535,ͬʱ0~1024��Χ�Ķ˿ں��Ѿ���ϵͳʹ�û�����
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY; //���������ֽ�˳��洢IP��ַ  INADDR_ANY:#define INADDR_ANY ((in_addr_t) 0x00000000) �������е�ַ
	if (bind(server_socket, (sockaddr *)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) { //�ɹ����� 0��SOCKET_ERROR: -1
		perror("bind:");
		return -1;
	}
	printf("bind succeed\n");
	//����
	if (listen(server_socket, 10) == SOCKET_ERROR) {//����������󳤶�10 //�ɹ����� 0��SOCKET_ERROR: -1
		perror("listen:");
		return -1;
	}
	printf("listen succeed\n");

	SOCKET client_socket;
	sockaddr_in client_addr;
	int len = sizeof(client_addr);
	client_socket = accept(server_socket, (sockaddr *)&client_addr, &len); //ͨ���������ؿͻ�����Ϣ
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
