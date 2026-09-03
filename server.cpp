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

//Removing a client from the connected list
void removeClient(int clientSocket)
{
	lock_guard<mutex> lock(clientsMutex);
	
	clients.erase(
		remove_if(
			clients.begin(),
			clients.end(),
			[clientSocket](const Client& client)
			{
				return client.socket == clientSocket;
			});
		clients.end()
		);
}

//Return a list of connected users
string getUserList()
{
	lock_guard<mutex> lock(clientsMutex);
	
	string result = "\nConnected users:\n";
	
	for(const Client& client : clients)
	{
		result += "- " + client.username + "\n";
	}
	return result;
}

//Handling communication with a client
void handleClient(int clientSocket)
{
	char buffer[BUFFER_SIZE];
	
	//prompt client for username
	string request = "Enter your username: ";
	sendMessage(clientSocket, request);
	int bytesReceived = recv(
		clientSocket, 
		buffer,
		BUFFER_SIZE,
		0
	);
	
	if(bytesReceived <= 0)
	{
		close(clientSockets);
		return;	
	}
	
	string username(buffer, bytesReceived);
	
	//Remove trailing newLine characters
	while(!username.empty() && (username.back() == '\n' || username.back() == '\r'))
	{
		username.pop_back();
	}
	
	//Add client to shared client list
	{
		lock_guard<mutex> lock(clientsMutex);
		
		Client newClient;
		
		newClient.socket = clientSocket;
		newClient.username = username;
		
		clients.push_back(newClients);
	}
	
	cout << username << " connected." << endl;
	
	string joinMessage = "\n[SERVER]" + username + " joined the chat. \n";
	
	broadcastingMessage(joinMessage, clientSocket);
	
	
	string welcomeMessage = "\nWelcome, " + username + "!\n" 
		"Commands:\n" 
		"/users - show connected users\n"
		"/quit - disconnected\n\n";
		
	sendMessage(clientSocket, welcomeMessage);
	
	//Receive messages from this client
	while(true)
	{
		bytesReceived = recv(
			clientSocket, 
			buffer, 
			BUFFER_SIZE, 
			0
		);
		
		if(bytesReceived <= 0)
		{
			break;
		}
		
		string message( buffer, bytesReceived);
		 
	//Removing trail newLine Characters
	while(!message.empty() && (message.back() =='\n' || message.back() == '\r'))
	{
		message.pop_back();
	}
	
	//Quit command
	if (message == "/quit")
	{
		break;
	}
	
	 //User command
	 if(message == "/users")
	 {
		 string userList = getUserList();
		 sendMessage(clientSocket, userList);
		 
		 continue;
	 }
	 
	 string formattedMessage = username + ": " + message + "\n";
	 
	 cout << formattedMessage;
	 broadcastMessage(formattedMessage, clientSocket);
 
	//Remove client from shared client list
	removeClient(clientSocket);
 
	//Release socket
	close(clientSocket);
	
	cout << username << " disconnected" << endl;
	
	string leaveMessage = "\n[SERVER} " + username + " left the chat.\n"
	broadcastMessage(leaveMessage);
}


	
	
	
	
	
	
