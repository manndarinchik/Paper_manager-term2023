/* --- DISPLAY VIEWS --- */
DROP VIEW IF EXISTS PUBLICATION_COMPOSITE;
CREATE VIEW PUBLICATION_COMPOSITE AS
   SELECT
	tmp.publicationID, tmp.name, tmp.publicationtype, tmp.authors, tmp.date,
	pr.fullName publisher
	FROM (
		SELECT 
            pn.publicationID,
			pn.name,
			CASE 
				WHEN type=0 THEN 'Статья'
				WHEN type=1 THEN 'Препринт'
				WHEN type=2 THEN 'Монография'
				WHEN type=3 THEN 'Диссертация'
				WHEN type=4 THEN 'Патент'
				WHEN type=5 THEN 'Отчет'
			END publicationType,
			STRING_AGG(a.name, ', ') authors,
			COALESCE(CAST(pn.publicationdate AS TEXT), 'Не опубликовано') date,
			pn.publisherID
		FROM PUBLICATION_AUTHORS pa
		JOIN AUTHOR a ON a.authorID = pa.authorID
		JOIN PUBLICATION pn ON pn.publicationID = pa.publicationID
		GROUP BY pn.publicationID
	) AS tmp
	JOIN PUBLISHER pr ON pr.publisherID = tmp.publisherID;

CREATE VIEW AUTHOR_COMPOSITE AS
SELECT 
    a.authorID,
	a.name,
	a.degree,
	get_publication_amount(a.authorID, 0) papers,
	get_publication_amount(a.authorID, 1) preprints,
	get_publication_amount(a.authorID, 2) monographs,
	get_publication_amount(a.authorID, 3) dissertations,
	get_publication_amount(a.authorID, 4) patents,
	get_publication_amount(a.authorID, 5) reports
FROM AUTHOR a;

CREATE VIEW COMPILATION_COMPOSITE AS
SELECT
    c.compilationID,
	c.name,
	p.fullName,
	c.publicationdate,
	tmp.publicationCount
FROM COMPILATION c
JOIN PUBLISHER p ON c.publisherID = p.publisherID
JOIN (SELECT compilationID, COUNT(*) publicationCount
	  FROM COMPILATION_ENTRY GROUP BY compilationID)
	  AS tmp ON tmp.compilationID = c.compilationID;

/* --- DATA MANAGEMENT VIEWS --- */
CREATE OR REPLACE VIEW PUBLICATION_DATA AS
SELECT
    pc.publicationID,
	pc.publisherID,
	STRING_AGG(CAST(pa.authorID AS TEXT), ' ') authorIDs
FROM PUBLICATION pc
JOIN PUBLICATION_AUTHORS pa ON pa.publicationID = pc.publicationID
GROUP BY pc.publicationID HAVING STRING_AGG(CAST(pa.authorID AS TEXT), ' ') IS NOT NULL;