// BottleSim source code
// Chih-Horng Kuo <chk@gate.sinica.edu.tw>

// preprocessor directive
#include <iostream.h>
#include <iomanip.h>
#include <string.h>
#include <ctype.h>
#include <fstream.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <vector.h>
#include <algo.h>

// For standard Template Library (STL)
using namespace std;

// constants 
const char programName[21] = "BottleSim";
    // program name
const char versionNumber[15] = "2.6.1(Linux)";
    // version number
const char versionDate[15] = "01/09/2006";
    // version date
const char author[61] = "Chih-Horng Kuo & Fred Janzen";
    // author
const char email[21] = "chk@gate.sinica.edu.tw";
    // author email
const char web[61] = "https://ipmb.sinica.edu.tw/chkuo/";
    // author web
const int fileNameLength = 21;
    // filename length
const int locusNameLength = 11;
    // locus name length
const int idLength = 11;
	// length of character array that holds individual id
const char inputMissingValue = '?';
    // default value for input missing value
const short maxAllele = 62;
    // maximum number of alleles per locus

// possible allele list for converting genotype input file
const char arrayAlleleId[maxAllele] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
                                       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
                                       'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
                                       'U', 'V', 'W', 'X', 'Y', 'Z',
                                       'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                       'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                                       'u', 'v', 'w', 'x', 'y', 'z'};

// typedef
typedef char fileName[fileNameLength];
    // new data type "fileName" to hold file name
typedef char locusName[locusNameLength];
    // new data type "locusName"  to hold locus name
typedef char individualId[idLength];
	// new data type "individualId" to hold individual id

// function prototypes
    // infomation display
    void welcome(void);
    void memoryError(void);
    // get settings
    int moduleSelection(void);
    void getCPSSimSetting(int &degreeOverlap, int &reproSys, int &longevity, int &ageMaturation, int &nPB, int &nB, int &nFemale, int &nMale, int &nYear, int &nIteration, int &boolOutputRawGenotype);
    void getVPSSimSetting(int &degreeOverlap, int &reproSys, int &longevity, int &ageMaturation, int &nIteration, int &boolOutputRawGenotype);
    int getDegreeOverlap(void);
    int getReproSys(void);
    int getLongevity(void);
    int getAgeMaturation(int longevity);
    int getNPB(void);
    int getNB(void);
    int getNFemale(int nB);
    int getNYear(void);
    int getNIteration(void);
    int getBoolOutputRawGenotype(void);
    int getNMaxPopSize(int *nPopSize, int nYear);
    // get random number
    int getRandomNumber(int min, int max);
    // get allele count
    void getDiSLAlleleCount(int *arrayAlleleCount, double *inputAlleleFreq, int nAllele, int nPopSize, int allelePerLocus);
    // get new genotype
    void getNewDiSLGenotype(int **arrayParent, int *arrayNewGenotype, int reproSys, int nAdultTotal, int nFemale, int parent1, int parent2);
    void getNewDiMLGenotype(int ***arrayParent, int **arrayNewGenotype, int reproSys, int nAdultTotal, int nFemale, int parent1, int parent2, int nLoci);
    // read input file
    int readInputFileNAllele(ifstream &alleleFreqInput);
    void readInputFileAlleleFreq(ifstream &alleleFreqInput, double *inputAlleleFreq, int nAllele);
    int readInputFileNYear(ifstream &popSizeInput);
    void readInputFilePopSize(ifstream &popSizeInput, int *nPopSize, int *nFemale, int *nMale, int nYear, int reproSys);
    int readInputFileNSize(ifstream &genotypeInputFile);
    int readInputFileNLoci(ifstream &genotypeInputFile);
    void readInputFileLocusName(ifstream &genotypeInputFile, locusName *arrayInputLocus, int nLoci);
    void readInputFileGenotype(ifstream &genotypeInputFile, individualId *arrayInputId, char ***arrayInput, int nSize, int nLoci, int allelePerLocus);
    // display
    void displayAlleleFreqInput(double *inputAlleleFreq, int nAllele);
    void displayVPopSizeInput(int *nPopSize, int *nFemale, int reproSys, int nYear);
    void displayGenotypeInput(locusName *arrayInputLocus, individualId *arrayInputId, char ***arrayInput, int nSize, int nLoci, int allelePerLocus);
    // verify input
    void checkAlleleFreqInput(double *inputAlleleFreq, int nAllele);
    void checkVPopSsizeInput(int *nPopSize, int *nFemale, int reproSys, int nYear);
    // write setting
    void writeOutputHeader(ofstream &outputFile);
    void writeSLRawGenotypeHeader(ofstream &rawGenotypeOutputFile);
    void writeMLRawGenotypeHeader(ofstream &rawGenotypeOutputFile, locusName *arrayInputLocus, int nLoci);
    void writeMainSetting(ofstream &outputFile, int module, fileName inputFileName, fileName outputFileName);
    void writeVPopSizeInput(ofstream &outputFile, int *nPopSize, int *nFemale, int reproSys, int nYear);       
    void writeAlleleFreq(ofstream &outputFile, int nLoci, locusName *arrayInputLocus, float **arrayAlleleFreq);
    void writeCPSSimSetting (ofstream &outputFile, int degreeOverlap, int reproSys, int longevity, int ageMaturation, int nPB, int nB, int nFemale, int nMale, int nYear, int nIteration);
    void writeVPSSimSetting (ofstream &outputFile, int degreeOverlap, int reproSys, int longevity, int ageMaturation, int nYear, int nIteration);
    void writeDegreeOverlap(ofstream &outputFile, int degreeOverlap);
    void writeReprosys(ofstream &outputFile, int reproSys);
    void writeLongevity(ofstream &outputFile, int longevity);
    void writeAgeMaturation(ofstream &outputFile, int ageMaturation);
    void writeNPB(ofstream &outputFile, int nPB);
    void writeNB(ofstream &outputFile, int nB);
    void writeSexRatio(ofstream &outputFile, int nFemale, int nMale);
    void writeNYear(ofstream &outputFile, int nYear);
    void writeNIteration(ofstream &outputFile, int nIteration);
    // write output
    void writeDiSLSummary(ofstream &outputFile, int nYear, int nIteration, int **arrayRawOA, float **arrayRawEA, float **arrayRawHo, float **arrayRawHe, float **arrayRawF);
    void writeDiMLSummary(ofstream &outputFile, int nLoci, locusName *arrayInputLocus, int nYear, int nIteration, int ***arrayRawOA, float ***arrayRawEA, float ***arrayRawHo, float ***arrayRawHe, float ***arrayRawF);
    void writeFixDiploidSingleLocus(ofstream &outputFile, int nIteration, int fixCase, float fixProb);
    void writeFixDiploidMultiLocus(ofstream &outputFile, int nLoci, locusName *arrayInputLocus, int nIteration, int *fixCase, float *fixProb);
    void writeSingleLocus(ofstream &outputFile, int nYear, float *arrayMean, float *arrayStdD, float *arrayStdE, float *arrayPercent);
    void writeMultiLocus(ofstream &outputFile, int nLoci, locusName *arrayInputLocus, int nYear, float **arrayMean, float *arrayAv, float *arrayStdD, float *arrayStdE, float *arrayPercent);
    void writeDiSLRawGenotype(ofstream &rawGenotypeOutputFile, int **arrayGenotype, int allelePerLocus, int nPopSize);
    void writeDiMLRawGenotype(ofstream &rawGenotypeOutputFile, int ***arrayGenotype, int allelePerLocus, int nPopSize, int nLoci);
    // calculation
    void statGenotypeInput(char ***arrayInput, int **arrayAlleleCount, int *alleleTotalCount, float **arrayAlleleFreq, int nSize, int nLoci, int allelePerLocus);
    void statFixDiploidSingleLocus(int **arrayRawOA, int nYear, int nIteration, int &fixCase, float &fixProb);    
    void statFixDiploidMultiLocus(int ***arrayRawOA, int nLoci, int nYear, int nIteration, int *fixCase, float *fixProb);
    void statDiploidSingleLocus(int **arrayGenotype, int allelePerLocus, int nPopSize, int nAllele, int &nOA, float &nEA, float &nHo, float &nHe, float &nF);
    void statDiploidMultiLocus(int ***arrayGenotype, int allelePerLocus, int nPopSize, int nLoci, int *arrayOA, float *arrayEA, float *arrayHo, float *arrayHe, float *arrayF);
    void outputDiploidSingleLocus(int **arrayRawdata,  int nYear, int nIteration, float *arrayMean, float *arrayStdD, float *arrayStdE, float *arrayPercent);
    void outputDiploidSingleLocus(float **arrayRawdata,  int nYear, int nIteration, float *arrayMean, float *arrayStdD, float *arrayStdE, float *arrayPercent);
    void outputDiploidMultiLocus(int ***arrayRawdata,  int nLoci, int nYear, int nIteration, float **arrayMean, float *arrayAv, float *arrayStdD, float *arrayStdE, float *arrayPercent);
    void outputDiploidMultiLocus(float ***arrayRawdata,  int nLoci, int nYear, int nIteration, float **arrayMean, float *arrayAv, float *arrayStdD, float *arrayStdE, float *arrayPercent);
    // file operation 
    void openInput(ifstream &inputFile, fileName &inputFileName);
    void openPopSizeInput(ifstream &popSizeInput, fileName &popSizeInputName);
    void openOutput(ofstream &outputFile, fileName &outputFileName);
    void openRawGenotypeOutput(ofstream &rawGenotypeOutputFile, fileName &rawGenotypeOutputName);
    void closeInputFile(ifstream &inputFile);
    void closeOutputFile(ofstream &outputFile);
    // simulation module
    void simDiploidSingleLocusCPS(void);
    void simDiploidSingleLocusVPS(void);
    void simDiploidMultiLocusCPS(void);
    void simDiploidMultiLocusVPS(void);


// start of program functions
// main function
int main(void)
{
    
    // call welcome function to display welcome message
    welcome();
    
    // seed random number generator with current system time
    srand(time(0));
    
    // declare variables
    int module = 0; //module used for the run
        
    // call moduleSelection function
    module = moduleSelection();    

    // call selected module
        switch (module)
        {
            case 1: // diploid singlelocus, constant population size
                    // call simDiploidSingleLocusCPS function
                    simDiploidSingleLocusCPS();
                    break;
            case 2: // diploid singlelocus, variable population size
                    // call simDiploidSingleLocusVPS function
                    simDiploidSingleLocusVPS();
                    break;
            case 3: // diploid multilocus, constant population size
                    // call simDiploidMultiLocusCPS function
                    simDiploidMultiLocusCPS();
                    break;
            case 4: // diploid multilocus, variable population size
                    // call simDiploidMultiLocusVPS function
                    simDiploidMultiLocusVPS();
                    break;
            default: cout << endl;
                     cout << "*** Simulation module selection error" << endl;  
                     exit(0);
        } // end of switch    

	// end, display message
    	cout << endl;
    	cout << "*** Program ended. ***" << endl;
        return 0;   

} // end of main function



// welcome function
// display welcome info
void welcome(void)
{
    // display information for users
    cout << "*** " << programName << " ***" << endl << endl;
    cout << "Version number: " << versionNumber << endl;
    cout << "Release date: " << versionDate << endl;
    cout << endl;

    cout << "Contact information" << endl; 
    cout << "  Author: " << author << endl; 
    cout << "  email: " << email << endl;
    cout << "  Web: " << web << endl;
    cout << endl;

    cout << "Description:" << endl;
    cout << "    This program is a population bottleneck simulator. "
         << "Please see distibution web site for detailed description, "
         << "input file format, program limitation, " 
         << "and version updates."
         << endl;
    cout << endl; 

} // end of welcome function

// memoryError function
void memoryError(void)
{
    cout << endl;
    cout << "*** Error allocating memory!" << endl;
    exit(0);                     

}

// moduleSelection function
// display modules available and return user selection to main function
int moduleSelection(void)
{
    // declare variables
    int module = 0;

    // display available modules 
    cout << endl;
    cout << "Simulation modules available:" << endl;
    cout << "  (1) Diploid singlelocus, constant population size" << endl;
    cout << "  (2) Diploid singlelocus, variable population size" << endl;
    cout << "  (3) Diploid multilocus, constant population size" << endl;
    cout << "  (4) Diploid multilocus, variable population size" << endl;
    
    // prompt for user selection
    do
    {
        cout << endl;
        cout << "Please choose simulation module: ";
        cin >> module;  
    } while (module < 1 || module > 4); // end of do-while loop
    
    // return user selection to main
    cout << endl; 
    cout << "Module selected = " << module << endl;
    return module;

} // end of moduleSelection function

// getCPSSimSetting function
void getCPSSimSetting(int &degreeOverlap, int &reproSys, int &longevity, int &ageMaturation, int &nPB, int &nB, int &nFemale, int &nMale, int &nYear, int &nIteration, int &boolOutputRawGenotype)
{
    // get simulation settings
        // display
        cout << endl;
        cout << "Simulation settings:" << endl;
        
        // call get simulation setting functions
        degreeOverlap = getDegreeOverlap();
            // overlapping generation setting
        reproSys = getReproSys();
            // reproductive system option
        longevity = getLongevity();
            // longevity the organism
        ageMaturation = getAgeMaturation(longevity);
            // age of reproduvtive maturation
        nPB = getNPB();
            // pre-bottleneck population size 
        nB = getNB();
            // bottlneck population size 
            
            // number of female/male for diecious models 
            if (reproSys == 5 || reproSys == 6 || reproSys == 7)
            {
                nFemale = getNFemale(nB);
                    // number of female individuals
                nMale = (nB - nFemale);
                    // number of male individuals
            }
            
        nYear = getNYear();
            // number of years to simulate
        nIteration = getNIteration();
            // number of iterations of the run

        boolOutputRawGenotype = getBoolOutputRawGenotype();
            // bool for output raw genotype data

} // end of getCPSSimSetting function

// getVPSSimSetting function
void getVPSSimSetting(int &degreeOverlap, int &reproSys, int &longevity, int &ageMaturation, int &nIteration, int &boolOutputRawGenotype)
{
    // get simulation settings
        // display
        cout << endl;
        cout << "Simulation settings:" << endl;
        
        // call get simulation setting functions
        degreeOverlap = getDegreeOverlap();
            // overlapping generation setting
        reproSys = getReproSys();
            // reproductive system option
        longevity = getLongevity();
            // longevity of the organism
        ageMaturation = getAgeMaturation(longevity);
            // age of reproduvtive maturation
        nIteration = getNIteration();
            // number of iterations of the run

        boolOutputRawGenotype = getBoolOutputRawGenotype();
            // bool for output raw genotype data

} // end of getVPSSimSetting function

// getDegreeOverlap function
int getDegreeOverlap(void)
{
    int degreeOverlap;
    
    do
    {
        cout << endl
             << "(0)   = Minimum overlap" << endl
             << "        (discrete generations, all individual start with age 0)" << endl 
             << "(100) = Maximum overlap" << endl 
             << "        (all individual start with random age value)" << endl
             << "Please enter generation overlap setting (0-100): ";
        cin >> degreeOverlap;        
    } while (degreeOverlap < 0 || degreeOverlap > 100);
    
    return degreeOverlap;
} // end of getDegreeOverlap function

// getReproSys function
int getReproSys(void)
{
    int reproSys;
    
    do
    {
        cout << endl
             << "(1) = Asexual reproduction" << endl
             << "(2) = Monoecy with complete selfing" << endl
             << "(3) = Monoecy with random mating (with selfing)" << endl
             << "(4) = Monoecy with random mating (without selfing)" << endl
             << "(5) = Dioecy with random mating" << endl
             << "(6) = Dioecy with single reproducing male each year" << endl
             << "(7) = Dioecy with single reproducing pair each year" << endl
             << "Please choose reproductive system of the organism: ";
        cin >> reproSys;
    } while (reproSys < 1 || reproSys > 7);     
           
    return reproSys;
} // end of getReproSys function

// getLongevity function
int getLongevity(void)
{
    int longevity;
    
    do
    {
        cout << endl
             << "Please enter the longevity of the organism (year): ";
        cin >> longevity;        
    } while (longevity < 1);
    
    return longevity;
} // end of getLongevity function


// getAgeMaturation function
int getAgeMaturation(int longevity)
{
    int ageMaturation;
    
    do
    {
        cout << endl
             << "Please enter the age of reproductive maturation: ";
        cin >> ageMaturation;
    } while (ageMaturation < 0 || ageMaturation > longevity);
    
    return ageMaturation;
} // end of getAgeMaturation function

// getNPB function
int getNPB(void)
{
    int nPB;
    
    do
    {   
        cout << endl
             << "Please enter the population size before the bottleneck: ";
        cin >> nPB;
    } while (nPB < 1);
    
    return nPB;
} // end of getNPB function

// getNB function
int getNB(void)
{
    int nB;
    
    do
    {   
        cout << endl
             << "Please enter the population size during the bottleneck: ";
        cin >> nB;
    } while (nB < 1);
    
    return nB;
} // end of getNB function

// getNFemale function
int getNFemale(int nB)
{
    int nFemale;

    do
    {
        cout << endl
             << "Please enter the number of females in the population during bottleneck: ";
        cin >> nFemale;
    } while (nFemale < 1 || nFemale > nB);

    return nFemale;
} // end of getNFemale function
            
// getNYear function
int getNYear(void)
{
    int nYear;
    
    do
    {    
        cout << endl
             << "Please enter the number of years to simulate: ";
        cin >> nYear;
    } while (nYear < 0);
    
    return nYear;
} // end of getNYear function        

// getNIteration function
int getNIteration(void)
{
    int nIteration;
    
    do
    {    
        cout << endl
             << "Please enter the number of iterations you want to perform: ";
        cin >> nIteration;
    } while (nIteration < 1);
    
    return nIteration;
} // end of getNIteration function

// getBoolOutputRawGenotype function
int getBoolOutputRawGenotype(void)
{
    int boolOutputRawGenotype;
    char yesNo;

    do
    {
        cout << endl
             << "Generate genotype raw data output file?" << endl
             << "  ***CAUTION***" << endl
             << "  The raw data output file can be very large, " << endl
             << "  please consider the simulation settings used" << endl
             << "  (population size, loci number, iteration number)." << endl
             << endl
             << "Enter Y or N: ";
        cin >> yesNo;
    } while (yesNo != 'Y' && yesNo != 'y' && yesNo != 'N' && yesNo != 'n');
    
    if (yesNo == 'Y' || yesNo == 'y')
        boolOutputRawGenotype = 1;
    else
        boolOutputRawGenotype = 0;
    
    return boolOutputRawGenotype;

} // end of getBoolOutputRawGenotype function


// getNMaxPopSize function
int getNMaxPopSize(int *nPopSize, int nYear)
{
    int nMaxPopSize;
        // maximum population size in pop size input
    int countY;

    nMaxPopSize = nPopSize[0];
    for (countY = 0; countY < (nYear+1); countY++)
    {
        if (nMaxPopSize < nPopSize[countY])
        {
            nMaxPopSize = nPopSize[countY];
        }
    }

    return nMaxPopSize;
} // end of getNMaxPopSize function


// getRandomNumber function
// receive minimum and maximum of the random number desired
// return random number within the min/max range
int getRandomNumber(int min, int max)
{
    return ((rand() % (max + 1 - min)) + min);
} // end of getRandomNumber function

// getDiSLAlleleCount function
void getDiSLAlleleCount(int *arrayAlleleCount, double *inputAlleleFreq, int nAllele, int nPopSize, int allelePerLocus)
{
    int countA;
    
    
    // calculate allele count for each allele according to input allele freq
    for (countA = 0; countA < nAllele; countA++)
    {
        arrayAlleleCount[countA] = int ((nPopSize * allelePerLocus) * inputAlleleFreq[countA]);
    }

    // check allele count 

    int sumAlleleCount = 0;

    for (countA = 0; countA < nAllele; countA++)
    {
        sumAlleleCount += arrayAlleleCount[countA]; 
    }

    if (sumAlleleCount != (nPopSize * allelePerLocus))
    {
        cout << endl;
        cout << "*** Input allele frequency error." << endl;
        cout << "*** Allele frequency in the input file resulted in non-integer allele count in year0 population." << endl; 
        exit(0);
    }


} // end of getDiSLAlleleCount function


// getNewDiSLGenotype function
void getNewDiSLGenotype(int **arrayParent, int *arrayNewGenotype, int reproSys, int nAdultTotal, int nFemale, int parent1, int parent2)
{
    // check nAdultTotal/nFemale/nMale
    switch (reproSys)
    {
        case 1: // asexual reproduction
        case 2: // Monoecious - complete selfing
        case 3: // Monoecious - random mating with selfing
                if (nAdultTotal < 1) // check nAdultTotal
                {
                    cout << "*** Population size setting error" << endl
                         << "*** Number of adults < 1" << endl
                         << "*** Unable to generate new individuals" << endl;
                    exit(0);
                }
                break;
        case 4: // Monoecious - random mating without selfing
                if (nAdultTotal < 2) // check nAdultTotal
                {
                    cout << "*** Population size setting error" << endl
                         << "*** Number of adults < 2" << endl
                         << "*** Unable to generate new individuals" << endl;
                    exit(0);
                    exit(0);
                }
                break;
        case 5: // Diecious - random mating
        case 6: // Diecious - single reproducing male
        case 7: // Diecious - single reproducing pair
                if (nAdultTotal < 2) // check nAdultTotal
                {
                    cout << "*** Population size setting error" << endl
                         << "*** Number of adults < 2" << endl
                         << "*** Unable to generate new individuals" << endl;
                    exit(0);
                    exit(0);
                }
                if (nFemale < 1) // check nFemale
                {
                    cout << "*** Population size setting error" << endl
                         << "*** Number of breeding female < 1" << endl
                         << "*** Unable to generate new individuals" << endl;
                    exit(0);
                }
                if ((nAdultTotal - nFemale) < 1) // check nMale
                {
                    cout << "*** Population size setting error" << endl
                         << "*** Number of breeding male < 1" << endl
                         << "*** Unable to generate new individuals" << endl;
                    exit(0);
                }
                break;
        default: cout << endl;
                 cout << "*** Reproductive system setting error" << endl;
                 exit(0);                     
                
    } // end of reproSys switch 

    // get new genotype
    switch (reproSys)
    {
        case 1: // asexual reproduction
                // get parent1
                parent1 = getRandomNumber(0, (nAdultTotal - 1));
                // allele1 from parent1
                arrayNewGenotype[0] = arrayParent[parent1][0];
                // allele2 from parent1
                arrayNewGenotype[1] = arrayParent[parent1][1];
                break;                             
        case 2: // Monoecious - complete selfing
                // get parent1
                parent1 = getRandomNumber(0, (nAdultTotal - 1));
                // allele1 from parent1
                arrayNewGenotype[0] = arrayParent[parent1][getRandomNumber(0, 1)];
                // allele2 from parent1
                arrayNewGenotype[1] = arrayParent[parent1][getRandomNumber(0, 1)];
                break;
        case 3: // Monoecious - random mating with selfing
                // get parent1
                parent1 = getRandomNumber(0, (nAdultTotal - 1));
                // get parent2
                parent2 = getRandomNumber(0, (nAdultTotal - 1));
                // allele from parent1
                arrayNewGenotype[0] = arrayParent[parent1][getRandomNumber(0, 1)];
                // allele from parent2
                arrayNewGenotype[1] = arrayParent[parent2][getRandomNumber(0, 1)];
                break;
        case 4: // Monoecious - random mating without selfing
                // get parent1 
                parent1 = getRandomNumber(0, (nAdultTotal - 1));
                // get parent2
                do
                {
                    parent2 = getRandomNumber(0, (nAdultTotal - 1));
                } while (parent2 == parent1);
                // allele from parent1
                arrayNewGenotype[0] = arrayParent[parent1][getRandomNumber(0, 1)];
                // allele from parent2
                arrayNewGenotype[1] = arrayParent[parent2][getRandomNumber(0, 1)];
                break;
        case 5: // Diecious - random mating
                // get parent1 and parent2
                parent1 = getRandomNumber(0, (nFemale - 1));
                parent2 = getRandomNumber(nFemale, (nAdultTotal - 1));
                // allele from parent1
                arrayNewGenotype[0] = arrayParent[parent1][getRandomNumber(0, 1)];
                // allele from parent2
                arrayNewGenotype[1] = arrayParent[parent2][getRandomNumber(0, 1)];
                break;
        case 6: // Diecious - single reproducing male
                // get parent1 
                parent1 = getRandomNumber(0, (nFemale - 1));
                // allele from parent1
                arrayNewGenotype[0] = arrayParent[parent1][getRandomNumber(0, 1)];
                // allele from parent2
                arrayNewGenotype[1] = arrayParent[parent2][getRandomNumber(0, 1)];
                break;
        case 7: // Diecious - single reproducing pair
                // allele from parent1
                arrayNewGenotype[0] = arrayParent[parent1][getRandomNumber(0, 1)];
                // allele from parent2
                arrayNewGenotype[1] = arrayParent[parent2][getRandomNumber(0, 1)];
                break;
        default: cout << endl;
                 cout << "*** Reproductive system setting error" << endl;
                 exit(0);                     
                
    } // end of reproSys switch 
    
} // end of getNewDiSLGenotype

