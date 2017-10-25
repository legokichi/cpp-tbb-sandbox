

## vscode shortcut

* `Ctrl + Shift + P` - command
* `Ctrl + Shift + B` - build
* `Ctrl + Shift + I` - code format all (or right click)
* `Ctrl + K Ctrl + F` - selective format (or right click)
* `Ctrl + T` - symbol search
* `Ctrl + Shift + F10` - Peek Definition
* `F12` - Go to Definition
* `F9` - add break point
* `Ctrl + Shift + D` - open debugger
* `F5` - start debugging
* `-exec -enable-pretty-printing` - input debug console

## install debug info
* gdb usage
  * https://doss.eidos.ic.i.u-tokyo.ac.jp/html/gdb_step_into_libraries.html
  * http://d.hatena.ne.jp/embedded/20130120/p1
  * https://www.hiroom2.com/2016/07/14/ubuntu-16-04-apt-sourceでソースコードをダウンロードする/

```sh
sudo apt libc6-dbg
mkdir debug
cd debug
apt source libc6
```

## core dump
* http://d.hatena.ne.jp/JiaLu/20120113/1330396897

```sh
ulimit -c unlimited
```
