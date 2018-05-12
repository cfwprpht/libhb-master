PS4 LibHomebrew
=================

## What's that ?
 A Homebrew Library based on the Official SDK.
 Auto Jailbreaks. No need to do anything here.
 If you want to change the FW version to compile for (needed for jb & kernel aka lv2 stuff),
 then set your consoles FW within fw.h like: FW_455 or FW_500 as example.
 Copy libhb Folder (not the -master one) and his samples into "/target/samples/sample_code/system" of your SDK.
 Move the "controller.h" from the libhb-master folder to: "/target/samples/sample_code/common/include/sampleUtil/input"
 That's it your Set Up !
 
## Thanks and used sources:
 First of all for sure Sony and the PS4. Without, this wouldn't exists. Also thanks for giving me the chance to learn more about this planet and his machines
 before they gonna controle us all.
 Then i crapped various sources which where already available like the one from f0f and ps4-kexec. The unjail patches for both 4.05 and 4.55
 where also crapped from the Net. The File System Dumper i ported to use LibHomebrew is the one from wildcard.
 For sure also Credits goes out to flatz, querty and xerpi. For fpkg, fself, webkit exploit, kernel exploit,.....
 Oh and not to forget The DarkProgrammer for testing and pointing me onto bugs.


## Updates:
Basic Image class written. May not functional right now. Need to look into about the texter loading.
Added a global accessable Application instance. This way all code can use the same instance working with overloads is much easyer.
Testing type definations for better readability.

## Update 2:
Added a basic console output stream to write messages to the screen rich text like, for our application code. For that i added a new form to the application main loop.
Which let me come to the new Forms added: PictureBox, RichTextBox and a Image class to handle various images. So the Application got a RichTextBox form added,
where all the messages will be drawen to screen. For that i added a static function called "Console::WriteLine(const char *message)" to write formated 
strings into the global console output string stream "cout". A class called FileInfo to store various informations about a file path.

Added definations:
 std::string as 'String' | usage example: String test("This is a test string"); if (test.find("as a coding example") == -1) Console::WriteLine("Error: 'as a coding example' not found within this string.");
 unsigned char as 'byte'
 vecmath::Vector2 as 'Position' and 'Size' | usage example: Position(x, y); Size(x, y); 
 vecmath::Vector4 as 'Color' usage is the same as above.
 tm as 'Time'
 DIR as 'DIR'
 vector of class FileInfo (std::vector<FileInfo *>) as 'FileInfoList'
 
## Update 3:
  Need to add
  
## Update 4:
 Added AV Player.
 Added Resource Manager.
 Added Fios2 and Sound Flag to the Application initialisation process.
 Added Lua Script Support.
 Added psarc archive supprt.
 Added unjail.c source.
 Added Process Class.
 
## Update 5:
 Added libLV2.
 Added FW selector within fw.h. (set your FW to use there and libHB will use this setting)
 Automaticly Jailbreak on App initializing.
 Broke Libc and Libkernel. Need to fix.
 
## Update 6:
 Fixed LibLV2.
 
## Update 7:
 Fixed Clear screen finally.
 Fixed Usb Resolving to use a totaly unusual test name.
 
## Update 8:
 Fixed File Reading.
 
## Update 9:
 Added ReadLine(), ReadAllLines(), WriteLine()
 Fixed Logger.
 
## Update 10:
 Finished Application class.