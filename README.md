# msx0_httpget

MSX0用のHTTPファイル取得プログラムです。

MSX0の新ファーム(ver.0.11.08)+MSX-DOS2用になっています。

## 動作確認した環境

- MSX0 Stack ver.0.11.08
    - MSX-DOS2 Version 2.20

### ビルド環境
- macOS Sonoma 14.6.1
- z88dk Nightly Build(v22793-e0f20fa179-20240813)

## ビルド方法
Xcode、z88dkをインストールしているmacOSでmakeで生成されます。

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

~~~
httpget 192.168.0.2 80 /httpget.com httget.com
~~~

これで http://192.168.0.2:80/httpget.com をダウンロードしてhttpget.comと言うファイルで保存することになります。



