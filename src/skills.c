/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
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
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"

/* command procedures needed */
DECLARE_DO_FUN(do_groups	);
DECLARE_DO_FUN(do_help		);
DECLARE_DO_FUN(do_say		);


/* used to get new skills */
void do_gain(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *trainer;
    if (IS_NPC(ch))
	return;

    /* find a trainer */
    for ( trainer = ch->in_room->people; 
	  trainer != NULL; 
	  trainer = trainer->next_in_room)
	if (IS_NPC(trainer) && (IS_SET(trainer->act,ACT_GAIN)
 || IS_SET(trainer->act,ACT_TRAIN)) )
	    break;

    if (trainer == NULL || !can_see(ch,trainer))
    {
	send_to_char("You can't do that here.\n\r",ch);
	return;
    }

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	do_say(trainer,"Use gain convert to convert 10 practices into 1 train.");
	do_say(trainer,"Or use gain revert to revert 1 train into 10 practices.");
	return;
    }

    if (!str_prefix(arg,"convert"))
    {
	if (ch->practice < 10)
	{
	    act("$N tells you 'You are not yet ready.'",
		ch,NULL,trainer,TO_CHAR);
	    return;
	}

	act("$N helps you apply your practice to training",
		ch,NULL,trainer,TO_CHAR);
	ch->practice -= 10;
	ch->train +=1 ;
	return;
    }

/* 'gain revert' converts one train into 10 pracs */
    if (!str_prefix(arg,"revert"))
    {
	if (ch->train < 1)
	{
	    act("$N tells you 'You do not have the training to apply to your skill practices yet.'",
		ch,NULL,trainer,TO_CHAR);
	    return;
	}

	act("$N helps you apply your training to skills practices.",
		ch,NULL,trainer,TO_CHAR);
	ch->practice += 10;
	ch->train -= 1 ;
	return;
    }

    act("$N tells you 'I do not understand...'",ch,NULL,trainer,TO_CHAR);
    return;
}
    



/* RT spells and skills show the players spells (or skills) */

void do_spells(CHAR_DATA *ch, char *argument)
{
    BUFFER *buffer;
    char arg[MAX_INPUT_LENGTH];
    char spell_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
    char spell_columns[LEVEL_HERO + 1];
    int sn, level, min_lev = 1, max_lev = LEVEL_HERO, mana;
    bool fAll = FALSE, found = FALSE;
    char buf[MAX_STRING_LENGTH];
 
    if (IS_NPC(ch))
      return;

    if (argument[0] != '\0')
    {
	fAll = TRUE;

	if (str_prefix(argument,"all"))
	{
	    argument = one_argument(argument,arg);
	    if (!is_number(arg))
	    {
		send_to_char("Arguments must be numerical or all.\n\r",ch);
		return;
	    }
	    max_lev = atoi(arg);

	    if (max_lev < 1 || max_lev > LEVEL_HERO)
	    {
		sprintf(buf,"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
		send_to_char(buf,ch);
		return;
	    }

	    if (argument[0] != '\0')
	    {
		argument = one_argument(argument,arg);
		if (!is_number(arg))
		{
		    send_to_char("Arguments must be numerical or all.\n\r",ch);
		    return;
		}
		min_lev = max_lev;
		max_lev = atoi(arg);

		if (max_lev < 1 || max_lev > LEVEL_HERO)
		{
		    sprintf(buf,
			"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
		    send_to_char(buf,ch);
		    return;
		}

		if (min_lev > max_lev)
		{
		    send_to_char("That would be silly.\n\r",ch);
		    return;
		}
	    }
	}
    }


    /* initialize data */
    for (level = 0; level < LEVEL_HERO + 1; level++)
    {
        spell_columns[level] = 0;
        spell_list[level][0] = '\0';
    }
 
    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL )
	    break;

	if ((level = skill_table[sn].skill_level[ch->class]) < LEVEL_HERO + 1
	&&  level >= min_lev && level <= max_lev
	&&  skill_table[sn].spell_fun != spell_null
	&&  ch->pcdata->learned[sn] > 0)
        {
	    found = TRUE;
	    level = skill_table[sn].skill_level[ch->class];
	    if (ch->level < level)
	    	sprintf(buf,"%-18s n/a      ", skill_table[sn].name);
	    else
	    {
        mana = UMAX(skill_table[sn].min_mana,
		    100/(2 + ch->level - level));
            sprintf(buf,"%-18s  %3d mana  ",skill_table[sn].name,mana);
	    }
 
	    if (spell_list[level][0] == '\0')
          	sprintf(spell_list[level],"\n\rLevel %2d: %s",level,buf);
	    else /* append */
	    {
          	if ( ++spell_columns[level] % 2 == 0)
		    strcat(spell_list[level],"\n\r          ");
          	strcat(spell_list[level],buf);
	    }
	}
    }
 
    /* return results */
 
    if (!found)
    {
      	send_to_char("No spells found.\n\r",ch);
      	return;
    }

    buffer = new_buf();
    for (level = 0; level < LEVEL_HERO + 1; level++)
      	if (spell_list[level][0] != '\0')
	    add_buf(buffer,spell_list[level]);
    add_buf(buffer,"\n\r");
    page_to_char(buf_string(buffer),ch);
    free_buf(buffer);
}

