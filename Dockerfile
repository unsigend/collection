FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    make \
    git \
    cloc \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

