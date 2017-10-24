FROM ubuntu:16.04

ENV DEBIAN_FRONTEND "noninteractive"

RUN apt-get update -y
RUN apt-get -y \
  -o Dpkg::Options::="--force-confdef" \
  -o Dpkg::Options::="--force-confold" dist-upgrade

RUN apt-get install -y --no-install-recommends \
  sudo apt-transport-https software-properties-common ppa-purge apt-utils \
  ca-certificates git curl wget \
  tar zip unzip zlib1g-dev bzip2 libbz2-dev \
  openssl libssl-dev \
  zsh vim screen tree htop

# C++17 が使いたい
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
RUN add-apt-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-5.0 main"
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update -y
RUN apt-get install -y --no-install-recommends \
  build-essential binutils cmake autoconf automake autogen pkg-config libtool \
  gcc-7 g++-7 gdb \
  clang-5.0 lldb-5.0 lld-5.0
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 20
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 20

# vscode 依存ライブラリ
RUN apt-get install -y --no-install-recommends \
  libgtk2.0-0 libgtk-3-0 libgconf2-4 gvfs-bin \
  libpango-1.0-0 libcairo2 libfontconfig1 gettext aspell aspell-en rxvt-unicode-256color \
  libasound2 libcanberra-gtk-module libgl1-mesa-glx \
  libsecret-1-0 libnss3 libnotify-bin \
  x11-xserver-utils libxkbfile1 libxtst6 libxss1 xterm

RUN curl https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor | apt-key add -
RUN add-apt-repository "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main"
RUN apt-get update -y
RUN apt-get install -y --no-install-recommends code
#RUN wget -O vscode-amd64.deb  https://go.microsoft.com/fwlink/?LinkID=760868
#RUN dpkg -i vscode-amd64.deb
#RUN rm vscode-amd64.deb

RUN wget -q https://www.ubuntulinux.jp/ubuntu-ja-archive-keyring.gpg -O- | sudo apt-key add -
RUN wget -q https://www.ubuntulinux.jp/ubuntu-jp-ppa-keyring.gpg -O- | sudo apt-key add -
RUN wget https://www.ubuntulinux.jp/sources.list.d/xenial.list -O /etc/apt/sources.list.d/ubuntu-ja.list
RUN apt-get update -y
RUN apt-get install -y --no-install-recommends \
  language-pack-ja-base \
  language-pack-ja \
  ibus-mozc \
  tzdata \
  ubuntu-defaults-ja
RUN echo "Asia/Tokyo" > /etc/timezone
RUN dpkg-reconfigure --frontend noninteractive tzdata
RUN update-locale LANG=ja_JP.UTF-8
ENV LANG ja_JP.UTF-8

RUN apt-get install -f -y
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get dist-upgrade -y

WORKDIR /tmp
RUN rm -rf /tmp/*
RUN apt-get clean
RUN apt-get autoremove -y
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get autoremove -y
RUN apt-get autoclean -y
RUN rm -rf /var/lib/apt/lists/* /var/cache/apt/archives/*

ARG user_name=ubuntu
ARG user_id=1942
ARG group_name=ubuntu
ARG group_id=1942

RUN groupadd -g ${group_id} ${group_name}
RUN useradd -u ${user_id} -g ${group_id} -d /home/${user_name} --create-home --shell /usr/bin/zsh ${user_name}
RUN echo "${user_name} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
RUN chown -R ${user_name}:${group_name} /home/${user_name}
RUN chsh -s /usr/bin/zsh ${user_name}

USER ${user_name}
WORKDIR /home/${user_name}
ENV HOME /home/${user_name}

RUN bash -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"
RUN echo 'shell "/usr/bin/zsh"' >>  /home/ubuntu/.screenrc

# vscode plugins
RUN /usr/bin/code --install-extension ms-vscode.cpptools
RUN /usr/bin/code --install-extension vector-of-bool.cmake-tools
RUN /usr/bin/code --install-extension DevonDCarew.bazel-code
RUN /usr/bin/code --install-extension naereen.makefiles-support-for-vscode
RUN /usr/bin/code --install-extension maelvalais.autoconf


