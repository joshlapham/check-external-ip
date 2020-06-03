# check-external-ip

By Josh Lapham [josh@joshlapham.com]

License: [Beerware](https://en.wikipedia.org/wiki/Beerware)

## What This Is

Keep track of the current external IP address in a local JSON file. If it has changed since last run, then take action (e.g. update VPN profile file, etc).

## How to Build

Requires [`conan` package manager](https://docs.conan.io/en/1.3/installation.html).

```bash
cd build
conan install ..
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```
