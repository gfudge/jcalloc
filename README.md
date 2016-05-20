# jcalloc

## Building the example
Clone the repo, cd and make:

`git clone https://github.com/gfudge/jcalloc.git`

`cd jcalloc`

`make`

## Running the example

The `Makefile` will generate a simple `ELF` that will do an allocation and `printf` the result. I highly recommend reading the source as it's less that 200 loc.


## Notes
This is a quick and dirty port of some quick and dirty `ARM` baremetal malloc replacement code, with some special sauce.
