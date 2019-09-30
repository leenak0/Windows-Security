#include <stdio.h>
#include <string.h>
#include <windows.h>

#define BUFFER_SIZE 1024

#pragma comment (lib,"ws2_32.lib") //라이브러리 링크

void main(void)

{
	WSADATA   wsaData;

	SOCKET   ClientSocket;

	SOCKADDR_IN  CnCServer; //C&C서버
	SOCKADDR_IN  AttackServer; //공격할 서버

	int   CnCServer_Size;
	int   AttackServer_Size;

	int   Recv_Size;
	int   Send_Size;

	char   Buffer[BUFFER_SIZE] = { "IMHACKER_2016111566" }; //C&C 서버로 전송할 메시지

	USHORT   ServerPort = 10004; //C&C 서버 포트번호



	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) //WS2_32.dll 사용 시작
	{
		printf("WinSock 초기화부분에서 문제 발생.\n");
		WSACleanup(); //오류가 나면 WS2_32.dll 사용 끝
		exit(0);
	}

	//CncServer 메모리의 모든 내용을 0으로 설정
	memset(&CnCServer, 0, sizeof(CnCServer));
	//AttackServer 메모리의 모든 내용을 0으로 설정
	memset(&AttackServer, 0, sizeof(AttackServer));

	
	CnCServer.sin_family = AF_INET; //데이터 전송방식을 IPv4로 지정

	CnCServer.sin_addr.s_addr = inet_addr("114.70.37.17"); //C&C 서버 주소

	CnCServer.sin_port = htons(ServerPort); //C&C 서버 포트번호


	ClientSocket = socket(AF_INET, SOCK_DGRAM, 0);

	if (ClientSocket == INVALID_SOCKET)
	{
		printf("소켓을 생성할수 없습니다.\n");
		closesocket(ClientSocket);
		WSACleanup();
		exit(0);
	}


	CnCServer_Size = sizeof(CnCServer);



	//---------- 패킷송신 ----------

	Send_Size = sendto(ClientSocket, Buffer, BUFFER_SIZE, 0,

		(struct sockaddr*) &CnCServer, CnCServer_Size);

	//---------- 패킷송신시 에러처리 ----------

	if (Send_Size<0) { printf("데이터 송신 실패.\n"); exit(0); }

	//---------- 송신한 패킷 출력 ----------

	//printf("Message to %s \n", inet_ntoa(CnCServer.sin_addr));

	printf("Send: %s \n", Buffer);


	//Buffer 메모리의 모든 내용을 0으로 설정
	memset(Buffer, 0, BUFFER_SIZE);



	//---------- 패킷수신 ----------

	Recv_Size = recvfrom(ClientSocket, Buffer, BUFFER_SIZE, 0,

		(struct sockaddr*) &CnCServer, &CnCServer_Size);

	//---------- 패킷수신시 에러처리 ----------

	if (Recv_Size<0) { printf("데이터 수신 실패.\n"); exit(0); }

	//---------- 수신한 패킷 출력 ----------

	//printf("Message from %s \n", inet_ntoa(CnCServer.sin_addr));

	printf("Receive: %s \n", Buffer);



	//---------- 받아온 데이터에서 공격지의 IP와 포트번호 분리하여 저장 ----------

	char *context = NULL;
	char *data[3];

	data[0] = strtok_s(Buffer, "_", &context); //OKAY
	data[1] = strtok_s(NULL, ":", &context); //공격지 서버 주소
	data[2] = strtok_s(NULL, " ", &context); //공격지 서버 포트번호
	
	USHORT   A_ServerPort = atoi(data[2]); //공격지 서버 포트번호

	AttackServer.sin_family = AF_INET; //데이터 전송방식을 IPv4로 지정

	AttackServer.sin_addr.s_addr = inet_addr(data[1]); //공격지 서버 주소

	AttackServer.sin_port = htons(A_ServerPort); //공격지 서버 포트번호

	AttackServer_Size = sizeof(AttackServer);


	char   AttackBuffer[BUFFER_SIZE] = { "2016111566" }; //공격지 서버로 전송할 메시지



	//---------- 학번 10회 전송 ----------

	printf("IP: %s \n", inet_ntoa(AttackServer.sin_addr)); //공격지 IP 출력
	printf("Port: %d \n", A_ServerPort); //공격지 포트번호 출력

	for (int i = 0; i < 10; i++) { //10회 반복
		Send_Size = sendto(ClientSocket, AttackBuffer, BUFFER_SIZE, 0,

			(struct sockaddr*) &AttackServer, AttackServer_Size);

		if (Send_Size<0) { printf("데이터 송신 실패.\n"); exit(0); }

		printf("Send: %s \n", AttackBuffer);
	}



	closesocket(ClientSocket); //소켓을 닫습니다.

	WSACleanup(); //이제 WS2_32.dll 사용을 끝내겠다고 호출
}