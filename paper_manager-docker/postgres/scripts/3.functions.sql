/* AUTHOR */
CREATE OR REPLACE PROCEDURE add_author(
	newName VARCHAR(200),
	newDegree VARCHAR(30) DEFAULT NULL
) AS
$$
BEGIN
	-- TODO: проверка на наличие имени
	INSERT INTO AUTHOR(name, degree) VALUES(newName, newDegree);
END;
$$ LANGUAGE  plpgsql security definer;

CREATE OR REPLACE PROCEDURE edit_author(
	editID int,
	newName VARCHAR(200) DEFAULT NULL,
	newDegree VARCHAR(30) DEFAULT NULL
) AS
$$
BEGIN
	IF newName IS NOT NULL THEN
		UPDATE AUTHOR SET name = newName WHERE authorID = editID;
	END IF;
	IF newDegree IS NOT NULL THEN
		UPDATE AUTHOR SET degree = newDegree WHERE authorID = editID;
	END IF;
END;
$$ LANGUAGE  plpgsql security definer;

CREATE OR REPLACE PROCEDURE remove_author(
	removeID int
) AS
$$
BEGIN
	DELETE FROM AUTHOR_STATS WHERE authorID = removeID;
END;
$$ LANGUAGE  plpgsql security definer security definer;

/* PUBLICATION */
CREATE OR REPLACE PROCEDURE add_publication(
	newName VARCHAR(200),
	newType SMALLINT,
	newPublisher INT,
	newDate DATE DEFAULT NULL
) AS
$$
BEGIN
	-- TODO: проверка на наличие имени
	INSERT INTO PUBLICATION(name, type, publisherID, publicationDate) VALUES(newName, newType, newPublisher, newDate);
END;
$$ LANGUAGE  plpgsql security definer;
CREATE OR REPLACE PROCEDURE add_publication(
	newName VARCHAR(200),
	newType INT, /*Почему psql неявно не приводит int в smalint..................*/
	newPublisher INT,
	newDate DATE
) AS
$$
BEGIN
	INSERT INTO PUBLICATION(name, type, publisherID, publicationDate) VALUES(newName, CAST (newType AS INT), newPublisher, newDate);
END;
$$ LANGUAGE  plpgsql security definer;

CREATE OR REPLACE PROCEDURE edit_publication(
	editID int,
	newName VARCHAR(200) DEFAULT NULL,
	newType SMALLINT DEFAULT NULL,
	newPublisher INT DEFAULT NULL,
	newDate DATE DEFAULT NULL
) AS
$$
BEGIN
	IF newName IS NOT NULL THEN
		UPDATE PUBLICATION SET name = newName WHERE publicationID = editID;
	END IF;
	IF newType IS NOT NULL THEN
		UPDATE PUBLICATION SET type = newType WHERE publicationID = editID;
	END IF;
	IF newPublisher IS NOT NULL THEN
		UPDATE PUBLICATION SET publisherID = newPublisher WHERE publicationID = editID;
	END IF;
	IF newDate IS NOT NULL THEN
		UPDATE PUBLICATION SET publicationDate = newDate WHERE publicationID = editID;
	END IF;
END;
$$ LANGUAGE  plpgsql security definer;

CREATE OR REPLACE PROCEDURE remove_publication(
	removeID int
) AS
$$
BEGIN
	DELETE FROM PUBLICATION WHERE publicationID = removeID;
END;
$$ LANGUAGE  plpgsql security definer;

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
	
	IF (SELECT COUNT(*) FROM PUBLICATION_AUTHORS pa WHERE pa.publicationID=editID)=0
	THEN ROLLBACK; END IF;
	-- TODO: raise error;
END;
$$ LANGUAGE  plpgsql security definer;


/* PUBLISHER */
CREATE OR REPLACE PROCEDURE add_publisher(
	newName VARCHAR(200),
	newCountry VARCHAR(100) DEFAULT NULL,
	newCity VARCHAR(100) DEFAULT NULL,
	newAddress VARCHAR(100) DEFAULT NULL,
	newPhoneNumber VARCHAR(13) DEFAULT NULL,
	newEmail VARCHAR(100) DEFAULT NULL
) AS
$$
DECLARE res INT;
BEGIN
	-- TODO: проверка на наличие имени
	INSERT INTO PUBLISHER(fullName, country, city, address, phoneNumber, email)
	VALUES(newName, newCountry, newCity, newAddress, newPhoneNumber, newEmail);
END;
$$ LANGUAGE  plpgsql security definer;

