//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Zone Memory Allocation. Neat.
//


#include "z_zone.h"
#include "i_system.h"
#include "doomtype.h"


//
// ZONE MEMORY ALLOCATION
//
// There is never any space between memblocks,
//  and there will never be two contiguous free memblocks.
// The rover can be left pointing at a non-empty block.
//
// It is of no value to free a cachable block,
//  because it will get overwritten automatically if needed.
// 
 
#define MEM_ALIGN sizeof(void *)
#define ZONEID	0x1d4a11

typedef struct memblock_s
{
    int			size;	// including the header and possibly tiny fragments
    void**		user;
    int			tag;	// PU_FREE if this is free
    int			id;	// should be ZONEID
    struct memblock_s*	next;
    struct memblock_s*	prev;
} memblock_t;


typedef struct
{
    // total bytes malloced, including header
    int		size;
    struct memzone_t *next;
    // start / end cap for linked list
    memblock_t	blocklist;
    
    memblock_t*	rover;
    
} memzone_t;



memzone_t*	mainzone = NULL;



//
// Z_ClearZone
//
void Z_ClearZone (memzone_t* zone)
{
    memblock_t*		block;
	
    // set the entire zone to one free block
    zone->blocklist.next =
	zone->blocklist.prev =
	block = (memblock_t *)( (byte *)zone + sizeof(memzone_t) );
    
    zone->blocklist.user = (void *)zone;
    zone->blocklist.tag = PU_STATIC;
    zone->rover = block;
	
    block->prev = block->next = &zone->blocklist;
    
    // a free block.
    block->tag = PU_FREE;

    block->size = zone->size - sizeof(memzone_t);
}


extern unsigned long _free_ram1_start, _free_ram1_end, _free_ram2_start, _free_ram2_end,
                    _free_ram3_start, _free_ram3_end, _free_ram4_start, _free_ram4_end;

static void Z_ZoneAdd(void *start, int size)
{
    memblock_t *block;
    memzone_t *zone = (memzone_t *)start;
    zone->size = size;
    zone->blocklist.next =
    zone->blocklist.prev =
    block = (memblock_t *)( (byte *)zone + sizeof(memzone_t));
    zone->blocklist.user = (void *)zone;
    zone->blocklist.tag = PU_STATIC;
    zone->rover = block;
    
    block->prev = block->next = &zone->blocklist;
    block->tag = PU_FREE;
    block->size = zone->size - sizeof(memzone_t);
    
    zone->next = mainzone;
    mainzone = zone;
}

//
// Z_Init
//
void Z_Init (void)
{
    Z_ZoneAdd(&_free_ram1_start, ((unsigned long)&_free_ram1_end) - ((unsigned long)&_free_ram1_start));
    Z_ZoneAdd(&_free_ram2_start, ((unsigned long)&_free_ram2_end) - ((unsigned long)&_free_ram2_start));
    Z_ZoneAdd(&_free_ram3_start, ((unsigned long)&_free_ram3_end) - ((unsigned long)&_free_ram3_start));
    Z_ZoneAdd(&_free_ram4_start, ((unsigned long)&_free_ram4_end) - ((unsigned long)&_free_ram4_start));
}

void Z_ZoneFree(memzone_t *zone, void *ptr)
{
    memblock_t*		block;
    memblock_t*		other;
	
    block = (memblock_t *) ( (byte *)ptr - sizeof(memblock_t));

    if (block->id != ZONEID)
	I_Error ("Z_Free: freed a pointer without ZONEID");
		
    if (block->tag != PU_FREE && block->user != NULL)
    {
    	// clear the user's mark
	    *block->user = 0;
    }

    // mark as free
    block->tag = PU_FREE;
    block->user = NULL;
    block->id = 0;
	
    other = block->prev;

    if (other->tag == PU_FREE)
    {
        // merge with previous free block
        other->size += block->size;
        other->next = block->next;
        other->next->prev = other;

        if (block == zone->rover)
            zone->rover = other;

        block = other;
    }
	
    other = block->next;
    if (other->tag == PU_FREE)
    {
        // merge the next free block onto the end
        block->size += other->size;
        block->next = other->next;
        block->next->prev = block;

        if (other == zone->rover)
            zone->rover = block;
    }
}

