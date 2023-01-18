#include "UEFI_boot_param_info.h"
#include "memory.h"
#include "print.h"
#include "lib.h"

//// each zone index

int ZONE_DMA_INDEX	= 0;
int ZONE_NORMAL_INDEX	= 0;	//low 1GB RAM ,was mapped in pagetable
int ZONE_UNMAPED_INDEX	= 0;	//above 1GB RAM,unmapped in pagetable

unsigned long * Global_CR3 = NULL;

struct Global_Memory_Descriptor memory_management_struct = {{0},0};

unsigned long page_init(struct Page * page,unsigned long flags)
{
	if(!page->attribute)
	{
		*(memory_management_struct.bits_map + ((page->PHY_address >> PAGE_2M_SHIFT) >> 6)) |= 1UL << (page->PHY_address >> PAGE_2M_SHIFT) % 64;
		page->attribute = flags;
		page->reference_count++;
		page->zone_struct->page_using_count++;
		page->zone_struct->page_free_count--;
		page->zone_struct->total_pages_link++;
	}
	else if((page->attribute & PG_Referenced) || (page->attribute & PG_K_Share_To_U) || (flags & PG_Referenced) || (flags & PG_K_Share_To_U))
	{
		page->attribute |= flags;
		page->reference_count++;
		page->zone_struct->total_pages_link++;
	}
	else
	{
		*(memory_management_struct.bits_map + ((page->PHY_address >> PAGE_2M_SHIFT) >> 6)) |= 1UL << (page->PHY_address >> PAGE_2M_SHIFT) % 64;	
		page->attribute |= flags;
	}
	return 0;
}


unsigned long page_clean(struct Page * page)
{
	if(!page->attribute)
	{
		page->attribute = 0;
	}
	else if((page->attribute & PG_Referenced) || (page->attribute & PG_K_Share_To_U))
	{		
		page->reference_count--;
		page->zone_struct->total_pages_link--;
		if(!page->reference_count)
		{
			page->attribute = 0;
			page->zone_struct->page_using_count--;
			page->zone_struct->page_free_count++;
		}
	}
	else
	{
		*(memory_management_struct.bits_map + ((page->PHY_address >> PAGE_2M_SHIFT) >> 6)) &= ~(1UL << (page->PHY_address >> PAGE_2M_SHIFT) % 64);

		page->attribute = 0;
		page->reference_count = 0;
		page->zone_struct->page_using_count--;
		page->zone_struct->page_free_count++;
		page->zone_struct->total_pages_link--;
	}
	return 0;
}


