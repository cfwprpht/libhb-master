@echo off
if not exist data\app\eboot.bin goto :LABEL1
if not exist data\app\sce_module goto :LABEL2

@echo on
orbis-pub-cmd img_create data\project.gp4 Libhb18TestSample.pkg
pause
exit

:LABEL1
echo [Error] Not exist data\app\eboot.bin
echo Please build Test sample.
pause
exit

:LABEL2
echo [Error] Not exist data\app\sce_module.
echo Please build Test sample.
pause
exit