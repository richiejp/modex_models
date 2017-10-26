Here is a partial formal verification using Modex and Spin (Promela). If you follow
the Modex installation instructions (including adding the verify script to
your path), then you can perform the verification by doing:

```
$ verify fzsync.c
mverify fzsync.c
        Extract Model:
        ==============
modex fzsync.c
MODEX Version 2.10 - 25 June 2016
created: model and _modex_.run
        Compile and Run:
        ================
sh _modex_.run
(Spin Version 6.4.7 -- 19 August 2017)
        + Partial Order Reduction
Full statespace search for:
        never claim             - (none specified)
        assertion violations    +
        acceptance   cycles     - (not selected)
        invalid end states      +
State-vector 72 byte, depth reached 70, errors: 0
      301 states, stored
      222 states, matched
      523 transitions (= stored+matched)
        0 atomic steps
hash conflicts:         0 (resolved)
Stats on memory usage (in Megabytes):
    0.029       equivalent memory usage for states (stored*(State-vector + overhead))
    0.274       actual memory usage for states
 4096.000       memory used for hash table (-w29)
    0.534       memory used for DFS stack (-m10000)
 4096.730       total actual memory usage
pan: elapsed time 0 seconds
No Errors Found
```

This for tst_fzsync_pair_wait() in
https://github.com/linux-test-project/ltp/blob/master/include/tst_fuzzy_sync.h.
