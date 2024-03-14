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

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	int mid = (r + l) / 2 + 1;
	
	// If there are only two items, sort them
	if (mid == l || mid == r) {
		if (pData[l] > pData[r]) {
			int temp = pData[l];
			pData[l] = pData[r];
			pData[r] = temp;
		}
		return;
	}
	else {
		mergeSort(pData, l, mid - 1);
		mergeSort(pData, mid, r);

		// Creates the two sub arrays for the left and right
		int* sub_arr1 = Alloc(sizeof(int*) * (mid - l));
		int* sub_arr2 = Alloc(sizeof(int*) * (r - mid));

		int i;
		int j;
		i = j = 0;

		// Populates the two arrays
		for (i; i < mid - l; i++) {
			sub_arr1[i] = pData[l + i];
		}

		for (j; j <= r - mid; j++) {
			sub_arr2[j] = pData[mid + j];
		}

		i = j = 0;
		int p = l;

		// While both arrays have items, use a section of the original array as a merge array
		// Using the section, if the item of the first subarray is less than the second, insert
		// it into the appropriate position in the section of the original, otherwise insert
		// the item from the second subarray
		while (i < mid - l && j <= r - mid) {
			if (sub_arr1[i] <= sub_arr2[j]) {
				pData[p] = sub_arr1[i];
				i++;
				p++;
			}
			else {
				pData[p] = sub_arr2[j];
				j++;
				p++;
			}
		}

		// If there is an array that still has items, simply add the remaining items
		while (i < mid - l) {
			pData[p] = sub_arr1[i];
			i++;
			p++;
		}

		while (j <= r - mid) {
			pData[p] = sub_arr2[j];
			j++;
			p++;
		}

		DeAlloc(sub_arr1);
		DeAlloc(sub_arr2);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i, sz = (dataSz > 100 ? dataSz - 100 : 0);
    int firstHundred = (dataSz < 100 ? dataSz : 100);
    printf("\tData:\n\t");
    for (i=0;i<firstHundred;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\t");
    
    for (i=sz;i<dataSz;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt"};
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
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
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}