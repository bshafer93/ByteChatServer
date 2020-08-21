#pragma once
#include<string>
class ClientInfo
{
public:
	ClientInfo();
	ClientInfo(int socketfd,char * n,char*ipAddr);
	ClientInfo(int socketfd, std::string& n, char* ipAddr);
	~ClientInfo();

	void SetFd(int clientfd);
	int GetFd();

	bool SetName(std::string n);
	std::string GetName();

	void SetIP(std::string ipAddr);
	std::string GetIP();


	//Custom Operators
	bool operator< (const ClientInfo& rhs) const {
		if (client_fd < rhs.client_fd) {
			return true;
		}
		return false;
	}

private:
	int client_fd;
	std::string name;
	std::string ip;

};

