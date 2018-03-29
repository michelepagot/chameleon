# chameleon
Chameleon executable for test purpose

I'm strugling in developing python script to manage external application, start it with right options and monitor and parse its stdou.
But I like to focus just on that and developing code with TDD. So using real application is not an option for me.
So ...

This project provide a pretty simple .c based executable that can change its 'color' like a chameleon. It is an executable that can be instructed to :
   * produce desired stdout
   * produce desired return code
   * store command line options to file

Executable is a single .c file that is 'sensible' to special environment variables. It is provided with simple cmake file so pretty easy to build with:

```
mkdir BUILD
cd BUILD
cmake ..
make
```

Python package is provided to manage this executable and all its functionalities.
