#include "ByteChatServer.h"


int main(void) {

	ByteChatServer server;
	server.Initialize();
	server.Run();

	return 0;
}
