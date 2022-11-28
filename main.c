#include "avatar.h"
#include "MCUFRIEND_kbv.h"
#include "stdio.h"
#include "stdlib.h"
#include "main.h"

int wait;
	
void initGame(struct Avatar *avatar, struct Obstacle *obstacle){
	avatar->posy = AVATAR_POSY_START;
	avatar->previous_posy = AVATAR_POSY_START;

	avatar->width = AVATAR_POSX;
	avatar->posx = AVATAR_WIDTH;
	avatar->length = AVATAR_LENGTH;
	avatar->color = WHITE;
	
	obstacle->width = 30;
	obstacle->counter = 24;
	
	LCD_Begin();
	HAL_Delay(20);
	LCD_SetRotation(1);
	LCD_FillScreen(COLOR_BACKGROUND);
	
	drawAvatar(avatar);
}

void drawAvatar(struct Avatar *avatar){
	int posy = avatar->posy;
	int previous_posy = avatar->previous_posy;
	
	int posx = avatar->posx;
	int width = avatar->width;
	int length = avatar->length;
	int color = avatar->color;
	int step = avatar->step;

	// DELETE PREVIOUS
	LCD_FillRect(posx, previous_posy, width+1, length, COLOR_BACKGROUND);
	
	// PRINT NEW
	LCD_DrawLine(posx+9,posy+11,posx+9,posy+19, color);
		
	LCD_DrawLine(posx+10,posy+3,posx+10,posy+4,color);	
	LCD_DrawLine(posx+10,posy+10,posx+10,posy+12,color);
	LCD_DrawLine(posx+10,posy+28,posx+10,posy+28,color);
		
	LCD_DrawLine(posx+11,posy+4,posx+11,posy+4,color);	
	LCD_DrawLine(posx+11,posy+10,posx+11,posy+28,color);
		
	LCD_DrawLine(posx+12,posy+3,posx+12,posy+28,color);
		
	LCD_DrawLine(posx+13,posy+2,posx+13,posy+21,color);
		
	LCD_DrawLine(posx+14,posy+1,posx+14,posy+20,color);
		
		
	LCD_DrawLine(posx+20,posy+11,posx+20,posy+19, color);
		
	LCD_DrawLine(posx+19,posy+3,posx+19,posy+4,color);	
	LCD_DrawLine(posx+19,posy+10,posx+19,posy+12,color);
	LCD_DrawLine(posx+19,posy+28,posx+19,posy+28,color);
		
	LCD_DrawLine(posx+18,posy+4,posx+18,posy+4,color);	
	LCD_DrawLine(posx+18,posy+10,posx+18,posy+28,color);
		
	LCD_DrawLine(posx+17,posy+3,posx+17,posy+28,color);
		
	LCD_DrawLine(posx+16,posy+2,posx+16,posy+21,color);
		
	LCD_DrawLine(posx+15,posy+1,posx+15,posy+20,color);	
}

void drawDino(struct Obstacle *obstacle){
	
	int posy = obstacle->posy;
	int id = obstacle->idTroupe;
	int posx = obstacle->posx;
	int color = obstacle->color;
	
	if (id == 1){
		drawDinos(posx, posy, color);
		obstacle->length = 30;
		obstacle->width = 30;
	}
	
	if (id == 2){
		drawDinos(posx, posy, color);
		drawDinos(posx+30, posy+30, color);
		obstacle->length = 60;
		obstacle->width = 60;
	}
	
	if (id == 3){
		drawDinos(posx, posy+60, color);
		drawDinos(posx+30, posy, color);
		drawDinos(posx+60, posy+30, color);
		obstacle->length = 90;
		obstacle->width = 90;
	}
	
	if (id == 4){
		drawDinos(posx, posy, color);
		drawDinos(posx+30, posy+30, color);
		drawDinos(posx, posy+60, color);
		obstacle->length = 90;
		obstacle->width = 60;
	}
	
	if (id == 5){
		drawDinos(posx, posy+60, color);
		drawDinos(posx+30, posy, color);
		drawDinos(posx+60, posy+30, color);
		drawDinos(posx+60, posy+60, color);
		obstacle->length = 90;
		obstacle->width = 90;
	}
	
	if (id == 6){
		drawDinos(posx, posy+30, color);
		drawDinos(posx+30, posy, color);
		drawDinos(posx+30, posy+60, color);
		drawDinos(posx+60, posy+30, color);
		obstacle->length = 90;
		obstacle->width = 90;
	}
	
	if (id == 7){
		drawDinos(posx, posy, color);
		drawDinos(posx+30, posy+30, color);
		drawDinos(posx+60, posy+30, color);
		obstacle->length = 60;
		obstacle->width = 90;
	}
	
	if (id == 8){
		drawDinos(posx, posy+60, color);
		drawDinos(posx+30, posy+60, color);
		drawDinos(posx+30, posy+30, color);
		drawDinos(posx+60, posy, color);
		obstacle->length = 90;
		obstacle->width = 90;
	}
	
}

