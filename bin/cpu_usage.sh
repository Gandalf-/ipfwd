# bash

finish() {
  echo
  echo $results
  echo $results | awk '{s+=$1}END{print "average:",s/NR}' RS=" "
  exit
}

trap finish INT
results=""

while true; do
  results="$results $(ps -C "$1" -o '%cpu' | tail -n 1 | grep -o '[0-9\.]*')"
  sleep 0.5
  echo -n "."
done
