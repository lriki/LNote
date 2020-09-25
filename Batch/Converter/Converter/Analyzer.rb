
#==============================================================================
#
#==============================================================================
class FuncDoc
  attr_accessor :name           # 関数名
  attr_accessor :summary        # 概要
  attr_accessor :param_pars     # 引数詳細の配列 (ParamPar クラスの配列)
  attr_accessor :return_detail  # return 説明
  attr_accessor :detail         # 詳細 (@par)
  attr_accessor :option_args    # オプション引数の配列 (OptArg クラスの配列)
  attr_accessor :args           # 引数リスト (FuncArg クラスの配列)
  attr_accessor :return_type    # 戻り値の型名
  def initialize
    @name = ""
    @summary = ""
    @param_pars = []
    @return_detail = nil
    @detail = ""
    @option_args = []
    @args = []
  end
  # 引数名からデフォルト値を取得する (無ければ nil)
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
  attr_accessor :io_type      # 入出力の種類 "[in]" "[out]" "[in,out]"
  attr_accessor :caption      # 引数名
  attr_accessor :detail       # 説明 (':' を含む)
  attr_accessor :arg_pars     # @arg 文字列配列 "NAME : DETAIL"
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
  attr_accessor :name         # 引数名
  attr_accessor :default      # デフォルト引数
  attr_accessor :type         # 型
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
  attr_accessor :name         # 引数名
  attr_accessor :value        # 値
  def initialize(name, value)
    @name = name
    @value = value
  end
end

#==============================================================================
#
#==============================================================================
class EnumValue
  attr_accessor :name         # 引数名
  attr_accessor :value        # 値
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
  attr_accessor :func_doc_list  # 解析結果の FuncDoc 配列
  attr_accessor :enum_list      # 解析結果の EnumValue 配列
  #----------------------------------------------------------------------------
  # 初期化
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
  # ひとつのファイルを解析する
  #     lang_name : オプション引数の解析に使用する言語名
  #----------------------------------------------------------------------------
  def analyze(filename, lang_name)
    @step = 0
    @lang_name = lang_name
    # 1 行ずつ読んでいく
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
  # step0 コメント開始位置検索 & 初期化
  #----------------------------------------------------------------------------
  def step0_init(text)
    # \s+   :スペースofタブを1回以上
    # \/{3} :/ を3回
    # \*{2} :* を2回
    if text =~ /\s+\/{3}\*{2}/
      @cur_func_doc = FuncDoc.new
      @step = 1
    end
  end
  #----------------------------------------------------------------------------
  # step1 doxy コメント解析
  #----------------------------------------------------------------------------
  def step1_analyze_doc(text)
    # doc 終端
    if text =~ /\s+\/{2}\*\//
      end_par_analyze
      @step = 2
      return
    end
    # @brief
    if text =~ /\@brief\s+/
      end_par_analyze
      @cur_func_doc.summary = $' # 最後にマッチした箇所の後
      return
    end
    # @param
    if text =~ /\@param/
      end_par_analyze
      @cur_param_par = ParamPar.new
      $' =~ /\[.+\]/
      @cur_param_par.io_type = $&    # $& = マッチした箇所
      # "x, y, z" の暫定対策。':' の前までを引数名とみなす
      if $'.include?(":")
        $' =~ /:.+/
        @cur_param_par.caption = $`.strip
        @cur_param_par.detail = $&.strip
      else
        $' =~ /\s+\S+\s+/
        @cur_param_par.caption = $&
        @cur_param_par.caption.strip!  # 前後のスペースを削除
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
    # @par 解析中
    if @func_detail_analyzing
      text =~ /\/\/\s*/
      @cur_func_doc.detail += $'
      return
    end
    # @return 解析中
    if @return_detail_analyzing
      text =~ /\/\/\s*/
      @cur_func_doc.return_detail += $'
      return
    end
  end
  #--------------------------------------------------------------------------
  # step 2 言語別オプション解析
  #   ["disable"] false の場合、disable
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
    # ひとつもない場合は有効であることを示す値のみセット
    if r.size == 0
      r.push(OptArg.new("disable", false))
    end
    @cur_func_doc.option_args = r
    @step = 3
  end
  #----------------------------------------------------------------------------
  # step3 関数宣言解析
  #----------------------------------------------------------------------------
  def step3_analyze_func_decl(text)
    if text =~ /\s+.*\(.*\);/
    
      # () とその前とに分ける
      text =~ /\(.*\)/
      arg_str = $&
      
      # 関数名・型名
      $` =~ /\S+$/
      @cur_func_doc.name = $&
      @cur_func_doc.return_type = $`.strip
      
      # 引数 - 括弧を削除し、分割
      arg_str.delete!("()")
      args = arg_str.split(",")
      for a in args
        arg_decl = FuncArg.new
        a.strip!
        pair = a.split("=")
        arg_decl.name = pair[0]
        if pair.size != 1 # デフォルト引数がある
          arg_decl.default = pair[1]
          arg_decl.default.strip!
        end
        # 型部と名前に分ける
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
  # @par 解析終了
  #----------------------------------------------------------------------------
  def end_par_analyze
    @func_detail_analyzing = false
    @return_detail_analyzing = false
  end
  #----------------------------------------------------------------------------
  # enum 解析
  #----------------------------------------------------------------------------
  def analyze_enum(filepath, enum_types)
    file = open(filepath)
    file_text = file.read
    
    for type in enum_types
      # enum 定義全体を取り出す
      if file_text =~ /enum\s#{type}.*?\}/m
        # {～} を取り出す
        $& =~ /\{.*\}/m
        # コメントと改行、空白、ブラケットを削除
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
      # 見つからなかった
      else
        #p "not found enum " + type
      end
    end
  end
end
          