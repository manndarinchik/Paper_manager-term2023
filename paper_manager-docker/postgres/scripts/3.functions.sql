/* AUTHOR */
CREATE OR REPLACE FUNCTION add_author(
	newName VARCHAR(200),
	newDegree VARCHAR(30)
) RETURNS int AS
$$
DECLARE res INT DEFAULT -1;
BEGIN
	IF (CAST((SELECT COUNT(*) FROM AUTHOR WHERE name=newName) as INT)> 0) 
		THEN RAISE EXCEPTION 'Автор с именем "%" уже существует', newName; 
	ELSE
		INSERT INTO AUTHOR_COMPOSITE(name, degree) VALUES(newName, newDegree)
		RETURNING authorID INTO res;
	END IF;
	RETURN res;
END;
$$ LANGUAGE  plpgsql;

CREATE OR REPLACE FUNCTION edit_author(
	editID int,
	newName VARCHAR(200),
	newDegree VARCHAR(30) 
) RETURNS void AS
$$
BEGIN
	IF (CAST((SELECT authorID FROM AUTHOR WHERE name=newName LIMIT 1) as INT) != editID) 
		THEN RAISE EXCEPTION 'Автор с именем "%" уже существует', newName; 
	ELSE
		UPDATE AUTHOR_COMPOSITE 
			SET name = newName,
			degree = newDegree 
		WHERE authorID = editID;
	END IF;
END;
$$ LANGUAGE  plpgsql;

CREATE OR REPLACE FUNCTION remove_author(
	removeID int
) RETURNS void AS
$$
BEGIN
	DELETE FROM AUTHOR_COMPOSITE WHERE authorID = removeID;
END;
$$ LANGUAGE  plpgsql;

/* PUBLICATION */
CREATE OR REPLACE FUNCTION add_publication(
	newName VARCHAR(200),
	newType INT,
	newPublisher INT,
	newDate DATE
) RETURNS int AS
$$
DECLARE res INT DEFAULT -1;
BEGIN
	IF (CAST((SELECT COUNT(*) FROM PUBLICATION WHERE name=newName) as INT)> 0) 
		THEN RAISE EXCEPTION 'Публикация с названием "%" уже существует', newName; 
	ELSE
		INSERT INTO PUBLICATION(name, type, publisherID, publicationDate) VALUES(newName, CAST (newType AS SMALLINT), newPublisher, newDate)
		RETURNING publicationID INTO res;
	END IF;
	RETURN res;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION edit_publication(
	editID int,
	newName VARCHAR(200),
	newType INT,
	newPublisher INT,
	newDate DATE
) RETURNS void AS
$$
BEGIN
	IF (CAST((SELECT publicationID FROM PUBLICATION WHERE name=newName LIMIT 1) as INT) != editID) 
		THEN RAISE EXCEPTION 'Публикация с названием "%" уже существует', newName; 
	ELSE
		UPDATE PUBLICATION 
			SET name = newName,
			type = CAST(newType AS SMALLINT),
			publisherID = newPublisher,
			publicationDate = newDate
		WHERE publicationID = editID;
	END IF;
END;
$$ LANGUAGE  plpgsql;

CREATE OR REPLACE FUNCTION remove_publication(
	removeID int
) RETURNS void AS
$$
BEGIN
	DELETE FROM PUBLICATION WHERE publicationID = removeID;
END;
$$ LANGUAGE  plpgsql;

CREATE OR REPLACE PROCEDURE set_publication_authors(
	editID INT,
	authorIDs INT[]
) AS
$$
DECLARE aID INT;
BEGIN
	DELETE FROM PUBLICATION_AUTHORS pa WHERE pa.publicationID = editID; 
	FOREACH aID IN ARRAY authorIDs
	LOOP
		INSERT INTO PUBLICATION_AUTHORS(publicationID, authorID) VALUES(editID, aID);
	END LOOP;
	COMMIT;
	IF (SELECT COUNT(*) FROM PUBLICATION_AUTHORS pa WHERE pa.publicationID=editID)=0
	THEN ROLLBACK;
	RAISE EXCEPTION 'У публикации % должен быть хотя бы 1 автор', (SELECT name FROM PUBLICATION WHERE publicationID=editID);
	END IF;
END;
$$ LANGUAGE  plpgsql;


/* PUBLISHER */
CREATE OR REPLACE FUNCTION add_publisher(
	newName VARCHAR(200),
	newCountry VARCHAR(100),
	newCity VARCHAR(100),
	newAddress VARCHAR(100),
	newPhoneNumber VARCHAR(13),
	newEmail VARCHAR(100)
) RETURNS int AS
$$
DECLARE res INT DEFAULT -1;
BEGIN
	IF (CAST((SELECT COUNT(*) FROM PUBLISHER WHERE fullName=newName) as INT) > 0) 
		THEN RAISE EXCEPTION 'Издатель с названием "%" уже существует', newName; 
	ELSE
		INSERT INTO PUBLISHER(fullName, country, city, address, phoneNumber, email)
		VALUES(newName, newCountry, newCity, newAddress, newPhoneNumber, newEmail)
		RETURNING publisherID INTO res;
	END IF;
	RETURN res;
