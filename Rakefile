# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

require 'open3'

task :default do
  puts "Available tasks:"
  call_system_check("rake -sT")
end

desc "Format the C/C++ source to match adopted conventions"
task :format do
  sources = all_sources
  puts "Formatting #{sources.length} files..."
  call_system_check "clang-format -i -style=file #{sources.join(' ')}"
  puts "Done formatting"
end

desc "Lint the C/C++ source against adopted conventions"
task :lint do
  sources = all_sources
  puts "Linting #{sources.length} files..."
  call_system_check "cpplint #{sources.join(' ')}"
  puts "Done linting"
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
    puts "Errors and Warnings:"
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
  header_files + source_files
end

def header_files
  Dir["{#{source_folders.join(',')}}/**/*.{h,hpp}"]
end

def source_files
  Dir["{#{source_folders.join(',')}}/**/*.{c,cc,cpp,c++,mm,m}"]
end

def source_folders
  ["Library", "Platforms"]
end
