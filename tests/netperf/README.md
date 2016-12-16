# netperf

"Netperf is a benchmark that can be used to measure various aspects of
networking performance. Currently, it's focus is on bulk data transfer and
request/response performance using either TCP or UDP, and the Berkeley Sockets
interface. In addition, tests for DLPI, and Unix Domain Sockets, tests for IPv6
may be conditionally compiled-in."

Tests are categorized by the systems they were run on
- droplets
> FreeBSD 11.0-RELEASE-p1 #0 r306420: Thu Sep 29 > 01:43:23 UTC 2016
> hw.model: Intel(R) Xeon(R) CPU E5-2650L v3 @ 1.80GHz
> hw.machine: amd64
> hw.ncpu: 1
> hw.physmem: 502874112

- wwu
> FreeBSD 11.0-RELEASE-p1 #0 r306420: Thu Sep 29 01:43:23 UTC 2016 
> hw.model: Intel(R) Core(TM) i7-2600 CPU @ 3.40GHz
> hw.machine: amd64
> hw.ncpu: 8
> hw.physmem: 17101189120

## server
root@cf498-nel-01 ~# netserver

## client
root@cf498-nel-02 ~# netperf -c -C -l 30 -H cf498-nel-01 -t TCP_STREAM -- -P 80

## ipfw_on.txt
denotes the results when the firewall was on using the rules in ipfw_rules.txt

## ipfw_off.txt
denotes the results when the firewall was disabled
  `service ipfw stop`

## ipfw_allow_all.txt
denotes the results when the firewall is on, and the first rule is 
  `ipfw -q add 1 prob 1.0 allow ip from any to any`