// getNewDiMLGenotype function
void getNewDiMLGenotype(int ***arrayParent, int **arrayNewGenotype, int reproSys, int nAdultTotal, int nFemale, int parent1, int parent2, int nLoci)
{
    int countL;

    // check nAdultTotal/nFemale/nMale
    switch (reproSys)
    {
        case 1: // asexual reproduction
        case 2: // Monoecious - complete selfing
        case 3: // Monoecious - random mating with selfing
                if (nAdultTotal < 1) // check nAdultTotal
                {
                    cout << "*** Population size setting error" << endl
                         << "*** Number of adults < 1" << endl
                         << "*** Unable to generate new individuals" << endl;
                    exit(0);
                }
                break;
        case 4: // Monoecious - random mating without selfing
                if (nAdultTotal < 2) // check nAdultTotal
                {
                    cout << "*** Population size setting error" << endl
                         << "*** Number of adults < 2" << endl
                         << "*** Unable to generate new individuals" << endl;
                    exit(0);
                    exit(0);
                }
                break;
        case 5: // Diecious - random mating
        case 6: // Diecious - single reproducing male
        case 7: // Diecious - single reproducing pair
                if (nAdultTotal < 2) // check nAdultTotal
                {
                    cout << "*** Population size setting error" << endl
                         << "*** Number of adults < 2" << endl
                         << "*** Unable to generate new individuals" << endl;
                    exit(0);
                    exit(0);
                }
                if (nFemale < 1) // check nFemale
                {
                    cout << "*** Population size setting error" << endl
                         << "*** Number of breeding female < 1" << endl
                         << "*** Unable to generate new individuals" << endl;
                    exit(0);
                }
                if ((nAdultTotal - nFemale) < 1) // check nMale
                {
                    cout << "*** Population size setting error" << endl
                         << "*** Number of breeding male < 1" << endl
                         << "*** Unable to generate new individuals" << endl;
                    exit(0);
                }
                break;
        default: cout << endl;
                 cout << "*** Reproductive system setting error" << endl;
                 exit(0);                     
                
    } // end of reproSys switch 


    // get new genotype    
    switch (reproSys)
    {
        case 1: // asexual reproduction
                // get parent1
                parent1 = getRandomNumber(0, (nAdultTotal - 1));
                for (countL = 0; countL < nLoci; countL++)
                {
                    // allele1 from parent1
                    arrayNewGenotype[countL][0] = arrayParent[parent1][countL][0];
                    // allele2 from parent1
                    arrayNewGenotype[countL][1] = arrayParent[parent1][countL][1];
                }
                break;                             
        case 2: // Monoecious - complete selfing
                // get parent1
                parent1 = getRandomNumber(0, (nAdultTotal - 1));
                for (countL = 0; countL < nLoci; countL++)
                {
                    // allele1 from parent1
                    arrayNewGenotype[countL][0] = arrayParent[parent1][countL][getRandomNumber(0, 1)];
                    // allele2 from parent1
                    arrayNewGenotype[countL][1] = arrayParent[parent1][countL][getRandomNumber(0, 1)];
                }
                break;
        case 3: // Monoecious - random mating with selfing
                // get parent1
                parent1 = getRandomNumber(0, (nAdultTotal - 1));
                // get parent2
                parent2 = getRandomNumber(0, (nAdultTotal - 1));
                for (countL = 0; countL < nLoci; countL++)
                {
                    // allele from parent1
                    arrayNewGenotype[countL][0] = arrayParent[parent1][countL][getRandomNumber(0, 1)];
                    // allele from parent2
                    arrayNewGenotype[countL][1] = arrayParent[parent2][countL][getRandomNumber(0, 1)];
                }
                break;
        case 4: // Monoecious - random mating without selfing
                // get parent1 and parent2
                parent1 = getRandomNumber(0, (nAdultTotal - 1));
                // get parent 2
                do
                {
                    parent2 = getRandomNumber(0, (nAdultTotal - 1));
                } while (parent2 == parent1);
                for (countL = 0; countL < nLoci; countL++)
                {
                    // allele from parent1
                    arrayNewGenotype[countL][0] = arrayParent[parent1][countL][getRandomNumber(0, 1)];
                    // allele from parent2
                    arrayNewGenotype[countL][1] = arrayParent[parent2][countL][getRandomNumber(0, 1)];
                }
                break;
        case 5: // Diecious - random mating
                // get parent1 and parent2
                parent1 = getRandomNumber(0, (nFemale - 1));
                parent2 = getRandomNumber(nFemale, (nAdultTotal - 1));
                for (countL = 0; countL < nLoci; countL++)
                {
                    // allele from parent1
                    arrayNewGenotype[countL][0] = arrayParent[parent1][countL][getRandomNumber(0, 1)];
                    // allele from parent2
                    arrayNewGenotype[countL][1] = arrayParent[parent2][countL][getRandomNumber(0, 1)];
                }
                break;
        case 6: // Diecious - single reproducing male
                // get parent1 
                parent1 = getRandomNumber(0, (nFemale - 1));
                for (countL = 0; countL < nLoci; countL++)
                {
                    // allele from parent1
                    arrayNewGenotype[countL][0] = arrayParent[parent1][countL][getRandomNumber(0, 1)];
                    // allele from parent2
                    arrayNewGenotype[countL][1] = arrayParent[parent2][countL][getRandomNumber(0, 1)];
                }
                break;
        case 7: // Diecious - single reproducing pair
                for (countL = 0; countL < nLoci; countL++)
                {
                    // allele from parent1
                    arrayNewGenotype[countL][0] = arrayParent[parent1][countL][getRandomNumber(0, 1)];
                    // allele from parent2
                    arrayNewGenotype[countL][1] = arrayParent[parent2][countL][getRandomNumber(0, 1)];
                }
                break;
        default: cout << endl;
                 cout << "*** Reproductive system setting error" << endl;
                 exit(0);                     
                
    } // end of reproSys switch 


} // end of getNewDiMLGenotype

// readInputFileNAllele function
int readInputFileNAllele(ifstream &alleleFreqInput)
{
    int nAllele;
    
    alleleFreqInput >> nAllele;
    if (nAllele < 1)
    {
        cout << endl;
        cout << "*** Observed number of alleles < 1" << endl;
        exit(0);
    }
    
    return nAllele;
}

// readInputFileAlleleFreq function
void readInputFileAlleleFreq(ifstream &alleleFreqInput, double *inputAlleleFreq, int nAllele)
{
    int countA;

    for (countA = 0; countA < nAllele; countA++)
    {
        if ( alleleFreqInput.eof())
            break;            

        alleleFreqInput >> inputAlleleFreq[countA];

    }

    // display
    cout << "Finished reading allele frequency input file." << endl;
    cout << endl;

} // end of readInputFileAlleleFreq function


// readInputFileNYear function
int readInputFileNYear(ifstream &popSizeInput)
{
    int nYear;
    
    popSizeInput >> nYear;
    if (nYear < 0)
    {
        cout << endl;
        cout << "*** Population size input error." << endl;
        cout << "*** Number of years < 0" << endl;
        exit(0);
    }
    
    return nYear;
} // end of readInputFileNYear function

// readInputFilePopSize function
void readInputFilePopSize(ifstream &popSizeInput, int *nPopSize, int *nFemale, int *nMale, int nYear, int reproSys)
{
    int countY; // loop counting
    int dummyYear; // read the "year" value in the input file, do NOT actully use the number
        
    for (countY = 0; countY < (nYear+1); countY++)
    {
        if (popSizeInput.eof())
            break;
                    
        popSizeInput >> dummyYear;
        popSizeInput >> nPopSize[countY];
        
        if (reproSys >= 5 && reproSys <= 7)
        {
            popSizeInput >> nFemale[countY]; // read nFemale
            nMale[countY] = (nPopSize[countY] - nFemale[countY]); // calculate nMale
        }

    }

    // display
    cout << "Finished reading population size input file." << endl;
    cout << endl;

} // end of readInputFilePopSize function

// readInputFileNSize function
int readInputFileNSize(ifstream &genotypeInputFile)
{
    int nSize;
    
    genotypeInputFile >> nSize;
    
    if (nSize < 1)
    {
        cout << endl;
        cout << "*** Number of individuals in the input file < 1" << endl;
        exit(0);
    } 
    
    return nSize;

} // end of readInputFileNSize function

// readInputFileNLoci function
int readInputFileNLoci(ifstream &genotypeInputFile)
{
    int nLoci;
    
    genotypeInputFile >> nLoci;
    
    if (nLoci < 1)
    {
        cout << endl;
        cout << "*** Number of loci in the input file < 1" << endl;
        exit(0);
    } 
    
    return nLoci;

} // end of readInputFileNLoci function

// readInputFileLocusName function
void readInputFileLocusName(ifstream &genotypeInputFile, locusName *arrayInputLocus, int nLoci)
{
    int countL;

    for ( countL = 0; countL < nLoci; countL++)
    {
        genotypeInputFile >> setw(locusNameLength) >> arrayInputLocus[countL];
        if ( genotypeInputFile.eof())
            break;
    }
} // end of readInputFileLocusName function

// readInputFileGenotype function
void readInputFileGenotype(ifstream &genotypeInputFile, individualId *arrayInputId, char ***arrayInput, int nSize, int nLoci, int allelePerLocus)
{
    int countN;
    int countL;
    int countA;
            
    for (countN = 0; countN < nSize; countN++)
    {
        genotypeInputFile >> setw(idLength) >> arrayInputId[countN];
        if ( genotypeInputFile.eof())
            break;
        for ( countL = 0; countL < nLoci; countL++)
        {
            for ( countA = 0; countA < allelePerLocus; countA++)
            {
                genotypeInputFile >> arrayInput[countN][countL][countA];
            }
        }
    }

    // display
    cout << "Finished reading input file." << endl;
    cout << endl;

    
} // end of readInputFileGenotype function

// displayAlleleFreqInput function
void displayAlleleFreqInput(double *inputAlleleFreq, int nAllele)
{
    int countA;

    cout << "Imported data from input file: " << endl; 
    cout << "  Total number of alleles = " << nAllele << endl;
    for ( countA = 0; countA < nAllele; countA++)
    {
        cout << "    Frequency of allele " 
             << (countA + 1) 
             << " =  " 
             << setprecision(4)
             << setiosflags(ios::fixed | ios::showpoint)
             << inputAlleleFreq[countA] << endl;
    }
    cout << endl;

} // end of displayAlleleFreqInput function


// displayVPopSizeInput function
void displayVPopSizeInput(int *nPopSize, int *nFemale, int reproSys, int nYear)
{
    int countY;
    
    cout << "Imported data from population size input file: " << endl; 
    cout << "  Number of years to simulate = " << nYear << endl;
    cout << endl;

    if (reproSys == 5 || reproSys == 6 || reproSys == 7)
    {
        cout << "Year      PopSize   nFemale " << endl;
        cout << "----------------------------" << endl;

        for (countY = 0; countY < (nYear+1); countY++)
        {
            cout << setw(8) 
                 << countY
                 << "  "
                 << setw(8)
                 << nPopSize[countY]
                 << "  "
                 << setw(8)
                 << nFemale[countY]
                 << endl;
        }
        cout << endl;
    }

    else
    {
        cout << "Year      PopSize " << endl;
        cout << "------------------" << endl;

        for (countY = 0; countY < (nYear+1); countY++)
        {
            cout << setw(8) 
                 << countY
                 << "  "
                 << setw(8)
                 << nPopSize[countY]
                 << endl;
        }
        cout << endl;
    }
} // end of displayVPopSizeInput function


// displayGenotypeInput function
void displayGenotypeInput(locusName *arrayInputLocus, individualId *arrayInputId, char ***arrayInput, int nSize, int nLoci, int allelePerLocus)
{
    int countN;
    int countL;
    int countA;

    cout << "Imported data from input file: " << endl; 
    cout << "  Total number of individuals = " << nSize << endl;
    cout << "  Total number of loci = " << nLoci << endl;
    cout << "  Locus name list:";
    for ( countL = 0; countL < nLoci; countL++)
    {
        cout << " " << arrayInputLocus[countL];
    }
    cout << endl;

    cout << "  Genotypic data:" << endl;
    for (countN = 0; countN < nSize; countN++)
    {
        cout << "    " << arrayInputId[countN];
        for ( countL = 0; countL < nLoci; countL++)
        {
            for ( countA = 0; countA < allelePerLocus; countA++)
            {
                cout << " " << arrayInput[countN][countL][countA];
            }
        }
        cout << endl;
    }
    cout << endl;

} // end of displayGenotypeInput function


// checkAlleleFreqInput function
void checkAlleleFreqInput(double *inputAlleleFreq, int nAllele)
{
    int countA;
    double sumAlleleFreq = 0.0; 

    // check for negative value
    for (countA = 0; countA < nAllele; countA++)
    {
        if (inputAlleleFreq[countA] < 0)
        {
            cout << endl;
            cout << "*** Input allele frequency error (negative value found!).";
            exit(0);
        }
    }

    // check for allele frequency sum
    for (countA = 0; countA < nAllele; countA++)
    {
        sumAlleleFreq += inputAlleleFreq[countA];
    }
    if (sumAlleleFreq <= 0.9999 || sumAlleleFreq >= 1.0001)
    {
        cout << endl;
        cout << "*** Input allele frequency error (not sum to 1!).";
        exit(0);                     
    }


} // end of checkAlleleFreqInput function


// checkVPopSsizeInput function
void checkVPopSsizeInput(int *nPopSize, int *nFemale, int reproSys, int nYear)
{
    int countY;

    for (countY = 0; countY < (nYear+1); countY++)
    {
        switch (reproSys)
        {
            case 1: // asexual reproduction
            case 2: // Monoecious - complete selfing
            case 3: // Monoecious - random mating with selfing
                    if (nPopSize[countY] < 1)
                    {
                        cout << "*** Population size setting error" << endl
                             << "*** Population size in year" << countY << " < 1" << endl
                             << "*** Population extinction occurred." << endl;
                        exit(0);
                    }
                    break;
            case 4: // Monoecious - random mating without selfing
                    if (nPopSize[countY] < 2)
                    {
                        cout << "*** Population size setting error" << endl
                             << "*** Population size in year" << countY << " < 2" << endl
                             << "*** Population extinction occurred." << endl;
                        exit(0);
                    }
                    break;
            case 5: // Diecious - random mating
            case 6: // Diecious - single reproducing male
            case 7: // Diecious - single reproducing pair
                    if (nPopSize[countY] < 2) // check nPopSize
                    {
                        cout << "*** Population size setting error" << endl
                             << "*** Population size in year" << countY << " < 2" << endl
                             << "*** Population extinction occurred." << endl;
                        exit(0);
                    }
                    if (nFemale[countY] < 1) // check nFemale
                    {
                        cout << "*** Population size setting error" << endl
                             << "*** nFemale in year" << countY << " < 1" << endl
                             << "*** Population extinction occurred." << endl;
                        exit(0);
                    }
                    if ((nPopSize[countY] - nFemale[countY]) < 1) // check nMale
                    {
                        cout << "*** Population size setting error" << endl
                             << "*** nMale in year" << countY << " < 1" << endl
                             << "*** Population extinction occurred." << endl;
                        exit(0);
                    }
                    break;
            default: cout << endl;
                     cout << "*** Reproductive system setting error" << endl;
                     exit(0);                     
                    
        } // end of reproSys switch 
    } // end of countY loop
} // end of checkVPopSsizeInput function


// writeOutputHeader function
void writeOutputHeader(ofstream &outputFile)
{   
    cout << "Writing header to output file..." << endl;
    cout << endl;

    // get time
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    // write header 
    outputFile << "*** Start of output header ***" << endl;
    outputFile << "Program name: " << programName << endl;
    outputFile << "Version number: " << versionNumber << endl;
    outputFile << "Release date: " << versionDate << endl;
    outputFile << endl;

    outputFile << "Contact information" << endl; 
    outputFile << "  Author: " << author << endl; 
    outputFile << "  email: " << email << endl;
    outputFile << "  Web: " << web << endl;
    outputFile << endl;
    
    outputFile << "Simulation date and time: " << asctime(timeinfo);
    outputFile << "*** End of output header ***" << endl;
    outputFile << endl;

} // end of writeOutputHeader function

// writeSLRawGenotypeHeader function
void writeSLRawGenotypeHeader(ofstream &rawGenotypeOutputFile)
{   
    // get time
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    // write header 
    rawGenotypeOutputFile << programName << " genotype raw data " << asctime(timeinfo);
    rawGenotypeOutputFile << "Loc1" << endl;
} // end of writeSLRawGenotypeHeader function

// writeMLRawGenotypeHeader function
void writeMLRawGenotypeHeader(ofstream &rawGenotypeOutputFile, locusName *arrayInputLocus, int nLoci)
{   
    int countL;
    
    // get time
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    // write header 
    rawGenotypeOutputFile << programName << " genotype raw data " << asctime(timeinfo);

    for (countL = 0; countL < nLoci; countL++)
    {
        rawGenotypeOutputFile << arrayInputLocus[countL] << endl;
    }

} // end of writeMLRawGenotypeHeader function

// writeVPopSizeInput function
void writeVPopSizeInput(ofstream &outputFile, int *nPopSize, int *nFemale, int reproSys, int nYear)
{

    int countY;
    
    outputFile << "*** Start of population size setting data from input file ***" << endl;
    outputFile << "  Number of years = " << nYear << endl;
    outputFile << endl;

    if (reproSys == 5 || reproSys == 6 || reproSys == 7)
    {
        outputFile << "Year      PopSize   nFemale " << endl;
        outputFile << "----------------------------" << endl;

        for (countY = 0; countY < (nYear+1); countY++)
        {
            outputFile << setw(8) 
                       << countY
                       << "  "
                       << setw(8)
                       << nPopSize[countY]
                       << "  "
                       << setw(8)
                       << nFemale[countY]
                       << endl;
        }
        outputFile << endl;
    }

    else
    {
        outputFile << "Year      PopSize " << endl;
        outputFile << "------------------" << endl;

        for (countY = 0; countY < (nYear+1); countY++)
        {
            outputFile << setw(8) 
                       << countY
                       << "  "
                       << setw(8)
                       << nPopSize[countY]
                       << endl;
        }
        outputFile << endl;
    }

    outputFile << "*** End of population size setting data from input file ***" << endl;

} // end of writeVPopSizeInput function



// writeAlleleFreq function
// write allele freq data calculatted from input file to output file
void writeAlleleFreq(ofstream &outputFile, int nLoci, locusName *arrayInputLocus, float **arrayAlleleFreq)
{
    // declare variable
        // loop counting
        int countL;
        int countA;
        int countX;
        
        // output writing
        int boolPrint;
        
        // inputOA
        int *inputOA;
        
        inputOA = new int [nLoci];
        if (inputOA == NULL)
            memoryError();

        // inputEA
        float *inputEA;
        
        inputEA = new float [nLoci];
        if (inputEA == NULL)
            memoryError();

        // calculation
        float sumPi2 = 0;
            // sum (Pi)2 for calcaulating effective number of alleles
        

    // display
    cout << "Writing allele frequcy to output file..." << endl;

    // count number of alleles in each locus (nOA)
    for (countL = 0; countL < nLoci; countL++)
    {
        inputOA[countL] = 0;
        
        for (countA = 0; countA < maxAllele; countA++)
        {
            if (arrayAlleleFreq[countL][countA] > 0)
                inputOA[countL]++;
        }
    }
    
    // calculate nEA
    for (countL = 0; countL < nLoci; countL++)
    {
        sumPi2 = 0;
        for (countA = 0; countA < maxAllele; countA++)
        {
            sumPi2 += (arrayAlleleFreq[countL][countA] * arrayAlleleFreq[countL][countA]);
        }
        inputEA[countL] = (1 / sumPi2);
    }
    
    // write allele freq data to output file
    outputFile << "*** Start of allele frequency data from input file ***" << endl;

    outputFile << "------------";    
    for (countL = 0; countL < nLoci; countL++)
    {
        for (countX = 0; countX < locusNameLength; countX++)
        {
            outputFile << '-';
        }
    }
    outputFile << endl;

    outputFile << "Allele/Locus";
    for (countL = 0; countL < nLoci; countL++)
    {
        outputFile << setw(locusNameLength)
                   << arrayInputLocus[countL];
    }
    outputFile << endl;

    outputFile << "------------";    
    for (countL = 0; countL < nLoci; countL++)
    {
        for (countX = 0; countX < locusNameLength; countX++)
        {
            outputFile << '-';
        }
    }
    outputFile << endl;
    
    for (countA = 0; countA < maxAllele; countA++)
    {
        // reset boolPrint
        boolPrint = 0;
        
        // check for allele presence in any locus
        for (countL = 0; countL < nLoci; countL++)
        {
            if (arrayAlleleFreq[countL][countA] != 0)
            {
                boolPrint = 1;
            }
        }
        
        if (boolPrint == 1)
        {
            outputFile << arrayAlleleId[countA]
                       << "           ";
            for (countL = 0; countL < nLoci; countL++)
            {
                if (arrayAlleleFreq[countL][countA] != 0)
                {
                    outputFile << setw(locusNameLength)
                               << setprecision(4)
                               << setiosflags(ios::fixed | ios::showpoint)
                               << arrayAlleleFreq[countL][countA];
                }
                else
                {
                    outputFile << setw(locusNameLength)
                               << "0";
                }
            }
            outputFile << endl;
        }
    }
    
    // write ---
    outputFile << "------------";    
    for (countL = 0; countL < nLoci; countL++)
    {
        for (countX = 0; countX < locusNameLength; countX++)
        {
            outputFile << '-';
        }
    }
    outputFile << endl;

    // write nOA
    outputFile << "nObsAllele  ";    
    for (countL = 0; countL < nLoci; countL++)
    {
        outputFile << setw(locusNameLength)
                   << inputOA[countL];
    }
    outputFile << endl;
    
    // write ---
    outputFile << "------------";    
    for (countL = 0; countL < nLoci; countL++)
    {
        for (countX = 0; countX < locusNameLength; countX++)
        {
            outputFile << '-';
        }
    }
    outputFile << endl;

    // write nEA
    outputFile << "nEffAllele  ";    
    for (countL = 0; countL < nLoci; countL++)
    {
        outputFile << setw(locusNameLength)
                   << setprecision(4)
                   << setiosflags(ios::fixed | ios::showpoint)
                   << inputEA[countL];
    }
    outputFile << endl;
    
    // write ---
    outputFile << "------------";    
    for (countL = 0; countL < nLoci; countL++)
    {
        for (countX = 0; countX < locusNameLength; countX++)
        {
            outputFile << '-';
        }
    }
    outputFile << endl;

    outputFile << "*** End of allele frequency data from input file ***" << endl;
    outputFile << endl;

} // end of writeAlleleFreq function

