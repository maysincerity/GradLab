#pragma once
#include "stdafx.h"
#include <iostream>
#include <string>
#include "Server.h"
#include "WinsockEnv.h"
#include "Config.h"
#include <winsock2.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "ChatRoomClientGUI.h"
#include "ChatRoomClientGUIDlg.h"

#pragma comment(lib, "Ws2_32.lib")

#pragma warning(disable:4996)


Server::Server(string ip, int port, string addr)
{
	this->ip = ip;//����IP
	this->port = port;//���ö˿�
	this->addr = addr;//������Ŀ¼
	this->is_close = 0;//������������־

	this->recvBuf = new char[Config::BUFFERLENGTH]; //��ʼ�����ܻ�����
	memset(this->recvBuf, '\0', Config::BUFFERLENGTH);

	this->rcvedMessages = new list<string>();
	this->sessions = new list<SOCKET>();
	this->closedSessions = new list<SOCKET>();
	this->clientAddrMaps = new map<SOCKET, string>();
}

Server::~Server(void)
{
	//�ͷŽ��ܻ�����
	if (this->recvBuf != NULL) {
		delete this->recvBuf;
		this->recvBuf = NULL;
	}


	//�ر�server socket
	if (this->srvSocket != NULL) {
		closesocket(this->srvSocket);
		this->srvSocket = NULL;
	}

	WSACleanup(); //����winsock ���л���
}

//��ʼ��Winsock
int Server::WinsockStartup() {
	if (WinsockEnv::Startup() == -1) return -1;	//��ʼ��Winsock
	return 0;
}

//��ʼ��Server����������sockect���󶨵�IP��PORT
int Server::ServerStartup() {
	//���� TCP socket
	this->srvSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->srvSocket == INVALID_SOCKET) {
		cout << "Server socket creare error !\n";
		WSACleanup();
		return -1;
	}
	cout << "Server socket create ok!\n";

	//���÷�����IP��ַ�Ͷ˿ں�
	this->srvAddr.sin_family = AF_INET;
	this->srvAddr.sin_port = htons(this->port);//���ö˿�
	this->srvAddr.sin_addr.S_un.S_addr = inet_addr(this->ip.c_str()); //����IP

	//�� socket to Server's IP and port
	int rtn = bind(this->srvSocket, (LPSOCKADDR)&(this->srvAddr), sizeof(this->srvAddr));
	if (rtn == SOCKET_ERROR) {
		cout << "Server socket bind error!\n";
		closesocket(this->srvSocket);
		WSACleanup();
		return -1;
	}

	cout << "Server socket bind ok!\n";
	return 0;
}

//��ʼ����,�ȴ��ͻ�����������
int Server::ListenStartup() {
	int rtn = listen(this->srvSocket, Config::MAXCONNECTION);
	if (rtn == SOCKET_ERROR) {
		cout << "Server socket listen error!\n";
		closesocket(this->srvSocket);
		WSACleanup();
		return -1;
	}

	cout << "Server socket listen ok!\n";
	return 0;
}

//���յ��Ŀͻ�����Ϣ���浽��Ϣ����
void Server::AddRecvMessage(string str) {
	if (!str.empty())
		this->rcvedMessages->insert(this->rcvedMessages->end(), str);

}

//���µĻỰSOCKET�������
void Server::AddSession(SOCKET session) {
	if (session != INVALID_SOCKET) {
		this->sessions->insert(this->sessions->end(), session);
	}
}

//��ʧЧ�ĻỰSOCKET�������
void Server::AddClosedSession(SOCKET session) {
	if (session != INVALID_SOCKET) {
		this->closedSessions->insert(this->closedSessions->end(), session);
	}
}

//��ʧЧ��SOCKET�ӻỰSOCKET����ɾ��
void Server::RemoveClosedSession(SOCKET closedSession) {
	if (closedSession != INVALID_SOCKET) {
		list<SOCKET>::iterator itor = find(this->sessions->begin(), this->sessions->end(), closedSession);
		if (itor != this->sessions->end())
			this->sessions->erase(itor);
	}
}

//��ʧЧ��SOCKET�ӻỰSOCKET����ɾ��
void Server::RemoveClosedSession() {
	for (list<SOCKET>::iterator itor = this->closedSessions->begin(); itor != this->closedSessions->end(); itor++) {
		this->RemoveClosedSession(*itor);
	}
}

