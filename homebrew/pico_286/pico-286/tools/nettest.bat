@ECHO OFF
ECHO HOSTDRV network redirector test
ECHO Preparing local source file
ECHO R36SX HOSTDRV copy test>C:\NETSRC.TXT
ECHO Creating remote directory
MKDIR H:\R36TEST
ECHO Copying local file to remote drive
COPY /Y C:\NETSRC.TXT H:\R36TEST\NETSRC.TXT
ECHO Copying remote file back to local drive
COPY /Y H:\R36TEST\NETSRC.TXT C:\NETBACK.TXT
ECHO Deleting remote file
DEL H:\R36TEST\NETSRC.TXT
ECHO Removing remote directory
RMDIR H:\R36TEST
ECHO HOSTDRV network redirector test complete
