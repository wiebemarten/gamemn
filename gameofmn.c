/* file:          gameofmn.c */
/* authors:       Wiebe Marten Wijnja (wiebemarten@gmx.com) */
/*                Tomas Karamazen (t.j.karamazen@student.rug.nl) */
/* date:          30th Oct 2014 */
/* version:       11*/

/* Version History:
 * 11. Increased code formatting.
 * 10. Efficiency increased, removing of double solutions.
 * 9. Changing from static lists to allocated lists. Now supports Game of M with n up to n=17 (though running out of memory at 15 on most PCs)
 * 8. Attempting to increase range of numbers to test.
 * 7. Documentation.
 * 6. Code cleaning.
 * 5. Advanced user-input functions.
 * 4. First version that can solve multiple different 'Game Of M with n' versions.
 * 3. Bugfixes to make ALL solutions actually appear.
 * 2. Speed improvements.
 * 1. First working version of Game of 24.
 */
#include  <stdio.h> 
#include  <stdlib.h> 

/* Remove the following line to confirm that the program will indeed check all  C(N-1) * 4^(N-1) * N! solutions. */
#define DONT_LIST_DUPLICATES 1

/* Maximum length of number list.
 * Total amount of possibilities is then C(N-1) * 4^(N-1) * N!
 * 
 * The maximum is set at 17, because that would mean 54014705746650885287281950720000 possibilities to check, taking days (if not weeks) on a normal PC.
 * Most current PC's will complain about not having enough memory when using  > 15, though.
 * */
#define MAX_NUMBERS_TO_ADD 18
#define MAX_STRLEN 128

/* Precomputed Catalan numbers.(up to the 18th number) Used to determine the amount of bracket possibilities there are for a given n.*/
unsigned long CATALAN[] = {1u, 1u, 2u, 5u, 14u, 42u, 132u, 429u, 1430u, 4862u, 16796u, 58786u, 
 208012u, 742900u, 2674440u, 9694845u, 35357670u, 129644790u, 477638700u};


unsigned long opVector=0;
long numSolutions = 0;  /* Keeps a running tally of the amount of solutions found. */
long numPossibilities = 0;  /* Keeps a running tally of the total amount of possibilities tried. Should in the end add up to C(N-1) * 4^(N-1) * N! for N = numbersToAdd */

/* The following values are user-defined on program startup. */
int programMode;
int totalNumber = 24;
int numbersToAdd = 4;

int operatorCombinations=0; /* calculated before testing as: 4^(numbersToAdd-1)*/


/* This struct keeps track of all possible tree-combinations(bracket-combinations) in the recursive function, by saving both the calculation result e.g. 8
 * and the visual written form. e.g. "(6 + 2)"
 * These solutions are recursively combined and at the top checked against the totalNumber. (and then output if they are correct).
 *  */
typedef struct{
    double* list;  /* Each of these values is one  possible output (as double) of a certain tree/bracket choice.*/
    char** visual;  /* And here is the corresponding visual string, the likes of:  "(((A+B)-C)*D)". Should, for normal < 1000 integer inputs not be longer than 64 chars, even when having 10 numbers.  */
    int length;
} solutionslist;


/* Mallocates space, but only if there still is space available.*/
void* safeMalloc(double size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    printf("Error: memory allocation failed. Out of memory?\n");
    exit(-1);
  }
  return ptr;
}


/* Swaps two array indices*/
void swap(int i, int j, double* arr) {
  double h = arr[i];
  arr[i] = arr[j];
  arr[j] = h;
}


/* Return the mathematical result by using the given operator on numbers A and B.
 * Uses the global opVector, shifted down by (optnum*2) to obtain a number between 0 - 3 that is unique to this operator position in the bracket tree.
 * */
double op(double A, double B, int optnum) {
	double result;
	switch ((opVector >>  ((optnum) * 2)) &3) {
		case 0:
			result =  A + B;
			break;
		case 1:
			result = A - B;
			break;
		case 2:
			result = A * B;
			break;
		case 3:
			result = A / B;
			break;
	}
	return result;
}


