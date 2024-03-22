#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}



void heapify(int arr[], int n, int i) 
{ 
    int temp, maximum, leftIndex, rightIndex; 
  
    // "i" is the Largest Value 
    maximum = i;
	
    rightIndex = 2 * i + 2; 
	leftIndex = 2 * i + 1; 
  
    // If Left Greater than Current Index Value
    if (leftIndex < n && arr[leftIndex] > arr[maximum]) 
        maximum = leftIndex; 
  
    // If Left Greater than Current Index Value
    if (rightIndex < n && arr[rightIndex] > arr[maximum]) 
        maximum = rightIndex; 
  
    // Checking for Swapping 
    if (maximum != i) 
	{ 
        temp = arr[i]; 
        arr[i] = arr[maximum]; 
        arr[maximum] = temp; 
        heapify(arr, n, maximum); 
    } 
}



// Implements Heap Sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n) // Modified because it didn't match for function call!
{
	// heapSort and heapify are NOT Combined!

	int i, temp; 
  
    // Heapify Non-Leaf Nodes for "n/2 - 1" to 0 
    for (i = n / 2 - 1; i >= 0; i--) 
        heapify(arr, n, i);

    // Current Array is Changed to Max
    for (i = n; i > 0; i--) 
	{ 
        temp = arr[0]; 
        arr[0] = arr[i]; 
        arr[i] = temp; 
        heapify(arr, i, 0); 
    } 
}



void merge(int pData[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	// Create Temps
	int *leftTemp = (int*) Alloc(sizeof(int) * n1);
	int *rightTemp = (int*) Alloc(sizeof(int) * n2);

	// Copy Data to Temps
	for (i = 0; i < n1; i++)
		leftTemp[i] = pData[l + i];
	for (j = 0; j < n2; j++)
		rightTemp[j] = pData[m + 1 + j];

	// Merge Temps back into pData[l..r]
	i = 0; // Initial index of Left
	j = 0; // Initial index of Right
	k = l; // Initial Index of Merged Subarray

	while (i < n1 && j < n2)
	{
		if (leftTemp[i] <= rightTemp[j])
		{
			pData[k] = leftTemp[i];
			i++;
		}

		else
		{
			pData[k] = rightTemp[j];
			j++;
		}
	
		k++;
	}

	// Remaining Elements of Left
	while (i < n1)
	{
		pData[k] = leftTemp[i];
		i++;
		k++;
	}

	// Remaining Elements of Right
	while (j < n2)
	{
		pData[k] = rightTemp[j];
		j++;
		k++;
	}
	DeAlloc(leftTemp);
	DeAlloc(rightTemp);
}



// Implement Merge Sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	// mergeSort and merge are NOT Combined!


	if (l < r) // If Left is Less than Right
	{
		int m = (l + r) / 2;
		
		mergeSort(pData, l, m); // Looks Towards Left
		mergeSort(pData, m + 1, r); // Looks Towards Right

		merge(pData, l, m, r);
	}
}



// Implement Insertion Sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, item, j;
	for (i = 1; i < n; i++)
	{
		item = pData[i];

		// Move elements greater than key one position ahead
		for(j = (i - 1); j >= 0; j--)
		{
			if(pData[j] > item)
				pData[j + 1] = pData[j];
			else
				break;
		}

		pData[j + 1] = item;
	}		
}



// Implement Bubble Sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
    int* swap = (int*) Alloc(sizeof(int) * n);

    for(int x = 0; x < n; x++)
        swap[x] = pData[x];

    int i, j, temp;
    for (i = 1; i < n; i++)
        for (j = 0; j < n - i; j++)
        {
            if (swap[j] > swap[j + 1])
            {
                temp = swap[j];
                swap[j] = swap[j + 1];
                swap[j + 1] = temp;
            }
        }

    for(int x = 0; x < n; x++) // Set pData equal to Swapped Array
        pData[x] = swap[x];

	DeAlloc(swap);

}



// Implement Selection Sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
    int* swap = (int*) Alloc(sizeof(int) * n);
    for(int x = 0; x < n; x++)
        swap[x] = pData[x];

    int i, j, minindex, temp, temp2;

    for (i = 0; i < n - 1; i++)
    {
        // Find the Minimum Element
        minindex = i;

        for (j = i; j < n; j++)
        {
            if (swap[j] < swap[minindex])
                minindex = j;
        }

        if (i == minindex) // Checks to make sure Minimum and i aren't Equal
            continue;

        // Temps
        temp = swap[i];
        temp2 = swap[i];

        // Swap the Minimum with the 1st
        swap[i] = swap[minindex];
        swap[i] = swap[minindex];
        swap[minindex] = temp;
        swap[minindex] = temp2;
    }


    for(int x = 0; x < n; x++) // Set pData equal to Swapped Array
        pData[x] = swap[x];

    DeAlloc(swap);
	
}



// Parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n", &dataSz);
		*ppData = (int *) Alloc(sizeof(int) * dataSz);

		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			fscanf(inFile, "%d ", &n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);

	}
	
	return dataSz;
}



// Prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i = 0; i < 100; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\t");
	
	for (i = sz; i < dataSz; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i = 2; i < 3; ++i) // FIX LATER TO BE 0!!
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *) Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz - 1); // ADJUSTED SO IT MATCHES PARAMETER NUMBERS!!
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
