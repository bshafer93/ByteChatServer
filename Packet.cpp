
#include "Packet.h"

Packet::Packet(Packet::MSGTYPE m) {
    msgtype = m;
    size = 2;
}

Packet::Packet(Packet::MSGTYPE m, std::string str) {
    //Set Type
    msgtype = m;
    //append
    data.str = new std::string;
    data.str->append(str.c_str());
    size = 2 + static_cast<int>(str.size());

}

Packet::Packet(Packet::MSGTYPE m, Packet::COMMANDTYPE c, std::string str) {
    //Set Type
    msgtype = m;
    data.cmd = c;

    switch (data.cmd)
    {
    case Packet::COMMANDTYPE::SETNAME:
        data.arg = new std::string;
        data.arg->append(str.c_str());
        size = 3 + static_cast<int>(str.size());
        break;
    case Packet::COMMANDTYPE::UNKNOWNCMD:
        //TODO
        break;
    case Packet::COMMANDTYPE::HELP:
    {
        data.arg = new std::string;
        data.arg->append(str.c_str());
        size = 3 + static_cast<int>(str.size());
        break;
    }
    case Packet::COMMANDTYPE::LSONLINE:
    {
        data.arg = new std::string;
        data.arg->append(str.c_str());
        size = 3 + static_cast<int>(str.size());
        break;
    }


    default:
        //TODO
        break;
    }

}



Packet::Packet(char* b)
{
    msgtype = (Packet::MSGTYPE)b[0];
    size = *((char*)(b + 1));


    if (msgtype == Packet::MSGTYPE::HEARTBEAT) {

    }

    else if (msgtype == Packet::MSGTYPE::STRING) {
        std::string s;
        data.str = new std::string;
        char* tmp = new char[size - 1];
        //TODO  need to find a less hacky way  
        memcpy(tmp, b + 2, size - 1);
        tmp[size - 2] = '\0';
        data.str->append(tmp);
    }

    else if (msgtype == Packet::MSGTYPE::COMMAND) {

        data.cmd = static_cast<Packet::COMMANDTYPE>(*((char*)(b + 2)));
        switch (data.cmd)
        {
        case Packet::COMMANDTYPE::SETNAME:
        {
            std::string s;
            data.arg = new std::string;
            char* tmp = new char[size - 2];
            //TODO  need to find a less hacky way 
            memcpy(tmp, b + 3, size - 2);
            tmp[size - 3] = '\0';
            data.arg->append(tmp);
            break;
        }
        case Packet::COMMANDTYPE::HELP:
        {
            std::string s;
            data.arg = new std::string;
            char* tmp = new char[size - 2];
            //TODO  need to find a less hacky way 
            memcpy(tmp, b + 3, size - 2);
            tmp[size - 3] = '\0';
            data.arg->append(tmp);
            break;
        }
        case Packet::COMMANDTYPE::LSONLINE:
        {
            std::string s;
            data.arg = new std::string;
            char* tmp = new char[size - 2];
            //TODO  need to find a less hacky way 
            memcpy(tmp, b + 3, size - 2);
            tmp[size - 3] = '\0';
            data.arg->append(tmp);
            break;

        }
        default:
            break;
        }
    }

}

Packet::~Packet() {

}

uint8_t* Packet::BuildPacket() {
    uint8_t* packet;

#ifdef PACKET_DEBUG 
    std::cout << "----------------------------Building Packet-------------------------" << std::endl;
    PrintPacketInfo();
#endif

    //Create Packet
    packet = new uint8_t[size];
    //Zero Out packet memory
    memset(packet, 0, size);


    switch (msgtype)
    {
    case Packet::MSGTYPE::HEARTBEAT:
        //Fill Packet Details...Note, size will overwrite any number larger than a char
        packet[0] = Packet::MSGTYPE::HEARTBEAT;

        //TODO  redo to find a more elegant solution to extract size
        *((char*)(packet + 1)) = size;
#ifdef PACKET_DEBUG 
        std::cout << "--------------------------------------------------------------------" << std::endl;
#endif #endif

        return packet;
        break;
    case Packet::MSGTYPE::STRING:
        packet[0] = msgtype;
        //TODO  redo to find a more elegant solution to extract size
        *((char*)(packet + 1)) = size;
        memcpy(packet + 2, data.str->c_str(), data.str->size());
#ifdef PACKET_DEBUG 
        std::cout << "--------------------------------------------------------------------" << std::endl;
#endif #endif
        return packet;
        break;
    case Packet::MSGTYPE::COMMAND:
        packet[0] = msgtype;
        //TODO  redo to find a more elegant solution to extract size
        *((char*)(packet + 1)) = size;
        *((char*)(packet + 2)) = static_cast<int>(data.cmd);
        memcpy(packet + 3, data.str->c_str(), data.str->size());
#ifdef PACKET_DEBUG 
        std::cout << "--------------------------------------------------------------------" << std::endl;
#endif #endif
        return packet;
        break;
    default:
        break;
    }

    return nullptr;
}



void Packet::PrintPacketInfo()
{
    switch (msgtype)
    {
    case Packet::MSGTYPE::HEARTBEAT:
        std::cout << "Packet Type: HEARTBEAT" << std::endl;
        std::cout << "Packet Size: " << size << " bytes" << std::endl;
        break;

    case Packet::MSGTYPE::STRING:
        std::cout << "Packet Type: STRING" << std::endl;
        std::cout << "Packet Size: " << size << " bytes" << std::endl;
        std::cout << "Packet Message: " << *data.str << std::endl;
        break;
    case Packet::MSGTYPE::COMMAND:
        std::cout << "Packet Type: COMMAND" << std::endl;
        if (data.cmd == Packet::COMMANDTYPE::SETNAME) {
            std::cout << "COMMAND TYPE: (setname)" << std::endl;
        }
        else if (data.cmd == Packet::COMMANDTYPE::HELP) {
            std::cout << "COMMAND TYPE: (help)" << std::endl;
        }
        else if (data.cmd == Packet::COMMANDTYPE::LSONLINE) {
            std::cout << "COMMAND TYPE: (showonline)" << std::endl;
        }

        std::cout << "Packet Size: " << size << " bytes" << std::endl;
        std::cout << "Packet Message: " << *data.arg << std::endl;
        break;
    default:
        std::cout << "Packet Type: UNKNOWN" << std::endl;
        break;
    }

}