void do_skills(CHAR_DATA *ch, char *argument)
{
    BUFFER *buffer;
    char arg[MAX_INPUT_LENGTH];
    char skill_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
    char skill_columns[LEVEL_HERO + 1];
    int sn, level, min_lev = 1, max_lev = LEVEL_HERO;
    bool fAll = FALSE, found = FALSE;
    char buf[MAX_STRING_LENGTH];
 
    if (IS_NPC(ch))
      return;

    if (argument[0] != '\0')
    {
	fAll = TRUE;

	if (str_prefix(argument,"all"))
	{
	    argument = one_argument(argument,arg);
	    if (!is_number(arg))
	    {
		send_to_char("Arguments must be numerical or all.\n\r",ch);
		return;
	    }
	    max_lev = atoi(arg);

	    if (max_lev < 1 || max_lev > LEVEL_HERO)
	    {
		sprintf(buf,"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
		send_to_char(buf,ch);
		return;
	    }

	    if (argument[0] != '\0')
	    {
		argument = one_argument(argument,arg);
		if (!is_number(arg))
		{
		    send_to_char("Arguments must be numerical or all.\n\r",ch);
		    return;
		}
		min_lev = max_lev;
		max_lev = atoi(arg);

		if (max_lev < 1 || max_lev > LEVEL_HERO)
		{
		    sprintf(buf,
			"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
		    send_to_char(buf,ch);
		    return;
		}

		if (min_lev > max_lev)
		{
		    send_to_char("That would be silly.\n\r",ch);
		    return;
		}
	    }
	}
    }


    /* initialize data */
    for (level = 0; level < LEVEL_HERO + 1; level++)
    {
        skill_columns[level] = 0;
        skill_list[level][0] = '\0';
    }
 
    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL )
	    break;

        /*if ((level = skill_table[sn].skill_level[ch->class]) < LEVEL_HERO + 1
	&&  (fAll || level <= ch->level)
	&&  level >= min_lev && level <= max_lev
	&&  skill_table[sn].spell_fun == spell_null
        &&  ch->pcdata->learned[sn] > 0)*/
	if ((level = skill_table[sn].skill_level[ch->class]) < LEVEL_HERO + 1
	&&  level >= min_lev && level <= max_lev
	&&  skill_table[sn].spell_fun == spell_null
	&&  ch->pcdata->learned[sn] > 0)
        {
	    found = TRUE;
	    level = skill_table[sn].skill_level[ch->class];
	    if (ch->level < level)
	    	sprintf(buf,"%-18s n/a      ", skill_table[sn].name);
	    else
	    	sprintf(buf,"%-18s %3d%%      ",skill_table[sn].name,
		    ch->pcdata->learned[sn]);
 
	    if (skill_list[level][0] == '\0')
          	sprintf(skill_list[level],"\n\rLevel %2d: %s",level,buf);
	    else /* append */
	    {
          	if ( ++skill_columns[level] % 2 == 0)
		    strcat(skill_list[level],"\n\r          ");
          	strcat(skill_list[level],buf);
	    }
	}
    }
 
    /* return results */
 
    if (!found)
    {
      	send_to_char("No skills found.\n\r",ch);
      	return;
    }

    buffer = new_buf();
    for (level = 0; level < LEVEL_HERO + 1; level++)
      	if (skill_list[level][0] != '\0')
	    add_buf(buffer,skill_list[level]);
    add_buf(buffer,"\n\r");
    page_to_char(buf_string(buffer),ch);
    free_buf(buffer);
}

