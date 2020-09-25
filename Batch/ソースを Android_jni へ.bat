
cd ..\
xcopy LNote\Core\*.h        Android\LNote_Android\jni\Core /s /i /y
xcopy LNote\Core\*.cpp      Android\LNote_Android\jni\Core /s /i /y
xcopy LNote\Binding\*.h     Android\LNote_Android\jni\Binding /s /i /y
xcopy LNote\Binding\*.cpp   Android\LNote_Android\jni\Binding /s /i /y
copy  LNote\lnote.hpp       Android\LNote_Android\jni

echo "Core、Binding、lnote.hpp のみをコピー。"
echo "Dependencies は LNote/Dependencies を直接参照する。"

pause