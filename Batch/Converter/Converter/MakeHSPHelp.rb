#! ruby -EWindows-31J
# -*- mode:ruby; coding:Windows-31J -*-
#==============================================================================
# �� HSP �w���v�t�@�C�������
#------------------------------------------------------------------------------
# ���Ӂc
#   �E[�Ή���]"@param[in]  x, y, z" �͐���Ɍ��o�ł��Ȃ��̂� "x,y,z" �̂悤�ɃX�y�[�X�������B
#   �E��L�̂悤�� "x,y,z" �Ƀf�t�H���g�����̎w��͂ł��Ȃ�
#   �E�֐��錾�͕����s�ɂ��Ȃ�
#==============================================================================
require './Config.rb'

# �O���[�v
$class_group = {
  "LConfig"           => "�����ݒ�",
  "LAudio"            => "�����@�\",
  "LSound"            => "�����@�\",
  "L3DSoundListener"  => "�����@�\"
}

# �w�b�_�e���v���[�g
$hs_header_filepath = "HSPHelpTemplate/header.txt"

# �֐��e���v���[�g
$hs_func_filepath = "HSPHelpTemplate/function.txt"

# �ŏI�o�͑S�̂ɑ΂��čs��������u��
$global_replace_strings = [
  ["�̃|�C���^", ""]
]

#==============================================================================
# �� �ݒ肱���܂�
#==============================================================================
require './Analyzer.rb'

file = open($hs_header_filepath)
$output = file.read

file = open($hs_func_filepath)
$func_template = file.read

$analyzer = Analyzer.new

# �S�t�@�C�����
for filename in $target_files
  p filename
  $analyzer.analyze($lnote_root + filename, "hsp")
end

# ���Ƃ�����
for doc in $analyzer.func_doc_list
  # �O����
  continue = false
  func_name = doc.name
  for opt in doc.option_args
    case opt.name
      when "disable"
        continue = opt.value
      when "name"
        func_name = opt.value
    end
  end
  if continue
    step = 0
    next
  end
  
  func_tmpl = $func_template.dup
  # ��{
  func_tmpl.sub!("_NAME_", func_name)
  func_tmpl.sub!("_BRIEF_", doc.summary)
  func_tmpl.sub!("_INST_", doc.detail)
  func_tmpl.sub!("_HREF_", "")
  
  # �O���[�v
  func_name =~ /^.+_/
  class_name = $&.delete("_")
  group = $class_group[class_name]
  if group != nil
    func_tmpl.sub!("_GROUP_", group)
  else
    func_tmpl.sub!("_GROUP_", "")
  end
  
  # �������X�g
  arg_list = ""
  for i in 0...doc.param_pars.size
    arg_list += ", " if i != 0
    arg_list += doc.param_pars[i].caption
  end
  func_tmpl.sub!("_PRM_LIST_", arg_list)
  
  # �����ڍ�
  arg_detail = ""
  # [in] ���ɕK�v�ȃX�y�[�X�̈�`�F�b�N
  io_space_count = 0
  for param in doc.param_pars
    io_space_count = param.io_type.length  if param.io_type.length > io_space_count
  end
  io_space_count += 1 # �X�y�[�X
  # ���O�ƃf�t�H���g�l�̃X�y�[�X�̈�`�F�b�N
  name_space_count = 0
  for param in doc.param_pars
    t = param.caption.length + 1    # +1 �̓X�y�[�X�̕�
    default_val = doc.find_default_arg(param.caption)  # �f�t�H���g����
    if default_val != nil
      t += default_val.length + 2 # +2 �� ()
    end
    name_space_count = t if t > name_space_count
  end
  
  # �����ڍׂ����
  for param in doc.param_pars
    # io type
    t = param.io_type
    snum = io_space_count - t.length
    t += " " * snum if snum > 0
    # name
    default_val = doc.find_default_arg(param.caption)  # �f�t�H���g����
    c = param.caption
    c += "(" + default_val + ")" if default_val != nil
    t += c
    snum = name_space_count - c.length
    t += " " * snum if snum > 0
    # detail
    t += param.detail + "\n"
    # args
    for a in param.arg_pars
      t += (" " * (io_space_count+name_space_count)) + "    "
      t += a + "\n"
    end
    arg_detail += t
  end
  
  # return
  if doc.return_detail != nil
    arg_detail += "\nstat : "
    lines = doc.return_detail.split("\n")
    for i in 0...lines.size
      if i >= 1
        arg_detail += "\n       " # "stat : " �̕��̋�
      end
      arg_detail += lines[i]
    end
  end
  
  func_tmpl.sub!("_PRM_DETAIL_", arg_detail)
  $output += "\n" + func_tmpl + "\n"
  step = 0
