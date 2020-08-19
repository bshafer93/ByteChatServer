#pragma once


#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <cstring>
#include <vector>
#include <set>
#include <ctime>
#include <sstream>
#include <thread>
#include <future>
#include<bits/stdc++.h>
#include "Packet.h"
#include "ClientInfo.h"

#define PORT 13900
#define PORTSTR "13900"
#define BACKLOG 50
#define MAXEVENTS 50
#define IPADDR "161.35.232.244"
#define MAXMSGLEN 256


class ByteChatServer
{
	public:
		ByteChatServer();
		~ByteChatServer();
		void Initialize();
		void Initialize(const char* ipaddr,int port);
		void Run();
		void NewClient();
		void RemoveClient(int clientfd);
		bool HandleIncoming(int clientsockfd);
		void HandleCommandPacket(ClientInfo* from,Packet *p);
		static std::string GetCurTime();
		void SendString(int clientfd, std::string s);
		void SendToAll(ClientInfo* fromClient,std::string s);



	private:
		epoll_event events[MAXEVENTS];
		std::vector<int> clientsfd;
		std::map<int,ClientInfo> connClients;
		epoll_event eventd;
		int sockfd,epollfd,waitfd;
		struct sockaddr_in serverInfo;
		int opt = true;
		std::vector<std::string> HELPLIST;

};