END;
$$ LANGUAGE  plpgsql;

CREATE OR REPLACE FUNCTION edit_publisher(
	editID int,
	newName VARCHAR(200),
	newCountry VARCHAR(100),
	newCity VARCHAR(100),
	newAddress VARCHAR(100),
	newPhoneNumber VARCHAR(13),
	newEmail VARCHAR(100)
) RETURNS void AS
$$
BEGIN
	IF (CAST((SELECT publisherID FROM PUBLISHER WHERE fullName=newName LIMIT 1) as INT) != editID) 
		THEN RAISE EXCEPTION 'Издатель с названием "%" уже существует', newName; 
	ELSE
		UPDATE PUBLISHER SET fullName = newName,
			country = newCountry,
			city = newCity,
			address = newAddress,
			phoneNumber = newPhoneNumber,
			email = newEmail
		WHERE publisherID = editID;
	END IF;
END;
$$ LANGUAGE  plpgsql;


CREATE OR REPLACE FUNCTION remove_publisher(
	removeID int
) RETURNS void AS
$$
BEGIN
	DELETE FROM PUBLISHER WHERE publisherID = removeID;
END;
$$ LANGUAGE  plpgsql;

/* COMPILATION */
CREATE OR REPLACE FUNCTION add_compilation(
	newName VARCHAR(200),
	newPublisherID INT,
	newDate DATE
) RETURNS int AS
$$
DECLARE res INT DEFAULT -1;
BEGIN
	IF (CAST((SELECT COUNT(*) FROM COMPILATION WHERE name=newName) as INT) > 0) 
		THEN RAISE EXCEPTION 'Сборник с названием "%" уже существует', newName;
	ELSE
		INSERT INTO COMPILATION(name, publisherID, publicationDate) VALUES (newName, newPublisherID, newDate)
		RETURNING compilationID INTO res;
	END IF;
	RETURN res;
END;
$$ LANGUAGE  plpgsql;

CREATE OR REPLACE FUNCTION edit_compilation(
	cID INT,
	newName VARCHAR(200),
	newPublisherID INT,
	newDate DATE
) RETURNS void AS
$$
DECLARE res INT;
BEGIN
	IF (CAST((SELECT compilationID FROM COMPILATION WHERE name=newName LIMIT 1) AS INT) != cID) 
		THEN RAISE EXCEPTION 'Сборник с названием "%" уже существует', newName; 
	ELSE
		UPDATE COMPILATION SET
			name = newName,
			publisherID = newPublisherID, 
			publicationDate = newDate
		WHERE compilationID = cID;
	END IF;
END;
$$ LANGUAGE  plpgsql;

CREATE OR REPLACE FUNCTION remove_compilation(
	removeID int
) RETURNS void AS
$$
BEGIN
	DELETE FROM COMPILATION WHERE compilationID = removeID;
END;
$$ LANGUAGE  plpgsql;

CREATE OR REPLACE PROCEDURE set_compilation_publications(
	editID INT,
	publicationIDs INT[]
) AS
$$
DECLARE pID INT;
BEGIN
	DELETE FROM COMPILATION_ENTRY ce WHERE ce.compilationID = editID; 
	FOREACH pID IN ARRAY publicationIDs
	LOOP
		INSERT INTO COMPILATION_ENTRY(publicationID, compilationID) VALUES(pID, editID);
	END LOOP;
	COMMIT;
	IF (SELECT COUNT(*) FROM COMPILATION_ENTRY ce WHERE ce.compilationID=editID)=0
	THEN ROLLBACK;
	RAISE EXCEPTION 'В сборнике % должна быть хотя бы 1 публикация', (SELECT name FROM COMPILATION WHERE compilationID=editID);
	END IF;
END;
$$ LANGUAGE  plpgsql;

CREATE OR REPLACE PROCEDURE set_publication_compilations(
	editID INT,
	compilationIDs INT[]
) AS
$$
DECLARE cID INT;
DECLARE empty_comps text[];
BEGIN
	DELETE FROM COMPILATION_ENTRY ce WHERE ce.publicationID = editID; 
	FOREACH cID IN ARRAY compilationIDs
	LOOP
		INSERT INTO COMPILATION_ENTRY(publicationID, compilationID) VALUES(editID, cID);
	END LOOP;
	COMMIT;
	
	SELECT array_agg(c.name) INTO empty_comps 
		FROM COMPILATION_ENTRY ce JOIN COMPILATION c on ce.compilationID=c.compilationID
		GROUP BY ce.compilationID HAVING COUNT(ce.publicationID)=0;
	
	IF (CARDINALITY(empty_comps)>0)
	THEN ROLLBACK;
	RAISE EXCEPTION 'В сборниках "%" должна быть хотя бы 1 публикация', array_to_string(empty_comps, ', ');
	END IF;
