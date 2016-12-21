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
> media: Ethernet 10Gbase-T <full-duplex>

- wwu
> FreeBSD 11.0-RELEASE-p1 #0 r306420: Thu Sep 29 01:43:23 UTC 2016  
> hw.model: Intel(R) Core(TM) i7-2600 CPU @ 3.40GHz  
> hw.machine: amd64  
> hw.ncpu: 8  
> hw.physmem: 17101189120  
> media: Ethernet autoselect (1000baseT <full-duplex>)

- isilon_10G
> Isilon OneFS v8.1.0.DEV.0 amd64 IQ  
> hw.model: Intel(R) Xeon(R) CPU D-1527 @ 2.20GHz  
> hw.machine: amd64  
> hw.ncpu: 8  
> hw.physmem: 68543340544  
> media: ???  

- isilon_40G
> Isilon OneFS v8.1.0.DEV.0 amd64 IQ  
> hw.model: Intel(R) Xeon(R) CPU D-1527 @ 2.20GHz  
> hw.machine: amd64  
> hw.ncpu: 8  
> hw.physmem: 68543340544  
> media: Ethernet autoselect (40Gbase-CR4 \<full-duplex,rxpause,txpause\>)  

```
uname -orim
sysctl hw.model hw.machine hw.ncpu hw.physmem
ifconfig | grep media
```

# incoming
Measures performance when IPFW is primarily filtering incoming packets

## SERVER
```
bash cpu_usage.sh netserver
netserver -D
```

## CLIENT
```
ipfwd 1 \<acceptance prob\>
bash cpu_usage.sh netperf
netperf -l 60 -H SERVER -t TCP_STREAM > tcp_\<acceptance prob\>/incoming.txt
```

## directory naming convention
\<protocol type\>_\<first rule acceptance probability\>
