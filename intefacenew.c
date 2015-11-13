Menu* interface_init(){
	Menu *mainMenu = new_Menu("Main menu", NULL, 4);

	Menu *menu1 = new_Menu("Play", play, 0);
	Menu *menu2 = new_Menu("Show Highscores", NULL, 7);
	Menu *menu3 = new_Menu("Screen options", NULL, 1);
	Menu *menu4 = new_Menu("Other options", NULL, 1);

	Menu *menu31 = new_Menu("Party mode", oled_initiate_party_mode, 0);

	mainMenu->submenus[0] = menu1, mainMenu->submenus[1] = menu2, mainMenu->submenus[2] = menu3, mainMenu->submenus[3] = menu4;

	int n = 0;
	char* currentName;
	while(n<7){ //string funker ikke, må konveretere alt til char*
		currentName = highscore_retrieveN;
		menu2->submenus[n] = new_Menu(currentName + ": " + highscore_retrieveS, NULL, 0);
		n++;
	}




	

	menu2->prev = mainMenu;

	menu2->prev = mainMenu;
	menu31->submenus = NULL;
	
	




	return mainMenu;
}
