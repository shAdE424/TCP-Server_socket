/*

    This software is the first version of NOTI-CHAT TCP server (Unix / Linux)

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
#include <time.h> //FD_SET, FD_ISSET, FD_ZERO macros


// Macros
#define SERVER_SOCK "server socket"
#define SERVER_SOCK_ERROR -1
#define BIND_SOCK "binding socket to the IP / port"
#define BIND_SOCK_ERROR -2
#define LISTEN_SOCK "listening acception"
#define LISTEN_SOCK_ERROR -3
#define CALL_ACCEPT "call accaption"
#define CALL_ACCEPT_ERROR -4  
#define RECEIVE_DATA "recieved data"
#define RECEIVE_DATA_ERROR -5


// Struct to store variebles and data structures
struct vars
{
    int bytesRecieve;

    bool fail_flag_server_sock = false;
    bool fail_flag_bind_sock = false;
    bool fail_flag_listen_sock = false;
    bool fail_flag_received_data = false;
    bool fail_flag_client_sock = false;

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
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
    // Create socket

    int serverListeningSock = socket(AF_INET, SOCK_STREAM, 0);

//------------------------------------------------------------------------//
    // Errors check

    var.fail_flag_server_sock = isFailure(serverListeningSock);

    errorExit(var.fail_flag_server_sock, SERVER_SOCK, SERVER_SOCK_ERROR);
//------------------------------------------------------------------------//
//########################################################################################//

//########################################################################################//
    // Bind the socket to a IP / port

    sockaddr_in bind_sock;

    bind_sock.sin_family = AF_INET;
    bind_sock.sin_port = htons(31000);

    inet_pton(AF_INET, "0.0.0.0", &bind_sock.sin_addr);

//------------------------------------------------------------------------//
    // Errors check

    var.fail_flag_bind_sock = isFailure(bind(serverListeningSock, (sockaddr*)&bind_sock, sizeof(bind_sock)));

    errorExit(var.fail_flag_bind_sock, BIND_SOCK, BIND_SOCK_ERROR);
//------------------------------------------------------------------------//
//########################################################################################//

//########################################################################################//
    // Mark the socket for a listening in

//------------------------------------------------------------------------//
    // Errors check
    var.fail_flag_listen_sock = isFailure(listen(serverListeningSock, SOMAXCONN));

    errorExit(var.fail_flag_listen_sock, LISTEN_SOCK, LISTEN_SOCK_ERROR);
//------------------------------------------------------------------------//
//########################################################################################//

//########################################################################################//
    // Accept a call

    sockaddr_in clientConnection;
    socklen_t clientConnection_Size = sizeof(clientConnection);

    int clientSocket = accept(serverListeningSock, (sockaddr*)&clientConnection, &clientConnection_Size);

//------------------------------------------------------------------------//
    // Errors check
    var.fail_flag_client_sock = isFailure(clientSocket);

    errorExit(var.fail_flag_client_sock, CALL_ACCEPT, CALL_ACCEPT_ERROR);
//------------------------------------------------------------------------//
//########################################################################################//

//########################################################################################//
    // Close the listening socket

    close(serverListeningSock);

    // Clear buffers from gurbige
    memset(var.host, 0, NI_MAXHOST);
    memset(var.service, 0, NI_MAXSERV);

    // Display client machine name
    int clientHostMachineName = getnameinfo((sockaddr*)&clientConnection, clientConnection_Size, 
        var.host, NI_MAXHOST, var.service, NI_MAXSERV, 0);

    if (clientHostMachineName)
    {
        std::cout << var.host << " connected on " << var.service << "\n";
    }
    else
    {
        inet_ntop(AF_INET, &clientConnection.sin_addr, var.host, NI_MAXHOST);

        std::cout << var.host << " connected on " << ntohs(clientConnection.sin_port) << "\n";
    }
//########################################################################################//

//########################################################################################//
    // While recieving display message, echo message

    while (true)
    {
        // Clear messageBuffer
        memset(var.messageBuffer, 0, 4096);

        // Wait for a message
        var.bytesRecieve = recv(clientSocket, var.messageBuffer, 4096, 0);

//------------------------------------------------------------------------//
    // Errors check

    var.fail_flag_server_sock = isFailure(var.bytesRecieve);

    errorExit(var.fail_flag_received_data, RECEIVE_DATA, RECEIVE_DATA_ERROR);

    if (var.bytesRecieve == 0)
    {
        std::cout << "Client connection terminated\n";

        break;
    }
//------------------------------------------------------------------------//
//########################################################################################//

//########################################################################################//
    // Display message
    std::cout << "Received: " << std::string(var.messageBuffer, 0, var.bytesRecieve) << "\n";
//########################################################################################//

//########################################################################################//
    // Resend Message
    send(clientSocket, var.messageBuffer, var.bytesRecieve + 1, 0);
//########################################################################################//
    }
//########################################################################################//
    // Close socket
    close(clientSocket);        
//########################################################################################//
}