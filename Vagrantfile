# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

GUEST_CPUS = 8
GUEST_MEMORY = 8192

Vagrant.configure(2) do |config|
  #
  #  Define a Linux VM
  #
  config.vm.define "linux" do |linux|
    linux.vm.box = "ubuntu/xenial64"
    linux.vm.provision "shell", inline: <<-SHELL
      wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
      sudo apt-add-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-3.9 main"
      sudo apt-get update
      sudo apt-get install -y build-essential cmake libgles2-mesa-dev libsdl2-dev ninja-build gdb clang-3.9 lldb-3.9
    SHELL
    linux.vm.provider :virtualbox do |vb|
      vb.memory = GUEST_MEMORY
      vb.cpus = GUEST_CPUS
    end
  end

  #
  #  Define a Windows 10 VM.
  #
  config.vm.define "windows" do |windows|
    windows.vm.box = "modernIE/w10-edge"
    windows.vm.communicator = "winrm"
    windows.vm.network "forwarded_port", host: 3389, guest: 3389
    windows.vm.provider :virtualbox do |vb|
      vb.cpus = GUEST_CPUS
      vb.memory = GUEST_MEMORY
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
      vb.cpus = GUEST_CPUS
      vb.memory = GUEST_MEMORY
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
