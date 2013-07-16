MineClone
=========
A Minesweeper clone written with C++ and Qt4 by Kevin Biskar

Installation
------------

### From Source
_Must have a C++ compiler and Qt4 installed and accessible in your path._

```bash
$ git clone https://github.com/biskark/MineClone.git
$ cd MineClone
$ qmake-qt4 -project
$ qmake-qt4
$ make
```

Then either run the executable from within the directory or copy it to your
path.

```bash
$ ./MineClone
```

or

```bash
$ cp MineClone ~/bin
$ MineClone
```

Other
-----
Note that to keep track of best times, this program will create a file in
your home directory called `.mineclone.sav`.
