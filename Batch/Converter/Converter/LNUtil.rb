require 'Win32API'
kbhit = Win32API.new('msvcrt','_kbhit',[],'l')


#--------------------------------------------------------------------------
# キーが押されるまで待つ
#--------------------------------------------------------------------------
def waitkey
  print '続行するには何かキーを押してください'
  while true
     if kbhit.call != 0
        break
     end
  end
end