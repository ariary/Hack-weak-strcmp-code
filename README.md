# Hack-weak-strcmp-code
A description of a basic hack over a C files using strcmp function. The source file *password.c*  is just here to document and is not supposed to be in the possession of the attacker. We will explain how to hack the executable *a.out*. When we execute we see that we are asked to enter a password and (if you do not have the *password.c* file) you will failed to log in:

    You failed to log in. Try again :-(

**So how to pass this password barrier?**

## Get the assembly code
As we have a binary file we want to examine it. A good way to do that is to get the assembly code.
### Using objdumps
Linux's objdump tool displays information from an object or program including functions and read-only data. The command we run is:

    objdump -d -j.text a.out
this command will enable us to access to the assembly code. Option `-d` disasssembles the data to make it more accessible and `-j`enables selection of specific data for review.
Here is the interesting snipset of the output in which we can see the call of `strcmp`:

    400690:	e8 8b fe ff ff       	callq  400520 <strcmp@plt>
      400695:	85 c0                	test   %eax,%eax
      400697:	75 0c                	jne    4006a5 <main+0x5f>
      400699:	bf 80 07 40 00       	mov    $0x400780,%edi

### Using gdb
We launch the debbugger on the binary:

    gdb -q a.out
We launch the program with a breakpoint on the main function:

    (gdb) break main
    Breakpoint 1 at 0x40064a
    (gdb) run
    Starting program: /path/to/file/a.out 
    
    Breakpoint 1, 0x000000000040064a in main ()
    (gdb)
We can thus disassemble the code:

    (gdb) disas main
    Dump of assembler code for function main:
       ...
       0x0000000000400688 <+66>:	mov    %rax,%rsi
       0x000000000040068b <+69>:	mov    $0x400776,%edi
       0x0000000000400690 <+74>:	callq  0x400520 <strcmp@plt>
       0x0000000000400695 <+79>:	test   %eax,%eax
       0x0000000000400697 <+81>:	jne    0x4006a5 <main+95>
       0x0000000000400699 <+83>:	mov    $0x400780,%edi
       0x000000000040069e <+88>:	callq  0x4004f0 <puts@plt>
       0x00000000004006a3 <+93>:	jmp    0x4006af <main+105>
       0x00000000004006a5 <+95>:	mov    $0x4007a0,%edi
       ...  
    End of assembler dump.

## Modify it
### Using Hexedit
*Hexedit* is a tool that allows you to edit and analyze file in hexadecimal. 
The aim is to change the  `jne` code (`0x75`) to the `je` code (`0x74`) in order to override the conditionnal stop after the `strcmp`.
### Using gdb
We see at 0x0000000000400690 `strcmp`, ie a string comparison function, then a test and a conditional jump (`jne`). We can try to transform the `jne` (jump if not equal) into `je` (jump if equal) and then continue the program. The code of `jne` is `0x75`, and that of je is `0x74`:

       (gdb) set {char}0x0000000000400697=0x74
       (gdb) continue
	    Continuing.
	    Please enter the password:
	    wecantapeanythingevenwrongpwd
	    Congratulations you are log in
	    [Inferior 1 (process 3577) exited normally]
To note that another technique is to observe which strings are comparated by strcmp by putting breakpoint and manipulating register. Here we just bybass the comparaison mechanism

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
You can write a script testing all the possibilities. It is not the most powerful attack because if there is a locking access for too many incorrect passwords, then hacking will be delayed a bit.

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
