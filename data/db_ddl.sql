CREATE TABLE "students" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`group_id`	INTEGER,
	`name`	TEXT NOT NULL,
	`surname`	TEXT NOT NULL,
	`patronymic`	TEXT,
	FOREIGN KEY(`group_id`) REFERENCES groups(id)
);
CREATE TABLE `questions` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`theme_id`	INTEGER NOT NULL,
	`for_learn`	INTEGER NOT NULL DEFAULT 0,
	`question`	TEXT NOT NULL,
	FOREIGN KEY(`theme_id`) REFERENCES q_themes(id)
);
CREATE TABLE "q_themes" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`parent_id`	INTEGER NOT NULL DEFAULT 0,
	`name`	TEXT NOT NULL UNIQUE,
	FOREIGN KEY(`parent_id`) REFERENCES q_themes(id)
);
CREATE TABLE `options` (
	`send_report_by_email`	INTEGER NOT NULL DEFAULT 0
);
CREATE TABLE `groups` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`Code`	TEXT NOT NULL UNIQUE
);
CREATE TABLE "answers" (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`question_id`	INTEGER NOT NULL,
	`correct`	INTEGER NOT NULL DEFAULT 0,
	`answer`	TEXT,
	`comment`	TEXT,
	FOREIGN KEY(`question_id`) REFERENCES questions ( id )
);
CREATE TABLE `email_addreses` (
	`recipient_name`	TEXT NOT NULL,
	`address`	TEXT NOT NULL
);
CREATE VIEW vw_test_questions
AS
select
    id,
    theme_id,
    question
from questions
where
    for_learn = 0;
CREATE VIEW vw_learn_questions
AS
select
    id,
    theme_id,
    question
from questions
where
    for_learn > 0;
COMMIT;
