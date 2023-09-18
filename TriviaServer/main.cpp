#include "TriviaServer.h"


int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // check for memory leaks
#endif

	try
	{
		TriviaServer(9568).run();
	}
	catch (const Exception& e)
	{
		cout << "Error: " << e.what() << endl;
	}

	return 0;
}
