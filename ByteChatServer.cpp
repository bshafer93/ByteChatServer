#include "ByteChatServer.h"

ByteChatServer::ByteChatServer()
{
   
}

ByteChatServer::~ByteChatServer()
{
    close(sockfd);
}

void ByteChatServer::Initialize()
{
    help_list.push_back("\\setname [name]               To give yourself a username.");
    help_list.push_back("\\help                         List available commands.");
    help_list.push_back("\\showonline                   Lists all active users currently in the chat.");
    help_list.push_back("\\setfontcolor                 change the color of the font.");

    std::cout << "Server Starting" << std::endl;

    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        std::cout << "socket() failed..." << std::endl;
        return;
    }
    else {
        std::cout << "Socket Aquired..." << std::endl;
    }

    //Ensure sockfd can accept multiple clients
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(int)) < 0) {
        std::cout << "setsockopt() failed..." << std::endl;
        return;
    }

    
    server_info.sin_family = AF_INET;
    inet_pton(AF_INET, IPADDR, &server_info.sin_addr.s_addr);
    server_info.sin_port = htons(PORT);

    
    if (bind(sockfd, (const sockaddr*)&server_info, sizeof(server_info)) == -1) {
        std::cout << "bind() failed..." << std::endl;
        return;
    }
    else {
        std::cout << "Bind Successful..." << std::endl;
    }


    if (listen(sockfd, BACKLOG)) {
        std::cout << "listen() failed..." << std::endl;
        return;

    }
    else {
        std::cout << "Listen Successful..." << std::endl;
    }
}

void ByteChatServer::Initialize(const char* ipaddr, int port)
{
    help_list.push_back("\\setname [name]               To give yourself a username.");
    help_list.push_back("\\help                         List available commands.");
    help_list.push_back("\\showonline                   Lists all active users currently in the chat.");


    std::cout << "Server Starting" << std::endl;

    // open TCP Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "socket() failed..." << std::endl;
    }
    else {
        std::cout << "Socket Aquired..." << std::endl;
    }

    //Ensure sockfd can accept multiple clients
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(int)) < 0) {
        std::cout << "setsockopt() failed..." << std::endl;
    }

    //Fill out server details
    server_info.sin_family = AF_INET;
    //serverInfo.sin_addr.s_addr = inet_addr("161.35.232.244");
    inet_pton(AF_INET, ipaddr, &server_info.sin_addr.s_addr);
    server_info.sin_port = htons(port);

    //bind to port
    if (bind(sockfd, (const sockaddr*)&server_info, sizeof(server_info)) == -1) {
        std::cout << "bind() failed..." << std::endl;
    }
    else {
        std::cout << "Bind Successful..." << std::endl;
    }




    // Listen to clients
    if (listen(sockfd, BACKLOG)) {
        std::cout << "listen() failed..." << std::endl;

    }
    else {
        std::cout << "Listen Successful..." << std::endl;
    }

}

void ByteChatServer::Run()
{

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        std::cout << "epoll_create() failed..." << std::endl;
    }

    eventd.events = EPOLLIN;
    eventd.data.fd = sockfd;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &eventd) == -1) {
        std::cout << "epoll_ctl() failed..." << std::endl;
    }
  
    
    while (true) {
        
        waitfd = epoll_wait(epollfd, events, MAXEVENTS, -1);

        for (int i = 0; i < waitfd; i++) {
            if (events[i].data.fd == sockfd) {
                NewClient();
            }
            else {
                bool result = HandleIncoming(events[i].data.fd);
                if (result == false) {
                    RemoveClient(events[i].data.fd);
                }
            }
        }

    }
}

void ByteChatServer::NewClient()
{

    //New Client connected

    // Connect to client
    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char hoststr[NI_MAXHOST];
    char portstr[NI_MAXSERV];
    std::cout << "New Client..." << std::endl;
    //accept4 Allows to set socket to be non blocking
    int newSock = accept4(sockfd, (struct sockaddr*) & client_addr, &addr_size,SOCK_NONBLOCK);
    clientsfd.push_back(newSock);

    eventd.events = EPOLLIN | EPOLLET;
    eventd.data.fd = newSock;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, newSock, &eventd) == -1) {
        std::cout << "epoll_ctl() failed..." << std::endl;
    }

    getnameinfo((struct sockaddr*) & client_addr, addr_size, hoststr, sizeof(hoststr), portstr, sizeof(portstr), NI_NUMERICHOST | NI_NUMERICSERV);
    //Add Client to Currently Connected List
    std::string na = "";
    connected_clients.insert({ newSock, ClientInfo(newSock,na,hoststr) });
    std::cout << "Client Connected: " << GetCurTime() << std::endl;
    std::cout << "Client IP:" << hoststr << std::endl;
    std::string hello = "Welcome, press tab to activate chat input.";
    std::string info = "Please use \\help for more information.";
    std::cout << "New Client Connected..." << std::endl;
    SendString(newSock, hello);
    SendString(newSock, info);
}

