# FreeRTOS Emulator

<p>
  <a href="https://travis-ci.com/alxhoff/FreeRTOS-Emulator">
  <img src="https://travis-ci.com/alxhoff/FreeRTOS-Emulator.svg?branch=master">
  </a>
  <a href="https://github.com/alxhoff/FreeRTOS-Emulator/blob/master/LICENSE">
    <img src="https://img.shields.io/badge/license-GPLv3-blue.svg" />
  </a>
</p>

An implementation of POSIX based FreeRTOS with the combination of SDL2 graphics. Aimed at providing an x86 emulation solution for teaching FreeRTOS to students without the need of embedded hardware.

Based on the FreeRTOS (V5.X) simulator developed by William Davy. Updated to use FreeRTOS V9.0.0.

## Dependencies

The simulator uses the SDL2 graphics libraries.

### Debian/Ubuntu

```bash
sudo apt-get install libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-gfx-dev libsdl2-dev

```
### Arch

```bash
sudo pacman -S sdl2 sdl2_gfx sdl2_image sdl2_mixer sdl2_ttf
```
### Windows/Mac
¯\\(°_o)/¯

....install linux?

## Building

```bash
cd build
cmake ..
make
```

For those requiring an IDE run
```bash
cmake -G "Eclipse CDT4 - Unix Makefiles" ./
```
to generate the appropriate project files to allow for the emulator to be imported into Eclipse.

## Debugging

The emulator uses the signals `SIGUSR1` and `SIG34` and as such GDB needs to be told to ignore the signal.
An appropriate `.gdbinit` is in the `bin` directory.
Copy the `.gdbinit` into your home directory or make sure to debug from the `bin` directory.
Such that GDB does not get interrupted by the POSIX signals used by the emulator for IPC.

If using an IDE, make sure to configure your debug to load the gdbinit file.

## Documentation

Documentation of the crude graphics library, found in `lib/Gfx`, and the example collision demo code library, can be generated by running

``` bash
doxygen
```
from the `docs` directory to generate the Doxygen documentation. Also generated on GitHub via Travis CI, available [*here*](https://alxhoff.github.io/FreeRTOS-Emulator/).

## Asynchronous IO

There are three implemented methods to perform asynchronous IO, with the hope of emulating the sort of IO you find on micro-controllers. The three methods are UDP and TCP sockets and POSIX message queues.

### TCP/UDP Sockets

Both protocols have the same functionality, differing only in that the UDP callback is called for every received transmission whilst the TCP callback is called for every established connection. Both work by establishing a connection to the given IPv4 dot-decimal address and port.

### POSIX Message Queues

Similarly to UDP sockets, message queues trigger their callback once for each message received. The message queue names, unlike the Linux POSIX implementation, do not require the prepending '/'.

### Callbacks

Each type of connection can be assigned its own callback function. The callback, if provided (else `NULL`), must take three arguments: `size_t`, `char *`, `void *`. These parameters, respectively, provide the callback with the number of bytes received into the buffer, the buffer and the args given as an argument when the connection was created.

### Sending to Connections

`aIOMessageQueuePut` and `aIOSocketPut` can be used to send a `char *` buffer to either a message queue (specified using the message queue's name) or a socket (specified using an IPv4 dot-decimal address and a port).

## Example

Pong game implementation on pong branch


<a href="https://www.buymeacoffee.com/xmyWYwD" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/lato-green.png" alt="Buy Me A Coffee" style="height: 11px !important;" ></a>

## YouCompleteMe Integration

This is not really related to the project but as I have left in the YCM symlinks and options in the CMake I may as well detail the YouCompleteMe Vim integration as it is applicable for other projects as well.

### Install vim-plug

#### Ubuntu users

Ubuntu does not yet install vim 8.X+ by default and as you must install it manually first

``` bash
sudo add-apt-repository ppa:jonathonf/vim
sudo apt update
sudo apt install vim
```
#### Installation

##### Prerequisites

A python version >= 3.5 is required.

``` bash
sudo apt install build-essential cmake3 python3.5-dev python3.5
```

##### Vim plugin manager

``` bash
curl -fLo ~/.vim/autoload/plug.vim --create-dirs \
    https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
```

##### YCM

Add YCM to vimrc

``` bash
echo "call plug#begin('~/.vim/plugged')" >> $HOME/.vimrc
echo "Plug 'valloric/youcompleteme'" >> $HOME/.vimrc
echo "call plug#end()" >> $HOME/.vimrc
```

Start vim and run `:PlugInstall`

Navigate to vim plugin folder and run install script

```bash
cd $HOME/.vim/plugged/youcompleteme
python3.5 install.py --clang-completer
```
Get the config script

``` bash
curl -Lo $HOME/.ycm_extra_conf.py https://raw.githubusercontent.com/alxhoff/dotfiles/master/ycm/.ycm_extra_conf.py
```

After running, you should be able to complete using CRTL+Space
