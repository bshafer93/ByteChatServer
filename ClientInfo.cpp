#include "ClientInfo.h"

ClientInfo::ClientInfo()
{

}

ClientInfo::ClientInfo(int socketfd, char* n, char* ipAddr)
{
	fd = socketfd;
	name = std::string(n);
	ip = std::string(ipAddr);
}

ClientInfo::ClientInfo(int socketfd, std::string& n, char* ipAddr)
{
	fd = socketfd;
	name = n;
	ip = std::string(ipAddr);

}

ClientInfo::~ClientInfo()
{

}

void ClientInfo::Set_fd(int clientfd)
{
	fd = clientfd;
}

int ClientInfo::Get_fd()
{
	return fd;
}

bool ClientInfo::Set_name(std::string n)
{
	if (n.length() > 32) {
		return false;
	}

	name = n;
	return true;
}

std::string ClientInfo::Get_name()
{
	return name;
}

void ClientInfo::Set_ip(std::string ipAddr)
{
	ip = ipAddr;
}

std::string ClientInfo::Get_ip()
{
	return ip;
}
