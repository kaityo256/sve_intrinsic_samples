# ARM SVE 組み込み関数のサンプル

## 概要

ARM SVEの組み込み関数のサンプルです。QEMUを使ってエミュレートできる環境をDockerで構築し、その中でARM SVEに対応したクロスコンパイラで動作を確認します。

## Dockerイメージの作成、実行

適当な場所でこのリポジトリをクローンします。

```sh
git clone https://github.com/kaityo256/sve_intrinsic_samples.git
cd sve_intrinsic_samples
```

`docker`ディレクトリに入ってmakeします。

```sh
cd docker
make
```

するとイメージ(`kaityo256/sve_intrinsic_samples`)ができます。イメージができたら`make run`でイメージの中に入れます。

```sh
$ make run
docker run -e GIT_USER= -e GIT_TOKEN= -it kaityo256/sve_intrinsic_samples
[user@85f489bea9ce ~]$
```

ここで環境変数`GIT_USER`や`GIT_TOKEN`は僕の開発用に渡しているものなので気にしないでください。

## サンプル

Dockerイメージの中に入ったら、`sve_intrinsic_samples/samples`の中にサンプルコードがあります。

```sh
cd sve_intrinsic_samples
cd samples
```

コンパイルコマンドは`aarch64-linux-gnu-g++ -static -march=armv8-a+sve -O2`と長いので、`ag++`とエイリアスを張ってあります。

`samples`の中にサンプルがいろいろ入っていますが、たとえば`01_predicate`はプレディケータの立っているビットを可視化するサンプルです。


```sh
$ cd 01_predicate/
$ ag++ test.cpp
$ qemu-aarch64 ./a.out
svptrue_b8
1111111111111111111111111111111111111111111111111111111111111111
svptrue_b16
0101010101010101010101010101010101010101010101010101010101010101
svptrue_b32
0001000100010001000100010001000100010001000100010001000100010001
svptrue_b64
0000000100000001000000010000000100000001000000010000000100000001
----
svptrue_pat_b8(SV_VL2)
0000000000000000000000000000000000000000000000000000000000000011
svptrue_pat_b16(SV_VL2)
0000000000000000000000000000000000000000000000000000000000000101
svptrue_pat_b32(SV_VL2)
0000000000000000000000000000000000000000000000000000000000010001
svptrue_pat_b64(SV_VL2)
0000000000000000000000000000000000000000000000000000000100000001
```

説明は[Qiitaの記事](https://qiita.com/kaityo256/items/71d4d3f6b2b77fd04cbb)を参照してください。

## ライセンス

MIT
