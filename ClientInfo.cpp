#include "ClientInfo.h"

ClientInfo::ClientInfo()
{

}

ClientInfo::ClientInfo(int socketfd, char* n, char* ipAddr)
{
	client_fd = socketfd;
	name = std::string(n);
	ip = std::string(ipAddr);
}

ClientInfo::ClientInfo(int socketfd, std::string& n, char* ipAddr)
{
	client_fd = socketfd;
	name = n;
	ip = std::string(ipAddr);

}

ClientInfo::~ClientInfo()
{

}

void ClientInfo::SetFd(int clientfd)
{
	client_fd = clientfd;
}

int ClientInfo::GetFd()
{
	return client_fd;
}

bool ClientInfo::SetName(std::string n)
{
	if (n.length() > 32) {
		return false;
	}

	name = n;
	return true;
}

std::string ClientInfo::GetName()
{
	return name;
}

void ClientInfo::SetIP(std::string ipAddr)
{
	ip = ipAddr;
}

std::string ClientInfo::GetIP()
{
	return ip;
}