// writeCPSSimSetting function
void writeCPSSimSetting (ofstream &outputFile, int degreeOverlap, int reproSys, int longevity, int ageMaturation, int nPB, int nB, int nFemale, int nMale, int nYear, int nIteration)
{

    // display message
    cout << "Writing simulation settings to output file..." << endl;
    cout << endl;

    outputFile << "*** Start of simulation settings ***" << endl;
    
    outputFile << "Simulation settings:" << endl;
        // call write functions
        writeDegreeOverlap(outputFile, degreeOverlap);
        writeReprosys(outputFile, reproSys);
        writeLongevity(outputFile, longevity);
        writeAgeMaturation(outputFile, ageMaturation);
        writeNPB(outputFile, nPB);
        writeNB(outputFile, nB);
        if ( reproSys == 5 || reproSys == 6 || reproSys == 7)
        {
            writeSexRatio(outputFile, nFemale, nMale);
        }
        writeNYear(outputFile, nYear);
        writeNIteration(outputFile, nIteration);

    outputFile << "*** End of simulation settings ***" << endl;

} // end of writeCPSSimSetting function


// writeVPSSimSetting function
void writeVPSSimSetting (ofstream &outputFile, int degreeOverlap, int reproSys, int longevity, int ageMaturation, int nYear, int nIteration)
{

    // display message
    cout << "Writing simulation settings to output file..." << endl;
    cout << endl;

    outputFile << "*** Start of simulation settings ***" << endl;
    
    outputFile << "Simulation settings:" << endl;
        // call write functions
        writeDegreeOverlap(outputFile, degreeOverlap);
        writeReprosys(outputFile, reproSys);
        writeLongevity(outputFile, longevity);
        writeAgeMaturation(outputFile, ageMaturation);
        writeNYear(outputFile, nYear);
        writeNIteration(outputFile, nIteration);

    outputFile << "*** End of simulation settings ***" << endl;

} // end of writeVPSSimSetting function


// writeDegreeOverlap function
void writeDegreeOverlap(ofstream &outputFile, int degreeOverlap)
{
    outputFile << "    Generation overlap setting = " << degreeOverlap << endl;
    outputFile << "        (0)   = Minimum overlap (discrete generations)" << endl;
    outputFile << "        (100) = Maximum overlap (random starting age for all individuals)" << endl;
    outputFile << endl;
} // end of writeDegreeOverlap function

// writeReprosys function
void writeReprosys(ofstream &outputFile, int reproSys)
{
    outputFile << "    Reproductive system setting = " << reproSys << endl;
    outputFile << "        (1) = Asexual reproduction" << endl;
    outputFile << "        (2) = Monoecy with complete selfing" << endl;
    outputFile << "        (3) = Monoecy with random mating (with selfing)" << endl;
    outputFile << "        (4) = Monoecy with random mating (without selfing)" << endl;
    outputFile << "        (5) = Dioecy with random mating" << endl;
    outputFile << "        (6) = Dioecy with single reproducing male each year" << endl;
    outputFile << "        (7) = Dioecy with single reproducing pair each year" << endl;
    outputFile << endl;   
} // end of writeReprosys function

// writeLongevity function
void writeLongevity(ofstream &outputFile, int longevity)
{
    outputFile << "    Expected longevity of the organism = " << longevity << " year(s)" <<endl;
} // end of writeLongevity function

// writeAgeMaturation function
void writeAgeMaturation(ofstream &outputFile, int ageMaturation)
{
    outputFile << "    Reproductive maturation age of the organism = " << ageMaturation <<endl;
} // end of writeAgeMaturation function

// writeNPB function
void writeNPB(ofstream &outputFile, int nPB)
{
    outputFile << "    Population size before the bottleneck = " << nPB << endl;
} // end of writeNPB function

// writeNB function
void writeNB(ofstream &outputFile, int nB)
{
    outputFile << "    Population size during the bottleneck = " << nB << endl;
} // end of writeNB function

// writeSexRatio function
void writeSexRatio(ofstream &outputFile, int nFemale, int nMale)
{
    // declare variable
    float sexRatio;
    
    // calculate sex ratio
    if (nFemale >= nMale)
        sexRatio = float (nFemale) / float (nMale);
    else
        sexRatio = float (nMale) / float (nFemale);
    
    // write to output file
    outputFile << "    Number of female:male = " << nFemale << ":" << nMale << endl;
    outputFile << "    Sex ratio (female:male) = ";
    if (nFemale >= nMale)
        outputFile << setprecision(4) << setiosflags(ios::fixed | ios::showpoint) << sexRatio << ":1.0000" << endl;
    else
        outputFile << setprecision(4) << setiosflags(ios::fixed | ios::showpoint) << "1.000:" << sexRatio << endl;

} // end of writeSexRatio function

// writeNYear function
void writeNYear(ofstream &outputFile, int nYear)
{
    outputFile << "    Number of years simulated = " << nYear << endl;
} // end of writeNYear function

// writeNIteration function
void writeNIteration(ofstream &outputFile, int nIteration)
{
    outputFile << "    Number of iterations = " << nIteration << endl;
} // end of writeNIteration function


// writeDiSLSummary function
void writeDiSLSummary(ofstream &outputFile, int nYear, int nIteration, int **arrayRawOA, float **arrayRawEA, float **arrayRawHo, float **arrayRawHe, float **arrayRawF)
{
    // fixation probability
        // # of cases (iterations) reached fixation
        int fixCase;
        // prob. of fixation    
        float fixProb;

    // output array
        // mean [(nYear + 1)] 
        float *arrayMean;
        
        arrayMean = new float[(nYear + 1)] ;
        if (arrayMean == NULL)
            memoryError();
            
        
        // StdD [(nYear + 1)] 
        float *arrayStdD;

        arrayStdD = new float[(nYear + 1)] ;
        if (arrayStdD == NULL)
            memoryError();
            
        
        // StdE [(nYear + 1)] 
        float *arrayStdE;
        
        arrayStdE = new float[(nYear + 1)] ;
        if (arrayStdE == NULL)
            memoryError();
            
        // Percent [(nYear + 1)] 
        float *arrayPercent;

        arrayPercent = new float[(nYear + 1)] ;
        if (arrayPercent == NULL)
            memoryError();
            



    // fixation probability
        // call statFixDiploidSingleLocus function
        statFixDiploidSingleLocus(arrayRawOA, nYear, nIteration, fixCase, fixProb);
    
        // call writeDiploidSingleLocus function
        writeFixDiploidSingleLocus(outputFile, nIteration, fixCase, fixProb);
    
    // calculate and write simulation data to output file
    // call calculation and writing functions
        // OA
        outputDiploidSingleLocus(arrayRawOA, nYear, nIteration, arrayMean, arrayStdD, arrayStdE, arrayPercent);

        outputFile << endl;
        outputFile << "*** Start of OA data ***" << endl;             

        writeSingleLocus(outputFile, nYear, arrayMean, arrayStdD, arrayStdE, arrayPercent);

        outputFile << "*** End of OA data ***" << endl;

        // EA
        outputDiploidSingleLocus(arrayRawEA, nYear, nIteration, arrayMean, arrayStdD, arrayStdE, arrayPercent);

        outputFile << endl;
        outputFile << "*** Start of EA data ***" << endl;             

        writeSingleLocus(outputFile, nYear, arrayMean, arrayStdD, arrayStdE, arrayPercent);

        outputFile << "*** End of EA data ***" << endl;

        // Ho
        outputDiploidSingleLocus(arrayRawHo, nYear, nIteration, arrayMean, arrayStdD, arrayStdE, arrayPercent);

        outputFile << endl;
        outputFile << "*** Start of Ho data ***" << endl;             

        writeSingleLocus(outputFile, nYear, arrayMean, arrayStdD, arrayStdE, arrayPercent);

        outputFile << "*** End of Ho data ***" << endl;

        // He
        outputDiploidSingleLocus(arrayRawHe, nYear, nIteration, arrayMean, arrayStdD, arrayStdE, arrayPercent);

        outputFile << endl;
        outputFile << "*** Start of He data ***" << endl;             

        writeSingleLocus(outputFile, nYear, arrayMean, arrayStdD, arrayStdE, arrayPercent);

        outputFile << "*** End of He data ***" << endl;

        // F
        outputDiploidSingleLocus(arrayRawF, nYear, nIteration, arrayMean, arrayStdD, arrayStdE, arrayPercent);

        outputFile << endl;
        outputFile << "*** Start of F data ***" << endl;             

        writeSingleLocus(outputFile, nYear, arrayMean, arrayStdD, arrayStdE, arrayPercent);

        outputFile << "*** End of F data ***" << endl;



} // end of writeDiSLSummary function

// writeDiMLSummary function
void writeDiMLSummary(ofstream &outputFile, int nLoci, locusName *arrayInputLocus, int nYear, int nIteration, int ***arrayRawOA, float ***arrayRawEA, float ***arrayRawHo, float ***arrayRawHe, float ***arrayRawF)
{
    int countL;

    // fixation probability [nLoci]
        // # of cases (iterations) reached fixation
        int *fixCase;
        
        fixCase = new int [nLoci];
        if (fixCase == NULL)
            memoryError();
        
        // prob. of fixation    
        float *fixProb;

        fixProb = new float [nLoci];
        if (fixProb == NULL)
            memoryError();
        


    // initiate output data array
        // mean [nLoci][(nYear+1)] 
        // mean value of all simulation iterations (for each locus)
        float **arrayMean;
        
        arrayMean = new float* [nLoci];
        if (arrayMean == NULL)
            memoryError();
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayMean[countL] = new float [(nYear+1)];
            if (arrayMean == NULL)
                memoryError();
        }
        
        // Av [(nYear+1)]
        // Average of all loci
        float *arrayAv;
        
        arrayAv = new float [(nYear + 1)];
        if (arrayAv == NULL)
            memoryError();

        // StdD [(nYear+1)]
        float *arrayStdD;
        
        arrayStdD = new float [(nYear + 1)];
        if (arrayStdD == NULL)
            memoryError();

        // StdE [(nYear+1)]
        float *arrayStdE;
        
        arrayStdE = new float [(nYear + 1)];
        if (arrayStdE == NULL)
            memoryError();
            
        // Percent [(nYear+1)]
        float *arrayPercent;
        
        arrayPercent = new float [(nYear + 1)];
        if (arrayPercent == NULL)
            memoryError();
    

    // fixation probability
        // call statFixDiploidMultiLocus function
        statFixDiploidMultiLocus(arrayRawOA, nLoci, nYear, nIteration, fixCase, fixProb);
    
        // call writeDiploidMultiLocus function
        writeFixDiploidMultiLocus(outputFile, nLoci, arrayInputLocus, nIteration, fixCase, fixProb);
    
    
    // calculate and write simulation data to output file
    // call calculation and writing functions
        // OA
        outputDiploidMultiLocus(arrayRawOA, nLoci, nYear, nIteration, arrayMean, arrayAv, arrayStdD, arrayStdE, arrayPercent);

        outputFile << endl;
        outputFile << "*** Start of OA data ***" << endl;             

        writeMultiLocus(outputFile, nLoci, arrayInputLocus, nYear, arrayMean, arrayAv, arrayStdD, arrayStdE, arrayPercent);

        outputFile << "*** End of OA data ***" << endl;

        // EA
        outputDiploidMultiLocus(arrayRawEA, nLoci, nYear, nIteration, arrayMean, arrayAv, arrayStdD, arrayStdE, arrayPercent);

        outputFile << endl;
        outputFile << "*** Start of EA data ***" << endl;             

        writeMultiLocus(outputFile, nLoci, arrayInputLocus, nYear, arrayMean, arrayAv, arrayStdD, arrayStdE, arrayPercent);

        outputFile << "*** End of EA data ***" << endl;

        // Ho
        outputDiploidMultiLocus(arrayRawHo, nLoci, nYear, nIteration, arrayMean, arrayAv, arrayStdD, arrayStdE, arrayPercent);

        outputFile << endl;
        outputFile << "*** Start of Ho data ***" << endl;             

        writeMultiLocus(outputFile, nLoci, arrayInputLocus, nYear, arrayMean, arrayAv, arrayStdD, arrayStdE, arrayPercent);

        outputFile << "*** End of Ho data ***" << endl;

        // He
        outputDiploidMultiLocus(arrayRawHe, nLoci, nYear, nIteration, arrayMean, arrayAv, arrayStdD, arrayStdE, arrayPercent);

        outputFile << endl;
        outputFile << "*** Start of He data ***" << endl;             

        writeMultiLocus(outputFile, nLoci, arrayInputLocus, nYear, arrayMean, arrayAv, arrayStdD, arrayStdE, arrayPercent);

        outputFile << "*** End of He data ***" << endl;

        // F
        outputDiploidMultiLocus(arrayRawF, nLoci, nYear, nIteration, arrayMean, arrayAv, arrayStdD, arrayStdE, arrayPercent);

        outputFile << endl;
        outputFile << "*** Start of F data ***" << endl;             

        writeMultiLocus(outputFile, nLoci, arrayInputLocus, nYear, arrayMean, arrayAv, arrayStdD, arrayStdE, arrayPercent);

        outputFile << "*** End of F data ***" << endl;


} // end of writeDiMLSummary function


// writeFixDiploidSingleLocus function
void writeFixDiploidSingleLocus(ofstream &outputFile, int nIteration, int fixCase, float fixProb)
{
    outputFile << endl;
    outputFile << "*** Start of fixation probability data ***" << endl;
    outputFile << fixCase << " out of " << nIteration << " iterations reached fixation" << endl;
    outputFile << "Fixation probability = " 
               << setprecision(4) 
               << setiosflags(ios::fixed | ios::showpoint)
               << fixProb
               << endl;
    outputFile << "*** End of fixation probability data ***" << endl;
    outputFile << endl;

} // end of writeFixDiploidSingleLocus function

// writeFixDiploidMultiLocus function
void writeFixDiploidMultiLocus(ofstream &outputFile, int nLoci, locusName *arrayInputLocus, int nIteration, int *fixCase, float *fixProb)
{
    // declare variable
        // loop counting
        int countL;
        int countX;

    
    outputFile << endl;
    outputFile << "*** Start of fixation probability data ***" << endl;

    // separation line
    outputFile << "---------------------";    
    for (countL = 0; countL < nLoci; countL++)
    {
        for (countX = 0; countX < locusNameLength; countX++)
        {
            outputFile << '-';
        }
    }
    outputFile << endl;
    
    // locus list
    outputFile << "Locus                ";
    for (countL = 0; countL < nLoci; countL++)
    {
        outputFile << setw(locusNameLength)
                   << arrayInputLocus[countL];
    }
    outputFile << endl;
    
    // separation line
    outputFile << "---------------------";    
    for (countL = 0; countL < nLoci; countL++)
    {
        for (countX = 0; countX < locusNameLength; countX++)
        {
            outputFile << '-';
        }
    }
    outputFile << endl;
    
    // fixation case
    outputFile << "nFixation iterations "; 
    for (countL = 0; countL < nLoci; countL++)
    {
        outputFile << setw(locusNameLength)
                   << fixCase[countL];

    }
    outputFile << endl;
    
    // nIteration
    outputFile << "nTotal iterations    "; 
    for (countL = 0; countL < nLoci; countL++)
    {
        outputFile << setw(locusNameLength)
                   << nIteration;

    }
    outputFile << endl;
    
    // fixation probability
    outputFile << "Fixation probability "; 
    for (countL = 0; countL < nLoci; countL++)
    {
        outputFile << setw(locusNameLength)
                   << setprecision(4)
                   << setiosflags(ios::fixed | ios::showpoint)
                   << fixProb[countL];

    }
    outputFile << endl;

    // separation line
    outputFile << "---------------------";    
    for (countL = 0; countL < nLoci; countL++)
    {
        for (countX = 0; countX < locusNameLength; countX++)
        {
            outputFile << '-';
        }
    }
    outputFile << endl;
    
    outputFile << "*** End of fixation probability data ***" << endl;
    outputFile << endl;


} // end of writeFixDiploidMultiLocus function


// writeSingleLocus function
void writeSingleLocus(ofstream &outputFile, int nYear, float *arrayMean, float *arrayStdD, float *arrayStdE, float *arrayPercent)
{
    // declare variable
        // loop counting
        int countY;

    // write data
    outputFile << "Year   Mean       SD         SE         %      " << endl;
    outputFile << "-----------------------------------------------" << endl;

    for (countY = 0; countY < (nYear + 1); countY++)
    {
        outputFile << setw(4) 
                   << countY
                   << "   "
                   << setw(8)
                   << setprecision(4)
                   << setiosflags(ios::fixed | ios::showpoint | ios::right)
                   << arrayMean[countY]
                   << "   "
                   << setw(8)
                   << setprecision(4)
                   << setiosflags(ios::fixed | ios::showpoint | ios::right)
                   << arrayStdD[countY]
                   << "   "
                   << setw(8)
                   << setprecision(4)
                   << setiosflags(ios::fixed | ios::showpoint | ios::right)
                   << arrayStdE[countY]
                   << "   "
                   << setw(8)
                   << setprecision(4)
                   << setiosflags(ios::fixed | ios::showpoint | ios::right)
                   << arrayPercent[countY]
                   << endl;

    }
    outputFile << endl;

} // end of writeSingleLocus function


// writeMultiLocus function
void writeMultiLocus(ofstream &outputFile, int nLoci, locusName *arrayInputLocus, int nYear, float **arrayMean, float *arrayAv, float *arrayStdD, float *arrayStdE, float *arrayPercent)
{
    // declare variable
        // loop counting
        int countL;
        int countY;

    // write header line
    outputFile << "Year/Locus";
    for (countL = 0; countL < nLoci; countL++)
    {
        outputFile << setw(locusNameLength)
                   << arrayInputLocus[countL];
    }
    outputFile << setw(locusNameLength) << "Average";
    outputFile << setw(locusNameLength) << "SD";
    outputFile << setw(locusNameLength) << "SE";
    outputFile << setw(locusNameLength) << "Percentage";
    outputFile << endl;
  
    // write data
    for (countY = 0; countY < (nYear + 1); countY++)
    {
        outputFile << setw(4)
                   << countY
                   << "      ";
        for (countL = 0; countL < nLoci; countL++)
        {
            outputFile << setw(locusNameLength)
                       << setprecision(4)
                       << setiosflags(ios::fixed | ios::showpoint | ios::right)
                       << arrayMean[countL][countY];
        }
        outputFile << setw(locusNameLength) << arrayAv[countY];
        outputFile << setw(locusNameLength) << arrayStdD[countY];
        outputFile << setw(locusNameLength) << arrayStdE[countY];
        outputFile << setw(locusNameLength) << arrayPercent[countY];
        
        outputFile << endl;
    }
    outputFile << endl;

} // end of writeMultiLocus function


// writeDiSLRawGenotype function
// write diploid singlelocus raw genotype data
void writeDiSLRawGenotype(ofstream &rawGenotypeOutputFile, int **arrayGenotype, int allelePerLocus, int nPopSize)
{
    int countN;
    int countA;
    
    // write "pop" keyword
    rawGenotypeOutputFile << "pop" << endl;

    // write raw genotype
    for (countN = 0; countN < nPopSize; countN++)
    {
        rawGenotypeOutputFile << "Ind" << (countN + 1) << ", ";
        for (countA = 0; countA < allelePerLocus; countA++)
        {
            rawGenotypeOutputFile << (arrayGenotype[countN][countA] + 11);
        }
        rawGenotypeOutputFile << endl;
    }

} // end of writeDiSLRawGenotype function


// writeDiMLRawGenotype function
// write diploid multilocus raw genotype data
void writeDiMLRawGenotype(ofstream &rawGenotypeOutputFile, int ***arrayGenotype, int allelePerLocus, int nPopSize, int nLoci)
{
    int countN;
    int countL;
    int countA;
    
    // write "pop" keyword
    rawGenotypeOutputFile << "pop" << endl;

    // write raw genotype
    for (countN = 0; countN < nPopSize; countN++)
    {
        rawGenotypeOutputFile << "Ind" << (countN + 1) << ", ";
        for (countL = 0; countL < nLoci; countL++)
        {
            for (countA = 0; countA < allelePerLocus; countA++)
            {
                rawGenotypeOutputFile << (arrayGenotype[countN][countL][countA] + 11);
            }
            rawGenotypeOutputFile << " ";
            
        }
        rawGenotypeOutputFile << endl;
    }

} // end of writeDiMLRawGenotype function

// statGenotypeInput function
void statGenotypeInput(char ***arrayInput, int **arrayAlleleCount, int *alleleTotalCount, float **arrayAlleleFreq, int nSize, int nLoci, int allelePerLocus)
{

    // declare variables
    char alleleId; 
        // allele id
    int countN;
    int countL;
    int countA;
    int countAId;
    
    // display
    cout << "Calculating allele frequcy from input genotype data..." << endl;
        
    // calculate allele freq
    for (countL = 0; countL < nLoci; countL++)
    {
        // reset allele count
        for ( countA = 0; countA < maxAllele; countA++)
        {
            arrayAlleleCount[countL][countA] = 0;
        }
        
        // get allele count
        for (countN = 0; countN < nSize; countN++)
        {

            for (countA = 0; countA < allelePerLocus; countA++)
            {
                alleleId = arrayInput[countN][countL][countA];
                if ( alleleId != inputMissingValue )
                {
                    for (countAId = 0; countAId < maxAllele; countAId++)
                    {
                        if (alleleId == arrayAlleleId[countAId])
                            arrayAlleleCount[countL][countAId]++;
                    }
                }
            } // end of A
        } // end of N
        
        // get allele total count
        alleleTotalCount[countL] = 0;
        
        for (countA = 0; countA < maxAllele; countA++)
        {
            alleleTotalCount[countL] += arrayAlleleCount[countL][countA];
        }
        
        // allele freq
        for (countA = 0; countA < maxAllele; countA++)
        {
            arrayAlleleFreq[countL][countA] = float (arrayAlleleCount[countL][countA]) / alleleTotalCount[countL];
        }
    
    } // end of countL   


} // end of statGenotypeInput function


