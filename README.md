# DVR

Try to implement Discrete Variable Representation(DVR).
And Play with the dynamic tech in C follow Tsoding's stream: [My Own 3b1b Animation Engine but in C](https://www.youtube.com/watch?v=RVwuYgXNUCM&list=PLpM-Dvs8t0VYH2HouIXMjFFuz7G1FrXiB)

## Build

I using msys64 ucrt64 in Windows and just add `/path/to/ucrt64/bin` to sys path.
Need install dlfcn

```bash
pacman -S mingw-w64-ucrt-x86_64-dlfcn
```

And using [nob.h](https://github.com/tsoding/nob.h) tech by Tsoding to build.

```bash
# start up
gcc -o nob.exe nob.c
```

after that, only need to

```bash
./nob [-r]
```




