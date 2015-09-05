
Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty64"

  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update
    sudo apt-get install -y build-essential
    sudo apt-get install -y cmake
    sudo apt-get install -y valgrind
    sudo apt-get install -y libgles2-mesa-dev
  SHELL
end
