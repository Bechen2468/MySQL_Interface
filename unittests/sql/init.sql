ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'testpass';
ALTER USER 'root'@'%'         IDENTIFIED WITH mysql_native_password BY 'testpass';

GRANT ALL PRIVILEGES ON *.* TO 'root'@'localhost' WITH GRANT OPTION;
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%'         WITH GRANT OPTION;





DROP PROCEDURE IF EXISTS reset_db;
DELIMITER $$


CREATE PROCEDURE reset_db()
BEGIN

DROP TABLE IF EXISTS test_string;
DROP TABLE IF EXISTS test_float;
DROP TABLE IF EXISTS test_double;
DROP TABLE IF EXISTS test_int;
DROP TABLE IF EXISTS test_bool;
DROP TABLE IF EXISTS test_datetime;
DROP TABLE IF EXISTS test_multi;



CREATE TABLE IF NOT EXISTS test_string (
value VARCHAR(16) PRIMARY KEY
);


CREATE TABLE IF NOT EXISTS test_float (
value FLOAT PRIMARY KEY
);



CREATE TABLE IF NOT EXISTS test_double (
value DOUBLE PRIMARY KEY
);


CREATE TABLE IF NOT EXISTS test_int (
value INT PRIMARY KEY
);


CREATE TABLE IF NOT EXISTS test_bool (
value TINYINT(1) PRIMARY KEY
);


CREATE TABLE IF NOT EXISTS test_datetime (
value DATETIME PRIMARY KEY
);


CREATE TABLE IF NOT EXISTS test_multi (
id INT PRIMARY KEY,
v1 DOUBLE,
v2 VARCHAR(128),
v3 TINYINT(1),
v4 DATETIME
);


INSERT INTO test_string
VALUES("a"),
("b"),
("c"),
("d"),
("e"),
("f"),
("g"),
("h"),
("i"),
("j");


INSERT INTO test_float
VALUES(0.0),
(1.1),
(2.2),
(3.3),
(4.4),
(5.5),
(6.6),
(7.7),
(8.8),
(9.9),
(11.0);


INSERT INTO test_double
VALUES(0.0),
(1.1),
(2.2),
(3.3),
(4.4),
(5.5),
(6.6),
(7.7),
(8.8),
(9.9),
(11.0);


INSERT INTO test_int
VALUES(1),
(2),
(3),
(4),
(5),
(6),
(7),
(8),
(9),
(10);


INSERT INTO test_bool
VALUES(1),
(0);


INSERT INTO test_datetime
VALUES('2025-01-01 01:00:00'),
('2025-01-02 02:00:00'),
('2025-01-03 03:00:00'),
('2025-01-04 04:00:00'),
('2025-01-05 05:00:00'),
('2025-01-06 06:00:00'),
('2025-01-07 07:00:00'),
('2025-01-08 08:00:00'),
('2025-01-09 09:00:00'),
('2025-01-10 10:00:00');


INSERT INTO test_multi
VALUES
(0, 0.0, "test", 1, '2025-01-01 01:00:00'),
(1, 0.5, "test", 0, '2025-01-01 01:00:00'),
(2, 1.0, "test", 1, '2025-01-01 01:00:00'),
(3, 1.5, "test", 0, '2025-01-01 01:00:00'),
(4, 2.0, "test", 1, '2025-01-01 01:00:00'),
(5, 2.5, "test", 0, '2025-01-01 01:00:00'),
(6, 3.0, "test", 1, '2025-01-01 01:00:00');



END$$


DELIMITER ;

