#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
require 'optparse'

# mode flag
is_encode = false
is_decode = false
aes_key = nil
OptionParser.new do |opt|
  opt.on("-e"){|v| is_encode = v }
  opt.on("-d"){|v| is_decode = v }
  opt.on("-k AESKEY"){|v| aes_key = v }
  opt.parse!(ARGV)
end

# switch mode
if(is_encode && is_decode)
  puts "invalid option -e (encode) and -d (decode)"
  exit -1
elsif(!is_encode && !is_decode)
  puts "select option -e (encode) or -d (decode)"
  exit -1
# encode mode (bit -> text)
elsif(is_encode)
  input = ""
  while line = $stdin.gets
    line.chomp.each_char{|c| 
      printf("%08b", c[0])
    }
    puts    
  end
# decode mode (text -> bit)
elsif(is_decode)
  while line = $stdin.gets
    puts [line.chomp].pack("B*")
  end  
else
  puts "must not happen!"
  exit -1
end
