# At first, add database 'systemdb' via:
#
# CREATE DATABASE systemdb; 
#
# And then run following commands for
# 'systemdb' database
#
# Table creating:
CREATE TABLE users(
	id SERIAL NOT NULL,
	role TEXT,
	login TEXT,
	password TEXT,
	name TEXT,
	surname TEXT,
	PRIMARY KEY(id));

CREATE TABLE groups(
	id SERIAL NOT NULL,
	teacherid INT,
	name TEXT,
	PRIMARY KEY(id),
	FOREIGN KEY(teacherid) REFERENCES users(id));

CREATE TABLE group_by_user(
	userid INT,
	groupid INT,
	FOREIGN KEY (userid) REFERENCES users(id),
	FOREIGN KEY (groupid) REFERENCES groups(id));

CREATE TABLE tests(
	id SERIAL NOT NULL,
	name TEXT,
	subject TEXT,
	planneddate DATE,
	teacherid INT,
	PRIMARY KEY(id),
	FOREIGN KEY (teacherid) REFERENCES users(id));

CREATE TABLE results(
	id SERIAL NOT NULL,
	percent INT,
	studentid INT,
	testid INT,
	PRIMARY KEY (id), 
	FOREIGN KEY (studentid) REFERENCES users(id),
	FOREIGN KEY (testid) REFERENCES tests(id));

CREATE TABLE tasks(
	id SERIAL NOT NULL,
	subject TEXT,
	task TEXT,
	answeroptions TEXT,
	answer TEXT,
	theme TEXT,
	teacherid INT,
	PRIMARY KEY(id),
	FOREIGN KEY (teacherid) REFERENCES users(id));
CREATE TABLE test_by_task(
	testid INT,
	taskid INT,
	FOREIGN KEY (testid) REFERENCES tests(id),
	FOREIGN KEY (taskid) REFERENCES tasks(id));

# Now, we have to add the first user
# You can replace 'admin' with any login/password
# you want

INSERT INTO users (login, password, role)
VALUES ('admin', 'admin', 'admin+');