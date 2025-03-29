select player_id, first_name, last_name, period, period_time
from (select pl.id as player_id,
            pl.first_name,
            pl.last_name,
            rec.period,
            rec.pctimestring as period_time,

            rec.event_msg_type as udalost_po,
            rec.game_id,

            lead(rec.event_msg_type, -1) over (partition by rec.game_id order by event_number) as udalost_pred,
            lead(rec.player1_id, -1) over (partition by rec.game_id order by event_number) as id_hraca

        from players pl join play_records rec
            on pl.id = rec.player1_id) as tabulka

where game_id = {{game_id}}
-- where game_id = 22000529
    and player_id = id_hraca
    and udalost_pred = 'REBOUND'
    and udalost_po = 'FIELD_GOAL_MADE'

order by period, period_time desc, player_id;
