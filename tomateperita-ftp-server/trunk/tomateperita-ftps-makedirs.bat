set ruta=c:\tometeperita
md %ruta%
md %ruta%\bin
md %ruta%\logs
md %ruta%\conf
md %ruta%\ftp-data
copy ..\debug\tomateperita-ftp-server.exe %ruta%\bin
copy /-Y tomateperita-ftps.properties %ruta%\conf
@pause