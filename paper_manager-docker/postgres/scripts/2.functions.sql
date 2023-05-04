/* --- 6. Основные операции в виде функций. --- */

/* AUTHOR */
CREATE OR REPLACE FUNCTION add_author(
	newName VARCHAR(200),
	newDegree VARCHAR(30)
)
RETURNS INT AS
$$ 
DECLARE res INT;
BEGIN
	INSERT INTO AUTHOR(name, degree) VALUES(newName, newDegree)
	RETURNING authorID INTO res;
	RETURN res;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION edit_author(
	editID int,
	newName VARCHAR(200) DEFAULT NULL,
	newDegree VARCHAR(30) DEFAULT NULL
)
RETURNS void AS
$$
BEGIN
	IF newName IS NOT NULL THEN
		UPDATE AUTHOR SET name = newName WHERE authorID = editID;
	END IF;
	IF newDegree IS NOT NULL THEN
		UPDATE AUTHOR SET degree = newDegree WHERE authorID = editID;
	END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION remove_author(
	removeID int
)
RETURNS void AS
$$
BEGIN
	DELETE FROM AUTHOR WHERE authorID = removeID;
END;
$$ LANGUAGE plpgsql;

/* PUBLICATION */
CREATE OR REPLACE FUNCTION add_publication(
	newName VARCHAR(200),
	newType SMALLINT,
	newPublisher INT,
	newDate DATE
)
RETURNS INT AS
$$
DECLARE res INT;
BEGIN
	INSERT INTO PUBLICATION(name, type, publisherID, publicationDate) VALUES(newName, newType, newPublisher, newDate)
	RETURNING publicationID INTO res;
	RETURN res;
END;
$$ LANGUAGE plpgsql;
CREATE OR REPLACE FUNCTION add_publication(
	newName VARCHAR(200),
	newType INT, /*Почему psql неявно не приводит int в smalint..................*/
	newPublisher INT,
	newDate DATE
)
RETURNS INT AS
$$
DECLARE res INT;
BEGIN
	INSERT INTO PUBLICATION(name, type, publisherID, publicationDate) VALUES(newName, CAST (newType AS INT), newPublisher, newDate)
	RETURNING publicationID INTO res;
	RETURN res;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION edit_publication(
	editID int,
	newName VARCHAR(200) DEFAULT NULL,
	newType SMALLINT DEFAULT NULL,
	newPublisher INT DEFAULT NULL,
	newDate DATE DEFAULT NULL
)
RETURNS void AS
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
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION remove_publication(
	removeID int
)
RETURNS void AS
$$
BEGIN
	DELETE FROM PUBLICATION WHERE publicationID = removeID;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION add_authors_to_publication(
	editID INT,
	authorIDs INT[]
)
RETURNS void AS
$$
DECLARE aID INT;
BEGIN
	FOREACH aID IN ARRAY authorIDs
	LOOP
		INSERT INTO PUBLICATION_AUTHORS(publicationID, authorID) VALUES(editID, aID);
	END LOOP;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION remove_authors_from_publication(
	editID INT,
	authorIDs INT[]
)
RETURNS void AS
$$
DECLARE aID INT;
BEGIN
	FOREACH aID IN ARRAY authorIDs
	LOOP
		DELETE FROM PUBLICATION_AUTHORS WHERE publicationID = editID AND authorID = aID;
	END LOOP;
END;
$$ LANGUAGE plpgsql;


/* PUBLISHER */
CREATE OR REPLACE FUNCTION add_publisher(
	newName VARCHAR(200),
	newCountry VARCHAR(100),
	newCity VARCHAR(100),
	newAddress VARCHAR(100),
	newPhoneNumber VARCHAR(13),
	newEmail VARCHAR(100)
)
RETURNS INT AS
$$
DECLARE res INT;
BEGIN
	INSERT INTO PUBLISHER(fullName, country, city, address, phoneNumber, email)
	VALUES(newName, newCountry, newCity, newAddress, newPhoneNumber, newEmail)
	RETURNING publisherID INTO res;
	RETURN res;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION edit_publisher(
	editID int,
	newName VARCHAR(200) DEFAULT NULL,
	newCountry VARCHAR(100) DEFAULT NULL,
	newCity VARCHAR(100) DEFAULT NULL,
	newAddress VARCHAR(100) DEFAULT NULL,
	newPhoneNumber VARCHAR(13) DEFAULT NULL,
	newEmail VARCHAR(100) DEFAULT NULL
)
RETURNS void AS
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
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION remove_publisher(
	removeID int
)
RETURNS void AS
$$
BEGIN
	DELETE FROM PUBLISHER WHERE publisherID = removeID;
END;
$$ LANGUAGE plpgsql;

/* COMPILATION */
CREATE OR REPLACE FUNCTION add_compilation(
	newName VARCHAR(200),
	newPublisherID INT,
	newDate DATE
)
RETURNS INT AS
$$
DECLARE res INT;
BEGIN
	INSERT INTO COMPILATION(name, publisherID, publicationDate) VALUES (newName, newPublisherID, newDate)
	RETURNING compilationID INTO res;
	RETURN res;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION remove_compilation(
	removeID int
)
RETURNS void AS
$$
BEGIN
	DELETE FROM COMPILATION WHERE compilationID = removeID;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION add_publications_to_compilation(
	editID INT,
	publicationIDs INT[]
)
RETURNS void AS
$$
DECLARE pID INT;
BEGIN
	FOREACH pID IN ARRAY publicationIDs
	LOOP
		INSERT INTO COMPILATION_ENTRY(publicationID, compilationID) VALUES(pID, editID);
	END LOOP;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION remove_publications_from_compilation(
	editID INT,
	publicationIDs INT[]
)
RETURNS void AS
$$
DECLARE pID INT;
BEGIN
	FOREACH pID IN ARRAY publicationIDs
	LOOP
		DELETE FROM COMPILATION_ENTRY WHERE publicationID = pID AND compilationID = editID;
	END LOOP;
END;
$$ LANGUAGE plpgsql;