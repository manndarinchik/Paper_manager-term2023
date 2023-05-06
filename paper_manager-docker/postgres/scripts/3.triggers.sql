/* --  5. Поле с триггером --- */
CREATE OR REPLACE FUNCTION generate_publisher_abbreviation() 
   RETURNS TRIGGER 
AS $$
DECLARE tokens TEXT[];
DECLARE token TEXT;
BEGIN
	NEW.abbreviation := '';
    tokens := string_to_array(NEW.fullName, ' ');
    FOREACH token IN ARRAY tokens
    LOOP
        NEW.abbreviation := NEW.abbreviation || upper(left(token, 1));
    END LOOP;
	RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;

CREATE OR REPLACE TRIGGER tg_generate_publisher_abbreviation
    BEFORE UPDATE OR INSERT 
    ON PUBLISHER 
    FOR EACH ROW
    EXECUTE PROCEDURE generate_publisher_abbreviation();
