#include <windows.h>
#include <memoryapi.h>

#include "Application.h"

int main(int argc, char* argv[]) {
	const unsigned int MINSIZE_KB = 1;
	const unsigned int MAXSIZE_KB = 4;
	//Performance issues in release mode caused by insane pagefaults (from 10k to 600k)

	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

	//SetProcessWorkingSetSize(GetCurrentProcess(),
	//	MINSIZE_KB * 1024,
	//	MAXSIZE_KB * 1024);


	Application app = Application("#version 330", 960, 512);

	int err = app.Init("Window");

	if (err == 0) {
		err = app.Run();
	}

	return err;
}