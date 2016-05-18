/*
 * alloc.h
 *
 *  Created on: 11 May 2016
 *      Author: elgf2
 */

#ifndef ALLOC_H_
#define ALLOC_H_

#include <stddef.h>

#define BYTE unsigned char
#define PAD 0x0
#define MIN_HEAP_SIZE 	0x1000	// 4kb
#define MAX_HEAP_SIZE 	0x10000	// 64kb
#define MIN_CHUNK_SIZE	0x40	// 64b
#define MAX_CHUNK_SIZE	0x800	// 2kb
#define MAX_N_CHUNKS	MAX_HEAP_SIZE/MIN_CHUNK_SIZE

#define MAGIC		"MEM"
#define EOM		    "EOM"
#define ENDOFMEM	0xF0
#define ALLOC		0x01
#define	FREE		0x00

typedef struct chunk_meta_t {
  char		magic[3];
  char		status;	// status of chunk
  size_t	size;	// chunk size
  void 		*first;	// First byte in chunk
  struct chunk_meta_t	*next;	// ptr to next chunk
} chunk_meta;


typedef struct {
  chunk_meta 	*head;	// Head of list of allocated chunks
  chunk_meta 	*end;	// End of memory meta chunk
  size_t 	n_chunks;
} heap_table;

// Malloc/free replacement
void *jcalloc( size_t size );
void jcfree( void *mem );

int init_jcalloc( void *heap, size_t size );

#endif /* ALLOC_H_ */
