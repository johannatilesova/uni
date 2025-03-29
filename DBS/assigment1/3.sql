with zakladna_tabulka as (
    select pr.game_id,
           pr.player1_id,
           event_number,
           event_msg_type,
           score_margin,
           score
    from play_records pr
    -- where game_id = 21701185
    where game_id = {{game_id}}
    order by event_number
    ),
    vypocty as (
        select t.player1_id,
               count(case when event_msg_type = 'FIELD_GOAL_MADE' then 1 end) over (partition by player1_id) AS total_made,
               count(case when event_msg_type = 'FIELD_GOAL_MISSED' then 1 end) over (partition by player1_id) AS missed_shots,
               count(case when event_msg_type = 'FREE_THROW' and score_margin is not null then 1 end) over (partition by player1_id) as FTM,
               count(case when event_msg_type = 'FREE_THROW' and score_margin is null then 1 end) over (partition by player1_id) as missed_free_throws

        from zakladna_tabulka t
    ),
    vyvolene_hry as (
        select event_number, player1_id, game_id, score, score_margin, event_msg_type,
               coalesce(nullif(score_margin, 'TIE')::int, 0) as aktualne_skore,

               coalesce(
                    lag(nullif(score_margin, 'TIE')::int) over (partition by game_id order by event_number),
                0) as score_predtym

        from play_records
        where score is not null
        order by event_number
    ),
    vypocet_bodov as (
        select vh.player1_id, vh.game_id,
               count(case when abs(score_predtym - aktualne_skore) = 2 then 1 end) as "2PM",
               count(case when abs(score_predtym - aktualne_skore) = 3 then 1 end) as "3PM",

               count(case when abs(score_predtym - aktualne_skore) = 2 then 1 end) * 2 +
               count(case when abs(score_predtym - aktualne_skore) = 3 then 1 end) * 3 +
               count(case when event_msg_type = 'FREE_THROW' then 1 end) * 1 as points

        from vyvolene_hry vh
        group by vh.player1_id, vh.game_id
    )
select distinct vb.player1_id,
                p.first_name,
                p.last_name,
                points,
                "2PM",
                "3PM",
                v.missed_shots,

                coalesce(round(
                    ("2PM" + "3PM")::decimal / nullif(v.total_made + v.missed_shots, 0) * 100, 2
                ), 0)
                    as shooting_percentage,

                v.FTM,
                v.missed_free_throws,

                coalesce(
                        round (
                    FTM::decimal / nullif(FTM + missed_free_throws, 0) * 100, 2
                ), 0)
                    as FT_percentage

from vypocet_bodov vb
    join players p on player1_id = p.id
    join zakladna_tabulka t on t.player1_id = vb.player1_id
    join vypocty v on t.player1_id = v.player1_id
-- where vb.game_id = 21701185
where vb.game_id = {{game_id}}
order by points desc, shooting_percentage desc, FT_percentage desc, player1_id;