             =================
              PS4 LibHomebrew
             =================

![image1](https://user-images.githubusercontent.com/1683181/45920389-9d220800-bea3-11e8-9ea6-56d9412584b4.png)
![image2](https://user-images.githubusercontent.com/1683181/45920409-de1a1c80-bea3-11e8-84b1-957bd7ce8f54.png)

## What's that ?
     A Homebrew Library based on the Official SDK.
     Auto Jailbreaks. No need to do anything here.
     If you want to change the FW version to compile for (needed for jb & kernel aka lv2 stuff),
     then set your consoles FW within fw.h like: FW_455 or FW_500 as example.
     Copy libhb Folder (not the -master one) and his samples into "/target/samples/sample_code/system" of your SDK.
     Extract the libs and dlls from the download of github page (under the tab 'releases') into 
     "/target/samples/sample_code/system/libhb/ORBIS_Debug" or compile the library on your machine.
     Move the "controller.h" from the libhb-master folder to: "/target/samples/sample_code/common/include/sampleUtil/input"
     Use the Visual Studio Integration installer to add two Template Projects for libhb to the official PS4 SDK entry,
     under the Visual C++ Category.
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


###Updates:
      Basic Image class written. May not functional right now. Need to look into about the texter loading.
      Added a global accessable Application instance. This way all code can use the same instance working with overloads is much easyer.
      Testing type definations for better readability.

###Update 2:
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
 
###Update 3:
     Need to add
  
###Update 4:
     Added AV Player.
     Added Resource Manager.
     Added Fios2 and Sound Flag to the Application initialisation process.
     Added Lua Script Support.
     Added psarc archive supprt.
     Added unjail.c source.
     Added Process Class.
 
###Update 5:
     Added libLV2.
     Added FW selector within fw.h. (set your FW to use there and libHB will use this setting)
     Automaticly Jailbreak on App initializing.
     Broke Libc and Libkernel. Need to fix.
 
###Update 6:
     Fixed LibLV2.
 
###Update 7:
     Fixed Clear screen finally.
     Fixed Usb Resolving to use a totaly unusual test name.
 
###Update 8:
     Fixed File Reading.
 
###Update 9:
     Added ReadLine(), ReadAllLines(), WriteLine()
     Fixed Logger.
 
###Update 10:
     Finished Application class.
 
###Update 11:
     Fixed missalign of the shader for the Banner.
     Added Abbility to stop a video when running by pressing button X.
     Added Functions drawRect() and fillRect() to the applications class. You can now draw from outside the Application code.
     Wrote a 3 lines Button sample.
     Added Functions AddDraw() and RemoveDraw() to the App class.
     Added compilled samples for FW 4.55.
     Added a LineBreak() function to the static console class.
 
###Update 12:
     Fixed Console::LineBreak().
     Added a sample "SaveDataMasterKeyDumper" on how to use libHB with your own kernel function.
     Moved the pkgs and the samples into the libhb-master.rar archive. Can be downloaded on the releases button.
     Will look into the FileSystem Dumper and fix it as next.
 
###Update 13:
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
 
###Update 14:
     Fixed lv2 offsets for FW 5.05.
     All Samples are back working now.
 
###Update 15:
     Added Functions GetSsid() & GetLanguage() to the SwissKnife class.

###Update 16:
     Fixed icc_nvs_read() and icc_nvs_write() offsets for 5.05.
     Added Line highlighting abbility to the Console.
     Added functions GetSelectedLine() & SetSelectedLine() as well as Clear() to the Console Class.
 
###Update 17:
     Apllication Lopp will now run in Core 3 which is less interrupted by the system then others. UserEntry Loop will run in Core 1.
     Added c language wrappers for the c++ Console Class. If you have some c source and you want to write to the screen, inlcude "c_console.h" and just do it.
     Added a BitConverter to SwissKnife to bit swapp bytes.
     PS4 Forms Class would be extendet. There is now a Dynamicall instance of the TTYD Class for on screen printing Text.
       Still the Static TTY Class will always directly write to the screen.
     Added RichTextBox Class to PS4Forms.
     Added TextViewer and HexViewer Classes to PS4Forms.
     The PictureBox of the Forms class got changed.
     Draw Functions of the Application class can now write out text blocks too.
     LibLua is now embeded as a static library into libhb.
     Added Event and Sound Manager to LibUtil.
     Resources can now handled with libhb.
       Modify resources.lua and config.lua to add your pictures, sounds, translations or what ever.
       Add images to the Resource Manager class of the libUtil and sound files to the Sound Manager.
       A Detailed information video can be found on my youtube channel.*
       To enable them use "app.UseResources()" and "app.UseSounds()".
     Event class would be added to the libUtil.
     The Applications initalisation process where we, till now, couldn't debug it proper without to use UART,
       can now be debugged to a log file on a usb drive. Put a USB Stick into your console before running the app.
       Use "app.UseDebug()" to enable it.
     Logger class would be extendet and have a static "Logger::Debug()" function call which will write into the debug log but only
       when the "app.Debug()" flag would be set. Use it to write some additional private debug messages into the log.
     A background image can be set. After adding the resource and AFTER "app.Exec()" would be called, you can define a image within the UserEntry loop with,
       "app.setBackgroundImage();". With "app.ShowBgi()" and "app.HideBgi()" you can show or hide the Background Image.
     The Consoles typically Message and IME (aka input) Dialog would be added to the Application loop.
       Use "app.UseIme()" & "app.UseDialog()" to tell the app initialistaion routine that they shall be initialized.
       To use them just call "app.ShowMsg()" or "app.ShowIme()" to use them.
     Console Class, to draw text to screen, would be extendet. Following Functions are now available:
       Pause();
	     EnableHighlighting();
	     DisableHighlighting();
	     LineBreak();
	     DeleteLastLine();
	     SetXCoordinate();
	     SetYCoordinate();
	     GetXCoordinate();
	     GetYCoordinate();
	     GetSelectedLineIndex();
	     SetSelectedLineIndex();
	     *ReadLine();
     TTY Class can now colorize a specific amount of characters per line. Just use the normal Color draw functions. The routine will do everyting on it self.
       A Line can now be rewindet with "\r".
       Line Highlighting would be extendet. It will draw a bar over the selected line now.
     SwissKniffe have two new functions. "GetLanguage()" & "GetUserName()".
     The PS4File and PS4Dir Classes got the Copy and CopyRecursive Function rewritten.
       Additional the "PS4File::Exists()" and "PS4Dir::Exists()" functions do use now sceFios2 library to check whether they exists or not.
     A ELF Loader was added to the libhb. (Still WIP)
     
     Minor Fixes:
       Fixed redefination of App instance.
       Dropped to run App Loop in Core 3 since libLua and the cast
       "lua_doFile()" will break and stop the initialisation process.

       Removed User_Entry_Manager and NP-Toolkit from the Project since we not
       gonna using them and they causing troubles between different SDK versions.
       THX @ XDPX for pointing me onto.
       
       Fixed another SDK version trouble. Added a additional defination to
       "defines.h" called "#define SDK_350" since 
       "getVideoOutHandle()" is not supported any more.
       There for, if you use a other SDK then 3.50 to compille
       your app code, then just change that defination to something not "SDK_350".
       May "SDK_XXX" is already ok.
       Again Thx to XDPX for telling me about that bug.

 * [Update 17](https://www.youtube.com/watch?v=-s3ilyo7bnI&t= "How to use Resources")

###Update 18.0
     The whole Project is now build as Dynamic Linked Library.
     useDebug() and Logger::Debug() would be dismissed since libhb is now using my printf driver to write
       debug messages from userland to the Systems UART line.
     SampleUtil aka BaseService lib got non working printf functions changed to _printf.
        With that, libhb will now write out SDK debug messages, just like on a official Debug/Test Console.
     There is now a template for new projects which includes main.cpp, program.cpp/.h.
       It's ready to use. After you createt a new project add thoes templates, then just add
       the libhb project file to it and set a depense to your project.
       That's basicly just the application.cpp source which is the Core.
       If you want to use libhb Tools, add the project prx from libhb folder
       and set a depense.
       finalize_pkg app will copy all prxs from libhb/ORBIS_Debug and the one from the 
       projects folder into /YourProject/data/app/sce_module/.
       A video explanation can be found on my youtube channel.*
     Rearanged the de-/initialisation functions and now we have the Application Loop running in Core 3.
       So: UserEntry == Core 1.
           AppLoop   == Core 3.
     TinyXml static library stub is included within the "libs" folder, which the SampleUtil will need to compile correctly.
       Just copy the stub to /SDK/target/lib/.
     
     [18.1]
     Added ps4_vs_template_hack.exe which will install libhbs main.cpp, program.cpp and program.h as templates for
       PS4 ELF Projects.
     
     [18.2]
     Added a Visual Studio Integration for libhb.
     Rearanged the Escape of the Sandbox process to avoid the usage of absolute paths for 
       the initialisation process.
       This way, paths from the Resources need not to know the TitleID of your application.
     Removed the function "setTitleId()" from the application loop.

 * [Update 18](https://www.youtube.com/watch?v=mg3RBstJ__8 "Libhb Update 18")