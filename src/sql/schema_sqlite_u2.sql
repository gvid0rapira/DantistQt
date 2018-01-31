DROP TABLE IF EXISTS employee;

CREATE TABLE employee (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  lname VARCHAR(50) NOT NULL,
  fname VARCHAR(50) NOT NULL,
  mname VARCHAR(50),
  birth_date DATE,
  tab_num INTEGER NOT NULL,
  work_place varchar(30) NOT NULL
);
