# msx0_httpget

MSX0用のHTTPファイル取得プログラムです。

MSX0の新ファーム(ver.0.11.08)+MSX-DOS(2)用になっています。

## 動作確認した環境

- MSX0 Stack ver.0.11.08
    - MSX-DOS2 Version 2.20
    - MSX-DOS Version 1.03

### ビルド環境
- macOS Sonoma 14.6.1
- z88dk Nightly Build(v22803-1d37583450-20240828)

## ビルド方法
Xcode、z88dkをインストールしているmacOS(UNIX風の環境ならmacOSでなくてもできると思います)で生成します。

MSX-DOS2版は make で生成。

MSX-DOS1版は make -f Makefile.dos1 で生成。(DOS1版はhttpget1.comが生成されます。)

## 使用方法

~~~
Usage : httpget HOSTNAME PORT SRCPATH DESTNAME
~~~

- HOSTNAME
    
    WEBサーバーのホスト名またはIPアドレス

- PORT

    WEBサーバーのポート番号

- SRCPATH

    取得したいファイルのパス

- DESTNAME

    保存したいファイル名

となり、次のように使用します。

※ MSX-DOS1版のコマンドはhttpget1.comになります。

~~~
httpget 192.168.0.2 80 /test.txt test.txt
~~~

これで http://192.168.0.2:80/test.txt をダウンロードしてtest.txtと言うファイルで保存することになります。


## 更新履歴
- 2024/9/1 Version 1.3

    - Content-Lengthを返すサーバーからの取得中にCtrl-Cで停止できなかったのを改修
    - 一部をアセンブラ化することにより高速化

- 2024/8/28 Version 1.2

    - 少し高速化。
    - DOS2版で書き込みエラーになったとき、エラーで止まるようにした。(ディスクフルでのみ確認)
    - DOS1版でダウンロードファイルのファイルサイズが正しいファイルサイズになるようにした。

- 2024/8/22 Version 1.1

    - Ctrl-Cで停止できなかったのをできるようにした。
    - DOS1版のバイナリ(httpget1.com)を用意した。※注 DOS1版で取得したファイルは128バイト単位になってしまいます。

- 2024/8/17 Version 1.0

    - 最初のバージョン

## [開発メモ？](memo.md)
