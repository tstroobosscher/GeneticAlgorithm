//
//  Catchphrase.cpp
//  
//
//  Created by Taylor  on 10/27/17.
//

#include <iostream>
#include <time.h>
#include <cmath>
#include <stdlib.h>

#define STRING_LENGTH 25//+1 for NULL
#define POPULATION_SIZE 4000
#define MUTATION_RATE .0001

char qTarget[STRING_LENGTH] = "Make America Great Again";

char createRandChar()
{
    //Rand ASCII excluding special Chars
    return(32 + rand()%95);
}

void printMember(int rStringLength, char* rString)
{
    for(int i = 0; i < rStringLength; i++)
    {
        std::cout << rString[i];
    }
    std::cout << std::endl;
}

void initializeRandString(int rStringLength, char* rStringPointer)
{
    for(int i = 0; i < rStringLength-1; i++)
    {
        rStringPointer[i] = createRandChar();
    }
    rStringPointer[rStringLength-1] = '\0';//NULL char
}

void createPopulation(int rStringLength, int rPopulationSize, char** rPopulation)
{
    for(int i = 0; i < rPopulationSize; i++)
    {
        rPopulation[i] = new char[rStringLength];
        initializeRandString(rStringLength, rPopulation[i]);
    }
}

void printLine(int rStringLength, int rIndex, char* rString, int* rFitness)
{
    std::cout << rIndex << " ";
    if(rIndex < 10)
    {
        std::cout << " ";
    }
    if(rIndex < 100)
    {
        std::cout << " ";
    }
    for(int i = 0; i < rStringLength; i++)
    {
        std::cout << rString[i];
    }
    std::cout << " "  << rFitness[rIndex] << "  " << std::endl;
}

int checkStringFitness(int rStringLength, char* rTargetString, char* rString)
{
    int score = 0;
    for(int i = 0; i < rStringLength; i++)
    {
        if(rTargetString[i] == rString[i])
        {
            score++;
        }
    }
    return(score);
}

void assignFitness(int rStringLength, int rPopulationSize, char* rTargetString, int* rFitness, char** rPopulation)
{
    for(int i = 0; i < rPopulationSize; i++)
    {
        rFitness[i] = pow(checkStringFitness(rStringLength, rTargetString, rPopulation[i]), 4);//quartic fitness
    }
}

void printPopulation(int rStringLength, int rPopulationSize, int* rFitness, char** rPopulation)
{
    for(int i = 0; i < rPopulationSize; i++)
    {
        printLine(rStringLength, i, rPopulation[i], rFitness);
    }
}

int calculateTotalFitness(int rPopulationSize, int* rFitness)
{
    int rTotal = 0;
    for(int i = 0; i < rPopulationSize; i++)
    {
        rTotal+=rFitness[i];
    }
    return(rTotal);
}

float calculateAverageFitness(int rPopulationSize, int* rFitness)
{
    float rPopulationFloat = rPopulationSize;
    float rAverageFitness = (calculateTotalFitness(rPopulationSize, rFitness))/rPopulationFloat;
    return(rAverageFitness);
}


int chooseEliteParent(int rPopulationSize, int* rFitness)
{
 /*
    choose random element
    choose random float between 0 and 1
    if float is less than fit/total, select
    else, continue
 */
    bool success = false;
    int rRandomElement;

    float rTotalFitness = calculateTotalFitness(rPopulationSize, rFitness); //NULL chars

    while(success == false)
    {
        rRandomElement = rand()%(rPopulationSize);
        float rFloat = rFitness[rRandomElement];//NULL char
        float rProbability = rFloat/rTotalFitness;
        float rRandomProbability = rand()%10000000/10000000.;
        //std::cout << rTotalFitness << " " << rProbability << " " << rRandomProbability << " ";
        if(rRandomProbability < rProbability)
        {
            success = true;
        }
    }
    return(rRandomElement);
}

int chooseRandParent(int rPopulationSize)
{
    return(rand()%rPopulationSize);
}

