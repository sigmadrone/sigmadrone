#include <iostream>
#include "application.h"

int main(int argc, char *argv[])
{
	application app;

	app.init(argc, argv);
	return app.run();
}