CREATE OR REPLACE PROCEDURE edit_publisher(
	editID int,
	newName VARCHAR(200) DEFAULT NULL,
	newCountry VARCHAR(100) DEFAULT NULL,
	newCity VARCHAR(100) DEFAULT NULL,
	newAddress VARCHAR(100) DEFAULT NULL,
	newPhoneNumber VARCHAR(13) DEFAULT NULL,
	newEmail VARCHAR(100) DEFAULT NULL
) AS
$$
BEGIN
	IF newName IS NOT NULL THEN
		UPDATE PUBLISHER SET fullName = newName WHERE publisherID = editID;
	END IF;
	IF newCountry IS NOT NULL THEN
		UPDATE PUBLISHER SET country = newCountry WHERE publisherID = editID;
	END IF;
	IF newCity IS NOT NULL THEN
		UPDATE PUBLISHER SET city = newCity WHERE publisherID = editID;
	END IF;
	IF newAddress IS NOT NULL THEN
		UPDATE PUBLISHER SET address = newAddress WHERE publisherID = editID;
	END IF;
	IF newPhoneNumber IS NOT NULL THEN
		UPDATE PUBLISHER SET phoneNumber = newPhoneNumber WHERE publisherID = editID;
	END IF;
	IF newEmail IS NOT NULL THEN
		UPDATE PUBLISHER SET email = newEmail WHERE publisherID = editID;
	END IF;
END;
$$ LANGUAGE  plpgsql security definer;


CREATE OR REPLACE PROCEDURE remove_publisher(
	removeID int
) AS
$$
BEGIN
	DELETE FROM PUBLISHER WHERE publisherID = removeID;
END;
$$ LANGUAGE  plpgsql security definer;

/* COMPILATION */
CREATE OR REPLACE PROCEDURE add_compilation(
	newName VARCHAR(200),
	newPublisherID INT,
	newDate DATE DEFAULT NULL
) AS
$$
BEGIN
	-- TODO: проверка на наличие имени
	INSERT INTO COMPILATION(name, publisherID, publicationDate) VALUES (newName, newPublisherID, newDate);
END;
$$ LANGUAGE  plpgsql security definer;

CREATE OR REPLACE PROCEDURE remove_compilation(
	removeID int
) AS
$$
BEGIN
	DELETE FROM COMPILATION WHERE compilationID = removeID;
END;
$$ LANGUAGE  plpgsql security definer;

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
	
	IF (SELECT COUNT(*) FROM COMPILATION_ENTRY ce WHERE ce.compilationID=editID)=0
	THEN ROLLBACK; END IF;
	-- TODO: raise error;
END;
$$ LANGUAGE  plpgsql security definer;

/* QUERY FUNCTIONS */

CREATE FUNCTION get_authors_publications(_id INT)
RETURNS SETOF PUBLICATION_COMPOSITE AS $$ BEGIN
	RETURN QUERY 
		SELECT *
		FROM PUBLICATION_COMPOSITE
		WHERE regexp_count(authors, (SELECT name FROM AUTHOR WHERE authorID=_ID)) > 0;
END; $$ LANGUAGE  plpgsql security definer;

CREATE FUNCTION get_publication_authors(_id INT)
RETURNS SETOF AUTHOR AS $$ BEGIN
	RETURN QUERY 
		SELECT a.*
		FROM AUTHOR a
		JOIN PUBLICATION_AUTHORS pa ON a.authorID = pa.authorID
		WHERE pa.publicationID = _id;
END; $$ LANGUAGE  plpgsql security definer;

CREATE OR REPLACE FUNCTION get_publication_compilations(_id INT)
RETURNS SETOF COMPILATION_COMPOSITE AS $$ BEGIN
	RETURN QUERY 
		SELECT c.*
		FROM COMPILATION_COMPOSITE c
		JOIN COMPILATION_ENTRY ce ON ce.compilationID = c.compilationID
		WHERE ce.publicationID = _id;
END; $$ LANGUAGE  plpgsql security definer;


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
		WITH tmp AS (
		SELECT
			p.publicationID p_id,
			array(SELECT authorID FROM PUBLICATION_AUTHORS pa WHERE pa.publicationID = p.publicationID) as a_ids
		FROM PUBLICATION p		
		) SELECT * FROM tmp
		WHERE tmp.a_ids && ARRAY[aID] AND NOT new_pIDs && ARRAY[tmp.p_id]);
	LOOP
		FETCH cur INTO i;
		IF NOT FOUND THEN EXIT; END IF;
		IF NOT array_length(i.a_IDs, 1) = 1 THEN
			DELETE FROM PUBLICATION_AUTHORS pa WHERE pa.authorID = aID; 
		ELSE
			-- TODO: raise warning;
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
$$ LANGUAGE  plpgsql security definer;