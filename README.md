# WIREDSTG MSX on C

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
