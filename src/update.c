/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/* command procedures needed */
DECLARE_DO_FUN(do_quit		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_follow	);

/*
 * Local functions.
 */
int	hit_gain	args( ( CHAR_DATA *ch ) );
int	mana_gain	args( ( CHAR_DATA *ch ) );
int	move_gain	args( ( CHAR_DATA *ch ) );
void	mobile_update	args( ( void ) );
void	weather_update	args( ( void ) );
void	char_update	args( ( void ) );
void	obj_update	args( ( void ) );
void	aggr_update	args( ( void ) );
void	riot_update	args( (void ) );
void	age_update	args( ( void ) );
/* used for saving */

int	save_number = 0;



/*
 * Advancement stuff.
 */
void advance_level( CHAR_DATA *ch, bool hide )
{
    char buf[MAX_STRING_LENGTH];
    int add_hp;
    int add_mana;
    int add_move;
    int add_prac;
    int int_mod;

    ch->pcdata->last_level = 
	( ch->played + (int) (current_time - ch->logon) ) / 3600;

    sprintf( buf, "the %s",
	title_table [ch->class] [ch->level] [ch->sex == SEX_FEMALE ? 1 : 0] );
    set_title( ch, buf );

    add_hp	= con_app[get_curr_stat(ch,STAT_CON)].hitp + number_range(
		    class_table[ch->class].hp_min,
		    class_table[ch->class].hp_max );

    int_mod = get_curr_stat(ch,STAT_INT) - 2;	

   if (!str_cmp(class_table[ch->class].name,"cleric") )
	add_mana = UMIN(1 + number_range(int_mod/2, int_mod), 16);
   else if (!str_cmp(class_table[ch->class].name,"channeler") )
	add_mana = UMIN(1 + number_range(int_mod*2/3, int_mod), 22);
   else if (!str_cmp(class_table[ch->class].name,"necromancer") )
	add_mana = UMIN(1 + number_range(int_mod*2/3, int_mod), 20);
   else if (!str_cmp(class_table[ch->class].name,"elementalist") )
	add_mana = UMIN(1 + number_range(int_mod*2/3, int_mod), 19);
   else if (!str_cmp(class_table[ch->class].name,"paladin") )
	add_mana = UMIN(1 + number_range(int_mod/3, int_mod), 16);
   else if (!str_cmp(class_table[ch->class].name,"nightwalker") )
	add_mana = UMIN(1 + number_range(int_mod/3, int_mod*3/4), 15);
   else if (!str_cmp(class_table[ch->class].name,"anti-paladin") )
	add_mana = UMIN(1 + number_range(int_mod/3, int_mod*3/4), 15);
    else add_mana = UMIN(1 + number_range(int_mod/3,int_mod/2), 11);

    add_move	= number_range( 1, (get_curr_stat(ch,STAT_CON)
				  + get_curr_stat(ch,STAT_DEX))/6 );
    add_prac	= wis_app[get_curr_stat(ch,STAT_WIS)].practice;

    if (!str_cmp(class_table[ch->class].name,"warrior"))
    {
	add_hp += number_range(1,4);
	add_hp = UMIN(add_hp,24 );
    }
    else if (!str_cmp(class_table[ch->class].name,"paladin"))
	add_hp = UMIN(add_hp,17 );
    else if (!str_cmp(class_table[ch->class].name,"anti-paladin"))
	add_hp = UMIN(add_hp, 17 );
    else if (!str_cmp(class_table[ch->class].name,"cleric"))
	add_hp = UMIN(add_hp, 15);
    else if (!str_cmp(class_table[ch->class].name,"ranger"))
	add_hp = UMIN(add_hp, 17);
    else if (!str_cmp(class_table[ch->class].name,"necromancer"))
	add_hp = UMIN(add_hp,12);
    else if (!str_cmp(class_table[ch->class].name,"elementalist"))
	add_hp = UMIN(add_hp,11);
    else if (!str_cmp(class_table[ch->class].name,"channeler"))
	add_hp = UMIN(add_hp,12);
    else if (!str_cmp(class_table[ch->class].name,"monk"))
	add_hp = UMIN(add_hp,16 );
    else if (!str_cmp(class_table[ch->class].name,"thief"))
	add_hp = UMIN(add_hp,15 );
    else if (!str_cmp(class_table[ch->class].name,"nightwalker"))
	add_hp = UMIN(add_hp, 17);
    else
	add_hp = UMIN(add_hp,15);

    add_hp	= UMAX(  2, add_hp   );
    add_mana	= UMAX(  2, add_mana );
    add_move	= UMAX(  6, add_move );

    ch->max_hit 	+= add_hp;
    ch->max_mana	+= add_mana;
    ch->max_move	+= add_move;
    ch->practice	+= add_prac;
    if (ch->level == 5 || ch->level == 10 || ch->level == 15 || ch->level == 20 ||
	ch->level == 25 || ch->level == 30 || ch->level == 35 || ch->level == 40 ||
	ch->level == 45 || ch->level == 50)
         ch->train		+= 1;

    ch->pcdata->perm_hit	+= add_hp;
    ch->pcdata->perm_mana	+= add_mana;
    ch->pcdata->perm_move	+= add_move;

    if (!hide)
    {
	sprintf(buf,
"You gain %d/%d hp, %d/%d mana, %d/%d move, and %d/%d practices.\n\r",
add_hp,ch->max_hit, add_mana, ch->max_mana,
add_move, ch->max_move, add_prac,ch->practice);
    send_to_char(buf,ch);
    }
    return;
}   