// statFixDiploidSingleLocus function
// receive raw OA data array, nYear, nIteration
// calculate # of fixation cases and fixation probability
void statFixDiploidSingleLocus(int **arrayRawOA, int nYear, int nIteration, int &fixCase, float &fixProb)
{
    // declare variables
        // loop counting
        int countI;
    
    // calculation
    fixCase = 0; // reset

    for (countI = 0; countI < nIteration; countI++)
    {
        if (arrayRawOA[nYear][countI] == 1)
            fixCase++;
    }
    
    
    fixProb = float (fixCase) / nIteration;

} // end of statFixDiploidSingleLocus function

// statFixDiploidMultiLocus function
// receive raw OA data array, nLoci, nYear, nIteration
// calculate # of fixation cases and fixation probability
void statFixDiploidMultiLocus(int ***arrayRawOA, int nLoci, int nYear, int nIteration, int *fixCase, float *fixProb)
{
    // declare variables
        // loop counting
        int countL;
        int countI;
        
    // calculation
    for (countL = 0; countL < nLoci; countL++)
    {
        // reset
        fixCase[countL] = 0;    
        
        // count fix case
        for (countI = 0; countI < nIteration; countI++)
        {
            if (arrayRawOA[countL][nYear][countI] == 1)
                fixCase[countL]++;
        }
        
        // calculate fix prob
        fixProb[countL] = (float (fixCase[countL])) / nIteration;
    }    

} // end of statFixDiploidMultiLocus function



// statDiploidSingleLocus function
// receive genotype array from simulation module
// receive nPopSize (# of individuals) and nAllele (# of starting alleles)
// return stat (nOA, nEA, nHo, nHe, nF)
void statDiploidSingleLocus(int **arrayGenotype, int allelePerLocus, int nPopSize, int nAllele, int &nOA, float &nEA, float &nHo, float &nHe, float &nF)
{
    // declare variables
        // loop counting 
        int countA;
        int countN;
        
        // calculation
        int nHeteroFound;
           // observed number of heterozygous individuals
        float sumPi2 = 0;
            // sum (Pi)2 for calcaulating effective number of alleles
        
    // initiate array
        // allele count array [nAllele]
        int *arrayAlleleCount;

        arrayAlleleCount = new int [nAllele];
        if (arrayAlleleCount == NULL)
            memoryError();
        
        // allele freq array [nAllele]
        float *arrayAlleleFreq;
        
        arrayAlleleFreq = new float [nAllele];
        if (arrayAlleleFreq == NULL)
            memoryError();

    // stat calculation
        // allele count
        for ( countA = 0; countA < nAllele; countA++)
        {
            arrayAlleleCount[countA] = 0;
        }

        for (countN = 0; countN < nPopSize; countN++)
        {
            for (countA = 0; countA < allelePerLocus; countA++)
            {
                arrayAlleleCount[(arrayGenotype[countN][countA])]++;
            }
        }

        // nOA
        nOA = 0; // reset

        for (countA = 0; countA < nAllele; countA++)
        {
            if (arrayAlleleCount[countA] > 0)
                nOA++;
        }

        // check if fixed
        if (nOA == 1)
            {
                nEA = 1;
                nHo = 0;
                nHe = 0;
                nF = 1;
            }
        else
            {        
                // allele freq           
                for (countA = 0; countA < nAllele; countA++)
                {
                    arrayAlleleFreq[countA] = float (arrayAlleleCount[countA]) / (nPopSize * allelePerLocus);
                }

                // effective number of alleles
                sumPi2 = 0; // reset
                
                for (countA = 0; countA < nAllele; countA++)
                {
                    sumPi2 += (arrayAlleleFreq[countA] * arrayAlleleFreq[countA]);
                }
                nEA = 1 / sumPi2;
                        
                // Ho
                nHeteroFound = 0; // reset
                
                for (countN = 0; countN < nPopSize; countN++)
                {
                    if (arrayGenotype[countN][0] != arrayGenotype[countN][1])
                        nHeteroFound++;
                }
                nHo = float (nHeteroFound) / nPopSize;
                
                // He
                nHe = 1; // reset
                
                for (countA = 0; countA < nAllele; countA++)
                {
                    nHe -= (arrayAlleleFreq[countA] * arrayAlleleFreq[countA]);
                }
                if (nHe < 0) {
                	nHe = -nHe;
                }
                
                // F
                nF = (nHe - nHo) / nHe;
            }


} // end of statDiploidSingleLocus function



// statDiploidMultiLocus function
// receive genotype array from simulation module
// receive nPopSize (# of individuals) and nLoci (# of Loci)
// return stat (nOA, nEA, nHo, nHe, nF)
void statDiploidMultiLocus(int ***arrayGenotype, int allelePerLocus, int nPopSize, int nLoci, int *arrayOA, float *arrayEA, float *arrayHo, float *arrayHe, float *arrayF)
{
    // declare variables
        // loop counting 
        int countN;
        int countL;
        int countA;
        
        // calculation
        int nHeteroFound;
           // observed number of heterozygous individuals
        float sumPi2 = 0;
            // sum (Pi)2 for calcaulating effective number of alleles
                
    // initiate array
        // allele count array [maxAllele]
        int *arrayAlleleCount;

        arrayAlleleCount = new int [maxAllele];
        if (arrayAlleleCount == NULL)
            memoryError();
        
        // allele freq array [maxAllele]
        float *arrayAlleleFreq;
        
        arrayAlleleFreq = new float [maxAllele];
        if (arrayAlleleFreq == NULL)
            memoryError();
    
    // stat calculation
    for (countL = 0; countL < nLoci; countL++)
    {
        // allele count
        for ( countA = 0; countA < maxAllele; countA++)
        {
            arrayAlleleCount[countA] = 0;
        }
        
        for (countN = 0; countN < nPopSize; countN++)
        {
            for (countA = 0; countA < allelePerLocus; countA++)
            {
                arrayAlleleCount[(arrayGenotype[countN][countL][countA])]++;
            }
        }
        
        // OA
        arrayOA[countL] = 0; // reset

        for (countA = 0; countA < maxAllele; countA++)
        {
            if (arrayAlleleCount[countA] > 0)
                arrayOA[countL]++;
        }

        // check if fixed
        if (arrayOA[countL] == 1)
            {
                arrayEA[countL] = 1;
                arrayHo[countL] = 0;
                arrayHe[countL] = 0;
                arrayF[countL] = 1;
            }
        else
            {        
                // allele freq           
                for (countA = 0; countA < maxAllele; countA++)
                {
                    arrayAlleleFreq[countA] = float (arrayAlleleCount[countA]) / (nPopSize * allelePerLocus);
                }

                // effective number of alleles
                sumPi2 = 0; // reset
                
                for (countA = 0; countA < maxAllele; countA++)
                {
                    sumPi2 += (arrayAlleleFreq[countA] * arrayAlleleFreq[countA]);
                }
                arrayEA[countL] = 1 / sumPi2;
                        
                // Ho
                nHeteroFound = 0; // reset
                
                for (countN = 0; countN < nPopSize; countN++)
                {
                    if (arrayGenotype[countN][countL][0] != arrayGenotype[countN][countL][1])
                        nHeteroFound++;
                }
                arrayHo[countL] = float (nHeteroFound) / nPopSize;
                
                // He
                arrayHe[countL] = 1; // reset
                
                for (countA = 0; countA < maxAllele; countA++)
                {
                    arrayHe[countL] -= (arrayAlleleFreq[countA] * arrayAlleleFreq[countA]);
                }
                if (arrayHe[countL] < 0) {
                	arrayHe[countL] = -arrayHe[countL];
                }
                
                // F
                arrayF[countL] = (arrayHe[countL] - arrayHo[countL]) / arrayHe[countL];
            }


    
    } // end of countL loop

} // end of statDiploidMultiLocus function


// outputDiploidSingleLocus function (int)
// receive raw data array (int), nYear, nIteration from simulation module
// calculate and return mean, StdD, StdE, and % array
void outputDiploidSingleLocus(int **arrayRawdata, int nYear, int nIteration, float *arrayMean, float *arrayStdD, float *arrayStdE, float *arrayPercent)
{
    // declare variables
        // loop counting
        int countY;
        int countI;
        // calculation
        int total;
        float sumS;

    // calculation
        // calculate mean
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            total = 0;
            for (countI = 0; countI < nIteration; countI++)
            {
                total += arrayRawdata[countY][countI];
            }
            arrayMean[countY] = ( float (total) / nIteration);                        
        }
        
        // calculate stdD, stdE, %
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            sumS = 0;
            for (countI = 0; countI < nIteration; countI++)
            {
                sumS += (float (arrayRawdata[countY][countI]) - arrayMean[countY]) * (float (arrayRawdata[countY][countI]) - arrayMean[countY]);
            }
            arrayStdD[countY] = (sqrt((sumS/(nIteration - 1))));
            arrayStdE[countY] = (arrayStdD[countY] / sqrt((float(nIteration))));
            arrayPercent[countY] = ((arrayMean[countY] / arrayMean[0]) * 100);
        }

} // end of outputDiploidSingleLocus function (int)

// outputDiploidSingleLocus function (float)
// receive raw data array (float), nYear, nIteration from simulation module
// calculate and return mean, StdD, StdE, and % array
void outputDiploidSingleLocus(float **arrayRawdata, int nYear, int nIteration, float *arrayMean, float *arrayStdD, float *arrayStdE, float *arrayPercent)
{
    // declare variables
        // loop counting
        int countY;
        int countI;
        // calculation
        float total;
        float sumS;

    // calculation
        // calculate mean
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            total = 0;
            for (countI = 0; countI < nIteration; countI++)
            {
                total += arrayRawdata[countY][countI];
            }
            arrayMean[countY] = (total / nIteration);                        
        }
        
        // calculate stdD, stdE, %
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            sumS = 0;
            for (countI = 0; countI < nIteration; countI++)
            {
                sumS += (arrayRawdata[countY][countI] - arrayMean[countY]) * (arrayRawdata[countY][countI] - arrayMean[countY]);
            }
            arrayStdD[countY] = (sqrt((sumS/(nIteration - 1))));
            arrayStdE[countY] = (arrayStdD[countY] / sqrt(float(nIteration)));
            arrayPercent[countY] = ((arrayMean[countY] / arrayMean[0]) * 100);
        }

} // end of outputDiploidSingleLocus function (float)

// outputDiploidSingleLocus function (int)
// receive raw data array (int), nLoci, nYear, nIteration from simulation module
// calculate and return mean, Av, StdD, StdE, and % array
void outputDiploidMultiLocus(int ***arrayRawdata,  int nLoci, int nYear, int nIteration, float **arrayMean, float *arrayAv, float *arrayStdD, float *arrayStdE, float *arrayPercent)
{
    // declare variables
        // loop counting
        int countL;
        int countY;
        int countI;
        // calculation
        int intTotal;
        float floatTotal;
        float sumS;

    // calculation
        // calculate mean
        for (countL = 0; countL < nLoci; countL++)
        {
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                intTotal = 0;
                for (countI = 0; countI < nIteration; countI++)
                {
                    intTotal += arrayRawdata[countL][countY][countI];
                }
                arrayMean[countL][countY] = ( float (intTotal) / nIteration);                        
            }
        }
        
        // calculate Av
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            floatTotal = 0;
            for (countL = 0; countL < nLoci; countL++)
            {
                floatTotal += arrayMean[countL][countY];
            }
            arrayAv[countY] = (floatTotal / nLoci);
        }
        
        // calculate stdD, stdE, %
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            sumS = 0;
            for (countL = 0; countL < nLoci; countL++)
            {
                sumS += ((arrayMean[countL][countY] - arrayAv[countY]) * (arrayMean[countL][countY] - arrayAv[countY]));
            }
            arrayStdD[countY] = (sqrt((sumS / (nLoci - 1))));            
            arrayStdE[countY] = (arrayStdD[countY] / sqrt(float(nLoci)));            
            arrayPercent[countY] = ((arrayAv[countY] / arrayAv[0]) * 100);    
        }



} // end of outputDiploidSingleLocus function (int)

// outputDiploidSingleLocus function (float)
// receive raw data array (float), nLoci, nYear, nIteration from simulation module
// calculate and return mean, Av, StdD, StdE, and % array
void outputDiploidMultiLocus(float ***arrayRawdata,  int nLoci, int nYear, int nIteration, float **arrayMean, float *arrayAv, float *arrayStdD, float *arrayStdE, float *arrayPercent)
{
    // declare variables
        // loop counting
        int countL;
        int countY;
        int countI;
        // calculation
        float floatTotal;
        float sumS;

    // calculation
        // calculate mean
        for (countL = 0; countL < nLoci; countL++)
        {
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                floatTotal = 0;
                for (countI = 0; countI < nIteration; countI++)
                {
                    floatTotal += arrayRawdata[countL][countY][countI];
                }
                arrayMean[countL][countY] = (floatTotal / nIteration);                        
            }
        }
        
        // calculate Av
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            floatTotal = 0;
            for (countL = 0; countL < nLoci; countL++)
            {
                floatTotal += arrayMean[countL][countY];
            }
            arrayAv[countY] = (floatTotal / nLoci);
        }
        
        // calculate stdD, stdE, %
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            sumS = 0;
            for (countL = 0; countL < nLoci; countL++)
            {
                sumS += ((arrayMean[countL][countY] - arrayAv[countY]) * (arrayMean[countL][countY] - arrayAv[countY]));
            }
            arrayStdD[countY] = (sqrt((sumS / (nLoci - 1))));            
            arrayStdE[countY] = (arrayStdD[countY] / sqrt(float(nLoci)));            
            arrayPercent[countY] = ((arrayAv[countY] / arrayAv[0]) * 100);    
        }


} // end of outputDiploidSingleLocus function (float)

// openInput function
// open input file and get input file name
void openInput(ifstream &inputFile, fileName &inputFileName)
{
 
    // prompt for input filename
    cout << endl;
    cout << "Input file must be in the same directory as this program," << endl;
    cout << "The limitation of file name length is " << (fileNameLength - 1) << " characters." << endl;
    cout << "Please enter input filename: ";
    cin >> inputFileName;

	// open input file
	inputFile.open(inputFileName);
	// check input file
	if ( inputFile == 0 )
	{
	    cout << endl;
		cout << "*** Error opening input file." << endl;
		exit(0);
	}
	else
		cout << endl;
		cout << "Input file (" << inputFileName << ") opened successfully." << endl;

} // end of openInput

// openPopSizeInput function
// open input file and get input file name
void openPopSizeInput(ifstream &popSizeInput, fileName &popSizeInputName)
{
 
    // prompt for input filename
    cout << endl;
    cout << "Population size setting file must be in the same directory as this program," << endl;
    cout << "The limitation of file name length is " << (fileNameLength - 1) << " characters." << endl;
    cout << "Please enter population size setting filename: ";
    cin >> popSizeInputName;

	// open input file
	popSizeInput.open(popSizeInputName);
	// check input file
	if ( popSizeInput == 0 )
	{
	    cout << endl;
		cout << "*** Error opening population size setting file." << endl;
		exit(0);
	}
	else
		cout << endl;
		cout << "Population size setting file (" << popSizeInputName << ") opened successfully." << endl;

} // end of openPopSizeInput

// openOutput function
// open output file and get output file name
void openOutput(ofstream &outputFile, fileName &outputFileName)
{
    // prompt for output filename
    cout << endl;
    cout << "The limitation of file name length is " << (fileNameLength - 1) << " characters." << endl;
    cout << "Please enter output filename: ";
    cin >> outputFileName;
        
    // open outputFile
        outputFile.open(outputFileName);
        if ( outputFile == 0 )
        {
    	    cout << endl;
    		cout << "*** Error opening output file." << endl;
            exit(0);
        }
        else
		cout << endl;
		cout << "Output file (" << outputFileName << ") opened successfully." << endl;

} // end of openOutput

// openRawGenotypeOutput function
void openRawGenotypeOutput(ofstream &rawGenotypeOutputFile, fileName &rawGenotypeOutputName)
{
    // prompt for output filename
    cout << endl;
    cout << "The limitation of file name length is " << (fileNameLength - 1) << " characters." << endl;
    cout << "Please enter raw genotype data output filename: ";
    cin >> rawGenotypeOutputName;
        
    // open outputFile
        rawGenotypeOutputFile.open(rawGenotypeOutputName);
        if ( rawGenotypeOutputFile == 0 )
        {
    	    cout << endl;
    		cout << "*** Error opening output file." << endl;
            exit(0);
        }
        else
		cout << endl;
		cout << "Output file (" << rawGenotypeOutputName << ") opened successfully." << endl;


} // end of openRawGenotypeOutput function

// closeInputFile function
// close input file
void closeInputFile(ifstream &inputFile)
{
    cout << endl;
    
    inputFile.close();
}

// closeOutputFile function
// close output file
void closeOutputFile(ofstream &outputFile)
{
    cout << endl;
    
    outputFile.close();
}

