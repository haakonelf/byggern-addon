


#include "eprom.h"




void EEPROM_write(unsigned int uiAddress, unsigned char ucData){
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address and data registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}



unsigned char EEPROM_read(unsigned int uiAddress){
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */
	return EEDR;
	}

//de første 7 bytesene til highscore til de henholdsvis 7 beste, deretter 7 * 4 bytes til de henholdsvis 7 beste sine navn


uint_8 highscore_retrieveS(uint_8 rank){
	return EEPROM_read(rank);
}

unsigned char* highscore_retrieveN(uint_8 rank){
	uint_8 nameAddress=numHighscores + 4*rank;
	unsigned char* name[4];

	for (int i = 0; i<3; i++){
		name[i]= EEPROM_read(nameAddress+i);
	}
	return name; //name[]?
}

void highscore_resetAll(void){
	bool usingNames = true;
	uint_8 endingDataByte = numHighscores + usingNames(4*numHighscores);

	for (int i=0;i<endingDataByte;i++){
		EEPROM_write(i, NULL);
	}
}

uint_8 highscore_isSufficient(uint_8 score){ //returnerer hvor scoren skulle ha vært stappet inn

	for (int i = 0; i<numHighscores; i++){
		if(EEPROM_read(i)<score){
			break;
		}
	}
	if(i==numHighscores-1){
		return max8bit; //feil, aka not sufficient
	}
	return i;
}


void highscore_set(uint_8 score, char name){ 
	uint_8 rank = highscore_isSufficient(score);
	bool usingNames = true;
	if(rank==max8bit){ //just in case
		return;
	}
	if (!usingNames){
		name = "BOB";
	}

	uint_8 oldScoreAtRank;
	unsigned char* oldNameAtRank;
	//foreløpig vil rank herpe seg selv med mindre jeg ptuter alt i samme while løkke


	while(numHighscores-rank){
//scores:
		oldScoreAtRank = EEPROM_read(rank);
		EEPROM_write(rank, score);
		score = oldScoreAtRank;
//names:
		if(usingNames){
			oldNameAtRank=numHighscores+4*rank;
			EEPROM_write(numHighscores+4*rank, name);
			name = oldNameAtRank;
		}
		rank++;
	}
}


void highscore_keyboardPrint(){
	char* alphabet = [abcdefghijklmnopqrstuvwxyz];
	oled_clear_screen();
	oled_set_start_col(0);
	printf("New Highscore!\n");
	for(int j=0;j<4;j++){
		for(int i = 0; i < 7; i++){
			if(j*7+i>=26){
				break;
			}
			oled_write_string("" + alphabet[j*7+i], 1+j);
		}
	}
	oled_set_start_col(7);
	//oled_write_string("      ____", 7)
	for (int i = 0;i<(16-4)/2;i++){
		oled_write(" ")
	}
	oled_write("____")
}


//må intitalisere disse før man går i menyen
void highscore_keyboardNav(Joystick joy, int *h_selectPosX, int *h_selectPosY){
	direction previous_direction = NEUTRAL;

	while(JOY_get_direction(joy) != NEUTRAL){
		previous_direction = JOY_get_direction(joy);
		JOY_read_joystick(&joy);
	} //Wait for joy to reach neutral position.
	switch(previous_direction){
		case UP:
			if(*h_selectPosY != 1){
				printf("case UP\n");
				(*h_selectPosY)--;
				//highscore_flashSelected(*h_selectPosX, *h_selectPosY);
			}
			break;
			
		case DOWN:
			if(h_selectPosY != 5){
				(*h_selectPosY)++;
				//highscore_flashSelected(*h_selectPosX, *h_selectPosY);
			}
			break;
			
		case RIGHT:
			if(h_selectPosX!=7){
				(*h_selectPosY)++;
				//highscore_flashSelected(*h_selectPosX, *h_selectPosY);
			break;
			
		case LEFT:
			if(h_selectPosX!=0){
				(*h_selectPosY)++;
				//highscore_flashSelected(*h_selectPosX, *h_selectPosY);
			}
			break;
		case NEUTRAL:
			break;
		
	}	
}



char highscore_xyToLetter(int *h_selectPosX, int *h_selectPosY){
	char* alphabet = [abcdefghijklmnopqrstuvwxyz];
	return alphabet[7*h_selectPosY+h_selectPosX];
}

void highscore_selectionFlicker(uint_8 speed, uint8_t x, uint8_t y, uint8_t nameS, bool visibility, uint8_t playerRank){
	visibility ^= 1;
	if visibility{
		oled_set_start_col(y);
		oled_write_string(highscore_xyToLetter, x*2) //husk på mellomrommene imellom bokstavene
		oled_set_start_col(7);
		oled_write_string("_", 6+nameS); 
		return;
	}
	oled_set_start_col(y);
	oled_write_string(" ", x*2) //husk på mellomrommene imellom bokstavene
	oled_set_start_col(7);
	oled_write_string("_", 6+nameS); 
	return;

	}
}

char highscore_toggle(uint8_t *h_selectPosX, uint8_t *h_selectPosY, *h_selectName){
	oled_set_start_col(7);
	char nameChar = highscore_xyToLetter(h_selectPosX, h_selectPosY)
	oled_write_string(nameChar, 6+h_selectName);
	 
	h_selectName++;
	return nameChar;
}


void highscore_newScore(uint8_t score){

	uint8_t rank = highscore_isSufficient(score);
	oled_clear_screen();
	if rank==max8bit{
		return;
	}
	highscore_keyboardPrint()
	uint8_t nameS=0;
	uint8_t posY=1;
	uint8_t posX=0; //MYE TULL MED X VERDIENE FIX PLS
	uint8_t speed = 100;
	uint8_t teller=0;


	while(nameS!=3){
		teller++;
		char* name;
		highscore_keyboardNav(joy, posX, posY);
		if !(teller%speed){
			highscore_selectionFlicker(speed, posX, posY, nameS, rank);
			teller=0;
		}
		if(JOY_read_toggle_button(){
			name[nameS] = highscore_toggle(&posX, &posY, &nameS);
		}
		highscore_set(score, name);
	}
	oled_clear_screen();


}
