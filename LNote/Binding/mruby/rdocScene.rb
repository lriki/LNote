#cd /d "D:\LumineProjects\LuminePackage\Lumine\GamePlayer\Binding\mruby"
#cd /cygdrive/d/LumineProjects/LuminePackage/Lumine/GamePlayer/Binding/mruby
#rdoc -N -c UTF-8 -t "Ruby Document" -m rdocScene.rb

# = RDoc のサンプル
# Author::    歩行者
# Copyright:: Copyright 2007 WebOS Goodies
# License::   ご自由に
#
# このファイルは RDoc[http://rdoc.sourceforge.net/] による
# ドキュメント生成のサンプルです。
#
# そうそう、書き忘れてましたがクラス名は自動的にリンクになります。
# RDocTest, RDocTest#method ってな具合に。

#==============================================================================

# シーングラフの各要素のスーパークラス。
class SceneNode

end

#==============================================================================

# 2D スプライトのクラス
class Sprite < SceneNode
 
  # Sprite オブジェクトの生成
  # [texture] 転送元のテクスチャ
  def initialize(texture = nil)
  end
 
  # [arg1]
  #   定義リストの内容は別の行に記述することもできます。
  # [arg2]
  #   複数行にわたることも
  #   もちろんできます。
  #
  # メソッドの使用例とかを書きたいときは
  #
  #   字下げすれば pre タグで表示されます。
 
  def method(arg1, arg2)
  end
 
end

#==============================================================================

# ビューポートのクラス。ビューポートは SceneNode をグループ化し、描画をトリミングします。
class Viewport
 
  # Sprite オブジェクトの生成
  # [texture] 転送元のテクスチャ
  def initialize(texture = nil)
  end
 
  # [arg1]
  #   定義リストの内容は別の行に記述することもできます。
  # [arg2]
  #   複数行にわたることも
  #   もちろんできます。
  #
  # メソッドの使用例とかを書きたいときは
  #
  #   字下げすれば pre タグで表示されます。
 
  def method(arg1, arg2)
  end
 
end