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