END;
$$ LANGUAGE  plpgsql;

/* QUERY FUNCTIONS */

CREATE FUNCTION get_authors_publications(_id INT)
RETURNS SETOF PUBLICATION_COMPOSITE AS $$ BEGIN
	RETURN QUERY 
		SELECT *
		FROM PUBLICATION_COMPOSITE
		WHERE regexp_count(authors, (SELECT name FROM AUTHOR WHERE authorID=_ID)) > 0;
END; $$ LANGUAGE  plpgsql;

CREATE OR REPLACE FUNCTION get_publication_authors(_id INT)
RETURNS SETOF AUTHOR AS $$ BEGIN
	RETURN QUERY 
		SELECT a.*
		FROM AUTHOR a
		WHERE authorID = ANY(
			SELECT authorID FROM PUBLICATION_AUTHORS pa WHERE pa.publicationID = _id);
END; $$ LANGUAGE  plpgsql;

CREATE OR REPLACE FUNCTION get_publication_compilations(_id INT)
RETURNS SETOF COMPILATION_COMPOSITE AS $$ BEGIN
	RETURN QUERY 
		SELECT c.*
		FROM COMPILATION_COMPOSITE c
		JOIN COMPILATION_ENTRY ce ON ce.compilationID = c.compilationID
		WHERE ce.publicationID = _id;
END; $$ LANGUAGE  plpgsql;

CREATE OR REPLACE FUNCTION get_publishers_compilations(_id int)
RETURNS setof compilation_composite AS $$ BEGIN
	return 
	QUERY WITH tmp AS (
		SELECT
			c.compilationID,
			c.name,
			c.publicationdate,
			c.publisherID,
			COALESCE(COUNT(ce.publicationID), 0) publicationCount
		FROM COMPILATION c
		JOIN COMPILATION_ENTRY ce ON ce.compilationID = c.compilationID
		GROUP BY c.compilationID HAVING c.publisherID=_id
	)
	SELECT tmp.compilationID, tmp.name, p.fullName, tmp.publicationdate, tmp.publicationCount FROM tmp
	JOIN PUBLISHER p ON tmp.publisherID = p.publisherID;
END; $$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION get_publishers_publications(_id INT)
RETURNS SETOF PUBLICATION AS $$ BEGIN
	RETURN QUERY 
		SELECT p.*
		FROM PUBLICATION p
		WHERE publisherID = _id;
END; $$ LANGUAGE  plpgsql;

CREATE PROCEDURE set_authors_publications(
	aID INT,
	new_pIDs INT[]
) AS
$$
DECLARE
	i record;
	pID int;
	cur REFCURSOR;
BEGIN
	-- Удалить публикации, в которых автор есть
	OPEN cur FOR( 
		SELECT tmp.* FROM (
			SELECT
				p.publicationID p_id,
				array(SELECT authorID FROM PUBLICATION_AUTHORS pa WHERE pa.publicationID = p.publicationID) as a_ids
			FROM PUBLICATION p		
		) as tmp
		WHERE tmp.a_ids && ARRAY[aID] AND NOT new_pIDs && ARRAY[tmp.p_id]);
	LOOP
		FETCH cur INTO i;
		IF NOT FOUND THEN EXIT; END IF;
		IF NOT array_length(i.a_IDs, 1) = 1 THEN
			DELETE FROM PUBLICATION_AUTHORS pa WHERE pa.authorID = aID; 
		ELSE 
			RAISE EXCEPTION 'У публикации % должен быть хотя бы 1 автор', (SELECT name FROM PUBLICATION WHERE publicationID=i.p_id);	
		END IF;
	END LOOP;
	CLOSE cur;
	
	-- Добавить публикации, в которых автора нет
	OPEN cur FOR(
	--RETURN QUERY
		WITH tmp AS (
		SELECT
			p.publicationID p_id,
			array(SELECT authorID FROM PUBLICATION_AUTHORS pa WHERE pa.publicationID = p.publicationID) as a_ids
		FROM PUBLICATION p		
		) SELECT * FROM tmp
		WHERE new_pIDs && ARRAY[tmp.p_id] AND NOT tmp.a_ids && ARRAY[aID]);
	LOOP
		FETCH cur INTO i;
		IF NOT FOUND THEN EXIT; END IF;
		INSERT INTO PUBLICATION_AUTHORS(authorID, publicationID) VALUES(aID, i.p_id);
	END LOOP;
END;	
$$ LANGUAGE  plpgsql;

CREATE OR REPLACE FUNCTION can_edit(username NAME)
RETURNS bool AS
$$
DECLARE roles name[];
BEGIN
	SELECT array_agg(rolname) INTO roles FROM pg_user
	JOIN pg_auth_members ON (pg_user.usesysid=pg_auth_members.member)
	JOIN pg_roles ON (pg_roles.oid=pg_auth_members.roleid)
	GROUP BY pg_user.usename having pg_user.usename=username;	
	return ('{"editor_role"}' && roles);
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;