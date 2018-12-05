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

For the term `getSE` in the control file, the value of the first number divide by the second number is usually 1. If the branch length in gene tree is not the product of the number of generations and mutation rate, the first number divide by the second number should be the value which can convert the branch length to the product of the number of generations and mutation rate by multiplying it. For example, the branch length in gene tree obtained by ms is in units of 4N (N is effective population size) generations, so the value of the first number divide by the second number should be 4Nμ (μ is the mutation rate), that is the value of parameter t in ms. The value of the third number divide by the fourth number is the threshold value ![](http://latex.codecogs.com/gif.latex?\\varepsilon) in MStree (see the approach in supplementary material), we suggest the threshold value should be between 0.01 and 0.03.


For input file `seqfile`, The input file of MStree is gene tree, which is in Newick format. Gene trees for all loci are in one file, one locus after another. For example, gene trees for eight loci are as follows

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
Note that. The format ((#:##,#:##):##,#:##) is not available in mstree.

For output file `outfile`, see an example of output below

```
mstree (Version 1.1, July 2018)
The number of tree ((1,2),3)is 7496
The number of tree ((1,3),2)is 1217
The number of tree ((2,3),1)is 1287
theta0 theta1 tau0 tau1 are 0.54292 0.51804 0.59731 0.72000
```

# Example
We simulated gene tree data at multiple loci under isolation-with-initial-migration model, secondary contact model and isolation-with-migration model using the program ms. The two sets of parameter values were used, roughly based on estimates from hominoids and mangroves. They are as follows: 
![](https://github.com/liujunfengtop/MStree/blob/master/equations/equation1.png)(hominoids); ![](https://github.com/liujunfengtop/MStree/blob/master/equations/equation2.png)(mangroves).For the three kinds of models, gene flow is symmetrical and the migration rate (the expected number of migrants per generation) is 1. The number of loci is 10,000 and the number of replicates is 1000. 

* 	Hominoids
  The simulation commands are as follows
  *   Isolation-with-initial-migration model:
  ```shell
  ms 3 10000 -t 0.005 -T -I 3 1 1 1 -m 1 2 0 -m 2 1 0 -em 0.533 1 2 4 -em 0.533 2 1 4 -em 0.8 1 2 0 -em 0.8 2 1 0 -ej 0.8 2 1 -ej 1.2 3 1 | tail -n +4 | grep -v // > tree
  ```
  *   Secondary contact model:
  ```shell
  ms 3 10000 -t 0.005 -T -I 3 1 1 1 -m 1 2 4 -m 2 1 4 -em 0.267 1 2 0 -em 0.267 2 1 0 -ej 0.8 2 1 -ej 1.2 3 1 | tail -n +4 | grep -v // > tree
  ```
  *   Isolation-with-migration model:
  ```shell
  ms 3 10000 -t 0.005 -T -I 3 1 1 1 -m 1 2 4 -m 2 1 4 -em 0.8 1 2 0 -em 0.8 2 1 0 -ej 0.8 2 1 -ej 1.2 3 1 | tail -n +4 | grep -v // > tree
  ```
  



The detialed instructions can also be found in MStree_User_Guide.pdf







