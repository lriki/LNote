
#==============================================================================
#
#==============================================================================
class FuncDoc
  attr_accessor :name           # �֐���
  attr_accessor :summary        # �T�v
  attr_accessor :param_pars     # �����ڍׂ̔z�� (ParamPar �N���X�̔z��)
  attr_accessor :return_detail  # return ����
  attr_accessor :detail         # �ڍ� (@par)
  attr_accessor :option_args    # �I�v�V���������̔z�� (OptArg �N���X�̔z��)
  attr_accessor :args           # �������X�g (FuncArg �N���X�̔z��)
  attr_accessor :return_type    # �߂�l�̌^��
  def initialize
    @name = ""
    @summary = ""
    @param_pars = []
    @return_detail = nil
    @detail = ""
    @option_args = []
    @args = []
  end
  # ����������f�t�H���g�l���擾���� (������� nil)
  def find_default_arg(arg_name)
    for a in args
      return a.default if a.name == arg_name
    end
    return nil
  end
end

#==============================================================================
#
#==============================================================================
class ParamPar
  attr_accessor :io_type      # ���o�͂̎�� "[in]" "[out]" "[in,out]"
  attr_accessor :caption      # ������
  attr_accessor :detail       # ���� (':' ���܂�)
  attr_accessor :arg_pars     # @arg ������z�� "NAME : DETAIL"
  def initialize
    @io_type = ""
    @caption = ""
    @detail = ""
    @arg_pars = []
  end
end

#==============================================================================
#
#==============================================================================
class FuncArg
  attr_accessor :name         # ������
  attr_accessor :default      # �f�t�H���g����
  attr_accessor :type         # �^
  def initialize
    @name = ""
    @default = nil
    @type = ""
  end
end

#==============================================================================
#
#==============================================================================
class OptArg
  attr_accessor :name         # ������
  attr_accessor :value        # �l
  def initialize(name, value)
    @name = name
    @value = value
  end
end

#==============================================================================
#
#==============================================================================
class EnumValue
  attr_accessor :name         # ������
  attr_accessor :value        # �l
  def initialize(name, value)
    @name = name
    @value = value
  end
end

