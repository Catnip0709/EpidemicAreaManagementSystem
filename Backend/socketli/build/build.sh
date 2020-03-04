dd if=/dev/zero of=/swapfile bs=64M count=16
mkswap /swapfile
swapon /swapfile
cmake ..
make
swapoff /swapfile
rm -f /swapfile