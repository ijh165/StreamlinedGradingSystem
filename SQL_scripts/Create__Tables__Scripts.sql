USE[c275g07A]
GO

----- TEAM INNOVATORS
---- Created by Kayhan Dehghani Mohammadi and Savio Yin Kit Lam
---- Modified by Ivan Jonathan Hoo




------- Account Table
----------------------
----------------------

CREATE TABLE Account(
	accountID  INT IDENTITY(1,1),
	employeeID VARCHAR(12) NOT NULL,
	firstName VARCHAR(50) NOT NULL,
	middleName VARCHAR(50),
	lastName VARCHAR(50) NOT NULL,
	userName VARCHAR(8) NOT NULL,
	[password] VARCHAR(20) NOT NULL,
	isPasswordTemp BIT NOT NULL,
	islocked BIT NOT NULL,
	attemptCtr INT NOT NULL DEFAULT 0,
	isAdminAssist BIT NOT NULL,
	isSystemAdmin BIT NOT NULL,
	isAdmin BIT NOT NULL,
	isTA BIT NOT NULL,
	isInstructor BIT NOT NULL,
	PRIMARY KEY (accountID),
	CONSTRAINT unique_employee UNIQUE(employeeID),
	CONSTRAINT unique_username UNIQUE(userName)
)
GO


------- Course Table
----------------------
----------------------

CREATE TABLE Course(
	courseID INT IDENTITY(1,1),
	courseNumber VARCHAR(50) NOT NULL,
	startDate DATE NOT NULL,
	endDate DATE NOT NULL,
	courseName VARCHAR(50) NOT NULL,
	instructorEmployeeID VARCHAR(12),
	PRIMARY KEY (courseID),
	CONSTRAINT unique_term_course UNIQUE(courseNumber,startDate,endDate,courseName)
)
GO




------- TA Table
----------------------
----------------------

CREATE TABLE TA(
	TAID INT IDENTITY(1,1),
	accountID INT NOT NULL,
	courseID INT NOT NULL,
	PRIMARY KEY (TAID),
	FOREIGN KEY (accountID) REFERENCES Account ON DELETE CASCADE,
	FOREIGN KEY (courseID) REFERENCES Course ON DELETE CASCADE,
	CONSTRAINT unique_TA_course UNIQUE(accountID,courseID)
)
GO



------- Activity Table
----------------------
----------------------

CREATE TABLE Activity(
	activityID INT IDENTITY(1,1),
	courseID INT NOT NULL,
	activityName VARCHAR(250) NOT NULL,
	activityType VARCHAR(50) NOT NULL,
	dueDateTime SMALLDATETIME,
	[language] VARCHAR(50) NOT NULL,
	pathToSubmissions VARCHAR(250),
	numberOfTests INT,
	pathToSolution VARCHAR(250) NOT NULL, 

	PRIMARY KEY (activityID),
	FOREIGN KEY (courseID) REFERENCES Course ON DELETE NO ACTION, 
	CONSTRAINT unique_course_activity UNIQUE(courseID,activityName)
)
GO

-------  Rubric Item Table
----------------------
----------------------

CREATE TABLE RubricItem(
	rubricItemID INT IDENTITY(1,1),
	activityID INT NOT NULL,
	rubricItemNumber  INT NOT NULL,
	rubricItemText VARCHAR(250) NOT NULL,
	rubricItemValue REAL NOT NULL,

	PRIMARY KEY (rubricItemID),
	FOREIGN KEY (activityID) REFERENCES Activity ON DELETE CASCADE,
	CONSTRAINT uniq_activity_rebricItem UNIQUE(activityID,rubricItemNumber)
)
GO


-------  BonusPenaltyType Table
----------------------
----------------------

CREATE TABLE BonusPenaltyType(
	bonusPenaltyTypeID INT IDENTITY(1,1),
	[type] VARCHAR(50) NOT NULL,

	PRIMARY KEY (bonusPenaltyTypeID)
)
GO



-------  BonusPenalty Table
----------------------
----------------------

