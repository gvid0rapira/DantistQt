SELECT e.lname, e.fname, e.mname, v.visit_time FROM employee e, visit v
--    WHERE (v.visit_time > '09.10.2012 14:20' AND v.visit_time < '10.10.2012 10:00') AND v.employee_id = e.id;
    WHERE (datetime(v.visit_time) BETWEEN  datetime('2012-10-09 14:20:00') AND datetime('2012-10-10 10:00:00')) AND v.employee_id = e.id;

