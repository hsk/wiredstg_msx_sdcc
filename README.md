# WIREDSTG MSX SDCC

C言語でぽいゲームを動かします。

## ソースコード概要

- bios.h BIOS定義
- vdp.h  VDP定義
- pattern.c パターンデータ
- System.c 基本的なライブラリ
- ctr0.s エントリポイント
  - main.c メインプログラム
    - App.c  アプリケーションプログラム (ここから,TitleかGameが呼ばれる)
      - Title.c タイトル画面
      - Game.c ゲーム本体
        - Ground.c 地面
        - Star.c 星
        - Ship.c ビックバイパー
          - Shot.c 自機弾
        - Enemy.c 敵のコントロール
          - Bullet.c 敵弾
          - EnemyBomb.c 敵の爆発
          - EnemyDee01.c  敵キャラ 砲台
          - EnemyDucker.c 敵キャラ 移動砲台
          - EnemyFans.c   敵キャラ 扇風機みたいなやつ
          - EnemyGarun.c  敵キャラ サインカーブを描いて移動
          - EnemyRugal.c  敵キャラ 寄ってくる
          - EnemyBigCore.c ビックコア
            - EnemyBeam.c ビックコアのビーム

## 各ステップで行うこと

WRIREDSTG MSX SDCC は以下のステップで段階的に作ることができます:

1. [自機を動かす](https://github.com/hsk/wiredstg_msx_sdcc/compare/0..1)
2. [敵機を出現させる](https://github.com/hsk/wiredstg_msx_sdcc/compare/1..2)
3. [自機弾をうち当たり判定をつける](https://github.com/hsk/wiredstg_msx_sdcc/compare/2..3)
4. [敵球を撃ってくる](https://github.com/hsk/wiredstg_msx_sdcc/compare/3..4)
5. [自機に当たり判定](https://github.com/hsk/wiredstg_msx_sdcc/compare/4..5)
6. [敵機の種類を増やす](https://github.com/hsk/wiredstg_msx_sdcc/compare/5..6)
7. [ビックコアを作る](https://github.com/hsk/wiredstg_msx_sdcc/compare/6..7)
8. [背景生成](https://github.com/hsk/wiredstg_msx_sdcc/compare/7..8)
9. [星を流す](https://github.com/hsk/wiredstg_msx_sdcc/compare/8..9)
10. [タイトル画面を出す](https://github.com/hsk/wiredstg_msx_sdcc/compare/9..10)
11. [タイトル画面をアニメーションさせる](https://github.com/hsk/wiredstg_msx_sdcc/compare/10..11)
12. [スコアをつける](https://github.com/hsk/wiredstg_msx_sdcc/compare/11..12)
13. [スピードをつける](https://github.com/hsk/wiredstg_msx_sdcc/compare/12..13)
14. [音をつける](https://github.com/hsk/wiredstg_msx_sdcc/compare/13..14)
