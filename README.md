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

[j\_lib2](https://github.com/jmcunx/j_lib2) is an **optional** dependency.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, but this seems to be good enough for me.

**To compile:**
* If "DESTDIR" is not set, will install under /usr/local
* Execute ./build.sh to create a Makefile
* Works on Linux, BSD and AIX

_To uninstall_, execute
"make uninstall"
from the source directory
