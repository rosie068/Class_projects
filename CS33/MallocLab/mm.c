/*
 * mm.c -  Simple allocator based on explicit lists,
 *         first fit placement, and boundary tag coalescing.
 *
 * Each block has header and footer of the form:
 *
 *      63       32   31        1   0
 *      --------------------------------
 *     |   unused   | block_size | a/f |
 *      --------------------------------
 *
 * a/f is 1 iff the block is allocated. The list has the following form:
 *
 * begin                                       end
 * heap                                       heap
 *  ----------------------------------------------
 * | hdr(8:a) | zero or more usr blks | hdr(0:a) |
 *  ----------------------------------------------
 * | prologue |                       | epilogue |
 * | block    |                       | block    |
 *
 * The allocated prologue and epilogue blocks are overhead that
 * eliminate edge conditions during coalescing.
 */
#include "memlib.h"
#include "mm.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Your info */
team_t team = {
    /* First and last name */
    "Rosemary He",
    /* UID */
    "*********",
    /* Custom message (16 chars) */
    "KikiEx",
};

typedef struct {
    uint32_t allocated : 1;
    uint32_t block_size : 31;
    uint32_t _;
} header_t;

typedef header_t footer_t;

typedef struct {
    uint32_t allocated : 1;
    uint32_t block_size : 31;
    uint32_t _;
    union {
        struct {
            struct block_t* next;
            struct block_t* prev;
        };
        int payload[0]; 
    } body;
} block_t;

/* This enum can be used to set the allocated bit in the block */
enum block_state { FREE,
                   ALLOC };

#define CHUNKSIZE (1 << 16) /* initial heap size (bytes) */
#define OVERHEAD (sizeof(header_t) + sizeof(footer_t)) /* overhead of the header and footer of an allocated block */
#define MIN_BLOCK_SIZE (32) /* the minimum block size needed to keep in a freelist (header + footer + next pointer + prev pointer) */
#define SIZE_BOUND (1 << 8) /* boundary to decide to put block in front or back */
#define REALLOC_SIZE (1 << 18) /* realloc size */

/* Global variables */
static block_t *prologue; /* pointer to first block */
static block_t *small_list_head; /* ptr to beginning of small size list */
//static block_t *big_list_head; /* ptr to beginning of big size list */

/* function prototypes for internal helper routines */
static block_t *extend_heap(size_t words);
static void place(block_t *block, size_t asize);
static block_t *find_fit(size_t asize);
static block_t *coalesce(block_t *block);
static footer_t *get_footer(block_t *block);
static void printblock(block_t *block);
static void checkblock(block_t *block);

static void add_block(block_t *block);
static void remove_block(block_t *block);
static void printlist();

/*
 * mm_init - Initialize the memory manager
 */
/* $begin mminit */
int mm_init(void) {
    small_list_head = NULL;
    /* create the initial empty heap */
    if ((prologue = mem_sbrk(CHUNKSIZE)) == (void*)-1)
        return -1;

    /* initialize the prologue */
    prologue->allocated = ALLOC;
    prologue->block_size = sizeof(header_t);

    /* initialize the first free block */
    block_t *init_block = (void *)prologue + sizeof(header_t);
    init_block->allocated = FREE;
    init_block->block_size = CHUNKSIZE - OVERHEAD;
    init_block->body.prev = NULL;
    init_block->body.next = NULL;

    footer_t *init_footer = get_footer(init_block);
    init_footer->allocated = FREE;
    init_footer->block_size = init_block->block_size;

    /* initialize the epilogue - block size 0 will be used as a terminating condition */
    block_t *epilogue = (void *)init_block + init_block->block_size;
    epilogue->allocated = ALLOC;
    epilogue->block_size = 0;

    add_block(init_block);
    //small_list_head = (void *)prologue + sizeof(header_t); //small list start at the beginning
    //big_list_head = (void *)epilogue - sizeof(header_t); //large list start at the end
    return 0;
}
/* $end mminit */

