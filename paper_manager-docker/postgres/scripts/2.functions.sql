/* AUTHOR */
CREATE OR REPLACE PROCEDURE add_author(
	newName VARCHAR(200),
	newDegree VARCHAR(30)
) AS
$$
BEGIN
	INSERT INTO AUTHOR(name, degree) VALUES(newName, newDegree);
END;
$$ LANGUAGE plpgsql;

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
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE remove_author(
	removeID int
) AS
$$
BEGIN
	DELETE FROM AUTHOR_STATS WHERE authorID = removeID;
END;
$$ LANGUAGE plpgsql;

/* PUBLICATION */
CREATE OR REPLACE PROCEDURE add_publication(
	newName VARCHAR(200),
	newType SMALLINT,
	newPublisher INT,
	newDate DATE
) AS
$$
BEGIN
	INSERT INTO PUBLICATION(name, type, publisherID, publicationDate) VALUES(newName, newType, newPublisher, newDate);
END;
$$ LANGUAGE plpgsql;
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
$$ LANGUAGE plpgsql;

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
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE remove_publication(
	removeID int
) AS
$$
BEGIN
	DELETE FROM PUBLICATION WHERE publicationID = removeID;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE add_authors_to_publication(
	editID INT,
	authorIDs INT[]
) AS
$$
DECLARE aID INT;
BEGIN
	FOREACH aID IN ARRAY authorIDs
	LOOP
		INSERT INTO PUBLICATION_AUTHORS(publicationID, authorID) VALUES(editID, aID);
	END LOOP;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE remove_authors_from_publication(
	editID INT,
	authorIDs INT[]
) AS
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
CREATE OR REPLACE PROCEDURE add_publisher(
	newName VARCHAR(200),
	newCountry VARCHAR(100),
	newCity VARCHAR(100),
	newAddress VARCHAR(100),
	newPhoneNumber VARCHAR(13),
	newEmail VARCHAR(100)
) AS
$$
DECLARE res INT;
BEGIN
	INSERT INTO PUBLISHER(fullName, country, city, address, phoneNumber, email)
	VALUES(newName, newCountry, newCity, newAddress, newPhoneNumber, newEmail);
END;
$$ LANGUAGE plpgsql;

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
$$ LANGUAGE plpgsql;


CREATE OR REPLACE PROCEDURE remove_publisher(
	removeID int
) AS
$$
BEGIN
	DELETE FROM PUBLISHER WHERE publisherID = removeID;
END;
$$ LANGUAGE plpgsql;

/* COMPILATION */
CREATE OR REPLACE PROCEDURE add_compilation(
	newName VARCHAR(200),
	newPublisherID INT,
	newDate DATE
) AS
$$
BEGIN
	INSERT INTO COMPILATION(name, publisherID, publicationDate) VALUES (newName, newPublisherID, newDate);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE remove_compilation(
	removeID int
) AS
$$
BEGIN
	DELETE FROM COMPILATION WHERE compilationID = removeID;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE add_publications_to_compilation(
	editID INT,
	publicationIDs INT[]
) AS
$$
DECLARE pID INT;
BEGIN
	FOREACH pID IN ARRAY publicationIDs
	LOOP
		INSERT INTO COMPILATION_ENTRY(publicationID, compilationID) VALUES(pID, editID);
	END LOOP;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE remove_publications_from_compilation(
	editID INT,
	publicationIDs INT[]
) AS
$$
DECLARE pID INT;
BEGIN
	FOREACH pID IN ARRAY publicationIDs
	LOOP
		DELETE FROM COMPILATION_ENTRY WHERE publicationID = pID AND compilationID = editID;
	END LOOP;
END;
$$ LANGUAGE plpgsql;

/* DATA FUNCTIONS */

CREATE OR REPLACE FUNCTION generate_abbreviation(str TEXT) 
   RETURNS TEXT 
AS $$
DECLARE tokens TEXT[];
DECLARE token TEXT;
DECLARE res TEXT;
BEGIN
	res := '';
    tokens := string_to_array(str, ' ');
    FOREACH token IN ARRAY tokens
    LOOP
        res := res || upper(left(token, 1));
    END LOOP;
	RETURN res;
END;
$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE FUNCTION get_publication_amount(_id INT, _type INT)
RETURNS INT
AS $$
BEGIN
	RETURN CAST((SELECT
		COUNT(*)
	FROM PUBLICATION_AUTHORS pa
	JOIN PUBLICATION p ON p.publicationID = pa.publicationID AND p.type = _type 
	WHERE pa.authorID = _id) AS INT);
END $$ LANGUAGE plpgsql;