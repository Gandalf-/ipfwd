#server
root@cf498-nel-01 ~# sudo netserver -p 80

# client
root@cf498-nel-02 ~# sudo netperf -l 30 -H cf498-nel-01 -t TCP_STREAM -- -P 80
