/*
 * gui_home.cpp
 *
 *  Created on: 11.05.2017
 *      Author: tilmann@bubecks.de
 */

#include <gui/settings/gui_filemanager.h>
#include "gui_home.h"

#include "common.h"
#include "../gui_list.h"
#include "../gui_dialog.h"
#include "../../fc/conf.h"
#include "../../fc/logger/logger.h"

void gui_home_init()
{
	gui_list_set(gui_home_item, gui_home_action, 2, GUI_SETTINGS);
}

void gui_home_stop() {}

void gui_home_loop()
{
	gui_list_draw();
}

void gui_home_irqh(uint8_t type, uint8_t * buff)
{
	gui_list_irqh(type, buff);
}

void gui_home_action(uint8_t index)
{
	switch(index)
	{
		case(0):
			config.home.flags ^= HOME_SET_AS_TAKEOFF;
			if (config.home.flags & HOME_SET_AS_TAKEOFF)
			{
				fc.flight.home_valid = false;

				config.home.flags = HOME_SET_AS_TAKEOFF;
				config.home.name[0] = 0;
				config.home.freq[0] = 0;
				config.home.rwy[0] = 0;
				config.home.traffic_pattern[0] = 0;
				config.home.info[0] = 0;
			}
			eeprom_busy_wait();
			eeprom_update_block((void *)&config.home, &config_ee.home, sizeof(config.home));

		break;

		case(1):
			if (!(config.home.flags & HOME_SET_AS_TAKEOFF))
			{
				gui_filemanager_set_dir((char *)"/HOMES");
				gui_filemanager_level = 0;
				gui_filemanager_set_tasks(GUI_HOMEDETAIL, GUI_HOME);
				gui_switch_task(GUI_FILEMANAGER);
			}
		break;
	}
}

void gui_home_item(uint8_t index, char * text, uint8_t * flags, char * sub_text)
{
	switch (index)
	{
		case 0:
			strcpy_P(text, PSTR("Set as Takeoff"));
			if (config.home.flags & HOME_SET_AS_TAKEOFF)
				*flags |= GUI_LIST_CHECK_ON;
			else
				*flags |= GUI_LIST_CHECK_OFF;
		break;

		case 1:
			strcpy_P(text, PSTR("From SD"));

			if (config.home.flags & HOME_SET_AS_TAKEOFF)
			{
				*flags |= GUI_LIST_DISABLED;
			}
			else
			{
				*flags |= GUI_LIST_SUB_TEXT;

				if (config.home.flags & HOME_LOADED_FROM_SD)
					strcpy(sub_text, (const char *)config.home.name);
				else
					strcpy_P(sub_text, PSTR("<Load>"));
			}
		break;
	}

}

