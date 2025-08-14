# SHA256 Collision Finder

## Description

This project is a C++ program for finding SHA256 hash function collisions using a brute-force method based on the birthday paradox. The program generates random input data, calculates their SHA256 hashes and checks if the hashes for different input data match (collision).

**Important note**: Finding SHA256 collisions is a computationally difficult task (requires about 2^128 operations), so on common hardware, including mobile devices, the probability of finding a collision is extremely small. The program is intended for demonstration and educational purposes.

## Features

- Generating random input strings of 16 bytes.
- Calculating SHA256 hashes using the OpenSSL library.
- Storing hashes in `std::map` for collision checking.
- Real-time progress display: iterations, hashes per second, and saved hashes.
- Minimize log output: live status bar.
- When a collision is found, detailed information is output: two different input messages, their common hash, and execution time.

## Dependencies

- **C++ compiler**: `clang++` or `g++` (supporting C++11 or higher).
- **OpenSSL**: for calculating SHA256 hashes.
- **C++ Standard Library**: for working with `std::map`, `std::random`, `std::chrono`, and other components.

## Install and run

### On Linux/macOS

1. **Install dependencies**:

```bash
sudo apt update
sudo apt install g++ libssl-dev