/* shows skills, groups and costs (only if not bought) */
void list_group_costs(CHAR_DATA *ch)
{
    return;
}


void list_group_chosen(CHAR_DATA *ch)
{
    if (IS_NPC(ch))
        return;
    return;
}

int exp_per_level(CHAR_DATA *ch)
{
    int epl;

    if (IS_NPC(ch))
        return 1500; 

    epl = pc_race_table[ch->race].xpadd + class_table[ch->class].xpadd + 1500;
    epl += (ch->level -1) * epl * .08;
    return epl;
}

/* this procedure handles the input parsing for the skill generator */
bool parse_gen_groups(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    int gn,sn,i;
 
    if (argument[0] == '\0')
	return FALSE;

    argument = one_argument(argument,arg);

    if (!str_prefix(arg,"help"))
    {
	if (argument[0] == '\0')
	{
	    do_help(ch,"group help");
	    return TRUE;
	}

        do_help(ch,argument);
	return TRUE;
    }

    if (!str_prefix(arg,"add"))
    {
	if (argument[0] == '\0')
	{
	    send_to_char("You must provide a skill name.\n\r",ch);
	    return TRUE;
	}

	gn = group_lookup(argument);
	if (gn != -1)
	{
	    if (ch->gen_data->group_chosen[gn]
	    ||  ch->pcdata->group_known[gn])
	    {
		send_to_char("You already know that group!\n\r",ch);
		return TRUE;
	    }

	    if (group_table[gn].rating[ch->class] < 1)
	    {
	  	send_to_char("That group is not available.\n\r",ch);
	 	return TRUE;
	    }

	    sprintf(buf,"%s group added\n\r",group_table[gn].name);
	    send_to_char(buf,ch);
	    ch->gen_data->group_chosen[gn] = TRUE;
	    ch->gen_data->points_chosen += group_table[gn].rating[ch->class];
	    gn_add(ch,gn);
	    return TRUE;
	}

	sn = skill_lookup(argument);
	if (sn != -1)
	{
	    if (ch->gen_data->skill_chosen[sn]
	    ||  ch->pcdata->learned[sn] > 0)
	    {
		send_to_char("You already know that skill!\n\r",ch);
		return TRUE;
	    }

	    if (skill_table[sn].rating[ch->class] < 1
	    ||  skill_table[sn].spell_fun != spell_null)
	    {
		send_to_char("That skill is not available.\n\r",ch);
		return TRUE;
	    }
	    sprintf(buf, "%s skill added\n\r",skill_table[sn].name);
	    send_to_char(buf,ch);
	    ch->gen_data->skill_chosen[sn] = TRUE;
	    ch->gen_data->points_chosen += skill_table[sn].rating[ch->class];
	    ch->pcdata->learned[sn] = 1;
	    return TRUE;
	}

	send_to_char("No skills or groups by that name...\n\r",ch);
	return TRUE;
    }

    if (!strcmp(arg,"drop"))
    {
	if (argument[0] == '\0')
  	{
	    send_to_char("You must provide a skill to drop.\n\r",ch);
	    return TRUE;
	}

	gn = group_lookup(argument);
	if (gn != -1 && ch->gen_data->group_chosen[gn])
	{
	    send_to_char("Group dropped.\n\r",ch);
	    ch->gen_data->group_chosen[gn] = FALSE;
	    ch->gen_data->points_chosen -= group_table[gn].rating[ch->class];
	    gn_remove(ch,gn);
	    for (i = 0; i < MAX_GROUP; i++)
	    {
		if (ch->gen_data->group_chosen[gn])
		    gn_add(ch,gn);
	    }
	    return TRUE;
	}

	sn = skill_lookup(argument);
	if (sn != -1 && ch->gen_data->skill_chosen[sn])
	{
	    send_to_char("Skill dropped.\n\r",ch);
	    ch->gen_data->skill_chosen[sn] = FALSE;
	    ch->gen_data->points_chosen -= skill_table[sn].rating[ch->class];
	    ch->pcdata->learned[sn] = 0;
	    return TRUE;
	}

	send_to_char("You haven't bought any such skill or group.\n\r",ch);
	return TRUE;
    }

    if (!str_prefix(arg,"premise"))
    {
	do_help(ch,"premise");
	return TRUE;
    }

    if (!str_prefix(arg,"list"))
    {
	list_group_costs(ch);
	return TRUE;
    }

    if (!str_prefix(arg,"learned"))
    {
	list_group_chosen(ch);
	return TRUE;
    }

    if (!str_prefix(arg,"info"))
    {
	do_groups(ch,argument);
	return TRUE;
    }

    return FALSE;
}
	    
	


        