/* Return the char that corresponds with the given operator.
 * Uses the global opVector, shifted down by (optnum*2) to obtain a number between 0 - 3 that is unique to this operator position in the bracket tree.
 * */
char visOp(int optnum) {
	char c;
	switch ((opVector >>  ((optnum) * 2)) &3) {
		case 0:
			c = '+';
			break;
		case 1:
			c = '-';
			break;
		case 2:
			c = '*';
			break;
		case 3:
			c = '/';
			break;
	}
	return c;
}


/* Recursive function that explores all different bracket possibilities.
 * numlist: list of numbers. In the parent call this is the full number list. In each of the recursive calls, it is a strict subset of this list.
 * length: length of the list of numbers.
 * 
 * */
solutionslist bracketTree(double numlist[], int length, int opNumber) {
    solutionslist solutions;
    solutions.length = 0;
    solutions.list = safeMalloc(sizeof(double) * CATALAN[length]);
    solutions.visual = safeMalloc(sizeof(char*) * CATALAN[length]);
    
    if (length  <=  1) { /*Base Case: Only one number, only one solution */
        solutions.list[0] = numlist[0];
        solutions.visual[0] = safeMalloc(sizeof(char) * MAX_STRLEN);
        sprintf(solutions.visual[0], "%ld", (long)numlist[0]);
        solutions.length = 1;
        return solutions;
    }
    /* Recursive Case:
     * We want to test all possible subtrees
     * This means we try all possibilities to split the tree into two, making sure that each side has at least 1 child(a leaf).
     * */
    int i, j, k;
    solutionslist left;

    solutionslist right;
	
	#ifdef DONT_LIST_DUPLICATES
	int operator = (opVector >> (opNumber * 2)) &3;
	#endif
    
    for (i = 1;  i < length; i++) {
        left = bracketTree(numlist, i, opNumber+1); /* Get all options from left node. If there's only one option, get that single number. */
        right = bracketTree(&numlist[i], length-i, opNumber+i); /* Idem for right node. opNumber is here incremented by i because we want to skip all possible left-subtrees so each operator really is independent. */
        
        for (j=0; j < left.length; j++) {/* For each of the left, right option combinations, add their operator result to the solutions of this tree */
            for (k=0; k < right.length; k++) {
				
				#ifdef DONT_LIST_DUPLICATES
				if (operator%2 ||  left.list[j] <= right.list[k]) {/* When using + or *, only use solution when the left part is smaller or equal to the right part. This is to remove (nearly) half of the identical cases because + and * are commutative.*/
				#endif	
				
					solutions.list[solutions.length] = op(left.list[j], right.list[k], opNumber); /* Combine solutions numerically */
					solutions.visual[solutions.length] = safeMalloc(sizeof(char)*MAX_STRLEN);
					sprintf(solutions.visual[solutions.length], "(%s %c %s)", left.visual[j], visOp(opNumber), right.visual[k]); /* And visually */
					solutions.length++;
					
				#ifdef DONT_LIST_DUPLICATES
				}
				#endif
            }
        }
        
		for (j=0; j < left.length; j++) {
			free(left.visual[j]);
		}
		for (j=0; j < right.length; j++) {
			free(right.visual[j]);
		}
		
		free(left.list);
		free(left.visual);
		free(right.list);
		free(right.visual);
    }
    
   
    
    return solutions;
}


/* For a single given operand order and number list order, test all bracket combinations
 * and print all solutions that add up to the correct number.
 * */
