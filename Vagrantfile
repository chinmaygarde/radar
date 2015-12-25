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

  #
  #  Define a FreeBSD 10.2 VM
  #
  config.vm.define "freebsd" do |freebsd|
    freebsd.vm.guest = :freebsd
    freebsd.vm.box = "freebsd/FreeBSD-10.2-RELEASE"
    freebsd.vm.network "private_network", ip: "192.168.50.4"
    freebsd.vm.synced_folder ".", "/vagrant", type: "nfs"
    freebsd.ssh.shell = "sh"
    freebsd.vm.base_mac = "080027D14C66"

    freebsd.vm.provider :virtualbox do |vb|
      vb.customize ["modifyvm", :id, "--memory", "2048"]
      vb.customize ["modifyvm", :id, "--cpus", "2"]
      vb.customize ["modifyvm", :id, "--hwvirtex", "on"]
      vb.customize ["modifyvm", :id, "--audio", "none"]
      vb.customize ["modifyvm", :id, "--nictype1", "virtio"]
      vb.customize ["modifyvm", :id, "--nictype2", "virtio"]
    end

    freebsd.vm.provision "shell", inline: <<-SHELL
      pkg install -y pkgconf cmake gmake glproto wget
      ( mkdir -p /tmp/mesa_source && \
        cd /tmp/mesa_source/ && \
        wget ftp://ftp.freedesktop.org/pub/mesa/older-versions/10.x/10.6.9/mesa-10.6.9.tar.xz && \
        tar -xjvf mesa-10.6.9.tar.xz )
      ( cd /tmp/mesa_source/mesa-10.6.9 && \
        ./configure --enable-gles2 --enable-osmesa --with-gallium-drivers=swrast && \
        gmake && \
        gmake install )
    SHELL
  end

end