end

# �O���[�o���̒u������
for pair in $global_replace_strings
  $output.gsub!(pair[0], pair[1])
end

# �o��
open(ARGV[0] + "lnote.hs", "w") {|f| f.write $output}

p "[finished.]"

=begin
#--------------------------------------------------------------------------
# �֐��R�����g�̐擪�s�����`�F�b�N���� "    ///**"
#--------------------------------------------------------------------------
def is_doc_begin_line(one_line_str)
  # \s+   :�X�y�[�Xof�^�u��1��ȏ�
  # \/{3} :/ ��3��
  # \*{2} :* ��2��
  if one_line_str =~ /\s+\/{3}\*{2}/
    return true
  end
  return false
end
#--------------------------------------------------------------------------
# �֐��R�����g�̏I�[�s�����`�F�b�N���� "    //*/"
#--------------------------------------------------------------------------
def is_doc_end_line(one_line_str)
  if one_line_str =~ /\s+\/{2}\*\//
    return true
  end
  return false
end
#--------------------------------------------------------------------------
# �֐��s�����`�F�b�N���� (is_doc_end_line �ŏI�[���o�ς݂ł��邱��)
#--------------------------------------------------------------------------
def is_decl_func_line(one_line_str)
  if one_line_str =~ /\s+.*\(.*\);/
    return true
  end
  return false
end
#--------------------------------------------------------------------------
# step 2 ����ʃI�v�V�������
#   ["disable"] false �̏ꍇ�Adisable
#--------------------------------------------------------------------------
def step_2_analyze_lang_option(one_line_str)
  r = []
  if one_line_str =~ /\[.+\]/
    lang_texts = $&.delete("[]").split(",")
    for text in lang_texts
      args = text.delete("{}").split(" ")
      if args[0] == "hsp"
        for a in args
          pair = a.split("=")
          case pair[0]
            when "disable"
              r.push(["disable", true])
            when "name"
              r.push(["name", pair[1]])
          end
        end
      end
    end
  end
  if r.size == 0
    r.push(["disable", false])
  end
  return r
end
#--------------------------------------------------------------------------
#
#--------------------------------------------------------------------------
class ParamPar
  attr_accessor :io_type        # ���o�͂̎�� "[in]" "[out]" "[in,out]"
  attr_accessor :caption        # ������
  attr_accessor :detail         # ���� (':' ���܂�)
  attr_accessor :arg_pars       # @arg ������z�� "NAME : DETAIL"
  def initialize
    @io_type = ""
    @caption = ""
    @detail = ""
    @arg_pars = []
  end
end
#--------------------------------------------------------------------------
#
#--------------------------------------------------------------------------
class FuncDecl
  attr_accessor :name     # �֐���
  attr_accessor :args     # �������X�g
  def initialize
    @name = ""
    @args = []
  end
end
class ArgDecl
  attr_accessor :name     # ������
  attr_accessor :default  # �f�t�H���g����
  def initialize
    @name = ""
    @default = ""
  end
end
#--------------------------------------------------------------------------
#--------------------------------------------------------------------------
$doc_brief = ""
$param_pars = []
$last_param_par = nil
$return_detail = nil
$func_decl = nil
$output = ""
$func_template = ""
$default_arg_hash = {}
$option_args = []

file = open($hs_header_filepath)
$output = file.read

file = open($hs_func_filepath)
$func_template = file.read

