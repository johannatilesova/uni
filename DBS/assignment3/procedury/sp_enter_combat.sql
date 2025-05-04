-- 3. sp_enter_combat
-- Zaregistruje postavu do prebiehajucej bojovej relacie a inicializuje
-- parametre specificke pre boj, ako je aktualny AP a cıslo kola

create or replace function sp_enter_combat (
    p_combat_id integer,
    p_character_id integer
) returns void as $$
DECLARE
    v_max_ap INTEGER;
    v_new_initiative_order INTEGER;
begin
    -- Get the character's maximum AP
    SELECT max_ap INTO v_max_ap
    FROM characters
    WHERE id = p_character_id;

    -- Determine initiative order (simple: max current initiative + 1)
    SELECT COALESCE(MAX(initiative_order), 0) + 1 INTO v_new_initiative_order
    FROM combat_participants
    WHERE combat_id = p_combat_id;

    -- Insert the character into combat_participants
    INSERT INTO combat_participants (combat_id, character_id, current_ap, status, initiative_order)
    VALUES (p_combat_id, p_character_id, v_max_ap, 'alive', v_new_initiative_order);

    -- Optionally: update characters table to mark the character as "in_combat"
    UPDATE characters
    SET in_combat = true
    WHERE id = p_character_id;
end;
$$ language plpgsql;