void drawDinos(posx, posy, color){
		
	//Queue
	LCD_DrawLine(29+posx,posy+15,29+posx,posy+21,color);
	LCD_DrawLine(29+posx-1,posy+18,29+posx-1,posy+22,color);
	LCD_DrawLine(29+posx-2,posy+20,29+posx-2,posy+23,color);
	LCD_DrawLine(29+posx-3,posy+21,29+posx-3,posy+23,color);
	LCD_DrawLine(29+posx-4,posy+22,29+posx-4,posy+24,color);
	LCD_DrawLine(29+posx-5,posy+22,29+posx-5,posy+24,color);
	
	//Corps
	LCD_DrawLine(29+posx-6,posy+20,29+posx-6,posy+23,color);
	LCD_DrawLine(29+posx-7,posy+18,29+posx-7,posy+24,color);
	LCD_DrawLine(29+posx-8,posy+17,29+posx-8,posy+24,color);
	LCD_DrawLine(29+posx-9,posy+16,29+posx-9,posy+29,color);
	LCD_DrawLine(29+posx-10,posy+15,29+posx-10,posy+29,color);
	LCD_DrawLine(29+posx-11,posy+15,29+posx-11,posy+26,color);
	LCD_DrawLine(29+posx-12,posy+14,29+posx-12,posy+25,color);
	LCD_DrawLine(29+posx-13,posy+13,29+posx-13,posy+23,color);
	LCD_DrawLine(29+posx-14,posy+12,29+posx-14,posy+23,color);
	LCD_DrawLine(29+posx-15,posy+1,29+posx-15,posy+23,color);
	LCD_DrawLine(29+posx-16,posy+1,29+posx-16,posy+25,color);
	LCD_DrawLine(29+posx-17,posy+0,29+posx-17,posy+25,color);
	LCD_DrawLine(29+posx-18,posy+0,29+posx-18,posy+26,color);
	LCD_DrawLine(29+posx-19,posy+0,29+posx-19,posy+28,color);
	LCD_DrawLine(29+posx-20,posy+0,29+posx-20,posy+28,color);

	// Tête
	LCD_DrawLine(29+posx-19,posy+3,29+posx-19,posy+4,COLOR_BACKGROUND); //oeil
	LCD_DrawLine(29+posx-20,posy+3,29+posx-20,posy+4,COLOR_BACKGROUND); //oeil
	LCD_DrawLine(29+posx-21,posy+0,29+posx-21,posy+20,color);
	LCD_DrawLine(29+posx-22,posy+0,29+posx-22,posy+10,color);
	LCD_DrawLine(29+posx-23,posy+0,29+posx-23,posy+10,color);
	LCD_DrawLine(29+posx-24,posy+0,29+posx-24,posy+7,color);
	LCD_DrawLine(29+posx-25,posy+0,29+posx-25,posy+7,color);
	LCD_DrawLine(29+posx-26,posy+0,29+posx-26,posy+7,color);
	LCD_DrawLine(29+posx-27,posy+1,29+posx-27,posy+7,color);
	LCD_DrawLine(29+posx-28,posy+1,29+posx-28,posy+7,color);
	LCD_DrawLine(29+posx-29,posy+1,29+posx-29,posy+7,color);
	
	// Bouche
	LCD_DrawLine(29+posx-24,posy+9,29+posx-24,posy+10,color);
	LCD_DrawLine(29+posx-25,posy+9,29+posx-25,posy+10,color);
	LCD_DrawLine(29+posx-26,posy+9,29+posx-26,posy+10,color);
	LCD_DrawLine(29+posx-27,posy+9,29+posx-27,posy+10,color);
	
	//Pied
	LCD_DrawLine(29+posx-11,posy+28,29+posx-11,posy+29,color);
	LCD_DrawLine(29+posx-12,posy+28,29+posx-12,posy+29,color);
	LCD_DrawLine(29+posx-21,posy+27,29+posx-19,posy+28,color);
	LCD_DrawLine(29+posx-22,posy+27,29+posx-20,posy+28,color);
	
	//Bras
	LCD_DrawLine(29+posx-22,posy+15,29+posx-22,posy+16,color);
	LCD_DrawLine(29+posx-23,posy+15,29+posx-23,posy+17,color);
	LCD_DrawLine(29+posx-24,posy+15,29+posx-24,posy+17,color);
	
}


