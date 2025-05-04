-- v_combat_state: Zobrazuje aktuálne kolo, zoznam aktívnych postśv a ich zostśvajúcu AP
create or replace view v_combat_state as
select
    cp.combat_id,
    cr.round_number as current_round,
    ch.id as character_id,
    ch.name as character_name,
    cp.current_ap,
    cp.status
from combat_participants cp
join characters ch on cp.character_id = ch.id
join (
    select combat_id, max(round_number) as round_number
    from combat_rounds
    group by combat_id
) cr on cp.combat_id = cr.combat_id
where cp.current_ap > 0 or cp.status = 'alive';

-- v_most_damage: Zaraduje postavy podla celkoveho sposobeneho poskodenia vo vsetkych bojoch.
create or replace view v_most_damage as
select
    ch.id as character_id,
    ch.name as character_name,
    coalesce(sum(cd.damage), 0) as total_damage
from characters ch
left join combat_diary cd on ch.id = cd.actor_id and cd.success = true and cd.damage > 0
group by ch.id, ch.name
order by total_damage desc;

-- v_strongest_characters: Zoznam postav zoradenych podla suhrnnych ukazovatelov vykonu (napr. sposobene poskodenie, zostavajuce zdravie).
create or replace view v_strongest_characters as
select
    ch.id as character_id,
    ch.name as character_name,
    coalesce(sum(cd.damage), 0) as total_damage,
    ch.current_hp,
    -- ch.max_hp,
    (coalesce(sum(cd.damage), 0) + ch.current_hp) as performance_score
from characters ch
left join combat_diary cd on ch.id = cd.actor_id and cd.success = true
group by ch.id, ch.name, ch.current_hp
order by performance_score desc;

-- v_combat_damage: Sumarizuje celkove poskodenie sposobene v kazdej bojovej relacii.
create or replace view v_combat_damage as
select
    cd.combat_id,
    coalesce(sum(cd.damage), 0) as total_combat_damage
from combat_diary cd
where cd.success = true and cd.damage >= 0
group by cd.combat_id
order by cd.combat_id;

-- v_spell_statistics: Statistika pouzivania kuziel a poskodenia.
create or replace view v_spell_statistics as
select
    sp.id as spell_id,
    sp.name as spell_name,
    count(cd.id) as times_used,
    coalesce(sum(cd.damage), 0) as total_damage,
    round(coalesce(avg(cd.damage), 0), 2) as average_damage
from spells sp
left join combat_diary cd on sp.id = cd.spell_id and cd.success = true
group by sp.id, sp.name
order by times_used desc;
