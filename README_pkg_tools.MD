## == SDK Helper ==
     copy both files into the dir of your project.
     open the make_pkg.bat with a editor and change the pkg and project name to the one of yours.
     when you have builded your elf just right click the finalize_pkg.exe and run as Administrator.
     the app will resolve the project name and check for the assembly elf, copy it
     into the data/app folder and run the make_pkg.bat to build the pkg.

## == Visual Studio Integration ==
     After you have installed libhb and printf to your sdks "/../samples/sample_code/system/" folder
     run the Integration Installer as admin.
     If you have previous used the ps4_vs_template_hack.exe, just run it again with the flag -remove
     from a terminal to have a clean base.
     After successfull install of he Integration you will have two new entrys with the
     Visual Studio C++ PS4 Category.
     One Project without and one with Resources as depencie referenced.