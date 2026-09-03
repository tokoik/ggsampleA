# ggsampleA - 自由課題 A (3D モデルの陰影付けと正規化 BRDF)

## 1. 概要

本プログラムは、[ゲームグラフィックス特論](https://tokoik.github.io/gg/)の自由課題 A 用のひな型プログラムです。

- 講義ポータル: [ゲームグラフィックス特論 - 床井研究室](https://tokoik.github.io/gg/)

## 2. 課題の内容

本プロジェクト (ggsampleA) は、3D モデルを読み込んで、陰影をつけて表示するプログラムです。以下の指示に従って変更してください。

1. このバーテックスシェーダのソースプログラム `simple.vert` の uniform 変数 **`rotate`** には、マウスの**左ボタン**のドラッグから求めた**回転の変換行列**が格納されます。`simple.vert` を修正して、マウスの左ボタンのドラッグで図形が回転するようにしてください。
2. また `simple.vert` の uniform 変数 **`translate`** には、マウスの**右ボタン**のドラッグから求めた**並進の変換行列**が格納されます。`simple.vert` を修正して、マウスの右ボタンのドラッグで図形が移動するようにしてください。なお、マウスカーソルをウィンドウの縦・横とも、端から端まで移動したときの移動量は 2 になります。
3. このプログラムではバーテックスシェーダ `simple.vert` において陰影計算を頂点単位に行っています。そのため、陰影にポリゴンの境界の影響が表れています。そこで、この陰影計算をフラグメントシェーダ `simple.frag` に移して、**フラグメント（画素）単位に陰影計算を行う**ように `simple.vert` と `simple.frag` を修正してください。
4. この陰影計算には Blinn-Phong の陰影付けモデルを用いています。この鏡面反射光の放射輝度の算出に Sloan と Hoffman による**正規化された BRDF** を用いるよう、フラグメントシェーダ `simple.frag` を修正してください。
5. 修正したシェーダのソースプログラム `simple.vert` と `simple.frag` を提出してください。

> [!NOTE]
> - 本プログラムは File メニューから他の形状データも読み込めるようにしています。サンプルの形状データはプロジェクト内のモデルファイル等に入っています。
> - 読み込みプログラムの作り上、Debug ビルドで大きなデータファイルを読み込むと結構時間がかかります。
> - これは試験ではないので、不明なことがあれば質問してくださって結構です。

## 3. 対応環境

- **Windows**: Visual Studio 2019 / 2022 / 2026 (CMake 経由で GLFW 3.4 を自動ダウンロード)
- **macOS**: Xcode (GLFW 3.4 を自動ダウンロード、OpenGL Framework を使用)
- **Ubuntu Linux**: GCC / Make (システム標準の libglfw3-dev, libgl1-mesa-dev を使用)

## 4. ビルド手順

### Windows (Visual Studio)

```pwsh
cmake -B build -S .
cmake --build build --config Release
```

### macOS (Xcode)

```bash
cmake -B build -G Xcode
cmake --build build --config Release
```

### Ubuntu Linux (Makefile)

```bash
sudo apt-get update
sudo apt-get install -y libglfw3-dev libgl1-mesa-dev
cmake -B build -S .
cmake --build build
```

## 5. 起動方法

ビルド完了後、生成された実行ファイルを実行します。

- **Windows**: `build/Release/ggsampleA.exe`
- **macOS**: `build/Release/ggsampleA.app`
- **Linux**: `build/ggsampleA`

## 6. 操作方法

- **マウス左ドラッグ**: 図形の回転（`simple.vert` の `rotate` 反映後）
- **マウス右ドラッグ**: 図形の並進移動（`simple.vert` の `translate` 反映後）
- **マウスホイール**: ズームイン / ズームアウト
- **メニューバー (File)**: 3D 形状データの読み込み (ImGui + NFD)
- **[q] / [Q] / [ESC]**: プログラムの終了
