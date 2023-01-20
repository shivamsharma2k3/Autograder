//
//  server.cpp
//  CCN final
//
//  Created by Shivam Sharma on 20/01/23.
//

#include "filereader.hpp"
#include "server.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

string process_file(std::string file_data) {
    string file_name = "submit";
    string file_extension = ".cpp";
    string::size_type pos = file_name.find_last_of(".");
    if (pos != std::string::npos) {
        file_extension = file_name.substr(pos);
        file_name = file_name.substr(0, pos);
    }
    ofstream new_file(file_name + file_extension, std::ios::binary);
    new_file.write(file_data.c_str(), file_data.size());
    new_file.close();
    return file_data;
}

string exec(string command) {
    char buffer[128];
    string result = "";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }

    while (!feof(pipe)) {

        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    return result;
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9999);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char buffer[1024] = {0};
    valread = read(new_socket, buffer, 1024);
    string file_data(buffer, valread);

    string output_string = process_file(file_data);
    string fileName="submit.cpp";
    string c1 = exec("g++ "+fileName+" -o sub");
    string c2 = exec("./sub");
    string c3;
    c3 = exec("cmp output.txt expectedoutput.txt");
    cout << c1;
    cout << c2;
    cout << c3;
    string res;
    string o = read("output.txt");
    string e = read("expectedoutput.txt");
    if(o.compare(e) == 0){
        res = "The output matches with the expected output.";
    }
    else {
        res = "The output does not match with the expected output.";
    }
    
    send(new_socket, res.c_str(), res.size(), 0);
    close(new_socket);

    return 0;
}
