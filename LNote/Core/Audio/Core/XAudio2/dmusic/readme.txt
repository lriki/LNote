
DirectMusic を使うために必要なヘッダーファイルです。

新しい DirectX SDK には入っていないため、
環境によってビルドできないという事を防ぐためにこちらにまとめてあります。

開発環境の設定を変更しなくても include が通るように、
ファイルの一部を書きなおしてあります。


・dmusici.h - 19 20 行目

//#include <dmusicc.h>
#include "dmusicc.h"     // ◆変更


・dmusici.h - 22 23 行目

//#include <dmplugin.h>
#include "dmplugin.h"    // ◆変更


・dmplugin - 19 20 行目

//#include <dmusici.h>
#include "dmusici.h"     // ◆変更


Unused フォルダには DirectMusic 関係のヘッダーファイルではあるけれども
ライブラリのビルドには必要ないものが入っています。


