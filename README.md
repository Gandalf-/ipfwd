# ipfwd
IPFW Packet Sampling Project

Increase ipfw performance during high system load by reducing the percentage of
packets inspected.

### what is the project?
- add a monitoring daemon for IPFW that can reduce the proportion of incoming
  and outgoing packets that have the full firewall rule set applied to them.
  this allows the system to dynamically and gracefully trade-off between
  policy enforcement and performance.

- the goal is not to provide the same level of security at higher
  performance. instead, we're allowing the system to trade between security
  and performance

- because not all the packets will have the firewall rules applied to them,
  some could get through to internal services or contain malicious content
  that the firewall would have normally prevented. however, as long as the
  percentage is still above zero, we will have a change to detect, not
  prevent, these kinds of violations.

- the mindset for this kind of firewall needs to be of detecting violations,
  not preventing them. this is still valuable because it can tell you about
  networking errors, attack vectors and vulnerabilities. in this scenario,
  logging and inspection of logs is more important than a traditional
  firewalled system. you can't assume that the firewall all dropped packet
  types of a kind didn't get through and cause some kind of damage. what
  you're gaining is the knowledge that these occurred without the cost of
  inspecting every packet.

- this kind of system makes sense in an environment where the network
  throughput is higher than the available system resources required to apply
  the full firewall policy to each packet. however, it could still be useful to
  reduce the load imposed by the firewall, allowing other work can be done.

- it's based on the idea that network traffic that violates policies is likely
  to occur again. for instance, a typical persistant threat beacons out to a
  control center for commands. we might not catch every beacon, but we will
  eventually, probabilistically catch one of them which will allow us to look
  more closely into the problem.
