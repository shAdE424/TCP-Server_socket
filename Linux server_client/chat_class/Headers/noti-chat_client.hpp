#pragma once

/*
    This class is the part of Noti-Chat client

    Messaging software produced by "Notibus Inc."
*/

#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <string>

// Macros
#define SERVER_CONNCT_SOCK "server connection socket"
#define SERVER_CONNCT_SOCK_ERROR -6
#define SERVER_CONNCT_RESULT "server connection"
#define SERVER_CONNCT_RESULT_ERROR -7
#define MESSAGE_SEND_RESULT "message send"
#define MESSAGE_SEND_RESULT_ERROR -8

class NotiChat_Client
{
private:

    int port = 54000;
    int sock_len = sizeof(sockaddr_in);

    // Loop-back address (local machine)
    std::string ipAddress = "127.0.0.1";

    std::string messageInUsr;

    bool fail_flag_clientConnection_sock = false;
    bool fail_flag_connection_result = false;
    bool fail_flag_message_send_result = false;

    char messageBuffer[4096];

public:

    NotiChat_Client()
    {}

    void errorExit(bool fatal_error, std::string fail_origin, int error_code);

    void connectionLog(bool fatal_error, std::string fail_origin, int error_code);

    bool isFailure(const int& atribute);
};