// simDiploidSingleLocusCPS function
// perform diploid singlelocus constant population size simulation
void simDiploidSingleLocusCPS(void)
{

    // constants
    const int allelePerLocus = 2;
        // number of alleles per locus

    // declare loop-counting variables
    int countN = 0;
        // count individual
    int countNTemp = 0;
        // count individual
    int countA = 0;
        // count allele
    int countAPool = 0;
        // count allele pool
    int countY = 0;
        // count year
    int countI = 0;
        // count iteration

    // file operation
        // input file - allele freq setting
            // declare fileName
            fileName inputFileName; // input - allele freq 
            // declare input file
            ifstream alleleFreqInput;
            // call openInput function
            openInput(alleleFreqInput, inputFileName);
                
        // output file - simulation summary       
            // declare fileName
            fileName outputFileName; // output - simulation summary
            // declare output file
            ofstream outputFile;
            // call openOutput function
            openOutput(outputFile, outputFileName);
            // call writeOutputHeader function
            // write header to output file
            writeOutputHeader(outputFile);

        // output file - raw genotype data       
            // declare fileName
            fileName rawGenotypeOutputName;
            // declare raw data output file
            ofstream rawGenotypeOutputFile;
  

    // read input file
        // declare variables
        int nAllele = 0;
            // observed number of alleles in the pre bottleneck population

        // display
        cout << endl;
        cout << "Reading input file..." << endl;
        
        // read nAllele
        nAllele = readInputFileNAllele(alleleFreqInput);

        // initiate input array (input allele frequency)
        double *inputAlleleFreq;
            // array for input allele frequency [nAllele]
            
        inputAlleleFreq = new double[nAllele];
        if (inputAlleleFreq == NULL)
            memoryError();
        
        // read input allele freq
        readInputFileAlleleFreq(alleleFreqInput, inputAlleleFreq, nAllele);

        // display imported input data  
        displayAlleleFreqInput(inputAlleleFreq, nAllele);
             
        // check input allele freq data
        checkAlleleFreqInput(inputAlleleFreq, nAllele);
                    

    // get simulation settings
        // declare variables
        int degreeOverlap;
            // generation overlapping setting
        int reproSys;
            // reproductive system option
        int longevity;
            // longevity of the organism
        int ageMaturation;
            // reproductive maturation age of the organism
        int nPB;
            // pre-bottleneck population size 
        int nB;
            // bottlneck population size 
            
            // number of female/male for diecious models 
            int nFemale;
            int nMale;
            
        int nYear;
            // duration of bottleneck
        int nIteration;
            // number of iterations of the run
        int boolOutputRawGenotype;
            // bool variable for output raw genotype data
        // call getCPSSimSetting function
        getCPSSimSetting(degreeOverlap, reproSys, longevity, ageMaturation, nPB, nB, nFemale, nMale, nYear, nIteration, boolOutputRawGenotype);

    // check boolOutputRawGenotype
    if (boolOutputRawGenotype == 1)
        {
            // open raw data output file
            // call openRawGenotypeOutput function
            openRawGenotypeOutput(rawGenotypeOutputFile, rawGenotypeOutputName);
            // write header
            writeSLRawGenotypeHeader(rawGenotypeOutputFile);
        } 

    // declare simulation variables
    int parent1 = 0;
        // id of parent1
    int parent2 = 0;
        // id of parent2
    int nSurvivedFemale = 0;
        // number of survived females
    int nSurvivedMale = 0;
        // number of survived males
    int nSurvivedTotal = 0;
        // number of survivors
    int nAdultFemale = 0;
        // number of adult females
    int nAdultMale = 0;
        // number of adult Males
    int nAdultTotal = 0;
        // number of adults

    
    // initiate data arrays
        // pre-bottleneck population genotype [nPB][allelePerLocus]
        int **arrayPBP;
        
        arrayPBP = new int* [nPB];
        if (arrayPBP == NULL)
            memoryError();

        for (countN = 0; countN < nPB; countN++)
        {
            arrayPBP[countN] = new int[allelePerLocus];

            if (arrayPBP == NULL)
                memoryError();
        }
        
        // bottleneck population genotype [nB][allelePerLocus]
        int **arrayBP;
        
        arrayBP = new int* [nB];
        
        if (arrayBP == NULL)
            memoryError();

        for (countN = 0; countN < nB; countN++)
        {
            arrayBP[countN] = new int[allelePerLocus];
            
            if (arrayBP == NULL)
            memoryError();
        }
        
        // breeding population genotype [nB][allelePerLocus]
        int **arrayParent;
        
        arrayParent = new int* [nB];
        
        if (arrayParent == NULL)
            memoryError();

        for (countN = 0; countN < nB; countN++)
        {
            arrayParent[countN] = new int[allelePerLocus];
            
            if (arrayParent == NULL)
            memoryError();
        }
        
        // new genotype array
        int *arrayNewGenotype;
        
        arrayNewGenotype = new int [allelePerLocus];
        if (arrayNewGenotype == NULL)
            memoryError();
        
        // age [nB]
        int *arrayAge;
    
        arrayAge = new int [nB];
        if (arrayAge == NULL)
            memoryError();
    
        // allele count array [nAllele]
        int *arrayAlleleCount;

        arrayAlleleCount = new int [nAllele];
        if (arrayAlleleCount == NULL)
            memoryError();

    // declare variable        
    int nOA = 0;
        // observed number of alleles
    float nEA = 0;
        // effective number of alleles
    float nHo = 0;
        // observed heterozygosity
    float nHe = 0;
        // expected heterozygosity
    float nF = 0;
        // fixation index
        
        
    // initiate raw data array
        // OA [(nYear+1)][nIteration]
        int **arrayRawOA;
        
        arrayRawOA = new int* [(nYear+1)];
        if (arrayRawOA == NULL)
            memoryError();
            
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            arrayRawOA[countY] = new int[nIteration];
            if (arrayRawOA == NULL)
                memoryError();
            
        }  
    
        // EA [(nYear+1)][nIteration]
        float **arrayRawEA;
        
        arrayRawEA = new float* [(nYear+1)];
        if (arrayRawEA == NULL)
            memoryError();
            
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            arrayRawEA[countY] = new float[nIteration];
            if (arrayRawEA == NULL)
                memoryError();
            
        }  
    
        // Ho [(nYear+1)][nIteration]
        float **arrayRawHo;
        
        arrayRawHo = new float* [(nYear+1)];
        if (arrayRawHo == NULL)
            memoryError();
            
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            arrayRawHo[countY] = new float[nIteration];
            if (arrayRawHo == NULL)
                memoryError();
        }  
    
        // He [(nYear+1)][nIteration]
        float **arrayRawHe;

        arrayRawHe = new float* [(nYear+1)];
        if (arrayRawHe == NULL)
            memoryError();
            
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            arrayRawHe[countY] = new float[nIteration];
            if (arrayRawHe == NULL)
                memoryError();            
        }  
    
        // F [(nYear+1)][nIteration]
        float **arrayRawF;

        arrayRawF = new float* [(nYear+1)];
        if (arrayRawF == NULL)
            memoryError();
            
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            arrayRawF[countY] = new float[nIteration];
            if (arrayRawF == NULL)
                memoryError();            
        }  
        
    // vector for list
        // allele pool [(nPB * allelePerLocus)]
        // allele pool of the pre-bottleneck population 
        vector<int> vectAllelePool;

        // get allele count
        getDiSLAlleleCount(arrayAlleleCount, inputAlleleFreq, nAllele, nPB, allelePerLocus);

        // fill allele pool vector
        for (countA = 0; countA < nAllele; countA++)
        {
            for (countAPool = 0; countAPool < (arrayAlleleCount[countA]); countAPool++)
            {
                vectAllelePool.push_back(countA);
            }
        }

        // pre bottleneck population list [nPB]
        // list of all individuals in the pre bottleneck population
        // for selecting individuals survived bottleneck
        vector<int> vectPBPList;
        
        for (countN = 0; countN < nPB; countN++)
            vectPBPList.push_back(countN);


    // start of iteration loop
    for (countI = 0; countI < nIteration; countI++)
    {
        // generation pre-bottleneck population 
            // random shuffle allele pool
            random_shuffle(vectAllelePool.begin(), vectAllelePool.end());
            
            // fill genotype array
            countAPool = 0; // reset allele pool counting variable

            for (countN = 0; countN < nPB; countN++)
            {
                for (countA = 0; countA < allelePerLocus; countA++)
                {        	            
                    arrayPBP[countN][countA] = vectAllelePool[countAPool];
                    countAPool++;
                }
            }
            
        // stat of pre bottleneck population
            // call statDiploidSingleLocus function
            statDiploidSingleLocus(arrayPBP, allelePerLocus, nPB, nAllele, nOA, nEA, nHo, nHe, nF);
            
            // write to raw data array
            arrayRawOA[0][countI] = nOA;
            arrayRawEA[0][countI] = nEA;
            arrayRawHo[0][countI] = nHo;
            arrayRawHe[0][countI] = nHe;
            arrayRawF[0][countI] = nF;
            
            // generate bottleneck population
            random_shuffle(vectPBPList.begin(), vectPBPList.end()); // survivor list
            
            for (countN = 0; countN < nB; countN++)
            {
                // genotype
                for (countA = 0; countA < allelePerLocus; countA++)
                {
                    arrayBP[countN][countA] = arrayPBP[(vectPBPList[countN])][countA];
                }
                
                // age
                // when degreeOverlap = 0, if-test will always return true -> all individual start at age 0
                // when degreeOverlap = 100, if-test will always return fales -> all individual start with random age value
                if (degreeOverlap < getRandomNumber(1, 100))
                {
                    arrayAge[countN] = 0;
                }
                else    
                {
                    arrayAge[countN] = getRandomNumber(0, (longevity - 1));
                }
            }


        // start of year loop
        // each year loop has 2 major steps
        //   1. from previous year end to current year begin
        //        generate survived population from the previous year
        //   2. from current year begin to current year end
        //        generate breeding population of the current year
        //        check age/replace old individuals reached longevity limit
        //        geneerate new individuals if population growth occurred
        //
        // individual death events (old genotypes eliminated or replaced by new ones) can happen at both steps
        //   death in step 1 is due to population decline (random death)
        //   death in step 2 is due to reaching longevity limit (check age)
        // individual birth events always happen at step 2
        //   possible birth event1: old individuals reached longevity limit
        //   possible birth event2: population growth occurred
        
        for (countY = 0; countY < nYear; countY++)
        {
            // check if asexual/monoecious or dioecious
            // if asexual/monoecious --> treat the population as a whole
            // else (dioecious)      --> treat female and male separately
            
            if (reproSys >= 1 && reproSys <= 4) // asexual/monoecious, treat the population aas a whole
            {
                // step1: from previous year end to curent year begin
                // in constant population size module all individuals survived
                // do nothing to genotype array
                nSurvivedTotal = nB;

                // age increased by 1
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    arrayAge[countN]++;
                }
                
                // step2: from current year begin to current year end
                // generate breeding population
                // check individual age
                // if age reached longevity limit -> generate new individuals
                
                // generate breeding population
                // check if reached reproductive maturation, copy genotype to arrayParent
                nAdultTotal = 0; // reset number of adults
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    if (arrayAge[countN] >= ageMaturation)
                    {
                        arrayParent[nAdultTotal][0] = arrayBP[countN][0];
                        arrayParent[nAdultTotal][1] = arrayBP[countN][1];
                        nAdultTotal++;
                    }
                } 

                // check survivor age
                // reset age and replace with new genotype if reached limit
                // else do nothing to genotype and age array
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    // check if reached longevity limit, generate new individual
                    if (arrayAge[countN] >= longevity) 
                    {
                        // reset age
                        arrayAge[countN] = 0;
                        // generate new genotype
                        // total nAdultTotal breeders
                        getNewDiSLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2);
                        // copy genotype
                        arrayBP[countN][0] = arrayNewGenotype[0];
                        arrayBP[countN][1] = arrayNewGenotype[1];
                    } // end of check age if
                    // else do nothing to genotype and age array
                } // end of check age for loop

            } // end of asexual/monoecious if
            else // dioecious reproSys, treat female/male separately
            {
                // step1: from previous year end to curent year begin
                // in constant population size module all individuals survived
                // do nothing to genotype array
                nSurvivedFemale = nFemale;
                nSurvivedMale = nMale;
                nSurvivedTotal = nB;

                // age increased by 1
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    arrayAge[countN]++;
                }
                
                
                // step2: from current year begin to current year end
                // generate breeding population
                // check individual age
                // if age reached longevity limit -> generate new individuals
                
                // generate breeding population
                // check if reached reproductive maturation, copy genotype to arrayParent
                
                // generate breeding population
                nAdultFemale = 0;
                for (countN = 0; countN < nSurvivedFemale; countN++)
                {
                    if (arrayAge[countN] >= ageMaturation)
                    {
                        arrayParent[nAdultFemale][0] = arrayBP[countN][0];
                        arrayParent[nAdultFemale][1] = arrayBP[countN][1];
                        nAdultFemale++;
                    }
                    
                }

                nAdultMale = 0;
                for (countN = 0; countN < nSurvivedMale; countN++)
                {
                    if (arrayAge[(countN + nSurvivedFemale)] >= ageMaturation)
                    {
                        arrayParent[(nAdultMale + nAdultFemale)][0] = arrayBP[(countN + nSurvivedFemale)][0];
                        arrayParent[(nAdultMale + nAdultFemale)][1] = arrayBP[(countN + nSurvivedFemale)][1];
                        nAdultMale++;
                    }
                }
                
                nAdultTotal = (nAdultFemale + nAdultMale);

                // get parent id for diecious model with single reproducing male/pair
                // single reproducing male
                // get male id
                if (reproSys == 6) 
                {
                    parent2 = getRandomNumber(nAdultFemale, (nAdultTotal - 1));
                }
                
                // single reproducing pair
                // get both parent ids 
                if (reproSys == 7) 
                {
                    parent1 = getRandomNumber(0, (nAdultFemale - 1));
                    parent2 = getRandomNumber(nAdultFemale, (nAdultTotal - 1));
                }
                
                // check survivor age
                // reset age and replace with new genotype if reached limit
                // else do nothing to genotype and age array
                
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    // check if reached longevity limit, generate new individual
                    if (arrayAge[countN] >= longevity) 
                    {
                        // reset age
                        arrayAge[countN] = 0;
                        // generate new genotype
                        // total nAdultTotal breeders
                        getNewDiSLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2);
                        // copy genotype
                        arrayBP[countN][0] = arrayNewGenotype[0];
                        arrayBP[countN][1] = arrayNewGenotype[1];
                    } // end of check age if
                    // else do nothing to genotype and age array
                } // end of check age for loop
            } // end of dioecious repro sys else

            // stat of bottleneck population
                // call statDiploidSingleLocus function
                statDiploidSingleLocus(arrayBP, allelePerLocus, nB, nAllele, nOA, nEA, nHo, nHe, nF);
                
                // write to data array
                arrayRawOA[(countY + 1)][countI] = nOA;
                arrayRawEA[(countY + 1)][countI] = nEA;
                arrayRawHo[(countY + 1)][countI] = nHo;
                arrayRawHe[(countY + 1)][countI] = nHe;
                arrayRawF[(countY + 1)][countI] = nF;

        } // end of year loop
            
        // check boolOutputRawGenotype
        if (boolOutputRawGenotype == 1)
        {
            // call writeDiSLRawGenotype function
            writeDiSLRawGenotype(rawGenotypeOutputFile, arrayBP, allelePerLocus, nB);
        }
        
        cout << '.'; // use dot to show iteration progress
        
    } // end of iteration loop

    // display message
    cout << endl;
    cout << "Simulation completed." << endl;   
    cout << endl;
    cout << "Preparing output file..." << endl;
    cout << endl;

    // write simulation settings to output file
    // call writeCPSSimSetting function
    writeCPSSimSetting(outputFile, degreeOverlap, reproSys, longevity, ageMaturation, nPB, nB, nFemale, nMale, nYear, nIteration);

    // write summary output file
    writeDiSLSummary(outputFile, nYear, nIteration, arrayRawOA, arrayRawEA, arrayRawHo, arrayRawHe, arrayRawF);

    // call closeFile function
        // close input file
        closeInputFile(alleleFreqInput);
        // close output file
        closeOutputFile(outputFile);
        // check boolOutputRawGenotype
        if (boolOutputRawGenotype == 1)
        {
            closeOutputFile(rawGenotypeOutputFile);
        }
        

} // end of simDiploidSingleLocusCPS function


