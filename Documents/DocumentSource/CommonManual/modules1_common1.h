//=============================================================================
//【 共通/ベース機能 】
//-----------------------------------------------------------------------------
/**
    @page modules1_common1 共通/ベース機能

    - @ref modules1_common1_section1   "エラー処理"
    - @ref modules1_common1_section2   "リザルトコード"
    <br>

    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    <hr>
    @anchor modules1_common1_section1
    @par エラー処理
    <br>
    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    機能の初期化やリソースの読み込み等の処理は、<br>
    終了後に成功または失敗とその原因を示す数値(リザルトコード)を返します。<br>
    <br>
    SUCCEEDED、FAILED マクロを使用することで、成功/失敗をチェックすることができます。<br>
    <br>
@ln_codeblock
        @ln_tabdecl
        @ln_tab{tab1,C/C++(関数)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
        @ln_code{tab1,cpp}
HRESULT hr = LAudio_Init();
if (SUCCEEDED(hr))
{
    // 成功
}
if (FAILED(hr))
{
    // 失敗
}

// ※リザルトコードの中身は独自定義の HRESULT 型です。
        @ln_endcode
        @ln_code{tab_hsp,cpp}
// 命令リファレンスに「stat:リザルトコード」と書かれている命令は stat にリザルトコードを格納します
LAudio_Init
if SUCCEEDED(stat) {
    // 成功
}
if FAILED(stat) {
    // 失敗
}
        @ln_endcode
    @ln_endcodeblock
    <br>
    <br>
    リザルトコードは成功/失敗の他に、失敗の原因を表します。
    以下のようにコーディングすることで、失敗の原因を知ることができます。<br>
    <br>
    @ln_codeblock
        @ln_tabdecl
        @ln_tab{tab1,C/C++(関数)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
        @ln_code{tab1,cpp}
HRESULT hr = LAudio_Init();
switch (hr)
{
    case LN_OK:
        // 成功
        break;
    case LN_ERR_INITIALIZED:
        // 既に初期化されている
        break;
    case LN_ERR_DIRECTX:
        // DirectX の機能でエラーが発生
        break;
    default:
        // その他
        break;
}
        @ln_endcode
        @ln_code{tab_hsp,cpp}
// 命令リファレンスに「stat:リザルトコード」と書かれている命令は stat にリザルトコードを格納します
LAudio_Init
switch stat
    case LN_OK
        // 成功
        swbreak
    case LN_ERR_INITIALIZED
        // 既に初期化されている
        swbreak
    case LN_ERR_DIRECTX:
        // DirectX の機能でエラーが発生
        swbreak
    default
        // その他
        swbreak
swend
        @ln_endcode
    @ln_endcodeblock
    <br>
    リザルトコードの定義と説明は @ref modules1_common1_section2 "リザルトコード" を参照してください。
    <br>
    <br>
    
    
    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    <hr>
    @anchor modules1_common1_section2
    @par リザルトコード
    <br>
    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    リザルトコードは以下の値が定義されています。<br>
    LN_ERR_～はすべて負の値です。<br>
    <br>
    <table>
    <tr bgcolor="#e0e0e0">
        <td>定数</td><td>説明</td>
    </tr>
    <tr>
        <td>LN_OK</td><td>成功。値は 0 です。</td>
    </tr>
    <tr>
        <td>LN_ERR_INVALIDCALL</td><td>引数が無効</td>
    </tr>
    <tr>
        <td>LN_ERR_OUTOFMEMORY</td><td>メモリ不足</td>
    </tr>
    <tr>
        <td>LN_ERR_NULLPOINTER</td><td>NULL アクセス</td>
    </tr>
    <tr>
        <td>LN_ERR_FILENOTFOUND</td><td>ファイルが無効 (オープンに失敗)</td>
    </tr>
    <tr>
        <td>LN_ERR_SYSTEM</td><td>システムAPI 呼び出しエラー</td>
    </tr>
    <tr>
        <td>LN_ERR_INITIALIZED</td><td>初期化済み</td>
    </tr>
    <tr>
        <td>LN_ERR_NOINITIALIZE</td><td>初期化されていない</td>
    </tr>
    <tr>
        <td>LN_ERR_FORMAT</td><td>非対応ファイル形式</td>
    </tr>
    <tr>
        <td>LN_ERR_NOTIMPL</td><td>実装されていない機能を呼び出そうとした</td>
    </tr>
    <tr>
        <td>LN_ERR_FATAL</td><td>継続不能なエラー</td>
    </tr>
    <tr>
        <td>LN_ERR_DIRECTX</td><td>DirectX 関係のエラー</td>
    </tr>
    <tr>
        <td>LN_ERR_OPENGL</td><td>OpenGL 関係のエラー</td>
    </tr>
    </table>
    
    <br>
    <br>
*/
//=============================================================================

