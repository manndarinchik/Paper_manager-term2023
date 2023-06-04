CREATE ROLE viewer LOGIN INHERIT PASSWORD 'viewer';
CREATE ROLE viewer_role NOINHERIT;
GRANT SELECT ON TABLE 
	PUBLISHER, PUBLICATION, AUTHOR, COMPILATION, PUBLICATION_AUTHORS, COMPILATION_ENTRY,
	PUBLICATION_COMPOSITE,
	COMPILATION_COMPOSITE,
	AUTHOR_COMPOSITE
TO viewer_role;
GRANT EXECUTE ON FUNCTION 
	get_publication_authors,
	get_publication_compilations,
	get_authors_publications
TO viewer_role;
GRANT USAGE ON ALL SEQUENCES IN SCHEMA PUBLIC TO viewer_role;
GRANT viewer_role TO viewer;

CREATE ROLE editor LOGIN INHERIT PASSWORD 'editor';
CREATE ROLE editor_role NOINHERIT;
GRANT SELECT, INSERT, UPDATE ON TABLE 
	PUBLISHER, PUBLICATION, AUTHOR, COMPILATION, PUBLICATION_AUTHORS, COMPILATION_ENTRY,
	PUBLICATION_COMPOSITE,
	COMPILATION_COMPOSITE,
	AUTHOR_COMPOSITE
TO editor_role;
GRANT EXECUTE ON FUNCTION 
	add_author,    add_compilation,    add_publication,    add_publisher,
	edit_author,   edit_publication,   edit_publication,   edit_publisher,
	remove_author, remove_compilation, remove_publication, remove_publisher
TO editor_role;
GRANT EXECUTE ON PROCEDURE
	set_publication_authors,
	set_compilation_publications,
	set_authors_publications
TO editor_role;
GRANT USAGE ON ALL SEQUENCES IN SCHEMA PUBLIC TO editor_role;
GRANT editor_role TO editor;