// simDiploidSingleLocusVPS function
// perform diploid singlelocus variable population size simulation
void simDiploidSingleLocusVPS(void)
{

    // constants
    const int allelePerLocus = 2;
        // number of alleles per locus

    // declare loop-counting variables
    int countN = 0;
        // count individual
    int countNTemp = 0;
        // count individual
    int countA = 0;
        // count allele
    int countAPool = 0;
        // count allele pool
    int countY = 0;
        // count year
    int countI = 0;
        // count iteration

    // file operation
        // input file - allele freq setting
            // declare fileName
            fileName alleleFreqInputName; // input - allele freq 
            // declare input file
            ifstream alleleFreqInput;
            // call openInput function
            openInput(alleleFreqInput, alleleFreqInputName);
                
        // input file - population size setting
            // declare fileName
            fileName popSizeInputName; // input - population size
            // declare input file
            ifstream popSizeInput;

        // output file - simulation summary       
            // declare fileName
            fileName outputFileName; // output - simulation summary
            // declare output file
            ofstream outputFile;
            // call openOutput function
            openOutput(outputFile, outputFileName);
            // call writeOutputHeader function
            // write header to output file
            writeOutputHeader(outputFile);

        // output file - raw genotype data       
            // declare fileName
            fileName rawGenotypeOutputName;
            // declare raw data output file
            ofstream rawGenotypeOutputFile;
  

    // read input file
        // declare variables
        int nAllele = 0;
            // observed number of alleles in the pre bottleneck population

        // display
        cout << endl;
        cout << "Reading allele frequency input file..." << endl;
        
        // read nAllele
        nAllele = readInputFileNAllele(alleleFreqInput);

        // initiate input array (input allele frequency)
        double *inputAlleleFreq;
            // array for input allele frequency [nAllele]
            
        inputAlleleFreq = new double[nAllele];
        if (inputAlleleFreq == NULL)
            memoryError();
        
        // read input allele freq
        readInputFileAlleleFreq(alleleFreqInput, inputAlleleFreq, nAllele);

        // display imported input data  
        displayAlleleFreqInput(inputAlleleFreq, nAllele);
        
             
        // check input allele freq data
        checkAlleleFreqInput(inputAlleleFreq, nAllele);
        

    // get simulation settings
        // declare variables
        int degreeOverlap;
            // generation overlapping setting
        int reproSys;
            // reproductive system option
        int longevity;
            // longevity of the organism
        int ageMaturation;
            // reproductive maturation age of the organism
        int nIteration;
            // number of iterations of the run
        int boolOutputRawGenotype;
            // bool variable for output raw genotype data
        // call getVPSSimSetting function
        getVPSSimSetting(degreeOverlap, reproSys, longevity, ageMaturation, nIteration, boolOutputRawGenotype);


    // check boolOutputRawGenotype
    if (boolOutputRawGenotype == 1)
        {
            // open raw data output file
            // call openRawGenotypeOutput function
            openRawGenotypeOutput(rawGenotypeOutputFile, rawGenotypeOutputName);
            // write header
            writeSLRawGenotypeHeader(rawGenotypeOutputFile);
        } 


    // get population size settings
    // call openPopSizeInput function
    openPopSizeInput(popSizeInput, popSizeInputName);
                
    // read population size setting input file
        // declare variables
        int nYear;
            // duration of bottleneck

        // display
        cout << endl;
        cout << "Reading population size setting input file..." << endl;
        
        // read nYear
        nYear = readInputFileNYear(popSizeInput);

        // initiate pop size input data array
        int *nPopSize;
            // array for popsize [(nYear+1)]        
        nPopSize = new int [(nYear+1)];
        if (nPopSize == NULL)
            memoryError();

        int *nFemale;
            // array for nFemale [(nYear+1)]
        nFemale = new int [(nYear+1)];
        if (nFemale == NULL)
            memoryError();

        int *nMale;
            // array for nMale [(nYear+1)]
        nMale = new int [(nYear+1)];
        if (nMale == NULL)
            memoryError();

        // read population size input
        readInputFilePopSize(popSizeInput, nPopSize, nFemale, nMale, nYear, reproSys);

        // write imported population size input data  
        writeVPopSizeInput(outputFile, nPopSize, nFemale, reproSys, nYear);       

        // display imported population size input data  
        displayVPopSizeInput(nPopSize, nFemale, reproSys, nYear);       

        // check population size input
        checkVPopSsizeInput(nPopSize, nFemale, reproSys, nYear);

        // get max pop size
        int nMaxPopSize;
            // maximum population size in pop size input

        nMaxPopSize = getNMaxPopSize(nPopSize, nYear);


    // declare simulation variables
    int parent1 = 0;
        // id of parent1
    int parent2 = 0;
        // id of parent2
    int nSurvivedFemale = 0;
        // number of survived females
    int nSurvivedMale = 0;
        // number of survived males
    int nSurvivedTotal = 0;
        // number of survivors
    int nAdultFemale = 0;
        // number of adult females
    int nAdultMale = 0;
        // number of adult Males
    int nAdultTotal = 0;
        // number of adults

    
    // initiate data arrays
        // population genotype [nMaxPopSize][allelePerLocus]
        int **arrayGenotype;
        
        arrayGenotype = new int* [nMaxPopSize];
        if (arrayGenotype == NULL)
            memoryError();

        for (countN = 0; countN < nMaxPopSize; countN++)
        {
            arrayGenotype[countN] = new int[allelePerLocus];

            if (arrayGenotype == NULL)
                memoryError();
        }
        
        // population genotype (temp) [nMaxPopSize][allelePerLocus]
        int **arrayGenotypeTemp;
        
        arrayGenotypeTemp = new int* [nMaxPopSize];
        if (arrayGenotypeTemp == NULL)
            memoryError();

        for (countN = 0; countN < nMaxPopSize; countN++)
        {
            arrayGenotypeTemp[countN] = new int[allelePerLocus];

            if (arrayGenotypeTemp == NULL)
                memoryError();
        }
        
        // breeding population genotype [nMaxPopSize][allelePerLocus]
        int **arrayParent;
        
        arrayParent = new int* [nMaxPopSize];
        if (arrayParent == NULL)
            memoryError();

        for (countN = 0; countN < nMaxPopSize; countN++)
        {
            arrayParent[countN] = new int[allelePerLocus];

            if (arrayParent == NULL)
                memoryError();
        }
        
        
        // new genotype
        int *arrayNewGenotype;
        
        arrayNewGenotype = new int [allelePerLocus];
        if (arrayNewGenotype == NULL)
            memoryError();
        
        // age [nMaxPopSize]
        int *arrayAge;
    
        arrayAge = new int [nMaxPopSize];
        if (arrayAge == NULL)
            memoryError();
    
        // age (temp) [nMaxPopSize]
        int *arrayAgeTemp;
    
        arrayAgeTemp = new int [nMaxPopSize];
        if (arrayAgeTemp == NULL)
            memoryError();
    
        // allele count array [nAllele]
        int *arrayAlleleCount;

        arrayAlleleCount = new int [nAllele];
        if (arrayAlleleCount == NULL)
            memoryError();

    // declare variable        
    int nOA = 0;
        // observed number of alleles
    float nEA = 0;
        // effective number of alleles
    float nHo = 0;
        // observed heterozygosity
    float nHe = 0;
        // expected heterozygosity
    float nF = 0;
        // fixation index
        
        
    // initiate raw data array
        // OA [(nYear+1)][nIteration]
        int **arrayRawOA;
        
        arrayRawOA = new int* [(nYear+1)];
        if (arrayRawOA == NULL)
            memoryError();
            
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            arrayRawOA[countY] = new int[nIteration];
            if (arrayRawOA == NULL)
                memoryError();
            
        }  
    
        // EA [(nYear+1)][nIteration]
        float **arrayRawEA;
        
        arrayRawEA = new float* [(nYear+1)];
        if (arrayRawEA == NULL)
            memoryError();
            
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            arrayRawEA[countY] = new float[nIteration];
            if (arrayRawEA == NULL)
                memoryError();
            
        }  
    
        // Ho [(nYear+1)][nIteration]
        float **arrayRawHo;
        
        arrayRawHo = new float* [(nYear+1)];
        if (arrayRawHo == NULL)
            memoryError();
            
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            arrayRawHo[countY] = new float[nIteration];
            if (arrayRawHo == NULL)
                memoryError();
        }  
    
        // He [(nYear+1)][nIteration]
        float **arrayRawHe;

        arrayRawHe = new float* [(nYear+1)];
        if (arrayRawHe == NULL)
            memoryError();
            
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            arrayRawHe[countY] = new float[nIteration];
            if (arrayRawHe == NULL)
                memoryError();            
        }  
    
        // F [(nYear+1)][nIteration]
        float **arrayRawF;

        arrayRawF = new float* [(nYear+1)];
        if (arrayRawF == NULL)
            memoryError();
            
        for (countY = 0; countY < (nYear + 1); countY++)
        {
            arrayRawF[countY] = new float[nIteration];
            if (arrayRawF == NULL)
                memoryError();            
        }  

        
    // vector for list
        // allele pool [(nPopSize[0] * allelePerLocus)]
        // allele pool of the pre-bottleneck population 
        vector<int> vectAllelePool;
                    
        // get allele count
        getDiSLAlleleCount(arrayAlleleCount, inputAlleleFreq, nAllele, nPopSize[0], allelePerLocus);
        
        // fill allele pool vector
        for (countA = 0; countA < nAllele; countA++)
        {
            for (countAPool = 0; countAPool < (arrayAlleleCount[countA]); countAPool++)
            {
                vectAllelePool.push_back(countA);
            }
        }

        // population list 
        // list of all individuals in the population
        // for selecting surviving individuals 
        vector<int> vectPopList;
        
    // start of iteration loop
    for (countI = 0; countI < nIteration; countI++)
    {
        // generation year0 population 
            // random shuffle allele pool
            random_shuffle(vectAllelePool.begin(), vectAllelePool.end());
            
            // fill genotype array (previous year end genotype array)
            countAPool = 0; // reset allele pool counting variable

            for (countN = 0; countN < nPopSize[0]; countN++)
            {
                for (countA = 0; countA < allelePerLocus; countA++)
                {        	            
                    arrayGenotype[countN][countA] = vectAllelePool[countAPool];
                    countAPool++;
                }
            }
            
        // stat of year0 population
            // call statDiploidSingleLocus function
            statDiploidSingleLocus(arrayGenotype, allelePerLocus, nPopSize[0], nAllele, nOA, nEA, nHo, nHe, nF);
            
            // write to raw data array
            arrayRawOA[0][countI] = nOA;
            arrayRawEA[0][countI] = nEA;
            arrayRawHo[0][countI] = nHo;
            arrayRawHe[0][countI] = nHe;
            arrayRawF[0][countI] = nF;

        // fill age array            
            for (countN = 0; countN < nPopSize[0]; countN++)
            {
                // age
                // when degreeOverlap = 0, if-test will always return true -> all individual start at age 0
                // when degreeOverlap = 100, if-test will always return fales -> all individual start with random age value
                if (degreeOverlap < getRandomNumber(1, 100))
                {
                    arrayAge[countN] = 0;
                }
                else    
                {
                    arrayAge[countN] = getRandomNumber(0, (longevity - 1));
                }
            }


        // start of year loop
        // each year loop has 2 major steps
        //   1. from previous year end to current year begin
        //        generate survived population from the previous year
        //   2. from current year begin to current year end
        //        generate breeding population of the current year
        //        check age/replace old individuals reached longevity limit
        //        geneerate new individuals if population growth occurred
        //
        // individual death events (old genotypes eliminated or replaced by new ones) can happen at both steps
        //   death in step 1 is due to population decline (random death)
        //   death in step 2 is due to reaching longevity limit (check age)
        // individual birth events always happen at step 2
        //   possible birth event1: old individuals reached longevity limit
        //   possible birth event2: population growth occurred

        
        for (countY = 0; countY < nYear; countY++)
        {
            // check if asexual/monoecious or dioecious
            // if asexual/monoecious --> treat the population as a whole
            // else (dioecious)      --> treat female and male separately
            
            if (reproSys >= 1 && reproSys <= 4) // asexual/monoecious, treat the population aas a whole
            {
                // step1: from previous year end to curent year begin
                // generate survived population (arrayGenotypeTemp)
                // check if population decline/stasis/growth
                // if decline -> get survivor list, copy survivor genotype and age
                // if stasis/growth  -> copy all genotype and age from previous year end
                
                // check if population decline
                if (nPopSize[(countY+1)] < nPopSize[countY])
                {
                    nSurvivedTotal = nPopSize[(countY + 1)];
                    
                    // fill population list vector for selecting surviving individuals
                    for (countN = 0; countN < nPopSize[countY]; countN++)
                        vectPopList.push_back(countN);

                    // random suffle to generate survivor list
                    random_shuffle(vectPopList.begin(), vectPopList.end());
                    
                    // copy survivor genotype from arrayGenotype (previous year end) to arrayGenotypeTemp (current year begin)
                    // copy survivor age (and increase by 1)
                    for (countN = 0; countN < nSurvivedTotal; countN++)
                    {
                        arrayGenotypeTemp[countN][0] = arrayGenotype[(vectPopList[countN])][0];
                        arrayGenotypeTemp[countN][1] = arrayGenotype[(vectPopList[countN])][1];
                        arrayAgeTemp[countN] = (arrayAge[(vectPopList[countN])] + 1);
                    } // end of copy genotype/age for loop

                    // erase population list vector for next year
                        vectPopList.erase(vectPopList.begin(), vectPopList.end());
                } // end of if population decline
                else // population stasis or growth
                {
                    nSurvivedTotal = nPopSize[countY];

                    // copy all genotype and age from previous year
                    for (countN = 0; countN < nSurvivedTotal; countN++)
                    {
                        arrayGenotypeTemp[countN][0] = arrayGenotype[countN][0];
                        arrayGenotypeTemp[countN][1] = arrayGenotype[countN][1];
                        arrayAgeTemp[countN] = (arrayAge[countN] + 1);
                    }
                } // end of population stasis or growth else
                
                
                // step2: from current year begin to current year end
                // generate breeding population
                // check individual age
                // if age reached longevity limit -> generate new individuals
                // check if population growth -> generate new individuals
                // copy genotype from arrayGenotypeTemp (current year begin) to arrayGenotype (current year end) 
                // copy age from arrayAgeTemp to arrayAge
                // if population decline /stasis
                //   -> total nPopSize[(countY+1)] individuals, all are from the previous year 
                // if population growth 
                //   -> total nPopSize[(countY+1)] individuals, nPopSize[countY] are from the previous year 
                //   -> individuals from (nPopSize[countY]+1) to nPopSize[(countY+1)] are newborns of current year
                
                // generate breeding population
                // check if reached reproductive maturation, copy genotype to arrayParent
                nAdultTotal = 0; // reset number of adults
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    if (arrayAgeTemp[countN] >= ageMaturation)
                    {
                        arrayParent[nAdultTotal][0] = arrayGenotypeTemp[countN][0];
                        arrayParent[nAdultTotal][1] = arrayGenotypeTemp[countN][1];
                        nAdultTotal++;
                    }
                } 

                // check survivor age
                // replace with new genotype if reached limit
                // else copy from arrayGenotypeTemp to arrayGenotype
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    // check if reached longevity limit, generate new individual
                    if (arrayAgeTemp[countN] >= longevity) 
                    {
                        // reset age
                        arrayAge[countN] = 0;
                        // generate new genotype
                        // total nAdultTotal breeders
                        getNewDiSLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2);
                        // copy genotype
                        arrayGenotype[countN][0] = arrayNewGenotype[0];
                        arrayGenotype[countN][1] = arrayNewGenotype[1];
                    } // end of check age if
                    else
                    {
                        // copy age
                        arrayAge[countN] = arrayAgeTemp[countN];
                        // copy genotype (from current year begin to current year end)
                        arrayGenotype[countN][0] = arrayGenotypeTemp[countN][0];
                        arrayGenotype[countN][1] = arrayGenotypeTemp[countN][1];
                    }
                } // end of check age for loop

                // if population growth occurs 
                // individual from (nPopSize[countY]) to (nPopSize[(countY+1)] - 1) are newborns of current year
                for (countN = nPopSize[countY]; countN < nPopSize[(countY+1)]; countN++)
                {
                    // reset age (newborn age = 0)
                    arrayAge[countN] = 0;
                    // generate new genotype
                    // total nAdultTotal breeders
                    getNewDiSLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2);
                    // copy genotype 
                    arrayGenotype[countN][0] = arrayNewGenotype[0];
                    arrayGenotype[countN][1] = arrayNewGenotype[1];
                } // end of population growth for loop

            } // end of asexual/monoecious if
            else // dioecious reproSys, treat female/male separately
            {
                // step1: from previous year end to curent year begin
                // generate breeding population (arrayGenotypeTemp)
                // check if nFemale/nMale decline/stasis/growth
                // if decline -> get survivor list, copy survivor genotype and age
                // if stasis/growth  -> copy all genotype and age from previous year end
                
                // check if nFemale decline
                if (nFemale[(countY+1)] < nFemale[countY])
                {
                    // number of survived female = nFemale[(countY + 1)]
                    nSurvivedFemale = nFemale[(countY + 1)];
                    
                    // fill nFemale list vector for selecting surviving individuals
                    for (countN = 0; countN < nFemale[countY]; countN++)
                        vectPopList.push_back(countN);

                    // random suffle to generate survivor list
                    random_shuffle(vectPopList.begin(), vectPopList.end());
                    
                    // copy survivor genotype from arrayGenotype (previous year end) to arrayGenotypeTemp (current year begin)
                    // copy survivor age (and increase by 1)
                    for (countN = 0; countN < nSurvivedFemale; countN++)
                    {
                        arrayGenotypeTemp[countN][0] = arrayGenotype[(vectPopList[countN])][0];
                        arrayGenotypeTemp[countN][1] = arrayGenotype[(vectPopList[countN])][1];
                        arrayAgeTemp[countN] = (arrayAge[(vectPopList[countN])] + 1);
                    } // end of copy genotype/age for loop

                    // erase population list vector for next year
                        vectPopList.erase(vectPopList.begin(), vectPopList.end());
                } // end of if nFemale decline
                else // nFemale stasis or growth
                {
                    // number of survived female = nFemale[countY]
                    nSurvivedFemale = nFemale[countY];
                    
                    // copy all genotype and age from previous year
                    for (countN = 0; countN < nSurvivedFemale; countN++)
                    {
                        arrayGenotypeTemp[countN][0] = arrayGenotype[countN][0];
                        arrayGenotypeTemp[countN][1] = arrayGenotype[countN][1];
                        arrayAgeTemp[countN] = (arrayAge[countN] + 1);
                    }
                } // end of nFemale stasis or growth else
                
                
                // check if nMale decline
                if (nMale[(countY+1)] < nMale[countY])
                {
                    // number of survived male = nMale[(countY + 1)]
                    nSurvivedMale = nMale[(countY + 1)];
                    
                    // fill nMale list vector for selecting surviving individuals
                    for (countN = 0; countN < nMale[countY]; countN++)
                        vectPopList.push_back((countN + nFemale[countY]));

                    // random suffle to generate survivor list
                    random_shuffle(vectPopList.begin(), vectPopList.end());
                    
                    // copy survivor genotype from arrayGenotype (previous year end) to arrayGenotypeTemp (current year begin)
                    // copy survivor age (and increase by 1)
                    for (countN = 0; countN < nSurvivedMale; countN++)
                    {
                        arrayGenotypeTemp[(countN + nSurvivedFemale)][0] = arrayGenotype[(vectPopList[countN])][0];
                        arrayGenotypeTemp[(countN + nSurvivedFemale)][1] = arrayGenotype[(vectPopList[countN])][1];
                        arrayAgeTemp[(countN + nSurvivedFemale)] = (arrayAge[(vectPopList[countN])] + 1);
                    } // end of copy genotype/age for loop

                    // erase population list vector for next year
                        vectPopList.erase(vectPopList.begin(), vectPopList.end());
                } // end of if nMale decline
                else // nMale stasis or growth
                {
                    // number of survived male = nMale[countY]
                    nSurvivedMale = nMale[countY];
                    
                    // copy all genotype and age from previous year
                    for (countN = 0; countN < nSurvivedMale; countN++)
                    {
                        arrayGenotypeTemp[(countN + nSurvivedFemale)][0] = arrayGenotype[(countN + nFemale[countY])][0];
                        arrayGenotypeTemp[(countN + nSurvivedFemale)][1] = arrayGenotype[(countN + nFemale[countY])][1];
                        arrayAgeTemp[(countN + nSurvivedFemale)] = (arrayAge[(countN + nFemale[countY])] + 1);
                    }
                } // end of nMale stasis or growth else

                
                // step2: from current year begin to current year end
                // generate breeding population
                // check individual age
                // if age reached longevity limit -> generate new individuals
                // check if population growth -> generate new individuals
                // copy genotype from arrayGenotypeTemp (current year begin) to arrayGenotype (current year end) 
                // copy age from arrayAgeTemp to arrayAge
                // generate female/male newborns if needed
                
                // generate breeding population
                nAdultFemale = 0;
                for (countN = 0; countN < nSurvivedFemale; countN++)
                {
                    if (arrayAgeTemp[countN] >= ageMaturation)
                    {
                        arrayParent[nAdultFemale][0] = arrayGenotypeTemp[countN][0];
                        arrayParent[nAdultFemale][1] = arrayGenotypeTemp[countN][1];
                        nAdultFemale++;
                    }
                }

                nAdultMale = 0;
                for (countN = 0; countN < nSurvivedMale; countN++)
                {
                    if (arrayAgeTemp[(countN + nSurvivedFemale)] >= ageMaturation)
                    {
                        arrayParent[(nAdultMale + nAdultFemale)][0] = arrayGenotypeTemp[(countN + nSurvivedFemale)][0];
                        arrayParent[(nAdultMale + nAdultFemale)][1] = arrayGenotypeTemp[(countN + nSurvivedFemale)][1];
                        nAdultMale++;
                    }
                }
                
                nAdultTotal = (nAdultFemale + nAdultMale);

                // get parent id for diecious model with single reproducing male/pair
                // single reproducing male
                // get male id
                if (reproSys == 6) 
                {
                    parent2 = getRandomNumber(nAdultFemale, (nAdultTotal - 1));
                }
                
                // single reproducing pair
                // get both parent ids 
                if (reproSys == 7) 
                {
                    parent1 = getRandomNumber(0, (nAdultFemale - 1));
                    parent2 = getRandomNumber(nAdultFemale, (nAdultTotal - 1));
                }
                
                // check survived female age
                // replace with new genotype if reached limit
                // else copy from arrayGenotypeTemp to arrayGenotype

                for (countN = 0; countN < nSurvivedFemale; countN++)
                {
                    // check if reached longevity limit, generate new individual
                    if (arrayAgeTemp[countN] >= longevity) 
                    {
                        // reset age
                        arrayAge[countN] = 0;
                        // generate new genotype
                        // total nAdultTotal breeders
                        getNewDiSLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2);
                        // copy genotype
                        arrayGenotype[countN][0] = arrayNewGenotype[0];
                        arrayGenotype[countN][1] = arrayNewGenotype[1];
                    } // end of check age if
                    else
                    {
                        // copy age
                        arrayAge[countN] = arrayAgeTemp[countN];
                        // copy genotype (from current year begin to current year end)
                        arrayGenotype[countN][0] = arrayGenotypeTemp[countN][0];
                        arrayGenotype[countN][1] = arrayGenotypeTemp[countN][1];
                    }
                } // end of check age for loop

                // if nFemale growth occurs 
                // individual from (nFemale[countY]) to (nFemale[(countY+1)] - 1) are newborns of current year
                for (countN = nFemale[countY]; countN < nFemale[(countY+1)]; countN++)
                {
                    // reset age (newborn age = 0)
                    arrayAge[countN] = 0;
                    // generate new genotype
                    // total nAdultTotal breeders
                    getNewDiSLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2);
                    // copy genotype 
                    arrayGenotype[countN][0] = arrayNewGenotype[0];
                    arrayGenotype[countN][1] = arrayNewGenotype[1];
                } // end of population growth for loop

                // check survived male age
                // replace with new genotype if reached limit
                // else copy from arrayGenotypeTemp to arrayGenotype

                for (countN = 0; countN < nSurvivedMale; countN++)
                {
                    // check if reached longevity limit, generate new individual
                    if (arrayAgeTemp[(countN + nSurvivedFemale)] >= longevity) 
                    {
                        // reset age
                        arrayAge[(countN + nFemale[(countY + 1)])] = 0;
                        // generate new genotype
                        // total nAdultTotal breeders
                        getNewDiSLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2);
                        // copy genotype
                        arrayGenotype[(countN + nFemale[(countY + 1)])][0] = arrayNewGenotype[0];
                        arrayGenotype[(countN + nFemale[(countY + 1)])][1] = arrayNewGenotype[1];
                    } // end of check age if
                    else
                    {
                        // copy age
                        arrayAge[(countN + nFemale[(countY + 1)])] = arrayAgeTemp[(countN + nSurvivedFemale)];
                        // copy genotype (from current year begin to current year end)
                        arrayGenotype[(countN + nFemale[(countY + 1)])][0] = arrayGenotypeTemp[(countN + nSurvivedFemale)][0];
                        arrayGenotype[(countN + nFemale[(countY + 1)])][1] = arrayGenotypeTemp[(countN + nSurvivedFemale)][1];
                    }
                } // end of check age for loop

                // if nMale growth occurs 
                for (countN = 0; countN < (nMale[(countY+1)] - nMale[countY]); countN++)
                {
                    // reset age (newborn age = 0)
                    arrayAge[(countN + nFemale[(countY + 1)] + nSurvivedMale)] = 0;
                    // generate new genotype
                    // total nAdultTotal breeders
                    getNewDiSLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2);
                    // copy genotype 
                    arrayGenotype[(countN + nFemale[(countY + 1)] + nSurvivedMale)][0] = arrayNewGenotype[0];
                    arrayGenotype[(countN + nFemale[(countY + 1)] + nSurvivedMale)][1] = arrayNewGenotype[1];
                } // end of nMale growth for loop
            } // end of dioecious reproSys else

            // stat 
                // call statDiploidSingleLocus function
                statDiploidSingleLocus(arrayGenotype, allelePerLocus, nPopSize[(countY+1)], nAllele, nOA, nEA, nHo, nHe, nF);
                
                // write to data array
                arrayRawOA[(countY + 1)][countI] = nOA;
                arrayRawEA[(countY + 1)][countI] = nEA;
                arrayRawHo[(countY + 1)][countI] = nHo;
                arrayRawHe[(countY + 1)][countI] = nHe;
                arrayRawF[(countY + 1)][countI] = nF;

        } // end of year loop
            
        // check boolOutputRawGenotype
        if (boolOutputRawGenotype == 1)
        {
            // call writeDiSLRawGenotype function
            writeDiSLRawGenotype(rawGenotypeOutputFile, arrayGenotype, allelePerLocus, nPopSize[nYear]);
        }
        
        cout << '.'; // use dot to show iteration progress
        
    } // end of iteration loop

    // display message
    cout << endl;
    cout << "Simulation completed." << endl;   
    cout << endl;
    cout << "Preparing output file..." << endl;
    cout << endl;

    // write simulation settings to output file
    // call writeVPSSimSetting function
    writeVPSSimSetting(outputFile, degreeOverlap, reproSys, longevity, ageMaturation, nYear, nIteration);

    // write summary output file
    writeDiSLSummary(outputFile, nYear, nIteration, arrayRawOA, arrayRawEA, arrayRawHo, arrayRawHe, arrayRawF);
    

    // call closeFile function
        // close input file
        closeInputFile(alleleFreqInput);
        closeInputFile(popSizeInput);
        // close output file
        closeOutputFile(outputFile);
        // check boolOutputRawGenotype
        if (boolOutputRawGenotype == 1)
        {
            closeOutputFile(rawGenotypeOutputFile);
        }
        

} // end of simDiploidSingleLocusVPS function



