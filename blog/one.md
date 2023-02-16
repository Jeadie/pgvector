# Overview
I came across [pgvector](https://github.com/pgvector/pgvector) whilst at [Marqo](https://www.marqo.ai/) in the vector search scene learning everything I could about LLMs, latency-sensitive AI inference, and all the performance difficulties of large scale search platforms. pgvector is a Postgres module for vector similarity with a bunch of other useful vector operations. On top of that it has support for using inverted file indexes (IVFs) (via a database index) for improved search performance. Hierarchical Navigable Small World (HNSW) is a newer approximate nearest neighbour method that is achieving SOTA performance and accuracy (given approximate methods don't give exact results). I was surprised it was not a supported index type out-of-the-box, and that noone had decided to implement it themselves. I thought it would be a valuable open-source Postgres module given the current state of AI and set out to create pg-hnsw. I did not realise what I had got myself in for.

## Reading
First, understanding how Postgres modules work (and really, what they were in the first place). Two good resources on this:
1. [A Guide to Create User-Defined Extension Modules to Postgres](https://www.highgo.ca/2019/10/01/a-guide-to-create-user-defined-extension-modules-to-postgres/): A quick overview of how modules sit in the postgres ecosystem and the minimal, viable requirements for making and using an SQL-facing C module (as opposed to those not called directly in SQL, but via DBMS events, for example).
2. [Writing PostgreSQL Extensions is Fun – C Language](https://www.percona.com/blog/writing-postgresql-extensions-is-fun-c-language/): A more detailed walkthrough covering everything from 1., but getting into the weeds of what's needed in the C code for Postgres compatibility.

Useful Reference: https://www.postgresql.org/docs/current/xfunc-c.html  

## SQL -- C code

## Identifying IVF index logic vs vector logic

## Index Access Method Interface Definition
<!-- Basic API Structure for Indexes: https://www.postgresql.org/docs/current/index-api.html -->

## Understanding Indexes
- https://www.crunchydata.com/blog/postgres-indexes-for-newbies
- https://use-the-index-luke.com/
- https://pganalyze.com/blog/how-postgres-chooses-index
- https://rcoh.me/posts/postgres-indexes-under-the-hood/
- https://www.cloudbees.com/blog/discovering-computer-science-behind-postgres-indexes