void gain_exp( CHAR_DATA *ch, int gain )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

    /*ch->exp = UMAX( exp_per_level(ch,ch->pcdata->points), ch->exp + gain );*/
    if (ch->level < LEVEL_HERO)
	ch->exp += gain;

    if (ch->exp > ch->exp_total)
	ch->exp_total = ch->exp;

    while ( ch->level < LEVEL_HERO && ch->exp >= 
        exp_per_level(ch) * (ch->level) )
    {
	send_to_char( "You raise a level!!  ", ch );
	ch->level += 1;
	sprintf(buf,"%s gained level %d",ch->name,ch->level);
	log_string(buf);
	sprintf(buf,"$N has attained level %d!",ch->level);
	wiznet(buf,ch,NULL,WIZ_LEVELS,0,0);
	advance_level(ch,FALSE);
	save_char_obj(ch);
    }

    return;
}



/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;

    if (ch->in_room->vnum == ROOM_VNUM_NIGHTWALK
	|| ch->in_room->vnum == 2901)
    {
	number = gsn_shadowplane;
	damage_old(ch,ch,40,number,DAM_NEGATIVE,TRUE);
	return 0;
    }

    if (is_affected(ch,gsn_atrophy) || is_affected(ch,gsn_prevent_healing))
	return 0;

    if (!IS_NPC(ch))
    {
	if (ch->pcdata->condition[COND_STARVING] > 6)
		return 0;
	if (ch->pcdata->condition[COND_DEHYDRATED] > 4)
		return 0;
    }

    if ( IS_NPC(ch) )
    {
	gain =  5 + ch->level;
 	if (IS_AFFECTED(ch,AFF_REGENERATION))
	    gain *= 2;

	switch(ch->position)
	{
	    default : 		gain /= 2;			break;
	    case POS_SLEEPING: 	gain = 3 * gain/2;		break;
	    case POS_RESTING:  					break;
	    case POS_FIGHTING:	gain /= 3;		 	break;
 	}

	
    }
    else
    {
	gain = UMAX(3,get_curr_stat(ch,STAT_CON) -3 + ch->level/2); 
	gain += class_table[ch->class].hp_max;
 	number = number_percent();
	if (number < get_skill(ch,gsn_fast_healing))
	{
	    gain += number * gain / 100;
	    if (ch->hit < ch->max_hit)
		check_improve(ch,gsn_fast_healing,TRUE,8);
	}

	switch ( ch->position )
	{
	    default:	   	gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:  	gain /= 2;			break;
	    case POS_FIGHTING: 	gain /= 6;			break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   == 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    gain /= 2;

    }

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = (gain * 7/5);

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
	gain /= 8;

    if (ch->position == POS_SLEEPING && get_skill(ch,gsn_dark_dream) > 5)
    {
	if (number_percent() < get_skill(ch,gsn_dark_dream))
	{
	check_improve(ch,gsn_dark_dream,TRUE,7);
		gain *=3;
		gain /=2;
	}
    }

    if (IS_AFFECTED(ch,AFF_HASTE) )
	gain /=2;
    if ( IS_AFFECTED(ch,AFF_SLOW))
	{
	gain *= 17;
	gain /= 10 ;
	}

    if (is_affected(ch,gsn_camp))
    {
	if (number_percent() < ch->pcdata->learned[gsn_camp])
	gain *=2;
    }

    return UMIN(gain, ch->max_hit - ch->hit);
}



int mana_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;
    if (is_affected(ch,gsn_atrophy) || is_affected(ch,gsn_prevent_healing))
	return 0;

    if (!IS_NPC(ch))
    {
	if (ch->pcdata->condition[COND_STARVING] > 6)
		return 0;
	if (ch->pcdata->condition[COND_DEHYDRATED] > 4)
		return 0;
    }

    if ( IS_NPC(ch) )
    {
	gain = 5 + ch->level;
	switch (ch->position)
	{
	    default:		gain /= 2;		break;
	    case POS_SLEEPING:	gain = 3 * gain/2;	break;
   	    case POS_RESTING:				break;
	    case POS_FIGHTING:	gain /= 3;		break;
    	}
    }
    else
    {
	gain = (get_curr_stat(ch,STAT_WIS)/2 - 9
	      + get_curr_stat(ch,STAT_INT)*2 + ch->level);
	number = number_percent();
	if (number < get_skill(ch,gsn_meditation))
	{
	    gain += number * gain / 100;
	    if (ch->mana < ch->max_mana)
	        check_improve(ch,gsn_meditation,TRUE,4);
	}
	number = number_percent();
	if (number < get_skill(ch,gsn_trance))
	{
	    gain += number * gain / 100;
	    if (ch->mana < ch->max_mana)
		check_improve(ch,gsn_trance,TRUE,4);
	}

	switch ( ch->position )
	{
	    default:		gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:	gain /= 2;			break;
	    case POS_FIGHTING:	gain /= 6;			break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   == 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    gain /= 2;

    }

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * 7/5;

    if ( IS_AFFECTED( ch, AFF_POISON ) )
	gain /= 4;

    if (ch->position == POS_SLEEPING && get_skill(ch,gsn_dark_dream) > 5)
    {
	if (number_percent() < get_skill(ch,gsn_dark_dream))
	{
	check_improve(ch,gsn_dark_dream,TRUE,5);
		gain *= 3;
		gain /= 2;
	}

    }

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) )
	gain /=2;
    if ( IS_AFFECTED(ch,AFF_SLOW))
	gain += (7*gain/10);


    if (is_affected(ch,gsn_camp))
    {
	if (number_percent() < ch->pcdata->learned[gsn_camp])
	gain *=2;
    }

    return UMIN(gain, ch->max_mana - ch->mana);
}



