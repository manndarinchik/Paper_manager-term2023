SELECT add_author('Иванов Иван', 'Доктор наук');
SELECT add_author('Игорь Матвиенко', 'Кандидат наук');
SELECT add_author('Ирина Соева', 'Бакалавр');
SELECT add_author('Ольга Вишнева', 'Аспирант');
SELECT add_author('Владимир Частный', 'Магистр');
SELECT add_author('Павел Павелецкий', 'Доктор наук');
SELECT add_author('Дмитрий Демитровский', 'Кандидат наук');
SELECT add_author('Анастасия Стасьева', 'Бакалавр');
SELECT add_author('Афанасий Федоткин', 'Бакалавр');

SELECT add_publisher('Московский математический журнал', 'Российская Федерация', 'Москва', 'пр. Ленина 1', '88005553535', 'mmzh@mail.ru');
SELECT add_publisher('Псковский химический журнал', 'Российская Федерация', 'Псков', 'ул. Калачиков 69', '86662286969', 'contacts@phzh.ru');
SELECT add_publisher('Нижегородский чебуреко-исследовательский журнал', 'Российская Федерация', 'Нижний Новгород', 'пр. Голодных Ежей 2', '77777777777', 'chebureki@novzhur.com');

SELECT add_publication('Исследование в области математического анализа', 1, 1, '2013-02-09');
SELECT add_publication('Рецензия на исследование в области математического анализа', 4, 1, '2015-09-22');
SELECT add_publication('Практическое применение углерода', 2, 2, '2015-10-01');
SELECT add_publication('Исследование органичнских веществ', 1, 3, '2019-02-09');

SELECT add_authors_to_publication(1, ARRAY[1, 2, 3]);
SELECT add_authors_to_publication(2, ARRAY[3, 4]);
SELECT add_authors_to_publication(3, ARRAY[5, 6]);
SELECT add_authors_to_publication(4, ARRAY[7, 8]);

SELECT add_compilation('Сборник исследований в области математического анализа', 1, '2016-01-01');
SELECT add_publications_to_compilation(1, ARRAY[1, 2]);