void ByteChatServer::RemoveClient(int clientfd)
{
  
    epoll_ctl(epollfd, EPOLL_CTL_DEL, clientfd, &eventd);
    close(clientfd);
    remove(clientsfd.begin(), clientsfd.end(), clientfd);
   std::cout<< connected_clients.at(clientfd).GetName() <<" has gone offline." << std::endl;
    connected_clients.erase(clientfd);
  

}

bool ByteChatServer::HandleIncoming(int clientsockfd)
{
    char buff[1024] = { 0 };
    int bytesRecv = recv(clientsockfd, buff, 1023, 0);

    if (bytesRecv <= 0) {

        return false;
    }
    else {

        Packet p(buff);
        std::cout << "----------------------------Packet Received-------------------------" << std::endl;
        p.PrintPacketInfo();
        std::cout << "--------------------------------------------------------------------" << std::endl;

        //Get who sent the packet... map structure is fine for low user count.
        ClientInfo* from = &connected_clients.at(clientsockfd);

        switch (p.msgtype)
        {
        case Packet::MSGTYPE::STRING:
            SendToAll(from, *p.data.str);
            break;
        case Packet::MSGTYPE::COMMAND:
            HandleCommandPacket(from,&p);
            break;
        default:
            break;
        }
        return true;
    }

}

void ByteChatServer::HandleCommandPacket(ClientInfo* from, Packet* p)
{

    switch (p->data.cmd)
    {
        case Packet::COMMANDTYPE::SETNAME:
        {
            if (!from->SetName(*p->data.arg)) {
                SendString(from->GetFd(), "Name exceeds character limit of 32.");
            }
            else {
                std::string response = "Name Set to: <" + *p->data.arg + ">";
                SendString(from->GetFd(), response);
            }
            break;
        }
        case Packet::COMMANDTYPE::HELP:
        {
            //Loop Through all Commands
            SendString(from->GetFd(), "------------------------HELP----------------------------------");
            
            for (int i = 0; i < help_list.size(); i++) 
            {
                SendString(from->GetFd(), help_list[i]);        
            }

            
         
          
            SendString(from->GetFd(), "--------------------------------------------------------------");
            break;
        }

        
        case Packet::COMMANDTYPE::LSONLINE:
        {
            SendString(from->GetFd(), "------------------------ONLINE----------------------------------");
            for (auto client : connected_clients) {
           
                SendString(from->GetFd(), client.second.GetName());
              
            }
            SendString(from->GetFd(), "----------------------------------------------------------------------");
            break;
        }
        default:
            break;
    }
}
    


std::string ByteChatServer::GetCurTime()
{
    std::chrono::system_clock::time_point cur = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(cur);
    std::tm* ptm = std::localtime(&t);
    char buffer[32];
    // Format: Mo, 15.06.2009 20:20:00
    std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);

    return std::string(buffer);
}

void ByteChatServer::SendString(int clientfd,std::string s)
{

    Packet p(Packet::MSGTYPE::STRING, s);
    if (send(clientfd, p.BuildPacket(), p.size, 0) == -1) {

        std::cout << "send() failed..." << std::endl;
    }
    

}

void ByteChatServer::SendToAll(ClientInfo* fromClient , std::string s) {

    std::cout << fromClient->GetName() << ":" << s << std::endl;
    std::string newMsg = "<" + fromClient->GetName() + ">:";
    newMsg.append(s);
    Packet msgP(Packet::MSGTYPE::STRING, newMsg);

    for (auto client : connected_clients) {
        
        std::cout << "Sending to" << client.second.GetName() << std::endl;
            if (send(client.second.GetFd(), msgP.BuildPacket(), msgP.size, 0) == -1) {
                
                std::cout << "send() failed..." << std::endl;
            }
       
    }
}
