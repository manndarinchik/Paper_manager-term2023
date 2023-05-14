/*
CALL add_author('Иванов Иван', 'Доктор наук');
CALL add_author('Игорь Матвиенко', 'Кандидат наук');
CALL add_author('Ирина Соева', 'Бакалавр');
CALL add_author('Ольга Вишнева', 'Аспирант');
CALL add_author('Владимир Частный', 'Магистр');
CALL add_author('Павел Павелецкий', 'Доктор наук');
CALL add_author('Дмитрий Демитровский', 'Кандидат наук');
CALL add_author('Анастасия Стасьева', 'Бакалавр');
CALL add_author('Афанасий Федоткин', 'Бакалавр');

CALL add_publisher('Московский математический журнал', 'Российская Федерация', 'Москва', 'пр. Ленина 1', '88005553535', 'mmzh@mail.ru');
CALL add_publisher('Псковский химический журнал', 'Российская Федерация', 'Псков', 'ул. Калачиков 69', '86662286969', 'contacts@phzh.ru');
CALL add_publisher('Нижегородский чебуреко-исследовательский журнал', 'Российская Федерация', 'Нижний Новгород', 'пр. Голодных Ежей 2', '77777777777', 'chebureki@novzhur.com');

CALL add_publication('Исследование в области математического анализа', 1, 1, '2013-02-09');
CALL add_publication('Рецензия на исследование в области математического анализа', 4, 1, '2015-09-22');
CALL add_publication('Практическое применение углерода', 2, 2, '2015-10-01');
CALL add_publication('Исследование органичнских веществ', 1, 3, '2019-02-09');

CALL set_publication_authors(1, ARRAY[1, 2, 3]);
CALL set_publication_authors(2, ARRAY[3, 4]);
CALL set_publication_authors(3, ARRAY[5, 6]);
CALL set_publication_authors(4, ARRAY[7, 8]);

CALL add_compilation('Сборник исследований в области математического анализа', 1, '2016-01-01');
CALL set_compilation_publications(1, ARRAY[1, 2]);
*/
CREATE OR REPLACE FUNCTION _random(low INT ,high INT) 
RETURNS INT AS $$ BEGIN
   RETURN floor(random()* (high-low + 1) + low);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION generate_data(pubCount INT)
RETURNS void AS $$ 
DECLARE 
i INT;
j INT;
s TEXT[] = ARRAY['Доктор наук', 'Кандидат наук', 'Бакалавр', 'Аспирант', 'Магистр'];
a INT;
p INT;
c INT;
BEGIN
	CALL add_author(substr(md5(random()::text), 0, 6), s[_random(0, 4)]);
	select authorID into a FROM author order by authorID desc limit 1;
	CALL add_publisher(substr(md5(random()::text), 0, 20), substr(md5(random()::text), 0, 8), substr(md5(random()::text), 0, 10), substr(md5(random()::text), 0, 11), substr(md5(random()::text), 0, 10));
	select publisherID into p FROM publisher order by publisherID desc limit 1;
	CALL add_compilation(substr(md5(random()::text), 0, 15), p, current_date - (random() * 365*10)::int);
	select compilationID into c FROM compilation order by compilationID desc limit 1;
	
	FOR i IN 1..pubCount LOOP
		if random() > 0.1 then
			call add_publication(
				substr(md5(random()::text), 0, 30),
				_random(0, 5),
				_random(1, p),
				current_date - (random() * 365*10)::int);
		else
			CALL add_publisher(substr(md5(random()::text), 0, 20), substr(md5(random()::text), 0, 8), substr(md5(random()::text), 0, 10), substr(md5(random()::text), 0, 11), substr(md5(random()::text), 0, 10));
			p = p + 1;
			call add_publication(
				substr(md5(random()::text), 0, 30),
				_random(0, 5),
				p,
				current_date - (random() * 365*10)::int);
		end if;
		
		for j in 1.._random(1, 8) loop
			if random() > 0.1 then
				insert into publication_authors(publicationID, authorID) values(i, _random(1, a));
			else		
				CALL add_author(substr(md5(random()::text), 0, 6), s[_random(0, 5)]);
				a = a + 1;
				insert into publication_authors(publicationID, authorID) values(i, a);
			end if;
		end loop;
		
		for j in 1.._random(1, 6) loop
			if random() > 0.99 then
				if random() > 0.2 then
					insert into compilation_entry(publicationID, compilationID) values(i, _random(1, c));
				else		
					CALL add_compilation(substr(md5(random()::text), 0, 15), _random(1, p), current_date - (random() * 365*10)::int);
					c = c + 1;
					insert into compilation_entry(publicationID, compilationID) values(i, c);
				end if;
			end if;
		end loop;
	END LOOP;
END;
$$ LANGUAGE plpgsql;

/*
TRUNCATE TABLE PUBLICATION RESTART IDENTITY CASCADE;
TRUNCATE TABLE AUTHOR RESTART IDENTITY CASCADE;
TRUNCATE TABLE PUBLISHER RESTART IDENTITY CASCADE;
TRUNCATE TABLE COMPILATION RESTART IDENTITY CASCADE;
*/

SELECT generate_data(1000);


/*
SELECT * FROM PUBLICATION;
SELECT * FROM AUTHOR;
SELECT * FROM COMPILATION;
SELECT * FROM PUBLISHER;

SELECT * FROM PUBLICATION_COMPOSITE;
SELECT * FROM AUTHOR_COMPOSITE;
SELECT * FROM COMPILATION_COMPOSITE;
*/
	