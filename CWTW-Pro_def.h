﻿//---------------------------------------------------------------------------
#ifndef CWTWPro_defH
#define CWTWPro_defH
//---------------------------------------------------------------------------
#define	DSound_Use			1

#define	REGISTORY_BASE		"Software\\CWTW"
#define	REGISTORY_BASE_ENV	"Software\\CWTW\\Environ"
//#define	REGISTORY_BASE_KEY	"Software\\CWTW\\LICENSE"	// コメントで未使用
//---------------------------------------------------------------------------
static String ProTitle = "CWTW-Pro";				// 送信あり版タイトル
static String FreeTitle= "CWTW";					// 受信のみ版タイトル
static String Revcode  = "Rev3.00";					// バージョンコード
static String Revdate  = "2020/02/05";

#define	LICENSE_FNAME	"License.txt"
#define	DOCUMENT_FNAME	"CWTW-Pro.txt"

#define	Progname		"Continuous Wave Training program for Windows."
#define	Cpymsg			"Copyright(C) 1997-2020 JR4QPV(T.Yokobayashi)"
#define	PassWord 		"CWTWHIRAKEGOMA"			// 登録パスワード
#define HomePageURL		"https://www.yokoweb.net/dokuwiki/develop/cwtw-pro/"
//---------------------------------------------------------------------------
#endif

