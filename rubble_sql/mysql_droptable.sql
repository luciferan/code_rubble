-- MySQL
-- database 에서 특정 테이블을 제외한 모든 테이블을 삭제

-- database 생성
--mysql> CREATE DATABASE target_db;
-- origin_db 백업
--C:>mysqldump -u [userid] -p origin_db > origin_db.sql
-- 백업을 indicator_db로 복원
--C:>mysql -u [userid] -p target_db < origin_db.sql

-- database 삭제
--mysql> DROP DATABASE target_db;

DELIMITER //
CREATE PROCEDURE DropTables()
BEGIN
	DECLARE done INT DEFAULT FALSE;

	DECLARE tableName VARCHAR(255);
    DECLARE constraintName VARCHAR(255);

	DECLARE constraint_cur CURSOR FOR
		SELECT `TABLE_NAME`, `CONSTRAINT_NAME`
		FROM information_schema.table_constraints
		WHERE `TABLE_SCHEMA` = DATABASE() AND `TABLE_NAME` NOT IN ('table_a', 'table_b', 'table_c') AND `CONSTRAINT_TYPE` = 'FOREIGN KEY';
	DECLARE view_cur CURSOR FOR
		SELECT `TABLE_NAME`
		FROM information_schema.tables
		WHERE `TABLE_SCHEMA` = DATABASE() AND `TABLE_NAME` NOT IN ('table_a', 'table_b', 'table_c') AND `TABLE_TYPE` = 'VIEW';
	DECLARE table_cur CURSOR FOR
		SELECT `TABLE_NAME`
		FROM information_schema.tables
		WHERE `TABLE_SCHEMA` = DATABASE() AND `TABLE_NAME` NOT IN ('table_a', 'table_b', 'table_c') AND `TABLE_TYPE` = 'BASE TABLE';

	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
    SET done = FALSE;
    OPEN constraint_cur;
    drop_contraint_loop: LOOP
		FETCH constraint_cur INTO tableName, constraintName;
		IF done THEN
			LEAVE drop_contraint_loop;
		END IF;

        SET @dropsql = CONCAT('ALTER TABLE ', tableName, ' DROP FOREIGN KEY ', constraintName);
        PREPARE stmt FROM @dropsql;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
    END LOOP;
    CLOSE constraint_cur;

	SET done = FALSE;
    OPEN view_cur;
    drop_loop: LOOP
		FETCH view_cur INTO tableName;
        IF done THEN
			LEAVE drop_loop;
		END IF;

        SET @dropsql = CONCAT('DROP VIEW ', tableName);
        PREPARE stmt FROM @dropsql;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
    END LOOP;
    CLOSE view_cur;

    SET done = FALSE;
    OPEN table_cur;
    drop_loop: LOOP
		FETCH table_cur INTO tableName;
        IF done THEN
			LEAVE drop_loop;
		END IF;

        SET @dropsql = CONCAT('DROP TABLE ', tableName);
        PREPARE stmt FROM @dropsql;
        EXECUTE stmt;
        DEALLOCATE PREPARE stmt;
    END LOOP;
    CLOSE table_cur;
END //
DELIMITER ;

CALL DropTables();
DROP PROCEDURE DropTables;