# BottleSim

## Citation
Kuo CH*, Janzen FJ (2003) BottleSim: a bottleneck simulation program for long-lived species with overlapping generations. Molecular Ecology Notes 3: 669-673. DOI: 10.1046/j.1471-8286.2003.00532.x 

## Synopsis
BottleSim is a simulation program for changes in genetic diversity during the process of population bottlenecks. The major novelties of the program include:
  1. implementation of an overlapping-generation model
  2. ability to simulate a wide range of scenarios regarding changes in population sizes
  3. fexible settings of reproductive systems

Modules included in this version:
  1. Diploid singlelocus with constant population size
  2. Diploid singlelocus with variable population size
  3. Diploid multilocus with constant population size
  4. Diploid multilocus with variable population size

An option of generating raw genotypic data output is also available. The raw genotypic data output file contains the genotypic data from the last year of each iteration. The genotypic data output is in GENEPOP (Raymond and Rousset 1995) format.

The degree of generation overlap indicates the percentage of individuals that being assigned a random age value with the longevity limit. The age of individuals that are not assigned randomly is set to zero in year0.

The operation is self-explanatory, details of the algorithm are provided in Kuo and Janzen (2003).

## Program limitation
The simulation process of this program is only limited by the amount of memory (RAM) available. (Mac OS 9 users: use command+I to change the amount of memory assigned to the program.)
Other limitations regarding input/output filenames and input data:
  1. Max. length of input and output filenames = 20 (characters)
  2. Max. length of locus name = 10 (characters)
  3. Max. length of individual's ID = 10 (characters)

## Frequently Asked Questions (FAQ)
Several users contacted me about the program quits without generating any error message (usually when running on a Windows machine). It turned out that this is often caused by a combination of small population size and a high maturation setting. When the population size is small (for any given year) and the maturation setting is high, it is possible that all males (or all females) in the population are juveniles just by chance. Under this scenario, the program encounters a logical error when it needs to generate new individuals. The program should generate an error message like this:

```
*** Population size setting error
*** Number of breeding male < 1
*** Unable to generate new individuals
```

before it quits. For some unknown reasons, Windows may just close the application window without showing the error message.

Currently there are no good solutions to this problem other than to increase the population size or to lower the maturation setting.

## References
Kuo CH, Janzen FJ. 2003. BottleSim: a bottleneck simulation program for long-lived species with overlapping generations. Molecular Ecology Notes, 3, 669-673. DOI: 10.1046/j.1471-8286.2003.00532.x
Raymond M, Rousset F. 1995 GENEPOP (version 1.2): population genetics software for exact tests and ecumenicism. Journal of Heredity, 86, 248-249.
