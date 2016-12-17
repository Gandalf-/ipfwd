# testing

save current rules
> ipfw show > ipfw_rules.txt

## outgoing
testing how IPFW handles outgoing traffic. 
server : ipfw off
client : ipfw on

### server
> service ipfw stop
> killall netserver; netserver

### client
> ./ipfwd 1 0.0
> netperf -l 60 -c -C -H 140.160.139.125 -t TCP_STREAM -- -P 80 | tee -a outgoing.txt


## incoming
testing how IPFW handles incoming traffic
server : ipfw on
client : ipfw off

### server
> ./ipfwd 1 0.0
> killall netserver; netserver

### client
> service ipfw stop; killall ipfwd
> netperf -l 60 -c -C -H cf418-nel-01 -t TCP_STREAM -- -P 80 | tee -a incoming.txt
