
# litehtml

This repository contains [Primate Labs'](https://www.primatelabs.com/) fork of [litehtml](https://github.com/litehtml/litehtml).  litehtml is a lightweight HTML5 rendering engine with CSS3 support.

## litehtml Build Prerequisites

* Python 3.7 or later
* CMake 3.12 or later
* C++14 compiler

## Building litehtml

litehtml uses CMake as its build system.  Build litehtml on macOS or Linux by executing the following commands from the source directory using the terminal:

    mkdir build
    cd build
    cmake ..
    make

You can also build and run the litehtml unit tests by executing the following command from the build directory using the terminal:

    make test

## litehtml Goals

TODO

# headless

litehtml includes headless, a simple headless web browser (i.e., a web browser without a graphical user interface).  headless downloads web pages and save the rendered output to a PNG file.

headless demonstrates how to use litehtml and integrate it into an application.  headless also provides a quick and convenient way to test litehtml against arbitrary web pages.

## headless Build Prerequisites

* Python 3.7 or later
* CMake 3.12 or later
* C++14 compiler
* Libraries
  * fmt
  * fontconfig
  * freetype2
  * libcurl
  * libpng
  * libjpeg

On macOS the easiest way to install the prerequisites is with Homebrew:

    brew install fmt \
        fontconfig \
        freetype \
        libjpeg-turbo \
        libpng \
        pkgconf

On Ubuntu the easiest way to install the prerequisites is with the following command:

    sudo apt install libcurl4-openssl-dev \
        libfmt-dev \
        libfontconfig-dev \
        libjpeg-dev \
        libpng-dev

## Building headless

headless also uses CMake as its build system.  Build litehtml on macOS or Linux by executing the following commands from the terminal:

    git submodule init
    git submodule update
    mkdir build
    cd build
    cmake .. -DBUILD_HEADLESS=1
    make
