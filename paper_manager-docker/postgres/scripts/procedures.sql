/* --- 6. Основные операции в виде функций. --- */

/* LECTURER */
CREATE OR REPLACE FUNCTION add_lecturer(
	newName VARCHAR(100),
	newDegree VARCHAR(30)
)
RETURNS INT AS
$$ 
DECLARE res INT;
BEGIN
	INSERT INTO LECTURER(name, degree) VALUES(newName, newDegree)
	RETURNING lecturerID INTO res;
	RETURN res;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION edit_lecturer(
	editID int,
	newName VARCHAR(100) DEFAULT NULL,
	newDegree VARCHAR(30) DEFAULT NULL
)
RETURNS void AS
$$
BEGIN
	IF newName IS NOT NULL THEN
		UPDATE LECTURER SET name = newName WHERE lecturerID = editID;
	END IF;
	IF newDegree IS NOT NULL THEN
		UPDATE LECTURER SET degree = newDegree WHERE lecturerID = editID;
	END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION remove_lecturer(
	removeID int
)
RETURNS void AS
$$
BEGIN
	DELETE FROM LECTURER WHERE lecturerID = removeID;
END;
$$ LANGUAGE plpgsql;

/* PUBLICATION */
CREATE OR REPLACE FUNCTION add_publication(
	newName VARCHAR(30),
	newType SMALLINT,
	newDate DATE
)
RETURNS INT AS
$$
DECLARE res INT;
BEGIN
	INSERT INTO PUBLICATION(name, type, publicationDate) VALUES(newName, newType, newDate)
	RETURNING publicationID INTO res;
	RETURN res;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION edit_publication(
	editID int,
	newName VARCHAR(30) DEFAULT NULL,
	newType SMALLINT DEFAULT NULL,
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
		INSERT INTO PUBLICATION_AUTHORS(publicationID, lecturerID) VALUES(editID, aID);
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
		DELETE FROM PUBLICATION_AUTHORS WHERE publicationID = editID AND lecturerID = aID;
	END LOOP;
END;
$$ LANGUAGE plpgsql;


/* PUBLISHER */
CREATE OR REPLACE FUNCTION add_publisher(
	newName VARCHAR(200),
	newAbbreviation VARCHAR(30),
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
	INSERT INTO PUBLISHER(fullName, abbreviation, country, city, address, phoneNumber, email)
	VALUES(newName, newAbbreviation, newCountry, newCity, newAddress, newPhoneNumber, newEmail)
	RETURNING publisherID INTO res;
	RETURN res;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION edit_publisher(
	editID int,
	newName VARCHAR(200) DEFAULT NULL,
	newAbbreviation VARCHAR(30) DEFAULT NULL,
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
	IF newAbbreviation IS NOT NULL THEN
		UPDATE PUBLISHER SET abbreviation = newAbbreviation WHERE publisherID = editID;
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
CREATE OR REPLACE FUNCTION create_compilation(
	newName VARCHAR(30),
	newPublisherID INT,
	newDate DATE
)
RETURNS INT AS
$$
DECLARE res INT;
BEGIN
	INSERT INTO COMPILATION(name, publisherID, publicationDate) VALUES (newName, newPublisherID, newDate)
	RETURNING publicationID INTO res;
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