#ifndef DBSTRUCTURE_H
#define DBSTRUCTURE_H

enum {
    Employee_Id = 0,
    Employee_Lname = 1,
    Employee_Fname = 2,
    Employee_Mname = 3,
    Employee_Birth_Date = 4,
    Employee_Tab_Num = 5,
    Employee_Work_Place = 6
};
enum {
    Diagnosis_Id = 0,
    Diagnosis_name = 1
};

enum {
    Visit_Id= 0,
    Visit_Employee_Id = 1,
    Visit_Date = 2,
    Visit_UET = 3
};

#endif // DBSTRUCTURE_H

