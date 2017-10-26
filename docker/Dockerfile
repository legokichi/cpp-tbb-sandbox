FROM ubuntu:16.04
ENV DEBIAN_FRONTEND "noninteractive"

RUN apt-get update -y
RUN apt-get -y \
  -o Dpkg::Options::="--force-confdef" \
  -o Dpkg::Options::="--force-confold" dist-upgrade

ENV HOME /root
ENV LANG ja_JP.UTF-8
ENV LC_ALL ${LANG}
ENV LANGUAGE ${LANG}
ENV TZ Asia/Tokyo

# Install the required packages for desktop
RUN apt-get install -y \
  xvfb xfce4 x11vnc xfce4-terminal \
  supervisor

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime
RUN echo $TZ > /etc/timezone

# Install japanese language packs(optional)
RUN apt-get install -y \
  language-pack-ja-base \
  language-pack-ja \
  ibus-mozc \
  fonts-takao

# Install utilities(optional).
RUN apt-get install -y --no-install-recommends \
  dconf-tools \
  apt-transport-https software-properties-common ppa-purge apt-utils \
  ca-certificates git curl wget \
  tar zip unzip zlib1g-dev bzip2 libbz2-dev \
  openssl libssl-dev \
  zsh vim screen tree htop \
  net-tools lynx iftop traceroute \
  sudo

# Install noVNC
RUN mkdir -p /opt/noVNC/utils/websockify && \
  wget -qO- "http://github.com/novnc/noVNC/tarball/master" | tar -zx --strip-components=1 -C /opt/noVNC && \
  wget -qO- "https://github.com/novnc/websockify/tarball/master" | tar -zx --strip-components=1 -C /opt/noVNC/utils/websockify && \
  ln -s /opt/noVNC/vnc.html /opt/noVNC/index.html

# Rename user directories Japanese to English.
RUN LANG=C xdg-user-dirs-update --force

# Install gcc and clang
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
RUN add-apt-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-5.0 main"
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update -y
RUN apt-get install -y --no-install-recommends \
  build-essential binutils cmake autoconf automake autogen pkg-config libtool \
  gcc-6 g++-6 gcc-7 g++-7 gdb \
  clang-5.0 lldb-5.0 lld-5.0
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 20
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-6 20

# Install vscode - https://code.visualstudio.com/docs/setup/linux
RUN curl https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > microsoft.gpg
RUN mv microsoft.gpg /etc/apt/trusted.gpg.d/microsoft.gpg
RUN sh -c 'echo "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main" > /etc/apt/sources.list.d/vscode.list'
RUN apt-get update -y
RUN apt-get install -y code

RUN apt-get install -y -f
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get dist-upgrade -y

RUN apt-get clean -y
RUN apt-get autoremove -y
RUN apt-get autoclean -y
RUN rm -rf /var/lib/apt/lists/* /var/cache/apt/archives/*

EXPOSE 8080
COPY supervisord/* /etc/supervisor/conf.d/

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

# vscode plugins
RUN /usr/bin/code --install-extension ms-vscode.cpptools
RUN /usr/bin/code --install-extension vector-of-bool.cmake-tools
RUN /usr/bin/code --install-extension DevonDCarew.bazel-code
RUN /usr/bin/code --install-extension naereen.makefiles-support-for-vscode
RUN /usr/bin/code --install-extension maelvalais.autoconf
RUN /usr/bin/code --install-extension PeterJausovec.vscode-docker

# https://github.com/pyenv/pyenv#basic-github-checkout
RUN git clone https://github.com/yyuu/pyenv.git $HOME/.pyenv

# https://github.com/creationix/nvm
RUN curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.6/install.sh | bash

RUN git clone https://gist.github.com/legokichi/fbf096e826d07e7724f6c3d09eefb216.git dotfiles && \
  cd dotfiles && \
  chmod +x install.sh && \
  /bin/bash install.sh
RUN echo 'shell "/usr/bin/zsh"' >>  /home/ubuntu/.screenrc
RUN echo "ibus-daemon -d -x && sleep 3 && ibus engine mozc-jp" >> $HOME/.zshrc

RUN git config --global user.email "legokichi@gmail.com"
RUN git config --global user.name "legokichi"

# https://wiki.manjaro.org/index.php?title=日本語の設定
ENV DefaultIMModule ibus
ENV XMODIFIERS "@im=ibus"
ENV XMODIFIER "@im=ibus"
ENV GTK_IM_MODULE ibus
ENV QT_IM_MODULE ibus
ENV IBUS_ENABLE_SYNC_MODE 1
RUN sudo dconf-editor set org.freedesktop.ibus.general.hotkey triggers ['<Control><Alt>space']

#CMD ["sudo", "/usr/bin/supervisord", "-c", "/etc/supervisor/supervisord.conf"]
CMD ["/usr/bin/zsh"]
