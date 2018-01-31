DROP TABLE IF EXISTS visit_diagnosis;
DROP TABLE IF EXISTS visit;
--DROP TABLE IF EXISTS employee;
--DROP TABLE IF EXISTS diagnosis;

--CREATE TABLE employee (
--  id INTEGER PRIMARY KEY AUTOINCREMENT,
--  lname VARCHAR(50) NOT NULL,
--  fname VARCHAR(50) NOT NULL,
--  mname VARCHAR(50),
--  tab_num INTEGER NOT NULL,
--  work_place varchar(30) NOT NULL
--);

--CREATE TABLE diagnosis (
--  id INTEGER PRIMARY KEY AUTOINCREMENT,
--  name VARCHAR(120) NOT NULL
--);

CREATE TABLE visit (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  employee_id INTEGER NOT NULL,
  visit_date DATE NOT NULL,
  uet FLOAT NOT NULL,
  CONSTRAINT fk_visit_empl FOREIGN KEY (employee_id) REFERENCES employee(id)
    ON DELETE CASCADE
);
CREATE TABLE visit_diagnosis (
  visit_id INTEGER NOT NULL,
  diagnosis_id INTEGER NOT NULL,
  CONSTRAINT pk_visit_diagnosis PRIMARY KEY (visit_id, diagnosis_id),
  CONSTRAINT fk_visit FOREIGN KEY (visit_id) REFERENCES visit(id)
    ON DELETE CASCADE,
  CONSTRAINT fk_diagnosis FOREIGN KEY (diagnosis_id) REFERENCES diagnosis(id)
    ON DELETE CASCADE
);
