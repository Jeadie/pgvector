#include <stdint.h>
// For Index access management on HNSW index.
// Index access management (`index_am`) 
// API: see https://www.postgresql.org/docs/current/index-api.html

PGDLLEXPORT PG_FUNCTION_INFO_V1(hnswhandler);
Datum
hnswhandler(PG_FUNCTION_ARGS)
{
    /**
     * For Index access management on HNSW index.
     * Index access management (`index_am`) 
     * API: see https://www.postgresql.org/docs/current/index-api.html
     */
    IndexAmRoutine *amroutine = makeNode(IndexAmRoutine);

    /**
    * Total number of strategies (operators) by which we can traverse/search
    * this AM.  Zero if AM does not have a fixed set of strategy assignments.
    */
    amroutine->amstrategies = 0;

    /**
    * total number of support functions that this AM uses
    * _"Operator classes are so called because one thing they specify is the set of WHERE-clause operators that can be used with an index"_
    */
    amroutine->amsupport =4;

    /* opclass options support function number or 0 */
    amroutine->amoptsprocnum=0;

    /* does AM support ORDER BY indexed column's value? */
    amroutine->amcanorder=false; 

    /* does AM support ORDER BY result of an operator on indexed column? */
    amroutine->amcanorderbyop = true;

    /* does AM support backward scanning? */
    amroutine->amcanbackward = false;

    /* does AM support UNIQUE indexes? */
    amroutine->amcanunique = false;

    /* does AM support multi-column indexes? */
    amroutine->amcanmulticol = false;

    /* does AM require scans to have a constraint on the first index column? */
    amroutine->amoptionalkey = true;

    /* does AM handle ScalarArrayOpExpr quals? */
    amroutine->amsearcharray = false;

    /* does AM handle IS NULL/IS NOT NULL quals? */
    amroutine->amsearchnulls = false; // Could?

    /* can index storage data type differ from column data type? */
    amroutine->amstorage = false;

    /* can an index of this type be clustered on? */
    amroutine->amclusterable = false;

    /* does AM handle predicate locks? */
    amroutine->ampredlocks = false; // ?

    /* does AM support parallel scan? */
    amroutine->amcanparallel = false; // ?

    /* does AM support columns included with clause INCLUDE? */
    amroutine->amcaninclude = false; // ?

    /* does AM use maintenance_work_mem? */
    amroutine->amusemaintenanceworkmem = false; // ?

    /* OR of parallel vacuum flags */
    amroutine->amparallelvacuumoptions VACUUM_OPTION_PARALLEL_BULKDEL; // ? 

    /* type of data stored in index, or InvalidOid if variable */
    amroutine->amkeytype = InvalidOid; 

    /* Functions: https://www.postgresql.org/docs/current/index-functions.html */ 
    /* "Build new index" function */
    amroutine->ambuild = ivfflatbuild;
    
    /* "Build empty index" function */
	amroutine->ambuildempty = ivfflatbuildempty;
    
    /* "Insert this tuple" function */
	amroutine->aminsert = ivfflatinsert;
    
    /* Bulk-delete function */
	amroutine->ambulkdelete = ivfflatbulkdelete;
    
    /* Post-VACUUM cleanup function */
	amroutine->amvacuumcleanup = ivfflatvacuumcleanup;
    
    /**
     * If the access method does not support index-only scans at all, the amcanreturn field in its
     * IndexAmRoutine struct can be set to NULL.
    */
	amroutine->amcanreturn = NULL;

    /* Function to estimate cost of an index scan */
	amroutine->amcostestimate = ivfflatcostestimate;
    
    /* Function to parse and validate reloptions for an index */
	amroutine->amoptions = ivfflatoptions;
    
    /**
     * The amproperty method allows index access methods to override the default behavior of 
     * pg_index_column_has_property and related functions. If the access method does not have
     * any special behavior for index property inquiries, the amproperty field in its
     * IndexAmRoutine struct can be set to NULL. 
    **/
	amroutine->amproperty = NULL; /* TODO AMPROP_DISTANCE_ORDERABLE */
#if PG_VERSION_NUM >= 120000
    /* Return the textual name of the given build phase number. */
	amroutine->ambuildphasename = ivfflatbuildphasename;
    
#endif
    /**
     * Validate the catalog entries for the specified operator class, so far as the access method 
     * can reasonably do that. For example, this might include testing that all required support
     * functions are provided. The amvalidate function must return false if the opclass is
     * invalid. Problems should be reported with ereport messages, typically at INFO level.
    */
	amroutine->amvalidate = ivfflatvalidate;
    
#if PG_VERSION_NUM >= 140000
    // TODO
	amroutine->amadjustmembers = NULL;
    
#endif
    /* "Prepare for index scan" function */
	amroutine->ambeginscan = ivfflatbeginscan;
    
    /* "(Re)start index scan" function */
	amroutine->amrescan = ivfflatrescan;
    
    /* "Next valid tuple" function, or zero if none */
	amroutine->amgettuple = ivfflatgettuple;
    
    /* "Fetch all valid tuples" function, or zero if none */
	amroutine->amgetbitmap = NULL;
    
    /* "Clean up after index scan" function */
	amroutine->amendscan = ivfflatendscan;
    
    /* "Mark current scan position" function */
	amroutine->ammarkpos = NULL;
    
    /* "Restore marked scan position" function*/
	amroutine->amrestrpos = NULL;
    

    /* Interface functions to support parallel index scans */
	amroutine->amestimateparallelscan = NULL;
	amroutine->aminitparallelscan = NULL;
	amroutine->amparallelrescan = NULL;
    

	PG_RETURN_POINTER(amroutine);
    
}