//��SOCKET s������Ϣ
void Server::recvMessage(SOCKET socket) {
	int receivedBytes = recv(socket, this->recvBuf, Config::BUFFERLENGTH, 0);
	if (receivedBytes == SOCKET_ERROR || receivedBytes == 0) {//�������ݴ��󣬰Ѳ�������ĻỰsocekt����sessionsClosed����
		this->AddClosedSession(socket);
	}
	else {
		recvBuf[receivedBytes] = '\0';
		string s("����" + this->GetClientAddress(this->clientAddrMaps, socket) + "�������:" + recvBuf + "\n");
		this->AddRecvMessage(s); //���յ�����Ϣ���뵽��Ϣ����
		cout << s;
		memset(this->recvBuf, '\0', Config::BUFFERLENGTH);//������ܻ�����
	}
}

//��SOCKET s������Ϣ
void Server::sendMessage(SOCKET socket, string msg) {
	int rtn = send(socket, msg.c_str(), msg.length(), 0);
	if (rtn == SOCKET_ERROR) {//�������ݴ��󣬰Ѳ�������ĻỰsocekt����sessionsClosed����
		string s("����" + this->GetClientAddress(this->clientAddrMaps, socket) + "��������뿪��\n");
		this->AddRecvMessage(s);
		this->AddClosedSession(socket);
		cout << s;
	}
}

//�������ͻ�ת����Ϣ
void Server::ForwardMessage() {
	if (this->numOfSocketSignaled > 0) {
		if (!this->rcvedMessages->empty()) {//�����Ϣ���в�Ϊ��
			for (list<string>::iterator msgItor = this->rcvedMessages->begin(); msgItor != this->rcvedMessages->end(); msgItor++) {//����Ϣ�����е�ÿһ����Ϣ
				for (list<SOCKET>::iterator sockItor = this->sessions->begin(); sockItor != this->sessions->end(); sockItor++) {//�ԻỰsocket�����е�ÿ��socket
					this->sendMessage(*sockItor, *msgItor);
				}
			}
		}
		this->rcvedMessages->clear(); //�������ͻ�ת����Ϣ�������Ϣ����
	}
}


int Server::AcceptRequestionFromClient() {
	//���srvSocket�Ƿ��յ��û���������
	if (this->numOfSocketSignaled > 0) {
		if (FD_ISSET(this->srvSocket, &rfds)) {  //�пͻ�����������
			this->numOfSocketSignaled--;

			//���̴߳�������
			HANDLE t_socket = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)socket_thread, (LPVOID)(this), 0, 0);
			CloseHandle(t_socket);
		}
	}
	return 0;
}



void Server::ReceieveMessageFromClients() {
	if (this->numOfSocketSignaled > 0) {
		//�����Ự�б��е�����socket������Ƿ������ݵ���
		for (list<SOCKET>::iterator itor = this->sessions->begin(); itor != this->sessions->end(); itor++) {
			if (FD_ISSET(*itor, &rfds)) {  //ĳ�Ựsocket�����ݵ���
				//��������
				this->recvMessage(*itor);
			}
		}//end for
	}
}

//�õ��ͻ���IP��ַ,�˿ں�
string  Server::GetClientAddress(SOCKET s) {
	string clientAddress; //clientAddress�Ǹ����ַ����� clientAddress.empty() is true
	sockaddr_in clientAddr;
	int nameLen, rtn;

	nameLen = sizeof(clientAddr);
	rtn = getsockname(s, (LPSOCKADDR)&clientAddr, &nameLen);
	if (rtn != SOCKET_ERROR) {
		clientAddress += inet_ntoa(clientAddr.sin_addr);
		clientAddress += ':';
		clientAddress += to_string(clientAddr.sin_port);
	}

	return clientAddress;
}

//�õ��ͻ���IP��ַ
string  Server::GetClientAddress(map<SOCKET, string> *maps, SOCKET s) {
	map<SOCKET, string>::iterator itor = maps->find(s);
	if (itor != maps->end())
		return (*itor).second;
	else {
		return string("");
	}

}

//���ܿͻ��˷�������������ݲ�ת��
int Server::Loop() {
	if (this->is_close)//�������ر�
		return 1;
	u_long blockMode = Config::BLOCKMODE;//��srvSock��Ϊ������ģʽ�Լ����ͻ���������
	int rtn;

	if ((rtn = ioctlsocket(this->srvSocket, FIONBIO, &blockMode) == SOCKET_ERROR)) { //FIONBIO��������ֹ�׽ӿ�s�ķ�����ģʽ��
		cout << "ioctlsocket() failed with error!\n";
		return -1;
	}
	cout << "ioctlsocket() for server socket ok!Waiting for client connection and data\n";

	while (true) {//�ȴ���������
		//Prepare the read and write socket sets for network I/O notification.
		FD_ZERO(&this->rfds);

		//��srvSocket���뵽rfds���ȴ��û���������
		FD_SET(this->srvSocket, &this->rfds);

		//�ȴ��û�����������û����ݵ�����Ựsocket�ɷ�������
		if ((this->numOfSocketSignaled = select(0, &this->rfds, NULL, NULL, NULL)) == SOCKET_ERROR) { //select���������пɶ����д��socket��������������rtn��.���һ�������趨�ȴ�ʱ�䣬��ΪNULL��Ϊ����ģʽ
			cout << "select() failed with error!\n";
			return -1;
		}
		if (this->AcceptRequestionFromClient() != 0) 
			return -1;
	}
	return 0;
}


