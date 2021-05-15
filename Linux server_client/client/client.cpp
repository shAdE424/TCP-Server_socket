/*

    This software is the first version of NOTI-CHAT TCP client (Unix / Linux)

    All rights reserved by Notibus Inc.

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


// Struct to store variebles and data structures
struct vars
{
    int port = 31000;
    int sock_len = sizeof(sockaddr_in);

    // Loop-back address (local machine)
    std::string ipAddress = "127.0.0.1";
    std::string messageInUsr;

    bool fail_flag_clientConnection_sock = false;
    bool fail_flag_connection_result = false;
    bool fail_flag_message_send_result = false;

    char messageBuffer[4096];
};

// Error program exit func
void errorExit(bool fatal_error, std::string fail_origin, int error_code)
{
    if (fatal_error == true)
    {
        std::cout << "Fatal error occured with " << fail_origin << "!\n";
        exit(error_code);
    }
    else
    {
        std::cout << fail_origin << " operation successed \n";
    }   
}

void connectionLog(bool fatal_error, std::string fail_origin, int error_code)
{
    std::cout << "Fatal error occured with " << fail_origin << "!\n";
}

// Checking if failure is occured 
template <typename T>
bool isFailure(const T& atribute)
{
    return (atribute == -1) ? true : false;
}

int main()
{
    vars var;

//########################################################################################//
    // Create a socket

    int serverConnection = socket(AF_INET, SOCK_STREAM, 0);

//------------------------------------------------------------------------//
    // Errors check

    var.fail_flag_clientConnection_sock = isFailure(serverConnection);

    errorExit(var.fail_flag_clientConnection_sock, SERVER_CONNCT_SOCK, SERVER_CONNCT_SOCK_ERROR);
//------------------------------------------------------------------------//
//########################################################################################//

//########################################################################################//
    // Create a connectionSet structure for the server we're connecting with

    sockaddr_in connectionSet;

    connectionSet.sin_family = AF_INET;
    connectionSet.sin_port = htons(var.port);

    inet_pton(AF_INET, var.ipAddress.c_str(), &connectionSet.sin_addr);
//########################################################################################//

//########################################################################################//
    // Connect to the server on the socket

    int connectionSetResult = connect(serverConnection, (sockaddr *)&connectionSet, var.sock_len);

//------------------------------------------------------------------------//
    // Errors check

    var.fail_flag_connection_result = isFailure(connectionSetResult);

    errorExit(var.fail_flag_connection_result, SERVER_CONNCT_RESULT, SERVER_CONNCT_SOCK_ERROR);
//------------------------------------------------------------------------//
//########################################################################################//

//########################################################################################//
    // While loop: 

    do
    {
        // Enter lines of text

        std::cout << "| Enter message << ";

        getline(std::cin, var.messageInUsr);

        // Send to server

        int messageSendResult = send(serverConnection, var.messageInUsr.c_str(), var.messageInUsr.size() + 1, 0);

    //------------------------------------------------------------------------//
        // Errors check

        var.fail_flag_message_send_result = isFailure(messageSendResult);

        // If message ws not delivered because of some errors skip cleaning buffer and start again
        if (var.fail_flag_message_send_result == true) 
        { 
            connectionLog(var.fail_flag_message_send_result, MESSAGE_SEND_RESULT, MESSAGE_SEND_RESULT_ERROR);
            continue; 
        }
    //------------------------------------------------------------------------//

        // Wait for response

        memset(var.messageBuffer, 0, 4096);

        int bytesRecieved = recv(serverConnection, var.messageBuffer, 4096, 0);

        // Display response

        std::cout << "Server response >> " << std::string(var.messageBuffer, bytesRecieved) << "\n";

    } while (true);
//########################################################################################//

//########################################################################################//
   // Close socket

   close(serverConnection);
//########################################################################################//
}