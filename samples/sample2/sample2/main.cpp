
#include <application.h>
#include <ps4_file.h>

using namespace LibHomebrew::PS4IO;

Application app;

// Simple IO Sample.
void IoSample(void) {
	Console::WriteLine("Reading /mnt/sandbox/pfsmnt/HXHB00103-app0/media/multi_test.txt...");
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));	
	FILE *fp = fopen("/mnt/sandbox/pfsmnt/HXHB00103-app0/media/multi_test.txt", "r");
	if (fp) {
		char *ptr = buffer;
		int count, total;
		count = total = sizeof(buffer);
		while ((ptr = fgets(ptr, count, fp)) != nullptr) {
			count = count - strlen(ptr);
			ptr = &buffer[total - count];
		}
		if (strlen(buffer) > 0) {
			Console::WriteLine("Ok !\n");
			Console::WriteWarning("%s\n", buffer);
		} else Console::WriteError("Couldn't read from file.\n");
	} else Console::WriteError("Couldn't open file.\n");
	fclose(fp);
}

// Write out a tiny nice usage.
void PrintUsage(void) {
	Console::WriteLine("Press square for IO sample\n");
	Console::WriteLine("Press triangle for screen shot. (saves to usb)\n");
	Console::WriteLine("Press R1 for Video sample\n");
	Console::WriteLine("Press circle for ELF loading sample\n");
	Console::WriteLine("Press L1 to print UID\n");
	Console::WriteLine("Press R3 to clear screen and L3 to show message.\n");
	Console::WriteWarning("\nChoose your weapons: ");
}

// Main entry for this sample.
void *MyEntry(void *) {
	PrintUsage();

	while (true) {
		if (app.Input() == ssi::kButtonSquare) {
			// Sometimes our own loop is kind of 100 times faster then the Application loop.
			// For that we may want to clean the input after reading it.
			// Else it could happen that we run this sub loop here for around 100 times and more.
			app.ClearInput();
			Console::WriteLine("...runing input / output sample.\n");
			IoSample();
		} else if (app.Input() == ssi::kButtonCircle) {
			app.ClearInput();
			Console::WriteLine("...Loading ELF.\n");
			sceKernelSleep(1);
			Proc::LoadExec("/app0/testme.elf", NULL);
		} else if (app.Input() == ssi::kButtonL1) {
			app.ClearInput();
			Console::WriteLine("...are we root ?\n");
			int UID = Sys::getuid();
			if (UID == 0) Console::WriteWarning("Yes we are.\nUID: %d\n", UID);
			else Console::WriteError("No we're not.\nUID: %d\n", UID);
		} else if (app.Input() == ssi::kButtonL3) {
			app.ClearInput();
			PrintUsage();
		}
	}

	return 0;
}

// Main enry.
int main(void) {
	// Set to write in blocks.
	app.MultiLine(true);
	
	// Set the Title.
	app.Title("LibHB Sample 2");
	app.UseTitle(true);

	// Show time.
	app.UseTime(true);

	// Set to use the videoplayer.
	app.UseVideo(true);
	app.Video("/mnt/sandbox/pfsmnt/HXHB00103-app0/media/switch_parodie.mp4");

	// Enable Screen Shot.
	app.UseScreenShot(true);

	// Add our self to the application.
	app.Add((void *)MyEntry);

	// Run that settings now.
	return app.exec();
}