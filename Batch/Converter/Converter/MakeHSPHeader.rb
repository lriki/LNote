
#==============================================================================
# ■ .as ファイルを作る
#   オプション引数
#     ・override=true
#         HSP 側での追加定義が必要なことを示す。
#         footer.as で、先頭に '_' を付けた命令を定義すること。
#           例) _LAudio_Init
#         上記命令定義内で関数を使うときは先頭に '__' を付けること。
#           例) __LAudio_Init
#
#     ・disable
#     ・name=newname
#         
#==============================================================================
require './Config.rb'

# ヘッダテンプレート
$header_filepath = "HSPHeaderTemplate/header.as"

# フッタテンプレート
$footer_filepath = "HSPHeaderTemplate/footer.as"

# 型名置換の対応表 (enum 等、ここにないものはすべて int に置き換えられる)
$replace_type = [
  ["int",       "int"],
  ["float",     "float"],
  ["bool",      "int"],
  ["lnChar*",   "str"],
  ["void*",     "var"],
  ["LNHandle*", "var"],   # 上から順に include? で判定しているので、* のあるものを先にしておく
  ["LNHandle",  "int"],
  ["LNResult",  "int"],
  ["void",      "void"]   # 戻り値の誤判定防止用
]

# enum 検索用ファイル
$const_search_filepash = [
  "LNote/Core/Base/Common.h",
  "LNote/Core/Audio/Interface.h"
]

# enum
$enum_cand_types = [
  "LNResultCode"
]

# 最終出力全体に対して行う文字列置換
$global_replace_strings = [
  ["\"LConfig_SetUserWindowHandle\" var", "\"LConfig_SetUserWindowHandle\" int"]
]

#==============================================================================
# ▲ 設定ここまで
#==============================================================================
require './Analyzer.rb'

#------------------------------
# 全ファイル解析
$analyzer = Analyzer.new
for filename in $target_files
  p filename
  $analyzer.analyze($lnote_root + filename, "hsp")
end

#------------------------------
# ヘッダ
file = open($header_filepath)
$output = file.read
$output += "\n"

#------------------------------
# 落とし込み
for doc in $analyzer.func_doc_list
  # 前処理
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
  
  # 引数部分
  for i in 0...doc.args.size
    t += " " if i == 0
    t += ", " if i != 0
    
    # 型置換
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
      $enum_cand_types.push(ts) # enum かも？
      ts = "int"
    end
    
    t += ts
  end
  
  # 戻り値 (型置換リストにないものであれば enum 候補)
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
  # 引数部分
  if doc.args.size > 0
    t += "("
    for i in 0...doc.args.size
      t += ", " if i != 0
      t += "%" + (i + 1).to_s
      t += "=" + doc.args[i].default if doc.args[i].default != nil
    end
    t += ")"
  end
  
  # 命令名
  t += " _" + func_name
  # 後半引数部分
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
# enum 検索
for filepath in $const_search_filepash
  $analyzer.analyze_enum($lnote_root + filepath, $enum_cand_types)
end
$output += "\n"
for enum in $analyzer.enum_list
  $output += "#const " + enum.name + " " + enum.value + "\n"
end

#------------------------------
# グローバルの置換処理
for pair in $global_replace_strings
  $output.gsub!(pair[0], pair[1])
end

#------------------------------
# フッタ
file = open($footer_filepath)
$output += file.read

#------------------------------
# 出力
open(ARGV[0] + "lnote.as", "w") {|f| f.write $output}

p "[finished.]"
