CREATE TABLE employee (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  lname VARCHAR(50) NOT NULL,
  fname VARCHAR(50) NOT NULL,
  mname VARCHAR(50),
  tab_num INTEGER NOT NULL,
  work_place varchar(30) NOT NULL
);

CREATE TABLE visit (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  employee_id INTEGER,
  visit_time INTEGER NOT NULL,
  CONSTRAINT fk_visit_empl FOREIGN KEY (employee_id) REFERENCES employee(id)
    ON DELETE CASCADE
);