int move_gain( CHAR_DATA *ch )
{
    int gain;

    if (ch->in_room == NULL)
	return 0;
    if (is_affected(ch,gsn_atrophy) || is_affected(ch,gsn_prevent_healing))
	return 0;
    if (!IS_NPC(ch))
    {
	if (ch->pcdata->condition[COND_STARVING] > 6)
		return 0;
	if (ch->pcdata->condition[COND_DEHYDRATED] > 4)
		return 0;
    }

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	gain = UMAX( 15, ch->level );

	switch ( ch->position )
	{
	case POS_SLEEPING: gain += get_curr_stat(ch,STAT_DEX);		break;
	case POS_RESTING:  gain += get_curr_stat(ch,STAT_DEX) / 2;	break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   == 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    gain /= 2;
    }

    gain = gain * ch->in_room->heal_rate/100;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * 6/5;


    if (ch->position == POS_SLEEPING && get_skill(ch,gsn_dark_dream) > 5)
    {
	if (number_percent() < get_skill(ch,gsn_dark_dream))
	{
	check_improve(ch,gsn_dark_dream,TRUE,8);
		gain *=3;
		gain /=2;
	}
    }

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_AFFECTED(ch,AFF_SLOW))
        gain *=2 ;

    if (is_affected(ch,gsn_camp))
    {
	if (number_percent() < ch->pcdata->learned[gsn_camp])
	gain *= 2;
    }

    return UMIN(gain, ch->max_move - ch->move);
}


/* If you don't want starvation/dehydration in your mud then simply return
from function before the if (ch->level > 10) check...(Ceran)
*/ 
void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;
    int counter;

    if ( value == 0 || IS_NPC(ch) || ch->level >= LEVEL_IMMORTAL)
	return;

    condition				= ch->pcdata->condition[iCond];
    if (condition == -1)
        return;
    ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 48 );

    if (ch->level > 10)
    {
    if (ch->pcdata->condition[iCond] == 0 && iCond == COND_HUNGER)
		ch->pcdata->condition[COND_STARVING]++;
    if (ch->pcdata->condition[iCond] == 0 && iCond == COND_THIRST)
		ch->pcdata->condition[COND_DEHYDRATED]++;
    }
    else
    {
		ch->pcdata->condition[COND_STARVING] = 0;
		ch->pcdata->condition[COND_DEHYDRATED] = 0;
    }

    if (iCond == COND_HUNGER && value > 0
	&& ch->pcdata->condition[COND_STARVING] > 0)
	{
	counter = ch->pcdata->condition[COND_STARVING];
	if (counter <= 4)
	send_to_char("You are no longer famished.\n\r",ch);
	else
	send_to_char("You are no longer starving.\n\r",ch);
	ch->pcdata->condition[COND_STARVING] = 0;
	ch->pcdata->condition[COND_HUNGER] = 2;
	}

    if (iCond == COND_THIRST && value > 0
	&& ch->pcdata->condition[COND_DEHYDRATED] > 0)
  	{
	counter = ch->pcdata->condition[COND_DEHYDRATED];
	if (counter <= 5)
	send_to_char("You are no longer dehydrated.\n\r",ch);
	else
	send_to_char("You are no longer dying of thirst.\n\r",ch);
	ch->pcdata->condition[COND_DEHYDRATED] = 0;
	ch->pcdata->condition[COND_THIRST] = 2;
	}

    if (is_affected(ch,gsn_aura_of_sustenance))
    {
	ch->pcdata->condition[COND_HUNGER] = 48;
	ch->pcdata->condition[COND_THIRST] = 48;
	ch->pcdata->condition[COND_STARVING] = 0;
	ch->pcdata->condition[COND_DEHYDRATED] = 0;
    }


    if ( ch->pcdata->condition[iCond] <= 4 )
    {
	switch ( iCond )
	{
	case COND_HUNGER:
	    if (ch->pcdata->condition[COND_STARVING] < 2)
	    send_to_char( "You are hungry.\n\r",  ch );
	    break;

	case COND_THIRST:
	    if (ch->pcdata->condition[COND_DEHYDRATED] < 2)
	    send_to_char( "You are thirsty.\n\r", ch );
	    break;

	case COND_DRUNK:
	    if ( condition != 0 )
		send_to_char( "You are sober.\n\r", ch );
	    break;
	}
    }

    if (ch->pcdata->condition[COND_STARVING] > 1
	&& iCond == COND_HUNGER)
    {
	counter = ch->pcdata->condition[COND_STARVING];
	if (counter <= 5)
	send_to_char("You are famished!\n\r",ch);
	else if (counter <= 8)
	send_to_char("You are beginning to starve!\n\r",ch);
	else
	{
	send_to_char("You are starving!\n\r",ch);
if (ch->level > 10)
damage_old(ch,ch,number_range(counter - 3,2*(counter - 3)),gsn_starvation,DAM_OTHER,TRUE);
	}

    }
    if (ch->pcdata->condition[COND_DEHYDRATED] > 1
	&& iCond == COND_THIRST)
    {
	counter = ch->pcdata->condition[COND_DEHYDRATED];
	if (counter <= 2)
	send_to_char("Your mouth is parched!\n\r",ch);
	else if (counter <= 5)
	send_to_char("You are beginning to dehydrate!\n\r",ch);
	else
	{
	send_to_char("You are dying of thirst!\n\r",ch);
if (ch->level > 10)
damage_old(ch,ch,number_range(counter,2*counter),gsn_dehydrated,DAM_OTHER,TRUE);
	}

    }

    return;
}

void spec_update( CHAR_DATA *ch )
{
    CHAR_DATA *mob;
    CHAR_DATA *mob_next;

    for ( mob = ch->in_room->people; mob != NULL; mob = mob_next )
    {
        mob_next = mob->next;

        if ( IS_NPC(mob) && mob->spec_fun != 0 )
	{
            if ( (*mob->spec_fun) ( mob ) )
                return;
	}
    }
    return;
}


