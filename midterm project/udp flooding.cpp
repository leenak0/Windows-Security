#include <stdio.h>
#include <string.h>
#include <windows.h>

#define BUFFER_SIZE 1024

#pragma comment (lib,"ws2_32.lib") //���̺귯�� ��ũ

void main(void)

{
	WSADATA   wsaData;

	SOCKET   ClientSocket;

	SOCKADDR_IN  CnCServer; //C&C����
	SOCKADDR_IN  AttackServer; //������ ����

	int   CnCServer_Size;
	int   AttackServer_Size;

	int   Recv_Size;
	int   Send_Size;

	char   Buffer[BUFFER_SIZE] = { "IMHACKER_2016111566" }; //C&C ������ ������ �޽���

	USHORT   ServerPort = 10004; //C&C ���� ��Ʈ��ȣ



	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) //WS2_32.dll ��� ����
	{
		printf("WinSock �ʱ�ȭ�κп��� ���� �߻�.\n");
		WSACleanup(); //������ ���� WS2_32.dll ��� ��
		exit(0);
	}

	//CncServer �޸��� ��� ������ 0���� ����
	memset(&CnCServer, 0, sizeof(CnCServer));
	//AttackServer �޸��� ��� ������ 0���� ����
	memset(&AttackServer, 0, sizeof(AttackServer));

	
	CnCServer.sin_family = AF_INET; //������ ���۹���� IPv4�� ����

	CnCServer.sin_addr.s_addr = inet_addr("114.70.37.17"); //C&C ���� �ּ�

	CnCServer.sin_port = htons(ServerPort); //C&C ���� ��Ʈ��ȣ


	ClientSocket = socket(AF_INET, SOCK_DGRAM, 0);

	if (ClientSocket == INVALID_SOCKET)
	{
		printf("������ �����Ҽ� �����ϴ�.\n");
		closesocket(ClientSocket);
		WSACleanup();
		exit(0);
	}


	CnCServer_Size = sizeof(CnCServer);



	//---------- ��Ŷ�۽� ----------

	Send_Size = sendto(ClientSocket, Buffer, BUFFER_SIZE, 0,

		(struct sockaddr*) &CnCServer, CnCServer_Size);

	//---------- ��Ŷ�۽Ž� ����ó�� ----------

	if (Send_Size<0) { printf("������ �۽� ����.\n"); exit(0); }

	//---------- �۽��� ��Ŷ ��� ----------

	//printf("Message to %s \n", inet_ntoa(CnCServer.sin_addr));

	printf("Send: %s \n", Buffer);


	//Buffer �޸��� ��� ������ 0���� ����
	memset(Buffer, 0, BUFFER_SIZE);



	//---------- ��Ŷ���� ----------

	Recv_Size = recvfrom(ClientSocket, Buffer, BUFFER_SIZE, 0,

		(struct sockaddr*) &CnCServer, &CnCServer_Size);

	//---------- ��Ŷ���Ž� ����ó�� ----------

	if (Recv_Size<0) { printf("������ ���� ����.\n"); exit(0); }

	//---------- ������ ��Ŷ ��� ----------

	//printf("Message from %s \n", inet_ntoa(CnCServer.sin_addr));

	printf("Receive: %s \n", Buffer);



	//---------- �޾ƿ� �����Ϳ��� �������� IP�� ��Ʈ��ȣ �и��Ͽ� ���� ----------

	char *context = NULL;
	char *data[3];

	data[0] = strtok_s(Buffer, "_", &context); //OKAY
	data[1] = strtok_s(NULL, ":", &context); //������ ���� �ּ�
	data[2] = strtok_s(NULL, " ", &context); //������ ���� ��Ʈ��ȣ
	
	USHORT   A_ServerPort = atoi(data[2]); //������ ���� ��Ʈ��ȣ

	AttackServer.sin_family = AF_INET; //������ ���۹���� IPv4�� ����

	AttackServer.sin_addr.s_addr = inet_addr(data[1]); //������ ���� �ּ�

	AttackServer.sin_port = htons(A_ServerPort); //������ ���� ��Ʈ��ȣ

	AttackServer_Size = sizeof(AttackServer);


	char   AttackBuffer[BUFFER_SIZE] = { "2016111566" }; //������ ������ ������ �޽���



	//---------- �й� 10ȸ ���� ----------

	printf("IP: %s \n", inet_ntoa(AttackServer.sin_addr)); //������ IP ���
	printf("Port: %d \n", A_ServerPort); //������ ��Ʈ��ȣ ���

	for (int i = 0; i < 10; i++) { //10ȸ �ݺ�
		Send_Size = sendto(ClientSocket, AttackBuffer, BUFFER_SIZE, 0,

			(struct sockaddr*) &AttackServer, AttackServer_Size);

		if (Send_Size<0) { printf("������ �۽� ����.\n"); exit(0); }

		printf("Send: %s \n", AttackBuffer);
	}



	closesocket(ClientSocket); //������ �ݽ��ϴ�.

	WSACleanup(); //���� WS2_32.dll ����� �����ڴٰ� ȣ��
}