ubuntu_config:
	sudo apt install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev \
              gawk build-essential bison flex texinfo gperf libtool patchutils bc \
              zlib1g-dev libexpat-dev pkg-config  libglib2.0-dev libpixman-1-dev git tmux python3 ninja-build zsh -y \
			  libfdt-dev libssl-dev libncurses5-dev libncursesw5-dev libelf-dev
	cd ../.. && wget http://ftp.loongnix.cn/toolchain/gcc/release/loongarch/gcc8/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.1.tar.xz
	cd ../.. && tar xvJf loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.1.tar.xz
	cd ../../loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.1/bin && sudo sh -c 'echo "export PATH=\$$PATH:'"$(pwd)"'" >> /etc/profile'
	cd ../.. && source /etc/profile
	cd ../.. && rm loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.1.tar.xz
# cd ${HOME} && gg git clone https://github.com/loongson/qemu.git
# cd ${HOME}/qemu && mkdir build && cd build && ../configure --target-list=loongarch64-softmmu,loongarch64-linux-user && make -j$(nproc)
# cd ${HOME}/qemu && sudo make install
	cd ${HOME} && wget https://download.qemu.org/qemu-8.0.0.tar.xz
	cd ${HOME} && tar xvJf qemu-8.0.0.tar.xz
	cd ${HOME}/qemu-8.0.0 && ./configure --target-list=loongarch64-softmmu,loongarch64-linux-user
	cd ${HOME}/qemu-8.0.0 && make -j$(nproc)
	cd ${HOME}/qemu-8.0.0 && sudo make install
	cd ${HOME} && rm qemu-8.0.0.tar.xz
	make clean
	qemu-system-loongarch64 --version
	qemu-loongarch64 --version

test:
# sudo sh -c 'echo "export PATH=\$$PATH:'"$$(pwd)"'" >> /etc/profile'
# cat /etc/profile
	cd ${HOME}/qemu-8.0.0 && ./configure --target-list=loongarch64-softmmu,loongarch64-linux-user
	cd ${HOME}/qemu-8.0.0 && make -j$(nproc)
	cd ${HOME}/qemu-8.0.0 && sudo make install
	cd ${HOME} && rm qemu-8.0.0.tar.xz
	make clean
	qemu-system-loongarch64 --version
	qemu-loongarch64 --version
	
