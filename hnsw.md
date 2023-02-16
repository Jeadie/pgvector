Implementing HNSW directly into pgvector requires several steps, and creating a new PostgreSQL extension is just one of them. Here's an example of how to create a new PostgreSQL extension that includes the HNSW implementation in pgvector:

1. First, create a new directory for your extension and navigate to it:
```bash
mkdir pgvector-hnsw
cd pgvector-hnsw
```
1. Create a new C file for your extension and add the necessary headers and functions. Here's an example of how to define the necessary functions to interface with the hnswlib library:
```c
#include "postgres.h"
#include "fmgr.h"
#include "access/gist.h"
#include "access/skey.h"
#include "catalog/pg_type.h"
#include "utils/builtins.h"
#include "utils/memutils.h"
#include "utils/array.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"

#include "hnswlib.h"

PG_MODULE_MAGIC;

typedef struct HNSWIndex {
  IndexMethodCallInfo   *method;
  HNSWlib::HierarchicalNSW<float> *hnsw_index;
  MemoryContext         index_cxt;
} HNSWIndex;

PG_FUNCTION_INFO_V1(hnsw_build_index);
Datum hnsw_build_index(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(hnsw_search_index);
Datum hnsw_search_index(PG_FUNCTION_ARGS);
```

The HNSWIndex struct contains the necessary metadata for your index, including the method call information, the hnsw_index object, and the memory context used for the index.

The hnsw_build_index function is used to build a new HNSW index from a set of input vectors. The hnsw_search_index function is used to search the HNSW index for the k-nearest neighbors of a given query vector.

3. Implement the hnsw_build_index function. Here's an example of how to build a new HNSW index from an array of input vectors:
```c
Datum
hnsw_build_index(PG_FUNCTION_ARGS)
{
  ArrayType    *input_array = PG_GETARG_ARRAYTYPE_P(0);
  int32         dimension = PG_GETARG_INT32(1);
  int32         max_elements = PG_GETARG_INT32(2);
  float         efConstruction = PG_GETARG_FLOAT4(3);

  int           num_elements = ArrayGetNItems(ARR_NDIM(input_array), ARR_DIMS(input_array));
  float        *data = (float *) ARR_DATA_PTR(input_array);

  HNSWIndex    *index;
  MemoryContext old_cxt;

  index = (HNSWIndex *) MemoryContextAllocZero(CurrentMemoryContext, sizeof(HNSWIndex));
  index->hnsw_index = new HNSWlib::HierarchicalNSW<float>(dimension, max_elements, HNSWlib::L2);
  index->index_cxt = CurrentMemoryContext;

  old_cxt = MemoryContextSwitchTo(index->index_cxt);

  for (int i = 0; i < num_elements; i++) {
    float *vector = data + i * dimension;
    index->hnsw_index->addPoint(vector, i);
  }

  index->hnsw_index->setEf(efConstruction);

  MemoryContextSwitchTo(old_cxt);

  PG_RETURN_POINTER(index);
}
```
This function takes as input an array of input vectors, the dimension of the vectors, the maximum number of elements in the index, and the efConstruction parameter for HNSW. It creates a new HNSW index, adds the input vectors to the index, and sets the efConstruction parameter.
 4. Implement the hnsw_search_index function. Here's an example of how to search the HNSW index for the k-nearest neighbors of a query vector:
 ```c
Datum
hnsw_search_index(PG_FUNCTION_ARGS)
{
  HNSWIndex    *index = (HNSWIndex *) PG_GETARG_POINTER(0);
  ArrayType    *query_array = PG_GETARG_ARRAYTYPE_P(1);
  int32         k = PG_GETARG_INT32(2);

  int32         dimension = PG_GETARG_INT32(3);
  float        *query_vector = (float *) ARR_DATA_PTR(query_array);

  MemoryContext old_cxt;
  int          *neighbors;

  old_cxt = MemoryContextSwitchTo(index->index_cxt);

  neighbors = (int *) palloc(sizeof(int) * k);
  index->hnsw_index->searchKnn(query_vector, neighbors, k);

  MemoryContextSwitchTo(old_cxt);

  PG_RETURN_POINTER(neighbors);
}
```
This function takes as input an HNSW index, a query vector, and the number of nearest neighbors to return. It searches the HNSW index for the k-nearest neighbors of the query vector and returns their indices.

5. Finally, create the necessary Makefile to build the extension and the hnswlib library. Here's an example of a Makefile that assumes you have already downloaded and compiled the hnswlib library:
```makefile
MODULES = pgvector_hnsw
EXTENSION = pgvector_hnsw
DATA = pgvector_hnsw--1.0.sql
OBJS = pgvector_hnsw.o

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

SHLIB_LINK += -L/path/to/hnswlib -lhnswlib
```
This Makefile compiles the pgvector_hnsw extension, links it with the hnswlib library, and installs it into the appropriate directory.

6. Build and install the extension:
make
make install
7. Finally, load the extension in PostgreSQL:
```sql
CREATE EXTENSION pgvector_hnsw;
```
This will create a new extension in PostgreSQL that includes the HNSW implementation in pgvector. You can then use this extension to build and search HNSW indices for your vector data.


