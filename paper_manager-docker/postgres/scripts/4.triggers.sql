
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
