:==============================================================================
: Release_HSP
:==============================================================================

cd Converter
call MakeHSPHeader.bat
call MakeHSPHelp.bat

cd ..\..\
mkdir Release\LNote_HSP
mkdir Release\LNote_HSP\Doc
mkdir Release\LNote_HSP\Doc\CommonManual
mkdir Release\LNote_HSP\Sample
mkdir Release\LNote_HSP\Sample\Media

:--------------------------------------------------------------------------
: Documents
:--------------------------------------------------------------------------
xcopy Documents\doxygen\CommonManual\html   Release\LNote_HSP\Doc\CommonManual /s /i /y
xcopy Documents\doxygen\src                 Release\LNote_HSP\Doc\CommonManual /s /i /y

copy Documents\DocumentSource\HSP\Manual.html.lnk   Release\LNote_HSP\
copy Documents\DocumentSource\HSP\Readme.txt        Release\LNote_HSP\

:--------------------------------------------------------------------------
: Module
:--------------------------------------------------------------------------
copy LNote\LNote.dll     Release\LNote_HSP\LNote.dll
copy LNote\LNote.dll     Release\LNote_HSP\Sample\LNote.dll

copy Batch\Converter\lnote.as   Release\LNote_HSP\lnote.as
copy Batch\Converter\lnote.as   Release\LNote_HSP\Sample\lnote.as

copy Batch\Converter\lnote.hs   Release\LNote_HSP\lnote.hs

:--------------------------------------------------------------------------
: Sample
:--------------------------------------------------------------------------
xcopy Sample\HSP\*.hsp          Release\LNote_HSP\Sample\ /i /y
xcopy Sample\HSP\Media          Release\LNote_HSP\Sample\Media /i /y

pause
