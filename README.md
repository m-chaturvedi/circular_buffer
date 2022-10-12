### Building and running

May need to install pv: `sudo apt install pv`

Then do:

```
    bash -x build.sh
    ./fizzbuzz | cat | pv | ./main
```

The process can be monitored trivially with monit via the pid file.

