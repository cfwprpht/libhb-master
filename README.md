=================
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
 Oh and not to forget The DarkProgrammer for the 4.55 pkg base, testing and pointing me onto bugs.
 SpecterDev for his syscall 11 implementation. Various devs i don't know the name now and which where included into the unjail and system whide patches.
 IDC and Vortex for various code blocks here and there as example "Vortexs GetLocalIP()" for the PS4. IDC as example for patches and the FTP source from xerpi.
 DarkElement for pointing me onto stuff while i am into work, like to pointing me onto the hdd key offset so i could add them for other fws too.
 zecoxao for the eap_part_key offset for 4.55. (?)

## Library Contributors:
 Vultra, Erickson, DarkElement, XDPX, SimplePerson,
 
## Downloads:
 LibHB-Sample-PKGs: http://www.mediafire.com/file/kyb27dfubtidqbu/libhb-sample-pkgs.rar


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
 
## Update 11:
 Fixed missalign of the shader for the Banner.
 Added Abbility to stop a video when running by pressing button X.
 Added Functions drawRect() and fillRect() to the applications class. You can now draw from outside the Application code.
 Wrote a 3 lines Button sample.
 Added Functions AddDraw() and RemoveDraw() to the App class.
 Added compilled samples for FW 4.55.
 Added a LineBreak() function to the static console class.
 
## Update 12:
 Fixed Console::LineBreak().
 Added a sample "SaveDataMasterKeyDumper" on how to use libHB with your own kernel function.
 Moved the pkgs and the samples into the libhb-master.rar archive. Can be downloaded on the releases button.
 Will look into the FileSystem Dumper and fix it as next.
 
## Update 13:
 Added nmount, fstatat, and readlink System calls to the Sys Class. (credits @IDC)
 Fixed lseek Systemcall. (Wrong number, credits @IDC)
 Added GetLocalIP() Function to SwissKnife class. (credits @Vortex)
 Added Shader Position for the Banner to the application class.
 Added GetTimeString() to SwissKnife.
 Added sceNetCtl Library to LibHomebrew.
 Changed Logger Initialisation to create the file if not existend.
 Included Headers: ps4_network.h, ps4_file.h, logger.h and libnetCtl.h into the Application Class header.
 Kernel Console is now working.
 Added a sample on how to use kernel console.
 Added critical_enter() and critical_exit() to liblv2.
 Added a Input Writer for the Kernel Console. You can either wait for a specific input 'kconsole.waitForUsrInput("TRIANGLE")'
  or you can overload a buffer pointer and compare the input your self. ['kconsole.waitForUserInput(*buffer)']
  Or just wait for any input without comparing.
 Added a function to liblv2 'lv2.csleep()' to wait a few milli/micro seconds before we write some new string to the buffer. Giving the App loop some time to do his job.
 Added 'lv2.toHexString(unsigned char *data, int len)' and 'lv2.hexify(char *string)'.
 Added over 400 lv2 calls to the library.
 Added 5.0X unjail patches. (credits @Vortex)
 Added Support for 5.0X exploited FW.
 Ported FTP App (Simple for now, no SELF Decryption) and made it match for libHB. It's running but have some problems with getting 'LIST'.
  I'll put it into the release so others also can have a look while i am bussy.
  It use the usb drive for logging. Create a Folder called 'PS4FTP' into the root of your usb drive. If you want to enable debug logging,
  create a empty text file within 'PS4FTP' directory and rename it to 'usedebug.txt'.
  Attemption: The drive need to be connected before you run the app. Since the check for logging is within the preparation process.
 Updated MasterKeyDumper to use Kernel Console.
 Added pkg helpers 'finalize_pkg.exe' and 'make_pkg.bat'. For your project, open the .bat in a editor and change the pkg and project name to yours.
  Else when your done with preparing your pkg, sfo, gp4, and have builded your elf, just right click the finalize_pkg.exe and run as administrator, wait a bit, press space then enter. Done.
 Added the custom sleep of lv2 to our WriteLine for the kernel. Giving the Application loop time to write out our message.
 Added SAMU lv2 calls.
 Added SceSblSrtcSetTime lv2 call to the library.
 Added HDD Key Offset for all 3 FWs. Dump your HDD Key !
 Added Patches for Disable Prcoess ASLR, Enable Browser Permanent and SUDO (DevKit ID patch and Allow mapping of SELF).
 Added SaveDataMasterKey Dumper v2. Since FW 5.05, getSealedKeySecret() changed and hase now flags to use (1-4).
 Splitted Sample PKGs into a own upload.
 
## Update 14:
 Fixed lv2 offsets for FW 5.05.
 All Samples are back working now.