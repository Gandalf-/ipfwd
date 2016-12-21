# bash

if test "$1" = ""; then
  echo "Must provide number of rules"
fi

if [ "$1" -ge 255 ]; then
  echo "Too many rules"
fi

base=5000

for i in $(seq "$1"); do
  num=$(( $i + $base ))
  ipfw -q add "$num" deny ip from 127.255.0."$i" to any
done


