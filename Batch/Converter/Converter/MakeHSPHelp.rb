#! ruby -EWindows-31J
# -*- mode:ruby; coding:Windows-31J -*-
#==============================================================================
# ■ HSP ヘルプファイルを作る
#------------------------------------------------------------------------------
# 注意…
#   ・[対応済]"@param[in]  x, y, z" は正常に検出できないので "x,y,z" のようにスペースを消す。
#   ・上記のような "x,y,z" にデフォルト引数の指定はできない
#   ・関数宣言は複数行にしない
#==============================================================================
require './Config.rb'

# グループ
$class_group = {
  "LConfig"           => "初期設定",
  "LAudio"            => "音声機能",
  "LSound"            => "音声機能",
  "L3DSoundListener"  => "音声機能"
}

# ヘッダテンプレート
$hs_header_filepath = "HSPHelpTemplate/header.txt"

# 関数テンプレート
$hs_func_filepath = "HSPHelpTemplate/function.txt"

# 最終出力全体に対して行う文字列置換
$global_replace_strings = [
  ["のポインタ", ""]
]

#==============================================================================
# ▲ 設定ここまで
#==============================================================================
require './Analyzer.rb'

file = open($hs_header_filepath)
$output = file.read

file = open($hs_func_filepath)
$func_template = file.read

$analyzer = Analyzer.new

# 全ファイル解析
for filename in $target_files
  p filename
  $analyzer.analyze($lnote_root + filename, "hsp")
end

# 落とし込み
for doc in $analyzer.func_doc_list
  # 前処理
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
  # 基本
  func_tmpl.sub!("_NAME_", func_name)
  func_tmpl.sub!("_BRIEF_", doc.summary)
  func_tmpl.sub!("_INST_", doc.detail)
  func_tmpl.sub!("_HREF_", "")
  
  # グループ
  func_name =~ /^.+_/
  class_name = $&.delete("_")
  group = $class_group[class_name]
  if group != nil
    func_tmpl.sub!("_GROUP_", group)
  else
    func_tmpl.sub!("_GROUP_", "")
  end
  
  # 引数リスト
  arg_list = ""
  for i in 0...doc.param_pars.size
    arg_list += ", " if i != 0
    arg_list += doc.param_pars[i].caption
  end
  func_tmpl.sub!("_PRM_LIST_", arg_list)
  
  # 引数詳細
  arg_detail = ""
  # [in] 等に必要なスペース領域チェック
  io_space_count = 0
  for param in doc.param_pars
    io_space_count = param.io_type.length  if param.io_type.length > io_space_count
  end
  io_space_count += 1 # スペース
  # 名前とデフォルト値のスペース領域チェック
  name_space_count = 0
  for param in doc.param_pars
    t = param.caption.length + 1    # +1 はスペースの分
    default_val = doc.find_default_arg(param.caption)  # デフォルト引数
    if default_val != nil
      t += default_val.length + 2 # +2 は ()
    end
    name_space_count = t if t > name_space_count
  end
  
  # 引数詳細を作る
  for param in doc.param_pars
    # io type
    t = param.io_type
    snum = io_space_count - t.length
    t += " " * snum if snum > 0
    # name
    default_val = doc.find_default_arg(param.caption)  # デフォルト引数
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
        arg_detail += "\n       " # "stat : " の分の空白
      end
      arg_detail += lines[i]
    end
  end
  
  func_tmpl.sub!("_PRM_DETAIL_", arg_detail)
  $output += "\n" + func_tmpl + "\n"
  step = 0
end

# グローバルの置換処理
for pair in $global_replace_strings
  $output.gsub!(pair[0], pair[1])
end

# 出力
open(ARGV[0] + "lnote.hs", "w") {|f| f.write $output}

p "[finished.]"

=begin
#--------------------------------------------------------------------------
# 関数コメントの先頭行かをチェックする "    ///**"
#--------------------------------------------------------------------------
def is_doc_begin_line(one_line_str)
  # \s+   :スペースofタブを1回以上
  # \/{3} :/ を3回
  # \*{2} :* を2回
  if one_line_str =~ /\s+\/{3}\*{2}/
    return true
  end
  return false
end
#--------------------------------------------------------------------------
# 関数コメントの終端行かをチェックする "    //*/"
#--------------------------------------------------------------------------
def is_doc_end_line(one_line_str)
  if one_line_str =~ /\s+\/{2}\*\//
    return true
  end
  return false
end
#--------------------------------------------------------------------------
# 関数行かをチェックする (is_doc_end_line で終端検出済みであること)
#--------------------------------------------------------------------------
def is_decl_func_line(one_line_str)
  if one_line_str =~ /\s+.*\(.*\);/
    return true
  end
  return false
