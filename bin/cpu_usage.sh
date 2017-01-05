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
  pid="$(pgrep $1 | head -n 1)"
  if test "$pid" != ""; then
    results="$results $(ps -p "$pid" -o '%cpu' | tail -n 1 | grep -o '[0-9\.]*')"
  fi
  sleep 0.5
done
