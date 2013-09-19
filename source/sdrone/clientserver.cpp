/*
 * clientserver.cpp
 *
 *  Created on: Jun 17, 2013
 *      Author: svassilev
 */

#include "clientserver.h"

using namespace std;

#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>

ClientServer::ClientServer()
{
	m_ServerThread = 0;
	m_CommandCallback = 0;
	m_IsReceivingCommands = false;
	m_ServerPort = 0;
	m_CallbackContext = 0;
}

ClientServer::~ClientServer()
{
	JoinServerThread();
}

void ClientServer::JoinServerThread()
{
	if (0 != m_ServerThread) {
		assert(pthread_self() != m_ServerThread);
		pthread_join(m_ServerThread,0);
		m_ServerThread = 0;
	}
}

int ClientServer::StartServerThread(
		ExecuteCommandCallbackT callback,
		void* context,
		int port)
{
	int err;
	JoinServerThread();
	m_CommandCallback = callback;
	m_CallbackContext = context;
	m_ServerPort = port;
	m_IsReceivingCommands = true;
	err = pthread_create(&m_ServerThread,0,ServerThread,this);
	if (err < 0) {
		StopServerThread();
	}
	return err;
}

char** ClientServer::DeserializeCommand(
		char* buffer,
		int* _argc)
{
	char** argv = 0;
	int argc = 0;
	char* ptr = buffer;
	char* argBegin = buffer;

	assert(0 != buffer);
	assert(0 != _argc);
	*_argc = 0;

	while (*ptr != 0 && *ptr != '\n') {
		if (' ' == *ptr) {
			if (*argBegin != ' ') {
				++argc;
			}
			argBegin = ptr+1;
		}
		ptr++;
	}

	if (' ' != *argBegin && '\0' != *argBegin && '\n' != *argBegin) {
		++argc;
	}

	if (0 == argc || 0 == (argv = new char*[argc])) {
		goto __return;
	}

	*_argc = argc;

	ptr = argBegin = buffer;
	argc = 0;
	while (*ptr != 0 && *ptr != '\n') {
		if (' ' == *ptr) {
			if (*argBegin != ' ') {
				argv[argc++] = argBegin;
				*ptr = 0;
			}
			argBegin = ptr + 1;
		}
		ptr++;
	}

	*ptr = 0;

	if (' ' != *argBegin && '\0' != *argBegin && '\n' != *argBegin) {
		argv[argc++] = argBegin;
	}

	assert(*_argc == argc);

	__return:
	return argv;
}

