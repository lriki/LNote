;============================================================
; ■ LNote ヘルプファイル
;============================================================

%dll
LNote

%ver
1.0

%date
2013/2/10

%author
Riki

%note
lnote.as をインクルードしてください

%type
拡張命令

%url
http://vianote.sakura.ne.jp/
;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_Init
音声機能の初期化

%group
音声機能
%prm


%inst
音声機能のみを使用する設定で初期化を行います。

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayBGM
BGM を演奏する

%group
音声機能
%prm
filename, volume, pitch, fade_time
[in] filename     : ファイル名
[in] volume(100)  : ボリューム (0 ～ 100)
[in] pitch(100)   : ピッチ (50 ～ 200)
[in] fade_time(0) : フェードインにかける時間 (ミリ秒)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayBGMMem
メモリ上の音声データから BGM を演奏する

%group
音声機能
%prm
data, data_size, volume, pitch, fade_time
[in] data         : メモリ上の音声データへ
[in] data_size    : データサイズ (バイト単位)
[in] volume(100)  : ボリューム (0 ～ 100)
[in] pitch(100)   : ピッチ (50 ～ 200)
[in] fade_time(0) : フェードインにかける時間 (ミリ秒)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_StopBGM
BGM の演奏を停止する

%group
音声機能
%prm
fade_time
[in] fade_time(0) : フェードアウトにかける時間 (ミリ秒)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayBGS
BGS を演奏する

%group
音声機能
%prm
filename, volume, pitch, fade_time
[in] filename     : ファイル名
[in] volume(100)  : ボリューム (0 ～ 100)
[in] pitch(100)   : ピッチ (50 ～ 200)
[in] fade_time(0) : フェードインにかける時間 (ミリ秒)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayBGSMem
メモリ上の音声データから BGS を演奏する

%group
音声機能
%prm
data, data_size, volume, pitch, fade_time
[in] data         : メモリ上の音声データへ
[in] data_size    : データサイズ (バイト単位)
[in] volume(100)  : ボリューム (0 ～ 100)
[in] pitch(100)   : ピッチ (50 ～ 200)
[in] fade_time(0) : フェードインにかける時間 (ミリ秒)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_StopBGS
BGS の演奏を停止する

%group
音声機能
%prm
fade_time
[in] fade_time(0) : フェードアウトにかける時間 (ミリ秒)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayME
ME を演奏する

%group
音声機能
%prm
filename, volume, pitch
[in] filename    : ファイル名
[in] volume(100) : ボリューム (0 ～ 100)
[in] pitch(100)  : ピッチ (50 ～ 200)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlayMEMem
メモリ上の音声データから ME を演奏する

%group
音声機能
%prm
data, data_size, volume, pitch
[in] data        : メモリ上の音声データへ
[in] data_size   : データサイズ (バイト単位)
[in] volume(100) : ボリューム (0 ～ 100)
[in] pitch(100)  : ピッチ (50 ～ 200)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_StopME
ME の演奏を停止する

%group
音声機能
%prm


stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlaySE
SE を演奏する

%group
音声機能
%prm
filename, volume, pitch
[in] filename    : ファイル名
[in] volume(100) : ボリューム (0 ～ 100)
[in] pitch(100)  : ピッチ (50 ～ 200)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlaySE3D
SE を演奏する (3D サウンド)

%group
音声機能
%prm
filename, x, y, z, distance, volume, pitch
[in] filename    : ファイル名
[in] x, y, z     : 3D 座標
[in] distance    : 減衰距離
[in] volume(100) : ボリューム (0 ～ 100)
[in] pitch(100)  : ピッチ (50 ～ 200)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlaySEMem
メモリ上の音声データから SE を演奏する

%group
音声機能
%prm
data, data_size, volume, pitch
[in] data        : メモリ上の音声データへ
[in] data_size   : データサイズ (バイト単位)
[in] volume(100) : ボリューム (0 ～ 100)
[in] pitch(100)  : ピッチ (50 ～ 200)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_PlaySE3DMem
メモリ上の音声データから SE を演奏する (3D サウンド)

%group
音声機能
%prm
data, data_size, x, y, z, distance, volume, pitch
[in] data        : メモリ上の音声データへ
[in] data_size   : データサイズ (バイト単位)
[in] x, y, z     : 3D 座標
[in] distance    : 減衰距離
[in] volume(100) : ボリューム (0 ～ 100)
[in] pitch(100)  : ピッチ (50 ～ 200)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_StopSE
すべての SE の演奏を停止する

%group
音声機能
%prm


stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LAudio_SetMetreUnitDistance
3D 空間の1メートル相当の距離の設定

%group
音声機能
%prm


stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
L3DSoundListener_SetPosition
リスナーの位置の設定

%group
音声機能
%prm
x, y, z
[in] x, y, z : 座標

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
L3DSoundListener_SetDirection
リスナーの正面方向の設定

%group
音声機能
%prm
x, y, z
[in] x, y, z : 向き

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
L3DSoundListener_SetUpDirection
リスナーの上方向の設定

%group
音声機能
%prm
x, y, z
[in] x, y, z : 向き (正面方向とは直交であること)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
L3DSoundListener_Velocity
リスナーの速度の設定

