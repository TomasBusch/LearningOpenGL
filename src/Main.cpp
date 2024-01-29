#include "Application.h"

int main(int argc, char* argv[]) {
	
	Application app = Application("#version 330", 960, 512);

	int err = app.Init("Window");

	if (err == 0) {
		err = app.Run();
	}

	return err;
}