CREATE TABLE BonusPenalty(
	bonusPenaltyID INT IDENTITY(1,1),
	activityID INT NOT NULL,
	bonusPenaltyTypeID INT NOT NULL,
	[hours] REAL NOT NULL,
	amount REAL NOT NULL,

	PRIMARY KEY (bonusPenaltyID),
	FOREIGN KEY (bonusPenaltyID) REFERENCES BonusPenaltyType,
	FOREIGN KEY (activityID) REFERENCES Activity ON DELETE CASCADE,
	CONSTRAINT unique_activity_bonus UNIQUE(activityID,bonusPenaltyTypeID)
)
GO



-------  ProgrammingTest Table
----------------------
----------------------

CREATE TABLE ProgrammingTest(
	testID INT IDENTITY(1,1),
	activityID INT NOT NULL,
	testNumber INT NOT NULL,
	numberOfInputFiles INT,
	numberOfOutputFiles INT,
	environment VARCHAR(50),

	PRIMARY KEY (testID),
	FOREIGN KEY (activityID) REFERENCES Activity ON DELETE CASCADE,
	CONSTRAINT unique_activity_test UNIQUE(activityID,testNumber)
)
GO



------- Grade Table
----------------------
----------------------

CREATE TABLE Grade(
	gradeID INT IDENTITY(1,1),
	studentID VARCHAR(50) NOT NULL,
	rubricItemID INT NOT NULL,
	earnedGrade REAL NOT NULL,

	PRIMARY KEY (gradeID),
	FOREIGN KEY (rubricItemID) REFERENCES RubricItem ON DELETE CASCADE,
	CONSTRAINT unique_student_rubricItem UNIQUE(studentID,rubricItemID)
)
GO



------- Test File Type Table
----------------------
----------------------

CREATE TABLE TestFileType(
	fileTypeID INT IDENTITY(1,1),
	[type] VARCHAR(50) NOT NULL,

	PRIMARY KEY (fileTypeID)
)
GO


------- File Table
----------------------
----------------------

CREATE TABLE [File](
	fileID INT IDENTITY(1,1),
	activityID INT NOT NULL,
	fileTypeID INT NOT NULL,
	testNumber INT NOT NULL,
	pathToFile VARCHAR(250) NOT NULL,

	PRIMARY KEY (fileID),
	FOREIGN KEY (activityID) REFERENCES Activity ON DELETE CASCADE,
	FOREIGN KEY (fileTypeID) REFERENCES TestFileType ON DELETE CASCADE,
	CONSTRAINT unique_activbity_fileType UNIQUE(activityID,fileTypeID)
)
GO



------- Student Submission Table
----------------------
----------------------

CREATE TABLE StudentSubmission(
	studentSubmissionID INT IDENTITY(1,1),
	activityID INT NOT NULL,
	studentID VARCHAR(50) NOT NULL,

	PRIMARY KEY (studentSubmissionID),
	FOREIGN KEY (activityID) REFERENCES Activity ON DELETE CASCADE,
	CONSTRAINT unique_student_activity UNIQUE(activityID)
)
GO


------- Group Submission Table
----------------------
----------------------

CREATE TABLE GroupSubmission(
	groupSubmissionID INT IDENTITY(1,1),
	activityID INT NOT NULL,
	groupName VARCHAR(50) NOT NULL,

	PRIMARY KEY (groupSubmissionID),
	FOREIGN KEY (activityID) REFERENCES Activity ON DELETE CASCADE,
	CONSTRAINT unique_group_activity UNIQUE(activityID)
)
GO


------- Answer Key Response Table
----------------------
----------------------

CREATE TABLE AnswerKeyResponse(
	answerKeyResponseID INT IDENTITY(1,1),
	activityID INT NOT NULL,
	questionNumber INT NOT NULL,
	response VARCHAR(50) NOT NULL,
	[weight] REAL NOT NULL, 

	PRIMARY KEY (answerKeyResponseID),
	FOREIGN KEY (activityID) REFERENCES Activity ON DELETE CASCADE,
	CONSTRAINT unique_activity_question UNIQUE(activityID,questionNumber)
)
GO


------- Student Table
----------------------
----------------------

CREATE TABLE Student(
	studentID VARCHAR(50),
	courseID INT NOT NULL,
	studentName VARCHAR(250) NOT NULL,

	PRIMARY KEY (studentID),
	FOREIGN KEY (courseID) REFERENCES Course ON DELETE CASCADE,
	CONSTRAINT unique_course_student UNIQUE(courseID)
)
GO


