# MStree: A robust approach for estimating parameters during speciation with gene flow
MStree is an open-source software for estimating ancestral population sizes and species divergence times during speciation with gene flow.


# Environment
MStree is written in C language. It has been tested under both Linux and Windows systems. 

# Instructions
Download MStree by the following command
```shell
git clone https://github.com/liujunfengtop/MStree
```

You should use gcc or any ANSI C-compatible compiler for UNIX or Windows. The compile command is as follows
```shell
gcc -o mstree mstree.c tools.c -lm
```

Then one can run MStree 
```shell
mstree mstree.ctl
```
Note that the format of control file mstree.ctl is as follows
-（+*） seed = -1 -（+*） outfile = example_out - （+*）seqfile  = example_tree - （+*）getSE = 1000 1000 10 1000
    
 
The detialed instructions can also be found in MStree_User_Guide.pdf