void* ClientServer::ServerThread(void* arg)
{
	assert(arg);
	ClientServer* _this = (ClientServer*)arg;
	int listenSocket = -1;
	int err = 0;
	sockaddr_in localAddress = {0};
	if (-1 == (listenSocket = socket(AF_INET,SOCK_STREAM,0)))
	{
		err = errno;
		printf("Failed to create listening socket, err=%d\n",errno);
		goto __return;
	}
	// Start listening on a port
	localAddress.sin_family = AF_INET;
	localAddress.sin_port = ntohs(_this->m_ServerPort);
	err = bind(
			listenSocket,
			(const sockaddr*)&localAddress,
			sizeof(localAddress));
	if (-1 == err) {
		printf("Failed to bind to local address,err=%d\n",errno);
		goto __return;
	}
	if (-1 == (err = listen(listenSocket,10))) {
		printf("Failed to listen on port %d, err=%d\n",
				ntohs(localAddress.sin_port),errno);
		goto __return;
	}

	//
	// Start the main loop that will accept connections.
	// Since we do not expect too much traffic the sockets
	// will be opened and closed on every command we receive
	//
	while (_this->IsServerThreadRunning() && err >= 0)
	{
		static const size_t bufSize = 1024;
		sockaddr_in remoteGuy;
		unsigned int addrSize;
		char buffer[bufSize];
		FILE* client = 0;
		int acceptedSocket = accept(listenSocket,(sockaddr*)&remoteGuy,&addrSize);
		if (-1 == acceptedSocket) {
			printf("Failed to accept connection,err=%d\n",errno);
			continue;
		}

		printf("Accepted connection from %s\n",inet_ntoa(remoteGuy.sin_addr));

		if (0 == (client = fdopen(acceptedSocket, "r"))) {
			printf("Failed to open file on accepted socket, err=%d\n",err);
			close(acceptedSocket);
			break;
		}

		//
		// Wait for 1 second to receive the command
		//
		fd_set rfds;
		timeval tv = {1,0};
		FD_ZERO(&rfds);
		FD_SET(acceptedSocket,&rfds);
		while (select(acceptedSocket+1,&rfds,0,0,&tv) > 0)
		{
			buffer[ARRAYSIZE(buffer)-1] = 0;
			if (fgets(buffer,ARRAYSIZE(buffer)-1,client)) {
				int argc = 0;
				char** argv = DeserializeCommand(buffer,&argc);
				if (0 != argv) {
					assert(argc>0);
					if (0 == strcmp(argv[0],DRONZONE_PROTOCOL)) {
						SdCommand command = SD_COMMAND_NONE;
						err = _this->m_CommandCallback(
								_this->m_CallbackContext,
								argc,
								argv,
								&command);
						if (SD_COMMAND_EXIT == command) {
							_this->StopServerThread();
						}
					} else {
						printf("Received invalid command protocol header %s\n",
								argv[0]);
					}
					delete [] argv;
					if (!_this->IsServerThreadRunning() || err < 0) {
						break;
					}
				}
				FD_ZERO(&rfds);
				FD_SET(acceptedSocket,&rfds);
			} else {
				break;
			}
		}

		close(acceptedSocket);
		fclose(client);
	}

	__return:
	if (-1 != listenSocket) {
		close(listenSocket);
	}
	return *(void**)&err;
}

int ClientServer::SendCommandToDrone(
		const char* droneAddress,
		int dronePort,
		const string& cmdLine)
{
	int err = -1;
	struct hostent *host = 0;
	struct sockaddr_in addr = {0};
	int fd = -1;
	FILE* server = 0;
	size_t sizeWritten = 0;

	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(droneAddress);
	if (INADDR_NONE == addr.sin_addr.s_addr) {
		if ((host = gethostbyname(droneAddress)) == NULL) {
			// IPv4 address was not specified, try to resolve the name
			printf("SendCommandToDrone: Invalid drone address specified %s\n",
					droneAddress);
			goto __return;

		} else {
			addr.sin_addr = *((struct in_addr *)host->h_addr);
		}
	}
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("SendCommandToDrone: Failed to create client socket, err=%d\n",
				errno);
		goto __return;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(dronePort);
	if (connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1) {
		printf("SendCommandToDrone: Failed to connect to %s\n",droneAddress);
		goto __return;;
	}
	if (0 == (server = fdopen(fd, "w"))) {
		printf("Failed to open file on socket, err=%d\n",err);
		goto __return;
	}

	sizeWritten = fwrite(cmdLine.c_str(),1,cmdLine.size(),server);
	if (cmdLine.size() != sizeWritten) {
		printf("SendCommandToDrone failure: sent only %d bytes out of %d\n",
				(int)sizeWritten, (int)cmdLine.size());
		err = ECOMM;
	}

	err = 0;

	__return:
	if (0 != server) {
		fclose(server);
	} else if (fd > 0) {
		close(fd);
	}
	return err;
}

void ClientServer::SerializeCommand(
	string* cmdLine,
	int argc,
	char** argv)
{
	cmdLine->clear();
	cmdLine->append(DRONZONE_PROTOCOL);
	cmdLine->append(" ");
	for (int i = 0; i < argc; i++) {
		cmdLine->append(argv[i]);
		cmdLine->append(" ");
	}
	cmdLine->append("\n");
}

void ClientServer::StopServerThread()
{
	m_IsReceivingCommands = false;
}

bool ClientServer::IsServerThreadRunning()
{
	return m_IsReceivingCommands;
}
