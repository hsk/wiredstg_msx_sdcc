// bios.h : BIOS コール
#include <stdint.h>
typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef uint16_t s16;
// MAIN-ROM コールアドレス
#define CHKRAM      0x0000          /* RAM をチェックし、システム用の RAM スロットをセット */
#define SYNCHR      0x0008          /* [HL] の 1 文字が指定した文字かどうかチェック */
#define RDSLT       0x000c          /* A の値に対応するスロットを選択し、そのスロットのメモリを 1 バイト読む */
#define CHRGTR      0x0010          /* BASIC テキストから文字 (またはトークン) を取り出す */
#define WRSLT       0x0014          /* A の値に対応するスロットを選択し、そのスロットのメモリに値を 1 バイト書き込む */
#define OUTDO       0x0018          /* 現在使っているデバイスに値を出力 */
#define CALSLT      0x001c          /* 他のスロットのルーチンを読み出す (インタースロット・コール) */
#define DCOMPR      0x0020          /* HL と DE の内容を比較 */
#define ENASLT      0x0024          /* A の値に対応するスロットを選択し、以降そのスロットを使用可能にする */
#define GETYPR      0x0028          /* DAC (デシマル・アキュムレータ) の型を返す */
#define CALLF       0x0030          /* 他のスロットのルーチンを呼び出す */
#define KEYINT      0x0038          /* タイマ割り込みの処理ルーチンを実行 */
#define INITIO      0x003b          /* デバイスを初期化 */
#define INIFNK      0x003e          /* ファンクションキーの内容を初期化 */
#define DISSCR      0x0041          /* 画面表示の禁止 */
#define ENASCR      0x0044          /* 画面の表示 */
#define WRTVDP      0x0047          /* VDP のレジスタにデータを書き込む */
#define RDVRM       0x004a          /* VRAM の内容を読む */
#define WRTVRM      0x004d          /* VRAM にデータを書き込む */
#define SETRD       0x0050          /* VDP に VRAM アドレスをセットして、読み出せる状態にする */
#define SETWRT      0x0053          /* VDP に VRAM アドレスをセットして、書き込める状態にする */
#define FILVRM      0x0056          /* VRAM の指定領域を同一のデータで埋める */
#define LDIRMV      0x0059          /* VRAM からメモリへブロック転送 */
#define LDIRVM      0x005c          /* メモリから VRAM へブロック転送 */
#define CHGMOD      0x005f          /* スクリーン・モードを変える */
#define CHGCLR      0x0062          /* 画面の色を変える */
#define NMI         0x0066          /* NMI (Non Maskable Interrupt) 処理ルーチンを実行 */
#define CLRSPR      0x0069          /* すべてのスプライトを初期化 */
#define INITXT      0x006c          /* 画面を TEXT1 モード (40*24) に初期化 */
#define INIT32      0x006f          /* 画面を GRAPHIC1 モード (32*24) に初期化 */
#define INIGRP      0x0072          /* 画面を高解像グラフィックモードに初期化 */
#define INIMLT      0x0075          /* 画面を MULTI COLOR モードに初期化 */
#define SETTXT      0x0078          /* VDP のみを TEXT1 モード (40*24) にする */
#define SETT32      0x007b          /* VDP のみを GRAPHIC1 モード (32*24) にする */
#define SETGRP      0x007e          /* VDP のみを GRAPHIC2 モードにする */
#define SETMLT      0x0081          /* VDP のみを MULTI COLOR モードにする */
#define CALPAT      0x0084          /* スプライト・ジェネレータ・テーブルのアドレスを返す */
#define CALATR      0x0087          /* スプライトアトリビュート・テーブルのアドレスを返す */
#define GSPSIZ      0x008a          /* 現在のスプライト・サイズを返す */
#define GRPPRT      0x008d          /* グラフィック画面に文字を表示 */
#define GICINI      0x0090          /* PSG を初期化し、PLAY 文のための初期値をセット */
#define WRTPSG      0x0093          /* PSG のレジスタにデータを書き込む */
#define RDPPSG      0x0096          /* PSG のレジスタの値を読む */
#define STRTMS      0x0099          /* バックグラウンド・タスクとして PLAY 文が実行中であるかどうかチェックし、実行中でなければ PLAY 文の実行を開始 */
#define CHSNS       0x009c          /* キーボード・バッファの状態をチェック */
#define CHGET       0x009f          /* 1 文字入力 (入力待ちあり) */
#define CHPUT       0x00a2          /* 1 文字表示 */
#define LPTOUT      0x00a5          /* 1 文字プリンタ出力 */
#define LPTSTT      0x00a8          /* プリンタの状態をチェック */
#define CNVCHR      0x00ab          /* グラフィック・ヘッダかどうかをチェックし、コードを変換 */
#define PINLIN      0x00ae          /* リターンキーや STOP キーがタイプされるまでに入力された文字コードを指定されたバッファに格納する */
#define INLIN       0x00b1          /* AUTFLG (F6AAH) がセットされる以外は PINLIN と同じ */
#define QINLIN      0x00b4          /* ？と空白 1 文字を出力 */
#define BREAKX      0x00b7          /* Ctrl-STOP キーを押しているかどうかチェック */
#define BEEP        0x00c0          /* ブザーを鳴らす */
#define CLS         0x00c3          /* 画面クリア */
#define POSIT       0x00c6          /* カーソルの移動 */
#define FNKSB       0x00c9          /* ファンクション・キーの表示がアクティブかどうかチェックし (FNKFLG)、アクティブなら表示、でなければ消す */
#define ERAFNK      0x00cc          /* ファンクション・キーの表示を消す */
#define DSPFNK      0x00cf          /* ファンクション・キーを表示 */
#define TOTEXT      0x00d2          /* 画面を強制的にテキストモードにする */
#define GTSTCK      0x00d5          /* ジョイスティックの状態を返す */
#define GTTRIG      0x00d8          /* トリガボタンの状態を返す */
#define GTPAD       0x00db          /* タッチパッドの状態を返す */
#define GTPDL       0x00de          /* パドルの値を返す */
#define TAPION      0x00e1          /* カセットのモーター ON の後、ヘッダ・ブロックを読む */
#define TAPIN       0x00e4          /* テープからデータを読む */
#define TAPIOF      0x00e7          /* テープからの読み込みをストップ */
#define TAPOON      0x00ea          /* カセットのモーター ON の後、ヘッダ・ブロックを書き込む */
#define TAPOUT      0x00ed          /* テープにデータを書き込む */
#define TAPOOF      0x00f0          /* テープへの書き込みをストップ */
#define STMOTR      0x00f3          /* カセットのモーターの動作設定 */
#define CHGCAP      0x0132          /* CAP ランプの状態を変える */
#define CHGSND      0x01d5          /* 1 ビット・サウンドポートの状態を変える */
#define RSLREG      0x0138          /* 基本スロット・レジスタに現在出力している内容を読む */
#define WSLREG      0x013b          /* 基本スロット・レジスタにデータを書き込む */
#define RDVDP       0x013e          /* VDP のステータス・レジスタを読む */
#define SNSMAT      0x0141          /* キーボード・マトリクスから指定した行の値を読む */
#define ISFLIO      0x014a          /* デバイスが動作中かどうかチェック */
#define OUTDLP      0x014d          /* プリンタ出力 */
#define KILBUF      0x0156          /* キーボード・バッファをクリア */
#define CALBAS      0x0159          /* BASIC インタープリタ内のルーチンをインタースロット・コール */
#define SUBROM      0x015c          /* SUB-ROM をインタースロット・コール */
#define EXTROM      0x015f          /* SUB-ROM をインタースロット・コール */
#define EOL         0x0168          /* 行の終わりまでデリート */
#define BIGFIL      0x016b          /* 機能的には FILVRM と同じ */
#define NSETRD      0x016e          /* VDP にアドレスをセットして、読み込める状態にする */
#define NSTWRT      0x0171          /* VDP にアドレスをセットして、書き込める状態にする */
#define NRDVRM      0x0174          /* VRAM の内容を読む */
#define NWRVRM      0x0177          /* VRAM にデータを書き込む */
// // SUB-ROM コールアドレス
// #define GRPPRT      0x0089          /* グラフィック画面に 1 文字出力 */
// #define NVBXLN      0x00c9          /* ボックスを描く */
// #define NVBXFL      0x00cd          /* 塗りつぶされたボックスを描く */
// #define CHGMOD      0x00d1          /* スクリーン・モードを変える */
// #define INITXT      0x00d5          /* 画面をテキスト・モード (40*24) にして初期化する */
// #define INIT32      0x00d9          /* 画面をテキスト・モード (32*24) にして初期化する */
// #define INIGRP      0x00dd          /* 画面を高解像グラフィック・モードにして初期化する */
// #define INIMLT      0x00e1          /* 画面を MULTI COLOR モードにして初期化する */
// #define SETTXT      0x00e5          /* VDP をテキスト・モード (40*24) にする */
// #define SETT32      0x00e9          /* VDP をテキスト・モード (32*24) にする */
// #define SETGRP      0x00ed          /* VDP を高解像度モードにする */
// #define SETMLT      0x00f1          /* VDP を MULTI COLOR モードにする */
// #define CLRSPR      0x00f5          /* すべてのスプライトを初期化 */
// #define CALPAT      0x00f9          /* スプライトジェネレータ・テーブルのアドレスを返す  */
// #define CALATR      0x00fd          /* スプライト属性テーブルのアドレスを返す */
// #define GSPSIZ      0x0101          /* 現在のスプライト・サイズを返す */
// #define GETPAT      0x0105          /* キャラクタ・パターンを返す */
// #define WRTVRM      0x0109          /* VRAM にデータを書き込む */
// #define RDVRM       0x010d          /* VRAM の内容を読む */
// #define CHGCLR      0x0111          /* 画面の色を変える */
// #define CLSSUB      0x0115          /* 画面クリア */
// #define DSPFNK      0x011d          /* ファンクション・キーの表示 */
// #define WRTVDP      0x012d          /* VDP のレジスタにデータを書き込む */
// #define VDPSTA      0x0131          /* VDP のレジスタを読む */
// #define SETPAG      0x013d          /* ページの切り替え */
// #define INIPLT      0x0141          /* パレットの初期化 */
// #define RSTPLT      0x0145          /* パレットを VRAM からリストアする */
// #define GETPLT      0x0149          /* パレットからカラーコードを得る */
// #define SETPLT      0x014d          /* カラーコードをパレットにセット */
// #define BEEP        0x017d          /* ブザーを鳴らす */
// #define PROMPT      0x0181          /* プロンプトを表示 */
// #define NEWPAD      0x01ad          /* マウス、ライトペンの状態を読む */
// #define CHGMDP      0x01b5          /* VDP のモードを変える */
// #define KNJPRT      0x01bd          /* グラフィック画面 (5～8) に漢字出力 */
// #define REDCLK      0x01f5          /* クロック・データを読む */
// #define WRTCLK      0x01f9          /* クロック・データを書き込む */
// ワークエリア
#define RDPRIM      0xf380          /* 基本スロットからの読み込み */
#define WRPRIM      0xf385          /* 基本スロットへ書き込み */
#define CLPRIM      0xf38c          /* 基本スロットコール */
#define USRTAB      0xf39a          /* USR関数のマシン語プログラム(0～9)の開始番地、機械語プログラム定義前の値はすべてエラールーチンFCERR(475AH)を指す */
#define LINL40      0xf3ae          /* SCREEN0のときの1行の幅(SCREEN0のときのWIDTH文により設定される) */
#define LINL32      0xf3af          /* SCREEN1のときの1行の幅(SCREEN1のときのWIDTH文により設定される) */
#define LINLEN      0xf3b0          /* 現在の画面の1行の幅 */
#define CRTCNT      0xf3b1          /* 現在の画面の行数 */
#define CLMLST      0xf3b2          /* PRINT命令において各項目がカンマで区切られている場合の横位置 */
#define TXTNAM      0xf3b3          /* パターンネーム・テーブル */
#define TXTCOL      0xf3b5          /* 使用せず*/
#define TXTCGP      0xf3b7          /* パターンジェネレータ・テーブル */
#define TXTATR      0xf3b9          /* 使用せず */
#define TXTPAT      0xf3bb          /* 使用せず */
#define T32NAM      0xf3bd          /* パターンネーム・テーブル */
#define T32COL      0xf3bf          /* カラーテーブル */
#define T32CGP      0xf3c1          /* パターンジェネレータ・テーブル */
#define T32ATR      0xf3c3          /* スプライトアトリビュート・テーブル */
#define T32PAT      0xf3c5          /* スプライト・ジェネレータ・テーブル */
#define GRPNAM      0xf3c7          /* パターンネーム・テーブル */
#define GRPCOL      0xf3c9          /* カラーテーブル */
#define GRPCGP      0xf3cb          /* パターンジェネレータ・テーブル */
#define GRPATR      0xf3cd          /* スプライトアトリビュート・テーブル */
#define GRPPAT      0xf3cf          /* スプライト・ジェネレータ・テーブル */
#define MLTNAM      0xf3d1          /* パターンネーム・テーブル */
#define MLTCOL      0xf3d3          /* 使用せず */
#define MLTCGP      0xf3d5          /* パターンジェネレータ・テーブル */
#define MLTATR      0xf3d7          /* スプライトアトリビュート・テーブル */
#define MLTPAT      0xf3d9          /* スプライト・ジェネレータ・テーブル */
#define CLIKSW      0xf3db          /* キークリックスイッチ(0=OFF、0以外=ON)。SCREEN文の<キークリックスイッチ>により設定される */
#define CSRY        0xf3dc          /* カーソルのY座標 */
#define CSRX        0xf3dd          /* カーソルのX座標 */
#define CNSDFG      0xf3de          /* ファンクションキー表示スイッチ(0=表示あり、0以外=表示なし)。KEY ON/OFF文によって設定される */
#define RG0SAV      0xf3df          /* VDPレジスタのセーブエリア*/
#define RG1SAV      0xf3e0
#define RG2SAV      0xf3e1
#define RG3SAV      0xf3e2
#define RG4SAV      0xf3e3
#define RG5SAV      0xf3e4
#define RG6SAV      0xf3e5
#define RG7SAV      0xf3e6
#define STATFL      0xf3e7          /* VDPのステータスを保存(MSX2ではステータスレジスタ0の内容) */
#define TRGFLG      0xf3e8          /* ジョイスティックのトリガボタンの状態を保存する */
#define FORCLR      0xf3e9          /* 前景色。COLOR文で設定される */
#define BAKCLR      0xf3ea          /* 背景色。COLOR文で設定される */
#define BDRCLR      0xf3eb          /* 周辺色。COLOR文で設定される */
#define MAXUPD      0xf3ec          /* CIRCLE文が内部で使用 */
#define MINUPD      0xf3ef          /* CIRCLE文が内部で使用 */
#define ATRBYT      0xf3f2          /* グラフィック使用時のカラーコード */
#define QUEUES      0xf3f3          /* PLAY文実行時のキューテーブルを指す */
#define FRCNEW      0xf3f5          /* BASICインタープリタが内部で使用する */
#define SCNCNT      0xf3f6          /* キースキャンの時間間隔 */
#define REPCNT      0xf3f7          /* キーのオートリピートが開始するまでの時間 */
#define PUTPNT      0xf3f8          /* キーバッファへの書き込みを行う番地を指す */
#define GETPNT      0xf3fa          /* キーバッファからの読み込みを行う番地を指す */
#define CS120       0xf3fc
#define LOW         0xf406          /* 現在のボーレートのビット0を表すLOWとHIGHの幅。SCREEN文の<カセットボーレート>により設定される */
#define HIGH        0xf408          /* 現在のボーレートのビット1を表すLOWとHIGHの幅。SCREEN文の<カセットボーレート>により設定される */
#define HEADER      0xf40a          /* 現在のボーレートのショートヘッダ用のヘッダビット(HEDLEN=2000)。SCREEN文の<カセットボーレート>により設定される */
#define ASPCT1      0xf40b          /* 256/アスペクト比。CIRCLE文で使用するためにSCREEN文で設定される */
#define ASPCT2      0xf40d          /* 256*アスペクト比。CIRCLE文で使用するためにSCREEN文で設定される */
#define ENDPRG      0xf40f          /* RESUME NEXT文のための仮のプログラムの終わり */
#define ERRFLG      0xf414          /* エラー番号を保存するためのエリア */
#define LPTPOS      0xf415          /* プリンタのヘッド位置 */
#define PRTFLG      0xf416          /* プリンタへ出力するかどうかのフラグ */
#define NTMSXP      0xf417          /* プリンタ種別(0=MSX用プリンタ、0以外=MSX用プリンタでない) */
#define RAWPRT      0xf418          /* raw-modeでプリント中なら0以外 */
#define VLZADR      0xf419          /* VAL関数で置き換えられる文字のアドレス */
#define VLZDAT      0xf41b          /* VAL関数で0に置き換わる文字 */
#define CURLIN      0xf41c          /* BASICが現在実行中の行番号 */
#define KBUF        0xf41f          /* クランチバッファ。BUF      =   0xf55EH)から中間言語に直されて入る */
#define BUFMIN      0xf55d          /* INPUT文で使われる */
#define BUF         0xf55e          /* タイプした文字が入るバッファ。ダイレクトステートメントがアスキーコードで入る */
#define ENDBUF      0xf660          /* BUF      =   0xf55EH)がオーバーフローするのを防ぐ */
#define TTYPOS      0xf661          /* BASICが内部で持つ仮想的なカーソル位置 */
#define DIMFLG      0xf662          /* BASICが内部で使用する */
#define VALTYP      0xf663          /* 変数の型の識別に使用する */
#define DORES       0xf664          /* 保存されている語がクランチできるかどうかを示す */
#define DONUM       0xf665          /* クランチ用のフラグ */
#define CONTXT      0xf666          /* CHRGETで使うテキストアドレスの保存 */
#define CONSAV      0xf668          /* CHRGETが呼ばれた後の定数のトークンを保存 */
#define CONTYP      0xf669          /* 保存した定数のタイプ */
#define CONLO       0xf66a          /* 保存した定数の値 */
#define MEMSIZ      0xf672          /* BASICが使用するメモリの最上位番地 */
#define STKTOP      0xf674          /* BASICがスタックとして使用する番地。CLEAR文により変化する */
#define TXTTAB      0xf676          /* BASICテキストエリアの先頭番地 */
#define TEMPPT      0xf678          /* テンポラリディスクリプタの空きエリアの先頭番地 */
#define TEMPST      0xf67a          /* NUMTEMP用の領域 */
#define DSCTMP      0xf698          /* ストリング関数の答えのストリングディスクリプタが入る */
#define FRETOP      0xf69b          /* 文字列領域の空きエリアの先頭番地 */
#define TEMP3       0xf69d          /* ガベージコレクションやUSR関数などに使われる */
#define TEMP8       0xf69f          /* ガベージコレクション用 */
#define ENDFOR      0xf6a1          /* FOR文の次の番地を保存する(ループ時にFOR文の次から実行するため) */
#define DATLIN      0xf6a3          /* READ文の実行により読まれたDATA文の行番号 */
#define SUBFLG      0xf6a5          /* USR関数などで配列を使うときのフラグ */
#define FLGINP      0xf6a6          /* INPUTやREADで使われるフラグ */
#define TEMP        0xf6a7          /* ステートメントコードのための一時保存場所。変数ポインタ、テキストアドレスなどに使用する */
#define PTRFLG      0xf6a9          /* 変換する行番号がなければ0、あれば0以外 */
#define AUTFLG      0xf6aa          /* AUTOコマンド有効、無効フラグ(0以外=有効中、0=無効中) */
#define AUTLIN      0xf6ab          /* 一番新しく入力された行番号 */
#define AUTINC      0xf6ad          /* AUTOコマンドの行番号の増分値 */
#define SAVTXT      0xf6af          /* 実行中のテキストのアドレスを保存する領域。主にRESUME文によりエラー回復で使用される */
#define SAVSTK      0xf6b1          /* スタックを保存する領域。主にエラーが起きたとき、エラー回復ルーチンがスタックをリストアするために使用される */
#define ERRLIN      0xf6b3          /* エラーが起きたときの行番号 */
#define DOT         0xf6b5          /* 何らかの形で画面に表示された、あるいは入力された最新の行番号 */
#define ERRTXT      0xf6b7          /* エラーが起きたテキストのアドレス。主にRESUME文によるエラー回復で使用される */
#define ONELIN      0xf6b9          /* エラーが起きたときの飛び先行のテキストアドレス。ON ERROR GOTO文により設定される */
#define ONEFLG      0xf6bb          /* エラールーチンの実行中を示すフラグ。(0以外=実行中、0=実行中でない) */
#define TEMP2       0xf6bc          /* 一時保存用 */
#define OLDLIN      0xf6be          /* Ctrl+STOP、STOP命令、END命令で中断されたか、あるいは最後に実行された行番号 */
#define OLDTXT      0xf6c0          /* 次に実行する文のテキストアドレス */
#define VARTAB      0xf6c2          /* 単純変数の開始番地。NEW文を実行すると〔TXTTAB      =   0xf676H)の内容+2〕が設定される */
#define ARYTAB      0xf6c4          /* 配列テーブルの開始番地 */
#define STREND      0xf6c6          /* テキストエリアや変数エリアとして使用中であるメモリの最後の番地 */
#define DATPTR      0xf6c8          /* READ文の実行により読まれたデータのテキストアドレス */
#define DEFTBL      0xf6ca          /* 英文字1字に対し変数の型を保持するエリア。CLEAR、DEFSTR、!、#などの型宣言で変化する */
#define PRMSTK      0xf6e4          /* スタック上の以前の定義ブロック(ガベージコレクション用) */
#define PRMLEN      0xf6e6          /* 処理対象のテーブルのバイト数 */
#define PARM1       0xf6e8          /* 処理対象のパラメータ定義テーブル。PRMSIZは定義ブロックのバイト数で初期値は100 */
#define PRMPRV      0xf74c          /* 以前のパラメータブロックのポインタ(ガベージコレクション用) */
#define PRMLN2      0xf74e          /* パラメータブロックの大きさ */
#define PARM2       0xf750          /* パラメータの保存用 */
#define PRMFLG      0xf7b4          /* PARM1がサーチ済みかどうかを示すフラグ */
#define ARYTA2      0xf7b5          /* サーチの終点 */
#define NOFUNS      0xf7b7          /* 処理対象関数がない場合は0 */
#define TEMP9       0xf7b8          /* ガベージコレクション用の一時保存場所 */
#define FUNACT      0xf7ba          /* 処理対象関数の数 */
#define SWPTMP      0xf7bc          /* SWAP文の最初の変数の値の一時保存場所 */
#define TRCFLG      0xf7c4          /* トレースフラグ。(0以外=TRACE ON、0=TRACE OFF) */
#define FBUFFR      0xf7c5          /* マスパックが内部で使用する */
#define DECTMP      0xf7f0          /* 10進整数を不動小数点数にするときに使用する */
#define DECTM2      0xf7f2          /* 除算ルーチンの実行時に使用する */
#define DECCNT      0xf7f4          /* 除算ルーチンの実行時に使用する */
#define DAC         0xf7f6          /* 演算の対象となる値を設定するエリア */
#define HOLD8       0xf806          /* 10進数の乗算のためのレジスタ保存エリア */
#define HOLD2       0xf836          /* マスパックが内部で使用する */
#define HOLD        0xf83e          /* マスパックが内部で使用する */
#define ARG         0xf847          /* DAC      =   0xf7F6H)との演算対象となる値を設定するエリア */
#define RNDX        0xf857          /* 最新の乱数を倍精度実数で保存する。RND関数で設定される */
#define MAXFIL      0xf85f          /* ファイル番号の最大値。MAXFILES文により設定される */
#define FILTAB      0xf860          /* ファイルデータエリアの先頭番地 */
#define NULBUF      0xf862          /* SAVE、LOADでBASICインタープリタが使用するバッファ */
#define PTRFIL      0xf864          /* アクセス中のファイルのファイルデータがある番地 */
#define RUNFLG      0xf866          /* プログラムをロード後実行するなら0でない値。LOAD文のRオプションなどで使用する */
#define FILNAM      0xf866          /* ファイル名の保存エリア */
#define FILNM2      0xf871          /* ファイル名の保存エリア */
#define NLONLY      0xf87c          /* プログラムロード中は0でない値となる */
#define SAVEND      0xf87d          /* セーブするマシン語プログラムの最終番地 */
#define FNKSTR      0xf87f          /* ファンクションキーの文字列保存エリア(16文字×10) */
#define CGPNT       0xf91f          /* ROM上の文字フォント格納アドレス */
#define NAMBAS      0xf922          /* 現在のパターンネーム・テーブルのベース番地 */
#define CGPBAS      0xf924          /* 現在のパターン・ジェネレーター・テーブルのベース番地 */
#define PATBAS      0xf926          /* 現在のスプライト・ジェネレーター・テーブルのベース番地 */
#define ATRBAS      0xf928          /* 現在のスプライトアトリビュート・テーブルのベース番地 */
#define CLOC        0xf92a          /* グラフィックルーチンが内部で使用する */
#define CMASK       0xf92c          /* グラフィックルーチンが内部で使用する */
#define MINDEL      0xf92d          /* グラフィックルーチンが内部で使用する */
#define MAXDEL      0xf92f          /* グラフィックルーチンが内部で使用する */
#define ASPECT      0xf931          /* 円の縦横の比率。CLRCLE文の<比率>により設定される */
#define CENCNT      0xf933          /* CIRCLE文が内部で使用する */
#define CLINEF      0xf935          /* 円の中心へ線を引くかどうかのフラグ。CIRCLE文の<角度>で指定 */
#define CNPNTS      0xf936          /* プロットする点 */
#define CPLOTF      0xf938          /* CIRCLE文が内部で使用する */
#define CPCNT       0xf939          /* 円の1/8分割の数 */
#define CPCNT8      0xf93b          /* CIRCLE文が内部で使用する */
#define CRCSUM      0xf93d          /* CIRCLE文が内部で使用する */
#define CSTCNT      0xf93f          /* CIRCLE文が内部で使用する */
#define CSCLXY      0xf941          /* xとyのスケール */
#define CSAVEA      0xf942          /* ADVGRPの保存エリア */
#define CASVEM      0xf944          /* ADVGRPの保存エリア */
#define CXOFF       0xf945          /* 中心からのxのオフセット */
#define CYOFF       0xf947          /* 中心からのyのオフセット */
#define LOHMSK      0xf949          /* PAINT文が内部で使用する */
#define LOHDIR      0xf94a          /* PAINT文が内部で使用する */
#define LOHADR      0xf94b          /* PAINT文が内部で使用する */
#define LOHCNT      0xf94d          /* PAINT文が内部で使用する */
#define SKPCNT      0xf94f          /* スキップカウント */
#define MIVCNT      0xf951          /* 移動カウント */
#define PDIREC      0xf953          /* ペイントの方向 */
#define LFPROG      0xf954          /* PAINT文が内部で使用する */
#define RTPROG      0xf955          /* PAINT文が内部で使用する */
#define MCLTAB      0xf956          /* PLAYマクロ、あるいはDROWマクロのテーブルの先頭を指す */
#define MCLFLG      0xf958          /* PLAY/DRAWの指示 */
#define QUETAB      0xf959          /* キューテーブル */
#define QUEBAK      0xf971          /* BCKQで使用する */
#define VOICAQ      0xf975          /* 音声1のキュー(1=a) */
#define VOICBQ      0xf9f5          /* 音声2のキュー(2=b) */
#define VOICCQ      0xfa75          /* 音声3のキュー(3=c) */
#define DFPAGE      0xfaf5          /* ディスプレイページ番号 */
#define ACPAGE      0xfaf6          /* アクティブページ番号 */
#define AVCSAV      0xfaf7          /* AVコントロールポートの保存 */
#define EXBRSA      0xfaf8          /* SUM-ROMのスロットアドレス */
#define CHRCNT      0xfaf9          /* バッファ中のキャラクタのカウンタ。ローマ字カナ変換で使用(値は0<=n<=2) */
#define ROMA        0xfafa          /* バッファ中のキャラクタを入れておくエリア。ローマ字カナ変換で使用 */
#define MODE        0xfafc          /* ローマ字カナ変換のモードスイッチとVRAMサイズ */
#define NORUSE      0xfafd          /* 未使用 */
#define XSAVE       0xfafe          /* 〔ＩＯＯＯＯＯＯＯ ＸＸＸＸＸＸＸＸ〕 */
#define YSAVE       0xfb00          /* 〔×ＯＯＯＯＯＯＯ ＹＹＹＹＹＹＹＹ〕 */
#define LOGOPR      0xfb02          /* ロジカル・オペレーション・コード */
#define RSTMP       0xfb03          /* RS-232Cまたはディスクのワークエリア */
#define TOCNT       0xfb03          /* RS-2332Cルーチンが内部で使用する */
#define RSFCB       0xfb04          /* RS-232Cのアドレス*/
#define RSIQLN      0xfb06          /* RS-232Cルーチンが内部で使用する */
#define MEXBIH      0xfb07
#define OLDSTT      0xfb0c
#define OLDINT      0xfb12
#define DEVNUM      0xfb17          /* RS-232Cルーチンが内部で使用する */
#define DATCNT      0xfb18
#define ERRORS      0xfb1b          /* RS-232Cルーチンが内部で使用する */
#define FLAGS       0xfb1b          /* RS-232Cルーチンが内部で使用する */
#define ESTBLS      0xfb1d          /* RS-232Cルーチンが内部で使用する */
#define COMMSK      0xfb1e          /* RS-232Cルーチンが内部で使用する */
#define LSTCOM      0xfb1f          /* RS-232Cルーチンが内部で使用する */
#define LSTMOD      0xfb20          /* RS-232Cルーチンが内部で使用する */
#define PRSCNT      0xfb35          /* D1～D0 文字列パース／D7=0 1パス */
#define SAVSP       0xfb36          /* プレー中のスタックポインタを保存 */
#define VOICEN      0xfb38          /* 解釈中の現在の音声 */
#define SAVVOL      0xfb39          /* 休止のために音量を保存する */
#define MCLLEN      0xfb39          /* PLAY文が内部で使用する */
#define MCLPTR      0xfb3c          /* PLAY文が内部で使用する */
#define QUEUEN      0xfb3e          /* PLAY文が内部で使用する */
#define MUSICF      0xfc3f          /* 音楽演奏用の割り込みフラグ */
#define PLYCNT      0xfb40          /* キューに格納されているPLAY文の数 */
#define VCBA        0xfb41          /* 音声0のスタティックデータ */
#define VCBB        0xfb66          /* 音声1のスタティックデータ */
#define VCBC        0xfb8b          /* 音声2のスタティックデータ */
#define ENSTOP      0xfbb0          /* [SHIFT+Ctrl+GRAPH+かなキー]によるウォームスタートを可能にするフラグ(0=不可能、0以外=可能) */
#define BASROM      0xfbb1          /* BASICテキストの存在場所を示す(0=RAM上、0以外=ROM上) */
#define LINTTB      0xfbb2          /* ラインターミナルテーブル。テキスト画面の各行の情報を保持するエリア */
#define FSTPOS      0xfbca          /* BIOSのINLIN(00B1H)で入力した行の最初の文字の位置 */
#define CODSAV      0xfbcc          /* カーソルが重なった部分のキャラクタを保存するエリア */
#define FNKSWI      0xfbcd          /* KEY ON時にどのファンクションキーが表示されているか表す(1=F1～F5が表示、0=F6～F10が表示) */
#define FNKFLG      0xfbce          /* ON KEY GOSUB文により定義された行の実行を許可、禁止、停止するかファンクションキーごとに保存するためのエリア。KEY(n)ON/OFF/STOP文により設定される(0=KEY(n)OFF/STOP、1=KEY(n)ON) */
#define ONGSBF      0xfbd8          /* TRPTBL      =   0xfc4CH)で待機中のイベントが発生したかどうかのフラグ */
#define CLIKFL      0xfbd9          /* キークリック・フラグ */
#define OLDKEY      0xfbda          /* キーマトリクスの状態(旧) */
#define NEWKEY      0xfbe5          /* キーマトリクスの状態(新) */
#define KEYBUF      0xfbf0          /* キーコードバッファ */
#define LINWRK      0xfc18          /* スクリーンハンドラが使う一時保存場所 */
#define PATWRK      0xfc40          /* パターンコンバータが使う一時保存場所 */
#define BOTTOM      0xfc48          /* 実装したRAMの先頭(低位)番地。MSX2では通常8000H */
#define HIMEM       0xfc4a          /* 利用可能なメモリーの上位番地。CLEAR文の<メモリ上限>により設定される */
#define TRPTBL      0xfc4c          /* 割り込み処理で使うトラップテーブル。ひとつのテーブルは3バイトで構成される1バイト目がON/OFF/STOP状態を表し、残りが分岐先のテキストアドレスを表す */
#define RTYCNT      0xfc9a          /* BASICが内部で使用する */
#define INTFLG      0xfc9b          /* Ctrl+STOPが押された場合など、ここに03Hを入れることによりストップする */
#define PADY        0xfc9c          /* パドルのY座標 */
#define PADX        0xfc9d          /* パドルのX座標 */
#define JIFFY       0xfc9e          /* PLAY文が内部で使用する */
#define INTVAL      0xfca0          /* インターバルの間隔。ON INTERVAL GOSUB文により設定される */
#define INTCNT      0xfca2          /* インターバルのためのカウンタ */
#define LOWLIM      0xfca4          /* カセットテープからの読み込み中に使う */
#define WINWID      0xfca5          /* カセットテープからの読み込み中に使う */
#define GRPHED      0xfca6          /* グラフィックキャラクタを出す時のフラグ(1=グラフィックキャラクタ、0=通常の文字) */
#define ESCCNT      0xfca7          /* エスケープコードがきてから何文字目かをカウントするエリア */
#define INSFLG      0xfca8          /* 挿入モードのフラグ(0=通常モード、0以外=挿入モード) */
#define CSRSW       0xfca9          /* カーソル表示の有無(0=表示なし、0以外=表示あり) */
#define CSTYLE      0xfcaa          /* カーソルの形(0=■、0以外=_) */
#define CAPST       0xfcab          /* CAPSキーの状態(0=CAP OFF、0以外=CAP ON) */
#define KANAST      0xfcac          /* かなキーの状態(0=かなOFF、0以外=かなON) */
#define KANAMD      0xfcad          /* かなキー配列の状態(0=50音配列、0以外=JIS配列) */
#define FLBMEM      0xfcae          /* BASICプログラムをロード中は0 */
#define SCRMOD      0xfcaf          /* 現在のスクリーンモードの番号 */
#define OLDSCR      0xfcb0          /* スクリーンモード保存エリア */
#define CASPRV      0xfcb1          /* CAS:が使う文字保存場所 */
#define BRDATR      0xfcb2          /* PAINTで使用する境界色のカラーコード。PAINT文の<境界色>で指定される */
#define GXPOS       0xfcb3          /* X座標 */
#define GYPOS       0xfcb5          /* Y座標 */
#define GRPACX      0xfcb7          /* グラフィックアキュムレータ(X座標) */
#define GRPACY      0xfcb9          /* グラフィックアキュムレータ(Y座標) */
#define DRWFLG      0xfcbb          /* DRAW文で使用するフラグ */
#define DRWSCL      0xfcbc          /* DRAWスケーリングファクタ(0=スケーリングしない、0以外=する) */
#define DRWANG      0xfcbd          /* DRAWするときの角度 */
#define RUNBNF      0xfcbe          /* BLOAD中、BSAVE中、どちらでもない、のいずれかを表すフラグ */
#define SAVENT      0xfcbf          /* BSAVEの開始番地 */
#define EXPTBL      0xfcc1          /* 拡張スロット用のフラグテーブル。各スロットの拡張の有無 */
#define SLTTBL      0xfcc5          /* 各拡張スロットレジスタ用の、現在のスロット選択状況 */
#define SLTATR      0xfcc9          /* 各スロット用に属性を保存する */
#define SLTWRK      0xfd09          /* 各スロット用に特定のワークエリアを確保する */
#define PROCNM      0xfd89          /* 拡張ステートメント(CALL文の後)、拡張デバイス(OPENの後)の名前が入る。0は終わり */
#define DEVICE      0xfd99          /* カートリッジ用の装置識別に使用する */
#define H_KEYI      0xfd9a          /* MSXIO割り込み処理の始め */
#define H_TIMI      0xfd9f          /* MSXIOタイマ割り込み処理 */
#define H_CHPH      0xfda4          /* MSXIO CHPUT(1文字出力)の始め */
#define H_DSPC      0xfda9          /* MSXIO DSPCSR(カーソル表示)の始め */
#define H_ERAC      0xfdae          /* MSXIO ERACSR(カーソル消去)の始め */
#define H_DSPF      0xfdb3          /* MSXIO DSPFNK(ファンクションキー表示)の始め */
#define H_ERAF      0xfdb8          /* MSXIO ERAFNK(ファンクションキー消去)の始め */
#define H_TOTE      0xfdbd          /* MSXIO TOTEXT(画面をテキストモードにする)の始め */
#define H_CHGE      0xfdc2          /* MSXIO CHGET(1文字取り出し)の始め */
#define H_INIP      0xfdc7          /* MSXIO INIPAT(文字パターンの初期化)の始め */
#define H_KEYC      0xfdcc          /* MSXIO KEYCOD(キーコード変換)の始め */
#define H_KYEA      0xfdd1          /* MSXIO NMIルーチン(Key Easy)の始め */
#define H_NMI       0xfdd6          /* MSXIO NMI(ノンマスカブルインタラプト)の始め */
#define H_PINL      0xfddb          /* MSXINL PINLIN(1行入力)の始め */
#define H_QINL      0xfde0          /* MSXINL QINLIN(”?”を表示して1行入力)の始め */
#define H_INLI      0xfde5          /* MSXINL INLIN(1行入力)の始め */
#define H_ONGO      0xfdea          /* MSXSTS INGOTP(ON GOTO)の始め */
#define H_DSKO      0xfdef          /* MSXSTS DSKO$(ディスク出力)の始め */
#define H_SETS      0xfdf4          /* MSXSTS SETS(セット アトリビュート)の始め */
#define H_NAME      0xfdf9          /* MSXSTS NAME(リネーム)の始め */
#define H_KILL      0xfdfe          /* MSXSTS KILL(ファイルの削除)の始め */
#define H_IPL       0xfe03          /* MSXSTS IPL(初期プログラムのロード)の始め */
#define H_COPY      0xfe08          /* MSXSTS COPY(ファイルのコピー)の始め */
#define H_CMD       0xfe0d          /* MSXSTS CMD(拡張コマンド)の始め */
#define H_DSKF      0xfe12          /* MSXSTS DSKF(ディスクの空き)の始め */
#define H_DSKI      0xfe17          /* MSXSTS DSKI(ディスク入力)の始め */
#define H_ATTR      0xfe1c          /* MSXSTS ATTR$(アトリビュート)の始め */
#define H_LSET      0xfe21          /* MSXSTS LSET(左詰め代入)の始め */
#define H_RSET      0xfe26          /* MSXSTS RSET(左詰め代入)の始め */
#define H_FIEL      0xde2b          /* MSXSTS FIELD(フィールド)の始め */
#define H_MKI$      0xfe30          /* MSXSTS MKI$(整数作成)の始め */
#define H_MKS$      0xfe35          /* MSXSTS MKS$(単精度実数作成)の始め */
#define H_MKD$      0xfe3a          /* MSXSTS MKD$(倍精度実数作成)の始め */
#define H_CVI       0xfe3f          /* MSXSTS CVI(整数変換)の始め */
#define H_CVS       0xfe44          /* MSXSTS CVS(単精度実数変換)の始め */
#define H_CVD       0xfe49          /* MSXSTS CVD(倍精度実数変換)の始め */
#define H_GETP      0xfe4e          /* SPDSK GETPTR(ファイルポインタ取り出し) */
#define H_SETF      0xfe53          /* SPCDSK SETFIL(ファイルポインタ設定) */
#define H_NOFO      0xfe58          /* SPDSK NOFOR(OPEN文にFORがない) */
#define H_NULO      0xfe5d          /* SPCDSK NULOPN(空きファイルをオープン) */
#define H_NTFL      0xfe62          /* SPCDSK NTFLO(ファイル番号が0でない) */
#define H_MERG      0xfe67          /* SPCDSK MERGE(プログラムファイルのマージ) */
#define H_SAVE      0xfe6c          /* SPCDSK SAVE(セーブ) */
#define H_BINS      0xfe71          /* SPCDSK BINSAV(機械語セーブ) */
#define H_BINL      0xfe76          /* SPCDSK BINLOD(機械語ロード) */
#define H_FILE      0xfd7b          /* SPCDSK FILES(ファイル名の表示) */
#define H_DGET      0xfe80          /* SPCDSK DGET(ディスクGET) */
#define H_FILO      0xfe85          /* SPCDSK FILOU1(ファイル出力) */
#define H_INDS      0xfe8a          /* SPCDSK INDSKC(ディスクの属性を入力) */
#define H_RSLF      0xfe8f          /* SPCDSK 前のドライブを再び選択する */
#define H_SAVD      0xfe94          /* SPCDSK 現在選択しているドライブを保存する */
#define H_LOC       0xfe99          /* SPCDSK LOC関数(場所を示す) */
#define H_LOF       0xfe9e          /* SPCDSK LOF関数(ファイルの長さ) */
#define H_EOF       0xfea3          /* SPCDSK EOF関数(ファイルの終わり) */
#define H_FPOS      0xfea8          /* SPCDSK FPOS関数(ファイルの場所) */
#define H_BAKU      0xfead          /* SPCDSK BAKUPT(バックアップ) */
#define H_PARD      0xfeb2          /* SPCDEV PARDEV(装置名の取り出し) */
#define H_NODE      0xfeb7          /* SPCDEV NODEVN(装置名なし) */
#define H_POSD      0xfebc          /* SPCDEV POSDSK */
#define H_DEVN      0xfec1          /* SPCDEV DEVNAM(装置名の処理) */
#define H_GEND      0xfec6          /* SPCDEV GENDSP(装置割り当て) */
#define H_RUNC      0xfecb          /* BIMISC RUNC(RUNのためのクリア) */
#define H_CLEA      0xfed0          /* BIMISC CLEARC(CLEAR文のためのクリア) */
#define H_LOPD      0xfed5          /* BIMISC LOPDFT(繰り返しと省略値の設定) */
#define H_STKE      0xfeda          /* BIMISC STKERR(スタックエラー) */
#define H_ISFL      0xfedf          /* BIMISC ISFLIO(ファイルの入出力かどうか) */
#define H_OUTD      0xfee4          /* BIO OUTDO(OUTを実行) */
#define H_CRDO      0xfee9          /* BIO CRDO(CRLFを実行) */
#define H_DSKC      0xfeee          /* BIO DSKCHI(ディスクの属性を入力) */
#define H_DOGR      0xfef3          /* GENGRP DOGRPH(グラフィック処理を実行) */
#define H_PRGE      0xfef8          /* BINTRP PRGEND(プログラム終了) */
#define H_ERRP      0xfefd          /* BINTRP ERRPRT(エラー表示) */
#define H_ERRF      0xff02          /* BINTRP */
#define H_READ      0xff07          /* BINTRP READY */
#define H_MAIN      0xff0c          /* BINTRP MAIN */
#define H_DIRD      0xff11          /* BINTRP DIRDO(ダイレクトステートメント実行) */
#define H_FINI      0xff16          /* BINTRP */
#define H_FINE      0xff1b          /* BINTRP */
#define H_CRUN      0xff20          /* BINTRP */
#define H_CRUS      0xff25          /* BINTRP */
#define H_ISRE      0xff2a          /* BINTRP */
#define H_NTFN      0xff2f          /* BINTRP */
#define H_NOTR      0xff34          /* BINTRP */
#define H_SNGF      0xff39          /* BINTRP */
#define H_NEWS      0xff3e          /* BINTRP */
#define H_GONE      0xff43          /* BINTRP */
#define H_CHRG      0xff48          /* BINTRP */
#define H_RETU      0xff4d          /* BINTRP */
#define H_PRTF      0xff52          /* BINTRP */
#define H_COMP      0xff57          /* BINTRP */
#define H_FINP      0xff5c          /* BINTRP */
#define H_TRMN      0xff61          /* BINTRP */
#define H_FRME      0xff66          /* BINTRP */
#define H_NTPL      0xff6b          /* BINTRP */
#define H_EVAL      0xff70          /* BINTRP */
#define H_OKNO      0xff75          /* BINTRP */
#define H_FING      0xff7a          /* BINTRP */
#define H_ISMI      0xff7f          /* BINTRP ISMID$(MID$かどうか) */
#define H_WIDT      0xff84          /* BINTRP WIDTHS(WIDTH) */
#define H_LIST      0xff89          /* BINTRP LIST */
#define H_BUFL      0xff8e          /* BINTRP BUFLIN(バッファライン) */
#define H_FRQI      0xff93          /* BINTRP FRQINT */
#define H_SCNE      0xff98          /* BINTRP */
#define H_FRET      0xff9d          /* BISTRS FRETMP */
#define H_PTRG      0xffa2          /* BIPTRG PTRGET(ポインタ取り出し) */
#define H_PHYD      0xffa7          /* MSXIO PHYDIO(物理ディスク入出力) */
#define H_FORM      0xffac          /* MSXIO FORMAT(ディスクをフォーマットする) */
#define H_ERRO      0xffb1          /* BINTRP ERROR */
#define H_LPTO      0xffb6          /* MSXIO LPTOUT(プリンタ出力) */
#define H_LPTS      0xffbb          /* MSXIO LPTSTT(プリンタの状態) */
#define H_SCRE      0xffc0          /* MSXSTS SCREEN文の入口 */
#define H_PLAY      0xffc5          /* MSXSTS PLAY文の入口 */
#define FCALL       0xffca          /* 拡張BIOSが使用するフック */
#define DISINT      0xffcf          /* DOSが使用する */
#define ENAINT      0xffd4          /* DOSが使用する */
