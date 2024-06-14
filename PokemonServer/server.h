#ifndef SERVER_H
#define SERVER_H

#endif // SERVER_H
#include<iostream>
#include<WinSock2.h>
#include<fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;
/*char RecvBuf[BUFSIZ] = { 0 };
 char SendBuf[BUFSIZ] = { 0 };*/

class Server {
private:
    string username;
    string password;
public:

    void ServerRegister(json j ,SOCKET ClientSocket);
    void ServerLogin(json j, SOCKET ClientSocket);

    void ServerAddPokemon(json j, SOCKET ClientSocket);

    void ServerCheckInfo(json j, SOCKET ClientSocket);
};
