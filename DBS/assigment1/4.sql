-- chat
with body_doskoky as (
    select
        g.season_id,
        pr.game_id,
        pr.player1_id as player_id,

            sum(case when pr.event_msg_type = 'FIELD_GOAL_MADE' then 2 else 0 end) +
            sum(case when pr.event_msg_type = 'FREE_THROW' then 1 else 0 end) as points,

            sum(case when pr.event_msg_type = 'REBOUND' then 1 else 0 end) as rebounds
    from play_records pr
    join games g on g.id = pr.game_id
    -- where g.season_id = '22018'
    where g.season_id = '{{season_id}}'
    group by g.season_id, pr.game_id, pr.player1_id
),
    asistencie as (
        select
            season_id,
            pr.game_id,
            player_id,
            count(*) as assists
        -- player2 alebo player 3 dal asistenciu
        from (
            select pr.game_id, g.season_id, pr.player2_id as player_id from play_records pr
            join games g on g.id = pr.game_id
            where pr.event_msg_type = 'FIELD_GOAL_MADE' and pr.player2_id is not null

            union all

            select pr.game_id, g.season_id, pr.player3_id as player_id
            from play_records pr
            join games g on g.id = pr.game_id
            where pr.event_msg_type = 'FIELD_GOAL_MADE' and pr.player3_id is not null
        ) as pr
    group by season_id, game_id, player_id
),
    statistiky as (
    select
        b.season_id,
        b.game_id,
        b.player_id,
        b.points,
        b.rebounds,
        coalesce(a.assists, 0) as assist
    from body_doskoky b
    left join asistencie a on b.season_id = a.season_id and b.game_id = a.game_id and b.player_id = a.player_id
),
    triple_doubles as (
    select *,
           case when points >= 10
                and rebounds >= 10
                and assist >= 10
            then 1 else 0 end as triple_double
    from statistiky
),
    s_triple as (
    select *,
           row_number() over (partition by player_id order by game_id) -
           sum(triple_double) over (partition by player_id order by game_id) as grp
    from triple_doubles
),
    streaks as (
    select player_id, count(*) as dlzka_streaku
    from s_triple
    where triple_double = 1
    group by player_id, grp
),
    najdlhsi_streak as (
    select player_id, max(dlzka_streaku) as longest_streak
    from streaks
    group by player_id
)
select player_id, longest_streak
from najdlhsi_streak
order by longest_streak desc, player_id;