for filename in $target_files
  p filename
  
  # 0 : doc �J�n������
  # 1 : doc �I�[������
  # 2 : �֐��錾������
  # 3 : �o��
  step = 0
  
  # 1 �s���ǂ�ł���
  file = open($lnote_root + filename)
  while text = file.gets do
  
    case step
      ################# doc �J�n������
      when 0
        if is_doc_begin_line(text)
          $param_pars.clear
          $default_arg_hash = {}
          $option_args = []
          $return_detail = nil
          step = 1
        end
      ################# doc �I�[������ (doc ��͒�)
      when 1
        # doc �I�[
        if is_doc_end_line(text)
          step = 2
        end
        # @brief
        if text =~ /\@brief\s+/
          $doc_brief = $' # �Ō�Ƀ}�b�`�����ӏ��̌�
        end
        # @param
        if text =~ /\@param/
          $last_param_par = ParamPar.new
          $' =~ /\[.+\]/
          $last_param_par.io_type = $&    # $& = �}�b�`�����ӏ�
          # "x, y, z" �̎b��΍�B':' �̑O�܂ł��������Ƃ݂Ȃ�
          if $'.include?(":")
            $' =~ /:.+/
            $last_param_par.caption = $`.strip
            $last_param_par.detail = $&
          else
            $' =~ /\s+\S+\s+/
            $last_param_par.caption = $&
            $last_param_par.caption.strip!  # �O��̃X�y�[�X���폜
            $last_param_par.detail = $'
          end
          $param_pars.push($last_param_par)
        end
        # @arg
        if text =~ /\@arg\s+/
          $last_param_par.arg_pars.push($')
        end
        # @return
        if text =~ /\@return\s+/
          $return_detail = $'
        end
      ################# ����ʃI�v�V�������
      when 2
        $option_args = step_2_analyze_lang_option(text)
        step = 3
      ################# �֐��錾������
      when 3
        if is_decl_func_line(text)
          $func_decl = FuncDecl.new
          # () �Ƃ��̑O�Ƃɕ�����
          text =~ /\(.*\)/
          arg_str = $&
          # �֐���
          $` =~ /\S+$/
          $func_decl.name = $&
          # ���� - ���ʂ��폜���A����
          arg_str.delete!("()")
          args = arg_str.split(",")
          for a in args
            arg_decl = ArgDecl.new
            a.strip!
            pair = a.split("=")
            arg_decl.name = pair[0]
            if pair.size != 1 # �f�t�H���g����������
              arg_decl.default = pair[1]
              arg_decl.default.strip!
            end
            # �^��������菜��
            arg_decl.name.strip!
            arg_decl.name =~ /\S+$/
            arg_decl.name = $&
            # �f�t�H���g�����o�^
            if !arg_decl.default.empty?
              $default_arg_hash[arg_decl.name] = arg_decl.default
            end
            
            $func_decl.args.push(arg_decl)
          end
          step = 4
        end
      ################# �o��
      when 4
        # �O����
        continue = false
        for pair in $option_args
          case pair[0]
            when "disable"
              continue = pair[1]
            when "name"
              $func_decl.name = pair[1]
          end
        end
        if continue
          step = 0
          next
        end
        
        func_tmpl = $func_template.dup
        # ��{
        func_tmpl.sub!("_NAME_", $func_decl.name)
        func_tmpl.sub!("_BRIEF_", $doc_brief)
        func_tmpl.sub!("_INST_", "")
        func_tmpl.sub!("_HREF_", "")
        
        # �O���[�v
        $func_decl.name =~ /^.+_/
        class_name = $&.delete("_")
        group = $class_group[class_name]
        if group != nil
          func_tmpl.sub!("_GROUP_", group)
        else
          func_tmpl.sub!("_GROUP_", "")
        end
        
        # �������X�g
        arg_list = ""
        for i in 0...$param_pars.size
          arg_list += ", " if i != 0
          arg_list += $param_pars[i].caption
        end
        func_tmpl.sub!("_PRM_LIST_", arg_list)
        
        # �����ڍ�
        arg_detail = ""
        # [in] ���ɕK�v�ȃX�y�[�X�̈�`�F�b�N
        io_space_count = 0
        for param in $param_pars
          io_space_count = param.io_type.length  if param.io_type.length > io_space_count
        end
        io_space_count += 1 # �X�y�[�X
        # ���O�ƃf�t�H���g�l�̃X�y�[�X�̈�`�F�b�N
        name_space_count = 0
        for param in $param_pars
          t = param.caption.length + 1    # +1 �̓X�y�[�X�̕�
          default_val = $default_arg_hash[param.caption]  # �f�t�H���g����
          if default_val != nil
            t += default_val.length + 2 # +2 �� ()
          end
          name_space_count = t if t > name_space_count
        end
        # �����ڍׂ����
        for param in $param_pars
          # io type
          t = param.io_type
          snum = io_space_count - t.length
          t += " " * snum if snum > 0
          # name
          default_val = $default_arg_hash[param.caption]  # �f�t�H���g����
          c = param.caption
          c += "(" + default_val + ")" if default_val != nil
          t += c
          snum = name_space_count - c.length
          t += " " * snum if snum > 0
          # detail
          t += param.detail
          for a in param.arg_pars
            t += (" " * (io_space_count+name_space_count)) + "    "
            t += a
          end
          arg_detail += t + "\n"
        end
        # return
        if $return_detail != nil
          arg_detail += "\nstat : " + $return_detail
        end
        
        func_tmpl.sub!("_PRM_DETAIL_", arg_detail)
        $output += "\n" + func_tmpl + "\n"
        step = 0
    end
  end
  file.close
end

for pair in $global_replace_strings
  $output.gsub!(pair[0], pair[1])
end

open("lnote.hs", "w") {|f| f.write $output}
=end