// simDiploidMultiLocusCPS function
// perform diploid multilocus constant population size simulation
void simDiploidMultiLocusCPS(void)
{
    // constants
    const int allelePerLocus = 2;
        // number of alleles per locus

    // declare loop counting variables
    int countN = 0; 
        // count individual 
    int countL = 0; 
        // count locus 
    int countA = 0; 
        // count allele 
    int countAId = 0; 
        // count allele id
    int countY = 0;
        // count year
    int countI = 0;
        // count iteration


    // file operation
        // input file - allele freq setting
            // declare fileName
            fileName genotypeInputName; // input - genotype data 
            // declare input file
            ifstream genotypeInputFile;
            // call openInput function
            openInput(genotypeInputFile, genotypeInputName);
                
        // output file - simulation summary       
            // declare fileName
            fileName outputFileName; // output - simulation summary
            // declare output file
            ofstream outputFile;
            // call openOutput function
            openOutput(outputFile, outputFileName);
            // call writeOutputHeader function
            // write header to output file
            writeOutputHeader(outputFile);

        // output file - raw genotype data       
            // declare fileName
            fileName rawGenotypeOutputName;
            // declare raw data output file
            ofstream rawGenotypeOutputFile;
  


    // read input file
        // declare variables        
        int nSize = 0;
            // sample size (# of individuals)
        int nLoci = 0;
            // # of loci
            
        // display
        cout << endl;
        cout << "Reading input file..." << endl;
        
        // read nSize
        nSize = readInputFileNSize(genotypeInputFile);
        
        // read nLoci
        nLoci = readInputFileNLoci(genotypeInputFile);
        
        
        // initiate input array
            // array to hold locus name
            locusName *arrayInputLocus;
            
            arrayInputLocus = new locusName[nLoci];
            if (arrayInputLocus == NULL)
                memoryError();            
            
            // array to hold individual ID
            individualId *arrayInputId;
            
            arrayInputId = new individualId[nSize];
            if (arrayInputId == NULL)
                memoryError();
                        
            // array to hold input genotype data
            char ***arrayInput;

            arrayInput = new char** [nSize];
            if (arrayInput == NULL)
                memoryError();
            
            for (countN = 0; countN < nSize; countN++)
            {
                arrayInput[countN] = new char* [nLoci];
                if (arrayInput == NULL)
                    memoryError();
                
                for (countL = 0; countL < nLoci; countL++)
                {
                    arrayInput[countN][countL] = new char [allelePerLocus];
                    if (arrayInput == NULL)
                        memoryError();
            
                }
            }
        
        // read input data
            // locus name
            readInputFileLocusName(genotypeInputFile, arrayInputLocus, nLoci);
            
            // genotype
            readInputFileGenotype(genotypeInputFile, arrayInputId, arrayInput, nSize, nLoci, allelePerLocus);
        
        // display imported input data  
        displayGenotypeInput(arrayInputLocus, arrayInputId, arrayInput, nSize, nLoci, allelePerLocus);

    // calculate allele freq from input file
        // initiate data array
            // arrayAlleleCount
            // array for allele count [nLoci][maxAllele]
            int **arrayAlleleCount;
            
            arrayAlleleCount = new int* [nLoci] ;
            if (arrayAlleleCount == NULL)
                memoryError();
                
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayAlleleCount[countL] = new int [maxAllele];
                if (arrayAlleleCount == NULL)
                    memoryError();
                
            }
            
            // alleleTotalCount 
            // array for allele total count [nLoci]
            int  *alleleTotalCount;
            
            alleleTotalCount = new int [nLoci];
            if (alleleTotalCount == NULL) 
                memoryError();
            
            // arrayAlleleFreq
            // array for allele freq [nLoci][maxAllele]
            float **arrayAlleleFreq;
            
            arrayAlleleFreq = new float* [nLoci] ;
            if (arrayAlleleFreq == NULL)
                memoryError();
                
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayAlleleFreq[countL] = new float [maxAllele];
                if (arrayAlleleFreq == NULL)
                    memoryError();
                
            }
        // call statGenotypeInput function
        statGenotypeInput(arrayInput, arrayAlleleCount, alleleTotalCount, arrayAlleleFreq, nSize, nLoci, allelePerLocus);            

    // write input allele freq data to output file
    // call writeAlleleFreq function
    writeAlleleFreq(outputFile, nLoci, arrayInputLocus, arrayAlleleFreq);

    // get simulation settings
        // declare variables
        int degreeOverlap;
            // generation overlapping setting
        int reproSys;
            // reproductive system option
        int longevity;
            // longevity of the organism
        int ageMaturation;
            // reproductive maturation age of the organism
        int nPB;
            // pre-bottleneck population size 
        int nB;
            // bottlneck population size 
            
            // number of female/male for diecious models 
            int nFemale;
            int nMale;
            
        int nYear;
            // duration of bottleneck
        int nIteration;
            // number of iterations of the run

        int boolOutputRawGenotype;
            // bool variable for output raw genotype data
        // call getCPSSimSetting function
        getCPSSimSetting(degreeOverlap, reproSys, longevity, ageMaturation, nPB, nB, nFemale, nMale, nYear, nIteration, boolOutputRawGenotype);


    // check boolOutputRawGenotype
    if (boolOutputRawGenotype == 1)
        {
            // open raw data output file
            // call openRawGenotypeOutput function
            openRawGenotypeOutput(rawGenotypeOutputFile, rawGenotypeOutputName);
            // write header
            writeMLRawGenotypeHeader(rawGenotypeOutputFile, arrayInputLocus, nLoci);
        } 


    // declare simulation variables
    int parent1 = 0;
        // id of parent1
    int parent2 = 0;
        // id of parent2
    int nSurvivedFemale = 0;
        // number of survived females
    int nSurvivedMale = 0;
        // number of survived males
    int nSurvivedTotal = 0;
        // number of survivors
    int nAdultFemale = 0;
        // number of adult females
    int nAdultMale = 0;
        // number of adult Males
    int nAdultTotal = 0;
        // number of adults

    
    // initiate data arrays
        // pre-bottleneck population genotype [nPB][nLoci][allelePerLocus]
        int ***arrayPBP;
        
        arrayPBP = new int** [nPB];
        if (arrayPBP == NULL)
            memoryError();
            
        for (countN = 0; countN < nPB; countN++)
        {
            arrayPBP[countN] = new int* [nLoci];
            if (arrayPBP == NULL)
                memoryError();
            
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayPBP[countN][countL] = new int[allelePerLocus];
                if (arrayPBP == NULL)
                    memoryError();
            
            }
        }
        
        // bottleneck population genotype [nB][nLoci][allelePerLocus]
        int ***arrayBP;
        
        arrayBP = new int** [nB];
        if (arrayBP == NULL)
            memoryError();
            
        for (countN = 0; countN < nB; countN++)
        {
            arrayBP[countN] = new int* [nLoci];
            if (arrayBP == NULL)
                memoryError();
            
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayBP[countN][countL] = new int[allelePerLocus];
                if (arrayBP == NULL)
                    memoryError();
            
            }
        }

        // breeding population genotype [nB][nLoci][allelePerLocus]
        int ***arrayParent;
        
        arrayParent = new int** [nB];
        if (arrayParent == NULL)
            memoryError();
            
        for (countN = 0; countN < nB; countN++)
        {
            arrayParent[countN] = new int* [nLoci];
            if (arrayParent == NULL)
                memoryError();
            
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayParent[countN][countL] = new int[allelePerLocus];
                if (arrayParent == NULL)
                    memoryError();
            
            }
        }
        
        // genotype array for new individuals
        int **arrayNewGenotype;
        
        arrayNewGenotype = new int* [nLoci];
        if (arrayNewGenotype == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayNewGenotype[countL] = new int [allelePerLocus];
            if (arrayNewGenotype == NULL)
                memoryError();
        }
        
        // age [nB]
        int *arrayAge;
    
        arrayAge = new int [nB];
        if (arrayAge == NULL)
            memoryError();                

    // initiate stat array [nLoci]
        // OA (observed number of alleles)      
        int *arrayOA;
        
        arrayOA = new int [nLoci];
        if (arrayOA == NULL )
            memoryError();
            
        // EA (effective number of alleles)
        float *arrayEA;
        
        arrayEA = new float [nLoci];
        if (arrayEA == NULL)
            memoryError();
            
        // Ho (observed heterozygosity)
        float *arrayHo;
        
        arrayHo = new float [nLoci];
        if (arrayHo == NULL)
            memoryError();
            
        // He (expected heterozygosity)
        float *arrayHe;
        
        arrayHe = new float [nLoci];
        if (arrayHe == NULL)
            memoryError();
            
        // F (fixation index)
        float *arrayF;
        
        arrayF = new float [nLoci];
        if (arrayF == NULL)
            memoryError();
            
    

    // initiate raw data array [nLoci][nYear][nIteration]
        // OA [nLoci][nYear][nIteration]
        int ***arrayRawOA;

        arrayRawOA = new int** [nLoci];
        if (arrayRawOA == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayRawOA[countL] = new int* [(nYear + 1)];
            if (arrayRawOA == NULL)
                memoryError();
        
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                arrayRawOA[countL][countY] = new int [nIteration];
                if (arrayRawOA == NULL)
                    memoryError();
        
            }
        }
        
        // EA [nLoci][nYear][nIteration]
        float ***arrayRawEA;
        
        arrayRawEA = new float** [nLoci];
        if (arrayRawEA == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayRawEA[countL] = new float* [(nYear + 1)];
            if (arrayRawEA == NULL)
                memoryError();
        
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                arrayRawEA[countL][countY] = new float [nIteration];
                if (arrayRawEA == NULL)
                    memoryError();
        
            }
        }
        
        // Ho [nLoci][nYear][nIteration]
        float ***arrayRawHo;
        
        arrayRawHo = new float** [nLoci];
        if (arrayRawHo == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayRawHo[countL] = new float* [(nYear + 1)];
            if (arrayRawHo == NULL)
                memoryError();
        
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                arrayRawHo[countL][countY] = new float [nIteration];
                if (arrayRawHo == NULL)
                    memoryError();
        
            }
        }
        
        // He [nLoci][nYear][nIteration]
        float ***arrayRawHe;
        
        arrayRawHe = new float** [nLoci];
        if (arrayRawHe == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayRawHe[countL] = new float* [(nYear + 1)];
            if (arrayRawHe == NULL)
                memoryError();
        
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                arrayRawHe[countL][countY] = new float [nIteration];
                if (arrayRawHe == NULL)
                    memoryError();
        
            }
        }
         
        // F [nLoci][nYear][nIteration]
        float ***arrayRawF;
        
        arrayRawF = new float** [nLoci];
        if (arrayRawF == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayRawF[countL] = new float* [(nYear + 1)];
            if (arrayRawF == NULL)
                memoryError();
        
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                arrayRawF[countL][countY] = new float [nIteration];
                if (arrayRawF == NULL)
                    memoryError();
        
            }
        }

    // vector for list
        // allele pool for generating the pre-bottleneck population 
        vector<int> vectAllelePool;
                    
        // pre bottleneck population list [nPB]
        // list of all individuals in the pre bottleneck population
        // for selecting individuals survived bottleneck
        vector<int> vectPBPList;
        
        for (countN = 0; countN < nPB; countN++)
            vectPBPList.push_back(countN);


    // start simulation
    // start of iteration loop
    for (countI = 0; countI < nIteration; countI++)
    {
        // generate the pre bottleneck population
        for (countL = 0; countL < nLoci; countL++)
        {
            // reset allele pool vector
            vectAllelePool.erase(vectAllelePool.begin(), vectAllelePool.end());
            
            // fill allele pool vector with the number of allele counts from input data
            for (countA = 0; countA < maxAllele; countA++)
            {
                for (countAId = 0; countAId < arrayAlleleCount[countL][countA]; countAId++)
                {
                    vectAllelePool.push_back(countA);
                }
            }
            
            // fill pre bottleneck population genotype array
            for (countN = 0; countN < nPB; countN++)
            {
                for (countA = 0; countA < allelePerLocus; countA++)
                {
                    // random shuffle allele pool
                    random_shuffle(vectAllelePool.begin(), vectAllelePool.end());
                    // get allele from allele pool
                    arrayPBP[countN][countL][countA] = vectAllelePool[0];
                }
            }
        } // end of countL loop  for generation pre bottleneck population

        // stat of pre bottleneck population
            // call statDiploidMultiLocus function
            statDiploidMultiLocus(arrayPBP, allelePerLocus, nPB, nLoci, arrayOA, arrayEA, arrayHo, arrayHe, arrayF);

            // write to raw data array
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayRawOA[countL][0][countI] = arrayOA[countL];
                arrayRawEA[countL][0][countI] = arrayEA[countL];
                arrayRawHo[countL][0][countI] = arrayHo[countL];
                arrayRawHe[countL][0][countI] = arrayHe[countL];
                arrayRawF[countL][0][countI] = arrayF[countL];
            }

        // generate bottleneck population
        random_shuffle(vectPBPList.begin(), vectPBPList.end()); // survivor list
        
        for (countN = 0; countN < nB; countN++)
        {
            // genotype
            for (countL = 0; countL < nLoci; countL++)
            {
                for (countA = 0; countA < allelePerLocus; countA++)
                {
                    arrayBP[countN][countL][countA] = arrayPBP[(vectPBPList[countN])][countL][countA];
                }
            }
            
            // age
            // when degreeOverlap = 0, if-test will always return true -> all individual start at age 0
            // when degreeOverlap = 100, if-test will always return fales -> all individual start with random age value
            if (degreeOverlap < getRandomNumber(1, 100))
            {
                arrayAge[countN] = 0;
            }
            else    
            {
                arrayAge[countN] = getRandomNumber(0, (longevity - 1));
            }
        }

        
        // start of year loop
        // each year loop has 2 major steps
        //   1. from previous year end to current year begin
        //        generate survived population from the previous year
        //   2. from current year begin to current year end
        //        generate breeding population of the current year
        //        check age/replace old individuals reached longevity limit
        //        geneerate new individuals if population growth occurred
        //
        // individual death events (old genotypes eliminated or replaced by new ones) can happen at both steps
        //   death in step 1 is due to population decline (random death)
        //   death in step 2 is due to reaching longevity limit (check age)
        // individual birth events always happen at step 2
        //   possible birth event1: old individuals reached longevity limit
        //   possible birth event2: population growth occurred
        
        for (countY = 0; countY < nYear; countY++)
        {

            // check if asexual/monoecious or dioecious
            // if asexual/monoecious --> treat the population as a whole
            // else (dioecious)      --> treat female and male separately
            
            if (reproSys >= 1 && reproSys <= 4) // asexual/monoecious, treat the population aas a whole
            {
                // step1: from previous year end to curent year begin
                // in constant population size module all individuals survived
                // do nothing to genotype array
                nSurvivedTotal = nB;

                // age increased by 1
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    arrayAge[countN]++;
                }
                
                // step2: from current year begin to current year end
                // generate breeding population
                // check individual age
                // if age reached longevity limit -> generate new individuals
                
                // generate breeding population
                // check if reached reproductive maturation, copy genotype to arrayParent
                nAdultTotal = 0; // reset number of adults
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    if (arrayAge[countN] >= ageMaturation)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayParent[nAdultTotal][countL][0] = arrayBP[countN][countL][0];
                            arrayParent[nAdultTotal][countL][1] = arrayBP[countN][countL][1];
                        }
                        nAdultTotal++;
                    }
                } 

                // check survivor age
                // reset age and replace with new genotype if reached limit
                // else do nothing to genotype and age array
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    // check if reached longevity limit, generate new individual
                    if (arrayAge[countN] >= longevity) 
                    {
                        // reset age
                        arrayAge[countN] = 0;
                        // generate new genotype
                        // total nAdultTotal breeders
                        getNewDiMLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2, nLoci);
                        // copy genotype
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayBP[countN][countL][0] = arrayNewGenotype[countL][0];
                            arrayBP[countN][countL][1] = arrayNewGenotype[countL][1];
                        }
                    } // end of check age if
                    // else do nothing to genotype and age array
                } // end of check age for loop

            } // end of asexual/monoecious if
            else // dioecious reproSys, treat female/male separately
            {
                // step1: from previous year end to curent year begin
                // in constant population size module all individuals survived
                // do nothing to genotype array
                nSurvivedFemale = nFemale;
                nSurvivedMale = nMale;
                nSurvivedTotal = nB;

                // age increased by 1
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    arrayAge[countN]++;
                }
                
                
                // step2: from current year begin to current year end
                // generate breeding population
                // check individual age
                // if age reached longevity limit -> generate new individuals
                
                // generate breeding population
                // check if reached reproductive maturation, copy genotype to arrayParent
                
                // generate breeding population
                nAdultFemale = 0;
                for (countN = 0; countN < nSurvivedFemale; countN++)
                {
                    if (arrayAge[countN] >= ageMaturation)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayParent[nAdultFemale][countL][0] = arrayBP[countN][countL][0];
                            arrayParent[nAdultFemale][countL][1] = arrayBP[countN][countL][1];
                        }
                        nAdultFemale++;
                    }
                }

                nAdultMale = 0;
                for (countN = 0; countN < nSurvivedMale; countN++)
                {
                    if (arrayAge[(countN + nSurvivedFemale)] >= ageMaturation)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayParent[(nAdultMale + nAdultFemale)][countL][0] = arrayBP[(countN + nSurvivedFemale)][countL][0];
                            arrayParent[(nAdultMale + nAdultFemale)][countL][1] = arrayBP[(countN + nSurvivedFemale)][countL][1];
                        }
                        nAdultMale++;
                    }
                }
                
                nAdultTotal = (nAdultFemale + nAdultMale);

                // get parent id for diecious model with single reproducing male/pair
                // single reproducing male
                // get male id
                if (reproSys == 6) 
                {
                    parent2 = getRandomNumber(nAdultFemale, (nAdultTotal - 1));
                }
                
                // single reproducing pair
                // get both parent ids 
                if (reproSys == 7) 
                {
                    parent1 = getRandomNumber(0, (nAdultFemale - 1));
                    parent2 = getRandomNumber(nAdultFemale, (nAdultTotal - 1));
                }
                
                // check survivor age
                // reset age and replace with new genotype if reached limit
                // else do nothing to genotype and age array
                
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    // check if reached longevity limit, generate new individual
                    if (arrayAge[countN] >= longevity) 
                    {
                        // reset age
                        arrayAge[countN] = 0;
                        // generate new genotype
                        // total nAdultTotal breeders
                        getNewDiMLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2, nLoci);
                        // copy genotype
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayBP[countN][countL][0] = arrayNewGenotype[countL][0];
                            arrayBP[countN][countL][1] = arrayNewGenotype[countL][1];
                        }
                    } // end of check age if
                    // else do nothing to genotype and age array
                } // end of check age for loop
            } // end of dioecious repro sys else
            
            // stat of bottleneck population
                // call statDiploidMultiLocus function
                statDiploidMultiLocus(arrayBP, allelePerLocus, nB, nLoci, arrayOA, arrayEA, arrayHo, arrayHe, arrayF);

                // write to raw data array
                for (countL = 0; countL < nLoci; countL++)
                {
                    arrayRawOA[countL][(countY + 1)][countI] = arrayOA[countL];
                    arrayRawEA[countL][(countY + 1)][countI] = arrayEA[countL];
                    arrayRawHo[countL][(countY + 1)][countI] = arrayHo[countL];
                    arrayRawHe[countL][(countY + 1)][countI] = arrayHe[countL];
                    arrayRawF[countL][(countY + 1)][countI] = arrayF[countL];
                }
        } // end of year loop

        // check boolOutputRawGenotype
        if (boolOutputRawGenotype == 1)
        {
            // call writeDiSLRawGenotype function
            writeDiMLRawGenotype(rawGenotypeOutputFile, arrayBP, allelePerLocus, nB, nLoci);
        }
        
        cout << '.'; // use dot to show iteration progress
        
    } // end of iteration loop

    // display message
    cout << endl;
    cout << "Simulation completed." << endl;   
    cout << endl;
    cout << "Preparing output file..." << endl;
    cout << endl;
    
    // write simulation settings to output file
    // call writeCPSSimSetting function
    writeCPSSimSetting(outputFile, degreeOverlap, reproSys, longevity, ageMaturation, nPB, nB, nFemale, nMale, nYear, nIteration);

    // write summary output file
    writeDiMLSummary(outputFile, nLoci, arrayInputLocus, nYear, nIteration, arrayRawOA, arrayRawEA, arrayRawHo, arrayRawHe, arrayRawF);


    // call closeFile function
        // close input file
        closeInputFile(genotypeInputFile);
        // close output file
        closeOutputFile(outputFile);
        // check boolOutputRawGenotype
        if (boolOutputRawGenotype == 1)
        {
            closeOutputFile(rawGenotypeOutputFile);
        }

} // end of diploidMultiLocusCPS function


