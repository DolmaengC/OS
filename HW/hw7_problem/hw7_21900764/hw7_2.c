/*
	This program creates or load a page table and uses it to translate logical addresses into physical addressing.
	Complete the program by following the instructions.
	Try to make the output as similar to the following examples as possible.


	Example) ./a.out 3 2 16
		pagenum_bits = 3, offset_bits = 2, process_size = 16
		=== Page Table:			// the output of PageTable_Display()
		pagenum_bits = 3
		offset_bits = 2
		process_size = 16
		offset_mask = 0x0003
		no_page = 4
		frame[0] = 6
		frame[1] = 2
		frame[2] = 4
		frame[3] = 1
		=== PageTable Test:		// the output of PageTable_Test()
		[DEBUG] logical = 0x0000, page_no = 0x0000, offset = 0x0000, frame_no = 0x0006, physical = 0x0018
		logical 000000 (0x0000) ==> physical 000024 (0x0018)
		[DEBUG] logical = 0x0001, page_no = 0x0000, offset = 0x0001, frame_no = 0x0006, physical = 0x0019
		logical 000001 (0x0001) ==> physical 000025 (0x0019)
		[DEBUG] logical = 0x0002, page_no = 0x0000, offset = 0x0002, frame_no = 0x0006, physical = 0x001a
		logical 000002 (0x0002) ==> physical 000026 (0x001a)
		[DEBUG] logical = 0x0003, page_no = 0x0000, offset = 0x0003, frame_no = 0x0006, physical = 0x001b
		logical 000003 (0x0003) ==> physical 000027 (0x001b)
		[DEBUG] logical = 0x0004, page_no = 0x0001, offset = 0x0000, frame_no = 0x0002, physical = 0x0008
		logical 000004 (0x0004) ==> physical 000008 (0x0008)
		[DEBUG] logical = 0x0005, page_no = 0x0001, offset = 0x0001, frame_no = 0x0002, physical = 0x0009
		logical 000005 (0x0005) ==> physical 000009 (0x0009)
		[DEBUG] logical = 0x0006, page_no = 0x0001, offset = 0x0002, frame_no = 0x0002, physical = 0x000a
		logical 000006 (0x0006) ==> physical 000010 (0x000a)
		[DEBUG] logical = 0x0007, page_no = 0x0001, offset = 0x0003, frame_no = 0x0002, physical = 0x000b
		logical 000007 (0x0007) ==> physical 000011 (0x000b)
		[DEBUG] logical = 0x0008, page_no = 0x0002, offset = 0x0000, frame_no = 0x0004, physical = 0x0010
		logical 000008 (0x0008) ==> physical 000016 (0x0010)
		[DEBUG] logical = 0x0009, page_no = 0x0002, offset = 0x0001, frame_no = 0x0004, physical = 0x0011
		logical 000009 (0x0009) ==> physical 000017 (0x0011)
		[DEBUG] logical = 0x000a, page_no = 0x0002, offset = 0x0002, frame_no = 0x0004, physical = 0x0012
		logical 000010 (0x000a) ==> physical 000018 (0x0012)
		[DEBUG] logical = 0x000b, page_no = 0x0002, offset = 0x0003, frame_no = 0x0004, physical = 0x0013
		logical 000011 (0x000b) ==> physical 000019 (0x0013)
		[DEBUG] logical = 0x000c, page_no = 0x0003, offset = 0x0000, frame_no = 0x0001, physical = 0x0004
		logical 000012 (0x000c) ==> physical 000004 (0x0004)
		[DEBUG] logical = 0x000d, page_no = 0x0003, offset = 0x0001, frame_no = 0x0001, physical = 0x0005
		logical 000013 (0x000d) ==> physical 000005 (0x0005)
		[DEBUG] logical = 0x000e, page_no = 0x0003, offset = 0x0002, frame_no = 0x0001, physical = 0x0006
		logical 000014 (0x000e) ==> physical 000006 (0x0006)
		[DEBUG] logical = 0x000f, page_no = 0x0003, offset = 0x0003, frame_no = 0x0001, physical = 0x0007
		logical 000015 (0x000f) ==> physical 000007 (0x0007)

	Example) ./a.out 2 3 24
		pagenum_bits = 2, offset_bits = 3, process_size = 24
		=== Page Table:
		pagenum_bits = 2
		offset_bits = 3
		process_size = 24
		offset_mask = 0x0007
		no_page = 3
		frame[0] = 2
		frame[1] = 0
		frame[2] = 1
		=== PageTable Test:
		[DEBUG] logical = 0x0000, page_no = 0x0000, offset = 0x0000, frame_no = 0x0002, physical = 0x0010
		logical 000000 (0x0000) ==> physical 000016 (0x0010)
		[DEBUG] logical = 0x0001, page_no = 0x0000, offset = 0x0001, frame_no = 0x0002, physical = 0x0011
		logical 000001 (0x0001) ==> physical 000017 (0x0011)
		[DEBUG] logical = 0x0002, page_no = 0x0000, offset = 0x0002, frame_no = 0x0002, physical = 0x0012
		logical 000002 (0x0002) ==> physical 000018 (0x0012)
		[DEBUG] logical = 0x0003, page_no = 0x0000, offset = 0x0003, frame_no = 0x0002, physical = 0x0013
		logical 000003 (0x0003) ==> physical 000019 (0x0013)
		[DEBUG] logical = 0x0004, page_no = 0x0000, offset = 0x0004, frame_no = 0x0002, physical = 0x0014
		logical 000004 (0x0004) ==> physical 000020 (0x0014)
		[DEBUG] logical = 0x0005, page_no = 0x0000, offset = 0x0005, frame_no = 0x0002, physical = 0x0015
		logical 000005 (0x0005) ==> physical 000021 (0x0015)
		[DEBUG] logical = 0x0006, page_no = 0x0000, offset = 0x0006, frame_no = 0x0002, physical = 0x0016
		logical 000006 (0x0006) ==> physical 000022 (0x0016)
		[DEBUG] logical = 0x0007, page_no = 0x0000, offset = 0x0007, frame_no = 0x0002, physical = 0x0017
		logical 000007 (0x0007) ==> physical 000023 (0x0017)
		[DEBUG] logical = 0x0008, page_no = 0x0001, offset = 0x0000, frame_no = 0x0000, physical = 0x0000
		logical 000008 (0x0008) ==> physical 000000 (0x0000)
		[DEBUG] logical = 0x0009, page_no = 0x0001, offset = 0x0001, frame_no = 0x0000, physical = 0x0001
		logical 000009 (0x0009) ==> physical 000001 (0x0001)
		[DEBUG] logical = 0x000a, page_no = 0x0001, offset = 0x0002, frame_no = 0x0000, physical = 0x0002
		logical 000010 (0x000a) ==> physical 000002 (0x0002)
		[DEBUG] logical = 0x000b, page_no = 0x0001, offset = 0x0003, frame_no = 0x0000, physical = 0x0003
		logical 000011 (0x000b) ==> physical 000003 (0x0003)
		[DEBUG] logical = 0x000c, page_no = 0x0001, offset = 0x0004, frame_no = 0x0000, physical = 0x0004
		logical 000012 (0x000c) ==> physical 000004 (0x0004)
		[DEBUG] logical = 0x000d, page_no = 0x0001, offset = 0x0005, frame_no = 0x0000, physical = 0x0005
		logical 000013 (0x000d) ==> physical 000005 (0x0005)
		[DEBUG] logical = 0x000e, page_no = 0x0001, offset = 0x0006, frame_no = 0x0000, physical = 0x0006
		logical 000014 (0x000e) ==> physical 000006 (0x0006)
		[DEBUG] logical = 0x000f, page_no = 0x0001, offset = 0x0007, frame_no = 0x0000, physical = 0x0007
		logical 000015 (0x000f) ==> physical 000007 (0x0007)
		[DEBUG] logical = 0x0010, page_no = 0x0002, offset = 0x0000, frame_no = 0x0001, physical = 0x0008
		logical 000016 (0x0010) ==> physical 000008 (0x0008)
		[DEBUG] logical = 0x0011, page_no = 0x0002, offset = 0x0001, frame_no = 0x0001, physical = 0x0009
		logical 000017 (0x0011) ==> physical 000009 (0x0009)
		[DEBUG] logical = 0x0012, page_no = 0x0002, offset = 0x0002, frame_no = 0x0001, physical = 0x000a
		logical 000018 (0x0012) ==> physical 000010 (0x000a)
		[DEBUG] logical = 0x0013, page_no = 0x0002, offset = 0x0003, frame_no = 0x0001, physical = 0x000b
		logical 000019 (0x0013) ==> physical 000011 (0x000b)
		[DEBUG] logical = 0x0014, page_no = 0x0002, offset = 0x0004, frame_no = 0x0001, physical = 0x000c
		logical 000020 (0x0014) ==> physical 000012 (0x000c)
		[DEBUG] logical = 0x0015, page_no = 0x0002, offset = 0x0005, frame_no = 0x0001, physical = 0x000d
		logical 000021 (0x0015) ==> physical 000013 (0x000d)
		[DEBUG] logical = 0x0016, page_no = 0x0002, offset = 0x0006, frame_no = 0x0001, physical = 0x000e
		logical 000022 (0x0016) ==> physical 000014 (0x000e)
		[DEBUG] logical = 0x0017, page_no = 0x0002, offset = 0x0007, frame_no = 0x0001, physical = 0x000f
		logical 000023 (0x0017) ==> physical 000015 (0x000f)


	Example)./a.out 3 2 16 pagetable_example.txt			// load page table from 'pagetable_example.txt'
		pagenum_bits = 3, offset_bits = 2, process_size = 16
		Loading page table from file pagetable_example.txt
		=== Page Table:
		pagenum_bits = 3
		offset_bits = 2
		process_size = 16
		offset_mask = 0x0003
		no_page = 4
		frame[0] = 5
		frame[1] = 6
		frame[2] = 1
		frame[3] = 2
		=== PageTable Test:
		[DEBUG] logical = 0x0000, page_no = 0x0000, offset = 0x0000, frame_no = 0x0005, physical = 0x0014
		logical 000000 (0x0000) ==> physical 000020 (0x0014)
		[DEBUG] logical = 0x0001, page_no = 0x0000, offset = 0x0001, frame_no = 0x0005, physical = 0x0015
		logical 000001 (0x0001) ==> physical 000021 (0x0015)
		[DEBUG] logical = 0x0002, page_no = 0x0000, offset = 0x0002, frame_no = 0x0005, physical = 0x0016
		logical 000002 (0x0002) ==> physical 000022 (0x0016)
		[DEBUG] logical = 0x0003, page_no = 0x0000, offset = 0x0003, frame_no = 0x0005, physical = 0x0017
		logical 000003 (0x0003) ==> physical 000023 (0x0017)
		[DEBUG] logical = 0x0004, page_no = 0x0001, offset = 0x0000, frame_no = 0x0006, physical = 0x0018
		logical 000004 (0x0004) ==> physical 000024 (0x0018)
		[DEBUG] logical = 0x0005, page_no = 0x0001, offset = 0x0001, frame_no = 0x0006, physical = 0x0019
		logical 000005 (0x0005) ==> physical 000025 (0x0019)
		[DEBUG] logical = 0x0006, page_no = 0x0001, offset = 0x0002, frame_no = 0x0006, physical = 0x001a
		logical 000006 (0x0006) ==> physical 000026 (0x001a)
		[DEBUG] logical = 0x0007, page_no = 0x0001, offset = 0x0003, frame_no = 0x0006, physical = 0x001b
		logical 000007 (0x0007) ==> physical 000027 (0x001b)
		[DEBUG] logical = 0x0008, page_no = 0x0002, offset = 0x0000, frame_no = 0x0001, physical = 0x0004
		logical 000008 (0x0008) ==> physical 000004 (0x0004)
		[DEBUG] logical = 0x0009, page_no = 0x0002, offset = 0x0001, frame_no = 0x0001, physical = 0x0005
		logical 000009 (0x0009) ==> physical 000005 (0x0005)
		[DEBUG] logical = 0x000a, page_no = 0x0002, offset = 0x0002, frame_no = 0x0001, physical = 0x0006
		logical 000010 (0x000a) ==> physical 000006 (0x0006)
		[DEBUG] logical = 0x000b, page_no = 0x0002, offset = 0x0003, frame_no = 0x0001, physical = 0x0007
		logical 000011 (0x000b) ==> physical 000007 (0x0007)
		[DEBUG] logical = 0x000c, page_no = 0x0003, offset = 0x0000, frame_no = 0x0002, physical = 0x0008
		logical 000012 (0x000c) ==> physical 000008 (0x0008)
		[DEBUG] logical = 0x000d, page_no = 0x0003, offset = 0x0001, frame_no = 0x0002, physical = 0x0009
		logical 000013 (0x000d) ==> physical 000009 (0x0009)
		[DEBUG] logical = 0x000e, page_no = 0x0003, offset = 0x0002, frame_no = 0x0002, physical = 0x000a
		logical 000014 (0x000e) ==> physical 000010 (0x000a)
		[DEBUG] logical = 0x000f, page_no = 0x0003, offset = 0x0003, frame_no = 0x0002, physical = 0x000b
		logical 000015 (0x000f) ==> physical 000011 (0x000b)


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define	_DEBUG

#define TRUE 1
#define FALSE 0

typedef struct {
	int pagenum_bits;
	int offset_bits;
	int process_size;

	int page_size;
	int no_page;		// PTLR stores this value
	int offset_mask;

	int *frame;			// array of frame numbers (f = frame[p]), PTBR store this address
} PageTable;

void PageTable_Create(PageTable *pt, int pagenum_bits, int offset_bits, int process_size); // allocate a page table
void PageTable_Destroy(PageTable *pt);				// deallocate page table

int PageTable_Load(PageTable *pt, char *filename);	// load page table from a file
int PageTable_Save(PageTable *pt, char *filename);	// save page table from a file
int PageTable_Init(PageTable *pt, int no_frame);	// randomly initialize a page table

int PageTable_Display(PageTable *pt);				// display a page table

int PageTable_Translate(PageTable *pt, int logical); // translate a logical address into the corresponding physical address

void PageTable_Test(PageTable *pt, int trials);		// test page table

int MakeMask(int offset_bit);						// make a mask for offset bits
int Search(int *array, int size, int target);		// if target is not in array, return -1

int main(int argc, char *argv[])
{
	srand(time(NULL));

	if(argc < 4){
		printf("Usage: %s <pagenum_bits> <offset_bits> <process_size> [pagetable_file]\n", argv[0]);
		return 0;
	}

	int pagenum_bits = atoi(argv[1]);
	int offset_bits = atoi(argv[2]);
	int process_size = atoi(argv[3]);
	printf("pagenum_bits = %d, offset_bits = %d, process_size = %d\n", pagenum_bits, offset_bits, process_size);

	PageTable pt = { 0 };
	PageTable_Create(&pt, pagenum_bits, offset_bits, process_size);

	if(argc >= 5){
		char *filename = argv[4];
		printf("Loading page table from file %s\n", filename);
		PageTable_Load(&pt, filename);
	} else {
		int no_frame = 1 << pagenum_bits;
		PageTable_Init(&pt, no_frame);
		PageTable_Save(&pt, "pagetable.txt");
	}

	PageTable_Display(&pt);

	PageTable_Test(&pt, 100);

	PageTable_Destroy(&pt);

	return 0;
}

void PageTable_Create(PageTable *pt, int pagenum_bits, int offset_bits, int process_size)
// Allocate a page table
// TO DO: complete this function following instructions
{
	pt->pagenum_bits = pagenum_bits;
	pt->offset_bits = offset_bits;
	pt->process_size = process_size;

	// TO DO: set the page_size, no_page, and offset_mask from pagenum_bits, offset_bits, and process_size
	pt->page_size = (1 <<  offset_bits);
	pt->no_page =  (process_size + pt->page_size - 1) / pt->page_size;
	pt->offset_mask = MakeMask(offset_bits);


	// TO DO: dynamically allocate an array of integers to store the frame numbers and store its addres in pt->frame
	// 	on failure, display an error message and return.
	pt->frame = (int *)malloc(sizeof(int) * pt->no_page);
	if ( pt->frame  == NULL ) {
		perror("Failed malloc");
		return;
	}



	// TO DO: for safety, initialize the elements of pt->frame by filling it with zeroes
	for (int i = 0; i < pt->no_page; i++) {
		pt->frame[i] = 0;
	}
}

void PageTable_Destroy(PageTable *pt)
{
	// TO DO: deallocate pt->frame and set all fields with zeros
	free(pt->frame);
	memset(pt, 0, sizeof(PageTable));
}


int PageTable_Load(PageTable *pt, char *filename)
// load page table from a file
{
 	// TO DO: implement this function to load a page table from a file saved by PageTable_Save()
	//		on failure, display an error message and return FALSE
	//		use PageTable_Create() to allocate a page table
	
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Failed to open file %s\n", filename);
		return FALSE;
	}
	char strTemp[15];
	char *pstr;

	pstr = fgets(strTemp, 15, fp);
	int pagenum_bits = atoi(pstr);
	
	pstr = fgets(strTemp, 15, fp);
	int offset_bits = atoi(pstr);
	
	pstr = fgets(strTemp, 15, fp);
	int process_size = atoi(pstr);
	
	PageTable_Create(pt, pt->pagenum_bits, pt->offset_bits, pt->process_size);	

	pstr = fgets(strTemp, 15, fp);
	
	int count = 0;
	for (int i = 0; i < strlen(strTemp); i++) {
		if( strTemp[i] != ' ' ) {
			pt->frame[count] = atoi(&strTemp[i]);
			count++;
		}
	}	

	return TRUE;
}

int PageTable_Save(PageTable *pt, char *filename)
// save page table from a file
{
	FILE *fp = fopen(filename, "w");
	printf("Passed line %d\n", __LINE__);
	if(fp == NULL){
		printf("Failed to open file %s\n", filename);
		return FALSE;
	}

	fprintf(fp, "%d\n", pt->pagenum_bits);
	fprintf(fp, "%d\n", pt->offset_bits);
	fprintf(fp, "%d\n", pt->process_size);

	for(int p = 0; p < pt->no_page; p++)
		fprintf(fp, "%d ", pt->frame[p]);

	fclose(fp);

	return TRUE;
}

int PageTable_Display(PageTable *pt)
// save page table from a file
{
	printf("=== Page Table:\n");
	printf("pagenum_bits = %d\n", pt->pagenum_bits);
	printf("offset_bits = %d\n", pt->offset_bits);
	printf("process_size = %d\n", pt->process_size);

	printf("offset_mask = 0x%04x\n", pt->offset_mask);
	printf("no_page = %d\n", pt->no_page);

	for(int p = 0; p < pt->no_page; p++)
		printf("frame[%d] = %d\n", p, pt->frame[p]);

	return TRUE;
}

int PageTable_Init(PageTable *pt, int no_frame)
// randomly initialize a page table
{
	int f = 0;
	for(int p = 0; p < pt->no_page; p++){
		do {
			f = rand() % no_frame;	
		} while(Search(pt->frame, p, f) >= 0);
		
		pt->frame[p] = f;
	}
	
	return TRUE;
}

int PageTable_Translate(PageTable *pt, int logical)
// Translate a logical address into physical address
{
	int physical = 0;

	// TO DO: implement this function
	//		if the page number is not the valid range, display an error message and call exit(-1).
	int page_number = logical >> pt->offset_bits;
	int offset = pt->offset_mask & logical;
	int frame_number = pt->frame[page_number];
	physical = (frame_number << pt->offset_bits) | offset;




	// TO DO: for debug, display the logical address, page number, offset, frame number, and physical address
#ifdef _DEBUG
	printf("[DEBUG] logical = 0x%04x, page_no = 0x%04x, offset = 0x%04x, frame_no = 0x%04x, physical = 0x%04x\n",
			logical, page_number, offset, frame_number, physical);
#endif



	return physical;
}

void PageTable_Test(PageTable *pt, int trial)
// test page table
{
	printf("=== PageTable Test:\n");
	if(trial == 0)
		trial = 100;

	int limit = pt->no_page * pt->page_size;
	int step = limit / trial;
	if(step == 0)
		step++;

	for(int logical = 0; logical < limit; logical += step){
		int physical = PageTable_Translate(pt, logical);
		printf("logical %06d (0x%04x) ==> physical %06d (0x%04x)\n", logical, logical, physical, physical);
	}
}

int MakeMask(int offset_bit)
// returns offset_mask, an integer mask to retrieve offset from a logical address
//	e.g., if offset_bits = 2, offset_mask = 0......000011 = 0x0003
//		  if offset_bits = 5, offset_mask = 0......011111 = 0x001f
//		  if offset_bits = 9, offset_mask = 0..0111111111 = 0x01ff
{
	int mask = 0;

	// TO DO: complete this function
	for (int i = 0; i < offset_bit; i++) {
		mask |= (1 << i);
	}

	return mask;
}

int Search(int *array, int size, int target)
{
	for(int i = 0; i < size; i++)
		if(array[i] == target)
			return i;

	return -1;
}

