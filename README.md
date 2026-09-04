### minimal reproducer of knl_imalloc/free race cond

vuln found by GLM 5.3 (sorry)

please `make` the project as you would any, and then upload the `.uf2` firmware file onto your own Pico. afterwards, attach the serial monitor:

```
picocom -b 115200 /dev/ttyACM0
```

and then press the GP20 on the Maker board. you will observe output something like this:

```
[RACE COND] program started! 
[RACE COND]it should take around 20k mallocs / frees for the program to suddenly crash.
[churn1] race started
[churn0] race started
[churn1] n=1000 ok=1000 fail=0
[churn0] n=1000 ok=1000 fail=0
[churn1] n=2000 ok=2000 fail=0
[churn0] n=2000 ok=2000 fail=0
[churn1] n=3000 ok=3000 fail=0
[churn0] n=3000 ok=3000 fail=0
[churn1] n=4000 ok=4000 fail=0
[churn0] n=4000 ok=4000 fail=0
[churn1] n=5000 ok=5000 fail=0
[churn0] n=5000 ok=5000 fail=0
[churn1] n=6000 ok=6000 fail=0
[churn0] n=6000 ok=6000 fail=0
[churn1] n=7000 ok=7000 fail=0
[churn0] n=7000 ok=7000 fail=0
[churn1] n=8000 ok=8000 fail=0
[churn0] n=8000 ok=8000 fail=0
[churn1] n=9000 ok=9000 fail=0
[churn0] n=9000 ok=9000 fail=0
[churn1] n=10000 ok=10000 fail=0
[churn0] n=10000 ok=10000 fail=0
[churn1] n=11000 ok=11000 fail=0
[churn0] n=11000 ok=11000 fail=0
[churn1] n=12000 ok=12000 fail=0
[churn0] n=12000 ok=12000 fail=0
[churn1] n=13000 ok=13000 fail=0
[churn0] n=13000 ok=13000 fail=0
[churn1] n=14000 ok=14000 fail=0
```

after which the program will come to a hard crash, due to the kernel freelist corrupting.

to observe the fix in action, just copy the folder `app_program` to the fixed branch, make it, and observe that the crash does not happen