/*
 * mm_malloc - Allocate a block with at least size bytes of payload
 */
/* $begin mmmalloc */
void *mm_malloc(size_t size) {
    uint32_t asize;       /* adjusted block size */
    uint32_t extendsize;  /* amount to extend heap if no fit */
    uint32_t extendwords; /* number of words to extend heap if no fit */
    block_t *block;

    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    size += OVERHEAD;
    asize = ((size + 7) >> 3) << 3; /* align to multiple of 8 */
    
    if (asize < MIN_BLOCK_SIZE) {
        asize = MIN_BLOCK_SIZE;
    }

    /* Search the free list for a fit */
    if ((block = find_fit(asize)) != NULL) {
        place(block, asize);
        return block->body.payload;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = (asize > REALLOC_SIZE) // extend by the larger of the two
                     ? asize
                     : REALLOC_SIZE;
    extendwords = extendsize >> 3; // extendsize/8
    if ((block = extend_heap(extendwords)) != NULL) {
        place(block, asize);
        return block->body.payload;
    }
    /* no more memory :( */
    return NULL;
}
/* $end mmmalloc */

/*
 * mm_free - Free a block
 */
/* $begin mmfree */
void mm_free(void *payload) {
    block_t *block = payload - sizeof(header_t);
    block->allocated = FREE;
    block->body.next = NULL;
    block->body.prev = NULL;

    footer_t *footer = get_footer(block);
    footer->allocated = FREE;

    block = coalesce(block);
    add_block(block);
}

/* $end mmfree */

/*
 * mm_realloc - naive implementation of mm_realloc
 * NO NEED TO CHANGE THIS CODE!
 */
void *mm_realloc(void *ptr, size_t size) {
    void *newp;
    size_t copySize;

    if ((newp = mm_malloc(size)) == NULL) {
        printf("ERROR: mm_malloc failed in mm_realloc\n");
        exit(1);
    }
    block_t* block = ptr - sizeof(header_t);
    copySize = block->block_size;
    if (size < copySize)
        copySize = size;
    memcpy(newp, ptr, copySize);
    mm_free(ptr);
    return newp;
}

/*
 * mm_checkheap - Check the heap for consistency
 */
void mm_checkheap(int verbose) {
    block_t *block = prologue;

    if (verbose)
        printf("Heap (%p):\n", prologue);

    if (block->block_size != sizeof(header_t) || !block->allocated)
        printf("Bad prologue header\n");
    checkblock(prologue);

    /* iterate through the heap (both free and allocated blocks will be present) */
    for (block = (void*)prologue+prologue->block_size; block->block_size > 0; block = (void *)block + block->block_size) {
        if (verbose)
            printblock(block);
        checkblock(block);
    }

    if (verbose)
        printblock(block);
    if (block->block_size != 0 || !block->allocated)
        printf("Bad epilogue header\n");
}

/* The remaining routines are internal helper routines */

/*
 * extend_heap - Extend heap with free block and return its block pointer
 */
/* $begin mmextendheap */
static block_t *extend_heap(size_t words) {
    block_t *block;
    uint32_t size;
    size = words << 3; // words*8
    if (size == 0 || (block = mem_sbrk(size)) == (void *)-1)
        return NULL;

    /* The newly acquired region will start directly after the epilogue block */ 
    /* Initialize free block header/footer and the new epilogue header */
    /* use old epilogue as new free block header */
    block = (void *)block - sizeof(header_t);
    block->allocated = FREE;
    block->block_size = size;
    block->body.next = NULL;
    block->body.prev = NULL;
    /* free block footer */
    footer_t *block_footer = get_footer(block);
    block_footer->allocated = FREE;
    block_footer->block_size = block->block_size;
    /* new epilogue header */
    header_t *new_epilogue = (void *)block_footer + sizeof(header_t);
    new_epilogue->allocated = ALLOC;
    new_epilogue->block_size = 0;

    /* Coalesce if the previous block was free */
    block = coalesce(block);
    add_block(block);

    return block;
}
/* $end mmextendheap */

/*
 * place - Place block of asize bytes at start of free block block
 *         and split if remainder would be at least minimum block size
 */
/* $begin mmplace */
static void place(block_t *block, size_t asize) {
    size_t split_size = block->block_size - asize;

    if (split_size >= MIN_BLOCK_SIZE) {
        /* split the block by updating the header and marking it allocated*/
        block->block_size = asize;
        block->allocated = ALLOC;
        /* set footer of allocated block*/
        footer_t *footer = get_footer(block);
        footer->block_size = asize;
        footer->allocated = ALLOC;
        remove_block(block);

        /* update the header of the new free block */
        block_t *new_block = (void *)block + block->block_size;
        new_block->block_size = split_size;
        new_block->allocated = FREE;
        new_block->body.prev = NULL;
        new_block->body.next = NULL;

        /* update the footer of the new free block */
        footer_t *new_footer = get_footer(new_block);
        new_footer->block_size = split_size;
        new_footer->allocated = FREE;

        add_block(new_block);
    }
    else {
        /* splitting the block will cause a splinter so we just include it in the allocated block */
        /* just remove the block from the list */
        block->allocated = ALLOC;
        footer_t *footer = get_footer(block);
        footer->allocated = ALLOC;
        remove_block(block);
    }
}
/* $end mmplace */

/*
 * find_fit - Find a fit for a block with asize bytes
 */
static block_t *find_fit(size_t asize) {
    block_t *b;
    for(b = (void *)small_list_head; b != NULL; b = b->body.next){
        if(b->block_size >= asize){
            return b;
        }
    }
    return NULL; //no fit

    /* small block, look in small list
    if(asize <= SIZE_BOUND){
        b = small_list_head;
        //if no block, look in large list
        if(b == NULL){
            b = big_list_head;
            //if no block, return NULL
            if(b == NULL){
                return NULL;
            }
            while(b->block_size < asize && b != NULL){
                b = b->body.next;
            }
        }
        while(b->block_size < asize && b != NULL){
            b = b->body.next;
        }
    }
    else{
        b = big_list_head;
        //if no block, return NULL
        if(b == NULL){
            return NULL;
        }
        while(b->block_size < asize && b != NULL){
            b = b->body.next;
        }
    }*/
}

/*
 * coalesce - boundary tag coalescing. Return ptr to coalesced block
 */
static block_t *coalesce(block_t *block) {
    footer_t *prev_footer = (void *)block - sizeof(header_t);
    header_t *next_header = (void *)block + block->block_size;
    bool prev_alloc = prev_footer->allocated;
    bool next_alloc = next_header->allocated;

    block_t *next = NULL;
    block_t *prev = NULL;

    if (prev_alloc && next_alloc) { /* Case 1 */
        /* no coalesceing */
        return block;
    }
    //next is free, coalesce and remove next from list
    else if (prev_alloc && !next_alloc) { /* Case 2 */
        next = (void *)next_header;
        remove_block(next);

        /* Update header of current block to include next block's size */
        block->block_size += next_header->block_size;
        /* Update footer of next block to reflect new size */
        footer_t *next_footer = get_footer(block);
        next_footer->block_size = block->block_size;
    }
    //prev is free, remove prev from list
    else if (!prev_alloc && next_alloc) { /* Case 3 */
        prev = (void *)prev_footer - prev_footer->block_size + sizeof(header_t);
        remove_block(prev);

        /* Update header of prev block to include current block's size */
        prev->block_size += block->block_size;
        /* Update footer of current block to reflect new size */
        footer_t *footer = get_footer(prev);
        footer->block_size = prev->block_size;
        block = prev;
    }
    //both free, remove both
    else { /* Case 4 */
        next = (void *)next_header;
        remove_block(next);
        prev = (void *)prev_footer - prev_footer->block_size + sizeof(header_t);
        remove_block(prev);

        /* Update header of prev block to include current and next block's size */
        prev->block_size += block->block_size + next_header->block_size;
        /* Update footer of next block to reflect new size */
        footer_t *next_footer = get_footer(prev);
        next_footer->block_size = prev->block_size;
        block = prev;
    }
    return block;
}

static footer_t* get_footer(block_t *block) {
    return (void*)block + block->block_size - sizeof(footer_t);
}

static void printblock(block_t *block) {
    uint32_t hsize, halloc, fsize, falloc;

    hsize = block->block_size;
    halloc = block->allocated;
    footer_t *footer = get_footer(block);
    fsize = footer->block_size;
    falloc = footer->allocated;

    if (hsize == 0) {
        printf("%p: EOL\n", block);
        return;
    }

    printf("%p: header: [%d:%c] footer: [%d:%c]\n", block, hsize,
           (halloc ? 'a' : 'f'), fsize, (falloc ? 'a' : 'f'));
}

static void checkblock(block_t *block) {
    if ((uint64_t)block->body.payload % 8) {
        printf("Error: payload for block at %p is not aligned\n", block);
    }
    footer_t *footer = get_footer(block);
    if (block->block_size != footer->block_size) {
        printf("Error: header does not match footer\n");
    }
}

//add to the list
static void add_block(block_t *block){
    if(block == NULL){
        printf("Null block");
        return;
    }
    else if(block->allocated == ALLOC){
        printf("Cannot add allocated block");
        return;
    }

    /*
    printf("adding block\n");
    printf("%p\n", block);
    printf("allocated %u\n", block->allocated);
    printf("\n\n\n");

    printf("print whole list\n");
    printlist();
    */

    //add new block to start of list
    block_t *curr_head;
    curr_head = (void *)small_list_head;
    if(curr_head == NULL){
        small_list_head = (void *)block;
    }
    else{
        block->body.prev = NULL;
        block->body.next = curr_head;
        curr_head->body.prev = block;
        small_list_head = (void *)block;
    }
    /*
    printf("\n\n after add print whole list\n");
    printlist();
    printf("\n\n\n");
    */

    /*
    if(split_size <= SIZE_BOUND){
        curr_head = small_list_head;
        new_block->body.next = curr_head;
        curr_head->body.prev = new_block;
        small_list_head = new_block;
    }
    else{
        curr_head = big_list_head;
        new_block->body.next = curr_head;
        curr_head->body.prev = new_block;
        big_list_head = new_block;
    }*/
}

static void remove_block(block_t *block){
    block_t *prev;
    block_t *next;
    //remove from list
    prev = block->body.prev;
    next = block->body.next;

    //only block in the list, then just remove
    if(prev == NULL && next == NULL){
        small_list_head = NULL;
        /*
        if(asize <= SIZE_BOUND){
            small_list_head = NULL;
        }
        else{
            big_list_head = NULL;
        }*/
    }
    //head of list, make next one head
    else if(prev == NULL){
        small_list_head = (void *)next;
        next->body.prev = NULL;
        /*
        if(asize <= SIZE_BOUND){
            small_list_head = next;
        }
        else{
            big_list_head = next;
        }*/
    }
    //end of list, make prev one the last one
    else if(next == NULL){
        prev->body.next = NULL;
    }
    //in the middle, then link prev and next
    else{
        prev->body.next = next;
        next->body.prev = prev;
    }
}

static void printlist(){
    printf("list start\n");

    block_t *start;
    start = (void *)small_list_head;
    if(start == NULL){
        printf("null list\n");
        return;
    }
    while(start != NULL){
        printf("block %p \n", start);
        printf("prev %p \n", start->body.prev);
        printf("next %p \n", start->body.next);
        printf("size %zu \n", start->block_size);
        printf("allocated %u \n", start->allocated);
        start = start->body.next;
    }
    printf("list end\n");
}