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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"

/* command procedures needed */
DECLARE_DO_FUN(	do_exits	);
DECLARE_DO_FUN( do_look		);
DECLARE_DO_FUN( do_help		);
DECLARE_DO_FUN( do_affects	);
DECLARE_DO_FUN( do_play		);
DECLARE_DO_FUN( do_morph_dragon );
DECLARE_DO_FUN( do_revert	);
DECLARE_DO_FUN( do_tell		);


char *	const	where_name	[] =
{
    "<used as light>     ",
    "<worn on finger>    ",
    "<worn on finger>    ",
    "<worn around neck>  ",
    "<worn around neck>  ",
    "<worn on torso>     ",
    "<worn on head>      ",
    "<worn on legs>      ",
    "<worn on feet>      ",
    "<worn on hands>     ",
    "<worn on arms>      ",
    "<worn as shield>    ",
    "<worn on body>      ",
    "<worn on waist>     ",
    "<worn around wrist> ",
    "<worn around wrist> ",
    "<wielded>           ",
    "<held>              ",
    "<dual wielded>      ",
    "<trinal wielded>    ",
    "<branded>           ",
};


/* for do_count */
int max_on = 0;



/*
 * Local functions.
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
void	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_2	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_3	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );

bool	can_shapeshift		args( ( CHAR_DATA *ch, int form_num) );
void	shapeshift_revert	args( ( CHAR_DATA *ch) );


char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';

    if ((fShort && (obj->short_descr == NULL || obj->short_descr[0] == '\0'))
    ||  (obj->description == NULL || obj->description[0] == '\0'))
	return buf;

    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "(Invis) "     );
    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_EVIL)   )   strcat( buf, "(Red Aura) "  );
    if (IS_AFFECTED(ch, AFF_DETECT_GOOD)
    &&  IS_OBJ_STAT(obj,ITEM_BLESS))	      strcat( buf,"(Blue Aura) "	);
    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "(Magical) "   );
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   strcat( buf, "(Glowing) "   );
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   strcat( buf, "(Humming) "   );

    if ( fShort )
    {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
    }
    else
    {
	if ( obj->description != NULL)
	    strcat( buf, obj->description );
    }

    return buf;
}



/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    int line;
    bool fCombine;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    output = new_buf();
    line = ch->lines;
    ch->lines = 0;
    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    prgpstrShow	= alloc_mem( count * sizeof(char *) );
    prgnShow    = alloc_mem( count * sizeof(int)    );
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    { 
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj )) 
	{
	    pstrShow = format_obj_to_char( obj, ch, fShort );

	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if (prgpstrShow[iShow][0] == '\0')
	{
	    free_string(prgpstrShow[iShow]);
	    continue;
	}

	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		sprintf( buf, "(%2d) ", prgnShow[iShow] );
		add_buf(output,buf);
	    }
	    else
	    {
		add_buf(output,"     ");
	    }
	}
	add_buf(output,prgpstrShow[iShow]);
	add_buf(output,"\n\r");
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    send_to_char( "     ", ch );
	send_to_char( "Nothing.\n\r", ch );
    }
    page_to_char(buf_string(output),ch);

    /*
     * Clean up.
     */
    free_buf(output);
    free_mem( prgpstrShow, count * sizeof(char *) );
    free_mem( prgnShow,    count * sizeof(int)    );
    ch->lines = line;
    return;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH],message[MAX_STRING_LENGTH];
   int sn_faerie_fog;
    OBJ_DATA *obj;
    buf[0] = '\0';

    obj = get_eq_char(victim,WEAR_HEAD);

    sn_faerie_fog = skill_lookup("faerie fog");
    if ( IS_SET(victim->comm,COMM_AFK	  )   ) strcat( buf, "[AFK] "	     );
    if ( !IS_NPC(victim))
	{
	if (victim->pcdata->death_status == HAS_DIED)
		strcat(buf,"(GHOST) ");
	}
    if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "(Invis) "      );
    if ( victim->invis_level >= LEVEL_HERO    ) strcat( buf, "(Wizi) "	     );
    if ( is_affected(victim,gsn_earthfade)	) strcat(buf,"(Earthfade) " );
    if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "(Hide) "       );
    if ( IS_AFFECTED(victim, AFF_CAMOUFLAGE)  ) strcat( buf, "(Camouflage) " );
    if (is_affected(victim,gsn_forest_blending)) strcat(buf,"(Blending) " );
    if ( IS_AFFECTED(victim, AFF_CHARM)       ) strcat( buf, "(Charmed) "    );
    if ( IS_AFFECTED(victim, AFF_PASS_DOOR)   ) strcat( buf, "(Translucent) ");
    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "(Pink Aura) "  );
   if (is_affected(victim,sn_faerie_fog) )	strcat( buf, "(Purple Aura) "	);
    if ( IS_EVIL(victim)
    &&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf, "(Red Aura) "   );
    if ( IS_GOOD(victim)
    &&   IS_AFFECTED(ch, AFF_DETECT_GOOD)     ) strcat( buf, "(Golden Aura) ");
    if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "(White Aura) " );
    if (obj != NULL && obj->pIndexData->vnum == OBJ_VNUM_TIARA)
		strcat( buf, "(Blue aura) ");
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER ) )
						strcat( buf, "(KILLER) "     );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF  ) )
						strcat( buf, "(THIEF) "      );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_CRIMINAL ) )
                                                strcat( buf, "(CRIMINAL) "   );
    if (( victim->long_descr[0] != '\0' ) || (victim->morph_form[0] != 0) )
    {
	strcat( buf, get_descr_form(victim,ch,TRUE) );
	send_to_char( buf, ch );
	return;
    }

    strcat( buf, get_descr_form(victim,ch,FALSE) );
    if ( !IS_NPC(victim) && !IS_SET(ch->comm, COMM_BRIEF)  && (ch->morph_form[0] == 0))
	strcat( buf, victim->pcdata->title );

    switch ( victim->position )
    {
    case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;
    case POS_MORTAL:   strcat( buf, " is mortally wounded." );   break;
    case POS_INCAP:    strcat( buf, " is incapacitated." );      break;
    case POS_STUNNED:  strcat( buf, " is lying here stunned." ); break;
    case POS_SLEEPING: 
	if (victim->on != NULL)
	{
	    if (IS_SET(victim->on->value[2],SLEEP_AT))
  	    {
		sprintf(message," is sleeping at %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET(victim->on->value[2],SLEEP_ON))
	    {
		sprintf(message," is sleeping on %s.",
		    victim->on->short_descr); 
		strcat(buf,message);
	    }
	    else
	    {
		sprintf(message, " is sleeping in %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else 
	    strcat(buf," is sleeping here.");
	break;
    case POS_RESTING:  
        if (victim->on != NULL)
	{
            if (IS_SET(victim->on->value[2],REST_AT))
            {
                sprintf(message," is resting at %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET(victim->on->value[2],REST_ON))
            {
                sprintf(message," is resting on %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else 
            {
                sprintf(message, " is resting in %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
	}
        else
	    strcat( buf, " is resting here." );       
	break;
    case POS_SITTING:  
        if (victim->on != NULL)
        {
            if (IS_SET(victim->on->value[2],SIT_AT))
            {
                sprintf(message," is sitting at %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET(victim->on->value[2],SIT_ON))
            {
                sprintf(message," is sitting on %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else
            {
                sprintf(message, " is sitting in %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
        }
        else
	    strcat(buf, " is sitting here.");
	break;
    case POS_STANDING: 
	if (victim->on != NULL)
	{
	    if (IS_SET(victim->on->value[2],STAND_AT))
	    {
		sprintf(message," is standing at %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET(victim->on->value[2],STAND_ON))
	    {
		sprintf(message," is standing on %s.",
		   victim->on->short_descr);
		strcat(buf,message);
	    }
	    else
	    {
		sprintf(message," is standing in %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else
	    strcat( buf, " is here." );               
	break;
    case POS_FIGHTING:
	strcat( buf, " is here, fighting " );
	if ( victim->fighting == NULL )
	    strcat( buf, "thin air??" );
	else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	else if ( victim->in_room == victim->fighting->in_room )
	{
	    strcat( buf, PERS( victim->fighting, ch ) );
	    strcat( buf, "." );
	}
	else
	    strcat( buf, "someone who left??" );
	break;
    }

    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
    return;
}



void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;

    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "$n looks at $mself.",ch,NULL,NULL,TO_ROOM);
	else
	{
	    act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
    }
    if(is_affected(victim, gsn_cloak_form)) {
	send_to_char("The figure is buried deep within a dark colored cloak.\n\r",ch);
    } else if ( victim->description[0] != '\0' ) {
	send_to_char( victim->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, get_descr_form(victim,ch,FALSE) );

    if (IS_SET(ch->act,PLR_EVALUATION))
    {
        if (percent >= 100)
          strcat(buf," is in perfect condition.\n\r");
        else if (percent >= 95)
            strcat(buf, "has a few scratches.\n\r");
        else if (percent >= 90)
            strcat(buf," has a few bruises.\n\r");
        else if (percent >= 80)
              strcat(buf," has some small wounds.\n\r");
       else if (percent >= 70)
            strcat(buf," has quite a few wounds.\n\r");
       else if (percent >= 60)
		strcat(buf," is covered in bleeding wounds.\n\r");
        else if (percent >= 50)
            strcat(buf," is bleeding profusely.\n\r");
        else if (percent >= 40)
            strcat(buf," is gushing blood.\n\r");
        else if (percent >= 30)
            strcat(buf," is screaming in pain.\n\r");
        else if (percent >= 20)
            strcat(buf," is spasming in shock.\n\r");
        else if (percent >= 10)
            strcat(buf," is writhing in agony.\n\r");
        else if (percent >= 1)
            strcat(buf," is convulsing on the ground.\n\r");
        else
            strcat(buf," is nearly dead.\n\r");
    }
    else
    {
    if (percent >= 100) 
        strcat( buf, " is in perfect condition.\n\r");
    else if (percent >= 90) 
	strcat( buf, " has a few scratches.\n\r");
    else if (percent >= 75) 
        strcat( buf, " has some small wounds.\n\r");
    else if (percent >=  50) 
        strcat( buf, " has some nasty cuts.\n\r");
    else if (percent >= 30)
        strcat( buf, " is bleeding profusely.\n\r");
    else if (percent >= 15)
        strcat( buf, " is screaming in pain.\n\r");
    else if (percent >= 0 )
        strcat( buf, " is in pretty bad shape.\n\r");
    else
        strcat(buf, " is nearly dead.\n\r");
    }

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    if(!is_affected(victim, gsn_cloak_form)||IS_IMMORTAL(ch)) {
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
    }

    if ( victim != ch
    &&   !IS_NPC(ch)
    && (IS_IMMORTAL(ch) || !IS_IMMORTAL(victim))
    &&   number_percent( ) < get_skill(ch,gsn_peek))
    {
	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
	check_improve(ch,gsn_peek,TRUE,4);
	show_list_to_char( victim->carrying, ch, TRUE, TRUE );
    }
    }
    return;
}


void show_char_to_char_2( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;

    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "$n glances at $mself.",ch,NULL,NULL,TO_ROOM);
	else
	{
	    act( "$n glances at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n glances at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, get_descr_form(victim,ch,FALSE) );

    if (IS_SET(ch->act,PLR_EVALUATION))
    {
        if (percent >= 100)
          strcat(buf," is in perfect condition.\n\r");
        else if (percent >= 95)
            strcat(buf," has a few scratches.\n\r");
        else if (percent >= 90)
            strcat(buf," has a few bruises.\n\r");
        else if (percent >= 80)
              strcat(buf," has some small wounds.\n\r");
       else if (percent >= 70)
            strcat(buf," has quite a few wounds.\n\r");
       else if (percent >= 60)
		strcat(buf," is covered in bleeding wounds.\n\r");
        else if (percent >= 50)
            strcat(buf," is bleeding profusely.\n\r");
        else if (percent >= 40)
            strcat(buf," is gushing blood.\n\r");
        else if (percent >= 30)
            strcat(buf," is screaming in pain.\n\r");
        else if (percent >= 20)
            strcat(buf," is spasming in shock.\n\r");
        else if (percent >= 10)
            strcat(buf," is writhing in agony.\n\r");
        else if (percent >= 1)
            strcat(buf," is convulsing on the ground.\n\r");
        else
            strcat(buf," is nearly dead.\n\r");
    }
    else     
    {   
    if (percent >= 100) 
        strcat( buf, " is in perfect condition.\n\r");
    else if (percent >= 90) 
	strcat( buf, " has a few scratches.\n\r");
    else if (percent >= 75) 
        strcat( buf, " has some small wounds.\n\r");
    else if (percent >=  50) 
        strcat( buf, " has some nasty cuts.\n\r");
    else if (percent >= 30)
        strcat( buf, " is bleeding profusely.\n\r");
    else if (percent >= 15)
        strcat( buf, " is screaming in pain.\n\r");
    else if (percent >= 0 )
        strcat( buf, " is in pretty bad shape.\n\r");
    else
        strcat(buf, " is nearly dead.\n\r");
    }

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
    }

    if ( victim != ch
    &&   !IS_NPC(ch)
    && (IS_IMMORTAL(ch) || !IS_IMMORTAL(victim))
    &&   number_percent( ) < get_skill(ch,gsn_peek))
    {
	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
	check_improve(ch,gsn_peek,TRUE,4);
	show_list_to_char( victim->carrying, ch, TRUE, TRUE );
    }

    return;
}

void show_char_to_char_3( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    int percent;

    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "$n looks closely at $mself.",ch,NULL,NULL,TO_ROOM);
	else
	{
	    act( "$n looks closely at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n looks closely at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
    }

    if ( victim->description[0] != '\0' )
    {
	send_to_char( victim->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, get_descr_form(victim,ch,FALSE));

    if (IS_SET(ch->act,PLR_EVALUATION))
    {
        if (percent >= 100)
          strcat(buf," is in perfect condition\n\r.");
        else if (percent >= 95)
            strcat(buf," has a few scratches.\n\r");
        else if (percent >= 90)
            strcat(buf," has a few bruises.\n\r");
        else if (percent >= 80)
              strcat(buf," has some small wounds.\n\r");
       else if (percent >= 70)
            strcat(buf," has quite a few wounds.\n\r");
       else if (percent >= 60)
		strcat(buf," is covered in bleeding wounds.\n\r");
        else if (percent >= 50)
            strcat(buf," is bleeding profusely.\n\r");
        else if (percent >= 40)
            strcat(buf," is gushing blood.\n\r");
        else if (percent >= 30)
            strcat(buf," is screaming in pain.\n\r");
        else if (percent >= 20)
            strcat(buf," is spasming in shock.\n\r");
        else if (percent >= 10)
            strcat(buf," is writhing in agony.\n\r");
        else if (percent >= 1)
            strcat(buf," is convulsing on the ground.\n\r");
        else
            strcat(buf," is nearly dead.\n\r");
    }
    else        
    {
    if (percent >= 100) 
        strcat( buf, " is in perfect condition.\n\r");
    else if (percent >= 90) 
	strcat( buf, " has a few scratches.\n\r");
    else if (percent >= 75) 
        strcat( buf, " has some small wounds.\n\r");
    else if (percent >=  50) 
        strcat( buf, " has some nasty cuts.\n\r");
    else if (percent >= 30)
        strcat( buf, " is bleeding profusely.\n\r");
    else if (percent >= 15)
        strcat( buf, " is screaming in pain.\n\r");
    else if (percent >= 0 )
        strcat( buf, " is in pretty bad shape.\n\r");
    else
        strcat(buf, " is nearly dead.\n\r");
    }

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    return;
}


void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;
    int light,i;
    OBJ_DATA *obj;
    bool cant_see = FALSE;

    if (!IS_NPC(ch)
    && IS_SET(ch->in_room->extra_room_flags,ROOM_GLOBE_DARKNESS)
    && str_cmp(pc_race_table[ch->race].name,"dark-elf") )
    {
	light = 0;
	for (i = 0; i < MAX_WEAR; i++)
	{
	obj = get_eq_char(ch,i);
	if (obj == NULL)
		continue;
	if (IS_SET(obj->extra_flags,ITEM_GLOW))
		light++;
	if (obj->item_type == ITEM_LIGHT)
		light += 3;
	}
	if (light < 8)
	{
	cant_see = TRUE;
    	}
    }

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( get_trust(ch) < rch->invis_level)
	    continue;

	if ((cant_see)
	&&        IS_AFFECTED(rch, AFF_INFRARED ) )
	{
	    send_to_char( "You see glowing red eyes watching YOU!\n\r", ch );
	}
	else if ( (cant_see))
		continue;
	else if ( can_see( ch, rch ))
	{
	    show_char_to_char_0( rch, ch );
	}
	else if ( room_is_dark( ch->in_room )
	&&        IS_AFFECTED(rch, AFF_INFRARED ) )
	{
	    send_to_char( "You see glowing red eyes watching YOU!\n\r", ch );
	}
    }

    return;
} 



bool check_blind( CHAR_DATA *ch )
{

    if (!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT))
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) )
    { 
	send_to_char( "You can't see a thing!\n\r", ch ); 
	return FALSE; 
    }

    return TRUE;
}

/* changes your scroll */
void do_scroll(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    int lines;

    one_argument(argument,arg);
    
    if (arg[0] == '\0')
    {
	if (ch->lines == 0)
	    send_to_char("You do not page long messages.\n\r",ch);
	else
	{
	    sprintf(buf,"You currently display %d lines per page.\n\r",
		    ch->lines + 2);
	    send_to_char(buf,ch);
	}
	return;
    }

    if (!is_number(arg))
    {
	send_to_char("You must provide a number.\n\r",ch);
	return;
    }

    lines = atoi(arg);

    if (lines == 0)
    {
        send_to_char("Paging disabled.\n\r",ch);
        ch->lines = 0;
        return;
    }

    if (lines < 10 || lines > 100)
    {
	send_to_char("You must provide a reasonable number.\n\r",ch);
	return;
    }

    sprintf(buf,"Scroll set to %d lines.\n\r",lines);
    send_to_char(buf,ch);
    ch->lines = lines - 2;
}

/* RT does socials */
void do_socials(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;
     
    col = 0;
   
    for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)
    {
	sprintf(buf,"%-12s",social_table[iSocial].name);
	send_to_char(buf,ch);
	if (++col % 6 == 0)
	    send_to_char("\n\r",ch);
    }

    if ( col % 6 != 0)
	send_to_char("\n\r",ch);
    return;
}


 
/* RT Commands to replace news, motd, imotd, etc from ROM */

void do_motd(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"motd");
}

void do_imotd(CHAR_DATA *ch, char *argument)
{  
    do_help(ch,"imotd");
}

void do_rules(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"rules");
}

void do_story(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"story");
}

void do_wizlist(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"wizlist");
	return;
}


/* RT this following section holds all the auto commands from ROM, as well as
   replacements for config */

void do_autolist(CHAR_DATA *ch, char *argument)
{
    /* lists most player flags */
    if (IS_NPC(ch))
      return;

    send_to_char("   action     status\n\r",ch);
    send_to_char("---------------------\n\r",ch);
 
    send_to_char("autoassist     ",ch);
    if (IS_SET(ch->act,PLR_AUTOASSIST))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch); 

    send_to_char("autoexit       ",ch);
    if (IS_SET(ch->act,PLR_AUTOEXIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autogold       ",ch);
    if (IS_SET(ch->act,PLR_AUTOGOLD))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autoloot       ",ch);
    if (IS_SET(ch->act,PLR_AUTOLOOT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autosac        ",ch);
    if (IS_SET(ch->act,PLR_AUTOSAC))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autosplit      ",ch);
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("compact mode   ",ch);
    if (IS_SET(ch->comm,COMM_COMPACT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("prompt         ",ch);
    if (IS_SET(ch->comm,COMM_PROMPT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("color          ",ch);
    if (IS_SET(ch->comm,COMM_ANSI))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("combine items  ",ch);
    if (IS_SET(ch->comm,COMM_COMBINE))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    if (IS_SET(ch->act,PLR_NOSUMMON))
	send_to_char("You cannot be summoned by anyone out of PK.\n\r",ch);
    else
	send_to_char("You can be summoned by anyone.\n\r",ch);
   
    if (IS_SET(ch->act,PLR_NOFOLLOW))
	send_to_char("You do not welcome followers.\n\r",ch);
    else
	send_to_char("You accept followers.\n\r",ch);

	if (IS_SET(ch->act,PLR_NO_TRANSFER))
		send_to_char("You do not accept transfered objects from transfer object spell.\n\r",ch);
	else
		send_to_char("You accept transfered objects from transfer object spell.\n\r",ch);

}

void do_color(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
	return;

    if(IS_SET(ch->comm,COMM_ANSI))
    {
	send_to_char("Color has been removed.\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_ANSI);
    }
    else
    {
	send_to_char("You now see color.\n\r",ch);
        SET_BIT(ch->comm,COMM_ANSI);
    }
}

void do_autoassist(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
    
    if (IS_SET(ch->act,PLR_AUTOASSIST))
    {
      send_to_char("Autoassist removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOASSIST);
    }
    else
    {
      send_to_char("You will now assist when needed.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOASSIST);
    }
}

void do_autoexit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOEXIT))
    {
      send_to_char("Exits will no longer be displayed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOEXIT);
    }
    else
    {
      send_to_char("Exits will now be displayed.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOEXIT);
    }
}

void do_autogold(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOGOLD))
    {
      send_to_char("Autogold removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOGOLD);
    }
    else
    {
      send_to_char("Automatic gold looting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOGOLD);
    }
}

void do_autoloot(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOLOOT))
    {
      send_to_char("Autolooting removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOLOOT);
    }
    else
    {
      send_to_char("Automatic corpse looting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOLOOT);
    }
}

void do_autosac(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOSAC))
    {
      send_to_char("Autosacrificing removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSAC);
    }
    else
    {
      send_to_char("Automatic corpse sacrificing set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSAC);
    }
}

void do_autosplit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
    {
      send_to_char("Autosplitting removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSPLIT);
    }
    else
    {
      send_to_char("Automatic gold splitting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSPLIT);
    }
}

void do_brief(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_BRIEF))
    {
      send_to_char("Full descriptions activated.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_BRIEF);
    }
    else
    {
      send_to_char("Short descriptions activated.\n\r",ch);
      SET_BIT(ch->comm,COMM_BRIEF);
    }
}

void do_compact(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMPACT))
    {
      send_to_char("Compact mode removed.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMPACT);
    }
    else
    {
      send_to_char("Compact mode set.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMPACT);
    }
}

void do_show(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
    {
      send_to_char("Affects will no longer be shown in score.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
    else
    {
      send_to_char("Affects will now be shown in score.\n\r",ch);
      SET_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
}

void do_prompt(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
 
   if ( argument[0] == '\0' )
   {
	if (IS_SET(ch->comm,COMM_PROMPT))
   	{
      	    send_to_char("You will no longer see prompts.\n\r",ch);
      	    REMOVE_BIT(ch->comm,COMM_PROMPT);
    	}
    	else
    	{
      	    send_to_char("You will now see prompts.\n\r",ch);
      	    SET_BIT(ch->comm,COMM_PROMPT);
    	}
       return;
   }
   return;
   if( !strcmp( argument, "all" ) )
      strcpy( buf, "<%hhp %mm %vmv> ");
   else
   {
      if ( strlen(argument) > 50 )
         argument[50] = '\0';
      strcpy( buf, argument );
      smash_tilde( buf );
      if (str_suffix("%c",buf))
	strcat(buf," ");
	
   }
 
   free_string( ch->prompt );
   ch->prompt = str_dup( buf );
   sprintf(buf,"Prompt set to %s\n\r",ch->prompt );
   send_to_char(buf,ch);
   return;
}

void do_combine(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMBINE))
    {
      send_to_char("Long inventory selected.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMBINE);
    }
    else
    {
      send_to_char("Combined inventory selected.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMBINE);
    }
}

void do_noloot(CHAR_DATA *ch, char *argument)
{
    return;
 
    if (IS_SET(ch->act,PLR_CANLOOT))
    {
      send_to_char("Your corpse is now safe from thieves.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_CANLOOT);
    }
    else
    {
      send_to_char("Your corpse may now be looted.\n\r",ch);
      SET_BIT(ch->act,PLR_CANLOOT);
    }
}

void do_nofollow(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *wch;
    if (IS_NPC(ch))
      return;

    if (IS_AFFECTED(ch,AFF_CHARM))
	{
	 send_to_char("Now why would you want to leave your master?\n\r",
ch);
	 return;
	}
 
    if (IS_SET(ch->act,PLR_NOFOLLOW))
    {
      send_to_char("You now accept followers.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_NOFOLLOW);
    }
    else
    {
      send_to_char("You no longer accept followers.\n\r",ch);
      SET_BIT(ch->act,PLR_NOFOLLOW);
      die_follower( ch );
    for (wch = char_list; wch != NULL; wch = wch->next)
    {
    if (wch->life_lined != NULL)
	{
	if (wch->life_lined == ch)
	{
	act("Your lifeline to $N breaks.",ch,0,wch,TO_CHAR);
	wch->life_lined = NULL;
	}
	}
    }
    }	
}

void do_nosummon(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
      if (IS_SET(ch->imm_flags,IMM_SUMMON))
      {
	send_to_char("You may now be summoned by anyone.\n\r",ch);
	REMOVE_BIT(ch->imm_flags,IMM_SUMMON);
      }
      else
      {
	send_to_char("You may no longer be summoned.\n\r",ch);
	SET_BIT(ch->imm_flags,IMM_SUMMON);
      }
    }
    else
    {
      if (IS_SET(ch->act,PLR_NOSUMMON))
      {
        send_to_char("You may now be summoned by anyone.\n\r",ch);
        REMOVE_BIT(ch->act,PLR_NOSUMMON);
      }
      else
      {
        send_to_char("You may only be summoned by those in PK.\n\r",ch);
        SET_BIT(ch->act,PLR_NOSUMMON);
      }
    }
}

void do_glance( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( ch->desc == NULL )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Glance at whom?\n\r", ch );
	return;
    }

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( (victim = get_char_room(ch, argument)) == NULL )
    {
	send_to_char( "That person isn't here.\n\r", ch );
	return;
    }
    if (is_affected(victim,gsn_cloak_form))
    {
        send_to_char("The figure is buried deep within a dark colored cloak.\n\r",ch);
        return;
    }

    show_char_to_char_2(victim,ch);
    return;
}

void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    if ( ch->desc == NULL )
	return;
    one_argument(argument,arg);

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    do_look(ch,arg);
    if ( (obj = get_obj_here(ch,arg)) != NULL)
    {
	switch(obj->item_type)
   	{
	default: 	break;
	case (ITEM_DRINK_CON):
	case (ITEM_CONTAINER):
	case (ITEM_CORPSE_NPC):
	case (ITEM_CORPSE_PC):
	sprintf(buf,"in %s",arg);
	do_look(ch,buf);
	}
    }
    return;
}

void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char *pdesc;
    int door;
    int number,count, light, i;
    OBJ_DATA *lobj;

    if ( ch->desc == NULL )
	return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   room_is_dark( ch->in_room )
    &&   !IS_AFFECTED(ch, AFF_INFRARED) )
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    }
    if (!IS_NPC(ch)
    && IS_SET(ch->in_room->extra_room_flags,ROOM_GLOBE_DARKNESS)
    && str_cmp(pc_race_table[ch->race].name,"dark-elf") )
    {
	light = 0;
	for (i = 0; i < MAX_WEAR; i++)
	{
	lobj = get_eq_char(ch,i);
	if (lobj == NULL)
		continue;
	if (IS_SET(lobj->extra_flags,ITEM_GLOW))
		light++;
	if (lobj->item_type == ITEM_LIGHT)
		light += 3;
	}
	if (light < 8)
	{
	send_to_char( "It is pitch black ... \n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    	}
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    number = number_argument(arg1,arg3);
    count = 0;

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */
	send_to_char( ch->in_room->name, ch );

	if (IS_IMMORTAL(ch) && (IS_NPC(ch) || IS_SET(ch->act,PLR_HOLYLIGHT)))
	{
	    sprintf(buf," [Room %d]",ch->in_room->vnum);
	    send_to_char(buf,ch);
	}

	send_to_char( "\n\r", ch );

	if ( arg1[0] == '\0'
	|| ( !IS_NPC(ch) && !IS_SET(ch->comm, COMM_BRIEF) ) )
	{
	    send_to_char( "  ",ch);
	    send_to_char( ch->in_room->description, ch );
	}

        if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT) )
	{
	    send_to_char("\n\r",ch);
            do_exits( ch, "auto" );
	}

	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );
	show_char_to_char( ch->in_room->people,   ch );
	return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp(arg1, "in")  || !str_cmp(arg1,"on"))
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    sprintf( buf, "It's %sfilled with  a %s liquid.\n\r",
		obj->value[1] <     obj->value[0] / 4
		    ? "less than half-" :
		obj->value[1] < 3 * obj->value[0] / 4
		    ? "about half-"     : "more than half-",
		liq_table[obj->value[2]].liq_color
		);

	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p holds:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );
	return;
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{  /* player can see object */
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}
	    	else continue;

 	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
 	    if ( pdesc != NULL )
 	    	if (++count == number)
 	    	{	
		    send_to_char( pdesc, ch );
		    return;
	     	}
		else continue;

	    if ( is_name( arg3, obj->name ) )
	    	if (++count == number)
	    	{
	    	    send_to_char( obj->description, ch );
	    	    send_to_char( "\n\r",ch);
		    return;
		  }
	  }
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}

	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}

	    if ( is_name( arg3, obj->name ) )
		if (++count == number)
		{
		    send_to_char( obj->description, ch );
		    send_to_char("\n\r",ch);
		    return;
		}
	}
    }

    pdesc = get_extra_descr(arg3,ch->in_room->extra_descr);
    if (pdesc != NULL)
    {
	if (++count == number)
	{
	    send_to_char(pdesc,ch);
	    return;
	}
    }
    
    if (count > 0 && count != number)
    {
    	if (count == 1)
    	    sprintf(buf,"You only see one %s here.\n\r",arg3);
    	else
    	    sprintf(buf,"You only see %d of those here.\n\r",count);
    	
    	send_to_char(buf,ch);
    	return;
    }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }

    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
	send_to_char( "Nothing special there.\n\r", ch );

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR );
	}
    }

    return;
}

/* RT added back for the hell of it */
void do_read (CHAR_DATA *ch, char *argument )
{
    do_look(ch,argument);
}


/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;

    if (fAuto)
	sprintf(buf,"[Exits:");
    else if (IS_IMMORTAL(ch))
	sprintf(buf,"Obvious exits from room %d:\n\r",ch->in_room->vnum);
    else
	sprintf(buf,"Obvious exits:\n\r");

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   can_see_room(ch,pexit->u1.to_room) 
	&&   !IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    found = TRUE;
	    if ( fAuto )
	    {
		strcat( buf, " " );
		strcat( buf, dir_name[door] );
	    }
	    else
	    {
		sprintf( buf + strlen(buf), "%-5s - %s",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->u1.to_room )
			?  "Too dark to tell"
			: pexit->u1.to_room->name
		    );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf), 
			" (room %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	}
    }

    if ( !found )
	strcat( buf, fAuto ? " none" : "None.\n\r" );

    if ( fAuto )
	strcat( buf, "]\n\r" );

    send_to_char( buf, ch );
    return;
}

void do_worth( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
    {
	sprintf(buf,"You have %ld gold and %ld silver.\n\r",
	    ch->gold,ch->silver);
	send_to_char(buf,ch);
	return;
    }

    sprintf(buf, 
    "You have %ld gold, %ld silver, and %d experience (%d exp to level).\n\r",
	ch->gold, ch->silver,ch->exp,
        ch->level * exp_per_level(ch) - ch->exp);

    send_to_char(buf,ch);

    return;
}


void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int i;

    if ( IS_NPC(ch) )
    {
	send_to_char( "Nope, not for NPC's. Try stat <mob>\n\r", ch );
	return;
    }

    sprintf( buf,
	"You are %s%s, level %d, %d years old (%d hours).\n\r",
	ch->original_name ? ch->original_name : ch->name,
	IS_NPC(ch) ? "" : ch->pcdata->title,
	ch->level, get_age(ch),
        ( ch->played + (int) (current_time - ch->logon) ) / 3600);
    send_to_char( buf, ch );
    sprintf(buf,"You are %s.\n\r",get_age_name(ch));
    send_to_char(buf,ch);

    if ( get_trust( ch ) != ch->level )
    {
	sprintf( buf, "You are trusted at level %d.\n\r",
	    get_trust( ch ) );
	send_to_char( buf, ch );
    }

    sprintf(buf, "Race: %s  Sex: %s  Class: %s\n\r",
	race_table[ch->race].name,
	ch->sex == 0 ? "sexless" : ch->sex == 1 ? "male" : "female",
 	IS_NPC(ch) ? "mobile" : class_table[ch->class].name);
    send_to_char(buf,ch);
	
    if(ch->level >= 30)
    {
    sprintf( buf,
	"You have %d/%d hit, %d/%d mana, %d/%d movement.\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move);
    send_to_char( buf, ch );
    }
    else
    {
    int percenta, percentb, percentc;
    if (ch->max_hit == 0)
	percenta = 0;
    else
	percenta = (ch->hit * 100) / ch->max_hit;
    if (ch->max_mana == 0)
	percentb = 0;
    else
        percentb = (ch->mana * 100) / ch->max_mana;
    if (ch->max_move == 0)
 	percentc = 0;
    else
        percentc = (ch->move * 100) / ch->max_move;
    sprintf( buf,
        "You have %d%% hit, %d%% mana, %d%% movement.\n\r",
        percenta,
        percentb,
        percentc);
    send_to_char( buf, ch);
    }
    sprintf( buf,
	"You have %d practices and %d training sessions.\n\r",
	ch->practice, ch->train);
    send_to_char( buf, ch );

    sprintf( buf,
	"You are carrying %d/%d items with weight %ld/%d pounds.\n\r",
	ch->carry_number, can_carry_n(ch),
	get_carry_weight(ch) / 10, can_carry_w(ch) /10 );
    send_to_char( buf, ch );
    if(ch->level >= 20)
    {
    sprintf( buf,
	"Str: %d(%d)  Int: %d(%d)  Wis: %d(%d)  Dex: %d(%d)  Con: %d(%d)\n\r",
	ch->perm_stat[STAT_STR],
	get_curr_stat(ch,STAT_STR),
	ch->perm_stat[STAT_INT],
	get_curr_stat(ch,STAT_INT),
	ch->perm_stat[STAT_WIS],
	get_curr_stat(ch,STAT_WIS),
	ch->perm_stat[STAT_DEX],
	get_curr_stat(ch,STAT_DEX),
	ch->perm_stat[STAT_CON],
	get_curr_stat(ch,STAT_CON) );
    send_to_char( buf, ch );
    }
    else
    {
    sprintf( buf,
        "Str: %d(??)  Int: %d(??)  Wis: %d(??)  Dex: %d(??)  Con: %d(??)\n\r",
        ch->perm_stat[STAT_STR],
        ch->perm_stat[STAT_INT],
        ch->perm_stat[STAT_WIS],
        ch->perm_stat[STAT_DEX],
        ch->perm_stat[STAT_CON] );
    send_to_char( buf, ch );
    }
    sprintf( buf,
	"You have scored %d exp, and have %ld gold and %ld silver coins.\n\r",
	ch->exp,  ch->gold, ch->silver );
    send_to_char( buf, ch );

    /* RT shows exp to level */
    if (!IS_NPC(ch) && ch->level < LEVEL_HERO)
    {
      sprintf (buf, 
	"You need %d exp to level.\n\r",
        (ch->level * exp_per_level(ch) - ch->exp));
      send_to_char( buf, ch );
     }
    if(ch->level >= 30)
    {
    sprintf( buf, "Wimpy set to %d hit points.  Hometown is %s\n\r",ch->wimpy,
(ch->temple == 8839) ? "Elvenhame (good)" :
(ch->temple == 8844) ? "Elvenhame (neutral)" :
(ch->temple == 8848) ? "Elvenhame (evil)" :
(ch->temple == 9609) ? "New thalos" :
(ch->temple == 699 ) ? "Ofcol" :
(ch->temple == 768) ? "Arkham" :
"Midgaard");
    } 
    else
    {
    sprintf( buf, "Wimpy set to %d%% hit points.  Hometown is %s\n\r",ch->wimpy, 
(ch->temple == 8839) ? "Elvenhame (good)" :
(ch->temple == 8844) ? "Elvenhame (neutral)" :
(ch->temple == 8848) ? "Elvenhame (evil)" :
(ch->temple == 9609) ? "New thalos" :
(ch->temple == 699 ) ? "Ofcol" :
(ch->temple == 768) ? "Arkham" :
"Midgaard");
    }
    send_to_char( buf, ch );

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( "You are drunk.\n\r",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] ==  0 )
	send_to_char( "You are thirsty.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER]   ==  0 )
	send_to_char( "You are hungry.\n\r",  ch );

    switch ( ch->position )
    {
    case POS_DEAD:     
	send_to_char( "You are DEAD!!\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "You are mortally wounded.\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "You are incapacitated.\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "You are stunned.\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "You are sleeping.\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_char( "You are resting.\n\r",		ch );
	break;
    case POS_SITTING:
	send_to_char( "You are sitting.\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_char( "You are standing.\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( "You are fighting.\n\r",		ch );
	break;
    }


    /* print AC values */
    if (ch->level >= 25)
    {	
	sprintf( buf,"Armor: pierce: %d  bash: %d  slash: %d  magic: %d\n\r",
		 GET_AC(ch,AC_PIERCE),
		 GET_AC(ch,AC_BASH),
		 GET_AC(ch,AC_SLASH),
		 GET_AC(ch,AC_EXOTIC));
    	send_to_char(buf,ch);
    }

    for (i = 0; i < 4; i++)
    {
	char * temp;

	switch(i)
	{
	    case(AC_PIERCE):	temp = "piercing";	break;
	    case(AC_BASH):	temp = "bashing";	break;
	    case(AC_SLASH):	temp = "slashing";	break;
	    case(AC_EXOTIC):	temp = "magic";		break;
	    default:		temp = "error";		break;
	}
	
	send_to_char("You are ", ch);

	if      (GET_AC(ch,i) >=  101 ) 
	    sprintf(buf,"hopelessly vulnerable to %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 80) 
	    sprintf(buf,"defenseless against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 60)
	    sprintf(buf,"barely protected from %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 40)
	    sprintf(buf,"slightly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 20)
	    sprintf(buf,"somewhat armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 0)
	    sprintf(buf,"armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -20)
	    sprintf(buf,"well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -40)
	    sprintf(buf,"very well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -60)
	    sprintf(buf,"heavily armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -80)
	    sprintf(buf,"superbly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -100)
	    sprintf(buf,"almost invulnerable to %s.\n\r",temp);
        else if (GET_AC(ch,i) >= -150)
	    sprintf(buf,"divinely armored against %s.\n\r",temp);
        else if (GET_AC(ch,i) >= -200)
            sprintf(buf,"supremely armored against %s.\n\r",temp);
        else
            sprintf(buf,"god-like against %s.\n\r",temp);

	send_to_char(buf,ch);
    }


    /* RT wizinvis and holy light */
    if ( IS_IMMORTAL(ch))
    {
      send_to_char("Holy Light: ",ch);
      if (IS_SET(ch->act,PLR_HOLYLIGHT))
        send_to_char("on",ch);
      else
        send_to_char("off",ch);
 
      if (ch->invis_level)
      {
        sprintf( buf, "  Invisible: level %d",ch->invis_level);
        send_to_char(buf,ch);
      }

      if (ch->incog_level)
      {
	sprintf(buf,"  Incognito: level %d",ch->incog_level);
	send_to_char(buf,ch);
      }
      send_to_char("\n\r",ch);
    }

    if ( ch->level >= 15 )
    {
	sprintf( buf, "Hitroll: %d  Damroll: %d. ",
	    GET_HITROLL(ch), GET_DAMROLL(ch) );
	send_to_char( buf, ch );
	if (ch->level < 39)
	send_to_char("\n\r",ch);
    }
    if (ch->level >= 39)
    {
	sprintf(buf,"Saves spell: %d.", ch->saving_throw);
	send_to_char(buf,ch);
    send_to_char("\n\r",ch);
    }

    send_to_char( "You are ", ch );
         if ( ch->alignment == 1000 ) send_to_char( "good,", ch );
    else if ( ch->alignment == 0 ) send_to_char( "neutral,", ch );
    else                             send_to_char( "evil,", ch );
    send_to_char( " and have a ", ch );
         if ( ch->pcdata->ethos == 1000 ) send_to_char( "lawful ethos.\n\r", ch );
    else if ( ch->pcdata->ethos == 0 ) send_to_char( "neutral ethos.\n\r", ch );
    else                               send_to_char( "chaotic ethos.\n\r", ch );

    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
	do_affects(ch,"");
}

void do_affects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];
    
    if ( ch->affected != NULL )
    {
	send_to_char( "You are affected by the following spells:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    if (paf_last != NULL && paf->type == paf_last->type)
		if (ch->level >= 20)
		    sprintf( buf, "                      ");
		else
		    continue;
	    else
	    	sprintf( buf, "Spell: %-15s", skill_table[paf->type].name );

	    send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
		sprintf( buf,
		    ": modifies %s by %d ",
		    affect_loc_name( paf->location ),
		    paf->modifier);
		send_to_char( buf, ch );
		if ( paf->duration == -1 )
		    sprintf( buf, "permanently" );
		else
		    sprintf( buf, "for %d hours", paf->duration );
		send_to_char( buf, ch );
	    }

	    send_to_char( "\n\r", ch );
	    paf_last = paf;
	}
    }
    else 
	send_to_char("You are not affected by any spells.\n\r",ch);

    return;
}



char *	const	day_name	[] =
{
    "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};

char *	const	month_name	[] =
{
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    sprintf( buf,
	"It is %d o'clock %s, Day of %s, %d%s the Month of %s.\n\r",
	(time_info.hour % 12 == 0) ? 12 : time_info.hour %12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month]);
    send_to_char(buf,ch);
    sprintf(buf,"Tartarus started up at %s\n\rThe system time is %s.\n\r",
	str_boot_time,
	(char *) ctime( &current_time )
	);

    send_to_char( buf, ch );
    return;
}



void do_weather( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    static char * const sky_look[4] =
    {
	"cloudless",
	"cloudy",
	"rainy",
	"lit by flashes of lightning"
    };

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You can't see the weather indoors.\n\r", ch );
	return;
    }

    sprintf( buf, "The sky is %s and %s.\n\r",
	sky_look[weather_info.sky],
	weather_info.change >= 0
	? "a warm southerly breeze blows"
	: "a cold northern gust blows"
	);
    send_to_char( buf, ch );
    return;
}

void do_help( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    BUFFER *output;
    bool found = FALSE;
    char argall[MAX_INPUT_LENGTH],argone[MAX_INPUT_LENGTH];
    int level;

    output = new_buf();

    if ( argument[0] == '\0' )
	argument = "summary";

    /* this parts handles help a b so that it returns help 'a b' */
    argall[0] = '\0';
    while (argument[0] != '\0' )
    {
	argument = one_argument(argument,argone);
	if (argall[0] != '\0')
	    strcat(argall," ");
	strcat(argall,argone);
    }

    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
    	level = (pHelp->level < 0) ? -1 * pHelp->level - 1 : pHelp->level;

	if (level > get_trust( ch ) )
	{
	if (pHelp->level < 61)
		continue;
	if ((pHelp->level == 61 && ch->clan != CLAN_ANCIENT)
	|| ( pHelp->level == 62 && ch->clan != CLAN_ARCANA)
	|| ( pHelp->level == 63 && ch->clan != CLAN_LIFE)
	|| ( pHelp->level == 64 && ch->clan != CLAN_OUTLAW)
	|| ( pHelp->level == 65 && ch->clan != CLAN_ENFORCER)
	|| ( pHelp->level == 66 && ch->clan != CLAN_CRUSADER) )
	    continue;
    	}

	if ( is_name( argall, pHelp->keyword ) )
	{
	    /* add seperator if found */
	    if (found)
		add_buf(output,
    "\n\r============================================================\n\r\n\r");
	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {
		add_buf(output,pHelp->keyword);
		add_buf(output,"\n\r");
	    }

	    /*
	     * Strip leading '.' to allow initial blanks.
	     */
	    if ( pHelp->text[0] == '.' )
		add_buf(output,pHelp->text+1);
	    else
		add_buf(output,pHelp->text);
	    found = TRUE;
	    /* small hack :) */
	    if (ch->desc != NULL && ch->desc->connected != CON_PLAYING 
	    &&  		    ch->desc->connected != CON_GEN_GROUPS)
		break;
	}
    }

    if (!found)
    	send_to_char( "No help on that word.\n\r", ch );
    else
	page_to_char(buf_string(output),ch);
    free_buf(output);
}


/* whois command */
void do_whois (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    BUFFER *output;
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    bool found = FALSE;

    one_argument(argument,arg);
  
    if (arg[0] == '\0')
    {
        send_to_char("Whois who?\n\r",ch);
	return;
    }

    output = new_buf();

    for (d = descriptor_list; d != NULL; d = d->next)
    {
	CHAR_DATA *wch;
	char const *class;

 	if (d->connected != CON_PLAYING || !can_see(ch,d->character))
	    continue;
	
	wch = ( d->original != NULL ) ? d->original : d->character;

 	if (!can_see(ch,wch))
	    continue;

	if ((wch->original_name) ? IS_IMMORTAL(ch) ?
		!str_prefix(arg,wch->original_name) : 0 :
		!str_prefix(arg,wch->name)) {
	    found = TRUE;
	    
	    /* work out the printing */
	    class = class_table[wch->class].who_name;
	    switch(wch->level)
	    {
		case MAX_LEVEL - 0 : class = "IMP"; 	break;
		case MAX_LEVEL - 1 : class = "CRE";	break;
		case MAX_LEVEL - 2 : class = "SUP";	break;
		case MAX_LEVEL - 3 : class = "DEI";	break;
		case MAX_LEVEL - 4 : class = "GOD";	break;
		case MAX_LEVEL - 5 : class = "IMM";	break;
		case MAX_LEVEL - 6 : class = "DEM";	break;
		case MAX_LEVEL - 7 : class = "ANG";	break;
		case MAX_LEVEL - 8 : class = "AVA";	break;
	    }
    
	    /* a little formatting */
            if (wch->level >= 52 || wch == ch || IS_IMMORTAL(ch))
            {
            sprintf(buf, "[%2d %5s %s] %s%s%s%s%s%s%s%s%s%s\n\r",
                wch->level,
		wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name
					: "     ",
                class,
             can_pk(ch, wch) ? "(PK) " : "",
	     wch->incog_level >= LEVEL_HERO ? "(Incog) ": "",
 	     wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
             clan_table[wch->clan].who_name,
	     IS_SET(wch->comm, COMM_AFK) ? "[AFK] " : "",
	     IS_SET(wch->act,PLR_CRIMINAL) ? "(CRIMINAL) " : "",
             IS_SET(wch->act,PLR_THIEF) ? "(THIEF) " : "",
		(wch->original_name) ? wch->original_name : wch->name,
 		IS_NPC(wch) ? "" : wch->pcdata->title,
	IS_NPC(wch) ? "" : 
	(wch->pcdata->extitle) ? wch->pcdata->extitle : "");
	    add_buf(output,buf);
            }
            else
            {
            sprintf(buf, "[   %5s    ] %s%s%s%s%s%s%s%s%s%s\n\r",
                wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name
					: "     ",
             can_pk(ch, wch) ? "(PK) " : "",
             wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
 	     wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
	     wch->clan == CLAN_ENFORCER ?
	     clan_table[wch->clan].who_name :
             (wch->clan == ch->clan) ?
	     clan_table[wch->clan].who_name : "",
	     IS_SET(wch->comm, COMM_AFK) ? "[AFK] " : "",
             IS_SET(wch->act,PLR_CRIMINAL) ? "(CRIMINAL) " : "",
             IS_SET(wch->act,PLR_THIEF) ? "(THIEF) " : "",
		wch->name, IS_NPC(wch) ? "" : wch->pcdata->title,
	IS_NPC(wch) ? "" : 
	(wch->pcdata->extitle) ? wch->pcdata->extitle : ""
	);
	    add_buf(output,buf);
            }
	}
    }

    if (!found)
    {
	send_to_char("No one of that name is playing.\n\r",ch);
	return;
    }

    page_to_char(buf_string(output),ch);
    free_buf(output);
}


/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    BUFFER *output;
    DESCRIPTOR_DATA *d;
    int iClass;
    int iRace;
    int iClan;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    int anc=0;
    bool rgfClass[MAX_CLASS];
    bool rgfRace[MAX_PC_RACE];
    bool rgfClan[MAX_CLAN];
    bool fCriminal = FALSE;
    bool fClassRestrict = FALSE;
    bool fClanRestrict = FALSE;
    bool fClan = FALSE;
    bool fRaceRestrict = FALSE;
    bool fImmortalOnly = FALSE;
    bool fPkOnly = FALSE;
    bool fBuilder = FALSE;
 
    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
        rgfClass[iClass] = FALSE;
    for ( iRace = 0; iRace < MAX_PC_RACE; iRace++ )
        rgfRace[iRace] = FALSE;
    for (iClan = 0; iClan < MAX_CLAN; iClan++)
        rgfClan[iClan] = FALSE;
 
    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
        char arg[MAX_STRING_LENGTH];
 
        argument = one_argument( argument, arg );
        if ( arg[0] == '\0' )
            break;
 
        if ( is_number( arg ) )
        {
                            send_to_char(
                            "That's not a valid race or clan.\n\r",
                               ch);
                            return;
        }
        else
        {
 
            /*
             * Look for classes to turn on.
             */
            if (!str_prefix(arg,"immortals"))
            {
                fImmortalOnly = TRUE;
            }
            else if (!str_prefix(arg,"pk"))
            {
                fPkOnly = TRUE;
            }
	    else if (!str_prefix(arg,"wanted") || !str_prefix(arg,"criminal"))
	    {
		fCriminal = TRUE;
	    }
	    else if (!str_prefix(arg,"builder") && IS_IMMORTAL(ch))
	    {
		fBuilder = TRUE;
	    }
            else
            {
                iClass = class_lookup(arg);
                if (iClass == -1)
                {
                    iRace = race_lookup(arg);
 
                    if (iRace == 0 || iRace >= MAX_PC_RACE)
		    {
                        iClan = clan_lookup(arg);
                        if (iClan)
                        {
                            if (iClan == ch->clan 
				|| IS_IMMORTAL(ch) )
                            {
                                fClanRestrict = TRUE;
                                rgfClan[iClan] = TRUE;
                            }
                            else
                            {
                                send_to_char(
                                "You're not a member of that clan.\n\r",
                                   ch);
                                return;
                            }
                        }
                        else
                        {
                            send_to_char(
                            "That's not a valid race or clan.\n\r",
                               ch);
                            return;
                        } 
		    }
                    else
                    {
                        fRaceRestrict = TRUE;
                        rgfRace[iRace] = TRUE;
                    }
                }
                else
                {
		if (IS_IMMORTAL(ch))
		{
                    fClassRestrict = TRUE;
                    rgfClass[iClass] = TRUE;
		}
		else
		   {
                            send_to_char(
                            "That's not a valid race or clan.\n\r",
                               ch);
                            return;
		   }
                }
            }
        }
    }
 
    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    output = new_buf();
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        CHAR_DATA *wch;
        char const *class;
 
        /*
         * Check for match against restrictions.
         * Don't use trust as that exposes trusted mortals.
         */
        if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
            continue;
 
        wch   = ( d->original != NULL ) ? d->original : d->character;

	if (!can_see(ch,wch)) continue;

	if(!IS_IMMORTAL(ch)&&is_affected(wch,gsn_cloak_form)&&ch!=wch
	   && (!((ch->clan == CLAN_ANCIENT) && (ch->pcdata->induct == 5))))
	{ 
	    if(can_pk(ch,wch)) anc=1;
	    continue;
	}

        if ( wch->level < iLevelLower
        ||   wch->level > iLevelUpper
        || ( fImmortalOnly  && wch->level < LEVEL_IMMORTAL )
        || ( fPkOnly && !can_pk(ch, wch) )
        || ( fClassRestrict && !rgfClass[wch->class] )
        || ( fRaceRestrict && !rgfRace[wch->race])
	|| ( fCriminal && !IS_SET(wch->act,PLR_CRIMINAL))
	|| ( fBuilder && !IS_SET(wch->comm,COMM_BUILDER))
        || ( fClan && !is_clan(wch) && !IS_IMMORTAL(ch))
        || ( fClanRestrict && !rgfClan[wch->clan]))
            continue;
 
        nMatch++;
 
        /*
         * Figure out what to print for class.
	 */
	class = class_table[wch->class].who_name;
	switch ( wch->level )
	{
	default: break;
            {
                case MAX_LEVEL - 0 : class = "IMP";     break;
                case MAX_LEVEL - 1 : class = "CRE";     break;
                case MAX_LEVEL - 2 : class = "SUP";     break;
                case MAX_LEVEL - 3 : class = "DEI";     break;
                case MAX_LEVEL - 4 : class = "GOD";     break;
                case MAX_LEVEL - 5 : class = "IMM";     break;
                case MAX_LEVEL - 6 : class = "DEM";     break;
                case MAX_LEVEL - 7 : class = "ANG";     break;
                case MAX_LEVEL - 8 : class = "AVA";     break;
            }
	}

	/*
	 * Format it up.
	 */
        if (wch->level >= 52 || IS_IMMORTAL(ch) || wch == ch)
        {
        sprintf( buf, "[%2d %5s %s] %s%s%s%s%s%s%s%s%s%s\n\r",
	    wch->level,
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name 
				    : "     ",
	    class,
            can_pk(ch, wch) ? "(PK) " : "",
	    wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
	    wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
            clan_table[wch->clan].who_name,
	    IS_SET(wch->comm, COMM_AFK) ? "[AFK] " : "",
            IS_SET(wch->act, PLR_CRIMINAL) ? "(CRIMINAL) " : "",
            IS_SET(wch->act, PLR_THIEF)  ? "(THIEF) "  : "",
	    (wch->original_name) ? wch->original_name : wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title ,
	IS_NPC(wch) ? "" : 
	(wch->pcdata->extitle) ? wch->pcdata->extitle : "");
	add_buf(output,buf);
 	} 
        else 
        {
        sprintf( buf, "[   %5s    ] %s%s%s%s%s%s%s%s%s%s\n\r",
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name 
				    : "     ",
            can_pk(ch, wch) ? "(PK) " : "",
	    wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
	    wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
	    wch->clan == CLAN_ENFORCER ?
	    clan_table[wch->clan].who_name : 
            (wch->clan == ch->clan) ?
	     clan_table[wch->clan].who_name : "",
	    IS_SET(wch->comm, COMM_AFK) ? "[AFK] " : "",
            IS_SET(wch->act, PLR_CRIMINAL) ? "(CRIMINAL) " : "",
            IS_SET(wch->act, PLR_THIEF)  ? "(THIEF) "  : "",
	    ((ch->clan == CLAN_ANCIENT) && (wch->original_name) &&
(ch->pcdata->induct==5)) ? wch->original_name : wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title, 
	IS_NPC(wch) ? "" : 
	(wch->pcdata->extitle) ? wch->pcdata->extitle : ""
);
	add_buf(output,buf);
        }
    }

    sprintf( buf2, "\n\rPlayers found: %d\n\r", nMatch );
    add_buf(output,buf2);
    if(anc==1) {
	sprintf(buf,"Ancient is on the prowl.\n\r");
	add_buf(output,buf);
    }
    page_to_char( buf_string(output), ch );
    free_buf(output);
    return;
}

void do_count ( CHAR_DATA *ch, char *argument )
{
    int count;
    DESCRIPTOR_DATA *d;
    int not_seen;
    char buf[MAX_STRING_LENGTH];

    not_seen = 0;
    count = 0;

    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected == CON_PLAYING)
	{
	 if ( can_see( ch, d->character ) )
	    count++;
	 else
	    not_seen++;
    	}

    if ( (count + not_seen) >= count_data[0])
	count_data[0] = (count + not_seen);

    if (count >= count_data[0])
        sprintf(buf,"There are %d characters on, the most so far today.\n\r",
	    count);
    else
	sprintf(buf,"There are %d characters on, the most on today was %d.\n\r",
	    count,count_data[0]);
    send_to_char(buf,ch);

    if (not_seen != 0)
    {
	sprintf(buf,"There are %d characters on that you can not see.\n\r",not_seen);
    send_to_char(buf,ch);
}
    return;
    
}

void do_inventory( CHAR_DATA *ch, char *argument )
{
    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	    continue;

	send_to_char( where_name[iWear], ch );
	if ( can_see_obj( ch, obj ) )
	{
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
	else
	{
	    send_to_char( "something.\n\r", ch );
	}
	found = TRUE;
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );

    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Compare what to what?\n\r", ch );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if (arg2[0] == '\0')
    {
	for (obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content)
	{
	    if (obj2->wear_loc != WEAR_NONE
	    &&  can_see_obj(ch,obj2)
	    &&  obj1->item_type == obj2->item_type
	    &&  (obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
		break;
	}

	if (obj2 == NULL)
	{
	    send_to_char("You aren't wearing anything comparable.\n\r",ch);
	    return;
	}
    } 

    else if ( (obj2 = get_obj_carry(ch,arg2,ch) ) == NULL )
    {
	send_to_char("You do not have that item.\n\r",ch);
	return;
    }

    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = "You compare $p to itself.  It looks about the same.";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
	msg = "You can't compare $p and $P.";
    }
    else
    {
	switch ( obj1->item_type )
	{
	default:
	    msg = "You can't compare $p and $P.";
	    break;

	case ITEM_ARMOR:
	    value1 = obj1->value[0] + obj1->value[1] + obj1->value[2];
	    value2 = obj2->value[0] + obj2->value[1] + obj2->value[2];
	    break;

	case ITEM_WEAPON:
	    if (obj1->pIndexData->new_format)
		value1 = (1 + obj1->value[2]) * obj1->value[1];
	    else
	    	value1 = obj1->value[1] + obj1->value[2];

	    if (obj2->pIndexData->new_format)
		value2 = (1 + obj2->value[2]) * obj2->value[1];
	    else
	    	value2 = obj2->value[1] + obj2->value[2];
	    break;
	}
    }

    if ( msg == NULL )
    {
	     if ( value1 == value2 ) msg = "$p and $P look about the same.";
	else if ( value1  > value2 ) msg = "$p looks better than $P.";
	else                         msg = "$p looks worse than $P.";
    }

    act( msg, ch, obj1, obj2, TO_CHAR );
    return;
}



void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}



void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument( argument, arg );

    if (IS_AFFECTED(ch,AFF_BLIND))
    {
	send_to_char("You can't see anything!",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Players near you:\n\r", ch );
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   !IS_SET(victim->in_room->room_flags,ROOM_NOWHERE)
 	    &&   (is_room_owner(ch,victim->in_room) 
	    ||    !room_is_private(victim->in_room))
	    &&   victim->in_room->area == ch->in_room->area
	    &&   can_see( ch, victim ) )
	    {
		found = TRUE;
		sprintf( buf, "%s%-28s %s\n\r",
		    can_pk(ch,victim) ? "(PK) " : "", victim->name,victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else if (!str_prefix(arg,"pk"))
    {
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   !IS_SET(victim->in_room->room_flags,ROOM_NOWHERE)
 	    &&   (is_room_owner(ch,victim->in_room) 
	    ||    !room_is_private(victim->in_room))
	    &&   victim->in_room->area == ch->in_room->area
	    &&   can_pk(ch,victim)
	    &&   can_see( ch, victim ) )
	    {
		found = TRUE;
		sprintf( buf, "%s%-28s %s\n\r",
		    can_pk(ch,victim) ? "(PK) " : "", victim->name,victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    act( "You didn't find anyone in PK.", ch, NULL, arg, TO_CHAR );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		sprintf( buf, "%s%-28s %s\n\r",
		    can_pk(ch,victim) ? "(PK) " : "", PERS(victim, ch),victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}




void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    int diff;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
	send_to_char("Don't even think about it.\n\r",ch);
	return;
    }

    diff = victim->level - ch->level;

         if ( diff <= -10 )
	     sprintf( buf, "Your sneeze would kill %s.\n\r", IS_NPC(victim) ? 
		victim->short_descr : victim->name );
    else if ( diff <=  -5 )
	     sprintf( buf, "%s wouldn't last long against you.\n\r", 
		IS_NPC(victim) ? victim->short_descr : victim->name );
    else if ( diff <=  -2 )
	     sprintf( buf, "%s looks like an easy kill.\n\r", IS_NPC(victim) ? 
		victim->short_descr : victim->name );
    else if ( diff <=   1 )
	     sprintf( buf, "The perfect match!\n\r" );
    else if ( diff <=   4 )
	     sprintf( buf, "%s looks just a little tough.\n\r", IS_NPC(victim) 
		? victim->short_descr : victim->name );
    else if ( diff <=   9 )
	     sprintf( buf, "%s wouldn't need much help to kill you.\n\r", 
		IS_NPC(victim) ? victim->short_descr : victim->name );
    else                   
 	     sprintf( buf, "You must have a fascination with death.\n\r" );

send_to_char( buf, ch );

         if ( victim->alignment >=  1 ) msg = "$N smiles happily at you.";
    else if ( victim->alignment <= -1 ) msg = "$N grins evilly at you.";
    else			        msg = "$N seems indifferent towards you."; 

    act( msg, ch, NULL, victim, TO_CHAR );
    return;
}



void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( title[0] != '.' && title[0] != ',' && title[0] != '!' && title[0] != '?' )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );
    return;
}



void do_title( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char *pArg;
    char cEnd;

    if ( IS_NPC(ch) || ch->level < 52 )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Change who's title to what?\n\r", ch );
	return;
    }

    if ( (victim = get_char_world(ch, arg1)) == NULL)
    {
    	send_to_char( "Player isn't on.\n\r", ch );
	return;
    }
    
    if ( IS_NPC(victim) )
    {
	send_to_char( "Trying to change a mob's title?\n\r", ch );
	return;
    }

    if ( ch != victim && get_trust(victim) >= get_trust(ch) )
    {
  	send_to_char( "You can't do that to that person.\n\r", ch );
	return;
    }

    if ( strlen(argument) > 45 )
	argument[45] = '\0';

    smash_tilde( argument );
    set_title( victim, argument );
    send_to_char( "Ok.\n\r", ch );
}

void set_extitle(CHAR_DATA *ch, char *title) {
    char buf[MAX_STRING_LENGTH];

    if(IS_NPC(ch)) {
        bug("Set_extitle: NPC.", 0);
        return;
    }

    if(title[0]!='.'&&title[0]!=','&&title[0]!='!'&&title[0]!='?') {
        buf[0] = ' ';
        strcpy(buf+1, title);
    } else {
        strcpy(buf, title);
    }

    free_string(ch->pcdata->extitle);
    ch->pcdata->extitle = str_dup(buf);
    return;
}

void do_extitle( CHAR_DATA *ch, char *argument ) {
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char *pArg;
    char cEnd;

    if (IS_NPC(ch))
        return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
        argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
        cEnd = *argument++;

    while ( *argument != '\0' )
    {
        if ( *argument == cEnd )
        {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0') 
    {
        send_to_char( "Change who's extra title to what?\n\r", ch );
        return;
    }

    if ( (victim = get_char_world(ch, arg1)) == NULL)
    {
        send_to_char( "That player isn't on.\n\r", ch );
        return;
    }
    
    if ( IS_NPC(victim) )
    {
        send_to_char( "Trying to change a mob's extra title?\n\r", ch );
        return;
    }

    if ( ch != victim && get_trust(victim) >= get_trust(ch) )
    {
        send_to_char( "You can't do that to that person.\n\r", ch );
        return;
    }

    if(argument[0]=='\0') {
	free_string(ch->pcdata->extitle);
	ch->pcdata->extitle=NULL;
	send_to_char("Extitle cleared.\n\r",ch);
	return;
    }	

    if ( strlen(argument) > 45 )
        argument[45] = '\0';

    smash_tilde( argument );
    set_extitle( victim, argument );
    send_to_char( "Ok.\n\r", ch );
}


void do_description( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
	return;

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	smash_tilde( argument );

    	if (argument[0] == '-')
    	{
            int len;
            bool found = FALSE;
 
            if (ch->description == NULL || ch->description[0] == '\0')
            {
                send_to_char("No lines left to remove.\n\r",ch);
                return;
            }
	
  	    strcpy(buf,ch->description);
 
            for (len = strlen(buf); len > 0; len--)
            {
                if (buf[len] == '\r')
                {
                    if (!found)  /* back it up */
                    {
                        if (len > 0)
                            len--;
                        found = TRUE;
                    }
                    else /* found the second one */
                    {
                        buf[len + 1] = '\0';
			free_string(ch->description);
			ch->description = str_dup(buf);
			send_to_char( "Your description is:\n\r", ch );
			send_to_char( ch->description ? ch->description : 
			    "(None).\n\r", ch );
                        return;
                    }
                }
            }
            buf[0] = '\0';
	    free_string(ch->description);
	    ch->description = str_dup(buf);
	    send_to_char("Description cleared.\n\r",ch);
	    return;
        }
	if ( argument[0] == '+' )
	{
	    if ( ch->description != NULL )
		strcat( buf, ch->description );
	    argument++;
	    while ( isspace(*argument) )
		argument++;
	}

        if ( strlen(buf) >= 2048)
	{
	    send_to_char( "Description too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description );
	ch->description = str_dup( buf );
    }

    send_to_char( "Your description is:\n\r", ch );
    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
    return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    int percenta, percentb, percentc;

    if (ch->max_hit == 0)
	percenta = 0;
    else
	percenta = (ch->hit * 100) / ch->max_hit;
    if (ch->max_mana == 0)
	percentb = 0;
    else
        percentb = (ch->mana * 100) / ch->max_mana;
    if (ch->max_move == 0)
 	percentc = 0;
    else
        percentc = (ch->move * 100) / ch->max_move;

    sprintf( buf,
	"You say 'I have %d%% hp %d%% mana %d%% mv %d xp.'\n\r",
	percenta, percentb,percentc, ch->exp);
    send_to_char( buf, ch );

    sprintf( buf, "$n says 'I have %d%% hp %d%% mana %d%% mv %d xp.'",
    	percenta, percentb, percentc, ch->exp);

    act( buf, ch, NULL, NULL, TO_ROOM );

    return;
}



void do_practice( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0')
    {
	int col;

	col    = 0;
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;

	    if (skill_table[sn].skill_level[ch->class] > 52)
		continue;
	    if ( ch->level < skill_table[sn].skill_level[ch->class] 
	      || ch->pcdata->learned[sn] < 1 /* skill is not known */)
		continue;

	    sprintf( buf, "%-18s %3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
	    send_to_char( buf, ch );
	    if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );
	}

	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

	sprintf( buf, "You have %d practice sessions left.\n\r",
	    ch->practice );
	send_to_char( buf, ch );
	if (IS_IMMORTAL(ch))
	send_to_char("Use 'set' to change class and see a prac list for other classes.\n\r",ch);
    }
    else
    {
	CHAR_DATA *mob;
	int adept;

	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

	for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )
	{
	    if ( IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) )
		break;
	}

	if ( mob == NULL )
	{
	    send_to_char( "You can't do that here.\n\r", ch );
	    return;
	}

	if ( ch->practice <= 0 )
	{
	    send_to_char( "You have no practice sessions left.\n\r", ch );
	    return;
	}

	if ( ( sn = find_spell( ch,argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   (ch->level < skill_table[sn].skill_level[ch->class] 
 	||    ch->pcdata->learned[sn] < 1 /* skill is not known */
	||    skill_table[sn].rating[ch->class] == 0)))
	{
	    send_to_char( "You can't practice that.\n\r", ch );
	    return;
	}

	adept = IS_NPC(ch) ? 100 : class_table[ch->class].skill_adept;

	if ( ch->pcdata->learned[sn] >= adept )
	{
	    sprintf( buf, "You are already learned at %s.\n\r",
		skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    ch->practice--;
	    ch->pcdata->learned[sn] += 
		int_app[get_curr_stat(ch,STAT_INT)].learn / 
	        skill_table[sn].rating[ch->class];
	    if ( ch->pcdata->learned[sn] < adept )
	    {
		act( "You practice $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
		act( "$n practices $T.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	    else
	    {
		ch->pcdata->learned[sn] = adept;
		act( "You are now learned at $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
		act( "$n is now learned at $T.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	}
    }
    return;
}



/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
        {
        if(ch->level >= 30)
	wimpy = ch->max_hit / 5;
        else
        wimpy = 20;
        }
    else
	wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }
    if (ch->level >= 30)
    {
    if ( wimpy > ch->max_hit/2 )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }

    ch->wimpy	= wimpy;
    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    }
    else
    {
    if ( wimpy > 50)
    {
        send_to_char( "Such cowardice ill becomes you.\n\r", ch );
        return;
    }
    ch->wimpy = wimpy;
    sprintf( buf, "Wimpy set to %d%% hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    }
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, ch->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( ch );
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_revert(CHAR_DATA *ch,char *argument)
{

shapeshift_revert(ch);
return;
    if (!is_affected(ch,gsn_morph_red) && !is_affected(ch,gsn_morph_blue)
	&& !is_affected(ch,gsn_morph_black)
	&& !is_affected(ch,gsn_morph_green)
	&& !is_affected(ch,gsn_morph_white) )
    {
        send_to_char("But you are already in your normal form.\n\r",ch);
        return;
    }
    send_to_char("Your mind clouds over as you force your body to change.\n\r",ch);
    send_to_char("With fluid motion you revert back to your natural form!\n\r",ch);
    act("$n seems to shift and lose solidity as $s reforms.",ch,NULL,NULL,TO_ROOM);

    affect_strip(ch,gsn_morph_red);
    affect_strip(ch,gsn_morph_black);
    affect_strip(ch,gsn_morph_blue);
    affect_strip(ch,gsn_morph_green);
    affect_strip(ch,gsn_morph_white);

    ch->morph_form[0] = 0;
    ch->morph_form[1] = 0;
    ch->morph_form[2] = 0;
    act("With a slight shiver $n reverts to $s natural form!",ch,NULL,NULL,TO_ROOM);
    return;
}

void do_morph_dragon(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;
    char form_type[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int morph_sn;
    int form_num, breath_num;
    struct lvl_check
    {
        char *color;
        int min_level;
    };
    static const struct lvl_check form_check[] =
    {
        {" ", 0},
        {"red", 40},
        {"black", 53},
        {"blue", 53},
        {"white", 53},
        {"green", 53},
	{"goat",61},
	{"lion",61},
	{"dragon",61},
	{"cloaked",61},
	{"arm",15},
	{"bat",10},
	{"wolf",20},
	{"bear",30},
    {"gaseous", 53},
        {"", 0}
    };

    one_argument(argument, form_type);
    if (!str_cmp(form_type,"red"))
    {
        breath_num = skill_lookup("fire breath");
        form_num = 1;
	morph_sn = gsn_morph_red;
    }
    else if (!str_cmp(form_type,"black"))
    {
        breath_num = skill_lookup("acid breath");
        form_num = 2;
	morph_sn = gsn_morph_black;
    }
    else if (!str_cmp(form_type,"blue"))
    {
        breath_num = skill_lookup("lightning breath");
        form_num = 3;	
	morph_sn = gsn_morph_blue;

    }
    else if (!str_cmp(form_type,"white"))
    {
        breath_num = skill_lookup("frost breath");
        form_num = 4;
	morph_sn = gsn_morph_white;
    }
    else if (!str_cmp(form_type,"green"))
    {
        breath_num = skill_lookup("gas breath");
        form_num = 5;
	morph_sn = gsn_morph_green;
    }
    else if (!str_cmp(form_type,"arm"))
	{
	breath_num = 0;
	form_num = MORPH_ARM;
	morph_sn = gsn_shapeshift;
	}
    else if (!str_cmp(form_type,"bat"))
	{
	breath_num = 0;
	form_num = MORPH_BAT;
	morph_sn = gsn_shapeshift;
	}
    else if (!str_cmp(form_type,"wolf"))
	{
	breath_num = 0;
	form_num = MORPH_WOLF;
	morph_sn = gsn_shapeshift;
	}
    else if (!str_cmp(form_type,"bear"))
	{
	breath_num = 0;
	form_num = MORPH_BEAR;
	morph_sn = gsn_shapeshift;
	}
    else if (!str_cmp(form_type,"revert"))
    {
        do_revert(ch,"");
        return;
    }
    else
    {
	form_num = -1;
	breath_num = 0;
	morph_sn = 0;	
    }

if (!can_shapeshift(ch,form_num))
	return;

shapeshift_revert(ch);

    if (ch->level < form_check[form_num].min_level)
    {
        send_to_char("You are not skilled enough to take that form yet.\n\r",ch);
        return;
    }

    send_to_char("You go into a trance as your form shifts.\n\r",ch);

    if (number_percent() > get_skill(ch,morph_sn) )
    {
        send_to_char("With a shudder you lose concentration and fail the shapeshifting.\n\r",ch);
        ch->mana -= 50;
        return;
    }

    if (ch->morph_form[0] != 0)
    {
	return;
        act("$n seems to shift and fluidly change.",ch,NULL,NULL,TO_ROOM);
        send_to_char("You concentrate a moment and fluidly change your form!\n\r",ch);
    }
    else
    {
    act("$n's body suddenly seems to flow into a new shape!",ch,NULL,NULL,TO_ROOM);
    sprintf( buf, "You concentrate a moment and fluidly change your form!\n\r");
    send_to_char( buf,ch);
    }
    affect_strip(ch,morph_sn);
    if (form_num > MORPH_GASEOUS)
        form_num = 0;
    affect_strip(ch,gsn_shapeshift);
    affect_strip(ch,gsn_gaseous_form);

    WAIT_STATE(ch,skill_table[morph_sn].beats);
    ch->mana -= 50;
    ch->morph_form[1] = breath_num;
    ch->morph_form[2] = ch->level/5;
    if (form_num == 0)
        return;

    af.where = TO_AFFECTS;
    af.type = morph_sn;
    af.level = ch->level;
    af.bitvector = 0;
    af.duration = -1;
    af.location = APPLY_MORPH_FORM;
    af.modifier = form_num;
    affect_to_char(ch,&af);

if (form_num == 11)	/* bat */
    {
	af.modifier = 0;
	af.location = 0;
	af.bitvector = AFF_FLYING;
    	affect_to_char(ch,&af);
send_to_char("Your rise of the ground as you change into bat form.\n\r",ch);
    }
if (form_num == 12)	/* wolf */
    {
	af.modifier = 0;
	af.location = 0;
	af.bitvector = AFF_HASTE;
	affect_to_char(ch,&af);
send_to_char("You feel yourself moving faster in wolf form.\n\r",ch);
   }
if (form_num == 13)	/* bear */
    {
	af.modifier = ch->level;
	af.location = APPLY_HIT;
	affect_to_char(ch,&af);
send_to_char("You feel much tougher.\n\r",ch);
    }

if (form_num > 5)
	return;

    af.location = APPLY_STR;
    af.modifier = ch->level/10;
    affect_to_char(ch,&af);
    af.location = APPLY_DEX;
    af.modifier = -3;
    affect_to_char(ch,&af);
    af.location = APPLY_HITROLL;
    af.modifier = ch->level/10;
    affect_to_char(ch,&af);
    af.location = APPLY_DAMROLL;
    af.modifier = ch->level/4;
    affect_to_char(ch,&af);

    return;
}


/* The morph stuff replaces the PERS() macro */
char * get_descr_form(CHAR_DATA *ch,CHAR_DATA *looker, bool get_long)
{
    struct l_form
    {
        char *short_descr;
        char *long_descr;
    };
    char buf[MAX_STRING_LENGTH];
    char *buf2;

/*  { "short_desc", "long_descr" } */
    static const struct l_form look_form[] =
    {
    {" ", " "},
    {"a red dragon","A red dragon is here breathing plumes of smoke.\n\r"},
    {"a black dragon","A black dragon is here spitting burning acids.\n\r"},
    {"a blue dragon","A blue dragon gazes down at you here.\n\r"},
    {"a white dragon","A white dragon sends out blasts of freezing air at you.\n\r"},
    {"a green dragon","A green dragon is here breathing noxious poison.\n\r"},
    {"a goat headed", "A goat headed person stands here.\n\r"},
    {"a lion headed","A lion headed person stands here.\n\r"},
    {"a dragon headed", "A dragon headed person stands here.\n\r"},
    {"a cloaked figure", "A cloaked figure stands here.\n\r"},
    {"normal", "the three armed.\n\r"},
    {"normal","A huge bat is flying here.\n\r"},
    {"normal","A large grey wolf stands here.\n\r"},
    {"normal","A large, mean looking bear is standing here.\n\r"},
    {"a gaseous cloud","A cloud of boiling gasses is here.\n\r"},
    {"",""}
    };

    buf[0] = '\0';
    buf2 = "";
    if (get_long)
    {
        if (ch->morph_form[0] == 0)
            return ch->long_descr;
        else if (ch->morph_form[0] != MORPH_ARM)
            return look_form[ch->morph_form[0]].long_descr;
	else 
	{
	sprintf(buf,"%s %s",ch->name,look_form[MORPH_ARM].long_descr);
	return str_dup(buf);
	}
    }
    else
    {
        if (ch->morph_form[0] == 0)
            return PERS(ch, looker);
        else
	{
	if (!str_cmp(look_form[ch->morph_form[0]].short_descr,"normal") )
	    return PERS(ch, looker);
	else
	{
	if ((ch->morph_form[0] < 6) || (ch->morph_form[0] >= 9))
		return look_form[ch->morph_form[0]].short_descr;
	else if (ch->sex == SEX_MALE)
	sprintf(buf,"%s %s",look_form[ch->morph_form[0]].short_descr,"man");
	else if (ch->sex == SEX_FEMALE)
	sprintf(buf,"%s %s",look_form[ch->morph_form[0]].short_descr,"woman");
	else 
	sprintf(buf,"%s %s",look_form[ch->morph_form[0]].short_descr,"creature");

	buf2 = buf;
	return buf2;
	}
/*
            return look_form[ch->morph_form[0]].short_descr;
*/
	}
    }
    if (get_long)
        return ch->long_descr;

    return PERS(ch,looker);
}

/* Last returns are redundancy ones in case of some failure in checks */

/* Replacement for the handler.c get_char_room to handle morph dragon */
CHAR_DATA * get_char_room( CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number, count;

    number = number_argument(argument, arg);
    count = 0;
    if (!str_cmp(arg,"self") )
        return ch;

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (!can_see(ch,rch) )
            continue;
        if (rch->morph_form[0] != 0)
        {
	    if (rch->morph_form[0] < 6 && !str_cmp(arg,"dragon"))
	    {
		if (++count == number)
			return rch;
	    }
	    else if (rch->morph_form[0] == 6 && !str_cmp(arg,"goat"))
	    {
		if (++count == number)
		return rch;
	    }
	    else if (rch->morph_form[0] == 7 && !str_cmp(arg,"lion"))
	    {
		if (++count == number)
		return rch;
	    }
	    else if (rch->morph_form[0] == 8 && !str_cmp(arg,"dragon"))
	    {
		if (++count == number)
		return rch;
	    }
	    else if (rch->morph_form[0] == MORPH_BAT && !str_cmp(arg,"bat"))
	    {
		if (++count == number)
		return rch;
	    }
	    else if (rch->morph_form[0] == MORPH_WOLF && !str_cmp(arg,"wolf"))
	    {
		if (++count == number)
		return rch;
	    }
	    else if (rch->morph_form[0] == MORPH_BEAR && !str_cmp(arg,"bear"))
	    {
		if (++count == number)
		return rch;
	    }

/*
            if (!str_cmp(arg,"dragon"))
            {
                if (++count == number)
                    return rch;
            }
*/
        }
        if ( (arg[0] != '\0') && !is_name(arg,rch->name) )
            continue;
        if ( ++count == number )
            return rch;
    }
    return NULL;
}


void do_eye_of_the_predator(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;

    if ( (get_skill(ch,gsn_eye_of_the_predator) == 0)
    || (ch->level < skill_table[gsn_eye_of_the_predator].skill_level[ch->class]) )
    {
	send_to_char("Huh?\n\r",ch);
 	return;
    }

    if (clan_down(ch,CLAN_ANCIENT))
    	return;

    if(!is_affected(ch,gsn_cloak_form)) {
	send_to_char("You may only invoke that power while concealed.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_eye_of_the_predator) )
    {
	send_to_char("You already watch things through preadatory eyes.\n\r",ch);
	return;
    }
if (ch->mana < 40)
    {
	send_to_char("You do not have the mana.\n\r",ch);
	return;
    }

    if (number_percent() > ch->pcdata->learned[gsn_eye_of_the_predator] )
    {
	send_to_char("Your vision sharpens but then fades.\n\r",ch);
	check_improve(ch,gsn_eye_of_the_predator,FALSE,1);
	ch->mana -= 20;
	return;
    }

    af.where = TO_AFFECTS;
    af.type = gsn_eye_of_the_predator;
    af.level = ch->level;
    af.modifier = 0;
    af.location = 0;
    af.duration = ch->level/2;
    af.bitvector = AFF_DETECT_INVIS;
    send_to_char("You begin watching things through the eyes of a hunter.\n\r",ch);
    send_to_char("The shadows are revealed to you.\n\r",ch);
    affect_to_char(ch, &af);
    af.bitvector = AFF_DETECT_HIDDEN;
    affect_to_char( ch, &af );
/*
    if(ch->level>50&&!is_affected(ch,gsn_acute_vision)) {
    	af.type = gsn_acute_vision;
    	af.bitvector = 0;
    	affect_to_char(ch,&af);
	send_to_char("You begin to see through the twigs and brush.\n\r",ch);
    }
*/
    ch->mana -= 40;
	check_improve(ch,gsn_eye_of_the_predator,TRUE,1);

    return;
}



void do_wanted(CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *criminal;
        char arg[MAX_INPUT_LENGTH];
        char status[MAX_INPUT_LENGTH];
        char *next_arg;
        bool wanted;

        wanted = FALSE;
        next_arg = one_argument(argument,arg);
        one_argument(next_arg,status);
        if (IS_NPC(ch))
        return;
        if ( (get_skill(ch,gsn_wanted) == 0)
        || (ch->level < skill_table[gsn_wanted].skill_level[ch->class]) )
        {
        send_to_char("Huh?\n\r",ch);
        return;
        }
	if (clan_down(ch,CLAN_ENFORCER))
		return;


        if (!str_cmp(status,"yes"))
                wanted = TRUE;
        else if (!str_cmp(status,"no"))
        ;
        else
        {
        send_to_char("You must use WANTED PLAYER (YES/NO).\n\r",ch);
        return;
        }

        if (( criminal = get_char_world(ch,arg) ) == NULL )
        {
        send_to_char("That person isn't here.\n\r",ch);
        return;
        }
        if (IS_NPC(criminal))
        {
        send_to_char("You can only flag other players.\n\r",ch);
        return;
        }

        if (wanted)
        {
        if (IS_SET(criminal->act,PLR_CRIMINAL))
          {
          send_to_char("That person is already a criminal.\n\r",ch);
          return;
          }
        else
          {
          SET_BIT(criminal->act,PLR_CRIMINAL);
          send_to_char("You are now WANTED!!\n\r",criminal);
          act("$N is now a criminal.",ch,0,criminal,TO_CHAR);
	criminal->pcdata->wanteds += 1;
          return;
          }
        }

        if (!IS_SET(criminal->act,PLR_CRIMINAL) )
          {
          send_to_char("That person is not a criminal.\n\r",ch);
          return;
          }
        else
          {
          REMOVE_BIT(criminal->act,PLR_CRIMINAL);
          send_to_char("You are no longer wanted.\n\r",criminal);
          act("$N is no longer wanted.",ch,0,criminal,TO_CHAR);
	criminal->pcdata->wanteds -= 1;
          }
        return;
}



void do_balance(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *banker;
    char buf[100];

    for (banker = ch->in_room->people; banker != NULL; banker = banker->next_in_room)
    {
        if (IS_NPC(banker) && IS_SET(banker->act,ACT_BANKER))
		break;
    }
    if (banker == NULL)
    {
        send_to_char("You can't do that here.\n\r",ch);
        return;
    }

    sprintf(buf,"You have %ld gold coins and %ld silver in your account.\n\r",ch->gold_bank,ch->silver_bank);
    if (ch->gold_bank == 0 && ch->silver_bank == 0)
        sprintf(buf,"You have no account here!\n\r");

    send_to_char(buf, ch);
    return;
}

void do_withdraw(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *banker;
    char arg[MAX_STRING_LENGTH];
    int amount;
    char buf[MAX_STRING_LENGTH];
    int charges;

    for (banker = ch->in_room->people; banker != NULL; banker = banker->next_in_room)
    {
        if (IS_SET(banker->act,ACT_BANKER))
            break;
    }
    if (banker == NULL)
    {
        send_to_char("You can't do that here.\n\r",ch);
        return;
    }

    argument = one_argument(argument,arg);
    if (arg[0] == '\0')
    {
        send_to_char("Withdraw how much of which coin type?\n\r",ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Withdraw how much of which type of coin?\n\r",ch);
        return;
    }

    amount = atoi(arg);
    argument = one_argument(argument,arg);
    if (amount <= 0 || (str_cmp(arg, "gold") && str_cmp(arg,"silver")))
    {
      sprintf(buf,"%s Sorry, withdraw how much of which coin type?",ch->name);
      do_tell(banker,buf);
      return;
    }
    charges = 5 * amount;
    charges /= 100;

    if (!str_cmp(arg,"gold"))
    {
        if (ch->gold_bank < amount)
        {
            act("$N tells you, 'Sorry you do not have we don't give loans.'",ch,0,banker,TO_CHAR);
            return;
        }
        ch->gold_bank -= amount;
        ch->gold += amount;
	ch->gold -= charges;
    }
    else if (!str_cmp(arg,"silver"))
    {
        if (ch->silver_bank < amount)
        {
            act("$N tells you, 'You don't have that much silver in the bank.'",ch,0,banker,TO_CHAR);
            return;
        }
        ch->silver_bank -= amount;
        ch->silver += amount;
	ch->silver -= charges;
    }

    sprintf(buf,"You withdraw %d %s.\n\r",amount,arg);
    send_to_char(buf,ch);
    sprintf(buf,"You are charged a small fee of %d %s.\n\r",charges, !str_cmp(arg,"gold") ? "gold" : "silver");

    send_to_char(buf,ch);   
    return;
}

void do_deposit(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *banker;
    char arg[MAX_STRING_LENGTH];
    int amount;
    char buf[MAX_STRING_LENGTH];

    for (banker = ch->in_room->people; banker != NULL; banker = banker->next_in_room)
    {
        if (IS_NPC(banker) && IS_SET(banker->act,ACT_BANKER))
		break;
    }

    if (banker == NULL)
    {
        send_to_char("You can't do that here.\n\r",ch);
        return;
    }

     argument = one_argument(argument,arg);
    if (arg[0] == '\0')
    {
        send_to_char("Deposit how much of which coin type?\n\r",ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Deposit how much of which type of coin?\n\r",ch);
        return;
    }

    amount = atoi(arg);
    argument = one_argument(argument,arg);
    if (amount <= 0 || (str_cmp(arg, "gold") && str_cmp(arg,"silver")))
    {
      act("$N tells you, 'Sorry, deposit how much of which coin type?'",ch,0,banker,TO_CHAR);
      return;
    }

    if (!str_cmp(arg,"gold"))
    {
        if (ch->gold < amount)
        {
            act("$N tells you, 'You don't have that much gold on you!'",ch,0,banker,TO_CHAR);
            return;
        }
        ch->gold_bank += amount;
        ch->gold -= amount;
    }
    else if (!str_cmp(arg,"silver"))
    {
        if (ch->silver < amount)
        {
            act("$N tells you, 'You don't have that much silver on you!'",ch,0,banker,TO_CHAR);
            return;
        }
        ch->silver_bank += amount;
        ch->silver -= amount;
    }

    sprintf(buf,"You deposit %d %s.\n\r",amount,arg);
    send_to_char(buf,ch);
    sprintf(buf,"Your new balance is %ld gold, %ld silver.\n\r",ch->gold_bank,ch->silver_bank);
    send_to_char(buf,ch);
    return;
}

/*
void do_trace(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;


*/

void do_shapeshift(CHAR_DATA *ch,char *argument)
{
do_morph_dragon(ch,argument);
return;
}

/* Some shapeshifter code */
/* do a if (can_shapeshift(ch,form_num)) check.
 * If not then return and don't revert. Otherwise shapeshift_revert()
 * and then apply new morph num to the player in the do_morph_dragon fun.
 */

bool can_shapeshift(CHAR_DATA *ch,int form_num)
{
    /* form_num == shape to take on */

    bool able = TRUE;

    if (ch->morph_form[0] == MORPH_ARM
    && hands_full(ch))
    {
        send_to_char("You can't change form while using your shapeshifted hand to carry an item.\n\r",ch);
        return FALSE;
    }
    if (str_cmp(pc_race_table[ch->race].name,"changeling"))
    {
        send_to_char("You are not of the changeling race and cannot shapeshift.\n\r",ch);
        return FALSE;
    }
    if (form_num == -1)
    {
send_to_char("Which form was that?\n\r",ch);
return FALSE;
}
    if (ch->morph_form[0] == MORPH_CLOAK)
    {
send_to_char("You can't shapeshift while cloaked.\n\r",ch);
return FALSE;
}

    if ((form_num == MORPH_BAT && ch->level < 10)
    ||  (form_num == MORPH_WOLF && ch->level < 15)
    ||  (form_num == MORPH_BEAR && ch->level < 30) )
    {
        send_to_char("You are not yet high enough to take that form.\n\r",ch);
        return FALSE;
    }

    if (ch->mana < 50)
    {
send_to_char("You don't have the mana.\n\r",ch);
	return FALSE;
    }

    if (form_num == MORPH_BAT)
    {
        if (get_eq_char(ch,WEAR_ARMS) != NULL
        || get_eq_char(ch,WEAR_LEGS) != NULL
        || get_eq_char(ch,WEAR_HEAD) != NULL
        || get_eq_char(ch,WEAR_BODY) != NULL
        || get_eq_char(ch,WEAR_WAIST) != NULL)
        able = FALSE;
    }
    else if (form_num == MORPH_WOLF)
    {
        if (get_eq_char(ch,WEAR_ARMS) != NULL
        || get_eq_char(ch,WEAR_BODY) != NULL)
        able = FALSE;
    }
    else if (form_num == MORPH_BEAR)
    {
        if (get_eq_char(ch,WEAR_BODY) != NULL)
        able = FALSE;
    }
    else if (form_num == MORPH_ARM)
    {
        if (get_eq_char(ch,WEAR_BODY) != NULL)
        able = FALSE;
    }

    if (able)
        return TRUE;

    send_to_char("You are wearing items which disrupt the flow of shapeshifting to that shape.\n\r",ch);
    return FALSE;
}


void shapeshift_revert(CHAR_DATA *ch)
{
    if (ch->morph_form[0] == 0)
        return;

    if (ch->morph_form[0] == MORPH_ARM
	&& hands_full(ch))
	{
	send_to_char("You can't change form while your third arm is in use.\n\r",ch);
	return;
	}

    if (ch->morph_form[0] == MORPH_BAT)
    {
    send_to_char("Your wings fold away and you slowly reform yourself.\n\r",ch);
    act("$n's wings fold away and a new form takes it's place.",ch,0,0,TO_ROOM);
    }
    else if (ch->morph_form[0] == MORPH_WOLF)
    {
    send_to_char("Your wolven coat shrinks away and you slowly reform yourself.\n\r",ch);
    act("$n's fur shrinks away to nothing as it slowly stands up and takes on a new form.",ch,0,0,TO_ROOM);
    }
    else if (ch->morph_form[0] == MORPH_BEAR)
    {
    send_to_char("The thick fur about your body recedes as you assume a new form.\n\r",ch);
    act("$n's thick brown hair recedes as it fluidly changes into a new form.",ch,0,0,TO_ROOM);
    }
    else if(ch->morph_form[0] == MORPH_ARM)
    {
send_to_char("You fluidly retract your shapeshifted arm.\n\r",ch);
act("$n's shapeshifted arm fluidly flows back into $s body.",ch,0,0,TO_ROOM);
    }
    else if (ch->morph_form[0] == MORPH_CLOAK)
    {
send_to_char("You stop cloaking yourself.\n\r",ch);
affect_strip(ch,gsn_cloak_form);
ch->morph_form[0] = 0;
    }

    affect_strip(ch,gsn_shapeshift);
    return;
}
 
void do_records(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char buf1[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
 	int count = 0;

    if (ch->clan != CLAN_ENFORCER && !IS_IMMORTAL(ch))
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

 	buf1[0] = '\0';
	sprintf(buf,"Listing of current active players :\n\r");

	for (victim = char_list; victim != NULL; victim = victim->next)
	{
	if (IS_NPC(victim))
		continue;
	if (victim->pcdata->wanteds > 0)
	{
	count++;
	sprintf(buf1,"%s has %d flags.\n\r",victim->name,victim->pcdata->wanteds);
	strcat(buf,buf1);
	}
	}
	if (count == 0)
	strcat(buf,"None in the realms right now have had a flag before.\n\r");

	send_to_char(buf,ch);
	send_to_char("\n\r",ch);
	return;
}

void do_ratings(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
	return;
send_to_char("\n\r
PK kills (levels):    MOB kills (levels):    PK deaths:    MOB deaths:\n\r",ch);
sprintf(buf,"    %d (%d)                  %d (%d)                %d              %d",
ch->pcdata->kills[PK_KILLS], ch->pcdata->kills[PK_LEVELS],
ch->pcdata->kills[MOB_KILLS], ch->pcdata->kills[MOB_LEVELS], 
ch->pcdata->killed[PK_KILLED], ch->pcdata->killed[MOB_KILLED]);

send_to_char(buf,ch);
send_to_char("\n\r",ch);
return;
}
