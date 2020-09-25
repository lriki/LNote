:-------------------------------------------------------------------------------
: ■ いらないファイルを全削除
:-------------------------------------------------------------------------------

cd ..\

:---------------------------------------------------------------------------
: ▼ 退避するもの


:---------------------------------------------------------------------------
: ▼ VisualStudio のビルド時出力ファイル

del /s /q *.idb
del /s /q *.pdb
del /s /q *.manifest
del /s /q *.exp
del /s /q *.ilk
del /s /q *.dep
del /s /q *.obj
del /s /q *.pch
:del /s /q *.res

: vc2010
del /s /q *.lastbuildstate
del /s /q *.tlog
del /s /q *.log
del /s /q *.ipch
del /s /q *.tmp

:---------------------------------------------------------------------------
: ▼ VisualStudio のプロジェクト関係ファイル

del /s /q *.ncb
del /s /q *.user
del /s /q *.suo
del /s /q /a:h *.suo

: vc2010 のインテリセンス
del /s /q *.sdf

: ヘルプファイルにしてた .html ファイルも削除されたみたいなのでここではコメントアウト
:del /s /q *.htm

:---------------------------------------------------------------------------
: ▼ Sample フォルダ内のファイル

cd Sample
del /s /q *.exe
del /s /q *.res
del /s /q *.htm

:---------------------------------------------------------------------------
: ▼ MSVCフォルダ内の実行ファイルとdll

cd ..\MSVC
del /s /q *.exe

: glew 対策待ち
:del /s /q *.dll


:---------------------------------------------------------------------------
: ▼ LNote フォルダ内のdll、lib

cd ..\LNote
del /s /q *.dll
del /s /q *.lib

:---------------------------------------------------------------------------
: ▼ 細かいの

cd ..\
del /s /q lnlog.txt
del /s /q UpgradeLog*


pause