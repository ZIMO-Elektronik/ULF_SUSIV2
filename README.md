# ULF_SUSIV2

[![build](https://github.com/ZIMO-Elektronik/ULF_SUSIV2/actions/workflows/build.yml/badge.svg)](https://github.com/ZIMO-Elektronik/ULF_SUSIV2/actions/workflows/build.yml) [![tests](https://github.com/ZIMO-Elektronik/ULF_SUSIV2/actions/workflows/tests.yml/badge.svg)](https://github.com/ZIMO-Elektronik/ULF_SUSIV2/actions/workflows/tests.yml) [![license](https://img.shields.io/github/license/ZIMO-Elektronik/ULF_SUSIV2)](https://github.com/ZIMO-Elektronik/ULF_SUSIV2/raw/master/LICENSE)

<img src="data/images/logo.svg" width="20%" align="right">

ULF_SUSIV2 is one of serval [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) protocols. It is used to perform [ZPP](https://github.com/ZIMO-Elektronik/ZPP) updates via the SUSI interface. SUSIV2 frames (basically [ZUSI](https://github.com/ZIMO-Elektronik/ZUSI) packets with a preceding header) are transmitted via a serial/CDC interface (e.g. from [ZSP](https://www.zimo.at/web2010/products/zsp_zimo-sound-programmer_EN.htm)) to a decoder update device where they are translated into physical ZUSI commands.

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#protocol">Protocol</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#build">Build</a></li>
      </ul>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>

## Protocol
The protocol consists of only a single frame and is essentially a wrapper around [ZUSI](https://github.com/ZIMO-Elektronik/ZUSI) packets. The following header is prepended to the actual data.
| Length | Description                                                                              |
| ------ | ---------------------------------------------------------------------------------------- |
| 4 byte | Length of the expected answer in byte (not including the ACK/NAK byte)                   |
| 1 byte | 1 - ZUSI command contains a busy phase<br>0 - ZUSI command doesn't contains a busy phase |
| N byte | ZUSI packet                                                                              |

Each SUSIV2 frame is followed by a response. This consists of an ACK or NAK byte and any data sent by the decoder including the CRC8 checksum.

![](data/images/protocol.png)

## Getting Started
### Prerequisites
- C++23 compatible compiler
- [CMake](https://cmake.org/) ( >= 3.25 )

### Installation
This library is meant to be consumed with CMake.

```cmake
#Either by including it with CPM
cpmaddpackage("gh:ZIMO-Elektronik/ULF_SUSIV2@0.0.1")

# or the FetchContent module
FetchContent_Declare(
  ULF_SUSIV2
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/ULF_SUSIV2"
  GIT_TAG 0.0.1)
FetchContent_MakeAvailable(ULF_SUSIV2)

target_link_libraries(YourTarget PRIVATE ULF::SUSIV2)
```

### Build
:construction:

## Usage
:construction: