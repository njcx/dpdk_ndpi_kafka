# dpdk_ndpi_kafka


```bash

dpdk >= DPDK 20.02.1

kernel >= 3.10.0

CentOS
#  yum install -y libpcap-devel gcc gcc-c++ make meson ninja  numactl-devel  numactl  net-tools pciutils  librdkafka-devel
#  yum install -y kernel-devel-$(uname -r) kernel-headers-$(uname -r)

Debian + Ubuntu
# apt install -y libpcap-dev gcc g++ make meson ninja-build libnuma-dev numactl net-tools pciutils librdkafka-dev
# apt install -y linux-headers-$(uname -r)


#  wget http://fast.dpdk.org/rel/dpdk-20.11.10.tar.xz
#  tar -Jxvf dpdk-20.11.10.tar.xz
#  cd dpdk-stable-20.11.10 && meson build && cd build && ninja && ninja install
#  export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
#  git clone git://dpdk.org/dpdk-kmods && cd  dpdk-kmods/linux/igb_uio
#  make
#  modprobe uio  &&  insmod igb_uio.ko
#  dpdk-devbind.py -b igb_uio 0000:03:00.0(pci-addr)



#  wget  https://github.com/ntop/nDPI/archive/refs/tags/4.12.zip && unzip 4.12.zip 
#  cd nDPI-4.12  && ./autogen.sh && ./configure  && make && make install


#  cd dpdk_ndpi_kafka && make

```
