
typedef struct HnswBuildState
{
	/* Info */
	Relation	heap;
	Relation	index;
	IndexInfo  *indexInfo;

    /* Settings */
    int			dimensions;
    int         M;
    int         efConstruction;
    bool        replacedDeleted;
    int         randomSeed;
}