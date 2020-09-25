:==============================================================================
: Release_CPP
:==============================================================================

set DocumentRoot=Documents
set ReleaseRoot=Release\LNote_CPP

cd ..\
mkdir Release\LNote_CPP
mkdir Release\LNote_CPP\Doc
mkdir Release\LNote_CPP\Doc\CommonManual
mkdir Release\LNote_CPP\Sample
mkdir Release\LNote_CPP\Sample\Media

:--------------------------------------------------------------------------
: Documents
:--------------------------------------------------------------------------
xcopy %DocumentRoot%\doxygen\CommonManual\html   %ReleaseRoot%\Doc\CommonManual /s /i /y
xcopy %DocumentRoot%\doxygen\src                 %ReleaseRoot%\Doc\CommonManual /s /i /y

copy %DocumentRoot%\DocumentSource\CPP\Manual.html.lnk   %ReleaseRoot%
copy %DocumentRoot%\DocumentSource\CPP\Readme.txt        %ReleaseRoot%

:--------------------------------------------------------------------------
: Module
:--------------------------------------------------------------------------
copy MSVC\Release\LNote.dll     .\LNote.dll
copy MSVC\Release\LNote.dll     .\Sample\LNote.dll

copy Batch\Converter\lnote.as   .\lnote.as
copy Batch\Converter\lnote.as   .\Sample\lnote.as

copy Batch\Converter\lnote.hs   .\lnote.hs

:--------------------------------------------------------------------------
: Sample
:--------------------------------------------------------------------------
xcopy Sample\HSP\*.hsp          Release\LNote_HSP\Sample\ /i /y
xcopy Sample\HSP\Media          Release\LNote_HSP\Sample\Media /i /y

pause
