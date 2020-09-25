


class Scene_GameMain
end

p Scene_GameMain

p Scene_GameMain.to_s

=begin
module LNScene
  def cur
    return 8
  end
end

module ExScene < LNScene
  def tts
    return self.cur + 6
  end
end

p ExScene.tts
=end

#GameSceneManagerBase.register_active_scene_key_name(Scene_GameMain.to_s)

p GameSceneManager



#p GameSceneManagerBase





class GameScene_Title < Core::GameScene
end

#scene = GameScene_Title.new

#p GameScene_Title

GameSceneManager.run



while true
  LNote::Framework.update()
end
