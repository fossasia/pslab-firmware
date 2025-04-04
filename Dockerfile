FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    cmake \
    make \
    git \
    wget \
    && rm -rf /var/lib/apt/lists/*

RUN wget -qO- https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/xc16-v2.10-full-install-linux64-installer.run \
    -O /tmp/xc16-installer.run && \
    chmod +x /tmp/xc16-installer.run && \
    /tmp/xc16-installer.run --mode unattended --prefix /opt/microchip/xc16/v2.10 --netservername "" && \
    rm /tmp/xc16-installer.run
