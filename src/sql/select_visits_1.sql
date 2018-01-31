SELECT v.id, e.lname, e.fname, e.mname, v.visit_date, v.uet, 
    (SELECT group_concat(d.name) FROM visit_diagnosis vd, diagnosis d 
             WHERE vd.visit_id = v.id AND d.id = vd.diagnosis_id) AS diagnosises
FROM visit v, employee e
WHERE e.id = v.employee_id
