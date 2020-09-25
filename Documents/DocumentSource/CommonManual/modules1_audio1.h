//=============================================================================
//【 音声機能 】
//-----------------------------------------------------------------------------
/**
    @page modules1_audio1 音声機能

    - @ref modules1_audio1_anchor6   "音声機能の初期化"
    - @ref modules1_audio1_anchor3   "サウンドの再生と操作"
    - @ref modules1_audio1_anchor1   "対応している音声ファイルフォーマット"
    - @ref modules1_audio1_anchor2   "MIDI の再生について"
    - @ref modules1_audio1_anchor4   "3D サウンド"
    - @ref modules1_audio1_anchor5   "ゲームオーディオ"
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_audio1_anchor6
    @par 音声機能の初期化
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    音声機能の初期化には LAudio_Init を使用します。<br>
    <br>
    音声機能を使用する前（できるだけプログラムの先頭）で、1度だけ呼び出してください。<br>
    <br>
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(関数)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
LNResult r = LAudio_Init();
if (FAILED(r))
{
    // 失敗
}
    @ln_endcode
    @ln_code{tab_hsp,cpp}
LAudio_Init
if FAILED(stat) {
    // 失敗
}
    @ln_endcode
@ln_endcodeblock
    <br>
    エラー処理については @ref modules1_common1_section1 "エラー処理" をご覧ください。<br>
    <br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_audio1_anchor3
    @par サウンドの再生と操作
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    サウンドの再生と操作は、音声データを元に作成したサウンドオブジェクトを通して行います。<br>
    <br>
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(関数)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
// サウンドオブジェクトを識別するためのサウンドハンドルが返ります
LNHandle sound_handle = LSound_Load( "sound.wav" );

// 読み込んだ音声を再生します
LSound_Play( sound_handle );
    @ln_endcode
    @ln_code{tab_hsp,cpp}
// サウンドオブジェクトを識別するためのサウンドハンドルを、変数 sound_handle に代入します
LSound_Load sound_handle, "sound.wav"

// 読み込んだ音声を再生します
LSound_Play sound_handle
    @ln_endcode
@ln_endcodeblock
    <br>
    サウンドの音量やピッチ、後述する3Dサウンドの位置情報は、再生中にいつでも変更することができます。<br>
    また、既に読み込まれている音声ファイルと同名のものを読み込もうとした場合、リソース節約のために音声データを自動で共有します。（Wave、Ogg のみ）<br>
    <br>
    <br>
    サウンドオブジェクトの作成時(上記 LSound_Load 等)には、下記のフラグで再生方法を指定することができます。<br>
    指定方法についてはリファレンスを参考にしてください。<br>
    - LN_SOUNDPLAYTYPE_ONMEMORY<br>
        音声を必ずオンメモリ再生します。<br>
        <br>
    - LN_SOUNDPLAYTYPE_STREAMING<br>
        音声を必ずストリーミング再生します。<br>
        <br>
    - LN_SOUNDPLAYTYPE_AUTO (推奨, デフォルト)<br>
        デコード後のサイズが 10000 バイト以上になる場合はストリーミング、<br>
        そうでない場合はオンメモリ再生になります。<br>
    <br>
    <br>

    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_audio1_anchor1
    @par 対応している音声ファイルフォーマット
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    対応している音声ファイルフォーマットは以下の通りです。
    <br>
    - WAVE (RIFF waveform Audio Format)
    - MP3 (MPEG Audio Layer-3)
    - Ogg (Ogg Vorbis)
    - MIDI (Musical Instrument Digital Interface)
    
    @par
    <br>
    MIDI ファイルは DirectMusic、それ以外のファイルは XAudio2 で演奏されます。<br>
    <br>
    また、Ogg と MIDI は以下の方法でループ位置を検出します。<br>
    (RPGツクールVXの仕様と同じものです)<br>
    - Ogg のループ位置<br>
    <br>
        ファイル内に LOOPSTART、LOOPLENGTH というコメントがある場合、
        それをもとにループ位置を設定します。<br>
        LOOPSTART はループ領域の開始サンプル数、<br>
        LOOPLENGTH はループ領域の長さのサンプル数です。<br>
        <br>
    - MIDI のループ位置<br>
    <br>
        ファイル内にコントロールチェンジ 111 番のイベントがある場合、
        そこをループ開始位置として設定します。<br>
        (複数ある場合は一番最後のものが対象になります)<br>
        データの最後まで再生された場合、そこから再開します。<br>
        <br>
        <br>
    
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_audio1_anchor2
    @par MIDI の再生について
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    MIDI は前述のとおり DirectMusic で演奏されますが、DirectMusic の初期化には数秒程度の時間がかかります。<br>
    <br>
    MIDI を使用しないソフトでも初期化に時間がかかってしまう・・・といったことを防ぐために、DirectMusic の有無は切り替えることができます。<br>
    MIDI を使用するためにはライブラリの初期化設定で、MIDI の使用（DirectMusic の初期化）を明示する必要があります。<br>
    <br>
    @ln_codeblock
        @ln_tabdecl
        @ln_tab{tab1,C/C++(関数)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
        @ln_code{tab1,cpp}
LConfig_SetDirectMusicUsingMode( LN_DMUSINGMODE_USE );
        @ln_endcode
        @ln_code{tab_hsp,cpp}
LConfig_SetDirectMusicUsingMode LN_DMUSINGMODE_USE
        @ln_endcode
    @ln_endcodeblock
    <br>
    指定できる値は以下の通りです。<br>
    - LN_DMUSINGMODE_NOTUSE (デフォルト)<br>
        DirectMusic を使用しません。(MIDI を演奏することはできません)<br>
        <br>
    - LN_DMUSINGMODE_USE<br>
        DirectMusic を使用します。<br>
        環境によっては初期化に数秒程度の時間がかかります。<br>
        <br>
    - LN_DMUSINGMODE_THREAD_REQUEST<br>
        DirectMusic を別スレッドで初期化するようにします。<br>
        初期化等のための遅延は一切なくなりますが、<br>
        初期化が完了する前に MIDI を演奏しようとしたときは、完了してから音が鳴り始めます。<br>
        （この状態でもサウンドは「再生中」として扱われます）<br>
        <br>
    
    @par
    なお、DirectMusic を使った MIDI の再生ではリバーブのエフェクトがかかりますが、<br>
    音割れを軽減するために DirectMusic が付けてくれるデフォルトのリバーブよりも若干弱めに設定してあります。<br>
    <br>
    また、MIDI はサウンドの再生位置やループ位置の指定に「ミュージックタイム」という値を使用します。<br>
    これは4分音符ひとつ分を 768 とした単位で、LN_MUSICTIMEBASE として定義されています。<br>
    （LNote では、MIDI の場合のサンプル数=ミュージックタイム として扱っています）<br>
    <br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_audio1_anchor4
    @par 3Dサウンド
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    3Dサウンドは、音声を3D空間上で再生する機能です。<br>
    視点からの位置によって、サウンドが立体的に聞こえてくるような演出をすることができます。<br>
    - 単位距離の設定<br>
        必要に応じて、最初に 3D 空間の 1 メートルに相当する距離を設定します。<br>
        この設定はドップラー効果の計算に適用されます。<br>
        デフォルトは 1.0 です。<br>
        <br>
        例えば MikuMikuDance のモデルを基準としてワールドを構築する場合、<br>
        1.58m のキャラクターが約 20.0 の高さのモデルとなっているため、<br>
        20.0 / 1.58 = 12.658... を 1 メートルとすることになります。<br>
        <br>
    - リスナー(聴取者)<br>
        3D空間上のサウンドを聞く位置に関する情報です。位置、向き、速度を表します。<br>
        一般に、リスナーの位置と方向はカメラの位置および方向と同じです。<br>
        リスナーは音声機能全体に対して1つだけ存在します。<br>
        <br>
    - エミッター(サウンドオブジェクト)<br>
        サウンドの発生源の情報です。<br>
        ひとつのサウンドオブジェクトがひとつの位置、速度、範囲を持ちます。<br>
        <br>
        
    @par 
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(関数)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
// 3D 空間の 1 メートルに相当する距離を設定します
LAudio_SetMetreUnitDistance( 20.0 / 1.58 );

// リスナーの位置と向きを設定します
L3DSoundListener_SetPosition( 10.0, 0.0, 5.0 );
L3DSoundListener_SetDirection( 0.0, 0.0, 1.0 );

// サウンドオブジェクトの位置と、音声の聞こえる距離を設定します
LSound_SetEmitterPosition( sound_handle, -10.0, 5.0, 20.0 );
LSound_SetEmitterDistance( sound_handle, 100.0 );
    @ln_endcode
    @ln_code{tab_hsp,cpp}
// 3D 空間の 1 メートルに相当する距離を設定します
LAudio_SetMetreUnitDistance 20.0 / 1.58

// リスナーの位置と向きを設定します
L3DSoundListener_SetPosition 10.0, 0.0, 5.0
L3DSoundListener_SetDirection 0.0, 0.0, 1.0

// サウンドオブジェクトの位置と、音声の聞こえる距離を設定します
LSound_SetEmitterPosition sound_handle, -10.0, 5.0, 20.0
LSound_SetEmitterDistance sound_handle, 100.0
    @ln_endcode
@ln_endcodeblock
    <br>
    3Dサウンドの座標空間は左手座標系です。<br>
    （x 軸の値は左から右方向に、y 軸の値は下から上方向に、z 軸の値は手前側から遠ざかる方向に大きくなります）<br>
    <br>
    なお、MIDI は 3D 音声として再生することはできません。<br>
    <br>
    <br>
    

    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    @anchor modules1_audio1_anchor5
    <hr>
    @par ゲームオーディオ
    <br>
    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    ゲームオーディオはゲーム用に特化したサウンド再生機能です。<br>
    BGM、SE 等のあらかじめ用意されている方法で演奏します。<br>
    演奏中の一時停止などの細かい設定はできませんが、全体的にシンプルにコーディングすることができます。<br>
    <br>
    ゲームオーディオではサウンドを以下の種類に分類し、それぞれの用途に基づいて演奏します。<br>
    <br>
    - <b>BGM</b> (背景音楽)<br>
        ゲーム中の BGM です。 <br>
        BGM はひとつだけ演奏することができ、ループ再生します。<br>
        新しい BGM が演奏ようとした場合、古いものはその場で停止するか、<br>
        フェードイン時間を指定した場合はクロスフェードしながら音声を切り替えます。<br>
        <br>
    - <b>BGS</b> (環境音)<br>
        風や滝の音などの環境音です。<br>
        BGM と同様、ひとつだけ演奏することができ、ループ再生します。<br>
        BGS は BGM と同時に演奏することができます。<br>
        <br>
        また、ME の演奏による影響を受けず、常に演奏されます。<br>
        <br>
    - <b>ME</b>  (効果音楽)<br>
        レベルアップやアイテム取得時のファンファーレ等の演奏に使います。<br>
        ME はひとつだけ演奏され、ループ再生せずに一度だけ演奏されます。<br>
        <br>
        ME の演奏開始時に BGM が演奏されていた場合は、<br>
        BGM を 500 ミリ秒かけて一時停止させ、その後に ME の演奏を開始します。<br>
        その場合、ME の演奏終了後に BGM の演奏を再開し、2000 ミリ秒かけて BGM をフェードインさせながら再開させます。 <br>
        (この時間はデフォルトの設定で、必要に応じて任意の値を設定することができます)<br>
        <br>
        BGS はこの影響を受けず、常に再生されます。<br>
        <br>
    - <b>SE</b>  (効果音)<br>
        ゲーム中の効果音です。
        SE は同時に複数再生することができ、同じ音声を演奏した場合は
        重ねがけされて演奏されます。<br>
        (SE は必ずオンメモリで再生されます。サイズの大きいファイルを演奏することは避けてください)<br>
        <br>
        また、3D 音声として演奏することも可能です。<br>
        <br>
    
    @par
    以下にそれぞれの性質をまとめます。
    <table>
    <tr>
        <td></td><td>ループ再生</td><td>重ね合わせ</td><td>メモリ管理</td>
    </tr>
    <tr>
        <td>BGM</td><td>○</td><td>×</td><td>ストリーミング</td>
    </tr>
    <tr>
        <td>BGS</td><td>○</td><td>×</td><td>ストリーミング</td>
    </tr>
    <tr>
        <td>ME</td><td>×</td><td>×</td><td>オンメモリ</td>
    </tr>
    <tr>
        <td>SE</td><td>×</td><td>○</td><td>オンメモリ</td>
    </tr>
    </table>
    <br>
    @attention
        <b>[HSP]</b><br>
        実行ファイルにパックされた音声ファイルは、すべてオンメモリで再生されます。<br>
        MP3 や OGG 形式の音声はデコード後のサイズ分のメモリを使うため、<br>
        大きいサイズのファイルを再生するときは注意してください。<br>
        (MP3 と OGG では、ファイルサイズの約 10～20 倍のメモリを使い、デコードにもその分の時間を使います)<br>
        
    <br>
    <br>

    <br>
    <br>
*/
//=============================================================================
