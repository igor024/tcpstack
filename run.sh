cmake -B build && 
cmake --build build && 
sudo ip tuntap add dev tap0 mode tap

process_id=$!
wait $process_id

sudo ip link set dev tap0 up &&
sudo ip addr replace 10.0.0.1/24 dev tap0

process_id=$!
wait $process_id

arping -I tap0 10.0.0.1 &
sudo ./build/tcp
