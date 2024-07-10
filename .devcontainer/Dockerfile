FROM ubuntu:22.04

# LABEL for the custom image
LABEL maintainer="michael.breitung@mibreit-photo.com"
LABEL version="0.1"
LABEL description="This is a custom Docker Image for Web Development."

# Disable Prompt During Packages Installation
ARG DEBIAN_FRONTEND=noninteractive

SHELL ["/bin/bash", "-c"]

ENV NVM_DIR /usr/local/nvm
ENV NODE_VERSION v20.11.0

# Update Ubuntu Software repository and install tools
RUN apt-get update && apt-get upgrade -y && apt-get install -y --no-install-recommends \
  git=1:2.34.1-1ubuntu1.11 \
  sudo=1.9.9-1ubuntu2.4 \
  ca-certificates=20230311ubuntu0.22.04.1 \
  curl=7.81.0-1ubuntu1.16 \
  openssh-server \
  wget=1.21.2-2ubuntu1 \
  build-essential=12.9ubuntu3 \
  gcc=4:11.2.0-1ubuntu1 \
  g++=4:11.2.0-1ubuntu1 \
  gdb \
  cmake=3.22.1-1ubuntu1.22.04.2 \
  valgrind=1:3.18.1-1ubuntu2 && \
  apt-get clean -y && \
  rm -rf /var/lib/apt/lists/*

# Install Docker
RUN install -m 0755 -d /etc/apt/keyrings && \
  curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc && \
  chmod a+r /etc/apt/keyrings/docker.asc

RUN echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo $VERSION_CODENAME) stable" | \
  tee /etc/apt/sources.list.d/docker.list > /dev/null 
RUN apt update && apt install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
RUN usermod -aG docker $(whoami)

# setup developer user
ENV UID=${UID:-1000}
ENV GID=${UID:-1000}
ENV USER=${USER:-developer}
RUN groupadd -g $GID $USER && \
  useradd -d /home/$USER -s /bin/bash -m $USER -l -u $UID -g $GID && \
  echo "developer ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
USER $USER

# Highlight Branch in Bash
RUN echo "parse_git_branch() {" >> ~/.bashrc && \
  echo "  git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/ (\1)/'" >> ~/.bashrc && \
  echo "}" >> ~/.bashrc && \
  echo "export PS1='\u@\h \[\033[32m\]\w\[\033[33m\]\$(parse_git_branch)\[\033[00m\] $ '" >> ~/.bashrc