/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    EXIT_DATA *pexit;
    int door;

    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

	if ( !IS_NPC(ch) || ch->in_room == NULL 
	|| (IS_NPC(ch) && ch->pIndexData->vnum != MOB_VNUM_ENFORCER && IS_AFFECTED(ch,AFF_CHARM)))
	    continue;

	if (ch->in_room->area->empty && !IS_SET(ch->act,ACT_UPDATE_ALWAYS))
	    continue;

	/* Examine call for special procedure */
	if ( ch->spec_fun != 0 )
	{
	    if ( (*ch->spec_fun) ( ch ) )
		continue;
	}
        /*
         * Check triggers only if mobile still in default position
         */
        if ( ch->position == ch->pIndexData->default_pos )
        {
            /* Delay */
            if ( HAS_TRIGGER( ch, TRIG_DELAY) 
            &&   ch->mprog_delay > 0 )
            {
                if ( --ch->mprog_delay <= 0 )
                {
                    mp_percent_trigger( ch, NULL, NULL, NULL, TRIG_DELAY );
                    continue;
                }
            } 
            if ( HAS_TRIGGER( ch, TRIG_RANDOM) )
            {
                if( mp_percent_trigger( ch, NULL, NULL, NULL, TRIG_RANDOM ) )
                continue;
            }
        }

	if (ch->pIndexData->pShop != NULL) /* give him some gold */
	    if ((ch->gold * 100 + ch->silver) < ch->pIndexData->wealth)
	    {
		ch->gold += ch->pIndexData->wealth * number_range(1,20)/5000000;
		ch->silver += ch->pIndexData->wealth * number_range(1,20)/50000;
	    }
	 

	/* That's all for sleeping / busy monster, and empty zones */
	if ( ch->position != POS_STANDING )
	    continue;

	/* Scavenge */
	if ( IS_SET(ch->act, ACT_SCAVENGER)
	&&   ch->in_room->contents != NULL
	&&   number_bits( 6 ) == 0 )
	{
	    OBJ_DATA *obj;
	    OBJ_DATA *obj_best;
	    int max;

	    max         = 1;
	    obj_best    = 0;
	    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	    {
		if ( CAN_WEAR(obj, ITEM_TAKE) && can_loot(ch, obj)
		     && obj->cost > max  && obj->cost > 0)
		{
		    obj_best    = obj;
		    max         = obj->cost;
		}
	    }

	    if ( obj_best )
	    {
/*		get_obj(ch,obj,NULL);*/
		obj_from_room( obj_best );
		obj_to_char( obj_best, ch );
		act( "$n gets $p.", ch, obj_best, NULL, TO_ROOM );
	    }
	}

	/* Wander */
	if ( !IS_SET(ch->act, ACT_SENTINEL) 
	&& number_bits(3) == 0
	&& ( door = number_bits( 5 ) ) <= 5
	&& ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&&   !IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB)
	&& ( !IS_SET(ch->act, ACT_STAY_AREA)
	||   pexit->u1.to_room->area == ch->in_room->area ) 
	&& ( !IS_SET(ch->act, ACT_OUTDOORS)
	||   !IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS)) 
	&& ( !IS_SET(ch->act, ACT_INDOORS)
	||   IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS)))
	{
	    move_char( ch, door, FALSE );
	}
    }

    return;
}



/*
 * Update the weather.
 */
void weather_update( void )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int diff;

    buf[0] = '\0';

    switch ( ++time_info.hour )
    {
    case  5:
	weather_info.sunlight = SUN_LIGHT;
	strcat( buf, "The day has begun.\n\r" );
	break;

    case  6:
	weather_info.sunlight = SUN_RISE;
	strcat( buf, "The sun rises in the east.\n\r" );
	break;

    case 19:
	weather_info.sunlight = SUN_SET;
	strcat( buf, "The sun slowly disappears in the west.\n\r" );
	break;

    case 20:
	weather_info.sunlight = SUN_DARK;
	strcat( buf, "The night has begun.\n\r" );
	break;

    case 24:
	time_info.hour = 0;
	time_info.day++;
	break;
    }

    if ( time_info.day   >= 35 )
    {
	time_info.day = 0;
	time_info.month++;
    }

    if ( time_info.month >= 17 )
    {
	time_info.month = 0;
	time_info.year++;
    }

    /*
     * Weather change.
     */
    if ( time_info.month >= 9 && time_info.month <= 16 )
	diff = weather_info.mmhg >  985 ? -2 : 2;
    else
	diff = weather_info.mmhg > 1015 ? -2 : 2;

    weather_info.change   += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
    weather_info.change    = UMAX(weather_info.change, -12);
    weather_info.change    = UMIN(weather_info.change,  12);

    weather_info.mmhg += weather_info.change;
    weather_info.mmhg  = UMAX(weather_info.mmhg,  960);
    weather_info.mmhg  = UMIN(weather_info.mmhg, 1040);

    switch ( weather_info.sky )
    {
    default: 
	bug( "Weather_update: bad sky %d.", weather_info.sky );
	weather_info.sky = SKY_CLOUDLESS;
	break;

    case SKY_CLOUDLESS:
	if ( weather_info.mmhg <  990
	|| ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The sky is getting cloudy.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_CLOUDY:
	if ( weather_info.mmhg <  970
	|| ( weather_info.mmhg <  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "It starts to rain.\n\r" );
	    weather_info.sky = SKY_RAINING;
	}

	if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "The clouds disappear.\n\r" );
	    weather_info.sky = SKY_CLOUDLESS;
	}
	break;

    case SKY_RAINING:
	if ( weather_info.mmhg <  970 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "Lightning flashes in the sky.\n\r" );
	    weather_info.sky = SKY_LIGHTNING;
	}

	if ( weather_info.mmhg > 1030
	|| ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The rain stopped.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_LIGHTNING:
	if ( weather_info.mmhg > 1010
	|| ( weather_info.mmhg >  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The lightning has stopped.\n\r" );
	    weather_info.sky = SKY_RAINING;
	    break;
	}
	break;
    }

    if ( buf[0] != '\0' )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   IS_OUTSIDE(d->character)
	    &&   IS_AWAKE(d->character) )
		send_to_char( buf, d->character );
	}
    }

    return;
}