void socket_thread(Server *server) {
	if (!server) {
		return;
	}
	u_long blockMode = Config::BLOCKMODE;//��session socket��Ϊ������ģʽ�Լ����ͻ���������
	sockaddr_in clientAddr;		//�ͻ���IP��ַ
	int namelen = sizeof(clientAddr);
	//�����Ựsocket
	SOCKET newSession = accept(server->srvSocket, (LPSOCKADDR)&(clientAddr), &namelen);
	//��ʾIP���˿�
	string property = string("Client IP:") + string(inet_ntoa(clientAddr.sin_addr)) + ", Port��" + to_string(ntohs(clientAddr.sin_port));
	if (newSession == INVALID_SOCKET) {//ʧЧsocket
		return;
	}
	//������Ӧ����
	string cmd;
	char buf[1024];
	while (true) {
		int receivedBytes = recv(newSession, buf, Config::BUFFERLENGTH, 0);
		//�������ݴ��󣬰Ѳ�������ĻỰsocekt�����ɾ��
		if (receivedBytes == SOCKET_ERROR || receivedBytes == 0) 
		{
			closesocket(newSession);
			break;
		}
		else 
		{
			buf[receivedBytes] = '\0';//����
			cmd = buf;
			if (cmd.find("Accept-Language") == string::npos && cmd.find("Host") != string::npos )
			{				
				string data;
				string head;
				int first_bit = cmd.find("GET /") + 5;//Ŀ¼��ʼ
				int last_bit = cmd.find(" HTTP") - 5;//����
				string req_addr = cmd.substr(first_bit, last_bit);//Ŀ¼
				string status_str = cmd.substr(0, last_bit + 14);
				fstream in((server->addr + req_addr), ios::binary | ios::in);

				if (in.is_open()) {
					ostringstream buffer;
					buffer << in.rdbuf();
					data = buffer.str();
					in.close();
					//��Ӧ����
					head = "HTTP/1.1 200 OK\r\nContent - type:text/plain\r\n\r\n";
					if (req_addr.find(".jpg") != string::npos) 
					{
						head= "HTTP/1.1 200 OK\r\nContent - type:image/jpeg\r\n\r\n";
					}
					if (req_addr.find(".gif") != string::npos) 
					{
						head = "HTTP/1.1 200 OK\r\nContent - type:image/gif\r\n\r\n";
					}
					if (req_addr.find(".txt") != string::npos) 
					{
						head = "HTTP/1.1 200 OK\r\nContent - type:text/html\r\n\r\n";
					}
					if (req_addr.find(".png") != string::npos)
					{
						head = "HTTP/1.1 200 OK\r\nContent - type:image/png\r\n\r\n";
					}
					if (req_addr.find(".ico") != string::npos)
					{
						head = "HTTP/1.1 200 OK\r\nContent - type:image/x-ico\r\n\r\n";
					}
					if (req_addr.find(".mp3") != string::npos)
					{
						head = "HTTP/1.1 200 OK\r\nContent - type:audio/mp3\r\n\r\n";
					}
					if (req_addr.find(".js") != string::npos)
					{
						head = "HTTP/1.1 200 OK\r\nContent - type:application/x-javascript\r\n\r\n";
					}
					if (req_addr.find(".css") != string::npos)
					{
						head = "HTTP/1.1 200 OK\r\nContent - type:text/css\r\n\r\n";
					}
					data = head + data;
				}
				else {//�Ҳ����ļ�
					data = "HTTP/1.1 404 NotFound\r\nContent-Type: text/html\r\n\r\n" + data;
				}
				CChatRoomClientGUIDlg *dlg = (CChatRoomClientGUIDlg *)(AfxGetApp()->GetMainWnd());
				dlg->m_List.AddString(CString(property.c_str()));
				dlg->m_List.AddString(CString(status_str.c_str()));
				int rtn = send(newSession, data.c_str(), data.length(), 0);
				dlg->m_List.AddString(CString(string("Send successfully.").c_str()));
				dlg->m_List.AddString(CString(string("").c_str()));
			}
			else
			{
				closesocket(newSession);
				break;
			}
		}
	}
}

