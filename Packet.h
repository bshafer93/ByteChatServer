#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include <chrono>
#include <cstring>
#define PACKET_DEBUG

class Packet {

public:
    enum MSGTYPE {
        STRING = 100,
        COMMAND,
        HEARTBEAT,
        UNKNOWNTYPE
    }msgtype;

    enum COMMANDTYPE {
        SETNAME = 100,
        HELP,
        LSONLINE,
        FONTCOLOR,
        UNKNOWNCMD
    };

    Packet(Packet::MSGTYPE m);
    Packet(Packet::MSGTYPE m, std::string str);
    Packet(Packet::MSGTYPE m, Packet::COMMANDTYPE c, std::string str);
    Packet(char* b);
    ~Packet();

    uint8_t* BuildPacket();

    static void ReadPacket(char* p);
    void PrintPacketInfo();

    union DATA {
        struct {
            std::string* str;
        };

        struct {
            std::string* arg;
            COMMANDTYPE cmd;
        };

    }data;

    int size;

};