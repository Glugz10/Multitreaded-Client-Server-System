/*
 * Author: Gustavo Lugo
 * 08-25-2026
 */

#include <iostream>
#include <string>
#include <thread>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std:

const string SERVER_IP = "127.0.0.1";
const int PORT = 54000;
const int BUFFER_SIZE = 1024;

// Continuously receive messages from server
void receiveMessages(int socket)
{
    char buffer[BUFFER_SIZE];

    while (true)
    {
        int bytesReceived = recv(
            socket,
            buffer,
            BUFFER_SIZE,
            0
        );


        if (bytesReceived <= 0)
        {
            cout
                << "\nDisconnected from server."
                << endl;

            break;
        }


        cout.write(
            buffer,
            bytesReceived
        );

        cout.flush();
    }
}


int main()
{
    // Create TCP socket
    int clientSocket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );


    if (clientSocket == -1)
    {
        cerr
            << "Could not create socket."
            << endl;

        return 1;
    }


    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;

    serverAddress.sin_port =
        htons(PORT);


    // Convert IP address from text to binary
    if (inet_pton(
            AF_INET,
            SERVER_IP.c_str(),
            &serverAddress.sin_addr
        ) <= 0)
    {
        cerr
            << "Invalid server address."
            << endl;

        close(clientSocket);

        return 1;
    }


    // Connect to server
    if (connect(
            clientSocket,
            (sockaddr*)&serverAddress,
            sizeof(serverAddress)
        ) == -1)
    {
        cerr
            << "Could not connect to server."
            << endl;

        close(clientSocket);

        return 1;
    }


    cout
        << "Connected to server."
        << endl;


    // Start thread that listens for server messages
    thread receiver(
        receiveMessages,
        clientSocket
    );


    string message;


    // Main thread handles user input
    while (getline(cin, message))
    {
        message += "\n";


        send(
            clientSocket,
            message.c_str(),
            message.size(),
            0
        );


        if (message == "/quit\n")
        {
            break;
        }
    }


    // Wake up receiver thread if it is blocked in recv()
    shutdown(
        clientSocket,
        SHUT_RDWR
    );


    // Wait for receiver thread to finish
    receiver.join();


    // Release socket
    close(clientSocket);


    return 0;
}
