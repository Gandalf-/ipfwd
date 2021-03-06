# IPFWD - IP Firewall Daemon
##Probabilistic Policy Violation Detection with IPFWD
  
IPFWD is a system load monitoring daemon for FreeBSD that updates an early rule
in IPFW that has a chance to accept any packet. This provides a graceful trade
off between performance and security policy enforcement. The probability is
dependent on the current system load. IFPWD also extends IPFW logs to include
information about the probability of undetected rule violations caused by the
accept any packet rule.

###Abstract

By allowing a probabilistic chance for allowing any packet through the
firewall, IPFWD can provide a graceful transition between performance and
security that dynamically follows current system load. 

Firewall performance can be improved by reducing the number of rules applied to
each packet. Supposing a white list policy, the firewall has to apply every
rule to each packet before it's denied. IPFWD is a daemon that monitors system
load and updates a new first rule in IPFW that allows a percentage chance to
allow any packet through.  

Instead of applying each rule to every denied packet, we have a chance to
immediately accept any packet. Testing shows this improves performance. 

IPFWD has the most potential for usefulness in high bandwidth environments,
such as data centers, where it's not feasible to apply an extensive rule set to
each packet but it is important to be notified of security policy violations.
The balance between information and performance is self tuning based on current
system load.

Under normal load, IPFW will still fully enforce the security policy. But under
heavy load, IPFWD will back IPFW off so performance isn't bottlenecked at the
firewall.

###IPFWD

This is a shift in mindset from typical firewalls. Instead of enforcing every
part of a security policy all of the time, IPFWD enforces the policy some of
the time and extrapolates the violations it does encounter. This provides
insight into the violations that weren't detected by IPFW. For this cost, you
gain increased firewall performance and network throughput in resource bound
systems. It's acceptable to allow a percentage of policy violations given that
network traffic patterns are typically repeated.

For example, under heavy load, IPFWD may allow 40% of packets through without
applying the security policy. Supposing a port scan was initiated during this
time, you system would still reject and report approximately 60% of the port
scan. Since the percentage chance will fluctuate over time, IPFWD provides
additional information in the IPFW logs to show the chance additional
undetected violations occurred for detected violation.

This also allows administrators to retain extensive rule sets that fully
implement their security policy. Instead of having to simplifying rule sets to
increase performance, IPFWD balances policy enforcement and performance itself.