#if 0
;!【CWTW-Pro 改版履歴】
;!《Rev1.00 1997/07/25》  …CWTWとして作成
;!《Rev1.10 1997/08/19》
;! ・BEEP音のサポート。
;! ・キー入力受信練習モードのサポート。
;!《Rev1.11 1997/08/23》
;! ・ｼﾞｮｲｽﾃｨｯｸが未接続の時にエラー発生するバグ修正(^^;)。
;!  （情報を頂いた JH3HOA ｸﾏｺﾞﾛｳさん TU）
;!《Rev2.00 1997/12/14》  …CWTW-Proと名称変更してシェアウェア化
;! ・シェアウェア化に伴う送金登録関係の追加。
;! ・送信練習モードのサポート。　　
;! ・練習モードの保存機能の追加。
;! ・小文字表示，符号表示の機能追加。
;! ・表示文字のフォント指定の機能追加。
;! ・環境設定情報は全てﾚｼﾞｽﾄﾘに保存するように変更。
;! ・経過時間を表示する機能の追加。
;! ・受信練習モードのデフォルトを乱数暗文に変更。
;! ・練習停止せずに終了した時、スレッドを停止するよう処理見直し。
;! ・最大化した時のｳｨﾝﾄﾞｳｻｲｽﾞを制限する処理を削除。
;!《Rev2.10 1998/08/17》
;! ・デフォルトの表示フォントを「ＭＳゴシック」に変更。
;! ・練習モード保存時は、メインフォームの位置を記憶する機能追加。
;! ・Dirext SoundによるWAVE音源に対応。
;! ・受信練習に行ランダムのモードを追加。
;! ・最小化して復帰した時にﾒｲﾝ画面の速度設定が初期値に戻る不具合修正。
;! ・ツールバーのサポート等、画面レイアウトの見直し。
;! ・ファイル編集でメモ帳を呼び出す機能を追加。
;!《Rev2.20 1999/06/06》
;! ・ウィンドウのサイズ変更に対応できるようにメイン画面レイアウトの見直し。
;! ・和文の'(',')'の符号を発音しなかった不具合の修正。それに伴い、
;!   受信練習モードでの、[ﾎﾚ],[ﾗﾀ]での和文･欧文の自動切替えをサポート。
;! ・キーボード入力練習を拡張し、キーボードメモリ機能を追加。
;!《Rev2.30 2003/10/17》
;! ・C++ Builder5でリコンパイル。
;! ・NEC PC-98は動作確認が困難になってきたので、正式な動作対象から外した。
;!　（でも、たぶんまだ動くと思う）
;! ・GIVEIO.SYSに対応し、NT系OS(Windows2000/XP)でBEEP音が鳴らせるように対応。
;! ・受信練習中に速度変更した時に、STOPが効かなくなる不具合の回避。
;! ・テキストファイル中の";|"以降の文字をコメントとしてを画面表示する機能追加。
;! ・ﾌｧｲﾙ編集や閲覧操作をするとｶﾚﾝﾄﾃﾞｨﾚｸﾄﾘが移動して、以降のﾌｧｲﾙ編集などがで
;! 　きなくなる不具合の修正。(Env->DataDirがフルパスでセットされてなかった）
;! ・スタートとストップボタンのアイコン変更。
;! ・ヘルプメニューに「ホームページ表示」機能の追加。
;!《Rev2.40 2003/11/27》
;! ・和文モードで'ヰ'と'ヱ'の文字対応。（'`'と'~'文字で代替え記述）
;! ・練習画面をRichEdit→Memoコンポーネントに変更。
;! ・練習画面ファイル保存と練習画面クリア機能の追加。
;!《Rev2.50 2004/04/30》
;! ・[ｳｳ]を判定しない不具合修正(符号テーブルが誤り0x0696→0x0596)
;! ・ENTERとESCキーで開始／停止ができるように修正。
;! （ｷｰﾎﾞｰﾄﾞ入力練習で、入力した文字が２回表示されてた不具合回避）
;! ・コメントメッセージ行を表示する／しないモードを環境設定で選択できる
;! 　ように機能追加。
;! ・[記号含む]指定の追加。
;! ・フリー版CWTWとのソースの共通化(バージョンコードも共通)。
;! ・表示文字数をステータスバーに表示するようにした。
;! ・CWTW-Proで受信練習のみはフリーで利用できるように、ライセンス確認ダイアログ
;! 　は送信練習を選択した時に表示するようにした。
;!《Rev2.51 2004/05/14》
;! ・環境設定の音程設定で18Hz以下は設定できないようにした。(16Hzはｴﾗｰ発生する)
;! ・乱数暗文で濁点(゛)と半濁点(゜)を付加する文字を判別するようにした。
;! ・ｲﾝｽﾄｰﾙ時と実行時の日付ﾌｫｰﾏｯﾄ(ｺﾝﾄﾛｰﾙﾊﾟﾈﾙで設定できる)が異なる時に、
;! 　TDateTimeへの変換で例外が発生してプログラムが終了する不具合の回避。
;! ・乱数暗文時(欧文）の区切り文字数を環境設定で指定できるようにした。
;!《Rev2.52 2004/05/28》
;! ・乱数暗文での濁点(゛)と半濁点(゜)文字の乱数発生の不具合修正。
;! ・環境設定に"受信設定"のタブを追加し、練習時間設定と改行時間設定を追加。
;! ・乱数暗文(欧文)で区切ランダムモードの機能を追加。
;! ・乱数暗文で、和文区切文字指定の機能を追加。
;! ・一時停止ボタンの機能を追加。
;! ・ファイルランダムで、ファイル中に記述のファイル名が、パスを含まない
;!  （フルパスでない）時は、そのランダムファイルのあるフォルダのファイルを
;!   検索するように変更。
;! ・ステータスバーのファイル名表示は、枠を超える時は短縮表示するようにした。
;! ・RS232CのCOMポートのリスト値にCOM4～8の定義を追加。
;! ・環境設定に"フォルダ"のタブを追加し、データディレクトリの指定をＧＵＩで
;! 　設定できるようにした。
;!《Rev2.53 2005/01/04》
;! ・新追加符号'@'(･--･-･)に対応。（2004/05/03交付）
;! ・環境設定に「ｳｨﾝﾄﾞｳ位置/ｻｲｽﾞ保存」のモードを追加。
;!　(SHIFT+CTRLを押しながら起動した時には、ﾃﾞﾌｫﾙﾄの位置/ｻｲｽﾞで起動)
;!《Rev2.54 2005/02/15》
;! ・環境設定のフォントの太字と斜体の設定を保存できるようにした。
;! ・受信練習のリピート機能を追加。
;! ・ﾗｲﾝﾗﾝﾀﾞﾑ,ﾜｰﾄﾞﾗﾝﾀﾞﾑ,ﾌｧｲﾙﾗﾝﾀﾞﾑの乱数ｱﾙｺﾞﾘｽﾞﾑの改良。（1024個以下の時には、
;! 　全てが１度づつ重複せずに網羅して出現するようにした）
;!《Rev2.55 2005/02/22》
;! ・ENTERキーで練習開始した時に、「読み込み違反」エラーが発生する不具合の修正。
;! ・送信練習モード時、[カタカナ]キーで"カナモード"に、[英数]キーで"英数モード"
;! 　に切り換えるようにした。
;! ・送信練習モード時、[SHIFT]キーの代わりに[ALT]キーも使えるようにした。
;! ・データファイル中の全角文字を、非完全ながら判読できるようにした。
;!　（濁点及び半濁点文字が未対応）
;!《Rev2.56 2006/02/19》
;! ・RS通信ポートのオープンと同時に、RTS信号をOFFにする処理を追加した。
;!　 （送信練習開始時に、RTS信号が一瞬出力される不具合の軽減）
;! ・送信練習時のキー入力がない時の自動改行の動作モード（なし,短,中,長）の
;!　 指定を環境設定に追加した。（従来の動きは｢短｣で同等）
;!-- CWTW-Pro open source version --------------------------------------
;!《Rev3.00 2020/02/05》
;! ・コンパイル環境を「C++Builder Community Edition 10.3」に移行しソース公開。
;! ・内部処理言語コードをUnicodeへ変更対応。
;! ・入力ファイルの文字コードはUTF-8(BOM付)に変更。（ヰヱ文字記述にも対応）
;!
#endif
