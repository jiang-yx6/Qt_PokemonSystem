#include<iostream>
#include<WinSock2.h>
#include<fstream>
#include<nlohmann/json.hpp>
#include "server.h"
using namespace std;
using json = nlohmann::json;
char RecvBuf[BUFSIZ] = { 0 };
char SendBuf[BUFSIZ] = { 0 };
void Server::ServerRegister(json j ,SOCKET ClientSocket)
{
    string fileName;
    string username;
    string password;
    username = j.at("username").get<std::string>();
    fileName = username + ".txt";

    if (j.contains("password"))
        password = j.at("password").get<std::string>();

    string info;
    std::ifstream checkFile(fileName);

    //检查是否用户已存在
    if (checkFile.is_open())
    {
        info = "error";
        checkFile.close();
    }
    else if (!password.empty())
    {
        info = "right and pwd good";
        std::ofstream inputFile(fileName, std::ios::app);
        json input;
        input["username"] = username;
        input["password"] = password;
        inputFile << input.dump() << endl;
        inputFile.close();
        std::cout << "create successfully! your info is already reserved in file: " << fileName << std::endl;
    }
    else
        info = "right but pwd empty";
    memset(SendBuf, 0, BUFSIZ * sizeof(char));
    info.copy(SendBuf, info.size());
    cout << "send>>" << SendBuf << endl;
    send(ClientSocket, SendBuf, BUFSIZ, 0);
}
// void Server::ServerLogin(json j, SOCKET ClientSocket)
// {
//     string fileName;
//     string username;
//     string password;
//     username = j.at("username").get<std::string>();
//     // 使用name
//     std::cout << " Name: " << username << std::endl;

//     password = j.at("password").get<std::string>();
//     // 使用pwd
//     std::cout << " Pwd: " << password << std::endl;

//     fileName = username + ".txt";
//     string info;
//     std::ifstream checkFile(fileName);
//     //检查是否用户已存在
//     if (checkFile.is_open())
//     {
//         info = "right";
//         string content;
//         json output;
//         if (std::getline(checkFile, content))
//         {
//             output = json::parse(content);
//         }
//         string out = output["password"].dump();
//         if (output["password"].dump() != ("\"" + password + "\""))
//             info = "error";
//         else
//         {
//             this->password = password;
//             this->username = username;
//         }
//     }
//     else
//         info = "error";
//     checkFile.close();

//     memset(SendBuf, 0, BUFSIZ * sizeof(char));
//     info.copy(SendBuf, info.size());
//     cout << "send>>" << SendBuf << endl;
//     int send_len = send(ClientSocket, SendBuf, BUFSIZ, 0);
//     if (send_len < 0) {
//         cout << "failed to send" << endl;
//     }

// }
// void Server::ServerAddPokemon(json j, SOCKET ClientSocket)
// {
//     string username;
//     string fileName;
//     username = j.at("username").get<std::string>();
//     fileName = username + ".txt";
//     ofstream inputFile(fileName, std::ios::app);
//     string info;
//     if (!inputFile.is_open()) {
//         cerr << "cannot create user file" << endl;
//     }
//     else
//     {
//         info = "got three pokemon";
//         json jPokemon;
//         jPokemon["Pokemon"] = j["Pokemon"];
//         inputFile << jPokemon.dump() << endl;
//         inputFile.close();
//         cout << "system has given you three pokemon , start it right now! " << fileName << endl;
//     }
//     memset(SendBuf, 0, BUFSIZ * sizeof(char));
//     info.copy(SendBuf, info.size());
//     cout << "send>>" << SendBuf << endl;
//     send(ClientSocket, SendBuf, BUFSIZ, 0);
// }
// void Server::ServerCheckInfo(json j, SOCKET ClientSocket)
// {
//     string fileName;
//     fileName = username + ".txt";
//     std::ifstream inputFile(fileName);
//     std::string line;
//     while (std::getline(inputFile, line)) { // 从文件读取
//         json jLine = json::parse(line);
//         if (jLine.contains("Pokemon"))
//         {
//             string info = jLine["Pokemon"].dump();
//             memset(SendBuf, 0, BUFSIZ * sizeof(char));
//             info.copy(SendBuf, info.size());
//             send(ClientSocket, SendBuf, BUFSIZ, 0);
//         }
//     }
//     string info = "finish";
//     memset(SendBuf, 0, BUFSIZ * sizeof(char));
//     info.copy(SendBuf, info.size());
//     send(ClientSocket, SendBuf, BUFSIZ, 0);
//     inputFile.close();
// }
