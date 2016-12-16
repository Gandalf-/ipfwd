# ipfwd

Monitors system load using uptime() to set probability of IPFW's first rule
allowing any packet. This rule increases performance by sacrificing complete
security. The probability is dynamic based on the current load.

### Todo
- update so that the rule number where the "accept everything" rule is placed
  can be specified. this way, rules before will always be applied to every
  packet; useful for routing rules or critical policy.
