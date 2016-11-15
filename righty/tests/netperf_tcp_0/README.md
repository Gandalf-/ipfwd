root@cf498-nel-01 ~/i/r/tests# service ipfw restart
root@cf498-nel-01 ~/i/r/tests#
for i in (seq 254); ipfw add 1"$i" deny ip from 1.1.1."$i" to any; end
