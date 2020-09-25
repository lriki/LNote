
cd ..\Documents\doxygen
doxygen Doxy_CommonManual
xcopy src CommonManual\html /s /i /y

CommonManual\html\index.html

cd ..\..\
