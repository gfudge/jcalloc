#include "alloc.h"

// TODO: FIX BULLSHIT INDENTATION

// Globally stored ptr to heap table
heap_table *heap_base = NULL;

static inline int is_magic( chunk_meta *meta );
static inline void *cenafy( void *dest, size_t size );

int init_jcalloc( void *base, size_t size ) {
  // Assign base value to global heap_base record
  if( base == NULL ) {
      return -1;
  }
  heap_base = (heap_table *)base;

  // Work backwards
  // EOM table base
  void *eom = (void *)(base + size) - sizeof(chunk_meta);
  *(chunk_meta *)eom = (chunk_meta){ EOM, ENDOFMEM, 0, NULL, NULL };

  // First table
  void *first = (void *)(base) + sizeof(heap_table);
  void *first_ptr = (void *)first + sizeof(chunk_meta);

  // Size of first chunk
  size_t first_sz = (void *)eom - (void *)(first + sizeof(chunk_meta));
  // Write first chunk meta table entry
  *(chunk_meta *)first = (chunk_meta){ MAGIC, FREE, first_sz, first_ptr, eom };
  // Write heap table entry
  *(heap_table *)heap_base = (heap_table){ first, eom, 1 };
  // Check heap base ptr is not null
  return ( heap_base != NULL ) ? 0 : -1;
}

void *jcalloc( size_t size ) {
  // Check heap initialised
  if( heap_base == NULL ) {
      // theres nothing we can do
      return NULL;
  }
  // We need to fit the meta table too
  size_t real_sz = size + sizeof(chunk_meta);
  // Get first node from heap table
  chunk_meta *node = ((heap_table *)heap_base)->head;
  // Search linked list until end
  while( node->status != ENDOFMEM ) {
      // check if node is suitable
      if( node->status == FREE && node->size >= real_sz ) {
	  // Allocate the chunk
	  node->status = ALLOC;
	  node->size = size;

	  // Construct replacement free chunk
	  // Store node next address
	  chunk_meta *old_next = node->next;
	  // Base address is end of alloc'd chunk + 1 ???
	  void *new_chunk = (void *)(node->first + node->size);
	  // new chunk first address
	  void *new_first = (void *)new_chunk + sizeof(chunk_meta);
	  // Calculate new chunk size
	  size_t new_sz = (void *)(node->next) - (void *)new_first;
	  // Construct meta header for new chunk
	  *(chunk_meta *)new_chunk = (chunk_meta){ MAGIC, FREE, new_sz, new_first, old_next };
	  // Redirect alloc'd chunk to new chunk
	  node->next = new_chunk;
      // but wait, theres more
      cenafy( node->first, node->size );
      // Return our newly alloc'd ptr :)
	  return (void *)node->first;
      }
      // Otherwise try the next chunk
      else {
	  // Check the next node is valid
	  if( node->next == NULL ) {
	      return NULL;
	  }
	  node = node->next;
      }
  }
  // If we reach here, there is no memory available :(
  return NULL;
}

void jcfree( void* chunk ) {
  // Check heap is not allocated
  if( heap_base == NULL ) {
      // Do not try to do anything
      return;
  }
  // Locate the meta block
  chunk_meta *meta = (void *)chunk - sizeof(chunk_meta);
  // This is not the chunk you are looking for
  if( !is_magic(meta) ) {
      // Fail (quickly)
      return;
  }
  // Change status to free
  meta->status = FREE;
  return;
}

static inline int is_magic( chunk_meta *meta ) {
  if( meta->magic[0] == MAGIC[0] && \
      meta->magic[1] == MAGIC[1] && \
      meta->magic[2] == MAGIC[2] ) {
      return 1;
  }
  else {
      return 0;
  }
}

static inline void *cenafy( void *dest, size_t size ) {
    BYTE *pdest = (void *)dest;

    BYTE doot[] = "doot";
    BYTE name[] = "johncena";
    size_t n_doots  = (size-sizeof(name)) / sizeof(doot);
    size_t padding  = (size-sizeof(name)) % sizeof(doot);

    if( sizeof(name) > size ) {
        // Why the fuck are you allocated less that 4 bytes bro
        return NULL;
    }

    for( size_t idx=0; idx<sizeof(name); idx++ ) {
        // Copy the name ofc
        *(pdest++) = name[idx];
    }

    if( n_doots > 0 ) {
        // let have me some doots
        for( size_t idx=0; idx<(n_doots * sizeof(doot)); idx++ ) {
            *(pdest++) = doot[ idx % sizeof(doot) ];
        }
    }

    if( padding > 0 ) {
        // i guess we should pad this or something
        for( size_t idx=0; idx<padding; idx++ ) {
            *(pdest++) = PAD;
        }
    }
    return dest;
}
