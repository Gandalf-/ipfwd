# bash

finish() {
  echo
  echo $results
  echo $results | awk '{s+=$1}END{print "average:",s/NR}' RS=" "
  exit
}

trap finish INT
results=""
pid=$(pgrep $1)

while true; do
  results="$results $(ps -p "$pid" -o '%cpu' | tail -n 1 | grep -o '[0-9\.]*')"
  sleep 0.5
  echo -n "."
done