/* shows all groups, or the sub-members of a group */
void do_groups(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
	return;
    return;
}

/* checks for skill improvement */
void check_improve( CHAR_DATA *ch, int sn, bool success, int multiplier )
{
    int chance;
    char buf[100];

    if (IS_NPC(ch))
	return;

    if (ch->level < skill_table[sn].skill_level[ch->class]
    ||  skill_table[sn].rating[ch->class] == 0
    ||  ch->pcdata->learned[sn] == 0
    ||  ch->pcdata->learned[sn] == 100)
	return;  /* skill is not known */ 

    /* check to see if the character has a chance to learn */
    chance = 10 * int_app[get_curr_stat(ch,STAT_INT)].learn;
    chance /= (		multiplier
		*	skill_table[sn].rating[ch->class] 
		*	4);
    chance += ch->level;

    if (number_range(1,1000) > chance)
	return;

    /* now that the character has a CHANCE to learn, see if they really have */	

    if (success)
    {
	chance = URANGE(5,100 - ch->pcdata->learned[sn], 95);
	if (number_percent() < chance)
	{
	    sprintf(buf,"You have become better at %s!\n\r",
		    skill_table[sn].name);
	    send_to_char(buf,ch);
	    ch->pcdata->learned[sn]++;
	    gain_exp(ch,2 * skill_table[sn].rating[ch->class]);
	}
    }

    else
    {
	chance = URANGE(5,ch->pcdata->learned[sn]/2,30);
	if (number_percent() < chance)
	{
	    sprintf(buf,
		"You learn from your mistakes, and your %s skill improves.\n\r",
		skill_table[sn].name);
	    send_to_char(buf,ch);
	    ch->pcdata->learned[sn] += number_range(1,3);
	    ch->pcdata->learned[sn] = UMIN(ch->pcdata->learned[sn],100);
	    gain_exp(ch,2 * skill_table[sn].rating[ch->class]);
	}
    }
}

/* returns a group index number given the name */
int group_lookup( const char *name )
{
    int gn;
 
    for ( gn = 0; gn < MAX_GROUP; gn++ )
    {
        if ( group_table[gn].name == NULL )
            break;
        if ( LOWER(name[0]) == LOWER(group_table[gn].name[0])
        &&   !str_prefix( name, group_table[gn].name ) )
            return gn;
    }
 
    return -1;
}

/* recursively adds a group given its number -- uses group_add */
void gn_add( CHAR_DATA *ch, int gn)
{
    int i;
    
    ch->pcdata->group_known[gn] = TRUE;
    for ( i = 0; i < MAX_IN_GROUP; i++)
    {
        if (group_table[gn].spells[i] == NULL)
            break;
        group_add(ch,group_table[gn].spells[i],FALSE);
    }
}

/* recusively removes a group given its number -- uses group_remove */
void gn_remove( CHAR_DATA *ch, int gn)
{
    int i;

    ch->pcdata->group_known[gn] = FALSE;

    for ( i = 0; i < MAX_IN_GROUP; i ++)
    {
	if (group_table[gn].spells[i] == NULL)
	    break;
	group_remove(ch,group_table[gn].spells[i]);
    }
}
	
/* use for processing a skill or group for addition  */
void group_add( CHAR_DATA *ch, const char *name, bool deduct)
{
    int sn,gn;

    if (IS_NPC(ch)) /* NPCs do not have skills */
	return;

    sn = skill_lookup(name);

    if (sn != -1)
    {
	if (ch->pcdata->learned[sn] == 0) /* i.e. not known */
	{
	    ch->pcdata->learned[sn] = 1;
	}
	return;
    }
	
    /* now check groups */

    gn = group_lookup(name);

    if (gn != -1)
    {
	if (ch->pcdata->group_known[gn] == FALSE)  
	{
	    ch->pcdata->group_known[gn] = TRUE;
	}
	gn_add(ch,gn); /* make sure all skills in the group are known */
    }
}

