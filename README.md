# Streamlined Grading System
By Team Innovators

# Description
Term project for CMPT275 at SFU.

Streamlined Grading System is a windows application supposedly to be used by Instructors and TAs so that they can efficiently grade student's assignments. Our team coded this software in C++ with Qt framework (using QtCreator IDE). Additionally our team also created an MS SQL database to store persistent data. This software is originally developed using Subversion but I put it here on GitHub after the development has finished.

# Team Members:
- Chi Chung Chan (Cyrus), cyrusc@sfu.ca 
- Ivan Jonathan Hoo, ihoo@sfu.ca 
- Jae Jun Yang (Darren), yangjaey@sfu.ca 
- Kayhan Dehghani Mohammadi, kdehghan@sfu.ca 
- Pak Lun Hoi (Joel), phoi@sfu.ca 
- Phuong Tran (Frank), ftran@sfu.ca 
- Raymond Chan, rca71@sfu.ca 
- Saad Alkhalifah, salkhali@sfu.ca 
- Yin Kit, Lam (Savio), yklam@sfu.ca

# Prerequisites
- Windows OS
- QtCreator IDE and Qt 5.4 (https://www.qt.io/download-open-source/) must be installed
- SQL Server (2008 or above) and SQL Server Management Studio must be installed

# To run the software

1) Create a database named 'c275g07A' in SQL Server Management Studio

2) Execute 'Create__Tables__Scripts.sql' (this sql script creates the tables in the database) 

3) Execute 'Init__SysAdmin.sql' (this sql script inserts one system admin data into the Account table as you would need at least one system admin to login and create accounts)

4) Go to 'SGS' folder

5) Open 'SGS.pro' (using QtCreator IDE)  

6) Under Control/DatabaseAPI open 'dbmanager.cpp' (you need to edit some code in it before you can properly run this software)</li>

7) In line 24 of 'dbmanager.cpp', change  

> connectionString = QString("DRIVER={SQL SERVER};" "SERVER=cypress.csil.sfu.ca;" "UID=c275g07;" "PWD=672t2hAjL7YN6mEh");  

into

> connectionString = QString("DRIVER={SQL Server};" "SERVER=serverName;" "DATABASE=c275g07A;" "UID=userName;" "PWD=password;")  

where 'serverName' is your own database server name, 'userName' is your own database user name, and 'password' is the password 

8) Click 'Run' (the "play" button)

9) Enjoy (you can login with username 'ygasai' and password 'abc123')

# Software Development Process
1. Requirement specification
2. Design
3. Implementation and coding
4. Testing and debugging
5. Deployment
