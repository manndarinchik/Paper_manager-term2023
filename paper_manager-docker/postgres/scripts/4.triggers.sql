
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

CREATE OR REPLACE FUNCTION generate_publisher_abbreviation() 
   RETURNS TRIGGER 
AS $$
BEGIN
	NEW.abbreviation := generate_abbreviation(NEW.fullName);
	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE TRIGGER tg_generate_publisher_abbreviation
    BEFORE UPDATE OR INSERT 
    ON PUBLISHER 
    FOR EACH ROW
    EXECUTE PROCEDURE generate_publisher_abbreviation();



CREATE OR REPLACE FUNCTION insert_author_composite()
    RETURNS TRIGGER
AS $$
BEGIN
	INSERT INTO AUTHOR(name, degree) VALUES(NEW.Name, NEW.Degree);
	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;
CREATE OR REPLACE FUNCTION update_author_composite()
    RETURNS TRIGGER
AS $$
BEGIN
    UPDATE AUTHOR 
        SET name = NEW.name,
        degree = NEW.degree 
    WHERE authorID = NEW.authorID;
	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;
CREATE OR REPLACE FUNCTION delete_author_composite()
    RETURNS TRIGGER
AS $$
BEGIN
    DELETE FROM AUTHOR WHERE authorID = NEW.authorID;
	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE TRIGGER tg_insert_author_composite
    INSTEAD OF INSERT
    ON AUTHOR_COMPOSITE 
    FOR EACH ROW
    EXECUTE PROCEDURE insert_author_composite();
CREATE OR REPLACE TRIGGER tg_update_author_composite
    INSTEAD OF UPDATE 
    ON AUTHOR_COMPOSITE 
    FOR EACH ROW
    EXECUTE PROCEDURE update_author_composite();
CREATE OR REPLACE TRIGGER tg_delete_author_composite
    INSTEAD OF DELETE 
    ON AUTHOR_COMPOSITE 
    FOR EACH ROW
    EXECUTE PROCEDURE delete_author_composite();