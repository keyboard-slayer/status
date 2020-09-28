/*
 * Copyright (C) 2020  Jordan DALCQ & contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>      
#include <stdlib.h>     
#include <stddef.h>     
#include <string.h>     
#include <time.h>

#define CHECKUPDATE "checkupdates+aur"

void
read_value(char *path, char *buffer)
{
    FILE *fp;
    size_t i = 0;

    fp = fopen(path, "r");
    while((buffer[i++] = fgetc(fp)) != EOF);
    buffer[i-2] = '\0'; 

    fclose(fp);
}


int
battery_level(void)
{
    int battery_level;

    char *level = (char *) malloc(sizeof(char) * 5);
    read_value("/sys/class/power_supply/BAT0/capacity", level);

    battery_level = atoi(level);
    
    free(level);
    return battery_level;
}


char
is_plugged(void)
{
    char plus;

    char *plugged = (char *) malloc(sizeof(char) * 2);
    read_value("/sys/class/power_supply/AC/online", plugged);

    if(strcmp(plugged, "1") == 0)
    {
        plus = '+';
    }

    else
    {
        plus = ' ';
    }

    free(plugged);
    return plus;
}


char *
datetime(void)
{
    time_t t;
    struct tm *tm;
    
    time(&t);
    tm = localtime(&t);

    return asctime(tm);
}


int 
update_nbr(void)
{
    FILE *fp;
    char line[1024];
    int updates;

    fp = popen(CHECKUPDATE, "r");
    updates = 0;

    if(fp == NULL)
    {
        fprintf(stderr, "Couldn't run the command %s\n", CHECKUPDATE);
        exit(1);
    }

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        updates++;
    }

    fclose(fp);

    return updates;
}


int 
main(void)
{
    int level;
    char plugged;
    char *time;
    int updates;

    level = battery_level();
    plugged = is_plugged();
    updates = update_nbr();

    time = datetime();
    time[strlen(time) - 3] = '\0';

    printf("Updates: %d | %s | %d%%%c", updates, time, level, plugged);

    return 0;
}
