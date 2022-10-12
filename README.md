### Building and running

May need to install pv: `sudo apt install pv`

Then do:

```
    bash -x build.sh
    ./fizzbuzz | cat | pv | ./main
```

The process can be monitored trivially with monit via the pid file.
Add the following to monit config:

```
check process circular_buffer with pidfile <path_to_this_directory>/circular_buffer.pid
```

Modify the seconds to refresh in the monit configuration by:

`set daemon 1`
