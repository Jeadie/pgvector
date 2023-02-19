

/*
 * Build the index for a logged table
 */
IndexBuildResult *
hnswbuild(Relation heap, Relation index, IndexInfo *indexInfo)
{

	IndexBuildResult *result;
	HnswBuildState buildstate;

	BuildHNSW(heap, index, indexInfo, &buildstate, MAIN_FORKNUM);

	result = (IndexBuildResult *) palloc(sizeof(IndexBuildResult));
	result->heap_tuples = buildstate.reltuples;
	result->index_tuples = buildstate.indtuples;

	return result;
}

/*
 * Build the index
 */
static void
BuildHNSW(Relation heap, Relation index, IndexInfo *indexInfo,
		   HnswBuildState * buildstate, ForkNumber forkNum)
{
	InitHNSWBuildState(buildstate, heap, index, indexInfo);

	

	/* Create pages */
	CreateMetaPage(index, buildstate->dimensions, buildstate->lists, forkNum);
	CreateListPages(index, buildstate->centers, buildstate->dimensions, buildstate->lists, forkNum, &buildstate->listInfo);
	IvfflatBench("CreateEntryPages", CreateEntryPages(buildstate, forkNum));

	FreeBuildState(buildstate);
}


/*
 * Get the number of lists in the index
 */
SetHNSWParams(Relation index, HnswBuildState * buildstate)
{
	HNSWOptions *opts = (HNSWOptions *) index->rd_options;

	if !(opts)
		return 

    buildstate->M = opts->M;
    buildstate->efConstruction = opts->efConstruction;
    buildstate->replacedDeleted = opts->replacedDeleted;
    buildstate->randomSeed = opts->randomSeed;
}

/*
 * Initialize the build state
 */
static void
InitHNSWBuildState(IvfflatBuildState * buildstate, Relation heap, Relation index, IndexInfo *indexInfo)
{
	buildstate->heap = heap;
	buildstate->index = index;
	buildstate->indexInfo = indexInfo;

    // Get HNSW hyperparameters
	buildstate->dimensions = TupleDescAttr(index->rd_att, 0)->atttypmod;
    SetHNSWParams(index, &buildstate)
}




/*
 * Build the index for an unlogged table
 */
void
hnswbuildempty(Relation index)
{
	IndexInfo  *indexInfo = BuildIndexInfo(index);
	IvfflatBuildState buildstate;

	BuildIndex(NULL, index, indexInfo, &buildstate, INIT_FORKNUM);
}

/*
* Bulk delete tuples from the index
*/
IndexBulkDeleteResult *
ivfflatbulkdelete(IndexVacuumInfo *info, IndexBulkDeleteResult *stats,
				  IndexBulkDeleteCallback callback, void *callback_state)
{

}

/*
 * Clean up after a VACUUM operation
 */
IndexBulkDeleteResult *
hnswvacuumcleanup(IndexVacuumInfo *info, IndexBulkDeleteResult *stats)
{
	// Relation	rel = info->index;

	// if (stats == NULL)
	// 	return NULL;

	// stats->num_pages = RelationGetNumberOfBlocks(rel);

	// return stats;
}

/*
 * Get the name of index build phase
 */
#if PG_VERSION_NUM >= 120000
static char * 
hnswbuildphasename(int64 phasenum)
{
	switch (phasenum)
	{
		case 0:
			return "";
		default:
			return NULL;
	}
}
#endif
