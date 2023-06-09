-- drop index author_index;
-- drop index pa_index;
-- drop index compilation_index;
-- drop index publisher_index;
-- drop index publication_index;
--drop index ce_index;

CREATE INDEX publication_index ON PUBLICATION USING HASH (publicationID);
CREATE INDEX compilation_index ON COMPILATION USING HASH (compilationID);
CREATE INDEX author_index ON AUTHOR USING BRIN (authorID);
CREATE INDEX publisher_index ON PUBLISHER USING BRIN (publisherID);
CREATE INDEX pa_index ON PUBLICATION_AUTHORS USING BTREE (publicationID);
CREATE INDEX ce_index ON COMPILATION_ENTRY USING BTREE (compilationID);

CLUSTER PUBLICATION_AUTHORS USING pa_index;
CLUSTER COMPILATION_ENTRY USING ce_index;  