void init_memory()
{
	int i,j;
	unsigned long TotalMem = 0 ;

	/*
	Use the macro function PAGE_2M_ALIGN to align the start addresses of these segments with the upper boundary of the 2 MB page,  
	 and the address after alignment is the effective memory start address of the segment.The end address of these segments is obtained by adding the original start address of the segmentand the segmentlength, 
	 and then the calculation result is aligned to the lower boundary of the 2 MB page by shifting
	*/

	TotalMem = 0;

	for(i = 0;i <= memory_management_struct.e820_length;i++)
	{
		unsigned long start,end;
		if(memory_management_struct.e820[i].type != 1)
			continue;
		start = PAGE_2M_ALIGN(memory_management_struct.e820[i].address);
		end   = ((memory_management_struct.e820[i].address + memory_management_struct.e820[i].length) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
		if(end <= start)
			continue;
		TotalMem += (end - start) >> PAGE_2M_SHIFT;
	}

	TotalMem = memory_management_struct.e820[memory_management_struct.e820_length].address + memory_management_struct.e820[memory_management_struct.e820_length].length;

	//bits map construction init

	memory_management_struct.bits_map = (unsigned long *)((memory_management_struct.end_brk + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);

	memory_management_struct.bits_size = TotalMem >> PAGE_2M_SHIFT;

	memory_management_struct.bits_length = (((unsigned long)(TotalMem >> PAGE_2M_SHIFT) + sizeof(long) * 8 - 1) / 8) & ( ~ (sizeof(long) - 1));

	memset(memory_management_struct.bits_map,0xff,memory_management_struct.bits_length);		//init bits map memory

	/*
	pages construction init
	This part of the program is responsible for creating the storage space and allocation records of the struct page structure array. The storage space of the struct page structure array is located behind the bit mapping bitmap. 
	The number of elements in the array is the number of pages that can be divided into physical address spaces. Its allocation and calculation methods are similar to the bit mapping bitmap, except that 
	the struct page structure array is here All cleared for use in subsequent initialization routines
	*/
	memory_management_struct.pages_struct = (struct Page *)(((unsigned long)memory_management_struct.bits_map + memory_management_struct.bits_length + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);

	memory_management_struct.pages_size = TotalMem >> PAGE_2M_SHIFT;

	memory_management_struct.pages_length = ((TotalMem >> PAGE_2M_SHIFT) * sizeof(struct Page) + sizeof(long) - 1) & ( ~ (sizeof(long) - 1));

	memset(memory_management_struct.pages_struct,0x00,memory_management_struct.pages_length);	//init pages memory

	/*zones construction init
	At present, it is impossible to calculate the number of elements in the struct zone structure array.
	We can only assign a value of 0 to the zones_size member variable, and temporarily calculate the zones_length member variable according to 5 struct zone structures.
	*/

	memory_management_struct.zones_struct = (struct Zone *)(((unsigned long)memory_management_struct.pages_struct + memory_management_struct.pages_length + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);

	memory_management_struct.zones_size   = 0;

	memory_management_struct.zones_length = (5 * sizeof(struct Zone) + sizeof(long) - 1) & (~(sizeof(long) - 1));

	memset(memory_management_struct.zones_struct,0x00,memory_management_struct.zones_length);	//init zones memory

	/*
	This program is the core code for initializing the bitmap map, struct page structure, and struct zone structure. 
	It will traverse all physical memory segment information to initialize available physical memory segments. 
	The code first filters out non-physical memory segments, and then performs page alignment on the remaining available physical memory segments.
	If there are available physical pages in this segment of physical memory, this segment of memory space is regarded as an available struct zone area space, and the It is initialized.
	*/

	for(i = 0;i <= memory_management_struct.e820_length;i++)
	{
		unsigned long start,end;
		struct Zone * z;
		struct Page * p;
		unsigned long * b;

		if(memory_management_struct.e820[i].type != 1)
			continue;
		start = PAGE_2M_ALIGN(memory_management_struct.e820[i].address);
		end   = ((memory_management_struct.e820[i].address + memory_management_struct.e820[i].length) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
		if(end <= start)
			continue;
		
		//zone init

		z = memory_management_struct.zones_struct + memory_management_struct.zones_size;
		memory_management_struct.zones_size++;

		z->zone_start_address = start;
		z->zone_end_address = end;
		z->zone_length = end - start;

		z->page_using_count = 0;
		z->page_free_count = (end - start) >> PAGE_2M_SHIFT;

		z->total_pages_link = 0;

		z->attribute = 0;
		z->GMD_struct = &memory_management_struct;

		z->pages_length = (end - start) >> PAGE_2M_SHIFT;
		z->pages_group =  (struct Page *)(memory_management_struct.pages_struct + (start >> PAGE_2M_SHIFT));

		//page init
		p = z->pages_group;
		for(j = 0;j < z->pages_length; j++ , p++)
		{
			p->zone_struct = z;
			p->PHY_address = start + PAGE_2M_SIZE * j;
			p->attribute = 0;

			p->reference_count = 0;

			p->age = 0;

			*(memory_management_struct.bits_map + ((p->PHY_address >> PAGE_2M_SHIFT) >> 6)) ^= 1UL << (p->PHY_address >> PAGE_2M_SHIFT) % 64;

		}
		
	}

	/*init address 0 to page struct 0; because the memory_management_struct.e820[0].type != 1
	After some traversal, all available physical memory pages have been initialized, but since the 0~2 MB physical memory pages contain multiple physical memory segments, 
	including kernel programs, special initialization must be performed on this page. After that, the number of elements in the spatial structure array of the struct zone area can be calculated.
	*/
	memory_management_struct.pages_struct->zone_struct = memory_management_struct.zones_struct;

	memory_management_struct.pages_struct->PHY_address = 0UL;
	memory_management_struct.pages_struct->attribute = 0;
	memory_management_struct.pages_struct->reference_count = 0;
	memory_management_struct.pages_struct->age = 0;



	memory_management_struct.zones_length = (memory_management_struct.zones_size * sizeof(struct Zone) + sizeof(long) - 1) & ( ~ (sizeof(long) - 1));

	ZONE_DMA_INDEX = 0;	//need rewrite in the future
	ZONE_NORMAL_INDEX = 0;	//need rewrite in the future

	/*
	This section of the program traverses and displays the detailed statistical information of the struct zone of each area space structure. 
	If the starting address of the current area is 0x100000000, the index value of this area is recorded in the global variable ZONE_UNMAPED_INDEX, indicating the physical memory page starting from the area space No page table mapping has been done.
	Finally, adjust the value of the member variable end_of_struct to record the end address of the above structure, and reserve a section of memory space to prevent out-of-bounds access.
	*/

	for(i = 0;i < memory_management_struct.zones_size;i++)	//need rewrite in the future
	{
		struct Zone * z = memory_management_struct.zones_struct + i;

		if(z->zone_start_address == 0x100000000)
			ZONE_UNMAPED_INDEX = i;
	}
	
	memory_management_struct.end_of_struct = (unsigned long)((unsigned long)memory_management_struct.zones_struct + memory_management_struct.zones_length + sizeof(long) * 32) & ( ~ (sizeof(long) - 1));	////need a blank to separate memory_management_struct

	i = Virt_To_Phy(memory_management_struct.end_of_struct) >> PAGE_2M_SHIFT;

	for(j = 0;j <= i;j++)
	{
		page_init(memory_management_struct.pages_struct + j,PG_PTable_Maped | PG_Kernel_Init | PG_Active | PG_Kernel);
	}
	print("complete£¡");
	flush_tlb();
}


/*
	number: number <= 64
	zone_select: zone select from dma , mapped in  pagetable , unmapped in pagetable
	page_flags: struct Page flages
*/

struct Page * alloc_pages(int zone_select,int number,unsigned long page_flags)
{
	int i;
	unsigned long page = 0;

	int zone_start = 0;
	int zone_end = 0;

	switch(zone_select)
	{
		case ZONE_DMA:
				zone_start = 0;
				zone_end = ZONE_DMA_INDEX;

			break;

		case ZONE_NORMAL:
				zone_start = ZONE_DMA_INDEX;
				zone_end = ZONE_NORMAL_INDEX;

			break;

		case ZONE_UNMAPED:
				zone_start = ZONE_UNMAPED_INDEX;
				zone_end = memory_management_struct.zones_size - 1;

			break;

		default:
			print("alloc_pages error zone_select index\n");
			return NULL;
			break;
	}

	for(i = zone_start;i <= zone_end; i++)
	{
		struct Zone * z;
		unsigned long j;
		unsigned long start,end,length;
		unsigned long tmp;

		if((memory_management_struct.zones_struct + i)->page_free_count < number)
			continue;

		z = memory_management_struct.zones_struct + i;
		start = z->zone_start_address >> PAGE_2M_SHIFT;
		end = z->zone_end_address >> PAGE_2M_SHIFT;
		length = z->zone_length >> PAGE_2M_SHIFT;

		tmp = 64 - start % 64;

		for(j = start;j <= end;j += j % 64 ? tmp : 64)
		{
			unsigned long * p = memory_management_struct.bits_map + (j >> 6);
			unsigned long shift = j % 64;
			unsigned long k;
			for(k = shift;k < 64 - shift;k++)
			{
				if( !(((*p >> k) | (*(p + 1) << (64 - k))) & (number == 64 ? 0xffffffffffffffffUL : ((1UL << number) - 1))) )
				{
					unsigned long	l;
					page = j + k - 1;
					for(l = 0;l < number;l++)
					{
						struct Page * x = memory_management_struct.pages_struct + page + l;
						page_init(x,page_flags);
					}
					goto find_free_pages;
				}
			}
		
		}
	}

	return NULL;

find_free_pages:

	return (struct Page *)(memory_management_struct.pages_struct + page);
}