#==============================================================================
#
#==============================================================================
class Analyzer
  #----------------------------------------------------------------------------
  # 
  #----------------------------------------------------------------------------
  attr_accessor :func_doc_list  # ��͌��ʂ� FuncDoc �z��
  attr_accessor :enum_list      # ��͌��ʂ� EnumValue �z��
  #----------------------------------------------------------------------------
  # ������
  #----------------------------------------------------------------------------
  def initialize
    @step = 0
    @lang_name = ""
    @cur_func_doc = nil
    @cur_param_par = nil
    @func_doc_list = []
    @enum_list = []
    @func_detail_analyzing = false
    @return_detail_analyzing = false
  end
  #----------------------------------------------------------------------------
  # �ЂƂ̃t�@�C������͂���
  #     lang_name : �I�v�V���������̉�͂Ɏg�p���錾�ꖼ
  #----------------------------------------------------------------------------
  def analyze(filename, lang_name)
    @step = 0
    @lang_name = lang_name
    # 1 �s���ǂ�ł���
    file = open(filename)
    while text = file.gets do
      case @step
        when 0
          step0_init(text)
        when 1
          step1_analyze_doc(text)
        when 2
          step2_analyze_lang_option(text)
        when 3
          step3_analyze_func_decl(text)
          @func_doc_list.push(@cur_func_doc)
      end
    end
  end
  #----------------------------------------------------------------------------
  # step0 �R�����g�J�n�ʒu���� & ������
  #----------------------------------------------------------------------------
  def step0_init(text)
    # \s+   :�X�y�[�Xof�^�u��1��ȏ�
    # \/{3} :/ ��3��
    # \*{2} :* ��2��
    if text =~ /\s+\/{3}\*{2}/
      @cur_func_doc = FuncDoc.new
      @step = 1
    end
  end
  #----------------------------------------------------------------------------
  # step1 doxy �R�����g���
  #----------------------------------------------------------------------------
  def step1_analyze_doc(text)
    # doc �I�[
    if text =~ /\s+\/{2}\*\//
      end_par_analyze
      @step = 2
      return
    end
    # @brief
    if text =~ /\@brief\s+/
      end_par_analyze
      @cur_func_doc.summary = $' # �Ō�Ƀ}�b�`�����ӏ��̌�
      return
    end
    # @param
    if text =~ /\@param/
      end_par_analyze
      @cur_param_par = ParamPar.new
      $' =~ /\[.+\]/
      @cur_param_par.io_type = $&    # $& = �}�b�`�����ӏ�
      # "x, y, z" �̎b��΍�B':' �̑O�܂ł��������Ƃ݂Ȃ�
      if $'.include?(":")
        $' =~ /:.+/
        @cur_param_par.caption = $`.strip
        @cur_param_par.detail = $&.strip
      else
        $' =~ /\s+\S+\s+/
        @cur_param_par.caption = $&
        @cur_param_par.caption.strip!  # �O��̃X�y�[�X���폜
        @cur_param_par.detail = $'
      end
      @cur_func_doc.param_pars.push(@cur_param_par)
      return
    end
    # @arg
    if text =~ /\@arg\s+/
      end_par_analyze
      @cur_param_par.arg_pars.push($'.strip)
      return
    end
    # @return
    if text =~ /\@return\s+/
      end_par_analyze
      @cur_func_doc.return_detail = $'
      @return_detail_analyzing = true
      return
    end
    # @par
    if text =~ /\@par\s/
      @func_detail_analyzing = true
      return
    end
    # @par ��͒�
    if @func_detail_analyzing
      text =~ /\/\/\s*/
      @cur_func_doc.detail += $'
      return
    end
    # @return ��͒�
    if @return_detail_analyzing
      text =~ /\/\/\s*/
      @cur_func_doc.return_detail += $'
      return
    end
  end
  #--------------------------------------------------------------------------
  # step 2 ����ʃI�v�V�������
  #   ["disable"] false �̏ꍇ�Adisable
  #--------------------------------------------------------------------------
  def step2_analyze_lang_option(one_line_str)
    r = []
    if one_line_str =~ /\[.+\]/
      lang_texts = $&.delete("[]").split(",")
      for text in lang_texts
        args = text.delete("{}").split(" ")
        if args[0] == @lang_name
          for a in args
            pair = a.split("=")
            case pair[0]
              when "disable"
                r.push(OptArg.new("disable", true))
              else
                r.push(OptArg.new(pair[0], pair[1]))
              #when "name"
              #  r.push(OptArg.new("name", pair[1]))
            end
          end
        end
      end
    end
    # �ЂƂ��Ȃ��ꍇ�͗L���ł��邱�Ƃ������l�̂݃Z�b�g
    if r.size == 0
      r.push(OptArg.new("disable", false))
    end
    @cur_func_doc.option_args = r
    @step = 3
  end
  #----------------------------------------------------------------------------
  # step3 �֐��錾���
  #----------------------------------------------------------------------------
  def step3_analyze_func_decl(text)
    if text =~ /\s+.*\(.*\);/
    
      # () �Ƃ��̑O�Ƃɕ�����
      text =~ /\(.*\)/
      arg_str = $&
      
      # �֐����E�^��
      $` =~ /\S+$/
      @cur_func_doc.name = $&
      @cur_func_doc.return_type = $`.strip
      
      # ���� - ���ʂ��폜���A����
      arg_str.delete!("()")
      args = arg_str.split(",")
      for a in args
        arg_decl = FuncArg.new
        a.strip!
        pair = a.split("=")
        arg_decl.name = pair[0]
        if pair.size != 1 # �f�t�H���g����������
          arg_decl.default = pair[1]
          arg_decl.default.strip!
        end
        # �^���Ɩ��O�ɕ�����
        arg_decl.name.strip!
        arg_decl.name =~ /\S+$/
        arg_decl.name = $&
        arg_decl.type = $`.strip
        
        @cur_func_doc.args.push(arg_decl)
      end
      @step = 0
    end
  end
  #----------------------------------------------------------------------------
  # @par ��͏I��
  #----------------------------------------------------------------------------
  def end_par_analyze
    @func_detail_analyzing = false
    @return_detail_analyzing = false
  end
  #----------------------------------------------------------------------------
  # enum ���
  #----------------------------------------------------------------------------
  def analyze_enum(filepath, enum_types)
    file = open(filepath)
    file_text = file.read
    
    for type in enum_types
      # enum ��`�S�̂����o��
      if file_text =~ /enum\s#{type}.*?\}/m
        # {�`} �����o��
        $& =~ /\{.*\}/m
        # �R�����g�Ɖ��s�A�󔒁A�u���P�b�g���폜
        t = $&.gsub(/\/\/.*$/, "").delete("\r\n \t{}")
        decls = t.split(",")
        last_value = 0
        for d in decls
          pair = d.split("=")
          if pair.size >= 2
            last_value = pair[1].to_i
            @enum_list.push(EnumValue.new(pair[0], pair[1]))
          else
            @enum_list.push(EnumValue.new(pair[0], last_value.to_s))
          end
          last_value += 1
        end
      # ������Ȃ�����
      else
        #p "not found enum " + type
      end
    end
  end
end
          