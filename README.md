## jfile -- Examine a Text File and show Character Types

This will read a Text File and list the various type of Characters
found, for example will shows which Characters are:

* UTF-8
* 7-bit ASCII
* ASCII Control (< 0x20)
* ASCII "upper" (> 0x7E)
* What I believe are Invalid UTF-8 values.

I have used it to examine files sent for loading into various
Databases which cannot handle some Characters.

This requires [j\_lib2](https://github.com/jmcunx/j_lib2) to build.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, but this seems to be good enough for me.

**To compile:**
* Set "DESTDIR" for where you want this to install.  Examples:
  * setenv DESTDIR /usr/local
  * export DESTDIR=/usr/local
* this assumes [j\_lib2](https://github.com/jmcunx/j_lib2)
  is also installed under "DESTDIR".
* Edit Makefile, uncomment areas associated to the OS
  you want to compile on.
  OS Sections:
  * Linux 64 bit (default)
  * Linux 32 bit
  * BSD 64 bit
  * BSD 32 bit
  * AIX

_To uninstall_, execute
"make uninstall"
from the source directory
