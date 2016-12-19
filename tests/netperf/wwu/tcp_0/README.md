# testing

## outgoing
testing how IPFW handles outgoing traffic

### server
> service ipfw restart
> netserver

### client
> ./ipfwd 1 0.0
> netperf -l 60 -c -C -H SERVER-t TCP_STREAM -- -P 80 > outgoing.txt

## incoming
testing how IPFW handles incoming traffic

### server
> service ipfw restart
> netserver

### client
> ./ipfwd 1 0.0
> netperf -l 60 -c -C -H SERVER -t TCP_STREAM -- -P 80

### generating more rules to see how it affects performance
> root@cf498-nel-01 ~/i/r/tests# service ipfw restart
> root@cf498-nel-01 ~/i/r/tests#
> for i in (seq 254); ipfw add 1"$i" deny ip from 1.1.1."$i" to any; end
