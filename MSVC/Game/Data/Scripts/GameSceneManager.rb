#==============================================================================
# �� GameSceneManager
#------------------------------------------------------------------------------
#==============================================================================

module GameSceneManager
  #--------------------------------------------------------------------------
  # �� �C���X�^���X�ϐ�
  #--------------------------------------------------------------------------
  @scene = nil                            # ���݂̃V�[���I�u�W�F�N�g
  @stack = []                             # �K�w�J�ڗp�̃X�^�b�N
  #--------------------------------------------------------------------------
  # �� ���s
  #--------------------------------------------------------------------------
  def self.run
    @scene = first_scene_class.new
    Core::GameSceneManager.run(@scene)
  end
  #--------------------------------------------------------------------------
  # �� �ŏ��̃V�[���N���X���擾
  #--------------------------------------------------------------------------
  def self.first_scene_class
  	return GameScene_Title
  end
  #--------------------------------------------------------------------------
  # �� ���݂̃V�[�����擾
  #--------------------------------------------------------------------------
  def self.scene
    @scene
  end
  #--------------------------------------------------------------------------
  # �� ���݂̃V�[���N���X����
  #--------------------------------------------------------------------------
  def self.scene_is?(scene_class)
    @scene.instance_of?(scene_class)
  end
  #--------------------------------------------------------------------------
  # �� ���ڑJ��
  #--------------------------------------------------------------------------
  def self.goto(scene_class)
    @scene = scene_class.new
    Core::GameSceneManager.goto(@scene)
  end
  #--------------------------------------------------------------------------
  # �� �Ăяo��
  #--------------------------------------------------------------------------
  def self.call(scene_class)
    @scene = scene_class.new
    Core::GameSceneManager.call(@scene)
  end
  #--------------------------------------------------------------------------
  # �� �Ăяo�����֖߂�
  #--------------------------------------------------------------------------
  def self.return
    #@scene = @stack.pop
  end
  #--------------------------------------------------------------------------
  # �� �Ăяo���X�^�b�N�̃N���A
  #--------------------------------------------------------------------------
  def self.clear
    Core::GameSceneManager.clear
  end
  #--------------------------------------------------------------------------
  # �� �Q�[���I��
  #--------------------------------------------------------------------------
  def self.exit
    Core::GameSceneManager.exit
  end
end

#==============================================================================
#
#==============================================================================