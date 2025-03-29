with zapasy as (
    select
        pr.game_id,
        g.season_id,
        pr.event_msg_type,
        pr.score_margin,

        -- primarny hrac
        pr.player1_id as player1_id,
        pr.player1_team_id as player1_team_id,

        -- asistent
        pr.player2_id as player2_id,
        pr.player2_team_id as player2_team_id
    from play_records pr
    join games g on g.id = pr.game_id
    where g.season_id = '22017'
    -- where g.season_id = '{{season_id}}'
),
-- unikatne id
    hrac_zapasy as (
    select player1_id as player_id, player1_team_id as team_id, game_id, event_msg_type, score_margin, 'player1' as pozicia from zapasy
    where player1_id is not null and player1_team_id is not null

    union all

    select player2_id as player_id, player2_team_id as team_id, game_id, event_msg_type, score_margin, 'player2' as pozicia from zapasy
    where player2_id is not null and player2_team_id is not null
),
top5_hraci as (
    select
        player_id,
        count(distinct team_id) - 1 as pocet_zmien_timov -- -1 lebo sa nepocita tim, v ktorom je, ale je to jedno v tomto pripade, i guess
    from hrac_zapasy
    group by player_id
    having count(distinct team_id) > 1
    order by pocet_zmien_timov desc
    limit 5
),
vypocet as (
    select
        h.player_id,
        h.team_id,
        h.game_id,

        case
            when h.pozicia = 'player1' and h.event_msg_type = 'FIELD_GOAL_MADE' and score_margin is not null then 2 -- musel skorovat aby sa to pocitalo
            else 0
        end as body_2,

        case
            when h.pozicia = 'player1' and h.event_msg_type = 'FREE_THROW' and score_margin is not null then 1
            else 0
        end as body_1,

        case
            when h.pozicia = 'player2' AND h.event_msg_type = 'FIELD_GOAL_MADE' then 1 -- lebo iba player2 dava asistencia
            else 0
        end as asistencia
    from hrac_zapasy h
    where h.player_id in (select player_id from top5_hraci)
),
statistiky as (
    select
        player_id,
        team_id,
        count (distinct game_id) as games,
        sum(body_2 + body_1) as total_points,
        sum(asistencia) as total_assists
    from vypocet
    group by player_id, team_id
),
final as (
    select
        player_id,
        team_id,
        games,
        round(total_points * 1.0 / nullif(games, 0), 2) as PPG,
        round(total_assists * 1.0 / nullif(games, 0), 2) as APG
    from statistiky
)
select
    f.player_id,
    p.first_name,
    p.last_name,
    f.team_id,
    t.full_name as team_name,
    f.PPG,
    f.APG,
    f.games
from final f
join players p on p.id = f.player_id
join teams t on t.id = f.team_id
order by f.player_id, f.team_id;
