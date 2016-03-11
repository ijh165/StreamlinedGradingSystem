----- TEAM INNOVATORS
---- Created by Ivan Jonathan Hoo

SET IDENTITY_INSERT Account ON;

INSERT INTO Account (accountID,
                    employeeID,
                    firstName,
                    middleName,
                    lastName,
                    userName,
                    password,
                    isPasswordTemp,
                    islocked,
                    isAdminAssist,
                    isSystemAdmin,
                    isAdmin,
                    isTA,
                    isInstructor,
                    attemptCtr) 
VALUES (1, '165276387498', 'Yuno', 'SuperYandere', 'Gasai', 'ygasai', 'abc123', 0, 0, 0, 1, 0, 0, 0, 0);

SET IDENTITY_INSERT Account OFF;
