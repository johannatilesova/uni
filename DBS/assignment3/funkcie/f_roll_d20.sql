-- funkcia na hod kocou d20
CREATE OR REPLACE FUNCTION f_roll_d20()
RETURNS INTEGER AS $$
DECLARE
    v_roll_result INTEGER;
BEGIN
    -- Náhodný hod od 1 po 20
    v_roll_result := FLOOR(RANDOM() * 20) + 1;
    RETURN v_roll_result;
END;
$$ LANGUAGE plpgsql;

SELECT * FROM class;