/*
 * Update all chars, including mobs.
*/
void char_update( void )
{   
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_quit;
    int noregen, hgain;
    ROOM_INDEX_DATA *pRoomIndex;
    int room;
    char chbuf[MAX_STRING_LENGTH];

    for (room = 0; room < 30000; room++)
    {
	pRoomIndex = get_room_index(room);
	if (pRoomIndex == NULL)
		continue;
     if (IS_SET(pRoomIndex->extra_room_flags,ROOM_GLOBE_DARKNESS))
		if (number_percent() < 16)
		REMOVE_BIT(pRoomIndex->extra_room_flags,ROOM_GLOBE_DARKNESS);
    }

    ch_quit	= NULL;

    /* update save counter */
    save_number++;

    if (save_number > 29)
	save_number = 0;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
    CHAR_DATA *master;
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;
	bool charm_gone;

	ch_next = ch->next;
        if ( ch->timer > 30 )
            ch_quit = ch;
	master = NULL;
	if ( ch->pause > 0 )
	    ch->pause--;

	if ( ch->ghost > 0 )
	    ch->ghost--;


	if (ch->race == 3 || ch->race == 4 || ch->race == 5)
	{
	    SET_BIT(ch->affected_by,AFF_SNEAK);
	    SET_BIT(ch->affected_by,AFF_INFRARED);
	}

	if (ch->race == 2)
	{
	    SET_BIT(ch->affected_by,AFF_INFRARED);
	}

	if (ch->race == 10)
	{
	    SET_BIT(ch->affected_by,AFF_FLYING);
	}

	if (ch->race == 11)
	{
	    SET_BIT(ch->affected_by,AFF_PASS_DOOR);
	    SET_BIT(ch->affected_by,AFF_INFRARED);
	    SET_BIT(ch->affected_by,AFF_FLYING);
	}

	if ( ch->position >= POS_STUNNED )
	{
	    if ( ch->hit  < ch->max_hit ) /* Some regen stuff */
	    {
		noregen = ch->noregen_dam;
		hgain = hit_gain(ch);
		ch->hit  += hgain;
		if (noregen > hgain)
			ch->noregen_dam -= hgain;
		else 
			ch->noregen_dam = 0;
	    }
	    else
		ch->hit = ch->max_hit;

	    if ( ch->mana < ch->max_mana )
		ch->mana += mana_gain(ch);
	    else
		ch->mana = ch->max_mana;

	    if ( ch->move < ch->max_move )
		ch->move += move_gain(ch);
	    else
		ch->move = ch->max_move;
	}

	if ( ch->position == POS_STUNNED )
	    update_pos( ch );

	if ( !IS_NPC(ch) && ch->level < LEVEL_IMMORTAL )
	{
	    OBJ_DATA *obj;

	    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 )
	    {
		if ( --obj->value[2] == 0 && ch->in_room != NULL )
		{
		    ch->in_room->light -= 3;
		    act( "$p goes out.", ch, obj, NULL, TO_ROOM );
		    act( "$p flickers and goes out.", ch, obj, NULL, TO_CHAR );
		    extract_obj( obj );
		}
	 	else if ( obj->value[2] <= 5 && ch->in_room != NULL)
		    act("$p flickers.",ch,obj,NULL,TO_CHAR);
	    }

	    if (IS_IMMORTAL(ch))
		ch->timer = 0;

	    if ( ++ch->timer >= 12 )
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
		    act( "$n disappears into the void.",
			ch, NULL, NULL, TO_ROOM );
		    send_to_char( "You disappear into the void.\n\r", ch );
		    if (ch->level > 1)
		        save_char_obj( ch );
		    ch->pause = 0;
		    do_quit(ch, "");
		}
	    }

	    gain_condition( ch, COND_DRUNK,  -1 );
	    gain_condition( ch, COND_FULL, -3 );
	    gain_condition( ch, COND_THIRST, -1 );
	    gain_condition( ch, COND_HUNGER, -1);
	}

	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
	    charm_gone = FALSE;
	    if ( paf->duration > 0 )
	    {
		paf->duration--;
		if (number_range(0,4) == 0 && paf->level > 0)
		  paf->level--;  /* spell strength fades with time */
            }
	    else if ( paf->duration < 0 )
		;
	    else
	    {
		if ( paf_next == NULL
		||   paf_next->type != paf->type
		||   paf_next->duration > 0 )
		{
		    if ( paf->type > 0 && skill_table[paf->type].msg_off )
		    {
			send_to_char( skill_table[paf->type].msg_off, ch );
			send_to_char( "\n\r", ch );
		    }
		}
	  	if (paf->type == skill_lookup("charm")
		&& IS_NPC(ch))
			charm_gone = TRUE;

		affect_remove( ch, paf );
	    }

