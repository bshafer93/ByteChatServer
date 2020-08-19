#pragma once
#include<string>
class ClientInfo
{
public:
	ClientInfo();
	ClientInfo(int socketfd,char * n,char*ipAddr);
	ClientInfo(int socketfd, std::string& n, char* ipAddr);
	~ClientInfo();

	void Set_fd(int clientfd);
	int Get_fd();

	bool Set_name(std::string n);
	std::string Get_name();

	void Set_ip(std::string ipAddr);
	std::string Get_ip();


	//Custom Operators
	bool operator< (const ClientInfo& rhs) const {
		if (fd < rhs.fd) {
			return true;
		}
		return false;
	}

private:
	int fd;
	std::string name;
	std::string ip;

};

