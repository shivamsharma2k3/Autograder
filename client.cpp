//
//  client.cpp
//  CCN final
//
//  Created by Shivam Sharma on 20/01/23.
//

#include "client.hpp"
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

using namespace std;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    ifstream file("code.cpp", std::ios::binary);
    ostringstream ss;
    ss << file.rdbuf();
    string file_data = ss.str();

    send(sock, file_data.c_str(), file_data.size(), 0);
    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    string output_string(buffer, valread);
    cout << output_string << std::endl;
    close(sock);
    return 0;
}
