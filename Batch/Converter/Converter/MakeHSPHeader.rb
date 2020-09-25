#! ruby -EWindows-31J
# -*- mode:ruby; coding:Windows-31J -*-
#==============================================================================
# �� .as �t�@�C�������
#   �I�v�V��������
#     �Eoverride=true
#         HSP ���ł̒ǉ���`���K�v�Ȃ��Ƃ������B
#         footer.as �ŁA�擪�� '_' ��t�������߂��`���邱�ƁB
#           ��) _LAudio_Init
#         ��L���ߒ�`���Ŋ֐����g���Ƃ��͐擪�� '__' ��t���邱�ƁB
#           ��) __LAudio_Init
#
#     �Edisable
#     �Ename=newname
#         
#==============================================================================
require './Config.rb'

# �w�b�_�e���v���[�g
$header_filepath = "HSPHeaderTemplate/header.as"

# �t�b�^�e���v���[�g
$footer_filepath = "HSPHeaderTemplate/footer.as"

# �^���u���̑Ή��\ (enum ���A�����ɂȂ����̂͂��ׂ� int �ɒu����������)
$replace_type = [
  ["int",       "int"],
  ["float",     "float"],
  ["bool",      "int"],
  ["lnChar*",   "str"],
  ["void*",     "var"],
  ["LNHandle*", "var"],   # �ォ�珇�� include? �Ŕ��肵�Ă���̂ŁA* �̂�����̂��ɂ��Ă���
  ["LNHandle",  "int"],
  ["LNResult",  "int"],
  ["void",      "void"]   # �߂�l�̌딻��h�~�p
]

# enum �����p�t�@�C��
$const_search_filepash = [
  "LNote/Core/Base/Common.h",
  "LNote/Core/Audio/Interface.h"
]

# enum
$enum_cand_types = [
  "LNResultCode"
]

# �ŏI�o�͑S�̂ɑ΂��čs��������u��
$global_replace_strings = [
  ["\"LConfig_SetUserWindowHandle\" var", "\"LConfig_SetUserWindowHandle\" int"]
]

#==============================================================================
# �� �ݒ肱���܂�
#==============================================================================
require './Analyzer.rb'

#------------------------------
# �S�t�@�C�����
$analyzer = Analyzer.new
for filename in $target_files
  p filename
  $analyzer.analyze($lnote_root + filename, "hsp")
end

#------------------------------
# �w�b�_
file = open($header_filepath)
$output = file.read
$output += "\n"

#------------------------------
# ���Ƃ�����
for doc in $analyzer.func_doc_list
  # �O����
  continue = false
  func_name = doc.name
  override_str = " _"
  for opt in doc.option_args
    case opt.name
      when "disable"
        continue = opt.value
      when "name"
        func_name = opt.value
      when "override"
        override_str = (opt.value) ? "__" : " _"
    end
  end
  next if continue
  
  # "#func"
  t = "#func " + override_str + func_name + " " + "\"" + doc.name + "\""
  
  # ��������
  for i in 0...doc.args.size
    t += " " if i == 0
    t += ", " if i != 0
    
    # �^�u��
    ts = doc.args[i].type
    found = false
    for pair in $replace_type
      if ts.include?(pair[0])
        ts = pair[1]
        found = true
        break
      end
    end
    if found == false
      $enum_cand_types.push(ts) # enum �����H
      ts = "int"
    end
    
    t += ts
  end
  
  # �߂�l (�^�u�����X�g�ɂȂ����̂ł���� enum ���)
  found = false
  for pair in $replace_type
    if doc.return_type == pair[0]
      found = true
      break
    end
  end
  $enum_cand_types.push(doc.return_type) if found == false
  
  # #define
  t += "\n"
  t += "#define " + func_name
  # ��������
  if doc.args.size > 0
    t += "("
    for i in 0...doc.args.size
      t += ", " if i != 0
      t += "%" + (i + 1).to_s
      t += "=" + doc.args[i].default if doc.args[i].default != nil
    end
    t += ")"
  end
  
  # ���ߖ�
  t += " _" + func_name
  # �㔼��������
  if doc.args.size > 0
    t += " "
    for i in 0...doc.args.size
      t += ", " if i != 0
      t += "%" + (i + 1).to_s
    end
  end
  
  t += "\n"
  $output += t
end

#------------------------------
# enum ����
for filepath in $const_search_filepash
  $analyzer.analyze_enum($lnote_root + filepath, $enum_cand_types)
end
$output += "\n"
for enum in $analyzer.enum_list
  $output += "#const " + enum.name + " " + enum.value + "\n"
end

#------------------------------
# �O���[�o���̒u������
for pair in $global_replace_strings
  $output.gsub!(pair[0], pair[1])
end

#------------------------------
# �t�b�^
file = open($footer_filepath)
$output += file.read

#------------------------------
# �o��
open(ARGV[0] + "lnote.as", "w") {|f| f.write $output}

p "[finished.]"