void testCombination(double numlist[], int length) {
	int i;
	solutionslist solutions;
	solutions = bracketTree(numlist, length, 0); /* get all possible bracket combinations.*/
	
	for (i=0; i < solutions.length; i++) {/* And now, loop through all combinations and check if they managed to reach the number*/
		if (solutions.list[i] == totalNumber) {
			
			printf("%s\n", solutions.visual[i]);
			free(solutions.visual[i]);
			
			numSolutions++;
			if ((programMode&1) == 0) {/* If we should stop after the first solution because we use programMode 0 or 2, stop. */
				exit(EXIT_SUCCESS);
			}
		}
			
		numPossibilities++;
		if (numPossibilities%50000 == 0) {/* Print a running tally every 50000 iterations to show the user that the program did not freeze.*/
			printf("%8ld possibilities tried so far...\n", numPossibilities);
		}
	}
	free(solutions.list);
	free(solutions.visual);
}

/* Test all different permutations of the numbers set a[]. 
 * Implementation of the Quick Perm algorithm of Philip Paul Fuchs (see http://www.quickperm.org/index.php )
 * */
void quickPerm(double a[]) {
	int p[MAX_NUMBERS_TO_ADD] = {0}; /* Array to keep track of permutations. */
	int i, j;
	
	testCombination(a, numbersToAdd); /* Try bracket options with the starting permutation. */
	
	 i = 1;
	while (i < numbersToAdd) {
		if (p[i] < i) {
			j = (i&1?p[i]:0); /* We swap with the first index if i is odd, or with the current value in o[i] if i is even. If we don't do this step, we do some combinations twice.(and others not at all) */
			
			swap(i, j, a); /*Swap the two values in the array to make a new permutation. */
			
			#ifdef DONT_LIST_DUPLICATES
			if (a[i] != a[j]) {/* If the numbers are duplicates, do not test the new permutation, as it results in the same output.*/
			#endif
				
				testCombination(a, numbersToAdd); /* Try all possible bracket options with this array permutation */
				
			#ifdef DONT_LIST_DUPLICATES
			}
			#endif
			p[i]++;
			i = 1;
		} else {
			p[i] = 0;
			i++;
		}
	}
}


/* A very simple recursive integer pow function.*/
int ipow(int base, int exp) {
	if (exp <= 0) {
		return 1;
	}
	return base * ipow(base, exp-1);
}


int main(int argc, char** argv) {
	int i;
	
	do {
		printf("What game do you want to use?\n");
		printf("0 - Game of 24: Stop after first solution is found.\n");
		printf("1 - Game of 24: Find all possible solutions.\n");
		printf("2 - Custom Game: Stop after first solution is found.\n");
		printf("3 - Custom Game: Find all possible solutions.\n");
		scanf("%d", &programMode);
	} while (programMode > 3 || programMode < 0);
	
	if (programMode > 1) {/* Custom game mode */
		printf("Fill in an integer number that will be the goal to archieve (Example: 24)\n");
		scanf("%d", &totalNumber);
		
		do {
			printf("How long is the list of numbers that should be used to find this goal? (Example: 4)\n");
			printf("The program can handle 1-%d numbers\n", MAX_NUMBERS_TO_ADD);
			scanf("%d", &numbersToAdd);
		} while (numbersToAdd > MAX_NUMBERS_TO_ADD ||  numbersToAdd < 1);
	}
	
	operatorCombinations = ipow(4, numbersToAdd-1); /*As for N numbers there are always N-1 binary operators, and our set of operators {+, -, *, /} has length 4, we have 4^(N-1) possible combinations. */
	
	printf("Input the %d numbers that will be used for the computation of the Game of %d.\n", numbersToAdd, totalNumber);
	double* nums = safeMalloc(numbersToAdd * sizeof(double));
	for (i = 0; i < numbersToAdd; i++) {
		scanf("%lf", &nums[i]);
	}
	
	/* For each possible operator combination, try all possible list permutations.*/
	for (opVector = 0; opVector < operatorCombinations; opVector++) {
		quickPerm(nums);
	}
	
	/* Print search statistics */
	printf("%ld possibilities tried.\n", numPossibilities);
	printf("Total Number of Solutions found:%ld\n", numSolutions);
	
	
	return 0;
}
