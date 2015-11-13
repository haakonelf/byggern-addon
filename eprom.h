


#define max8bit 2^8;
#define numHighscores 7;



void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);
uint_8 highscore_retrieveS(uint_8 rank);
unsigned char* highscore_retrieveN(uint_8 rank);
void highscore_resetAll(void);
uint_8 highscore_isSufficient(uint_8 score);
void highscore_set(uint_8 score, char name);
void highscore_keyboardPrint();
void highscore_keyboardNav(Joystick joy, int *h_selectPosX, int *h_selectPosY);
char highscore_xyToLetter(int *h_selectPosX, int *h_selectPosY);
void highscore_selectionFlicker(uint_8 speed, uint8_t x, uint8_t y, uint8_t nameS, bool visibility, uint8_t playerRank);
char highscore_toggle(uint8_t *h_selectPosX, uint8_t *h_selectPosY, *h_selectName);
void highscore_newScore(uint8_t score);



