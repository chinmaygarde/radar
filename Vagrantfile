# Copyright 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

Vagrant.configure(2) do |config|
  #
  #  Define a Linux VM
  #
  config.vm.define "linux" do |linux|
    linux.vm.box = "ubuntu/trusty64"
    linux.vm.provision "shell", inline: <<-SHELL
      sudo apt-get update
      sudo apt-get install -y build-essential
      sudo apt-get install -y cmake
      sudo apt-get install -y valgrind
      sudo apt-get install -y libgles2-mesa-dev
      sudo apt-get install -y libsdl2-dev
    SHELL
  end

  #
  #  Define a Windows 10 VM.
  #
  config.vm.define "windows" do |windows|
    windows.vm.box = "modernIE/w10-edge"
    windows.vm.communicator = "winrm"
    windows.vm.network "forwarded_port", host: 3389, guest: 3389
    windows.vm.provider "virtualbox" do |v|
      v.cpus = 2
      v.memory = 2048
    end
  end
end