/* Next block makes charmed mobs attack charmer if they come out of charm
and the master fails a level check. Just remove this if you don't
want charmies to attack players when the charm wears off....(Ceran) 
*/
	if (charm_gone)
		master = ch->master;
	if (master == NULL)
		continue;
	if ((charm_gone) && ch->position != POS_FIGHTING
	&& (number_percent() >= master->level)
	&& (ch->in_room == master->in_room))
	{
     sprintf(chbuf,"Help! I'm being attacked by %s!",
	IS_NPC(ch) ? ch->short_descr : ch->name);

	do_yell(master,chbuf);
	multi_hit(ch,master,TYPE_UNDEFINED);
	}
	}

	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */

        if (is_affected(ch, gsn_plague) && ch != NULL)
        {
            AFFECT_DATA *af, plague;
            CHAR_DATA *vch;
            int dam;

	    if (ch->in_room == NULL)
		return;
            
	    act("$n writhes in agony as plague sores erupt from $s skin.",
		ch,NULL,NULL,TO_ROOM);
	    send_to_char("You writhe in agony from the plague.\n\r",ch);
            for ( af = ch->affected; af != NULL; af = af->next )
            {
            	if (af->type == gsn_plague)
                    break;
            }
        
            if (af == NULL)
            {
            	REMOVE_BIT(ch->affected_by,AFF_PLAGUE);
            	return;
            }
        
            if (af->level == 1)
            	return;
        
	    plague.where		= TO_AFFECTS;
            plague.type 		= gsn_plague;
            plague.level 		= af->level - 1; 
            plague.duration 	= number_range(1,2 * plague.level);
            plague.location		= APPLY_STR;
            plague.modifier 	= -5;
            plague.bitvector 	= AFF_PLAGUE;
        
            for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(plague.level - 2,vch,DAM_DISEASE) 
		&&  !IS_IMMORTAL(vch)
            	&&  !IS_AFFECTED(vch,AFF_PLAGUE) && number_bits(4) == 0)
            	{
            	    send_to_char("You feel hot and feverish.\n\r",vch);
            	    act("$n shivers and looks very ill.",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&plague);
            	}
            }

	    dam = UMIN(ch->level,af->level);
	    damage_old( ch, ch, dam, gsn_plague,DAM_DISEASE,TRUE);
        }
	if ( IS_AFFECTED(ch, AFF_POISON) && ch != NULL
	     &&   !IS_AFFECTED(ch,AFF_SLOW))

	{
	    AFFECT_DATA *poison;

	    poison = affect_find(ch->affected,gsn_poison);

	    if (poison != NULL)
	    {
	        act( "$n shivers and suffers.", ch, NULL, NULL, TO_ROOM );
	        send_to_char( "You shiver and suffer.\n\r", ch );

	        damage_old(ch,ch,poison->level,gsn_poison,
		    DAM_POISON,TRUE);
	    }
	}

	else if ( ch->position == POS_INCAP && number_range(0,1) == 0)
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE,FALSE);
	}
	else if ( ch->position == POS_MORTAL )
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE,FALSE);
	}
    }

    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
        ch_next = ch->next;

	if (ch->desc != NULL && ch->desc->descriptor % 30 == save_number)
	    save_char_obj(ch);

        if ( ch == ch_quit )
            do_quit( ch, "" );
    }

    return;
}




/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{   
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    AFFECT_DATA *paf, *paf_next;

    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	CHAR_DATA *rch;
	char *message;

	obj_next = obj->next;

	/* go through affects and decrement */
        for ( paf = obj->affected; paf != NULL; paf = paf_next )
        {
            paf_next    = paf->next;
            if ( paf->duration > 0 )
            {
                paf->duration--;
                if (number_range(0,4) == 0 && paf->level > 0)
                  paf->level--;  /* spell strength fades with time */
            }
            else if ( paf->duration < 0 )
                ;
            else
            {
                if ( paf_next == NULL
                ||   paf_next->type != paf->type
                ||   paf_next->duration > 0 )
                {
                    if ( paf->type > 0 && skill_table[paf->type].msg_obj )
                    {
			if (obj->carried_by != NULL)
			{
			    rch = obj->carried_by;
			    act(skill_table[paf->type].msg_obj,
				rch,obj,NULL,TO_CHAR);
			}
			if (obj->in_room != NULL 
			&& obj->in_room->people != NULL)
			{
			    rch = obj->in_room->people;
			    act(skill_table[paf->type].msg_obj,
				rch,obj,NULL,TO_ALL);
			}
                    }
                }

                affect_remove_obj( obj, paf );
            }
        }


	if ( obj->timer <= 0 || --obj->timer > 0 )
	    continue;

	switch ( obj->item_type )
	{
	default:              message = "$p crumbles into dust.";  break;
	case ITEM_FOUNTAIN:   message = "$p dries up.";         break;
	case ITEM_CORPSE_NPC: message = "$p decays into dust."; break;
	case ITEM_CORPSE_PC:  message = "$p decays into dust.";
        break;
	case ITEM_FOOD:       message = "$p decomposes.";	break;
	case ITEM_POTION:     message = "$p has evaporated from disuse.";	
								break;
	case ITEM_PORTAL:     message = "$p fades out of existence."; break;
	case ITEM_CONTAINER: 
	    if (CAN_WEAR(obj,ITEM_WEAR_FLOAT))
		if (obj->contains)
		    message = 
		"$p flickers and vanishes, spilling its contents on the floor.";
		else
		    message = "$p flickers and vanishes.";
	    else
		message = "$p crumbles into dust.";
	    break;
	}

	if ( obj->carried_by != NULL )
	{
	    if (IS_NPC(obj->carried_by) 
	    &&  obj->carried_by->pIndexData->pShop != NULL)
		obj->carried_by->silver += obj->cost/5;
	    else
	    {
	    	act( message, obj->carried_by, obj, NULL, TO_CHAR );
		if ( obj->wear_loc == WEAR_FLOAT)
		    act(message,obj->carried_by,obj,NULL,TO_ROOM);
	    }
	}
	else if ( obj->in_room != NULL
	&&      ( rch = obj->in_room->people ) != NULL )
	{
	    if (! (obj->in_obj && obj->in_obj->pIndexData->vnum == OBJ_VNUM_PIT
	           && !CAN_WEAR(obj->in_obj,ITEM_TAKE)))
	    {
	    	act( message, rch, obj, NULL, TO_ROOM );
	    	act( message, rch, obj, NULL, TO_CHAR );
	    }
	}

        if (obj->item_type == ITEM_CORPSE_PC && obj->contains)
        {   /* objects go to pit */
            OBJ_DATA *t_obj, *next_obj, *pit_obj;

/* Put items into the pit that the player recalls to */
	    if (obj->value[4] == TEMPLE_OFCOL)
		pit_obj = get_obj_type(get_obj_index(PIT_OFCOL));
	    else if (obj->value[4] == TEMPLE_NEW_THALOS)
		pit_obj = get_obj_type(get_obj_index(PIT_NEWTHALOS));
	    else if (obj->value[4] == TEMPLE_ARKHAM)
		pit_obj = get_obj_type(get_obj_index(PIT_ARKHAM));
	    else
            pit_obj = get_obj_type(get_obj_index(OBJ_VNUM_PIT));

	    for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
	    {
		next_obj = t_obj->next_content;
		obj_from_obj(t_obj);
                obj_to_obj(t_obj,pit_obj);
	    }
	}

	extract_obj( obj );
    }

    return;
}