/* used for processing a skill or group for deletion -- no points back! */

void group_remove(CHAR_DATA *ch, const char *name)
{
    int sn, gn;
    
     sn = skill_lookup(name);

    if (sn != -1)
    {
	ch->pcdata->learned[sn] = 0;
	return;
    }
 
    /* now check groups */
 
    gn = group_lookup(name);
 
    if (gn != -1 && ch->pcdata->group_known[gn] == TRUE)
    {
	ch->pcdata->group_known[gn] = FALSE;
	gn_remove(ch,gn);  /* be sure to call gn_add on all remaining groups */
    }
}


/* Specializing stuff. This isn't implemented yet. If you want to use it
you'll have to fix/redo it yourself. I was in the process of coding this
new stuff in when I left the mud i was on and decided real life took
precedence over MUD life. Feel free to delete it or work on it as you
want. (Ceran) 
*/

int get_min_sp args((CHAR_DATA *ch,int spec));
bool specialize args((CHAR_DATA *ch,int spec));


void do_specialize(CHAR_DATA *ch,char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int spec = 0;
	
	if (IS_NPC(ch))
		return;

	if (ch->pcdata->special != SPEC_NONE)
	{
	send_to_char("You already have a specialization.\n\r",ch);
	return;
	}

	one_argument(argument,arg);
	if (arg[0] == '\0')
	{
	sprintf(buf,"You can currently specialize ");
	switch (ch->class)
	{
	case (CLASS_CLERIC):
	strcat(buf,"nothing.\n\r");	
	send_to_char(buf,ch);
	break;
	
	case (CLASS_WARRIOR):
	strcat(buf,"as a barbarian.\n\r");
	strcat(buf,"In doing so you will lose certain abilities related to a standard\n\r");
	strcat(buf,"warrior, but gain specialized skills of a barbarian. There is no\n\r");
	strcat(buf,"real net gain/advantage other than the ability to take the role\n\r");
	strcat(buf,"of a barbarian.\n\r");
	send_to_char(buf,ch);
	break;

	case (CLASS_THIEF):
	strcat(buf,"as a swashbuckler, bandit, herald or jester.\n\r");
	send_to_char(buf,ch);
	break;

	case (CLASS_PALADIN):
	strcat(buf,"as an Undead hunter, Zealot, or Avenger.\n\r");
	send_to_char(buf,ch);
	break;

	case (CLASS_ANTI_PALADIN):
	strcat(buf,"as a knight of Famine, Pestilence or Death.\n\r");
	send_to_char(buf,ch);
	break;

	case (CLASS_RANGER):
	strcat(buf,"nothing.\n\r");
	send_to_char(buf,ch);
	break;

	case (CLASS_MONK):
	strcat(buf,"nothing.\n\r");
	send_to_char(buf,ch);
	break;

	case (CLASS_CHANNELER):
	strcat(buf,"as a temporal mage.\n\r");
	send_to_char(buf,ch);
	break;

	case (CLASS_ASSASSIN):
	strcat(buf,"nothing.\n\r");
	send_to_char(buf,ch);
	break;

	case (CLASS_NECROMANCER):
	strcat(buf,"nothing.\n\r");
	send_to_char(buf,ch);
	break;

	case (CLASS_ELEMENTALIST):
	strcat(buf,"nothing.\n\r");
	send_to_char(buf,ch);
	break;
	}
	return;
	}

	capitalize(arg);
	switch (ch->class)
	{
	default:	spec = 0;	break;
	case (CLASS_WARRIOR):
		if  (!str_cmp(arg,"Barbarian"))
			spec = SPEC_BARBARIAN;
		break;

	case (CLASS_ANTI_PALADIN):
 		if (!str_cmp(arg,"Famine"))
			spec = SPEC_KNIGHT_FAMINE;
		else if (!str_cmp(arg,"Pestilence"))
			spec = SPEC_KNIGHT_PESTILENCE;
		else if (!str_cmp(arg,"Death"))
			spec = SPEC_KNIGHT_DEATH;
		break;

	case (CLASS_PALADIN):
		if (!str_cmp(arg,"Hunter") 
		|| !str_cmp(arg,"Undead")
		|| !str_cmp(arg,"Undead hunter"))
			spec = SPEC_PALADIN_HUNTER;
		else if (!str_cmp(arg,"Avenger"))
			spec = SPEC_PALADIN_AVENGER;
		break;
	}

	if (spec == 0)
	{
	do_specialize(ch,"");
	return;
	}

	if (ch->pcdata->sp < get_min_sp(ch,spec))
	{
	send_to_char("You have not currently acquired enough experience in that field of adventure.\n\r",ch);
	return;
	}

	return;

}

