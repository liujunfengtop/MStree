# MStree: A robust approach for estimating parameters during speciation with gene flow
MStree is an open-source software for estimating ancestral population sizes and species divergence times during speciation with gene flow.


# Environment
MStree was written in C language. It has been tested under both Linux and Windows systems. 

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
Note that the format of control file ```mstree.ctl``` is as follows

*   seed = -1 --- random seed , no change
*   outfile = example_out --- output file
*   seqfile  = example_tree --- input file
*   getSE = 1000 1000 10 1000 --- four integers used for adjusting output

For the term “getSE” in the control file, the value of the first number divide by the second number is usually 1. If the branch length in gene tree is not the product of the number of generations and mutation rate, the first number divide by the second number should be the value which can convert the branch length to the product of the number of generations and mutation rate by multiplying it. For example, the branch length in gene tree obtained by ms is in units of 4N (N is effective population size) generations, so the value of the first number divide by the second number should be 4Nμ (μ is the mutation rate), that is the value of parameter t in ms. The value of the third number divide by the fourth number is the threshold value ($\varepsilon $) in MStree (see the approach in supplementary material), we suggest the threshold value should be between 0.01 and 0.03.

$\varepsilon $

For input file ```seqfile```, The input file of MStree is gene tree, which is in Newick format. Gene trees for all loci are in one file, one locus after another. For example, gene trees for eight loci are as follows

```
(3:1.787,(1:0.671,2:0.671):1.116);
(3:2.236,(1:1.288,2:1.288):0.948);
(3:1.390,(1:0.665,2:0.665):0.725);
(3:1.732,(1:0.853,2:0.853):0.879);
(1:1.249,(2:1.218,3:1.218):0.031);
(3:1.383,(1:0.875,2:0.875):0.508);
(1:1.429,(2:1.233,3:1.233):0.196);
(2:3.617,(1:1.544,3:1.544):2.073);
```
 
The detialed instructions can also be found in MStree_User_Guide.pdf