void createChild(int rStringLength, char* rParentA, char* rParentB, char* rChild)
{
    float rCrossoverRate;
    for(int i = 0; i < rStringLength; i++)
    {
        rCrossoverRate = rand()%100000/100000.;
        if(rCrossoverRate < .5)
        {
            rChild[i] = rParentA[i];
        }
        else
        {
            rChild[i] = rParentB[i];
        }
    }
    
    for(int i = 0; i < rStringLength-1; i++)
    {
        float rMutationRate = rand()%10000/10000.;
        if(rMutationRate < MUTATION_RATE)
        {
            rChild[i] = 32 + rand()%95;
        }
    }
}

void createNewGeneration(int rStringLength, int rPopulationSize, int* rFitness, char** rPopulation)
{
    /*
     for(int i = 0; i < pop; i++)
     pick parent a
     pick parent b
     crossover child
     mutate child
     */
    char** newPopulation = new char*[rPopulationSize];
    for(int i = 0; i < rPopulationSize; i++)
    {
        newPopulation[i] = rPopulation[i];
        //std::cout << newPopulation[i];
    }
    //std::cout << newPopulation[0];
    for(int i = 0; i < rPopulationSize; i++)
    {
        int rParentA = chooseEliteParent(rPopulationSize, rFitness);
        int rParentB = chooseEliteParent(rPopulationSize, rFitness);
        createChild(rStringLength, newPopulation[rParentA], newPopulation[rParentB], rPopulation[i]);
        //printMember(rStringLength, rPopulation[i]);
    }
    delete newPopulation[rPopulationSize];
}

int findMaxFitness(int rPopulationSize, int* rFitness)
{
    int rMaxFitness = 0;
    for(int i = 0; i < rPopulationSize; i++)
    {
        if(rFitness[i] > rMaxFitness)
        {
            rMaxFitness = rFitness[i];
        }
    }
    return(rMaxFitness);
}

int findBestMember(int rPopulationSize, int* rFitness)
{
    int rMaxFitness = 0;
    int rMaxIndex = 0;
    for(int i = 0; i < rPopulationSize; i++)
    {
        if(rFitness[i] > rMaxFitness)
        {
            rMaxFitness = rFitness[i];
            rMaxIndex = i;
        }
    }
    return(rMaxIndex);
}

int main()
{
    srand(time(NULL));

    char* qPopulation[POPULATION_SIZE];
    int qFitness[POPULATION_SIZE];
    
    createPopulation(STRING_LENGTH, POPULATION_SIZE, qPopulation);//Calls new operator to initialize generation 0
    assignFitness(STRING_LENGTH, POPULATION_SIZE, qTarget, qFitness, qPopulation);
    printPopulation(STRING_LENGTH, POPULATION_SIZE, qFitness, qPopulation);
    std::cout << calculateTotalFitness(POPULATION_SIZE, qFitness) << std::endl;

    int qMaxFitness = 0;
    int qGeneration = 0;
    //std::cout << calculateAverageFitness(POPULATION_SIZE,fitnessPopulation) << std::endl;
    
    while(qMaxFitness < pow(STRING_LENGTH, 4))
    {
        createNewGeneration(STRING_LENGTH, POPULATION_SIZE, qFitness, qPopulation);
        assignFitness(STRING_LENGTH, POPULATION_SIZE, qTarget, qFitness, qPopulation);
        //std::cout << findMaxFitness(POPULATION_SIZE, fitnessPopulation) << std::endl;
        qMaxFitness = findMaxFitness(POPULATION_SIZE, qFitness);

        //std::cout << std::endl << qMaxFitness << " ";
        
        printMember(STRING_LENGTH, qPopulation[findBestMember(POPULATION_SIZE, qFitness)]);
        qGeneration++;
    }
    //printPopulation(STRING_LENGTH, POPULATION_SIZE, fitnessPopulation, testPopulation);
    
    std::cout << "Generations: " << qGeneration << std::endl;

	return(0);
}

//fuck ucla