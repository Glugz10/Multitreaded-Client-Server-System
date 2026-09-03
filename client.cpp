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

