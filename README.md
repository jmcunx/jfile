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

To build, execute build.sh to generate a Makefile from
Makefile.template on most BSD/Linux systems and IBM AIX.
For MS-DOS, it should be rather easy to create a Makefile.

This requires [j\_lib2](https://github.com/jmcunx/j_lib2) to build.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, so I came up with my own method which
is a real hack.  But works for me.
