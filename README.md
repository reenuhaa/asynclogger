# AsyncLogger

A small header-only C++20 asynchronous logger. Producer threads push messages into a bounded thread-safe queue; a single background thread writes them to a file, so callers never block on disk I/O.

## Features

- Bounded queue with backpressure (mutex + condition variables)
- Single writer thread, so log lines never interleave
- Graceful shutdown: remaining messages are flushed before exit
- RAII: the destructor shuts down and joins the worker

## Usage

```cpp
#include "AsyncLogger.hpp"

int main() {
    AsyncLogger logger{"Traces.log"};
    logger.log(TelemetryMessage{std::chrono::steady_clock::now(), "sender1", "OverHeating"});
}   // destructor flushes and joins
```

## Build

Requires a C++20 compiler and CMake.

```bash
mkdir build && cd build
cmake ..
cmake --build .
./telemetry
```

## Files

| File | Purpose |
|------|---------|
| `BoundedQueue.hpp` | Blocking bounded queue (`push`, `pop`, `shutdown`) |
| `AsyncLogger.hpp` | `TelemetryMessage` and the `AsyncLogger` class |
| `main.cpp` | Demo |
