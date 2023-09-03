#include "TriviaServer.h"
#include "WSAInitializer.h"


int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // check for memory leaks
#endif

	try
	{
		WSAInitializer WSAInitializer;
		TriviaServer server(9568);
		server.run();
	}
	catch (const Exception& e)
	{
		cout << "Error: " << e.what() << endl;
	}

	return 0;
}
