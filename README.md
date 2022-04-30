# IPC (Interprocess Communication)

## Signal

```bash
$ make signal
$ ./signal-handler

Starting process...
PID: 28258
```

```bash
$ ./signal-sender 2
```

## Pipe

```bash
$ make pipe
$ ./pipe 10
```

## Socket

```bash
$ make socket
$ ./socket server 8080
```

```bash
$ ./socket client 8080 10
```
