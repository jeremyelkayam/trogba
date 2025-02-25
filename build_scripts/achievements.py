"""
Copyright (c) 2025 Jeremy Elkayam

based on python-script.py example from Butano game engine
"""

import os
from pathlib import Path
import csv
#from dataclasses import dataclass
#import xml.etree.ElementTree as ET
#    ;
    
def cpp():
    result = ""
    achievements_count = 0
    with open('data/achievements.csv', newline='') as csvfile:

        acreader = csv.DictReader(csvfile)

        for row in acreader:

            tag = row['tag']

            if(tag != ''):
                threshold = row['num']
                if(row['is_number'] == 'FALSE'):
                    threshold = '0b' + '1'*int(threshold)

                result += ('\n\t{\n\t\t"' + tag
                    + '",\n\t\t"' + row['name']
                    + '",\n\t\t"' + row['dscr']
                    + '",\n\t\t' + (row['is_number']).lower()
                    + ',\n\t\t' + threshold + 
                    '\n\t},')
                achievements_count += 1
    result = result[:-1] + "\n};"

    result = ("constexpr const achievement_rom_data acdata[" 
        + str(achievements_count) + "] = \n{") + result
    result = '''
struct achievement_rom_data
{
    const char *tag;
    const char *name;
    const char *desc;
    bool is_number;
    long threshold;
};
''' + result

    result += ('\nconstexpr const int num_achievements = ' + 
        str(achievements_count) + ";")
    return result

def need_to_remake_achievement_data_files():
    if(os.path.exists('trogba.gba')):
        for subdir, dirs, files in os.walk('data/'):
            for file in files:
                #Check if any TMX files were updated
                # since we last compiled
                if(os.path.splitext(file)[-1].lower() == '.csv' and 
                    os.path.getmtime(subdir + '/' + file) > 
                    os.path.getmtime('trogba.gba')):
                        return True
        if(os.path.getmtime('build_scripts/achievements.py') > 
            os.path.getmtime('trogba.gba')):
            #Also check if this file was updated since last build.
            return True

        return False
    #if no TROGBA file exists, return TRUE
    return True