//
// Z_Free
//
void Z_Free (void* ptr)
{
    memzone_t *p = mainzone;
    while (p) {
        if ((unsigned long)ptr >= (unsigned long)p->blocklist.next &&
            (unsigned long)ptr < ((unsigned long)p->blocklist.next) + p->size) {
                Z_ZoneFree(p, ptr);
                return;
        }
        p = p->next;
    }
    I_Error("Z_Free: Invalid address 0x%x\n", ptr);
}



//
// Z_Malloc
// You can pass a NULL user if the tag is < PU_PURGELEVEL.
//
#define MINFRAGMENT		64


void *Z_ZoneMalloc(memzone_t *zone, int size, int tag, void *user)
{
    int		extra;
    memblock_t*	start;
    memblock_t* rover;
    memblock_t* newblock;
    memblock_t*	base;
    void *result;

    size = (size + MEM_ALIGN - 1) & ~(MEM_ALIGN - 1);
    
    // scan through the block list,
    // looking for the first free block
    // of sufficient size,
    // throwing out any purgable blocks along the way.

    // account for size of block header
    size += sizeof(memblock_t);
    
    // if there is a free block behind the rover,
    //  back up over them
    base = zone->rover;
    
    if (base->prev->tag == PU_FREE)
        base = base->prev;
	
    rover = base;
    start = base->prev;
	
    do
    {
        if (rover == start)
        {
            // scanned all the way around the list
            // I_Error ("Z_Malloc: failed on allocation of %i bytes", size);
            return NULL;
        }
	
        if (rover->tag != PU_FREE)
        {
            if (rover->tag < PU_PURGELEVEL)
            {
                // hit a block that can't be purged,
                // so move base past it
                base = rover = rover->next;
            }
            else
            {
                // free the rover block (adding the size to base)

                // the rover can be the base block
                base = base->prev;
                Z_Free ((byte *)rover+sizeof(memblock_t));
                base = base->next;
                rover = base->next;
            }
        }
        else
        {
            rover = rover->next;
        }

    } while (base->tag != PU_FREE || base->size < size);

    
    // found a block big enough
    extra = base->size - size;
    
    if (extra >  MINFRAGMENT)
    {
        // there will be a free fragment after the allocated block
        newblock = (memblock_t *) ((byte *)base + size );
        newblock->size = extra;
	
        newblock->tag = PU_FREE;
        newblock->user = NULL;	
        newblock->prev = base;
        newblock->next = base->next;
        newblock->next->prev = newblock;

        base->next = newblock;
        base->size = size;
    }
	
	if (user == NULL && tag >= PU_PURGELEVEL)
	    I_Error ("Z_Malloc: an owner is required for purgable blocks");

    base->user = user;
    base->tag = tag;

    result  = (void *) ((byte *)base + sizeof(memblock_t));

    if (base->user)
    {
        *base->user = result;
    }

    // next allocation will start looking here
    zone->rover = base->next;	
	
    base->id = ZONEID;
    
    return result;
}

void Z_ZoneFreeTags(memzone_t *zone, int lowtag, int hightag)
{
    memblock_t*	block;
    memblock_t*	next;
	
    for (block = zone->blocklist.next ;
	 block != &zone->blocklist ;
	 block = next)
    {
	// get link before freeing
	next = block->next;

	// free block?
	if (block->tag == PU_FREE)
	    continue;
	
	if (block->tag >= lowtag && block->tag <= hightag)
	    Z_Free ( (byte *)block+sizeof(memblock_t));
    }
}

