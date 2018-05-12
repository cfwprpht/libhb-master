
#include <application.h>

Application app;

void *MyEntry(void *) {
	Console::WriteLine("Hello Ps4 Homebrewing World !\n");

	while (app.Input() != ssi::kButtonCross) { /* Nothing to do here */ }

	Console::WriteLine("Exiting...");

	app.Close();

	return 0;
}

int main(void) {
	app.MultiLine(true);
	app.Add((void *)MyEntry);
	return app.exec();
}
