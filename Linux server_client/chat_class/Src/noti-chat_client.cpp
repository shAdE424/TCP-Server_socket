#include "noti-chat_client.hpp"

// Error program exit method
void NotiChat_Client::errorExit(bool fatal_error, std::string fail_origin, int error_code)
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

//
void NotiChat_Client::connectionLog(bool fatal_error, std::string fail_origin, int error_code)
{
    std::cout << "Error occured with " << fail_origin << "!\n";
}

// Checking if failure is occured 
bool isFailure(const int& atribute)
{
    return (atribute == -1) ? true : false;
}