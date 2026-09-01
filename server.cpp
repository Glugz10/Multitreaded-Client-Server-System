 /*
 * Author: Gustavo Lugo
 * 08-25-2026
 */

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

const int PORT = 54000;
const int BUFFER_SIZE = 1024;

struct Client 
{
	int socket;
	string username;
};

vector<Client> clients;
mutex clientsMutex;

//sending a message to a client
void sendMessage(int clientSocket, const string& message)
{
	send(
		clientSocket,
		message.c_str(),
		message.size(),
		0
	);
	
//sending a message to every connected client
void broadcastMessage(const string& message, int senderSocket = -1)
{
	vector<int> clientSockets;
	
	{
		lock_guard<mutex> lock(clientsMutex);
		for (const Client& client : clients)
		{
			if(client.socket != senderSocket)
			{
				clientSockets.push_back(client.socket);
			}
		}
	}
	
	for(int socket : clientSockets)
	{
		sendMessage(socet, message);
	}
}
