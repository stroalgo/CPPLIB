FROM ubuntu:24.10
LABEL Description="Build environment"

ENV HOME /root

SHELL ["/bin/bash", "-c"]

# Install tools
RUN apt-get update && apt-get -y --no-install-recommends install \
  build-essential \
  clang \
  cmake \
  gdb \
  wget \ 
  graphviz \
  doxygen \
  valgrind && \
  apt-get clean
