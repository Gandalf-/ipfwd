# ipfwd

Monitors system load using uptime() to set probability of IPFW's first rule
allowing any packet. This rule increases performance by sacrificing complete
security. The probability is dynamic based on the current load.

```
ipfwd [rule_number static_prob]  
  rule_number : int, rule number to use for accept all rule. rules before  
                this will always be run. defaults to 1  
  
  static_prob : double, always use this probability for accept all rule,  
                ignore current system load. 0 denotes no static_prob,  
                defaults to 0  
```

### Todo
- update so that the rule number where the "accept everything" rule is placed
  can be specified. this way, rules before will always be applied to every
  packet; useful for routing rules or critical policy.