end
#--------------------------------------------------------------------------
# step 2 言語別オプション解析
#   ["disable"] false の場合、disable
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
  attr_accessor :io_type        # 入出力の種類 "[in]" "[out]" "[in,out]"
  attr_accessor :caption        # 引数名
  attr_accessor :detail         # 説明 (':' を含む)
  attr_accessor :arg_pars       # @arg 文字列配列 "NAME : DETAIL"
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
  attr_accessor :name     # 関数名
  attr_accessor :args     # 引数リスト
  def initialize
    @name = ""
    @args = []
  end
end
class ArgDecl
  attr_accessor :name     # 引数名
  attr_accessor :default  # デフォルト引数
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
  
  # 0 : doc 開始検索中
  # 1 : doc 終端検索中
  # 2 : 関数宣言検索中
  # 3 : 出力
  step = 0
  
  # 1 行ずつ読んでいく
  file = open($lnote_root + filename)
  while text = file.gets do
  
    case step
      ################# doc 開始検索中
      when 0
        if is_doc_begin_line(text)
          $param_pars.clear
          $default_arg_hash = {}
          $option_args = []
          $return_detail = nil
          step = 1
        end
      ################# doc 終端検索中 (doc 解析中)
      when 1
        # doc 終端
        if is_doc_end_line(text)
          step = 2
        end
        # @brief
        if text =~ /\@brief\s+/
          $doc_brief = $' # 最後にマッチした箇所の後
        end
        # @param
        if text =~ /\@param/
          $last_param_par = ParamPar.new
          $' =~ /\[.+\]/
          $last_param_par.io_type = $&    # $& = マッチした箇所
          # "x, y, z" の暫定対策。':' の前までを引数名とみなす
          if $'.include?(":")
            $' =~ /:.+/
            $last_param_par.caption = $`.strip
            $last_param_par.detail = $&
          else
            $' =~ /\s+\S+\s+/
            $last_param_par.caption = $&
            $last_param_par.caption.strip!  # 前後のスペースを削除
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
      ################# 言語別オプション解析
      when 2
        $option_args = step_2_analyze_lang_option(text)
        step = 3
      ################# 関数宣言検索中
      when 3
        if is_decl_func_line(text)
          $func_decl = FuncDecl.new
          # () とその前とに分ける
          text =~ /\(.*\)/
          arg_str = $&
          # 関数名
          $` =~ /\S+$/
          $func_decl.name = $&
          # 引数 - 括弧を削除し、分割
          arg_str.delete!("()")
          args = arg_str.split(",")
          for a in args
            arg_decl = ArgDecl.new
            a.strip!
            pair = a.split("=")
            arg_decl.name = pair[0]
            if pair.size != 1 # デフォルト引数がある
              arg_decl.default = pair[1]
              arg_decl.default.strip!
            end
            # 型部分を取り除く
            arg_decl.name.strip!
            arg_decl.name =~ /\S+$/
            arg_decl.name = $&
            # デフォルト引数登録
            if !arg_decl.default.empty?
              $default_arg_hash[arg_decl.name] = arg_decl.default
            end
            
            $func_decl.args.push(arg_decl)
          end
          step = 4
        end
      ################# 出力
      when 4
        # 前処理
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
        # 基本
        func_tmpl.sub!("_NAME_", $func_decl.name)
        func_tmpl.sub!("_BRIEF_", $doc_brief)
        func_tmpl.sub!("_INST_", "")
        func_tmpl.sub!("_HREF_", "")
        
        # グループ
        $func_decl.name =~ /^.+_/
        class_name = $&.delete("_")
        group = $class_group[class_name]
        if group != nil
          func_tmpl.sub!("_GROUP_", group)
        else
          func_tmpl.sub!("_GROUP_", "")
        end
        
        # 引数リスト
        arg_list = ""
        for i in 0...$param_pars.size
          arg_list += ", " if i != 0
          arg_list += $param_pars[i].caption
        end
        func_tmpl.sub!("_PRM_LIST_", arg_list)
        
        # 引数詳細
        arg_detail = ""
        # [in] 等に必要なスペース領域チェック
        io_space_count = 0
        for param in $param_pars
          io_space_count = param.io_type.length  if param.io_type.length > io_space_count
        end
        io_space_count += 1 # スペース
        # 名前とデフォルト値のスペース領域チェック
        name_space_count = 0
        for param in $param_pars
          t = param.caption.length + 1    # +1 はスペースの分
          default_val = $default_arg_hash[param.caption]  # デフォルト引数
          if default_val != nil
            t += default_val.length + 2 # +2 は ()
          end
          name_space_count = t if t > name_space_count
        end
        # 引数詳細を作る
        for param in $param_pars
          # io type
          t = param.io_type
          snum = io_space_count - t.length
          t += " " * snum if snum > 0
          # name
          default_val = $default_arg_hash[param.caption]  # デフォルト引数
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
