set ruta=c:\tometeperita
md %ruta%
md %ruta%\bin
md %ruta%\db
md %ruta%\logs
md %ruta%\conf
copy ..\debug\tomateperita-vda.exe %ruta%\bin
copy /-Y tomateperita-vda.properties %ruta%\conf
@pause