/* 
   Minimum Specialization Points needed to specialize. These are gained
   through doing certain things during play and a player won't know how
   much they have. 
	eg, Barbarians gain points on good bashes, crushes and such,
   paladins gain points for killing evil etc
	Just simple checks and points given in various parts of game.
	This isn't implemented (Ceran)
*/

int get_min_sp(CHAR_DATA *ch,int spec)
{
	int min;

	min = 1;
	if (spec == SPEC_BARBARIAN)
	{
	/* gain sp using bash, dirt, kick, berserk and crush */
	min = (1000 - 10*ch->perm_stat[STAT_STR] - 8*ch->perm_stat[STAT_CON]);
	}
	
return min;
}

/* Once again this specialisation code wasn't completed  but I left it in
so that anyone interested can take over, or gain some ideas for their own
mud...This is messy, messy, messy anyway.(Ceran) 

bool specialize(CHAR_DATA *ch,int spec)
{
	ch->pcdata->special = spec;

	switch(spec)
	{
	case(SPEC_BARBARIAN):
	ch->pcdata->learned[gsn_enlist] = 0;
	ch->pcdata->learned[gsn_ground_control] = 0;
	ch->pcdata->learned[gsn_rescue] = 0;
	ch->pcdata->learned[gsn_barbarian_skin] = 1;
	ch->pcdata->learned[gsn_barbarian_strike] = 1;
	ch->pcdata->learned[gsn_bone_tear] = 1;
send_to_char("You relinquish your more subtle fighting methods for those of a barbarian.\n\r");
	return TRUE;
	break;

	case (SPEC_KNIGHT_FAMINE):
	ch->pcdata->learned[gsn_power_word_stun] = 0;
	ch->pcdata->learned[gsn_charm] = 0;
	ch->pcdata->learned[gsn_armor] = 0;
	ch->pcdata->learned[gsn_starvation] = 1;
	ch->pcdata->learned[gsn_wither] = 1;
	ch->pcdata->learned[gsn_rot] = 1;
	send_to_char("Some message here...\n\r",ch);
	return TRUE;
	break;

	case(SPEC_KNIGHT_PESTILENCE):
	ch->pcdata->learned[gsn_power_word_stun] = 0;
	ch->pcdata->learned[gsn_armor] = 0;
	ch->pcdata->learned[gsn_charm] = 0;
	ch->pcdata->learned[gsn_epidemic] = 1;
	ch->pcdata->learned[gsn_creeping_death] = 1;
	SET_BIT(ch->immune,IMM_POISON);
	SET_BIT(ch->immune,IMM_DISEASE);
	return TRUE;
	break;

	case(SPEC_KNIGHT_DEATH):
	ch->pcdata->learned[gsn_charm] = 0;
	ch->pcdata->learned[gsn_decay] = 1;
	ch->pcdata->learned[gsn_animate_skeleton] = 1;
	ch->pcdata->learned[gsn_cleave] = 0;
	ch->pcdata->learned[gsn_black_death] = 1
	ch->pcdata->learned[gsn_armor] = 0;
	ch->pcdata->learned[gsn_demand] = 0;
	return TRUE;
	break;

	case(SPEC_PALADIN_HUNTER):
	ch->pcdata->learned[gsn_haggle] = 0;
	ch->pcdata->learned[gsn_heal] = 0;
	ch->pcdata->learned[gsn_lay_hands] = 0;
	ch->pcdata->learned[gsn_talk_to_dead] = 0;
	ch->pcdata->learned[gsn_spike] = 1;
	return TRUE;
	break;
	case(SPEC_PALADIN_AVENGER):
	ch->pcdata->learned[gsn_heal] = 0;
	ch->pcdata->learned[gsn_lay_hands] = 0;
	ch->pcdata->learned[gsn_righteous_fury] = 1;
	ch->pcdata->learned[gsn_vengeance] = 1;
	return TRUE;
	break;
	}

return FALSE;
}

*/
