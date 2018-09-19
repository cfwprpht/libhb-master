
#include "application.h"
//#include "console.h"
#pragma comment( lib , "application_stub_weak.a")

// The Entry for the ZLibTest.
void *UserEntry(void *) {
	// Add Your Code here.
	...
	
	return 0;
}

// The Program Entry.
int Program(void) {
	// Application Instance.
	Application app;

	// Enable Multi Line.
	app.MultiLine(true);

	// Enable the clock.
	app.UseTime(true);

	// Enable screen shot.
	app.UseScreenShot(true);

	// Enable Banner.
	String myBanner = "";
	app.UseBanner(true);
	app.Banner(myBanner.c_str());
	app.BannerPos(0.45f, 0.01f);
	app.ShaderPos(0.452f, 0.01f);

	// Enable resources to use.
	//app.UseSound(true);
	//app.UseResources(true);

	// Determine to use a Message Dialog for this Application loop.
	//app.UseDialog();

	// Set Title Id of this Program.
	app.SetTitleId("Add_Your_App_TitleID_Here");

	// Add our self.
	app.AddCode((void *)UserEntry);

	// Run Application code now.
	return app.exec(&app);
}