/* Mobile tracking. I implemented the most stupid, basic mob tracking you
could possibly come up with, but it works ok from a player point of view.
If you want something more sophisticated just hack that into here
instead...(Ceran)
*/
void track_update( void )
{
    CHAR_DATA *tch;
    CHAR_DATA *tch_next;
    char buf[MAX_STRING_LENGTH];

    for (tch = char_list; tch != NULL; tch = tch_next)
    {
	tch_next = tch->next;
	if (!IS_NPC(tch) || (tch->last_fought == NULL))
		continue;
	if (tch->fighting != NULL || (tch->position != POS_STANDING)
	|| (is_affected(tch,gsn_power_word_stun)))
		continue;

	track_char(tch,tch->last_fought,tch->in_room->vnum);
	
	if (tch->in_room == tch->last_fought->in_room)
	{
		if (can_see(tch,tch->last_fought))
		{
		sprintf(buf,"%s, now you die!",tch->last_fought->name);
		do_yell(tch,buf);
		multi_hit(tch,tch->last_fought,TYPE_UNDEFINED);
		}
	}
    }
    return;
}
	

/* This is for the riot skill used by outlaws. If you don't plan on using
riot in the mud it would be a good idea to juse remove this function to
speed up the update calls...(Ceran)
*/
 void riot_update( void )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *rioter;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *rioter_next;

    for (rioter = char_list; rioter != NULL; rioter = rioter_next)
    {
	rioter_next = rioter->next; 
	if (!is_affected(rioter,gsn_riot) || rioter->position != POS_STANDING)
 		continue;
        for (vch = rioter->in_room->people; vch != NULL; vch = vch_next)
 	{
	vch_next = vch->next_in_room;
	if (is_same_group(vch,rioter))
		continue;
	sprintf(buf, "Help! %s is rioting!",(IS_NPC(rioter) ? rioter->short_descr : rioter->name));
	do_yell(vch,buf);
        multi_hit(rioter,vch,TYPE_UNDEFINED); 
	break;
	}
    }
    return;
}

/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25% to 35% of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */
void aggr_update( void )
{
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *victim;

    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
	wch_next = wch->next;
	if ( IS_NPC(wch)
	||   wch->level >= LEVEL_IMMORTAL
	||   wch->in_room == NULL 
	||   wch->in_room->area->empty)
	    continue;

	for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )
	{
	    int count;

	    ch_next	= ch->next_in_room;

	    if ( !IS_NPC(ch)
	    ||   !IS_SET(ch->act, ACT_AGGRESSIVE)
	    ||   IS_SET(ch->in_room->room_flags,ROOM_SAFE)
	    ||   IS_AFFECTED(ch,AFF_CALM)
	    ||   ch->fighting != NULL
	    ||   IS_AFFECTED(ch, AFF_CHARM)
	    ||   !IS_AWAKE(ch)
	    ||   ( IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch) )
	    ||   !can_see( ch, wch ) 
	    ||   number_bits(1) == 0)
		continue;

	    /*
	     * Ok we have a 'wch' player character and a 'ch' npc aggressor.
	     * Now make the aggressor fight a RANDOM pc victim in the room,
	     *   giving each 'vch' an equal chance of selection.
	     */
	    count	= 0;
	    victim	= NULL;
	    for ( vch = wch->in_room->people; vch != NULL; vch = vch_next )
	    {
		vch_next = vch->next_in_room;

		if ( !IS_NPC(vch)
		&&   vch->level < LEVEL_IMMORTAL
		&&   ch->level >= vch->level - 5 
		&&   ( !IS_SET(ch->act, ACT_WIMPY) || !IS_AWAKE(vch) )
		&&   can_see( ch, vch ) )
		{
		    if ( number_range( 0, count ) == 0 )
			victim = vch;
		    count++;
		}
	    }

	    if ( victim == NULL )
		continue;

	    multi_hit( ch, victim, TYPE_UNDEFINED );
	}
    }

      
    return;
}



/* All this age stuff is for characters to die of old age. If you don't
like this idea (and old age death is a permenant death) then you should
remove this update code. If you don't like the time set just change the
values...(Ceran)
*/

int get_age(CHAR_DATA *ch) /* returns age in years. */

{
    int age;
    char *race;

    if (IS_NPC(ch))
        return 17;

	age = ((ch->played + (int) (current_time - ch->logon) ) / 5000);

    race = pc_race_table[ch->race].name;

    if (!str_cmp(race,"human"))
    {
        age /= 6;   age += 17;
    }
    else if ( !str_cmp(race,"elf") || !str_cmp(race,"grey-elf") || !str_cmp(race,"dark-elf") )
        age += 70;
    else if (!str_cmp(race,"gnome"))
        age += 80;
    else if (!str_cmp(race,"dwarf"))
        age += 40;
    else if (!str_cmp(race,"giant") || !str_cmp(race,"troll") || !str_cmp(race,"centaur") )
    {
        age /= 3;   age += 30;
    }
    else if (!str_cmp(race,"draconian"))
    {
        age /= 6;   age += 17;
    }
    else if (!str_cmp(race,"ethereal"))
    {
        age /= 6;   age += 18;
    }
    else if (!str_cmp(race,"changeling"))
    {
        age /= 6;   age += 15;
    }
    else
    {
        age /= 6;   age += 17;
    }

    return age;
}

