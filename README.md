# VINCE (VIsual Novel Creation Engine)

A wrapper for Allegro 5 to make making VNs easier by enabling Lua support

## Building and running example

### Linux

Requirements:

- cmake
- make
- g++

- liballegro5-dev
- liballegro-image5-dev
- liballegro-ttf5-dev
- liballegro-video5-dev

- liblua5.4-dev

Building:


```bash
mkdir build
cd build
cmake ..
make
```

Running example:

```
./copy_to_example.sh
cd example
./vince
```