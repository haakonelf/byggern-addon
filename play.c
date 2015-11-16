
int gamescore=0;
int game_lost = 0;
int game_score_counter = 0;

void play(teller){
	

	oled_clear_screen();
	oled_set_start_col(0);
	oled_write_string("Bounce the ball\n", 0);
	oled_write_string("to begin.", 0);



	while(1){

		if(can_pollInterrupt()){
			msg = can_read();
		}
		
		//Kanskje bruke en switch
		can_handle_joystick_message(msg);
		can_handle_score_message(msg);
		can_handle_slider_message(msg); //FJERN GLOBAL VARIABEL

		
		_delay_ms(10);
		if (JOY_READ_RIGHT_BUTTON()){
			break;
		}
	}
	timer.enable();
	play_writeIngameScreen();
	while(!IR_signal_function()){
		oled_set_start_col(3);
		oled_write_string(gamescore, 11);

	game_lost = 1 //game lost og alt relatert unødvendig???
	timer.disable();
	int result = highscore_newscore(gamescore);
	if(!result){
		play_endgame(gamescore);
	}
	return 1;
}

void play_endgame(void){
	oled_clear_screen();
	oled_set_start_col(0);
	oled_write_string("Not good enough.\n",0);
	oled_write_string("You got:\n", 0);
	oled_write_string(gamescore,0);
	oled_write_string(" points.", 10);	
}

void play_writeIngameScreen(void){
	oled_clear_screen();
	oled_set_start_col(0);
	oled_write_string("SUPERGAEM lol\n", 0);
	oled_set_start_col(3);
	oled_write_string("Points: ", 0);

}

void game_keep_score(){
	if (!game_lost)  { game_score_counter++; }

	if (game_score_counter > 7) {
		gamescore++;
		game_score_counter = 0;
	}
}




