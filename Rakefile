# Copyright 2016 The Chromium Authors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following disclaimer
#      in the documentation and/or other materials provided with the
#      distribution.
#    * Neither the name of Google Inc. nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

require 'open3'

task :default do
  puts "Available tasks:"
  call_system_check("rake -sT")
end

desc "Format the C/C++ source to match adopted conventions"
task :format do
  sources = all_sources
  call_system_check "clang-format -i -style=file #{sources.join(' ')}"
  puts "Formatted #{sources.length} source files..."
end

desc "Lint the C/C++ source against adopted conventions"
task :lint do
  sources = all_sources
  puts "Linting ..."
  call_system_check "cpplint #{sources.join(' ')}"
  puts "Linted #{sources.length} source files using defined rules..."
end

def call_system(command)
  output = ''
  error = ''

  status = Open3.popen3(ENV, command) do |stdin, stdout, stderr, wait_thr|
    stdin.close
    output = stdout.read
    error = stderr.read
    wait_thr.value
  end

  if not output.empty?
    STDOUT.puts output
  end
  
  if not error.empty?
    STDERR.puts error
  end

  return status.success?
end

def call_system_check(command)
  if not call_system(command)
    raise "Error On: '#{command}'"
  end
end

def all_sources
  Dir["{Headers,Source}/**/*.{h,hpp,c,cc,cpp,c++,mm,m}"]
end