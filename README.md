﻿CWTW-Pro Open source version
================================
Continuous Wave Training program for Windows.

概要
-----
「CWTW-Pro」はモールス符号の練習用のWindowsソフトウェア。  
このRev3.xからオープンソースソフトウェアとして公開し全機能フリーで利用できるようになりました。

CWTW-Pro Rev2.xの初版は1997年にRev2.00を公開。受信練習機能はフリーで利用でき送信練習機能はシェアウェアの扱いでした。Windows10でもなんとか動作していたので2006年のRev2.56から暫く更新してませんでしたが、2020年1月には送信機能解除キーワードを公開し（ダウンロードページ）フリーソフトとして利用して頂けるようにしました。

更に、最新のC++BuilderコンパイラでUnicode対応した本プログラムソース(Ver3.x)を、オープンオースソフトウェアとして公開し多くの人に活用して頂きたいと思っています。

従来のRev2.xは、C++Builder 5でコンパイルしてましたが、このRev3.xは「C++Builder Community Edition 10.3」でコンパイル確認しており個人ユーザは無料で利用できる開発ツールです。

### CWTW-Pro Rev3.xの特徴
Rev3.xではコンパイラ変更に伴い以下の見直しをしています。

* プログラム内部の文字コードをUnicodeに変更。
* 入力ファイルの文字コードは、UTF-8(BOM付推奨)に変更。拡張子は `*.txt` が基本。
* モールス練習機能は、Rev2.xの機能をそのまま踏襲。
* 動作確認OSは、Windows10 Pro (64bit版)。
* オープンソースソフトウェア。

Gitリポジトリ
----------
GitHubの下記URLでソース一式を公開。

<https://github.com/jr4qpv/cwtw-pro.git>

ソースファイルの簡単な説明
-----------------------------
C++Builderが生成するファイルの説明は省略。

|ファイル名         |説明                       |備考                |
|:------------------|:--------------------------|:-------------------|
|`README.md`       |本ドキュメントファイル     |Markdown記述        |
|`CWTW-Pro.cbproj`|C++Builderプロジェクトファイル|                 |
|`CWTW-Pro.cpp`    |プロジェクトメイン         |                    |
|`CWTW-Pro_Icon.ico`|アイコンファイル         |                    |
|`CWTW-ProPCH1.h`  |プロジェクトヘッダ         |                   |
|`*.dfm`            |画面フォーム定義           |                   |
|`BSnd.cpp`         |Beep音発生クラス           |                   |
|`BSnd.h`           |↑ヘッダ                   |                   |
|`CwTable.cpp`     |モールス符号テーブルクラス |                   |
|`CwTable.h`       |↑ヘッダ                   |                   |
|`debug.cpp`       |デバック用関数             |                   |
|`debug.h`         |↑ヘッダ                   |                   |
|`DSnd.cpp`        |Weve音発生クラス           |                   |
|`DSnd.h`          |↑ヘッダ                    |                  |
|`LingBuf.cpp`    |リングバッファー関数クラス  |                   |
|`LingBuf.h`      |↑ヘッダ                    |                   |
|`MSnd.cpp`       |Midi音発生クラス            |                   |
|`MSnd.h`          |↑ヘッダ                   |                   |
|`MyRandom.cpp`   |独自乱数関数               |                   |
|`MyRandom.h`     |↑ヘッダ                   |                   |
|`ToneThd.cpp`    |モールス練習メインスレッド  |                  |
|`ToneThd.h`      |↑ヘッダ                    |                  |
|`UAbout.cpp`     |バージョン情報表示画面      |                  |
|`UAbout.h`       |↑ヘッダ                    |                  |
|`UEdit.cpp`      |簡易ビューワ（簡易エディタ） |                 |
|`UEdit.h`        |↑ヘッダ                     |                 |
|`UEnv.cpp`       |環境設定フォーム             |                 |
|`UEnv.h`         |↑ヘッダ                     |                 |
|`UMain.cpp`      |メインフォーム               |                 |
|`UMain.h`        |↑ヘッダ                     |                 |
|`UReg.cpp`       |ライセンス登録ダイアログ    |未使用            |
|`UReg.h`         |↑ヘッダ                    |                  |
|`URegInf.cpp`   |ライセンス情報表示画面      |未使用            |
|`URegInf.h`       |↑ヘッダ                  |                  |
|`UTone.cpp`       |モールス信号生成クラス    |                  |
|`UTone.h`         |↑ヘッダ                  |                  |
|`CWTW-Pro_def.h` |共通定義情報              |                  |
|`Constant.h`     |定数定義                   |                  |
|`KbdTbl.h`        |キーボードテーブル        |                  |
|`Keytable.h`     |Midi音階テーブル           |                  |
|`Tonename.h`     |Midi音色名テーブル         |                  |
|`test?.txt`      |テスト用データファイル     |デバック確認用    |


### 実行ファイル出力フォルダ
`ExeFiles/`フォルダ以下に実行時に必要なファイルを格納。

|`ExeFiles/`ファイル名 |説明                       |備考                |
|:------------------|:--------------------------|:-------------------|
|`Readme.txt`      |説明ファイル               |                    |
|`License.txt`     |ライセンス説明ファイル     |MITライセンス       |
|`CWTW-Pro.txt`    |プログラムドキュメント     |                    |
|`(CWTW-Pro.exe)`  |コンパイルにより生成される実行ファイル |        |
|`DATA/`            |サンプルデータファイルフォルダ  |               |
|`_post_files.bat` |実行時必要なファイルの転送スクリプト| `C:\CWTW-Pro\`に転送 |

### CwTable.cpp 記述のモールス符号表現規則
　短点（・）…	01B  
　長点（－）…	10B

　上記の規則で、並べて2進数をつくって、右詰め（LSB側)したもの。

　例）Ａ（・－）  
　　2進数表記で 0110B  →　16進表記で 0x0006

コンパイル手順
-------------
下記サポートページのURLで、コンパイル手順などの技術情報を掲載。

<https://www.yokoweb.net/dokuwiki/develop/cwtw-pro/>

* バイナリファイルのみの公開も、同ページより行います。

ライセンスについて
--------------------
本プログラムは、オープンソースソフトウェアで「MITライセンス」に準じます。  
同梱の `License.txt` ファイルを参照ください。

### 免責
作者または著作権者は、契約行為、不法行為、またはそれ以外であろうと、ソフトウェアに起因または関連し、あるいはソフトウェアの使用またはその他の扱いによって生じる一切の請求、損害、その他の義務について何らの責任も負わないものとします。

来歴
----
* 2020/02/05 Rev3.00 「C++Builder Community Edition 10.3」に移行し、オープンソースソフトウェアとして公開。