%group
音声機能
%prm
x, y, z
[in] x, y, z : 速度

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_CreateMem
メモリ上のデータから音声を作成する

%group
音声機能
%prm
data, data_size, enable_3d
[in] data             : メモリ上の音声データへ
[in] data_size        : データサイズ (バイト単位)
[in] enable_3d(false) : true の場合、3D 音源として作成する

stat : サウンドハンドル
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Load
ファイルから音声を作成する

%group
音声機能
%prm
out_handle, filename, enable_3d, play_type
[out] out_handle                       : サウンドハンドルを格納する変数
[in]  filename                         : ファイル名
[in]  enable_3d(false)                 : true の場合、3D 音源として作成する
[in]  play_type(LN_SOUNDPLAYTYPE_AUTO) : 音声の再生方法

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Release
音声を削除する

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetVolume
ボリュームの設定

%group
音声機能
%prm
sound_handle, volume
[in] sound_handle : サウンドハンドル
[in] volume       : ボリューム (0～100)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetVolume
ボリュームの取得

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

stat : ボリューム
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetPitch
ピッチの設定

%group
音声機能
%prm
sound_handle, pitch
[in] sound_handle : サウンドハンドル
[in] pitch        : ピッチ (50～200)

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetPitch
ピッチの取得

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

stat : ピッチ
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetLoopState
ループ再生の有効設定

%group
音声機能
%prm
sound_handle, flag, begin, length
[in] sound_handle : サウンドハンドル
[in] flag         : true = ループ再生する / false = しない
[in] begin(0)     : ループ領域の先頭位置 (サンプル数単位)
[in] length(0)    : ループ領域長さ (サンプル数単位)

stat : リザルトコード
%inst
begin と length に 0 を指定すると、全体をループ領域として設定します。

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_IsLoop
ループ再生が有効かを調べる

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

stat : true = ループ再生する / false = ループ再生しない
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetPlayState
サウンドの再生状態を取得する

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

stat : LN_SOUNDPLAYSTATE_STOP    : 停止中
       LN_SOUNDPLAYSTATE_PLAYING : 再生中
       LN_SOUNDPLAYSTATE_PAUSING : 一時停止中
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Play
音声を再生する

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Stop
再生を停止する

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Pause
一時停止

%group
音声機能
%prm
sound_handle, pause
[in] sound_handle : サウンドハンドル
[in] pause        : true = 一時停止 / false = 一時停止解除

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_FadeVolume
音量のフェード

%group
音声機能
%prm
sound_handle, target_volume, time, fade_state
[in] sound_handle  : サウンドハンドル
[in] target_volume : 変更先の音量
[in] time          : フェードにかける時間 (ミリ秒)
[in] fade_state    : フェード完了後の動作の指定
                       LN_SOUNDFADE_CONTINUE : 継続
                       LN_SOUNDFADE_STOP : 停止する
                       LN_SOUNDFADE_STOP_RESET : 停止して、音量を元の値に戻す
                       LN_SOUNDFADE_PAUSE : 一時停止する
                       LN_SOUNDFADE_PAUSE_RESET : 一時停止して、音量を元の値に戻す

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetUnitsPlayed
再生したサンプル数の取得

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

stat : 再生したサンプル数
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetTotalUnits
音声データ全体のサンプル数の取得

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

stat : 音声データ全体のサンプル数
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_GetUnitsParSecond
サンプリング周波数の取得

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

stat : サンプリング周波数
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_Is3DSound
3D 音源かを調べる

%group
音声機能
%prm
sound_handle
[in] sound_handle : サウンドハンドル

stat : true=3D音源 / false=3D音源ではない
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetEmitterPosition
3D 音源としての位置を設定する

%group
音声機能
%prm
sound_handle, x, y ,z
[in] sound_handle : サウンドハンドル
[in] x, y ,z      : 3D 座標

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetEmitterVelocity
3D 音源としての速度を設定する

%group
音声機能
%prm
sound_handle, x, y, z
[in] sound_handle : サウンドハンドル
[in] x, y, z      : 速度

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LSound_SetEmitterDistance
3D 音源の減衰距離 (聴こえなくなる距離) を設定する

%group
音声機能
%prm
sound_handle, distance
[in] sound_handle : サウンドハンドル
[in] distance     : 距離

stat : リザルトコード
%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LConfig_SetEnableLogFile
ログファイルの出力設定

%group
初期設定
%prm
flag
[in] flag : true=ログファイルを出力する / false=しない

%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LConfig_SetEnableConsole
コンソールの割り当て設定

%group
初期設定
%prm
flag
[in] flag : true=割り当てる / false=割り当てない

%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LConfig_SetUserWindowHandle
ユーザー定義のウィンドウハンドルの設定

%group
初期設定
%prm
window_handle
[in] window_handle : ユーザー定義のウィンドウハンドル

%inst

%href


;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LConfig_SetDirectMusicUsingMode
DirectMusic の使用設定

%group
初期設定
%prm
mode
[in] mode : DirectMusic の使用方法
              LN_DMUSINGMODE_NOTUSE         : 使用しない
              LN_DMUSINGMODE_USE            : 使用する
              LN_DMUSINGMODE_THREAD_REQUEST : 別スレッドで初期化して、再生時に未完了の場合は再生を予約する

%inst

%href

