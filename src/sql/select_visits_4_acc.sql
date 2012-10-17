SELECT DISTINCT e.lname, e.fname, e.mname, e.tab_num, e.work_place, v.visit_time,
    count(e.id)
    FROM employee e, visit v
    WHERE (v.visit_time BETWEEN datetime('2012-10-01 00:00:00') AND datetime('2012-10-20 23:59:59'))
        AND v.employee_id = e.id
    GROUP BY e.lname
