#!/usr/local/bin/bash

for i in {1..5}; do
  echo "Iteration: $i"
  sudo iperf -p 80 -c lefti.anardil.net
done