void Z_ZoneDumpHeap(memzone_t *zone, int lowtag, int hightag)
{
    memblock_t*	block;
	
    printf ("zone size: %i  location: %p\n",
	    zone->size,zone);
    
    printf ("tag range: %i to %i\n",
	    lowtag, hightag);
	
    for (block = zone->blocklist.next ; ; block = block->next)
    {
	if (block->tag >= lowtag && block->tag <= hightag)
	    printf ("block:%p    size:%i    user:%p    tag:%i\n",
		    block, block->size, block->user, block->tag);
		
	if (block->next == &zone->blocklist)
	{
	    // all blocks have been hit
	    break;
	}
	
	if ( (byte *)block + block->size != (byte *)block->next)
	    printf ("ERROR: block size does not touch the next block\n");

	if ( block->next->prev != block)
	    printf ("ERROR: next block doesn't have proper back link\n");

	if (block->tag == PU_FREE && block->next->tag == PU_FREE)
	    printf ("ERROR: two consecutive free blocks\n");
    }
}

void Z_ZoneCheckHeap(memzone_t *zone)
{
    memblock_t*	block;
	
    for (block = zone->blocklist.next ; ; block = block->next)
    {
	if (block->next == &zone->blocklist)
	{
	    // all blocks have been hit
	    break;
	}
	
	if ( (byte *)block + block->size != (byte *)block->next)
	    I_Error ("Z_CheckHeap: block size does not touch the next block\n");

	if ( block->next->prev != block)
	    I_Error ("Z_CheckHeap: next block doesn't have proper back link\n");

	if (block->tag == PU_FREE && block->next->tag == PU_FREE)
	    I_Error ("Z_CheckHeap: two consecutive free blocks\n");
    }
}

int Z_ZoneFreeMemory (memzone_t *zone)
{
    memblock_t*		block;
    int			free;
	
    free = 0;
    
    for (block = zone->blocklist.next ;
         block != &zone->blocklist;
         block = block->next)
    {
        if (block->tag == PU_FREE || block->tag >= PU_PURGELEVEL)
            free += block->size;
    }

    return free;
}


void*
Z_Malloc
( int		size,
  int		tag,
  void*		user )
{
    memzone_t *p = mainzone;
    while (p) {
        void *ptr = Z_ZoneMalloc(p, size, tag, user);
        if (ptr)
            return ptr;
        p = p->next;
    }
    Z_DumpHeap(0, PU_NUM_TAGS);
    I_Error ("Z_Malloc: failed on allocation of %d bytes", size);
    while (1);
}

//
// Z_FreeTags
//
void
Z_FreeTags
( int		lowtag,
  int		hightag )
{
    memzone_t *p = mainzone;
    while (p) {
        Z_ZoneFreeTags(p, lowtag, hightag);
        p = p->next;
    }
}



//
// Z_DumpHeap
// Note: TFileDumpHeap( stdout ) ?
//
void
Z_DumpHeap
( int		lowtag,
  int		hightag )
{
    memzone_t *p = mainzone;
    while (p) {
        Z_ZoneDumpHeap(p, lowtag, hightag);
        p = p->next;
    }
}


//
// Z_CheckHeap
//
void Z_CheckHeap (void)
{
    memzone_t *p = mainzone;
    while (p) {
        Z_ZoneCheckHeap(p);
        p = p->next;
    }   
}




//
// Z_ChangeTag
//
void Z_ChangeTag2(void *ptr, int tag, char *file, int line)
{
    memblock_t*	block;
	
    block = (memblock_t *) ((byte *)ptr - sizeof(memblock_t));

    if (block->id != ZONEID)
        I_Error("%s:%i: Z_ChangeTag: block without a ZONEID!",
                file, line);

    if (tag >= PU_PURGELEVEL && block->user == NULL)
        I_Error("%s:%i: Z_ChangeTag: an owner is required "
                "for purgable blocks", file, line);

    block->tag = tag;
}

void Z_ChangeUser(void *ptr, void **user)
{
    memblock_t*	block;

    block = (memblock_t *) ((byte *)ptr - sizeof(memblock_t));

    if (block->id != ZONEID)
    {
        I_Error("Z_ChangeUser: Tried to change user for invalid block!");
    }

    block->user = user;
    *user = ptr;
}



//
// Z_FreeMemory
//
int Z_FreeMemory (void)
{
    memzone_t *p = mainzone;
    int x = 0;
    while (p) {
        x += Z_ZoneFreeMemory(p);
        p = p->next;
    }
    return x;
}
