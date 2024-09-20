FROM ubuntu:24.10
LABEL Description="Build environment"

ENV HOME /root

SHELL ["/bin/bash", "-c"]

# Install tools


RUN apt-get update && apt-get -y --no-install-recommends install \
  ca-certificates \
  build-essential \
  clang \
  cmake \
  gdb \
  wget \ 
  graphviz \
  doxygen \
  unzip \
  cppcheck \
  libexpat1 \
  libexpat1-dev \
  valgrind && \
  apt-get clean 

# Install sonar scanner
RUN wget https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-6.1.0.4477-linux-x64.zip 
RUN unzip sonar-scanner-cli-6.1.0.4477-linux-x64.zip 
RUN mv sonar-scanner-6.1.0.4477-linux-x64 /opt/sonar-scanner

# Install RATS checker
RUN wget https://github.com/andrew-d/rough-auditing-tool-for-security/archive/refs/heads/master.zip
RUN unzip master.zip
WORKDIR /rough-auditing-tool-for-security-master
RUN chmod u+x configure
RUN ./configure
RUN make
RUN make install
