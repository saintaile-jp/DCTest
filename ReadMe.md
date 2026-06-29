# Dreamcast Game Starter

DreamSDK / KallistiOS 向けの最小ゲームプロジェクトです。コントローラーで四角を動かすところから開発を始められます。

## 必要環境

- Windows版 DreamSDK
- KallistiOS（DreamSDK Managerで構築済みであること）
- 実行先: Dreamcast実機 + dc-tool、またはDreamcastエミュレーター

PNG、Oggなどの追加KOS Portsは、この初期プロジェクトでは不要です。

## ビルド

DreamSDK Shellでこのディレクトリを開き、次を実行します。

```sh
make
```

PowerShellからは、DreamSDKの標準インストール先が `C:\DreamSDK` の場合に次を使えます。

```powershell
.\build.ps1
```

生成物は `build/dctest.elf` です。

## 実行とディスクイメージ

DreamSDKで `KOS_LOADER` が設定済みなら、実機へ転送できます。

```sh
make run
```

エミュレーターやCD-R用のセルフブートCDIは次で作成します。

```sh
make disc
```

CDIは `build/dctest.cdi` に生成されます。

PowerShellの場合:

```powershell
.\build.ps1 -Target run
.\build.ps1 -Target disc
.\build.ps1 -Target clean
```

## 操作

- 十字キー / アナログスティック: プレイヤーを移動
- START: 終了

## ディレクトリ構成

```text
.
|-- src/          Cソース
|-- romdisk/      ELFへ組み込むリソース（実行時は /rd）
|-- disc/         CDIに収録する追加ファイル
|-- build/        ビルド成果物
|-- Makefile      KallistiOS用ビルド定義
|-- ip.txt        Dreamcastブートセクターの情報
`-- build.ps1    PowerShell用DreamSDKラッパー
```

`romdisk/readme.txt` は `/rd/readme.txt` として読み込めます。画像やサウンドも `romdisk` に配置できますが、ディスクから逐次読み込む大きなファイルは `disc` 側に配置してください。