/* returns death age in years  */
int get_death_age(CHAR_DATA *ch)
{
    int age,con;
    char *race;

    if (IS_NPC(ch))
        return 1000;

    race = pc_race_table[ch->race].name;
    con = ch->perm_stat[STAT_CON];
    age = 1000;

    if (!str_cmp(race,"human"))
        age = (28 + 3*con);
    else if ( !str_cmp(race,"elf") || !str_cmp(race,"grey-elf") || !str_cmp(race,"dark-elf") )
        age = (320 + 25*con);
    else if (!str_cmp(race,"gnome"))
        age = (230 + 22*con);
    else if (!str_cmp(race,"dwarf"))
        age = (270 + 13*con);
    else if (!str_cmp(race,"giant") || !str_cmp(race,"troll") || !str_cmp(race,"centaur") )
        age = (80 + 6*con);
    else if (!str_cmp(race,"draconian"))
        age = (45 + 3*con);
    else if (!str_cmp(race,"ethereal"))
        age = (47 + 3*con);
    else if (!str_cmp(race,"changeling"))
        age = (53 + 3*con);
    else
        age = (40 + 3*con);
    age -= ch->pcdata->age_mod;

    return age;
}


/* returns the age name type */
char * get_age_name(CHAR_DATA *ch)
{
    char *name;
    char *race;
    int age;

    if (IS_NPC(ch))
        return "young";

    race = pc_race_table[ch->race].name;
    age = get_age(ch);

    if (!str_cmp(race,"human"))
    {
        if (age <= 20)  name = "young";
        else if (age <= 29) name = "mature";
        else if (age <= 59) name = "middle aged";
        else if (age <= 69) name = "old";
        else    name = "very old";
    }
    else if (!str_cmp(race,"elf") || !str_cmp(race,"dark-elf") || !str_cmp(race,"grey-elf") )
    {
        if (age <= 100) name = "young";
        else if (age <= 240)    name = "mature";
        else if (age <= 550)    name = "middle aged";
        else if (age <= 650)    name = "old";
        else if (age <= 700)    name = "very old";
        else                    name = "ancient";
    }
    else if (!str_cmp(race,"dwarf"))
    {
        if (age <= 68)  name = "young";
        else if (age <= 120) name = "mature";
        else if (age <= 400) name = "middle aged";
        else if (age <= 500) name = "old";
        else    name = "very old";
    }
    else if (!str_cmp(race,"gnome"))
    {
        if (age <= 140) name = "mature";
        else if (age <= 500)    name = "middle aged";
        else if (age <= 600)    name = "old";
        else if (age <= 700)    name = "very old";
        else                    name = "ancient";
    }
    else if (!str_cmp(race,"giant") || !str_cmp(race,"troll") || !str_cmp(race,"centaur"))
    {
        if (age <= 60)  name = "young";
        else if (age <= 75) name = "mature";
        else if (age <= 150) name = "middle aged";
        else if (age <= 1800) name = "old";
        else    name = "very old";
    }
    else
    {
        if (age <= 20)  name = "young";
        else if (age <= 29) name = "mature";
        else if (age <= 49) name = "middle aged";
        else if (age <= 69) name = "old";
        else    name = "very old";
    }

    return name;
}

 void age_update(void)
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    int age, death_age;
    int death_percent;
    char strsave[MAX_STRING_LENGTH];

    for (ch = char_list; ch != NULL; ch = ch_next)
    {
	ch_next = ch->next;
	if (IS_NPC(ch))
		continue;
	if (IS_IMMORTAL(ch))
		continue;
	if (ch->pcdata->death_timer > 0
	&& ch->pcdata->death_status == HAS_DIED)
		continue;

	if (ch->pcdata->death_status == HAS_DIED)
	{
	ch->pcdata->death_timer -= 1;
	if (ch->pcdata->death_timer > 0)
		continue;
	else
	{
	act("$n slowly fades away as $s souls departs the mortal planes.",ch,0,0,TO_ROOM);
send_to_char("Your soul finally departs the mortal planes.\n\r",ch);
	            sprintf( strsave, "%s%s.plr", PLAYER_DIR, capitalize(ch->name ) );
            wiznet("$ has finally died of old age..",ch,NULL,0,0,0);
            ch->pause = 0;
            stop_fighting(ch,TRUE);
            do_quit(ch, "");
            remove(strsave);
	}
	continue;
	}
	age = get_age(ch);
	death_age = get_death_age(ch);
	if (age < death_age)
		continue;
	death_percent = (age - death_age)*5;
 	if (number_percent() > death_percent)
		continue;
	act("$n's heart suddenly gives out and $s keels over dead!",
	ch,0,0,TO_ROOM);
send_to_char("Your heart finally gives out from your hard life and you keel over dead!\n\r",ch);
	age_death(ch);	
	ch->pcdata->death_timer = 150;
	ch->pcdata->death_status = HAS_DIED;
	}
return;
}


/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */

void update_handler( void )
{
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_point;
    static  int	    pulse_track;
    static  int	    pulse_riot;

    if ( --pulse_area     <= 0 )
    {
	pulse_area	= PULSE_AREA;
	/* number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 ); */
	area_update	( );
    }

    if ( --pulse_track <= 0)
    {
	pulse_track = PULSE_TRACK;
	track_update	( );
    }

    if ( --pulse_riot <= 0)
    {
	pulse_riot = PULSE_RIOT;
	riot_update	( );
    }

    if ( --pulse_mobile   <= 0 )
    {
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
    }

    if ( --pulse_violence <= 0 )
    {
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );
    }


    if ( --pulse_point    <= 0 )
    {
	wiznet("TICK!",NULL,NULL,WIZ_TICKS,0,0);
	pulse_point     = PULSE_TICK;
/* number_range( PULSE_TICK / 2, 3 * PULSE_TICK / 2 ); */
	weather_update	( );
	char_update	( );
	obj_update	( );
	age_update	( );
    }

    aggr_update( );
    tail_chain( );
    return;
}