void moveAvatar(struct Avatar *avatar){	
	
	
	avatar->previous_posy = avatar->posy;
	avatar->posy = posdino;
	if(avatar->posy > 210)
			avatar->posy = 210;
	if(avatar->posy < 0)
			avatar->posy = 0;
	avatar->step++;
	drawAvatar(avatar);
	
}

void moveObstacle(struct Obstacle *obstacle){
		
		//On entre si l'obstacle a fini sa course <=> a dépassé l'écran
		if (obstacle->counter >  22 + obstacle->width/15){
			
			//On entre si quelque chose a été envoyé
			if (flag_go == 1){
				
				switch (color_switch){
					case 0:
						obstacle->color = WHITE;
						break;
					case 1:
						obstacle->color = CYAN;
						break;
					case 2:
						obstacle->color = GREEN;
						break;
					case 3:
						obstacle->color = YELLOW;
						break;
					case 4:
						obstacle->color = PINK;
						break;
			}
				flag_go = 0;
				obstacle->counter = 0; //on envoie donc on reset
				
				obstacle->posx = 330;
				obstacle->posy = posdinoRecu;
				
				
				//Puisque flag_go = 1, idRecu > 0, on envoie
				obstacle->idTroupe = idRecu;
				drawDino(obstacle);
				
				//On reset, sait-on jamais
				posdinoRecu = 0;

				idRecu = 0;
			}
		}

		obstacle->posx -= 15; //15 est la largeur minimale d'un obstacle
		LCD_FillRect(obstacle->posx+15,obstacle->posy,obstacle->width, obstacle->length,BLACK);
		drawDino(obstacle);
		
		obstacle->counter++;
}

int lookColision(struct Avatar *avatar, struct Obstacle *obstacle){
			if(obstacle->posx == 45 || obstacle->posx == 30 ||  obstacle->posx == 15){
			if (avatar->posy >= obstacle->posy && avatar->posy < obstacle->posy+obstacle->length) {
				LCD_FillScreen(BLACK);
				char buffer[20] = {0};
				sprintf(buffer, "GAME OVER");
				LCD_SetCursor(45, 50);
				LCD_SetTextSize(4);
				LCD_SetTextColor(RED, BLACK);
				LCD_Printf(buffer);
				return 1;
			}
			if (avatar->posy <= obstacle->posy && avatar->posy + avatar->length > obstacle->posy) {
				LCD_FillScreen(BLACK);
				char buffer[20] = {0};
				sprintf(buffer, "GAME OVER");
				LCD_SetCursor(45, 50);
				LCD_SetTextSize(4);
				LCD_SetTextColor(RED, BLACK);
				LCD_Printf(buffer);
				return 1;
			}
		}
		return 0;
}
