-- Очищаем таблицы. Из visit строки удалятся каскадно.
DELETE FROM employee;

INSERT INTO employee (lname, fname, mname, tab_num, work_place)
	VALUES ('Кукушкин', 'Андрей','Владимирович', 810, 'АСУ, А и ТМ');
INSERT INTO employee (lname, fname, mname, tab_num, work_place)
	VALUES ('Пиманов', 'Александр','Евгениевич', 615, 'АСУ, А и ТМ');
INSERT INTO employee (lname, fname, mname, tab_num, work_place)
	VALUES ('Безруков', 'Александр','Владимирович', 345, 'АСУ, А и ТМ');
INSERT INTO employee (lname, fname, mname, tab_num, work_place)
	VALUES ('Дебелый', 'Владимир','Николаевич', 645, 'АСУ, А и ТМ');
INSERT INTO employee (lname, fname, mname, tab_num, work_place)
	VALUES ('Струментов', 'Лев','Александрович', 214, 'АСУ, А и ТМ');
INSERT INTO employee (lname, fname, mname, tab_num, work_place)
	VALUES ('Воеводин', 'Денис','Константинович', 761, 'АСУ, А и ТМ');
INSERT INTO employee (lname, fname, mname, tab_num, work_place)
	VALUES ('Буйлов', 'Александр','Генадьевич', 598, 'АСУ, А и ТМ');
INSERT INTO employee (lname, fname, mname, tab_num, work_place)
	VALUES ('Попов', 'Максим','Андрианович', 867, 'АСУ, А и ТМ');

INSERT INTO visit(employee_id, visit_time)
	VALUES((SELECT id FROM employee WHERE tab_num = 867), '2012.08.21');
