# Redis Clone in C++

A Redis-compatible key-value store built from scratch in C++, implementing the RESP (Redis Serialization Protocol). Compatible with `redis-cli` and any standard Redis client.

## Features
- **RESP Protocol** — fully compatible with `redis-cli`
- **Custom Hash Table** — open addressing with double hashing and dynamic resizing
- **Persistent Connections** — clients stay connected across multiple commands
- **Supported Commands:** SET, GET, DEL, EXISTS, KEYS, FLUSHALL, APPEND, INCR

## Architecture
- `main.cpp` — TCP server, accepts and handles client connections
- `src/hash_table` — custom hash table with prime sizing and sentinel deletion
- `src/command` — command parsing and enum definitions
- `src/resp` — RESP protocol parser and serializer
- `src/prime` — prime number utilities for hash table sizing

## Usage
Build and run the server:
```bash
mkdir cmake-build-debug && cd cmake-build-debug
cmake ..
make
./redis_cpp
```

Connect with redis-cli:
```bash
redis-cli -p 1234
127.0.0.1:1234> set apple 5
OK
127.0.0.1:1234> get apple
"5"
127.0.0.1:1234> incr apple
(integer) 6
127.0.0.1:1234> exists apple
(integer) 1
127.0.0.1:1234> keys
1) "apple"
127.0.0.1:1234> flushall
OK
```

## Implementation Details
- Double hashing for collision resolution
- Load factor kept between 10% and 70% with automatic resizing
- Sentinel deletion to preserve collision chains
- RESP parser handles inline commands and array format