
#include <application.h>

// Our Application.
Application app;

// Set a size.
Size _size(0.1, 0.06);

// Character size for the button text.
static float CHAR_XS = 0.03;

// Declare centered Position.
Position center;

// Draw a simple button, without any action.
void DrawButton(void) {
	// Draw a block of something to the screen.
	app.fillRect(center, _size, DARK_GRAY);
	
	// Now draw a border around it.
	app.drawRect(center, _size, ORANGE);
	
	// At last, we draw a text into.
	app.drawStringf(center.getX() + 0.016, center.getY() + 0.015, CHAR_XS, "Click Me");
}

// Our Entry.
void *MyEntry(void *) {
	// Tell the user what he need to do.
	Console::WriteLine("Press X to draw Form.\n");

	// Now we get the centered Position of our form.
	center = app.CenterForm(_size.getX(), _size.getY());

	// Wait for input.
	while (true) {
		if (app.Input() == ssi::kButtonCross) {
			app.AddDraw(DrawButton);
			TTY::clear();
			Console::WriteLine("Press O to hide Form.\n");
			app.ClearInput();
		} else if (app.Input() == ssi::kButtonCircle) {
			app.RemoveDraw(DrawButton);
			TTY::clear();
			Console::WriteLine("Press X to draw Form.\n");
			app.ClearInput();
		} else if (app.Input() == ssi::kButtonOptions) {
			TTY::clear();
			app.Close();
			break;
		}
	}
	return 0;
}

// Main entry.
int main(void) {
	// Set to write in blocks.
	app.MultiLine(true);

	// Show a time.
	app.UseTime(true);

	// Add our self.
	app.Add((void *)MyEntry);
	
	// Run the App.
	return app.exec();
}