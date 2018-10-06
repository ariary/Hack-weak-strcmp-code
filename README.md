# Hack-weak-strcmp-code
A description of a basic hack over a C files using strcmp function. The source file *password.c*  is just here to document and is not supposed to be in the possession of the attacker. We will explain how to hack the executable *a.out*. When we execute we see that we are asked to enter a password and (if you do not have the *password.c* file) you will failed to log in:

    You failed to log in. Try again :-(

**So how to pass this password barrier?**

## Get the assembly code
As we have a binary file we want to examine it. A good way to do that is to get the assembly code.

## Modify it

## Other Way
### Using strings
**Strings** is a Unix program for finding character strings in binary files.

    strings a.out -d
And here is the output:

    /lib64/ld-linux-x86-64.so.2
    libc.so.6
    __isoc99_scanf
    puts
    __stack_chk_fail
    strcmp
    __libc_start_main
    __gmon_start__
    GLIBC_2.7
    GLIBC_2.4
    GLIBC_2.2.5
    UH-P
    AWAVA
    AUATL
    []A\A]A^A_
    Please enter the password:
    hackm3
    Congratulations you are log in
    You failed to log in. Try again :-(
    ;*3$"
You can write a script testing all the possibilities. It is not the most powerful attack because if there is a locking access for too many incorrect passwords would delay getting hacked a bit.

### Using ltrace
**ltrace** is a debugging utility in Linux, used to display the calls a userspace application makes to shared libraries.

    ltrace ./a.out 
Here the output:

    __libc_start_main(0x400646, 1, 0x7ffdb5632e68, 0x4006d0 <unfinished ...>
    puts("Please enter the password:"Please enter the password:
    )      = 27
    __isoc99_scanf(0x400773, 0x7ffdb5632d60, 0x7fc613a0b780, 0x7fc61373c6e0
I enter "test":

    test
    ) = 1
    strcmp("hackm3", "test")                = -12
    puts("You failed to log in. Try again "...You failed to log in. Try again :-(
    ) = 36

We see that the `strcmp("hackm3", "test")` is exposed in clear and so we can retrieve the password.