// simDiploidMultiLocusVPS function
// perform diploid multilocus variable population size simulation
void simDiploidMultiLocusVPS(void)
{
    // constants
    const int allelePerLocus = 2;
        // number of alleles per locus

    // declare loop counting variables
    int countN = 0; 
        // count individual 
    int countL = 0; 
        // count locus 
    int countA = 0; 
        // count allele 
    int countAId = 0; 
        // count allele id
    int countY = 0;
        // count year
    int countI = 0;
        // count iteration


    // file operation
        // input file - allele freq setting
            // declare fileName
            fileName genotypeInputName; // input - genotype data 
            // declare input file
            ifstream genotypeInputFile;
            // call openInput function
            openInput(genotypeInputFile, genotypeInputName);
                
        // input file - population size setting
            // declare fileName
            fileName popSizeInputName; // input - population size
            // declare input file
            ifstream popSizeInput;

        // output file - simulation summary       
            // declare fileName
            fileName outputFileName; // output - simulation summary
            // declare output file
            ofstream outputFile;
            // call openOutput function
            openOutput(outputFile, outputFileName);
            // call writeOutputHeader function
            // write header to output file
            writeOutputHeader(outputFile);

        // output file - raw genotype data       
            // declare fileName
            fileName rawGenotypeOutputName;
            // declare raw data output file
            ofstream rawGenotypeOutputFile;
  


    // read input file
        // declare variables        
        int nSize = 0;
            // sample size (# of individuals)
        int nLoci = 0;
            // # of loci
            
        // display
        cout << endl;
        cout << "Reading input file..." << endl;
        
        // read nSize
        nSize = readInputFileNSize(genotypeInputFile);
        
        // read nLoci
        nLoci = readInputFileNLoci(genotypeInputFile);
        
        
        // initiate input array
            // array to hold locus name
            locusName *arrayInputLocus;
            
            arrayInputLocus = new locusName[nLoci];
            if (arrayInputLocus == NULL)
                memoryError();            
            
            // array to hold individual ID
            individualId *arrayInputId;
            
            arrayInputId = new individualId[nSize];
            if (arrayInputId == NULL)
                memoryError();
                        
            // array to hold input genotype data
            char ***arrayInput;

            arrayInput = new char** [nSize];
            if (arrayInput == NULL)
                memoryError();
            
            for (countN = 0; countN < nSize; countN++)
            {
                arrayInput[countN] = new char* [nLoci];
                if (arrayInput == NULL)
                    memoryError();
                
                for (countL = 0; countL < nLoci; countL++)
                {
                    arrayInput[countN][countL] = new char [allelePerLocus];
                    if (arrayInput == NULL)
                        memoryError();
                }
            }
        
        // read input data
            // locus name
            readInputFileLocusName(genotypeInputFile, arrayInputLocus, nLoci);
            
            // genotype
            readInputFileGenotype(genotypeInputFile, arrayInputId, arrayInput, nSize, nLoci, allelePerLocus);
        
        // display imported input data  
        displayGenotypeInput(arrayInputLocus, arrayInputId, arrayInput, nSize, nLoci, allelePerLocus);

    // calculate allele freq from input file
        // initiate data array
            // arrayAlleleCount
            // array for allele count [nLoci][maxAllele]
            int **arrayAlleleCount;
            
            arrayAlleleCount = new int* [nLoci] ;
            if (arrayAlleleCount == NULL)
                memoryError();
                
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayAlleleCount[countL] = new int [maxAllele];
                if (arrayAlleleCount == NULL)
                    memoryError();
            }
            
            // alleleTotalCount 
            // array for allele total count [nLoci]
            int  *alleleTotalCount;
            
            alleleTotalCount = new int [nLoci];
            if (alleleTotalCount == NULL) 
                memoryError();
            
            // arrayAlleleFreq
            // array for allele freq [nLoci][maxAllele]
            float **arrayAlleleFreq;
            
            arrayAlleleFreq = new float* [nLoci] ;
            if (arrayAlleleFreq == NULL)
                memoryError();
                
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayAlleleFreq[countL] = new float [maxAllele];
                if (arrayAlleleFreq == NULL)
                    memoryError();
            }
        // call statGenotypeInput function
        statGenotypeInput(arrayInput, arrayAlleleCount, alleleTotalCount, arrayAlleleFreq, nSize, nLoci, allelePerLocus);            

    // write input allele freq data to output file
    // call writeAlleleFreq function
    writeAlleleFreq(outputFile, nLoci, arrayInputLocus, arrayAlleleFreq);

    // get simulation settings
        // declare variables
        int degreeOverlap;
            // generation overlapping setting
        int reproSys;
            // reproductive system option
        int longevity;
            // longevity of the organism
        int ageMaturation;
            // reproductive maturation age of the organism
        int nIteration;
            // number of iterations of the run
        int boolOutputRawGenotype;
            // bool variable for output raw genotype data
        // call getVPSSimSetting function
        getVPSSimSetting(degreeOverlap, reproSys, longevity, ageMaturation, nIteration, boolOutputRawGenotype);


    // check boolOutputRawGenotype
    if (boolOutputRawGenotype == 1)
        {
            // open raw data output file
            // call openRawGenotypeOutput function
            openRawGenotypeOutput(rawGenotypeOutputFile, rawGenotypeOutputName);
            // write header
            writeMLRawGenotypeHeader(rawGenotypeOutputFile, arrayInputLocus, nLoci);
        } 


    // get population size settings
    // call openPopSizeInput function
    openPopSizeInput(popSizeInput, popSizeInputName);
                
    // read population size setting input file
        // declare variables
        int nYear;
            // duration of bottleneck

        // display
        cout << endl;
        cout << "Reading population size setting input file..." << endl;
        
        // read nYear
        nYear = readInputFileNYear(popSizeInput);

        // initiate pop size input data array
        int *nPopSize;
            // array for popsize [(nYear+1)]        
        nPopSize = new int [(nYear+1)];
        if (nPopSize == NULL)
            memoryError();

        int *nFemale;
            // array for nFemale [(nYear+1)]
        nFemale = new int [(nYear+1)];
        if (nFemale == NULL)
            memoryError();

        int *nMale;
            // array for nMale [(nYear+1)]
        nMale = new int [(nYear+1)];
        if (nMale == NULL)
            memoryError();

        // read population size input
        readInputFilePopSize(popSizeInput, nPopSize, nFemale, nMale, nYear, reproSys);

        // write imported population size input data  
        writeVPopSizeInput(outputFile, nPopSize, nFemale, reproSys, nYear);       

        // display imported population size input data  
        displayVPopSizeInput(nPopSize, nFemale, reproSys, nYear);       

        // check population size input
        checkVPopSsizeInput(nPopSize, nFemale, reproSys, nYear);

        // get max pop size
        int nMaxPopSize;
            // maximum population size in pop size input

        nMaxPopSize = getNMaxPopSize(nPopSize, nYear);


    // declare simulation variables
    int parent1 = 0;
        // id of parent1
    int parent2 = 0;
        // id of parent2
    int nSurvivedFemale = 0;
        // number of survived females
    int nSurvivedMale = 0;
        // number of survived males
    int nSurvivedTotal = 0;
        // number of survivors
    int nAdultFemale = 0;
        // number of adult females
    int nAdultMale = 0;
        // number of adult Males
    int nAdultTotal = 0;
        // number of adults

    // initiate data arrays
        // population genotype [nMaxPopSize][nLoci][allelePerLocus]
        int ***arrayGenotype;
        
        arrayGenotype = new int** [nMaxPopSize];
        if (arrayGenotype == NULL)
            memoryError();
            
        for (countN = 0; countN < nMaxPopSize; countN++)
        {
            arrayGenotype[countN] = new int* [nLoci];
            if (arrayGenotype == NULL)
                memoryError();
            
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayGenotype[countN][countL] = new int[allelePerLocus];
                if (arrayGenotype == NULL)
                    memoryError();
            
            }
        }
        
        // population genotype (temp) [nMaxPopSize][nLoci][allelePerLocus]
        int ***arrayGenotypeTemp;
        
        arrayGenotypeTemp = new int** [nMaxPopSize];
        if (arrayGenotypeTemp == NULL)
            memoryError();
            
        for (countN = 0; countN < nMaxPopSize; countN++)
        {
            arrayGenotypeTemp[countN] = new int* [nLoci];
            if (arrayGenotypeTemp == NULL)
                memoryError();
            
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayGenotypeTemp[countN][countL] = new int[allelePerLocus];
                if (arrayGenotypeTemp == NULL)
                    memoryError();
            }
        }
        
        // breeding population genotype [nMaxPopSize][nLoci][allelePerLocus]
        int ***arrayParent;
        
        arrayParent = new int** [nMaxPopSize];
        if (arrayParent == NULL)
            memoryError();
            
        for (countN = 0; countN < nMaxPopSize; countN++)
        {
            arrayParent[countN] = new int* [nLoci];
            if (arrayParent == NULL)
                memoryError();
            
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayParent[countN][countL] = new int[allelePerLocus];
                if (arrayParent == NULL)
                    memoryError();
            }
        }
        
        
        // genotype array for new individuals
        int **arrayNewGenotype;
        
        arrayNewGenotype = new int* [nLoci];
        if (arrayNewGenotype == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayNewGenotype[countL] = new int [allelePerLocus];
            if (arrayNewGenotype == NULL)
                memoryError();
        }
        
        // age [nMaxPopSize]
        int *arrayAge;
    
        arrayAge = new int [nMaxPopSize];
        if (arrayAge == NULL)
            memoryError();                

        // age (temp) [nMaxPopSize]
        int *arrayAgeTemp;
    
        arrayAgeTemp = new int [nMaxPopSize];
        if (arrayAgeTemp == NULL)
            memoryError();                

    // initiate stat array [nLoci]
        // OA (observed number of alleles)      
        int *arrayOA;
        
        arrayOA = new int [nLoci];
        if (arrayOA == NULL )
            memoryError();
            
        // EA (effective number of alleles)
        float *arrayEA;
        
        arrayEA = new float [nLoci];
        if (arrayEA == NULL)
            memoryError();
            
        // Ho (observed heterozygosity)
        float *arrayHo;
        
        arrayHo = new float [nLoci];
        if (arrayHo == NULL)
            memoryError();
            
        // He (expected heterozygosity)
        float *arrayHe;
        
        arrayHe = new float [nLoci];
        if (arrayHe == NULL)
            memoryError();
            
        // F (fixation index)
        float *arrayF;
        
        arrayF = new float [nLoci];
        if (arrayF == NULL)
            memoryError();

    // initiate raw data array [nLoci][nYear][nIteration]
        // OA [nLoci][nYear][nIteration]
        int ***arrayRawOA;

        arrayRawOA = new int** [nLoci];
        if (arrayRawOA == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayRawOA[countL] = new int* [(nYear + 1)];
            if (arrayRawOA == NULL)
                memoryError();
        
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                arrayRawOA[countL][countY] = new int [nIteration];
                if (arrayRawOA == NULL)
                    memoryError();
            }
        }
        
        // EA [nLoci][nYear][nIteration]
        float ***arrayRawEA;
        
        arrayRawEA = new float** [nLoci];
        if (arrayRawEA == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayRawEA[countL] = new float* [(nYear + 1)];
            if (arrayRawEA == NULL)
                memoryError();
        
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                arrayRawEA[countL][countY] = new float [nIteration];
                if (arrayRawEA == NULL)
                    memoryError();
            }
        }
        
        // Ho [nLoci][nYear][nIteration]
        float ***arrayRawHo;
        
        arrayRawHo = new float** [nLoci];
        if (arrayRawHo == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayRawHo[countL] = new float* [(nYear + 1)];
            if (arrayRawHo == NULL)
                memoryError();
        
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                arrayRawHo[countL][countY] = new float [nIteration];
                if (arrayRawHo == NULL)
                    memoryError();
            }
        }
        
        // He [nLoci][nYear][nIteration]
        float ***arrayRawHe;
        
        arrayRawHe = new float** [nLoci];
        if (arrayRawHe == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayRawHe[countL] = new float* [(nYear + 1)];
            if (arrayRawHe == NULL)
                memoryError();
        
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                arrayRawHe[countL][countY] = new float [nIteration];
                if (arrayRawHe == NULL)
                    memoryError();
            }
        }
         
        // F [nLoci][nYear][nIteration]
        float ***arrayRawF;
        
        arrayRawF = new float** [nLoci];
        if (arrayRawF == NULL)
            memoryError();
        
        for (countL = 0; countL < nLoci; countL++)
        {
            arrayRawF[countL] = new float* [(nYear + 1)];
            if (arrayRawF == NULL)
                memoryError();
        
            for (countY = 0; countY < (nYear + 1); countY++)
            {
                arrayRawF[countL][countY] = new float [nIteration];
                if (arrayRawF == NULL)
                    memoryError();
            }
        }

    // vector for list
        // allele pool for generating the pre-bottleneck population 
        vector<int> vectAllelePool;
                    
        // population list 
        // list of all individuals in the population
        // for selecting surviving individuals 
        vector<int> vectPopList;
        

    // start simulation
    // start of iteration loop
    for (countI = 0; countI < nIteration; countI++)
    {
        // generate the pre bottleneck population
        for (countL = 0; countL < nLoci; countL++)
        {
            // reset allele pool vector
            vectAllelePool.erase(vectAllelePool.begin(), vectAllelePool.end());
            
            // fill allele pool vector with the number of allele counts from input data
            for (countA = 0; countA < maxAllele; countA++)
            {
                for (countAId = 0; countAId < arrayAlleleCount[countL][countA]; countAId++)
                {
                    vectAllelePool.push_back(countA);
                }
            }
            
            // fill pre bottleneck population genotype array
            for (countN = 0; countN < nPopSize[0]; countN++)
            {
                for (countA = 0; countA < allelePerLocus; countA++)
                {
                    // random shuffle allele pool
                    random_shuffle(vectAllelePool.begin(), vectAllelePool.end());
                    // get allele from allele pool
                    arrayGenotype[countN][countL][countA] = vectAllelePool[0];
                }
            }
        } // end of countL loop for generating pre bottleneck population

        // stat of pre bottleneck population
            // call statDiploidMultiLocus function
            statDiploidMultiLocus(arrayGenotype, allelePerLocus, nPopSize[0], nLoci, arrayOA, arrayEA, arrayHo, arrayHe, arrayF);

            // write to raw data array
            for (countL = 0; countL < nLoci; countL++)
            {
                arrayRawOA[countL][0][countI] = arrayOA[countL];
                arrayRawEA[countL][0][countI] = arrayEA[countL];
                arrayRawHo[countL][0][countI] = arrayHo[countL];
                arrayRawHe[countL][0][countI] = arrayHe[countL];
                arrayRawF[countL][0][countI] = arrayF[countL];
            }

        // fill age array            
        for (countN = 0; countN < nPopSize[0]; countN++)
        {
            // age
            // when degreeOverlap = 0, if-test will always return true -> all individual start at age 0
            // when degreeOverlap = 100, if-test will always return fales -> all individual start with random age value
            if (degreeOverlap < getRandomNumber(1, 100))
            {
                arrayAge[countN] = 0;
            }
            else    
            {
                arrayAge[countN] = getRandomNumber(0, (longevity - 1));
            }
        }


        // start of year loop
        // each year loop has 2 major steps
        //   1. from previous year end to current year begin
        //        generate survived population from the previous year
        //   2. from current year begin to current year end
        //        generate breeding population of the current year
        //        check age/replace old individuals reached longevity limit
        //        geneerate new individuals if population growth occurred
        //
        // individual death events (old genotypes eliminated or replaced by new ones) can happen at both steps
        //   death in step 1 is due to population decline (random death)
        //   death in step 2 is due to reaching longevity limit (check age)
        // individual birth events always happen at step 2
        //   possible birth event1: old individuals reached longevity limit
        //   possible birth event2: population growth occurred
        
        for (countY = 0; countY < nYear; countY++)
        {
        
            // check if asexual/monoecious or dioecious
            // if asexual/monoecious --> treat the population as a whole
            // else (dioecious)      --> treat female and male separately
            
            if (reproSys >= 1 && reproSys <= 4) // asexual/monoecious, treat the population aas a whole
            {
                // step1: from previous year end to curent year begin
                // generate survived population (arrayGenotypeTemp)
                // check if population decline/stasis/growth
                // if decline -> get survivor list, copy survivor genotype and age
                // if stasis/growth  -> copy all genotype and age from previous year end
                
                // check if population decline
                if (nPopSize[(countY+1)] < nPopSize[countY])
                {
                    nSurvivedTotal = nPopSize[(countY + 1)];

                    // fill population list vector for selecting surviving individuals
                    for (countN = 0; countN < nPopSize[countY]; countN++)
                        vectPopList.push_back(countN);

                    // random suffle to generate survivor list
                    random_shuffle(vectPopList.begin(), vectPopList.end());
                    
                    // copy survivor genotype from arrayGenotype (previous year end) to arrayGenotypeTemp (current year begin)
                    // copy survivor age (and increase by 1)
                    for (countN = 0; countN < nSurvivedTotal; countN++)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotypeTemp[countN][countL][0] = arrayGenotype[(vectPopList[countN])][countL][0];
                            arrayGenotypeTemp[countN][countL][1] = arrayGenotype[(vectPopList[countN])][countL][1];
                        }
                        arrayAgeTemp[countN] = (arrayAge[(vectPopList[countN])] + 1);
                    } // end of copy genotype/age for loop

                    // erase population list vector for next year
                        vectPopList.erase(vectPopList.begin(), vectPopList.end());
                } // end of if population decline
                else // population stasis or growth
                {
                    nSurvivedTotal = nPopSize[countY];

                    // copy all genotype and age from previous year
                    for (countN = 0; countN < nSurvivedTotal; countN++)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotypeTemp[countN][countL][0] = arrayGenotype[countN][countL][0];
                            arrayGenotypeTemp[countN][countL][1] = arrayGenotype[countN][countL][1];
                        }
                        arrayAgeTemp[countN] = (arrayAge[countN] + 1);
                    }
                } // end of population stasis or growth else
                
                // step2: from current year begin to current year end
                // generate breeding population
                // check individual age
                // if age reached longevity limit -> generate new individuals
                // check if population growth -> generate new individuals
                // copy genotype from arrayGenotypeTemp (current year begin) to arrayGenotype (current year end) 
                // copy age from arrayAgeTemp to arrayAge
                // if population decline /stasis
                //   -> total nPopSize[(countY+1)] individuals, all are from the previous year 
                // if population growth 
                //   -> total nPopSize[(countY+1)] individuals, nPopSize[countY] are from the previous year 
                //   -> individuals from (nPopSize[countY]+1) to nPopSize[(countY+1)] are newborns of current year

                // generate breeding population
                // check if reached reproductive maturation, copy genotype to arrayParent
                nAdultTotal = 0; // reset number of adults
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    if (arrayAgeTemp[countN] >= ageMaturation)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayParent[nAdultTotal][countL][0] = arrayGenotypeTemp[countN][countL][0];
                            arrayParent[nAdultTotal][countL][1] = arrayGenotypeTemp[countN][countL][1];
                        }
                        nAdultTotal++;
                    }
                } 


                // check survivor age
                // replace with new genotype if reached limit
                // else copy from arrayGenotypeTemp to arrayGenotype
                for (countN = 0; countN < nSurvivedTotal; countN++)
                {
                    // check if reached longevity limit, generate new individual
                    if (arrayAgeTemp[countN] >= longevity) 
                    {
                        // reset age
                        arrayAge[countN] = 0;
                        // generate new genotype
                        // total nAdultTotal breeders
                        getNewDiMLGenotype(arrayParent, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2, nLoci);
                        // copy genotype
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotype[countN][countL][0] = arrayNewGenotype[countL][0];
                            arrayGenotype[countN][countL][1] = arrayNewGenotype[countL][1];
                        }
                    } // end of check age if
                    else
                    {
                        // copy age
                        arrayAge[countN] = arrayAgeTemp[countN];
                        // copy genotype (from current year begin to current year end)
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotype[countN][countL][0] = arrayGenotypeTemp[countN][countL][0];
                            arrayGenotype[countN][countL][1] = arrayGenotypeTemp[countN][countL][1];
                        }
                    }
                } // end of check age for loop

                // if population growth occurs 
                // individual from (nPopSize[countY]) to (nPopSize[(countY+1)] - 1) are newborns of current year
                for (countN = nPopSize[countY]; countN < nPopSize[(countY+1)]; countN++)
                {
                    // reset age (newborn age = 0)
                    arrayAge[countN] = 0;
                    // generate new genotype
                    // total nAdultTotal breeders
                    getNewDiMLGenotype(arrayGenotypeTemp, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2, nLoci);
                    // copy genotype 
                    for (countL = 0; countL < nLoci; countL++)
                    {
                        arrayGenotype[countN][countL][0] = arrayNewGenotype[countL][0];
                        arrayGenotype[countN][countL][1] = arrayNewGenotype[countL][1];
                    }
                } // end of population growth for loop
                
            } // end of asexual/monoecious if
            else // dioecious reproSys, treat female/male separately
            {
                
                // step1: from previous year end to curent year begin
                // generate breeding population (arrayGenotypeTemp)
                // check if nFemale/nMale decline/stasis/growth
                // if decline -> get survivor list, copy survivor genotype and age
                // if stasis/growth  -> copy all genotype and age from previous year end
                
                // check if nFemale decline
                if (nFemale[(countY+1)] < nFemale[countY])
                {
                    // number of survived female = nFemale[(countY + 1)]
                    nSurvivedFemale = nFemale[(countY + 1)];
                    
                    // fill nFemale list vector for selecting surviving individuals
                    for (countN = 0; countN < nFemale[countY]; countN++)
                        vectPopList.push_back(countN);

                    // random suffle to generate survivor list
                    random_shuffle(vectPopList.begin(), vectPopList.end());
                    
                    // copy survivor genotype from arrayGenotype (previous year end) to arrayGenotypeTemp (current year begin)
                    // copy survivor age (and increase by 1)
                    for (countN = 0; countN < nSurvivedFemale; countN++)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotypeTemp[countN][countL][0] = arrayGenotype[(vectPopList[countN])][countL][0];
                            arrayGenotypeTemp[countN][countL][1] = arrayGenotype[(vectPopList[countN])][countL][1];
                        }
                        arrayAgeTemp[countN] = (arrayAge[(vectPopList[countN])] + 1);
                    } // end of copy genotype/age for loop

                    // erase population list vector for next year
                        vectPopList.erase(vectPopList.begin(), vectPopList.end());
                } // end of if nFemale decline
                else // nFemale stasis or growth
                {
                    // number of survived female = nFemale[countY]
                    nSurvivedFemale = nFemale[countY];
                    
                    // copy all genotype and age from previous year
                    for (countN = 0; countN < nSurvivedFemale; countN++)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotypeTemp[countN][countL][0] = arrayGenotype[countN][countL][0];
                            arrayGenotypeTemp[countN][countL][1] = arrayGenotype[countN][countL][1];
                        }
                        arrayAgeTemp[countN] = (arrayAge[countN] + 1);
                    }
                } // end of nFemale stasis or growth else
                
                
                // check if nMale decline
                if (nMale[(countY+1)] < nMale[countY])
                {
                    // number of survived male = nMale[(countY + 1)]
                    nSurvivedMale = nMale[(countY + 1)];
                    
                    // fill nMale list vector for selecting surviving individuals
                    for (countN = 0; countN < nMale[countY]; countN++)
                        vectPopList.push_back((countN + nFemale[countY]));

                    // random suffle to generate survivor list
                    random_shuffle(vectPopList.begin(), vectPopList.end());
                    
                    // copy survivor genotype from arrayGenotype (previous year end) to arrayGenotypeTemp (current year begin)
                    // copy survivor age (and increase by 1)
                    for (countN = 0; countN < nSurvivedMale; countN++)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotypeTemp[(countN + nSurvivedFemale)][countL][0] = arrayGenotype[(vectPopList[countN])][countL][0];
                            arrayGenotypeTemp[(countN + nSurvivedFemale)][countL][1] = arrayGenotype[(vectPopList[countN])][countL][1];
                        }
                        arrayAgeTemp[(countN + nSurvivedFemale)] = (arrayAge[(vectPopList[countN])] + 1);
                    } // end of copy genotype/age for loop

                    // erase population list vector for next year
                        vectPopList.erase(vectPopList.begin(), vectPopList.end());
                } // end of if nMale decline
                else // nMale stasis or growth
                {
                    // number of survived male = nMale[countY]
                    nSurvivedMale = nMale[countY];
                    
                    // copy all genotype and age from previous year
                    for (countN = 0; countN < nSurvivedMale; countN++)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotypeTemp[(countN + nSurvivedFemale)][countL][0] = arrayGenotype[(countN + nFemale[countY])][countL][0];
                            arrayGenotypeTemp[(countN + nSurvivedFemale)][countL][1] = arrayGenotype[(countN + nFemale[countY])][countL][1];
                        }
                        arrayAgeTemp[(countN + nSurvivedFemale)] = (arrayAge[(countN + nFemale[countY])] + 1);
                    }
                } // end of nMale stasis or growth else

                
                // step2: from current year begin to current year end
                // check individual age
                // if age reached longevity limit -> generate new individuals
                // check if population growth -> generate new individuals
                // copy genotype from arrayGenotypeTemp (current year begin) to arrayGenotype (current year end) 
                // copy age from arrayAgeTemp to arrayAge
                // generate female/male newborns if needed

                // generate breeding population
                nAdultFemale = 0;
                for (countN = 0; countN < nSurvivedFemale; countN++)
                {
                    if (arrayAgeTemp[countN] >= ageMaturation)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayParent[nAdultFemale][countL][0] = arrayGenotypeTemp[countN][countL][0];
                            arrayParent[nAdultFemale][countL][1] = arrayGenotypeTemp[countN][countL][1];
                        }
                        nAdultFemale++;
                    }
                }

                nAdultMale = 0;
                for (countN = 0; countN < nSurvivedMale; countN++)
                {
                    if (arrayAgeTemp[(countN + nSurvivedFemale)] >= ageMaturation)
                    {
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayParent[(nAdultMale + nAdultFemale)][countL][0] = arrayGenotypeTemp[(countN + nSurvivedFemale)][countL][0];
                            arrayParent[(nAdultMale + nAdultFemale)][countL][1] = arrayGenotypeTemp[(countN + nSurvivedFemale)][countL][1];
                        }
                        nAdultMale++;
                    }
                }
                
                nAdultTotal = (nAdultFemale + nAdultMale);


                // get parent id for diecious model with single reproducing male/pair
                // single reproducing male
                // get male id
                if (reproSys == 6) 
                {
                    parent2 = getRandomNumber(nAdultFemale, (nAdultTotal - 1));
                }
                
                // single reproducing pair
                // get both parent ids 
                if (reproSys == 7) 
                {
                    parent1 = getRandomNumber(0, (nAdultFemale - 1));
                    parent2 = getRandomNumber(nAdultFemale, (nAdultTotal - 1));
                }
                
                
                // check survived female age
                // replace with new genotype if reached limit
                // else copy from arrayGenotypeTemp to arrayGenotype
                for (countN = 0; countN < nSurvivedFemale; countN++)
                {
                    // check if reached longevity limit, generate new individual
                    if (arrayAgeTemp[countN] >= longevity) 
                    {
                        // reset age
                        arrayAge[countN] = 0;
                        // generate new genotype
                        // total nAdultTotal breeders
                        getNewDiMLGenotype(arrayGenotypeTemp, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2, nLoci);
                        // copy genotype
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotype[countN][countL][0] = arrayNewGenotype[countL][0];
                            arrayGenotype[countN][countL][1] = arrayNewGenotype[countL][1];
                        }
                    } // end of check age if
                    else
                    {
                        // copy age
                        arrayAge[countN] = arrayAgeTemp[countN];
                        // copy genotype (from current year begin to current year end)
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotype[countN][countL][0] = arrayGenotypeTemp[countN][countL][0];
                            arrayGenotype[countN][countL][1] = arrayGenotypeTemp[countN][countL][1];
                        }
                    }
                }
                
                // individual from (nFemale[countY]) to (nFemale[(countY+1)] - 1) are newborns of current year
                for (countN = nFemale[countY]; countN < nFemale[(countY+1)]; countN++)
                {
                    // reset age (newborn age = 0)
                    arrayAge[countN] = 0;
                    // generate new genotype
                    // total nAdultTotal breeders
                    getNewDiMLGenotype(arrayGenotypeTemp, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2, nLoci);
                    // copy genotype 
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotype[countN][countL][0] = arrayNewGenotype[countL][0];
                            arrayGenotype[countN][countL][1] = arrayNewGenotype[countL][1];
                        }
                } // end of nFemale growth for loop
                

                // check survived male age
                // replace with new genotype if reached limit
                // else copy from arrayGenotypeTemp to arrayGenotype
                for (countN = 0; countN < nSurvivedMale; countN++)
                {
                    // check if reached longevity limit, generate new individual
                    if (arrayAgeTemp[(countN + nSurvivedFemale)] >= longevity) 
                    {
                        // reset age
                        arrayAge[(countN + nFemale[(countY + 1)])] = 0;
                        // generate new genotype
                        // total nAdultTotal breeders
                        getNewDiMLGenotype(arrayGenotypeTemp, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2, nLoci);
                        // copy genotype
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotype[(countN + nFemale[(countY + 1)])][countL][0] = arrayNewGenotype[countL][0];
                            arrayGenotype[(countN + nFemale[(countY + 1)])][countL][1] = arrayNewGenotype[countL][1];
                        }
                    } // end of check age if
                    else
                    {
                        // copy age
                        arrayAge[(countN + nFemale[(countY + 1)])] = arrayAgeTemp[(countN + nSurvivedFemale)];
                        // copy genotype (from current year begin to current year end)
                        for (countL = 0; countL < nLoci; countL++)
                        {
                            arrayGenotype[(countN + nFemale[(countY + 1)])][countL][0] = arrayGenotypeTemp[(countN + nSurvivedFemale)][countL][0];
                            arrayGenotype[(countN + nFemale[(countY + 1)])][countL][1] = arrayGenotypeTemp[(countN + nSurvivedFemale)][countL][1];
                        }
                    }
                } // end of check age for loop

                // if nMale growth occurs 
                for (countN = 0; countN < (nMale[(countY+1)] - nMale[countY]); countN++)
                {
                    // reset age (newborn age = 0)
                    arrayAge[(countN + nFemale[(countY + 1)])] = 0;
                    // generate new genotype
                    // total nAdultTotal breeders
                    getNewDiMLGenotype(arrayGenotypeTemp, arrayNewGenotype, reproSys, nAdultTotal, nAdultFemale, parent1, parent2, nLoci);
                    // copy genotype 
                    for (countL = 0; countL < nLoci; countL++)
                    {
                        arrayGenotype[(countN + nFemale[(countY + 1)] + nSurvivedMale)][countL][0] = arrayNewGenotype[countL][0];
                        arrayGenotype[(countN + nFemale[(countY + 1)] + nSurvivedMale)][countL][1] = arrayNewGenotype[countL][1];
                    }
                } // end of nMale growth for loop
                    
            } // end of dioecious reproSys else

            // stat of bottleneck population
                // call statDiploidMultiLocus function
                statDiploidMultiLocus(arrayGenotype, allelePerLocus, nPopSize[(countY+1)], nLoci, arrayOA, arrayEA, arrayHo, arrayHe, arrayF);

                // write to raw data array
                for (countL = 0; countL < nLoci; countL++)
                {
                    arrayRawOA[countL][(countY + 1)][countI] = arrayOA[countL];
                    arrayRawEA[countL][(countY + 1)][countI] = arrayEA[countL];
                    arrayRawHo[countL][(countY + 1)][countI] = arrayHo[countL];
                    arrayRawHe[countL][(countY + 1)][countI] = arrayHe[countL];
                    arrayRawF[countL][(countY + 1)][countI] = arrayF[countL];
                }

        } // end of year loop

        // check boolOutputRawGenotype
        if (boolOutputRawGenotype == 1)
        {
            // call writeDiSLRawGenotype function
            writeDiMLRawGenotype(rawGenotypeOutputFile, arrayGenotype, allelePerLocus, nPopSize[nYear], nLoci);
        }
        
        cout << '.'; // use dot to show iteration progress
        
    } // end of iteration loop

    // display message
    cout << endl;
    cout << "Simulation completed." << endl;   
    cout << endl;
    cout << "Preparing output file..." << endl;
    cout << endl;

    // write simulation settings to output file
    // call writeVPSSimSetting function
    writeVPSSimSetting(outputFile, degreeOverlap, reproSys, longevity, ageMaturation, nYear, nIteration);

    // write summary output file
    writeDiMLSummary(outputFile, nLoci, arrayInputLocus, nYear, nIteration, arrayRawOA, arrayRawEA, arrayRawHo, arrayRawHe, arrayRawF);

    // call closeFile function
        // close input file
        closeInputFile(genotypeInputFile);
        closeInputFile(popSizeInput);
        // close output file
        closeOutputFile(outputFile);
        // check boolOutputRawGenotype
        if (boolOutputRawGenotype == 1)
        {
            closeOutputFile(rawGenotypeOutputFile);
        }

} // end of simDiploidMultiLocusVPS function


// end of source code file
