#ifndef AVATAR_H
#define AVATAR_H

#define AVATAR_POSY_START 90
#define AVATAR_POSX 30
#define AVATAR_WIDTH 30
#define AVATAR_LENGTH 30
#include "arm_math.h"
#define COLOR_BACKGROUND 0x0000 // BLACK


extern volatile float flag_no_detection;
extern float posdino;

extern uint8_t posdinoRecu;
extern uint8_t idRecu;
extern volatile float flag_go;

extern volatile int color_switch;



struct Avatar {
	int posy;
	int previous_posy;
	
	int posx;
	int length;
	int width;
	int color;
	
	int step;
};

struct Obstacle {
	int posy;
	int posx;
	int length;
	int width;
	int color;
	int idTroupe;
	int counter;
};

void initGame(struct Avatar *avatar, struct Obstacle *obstacle);
void drawAvatar(struct Avatar *avatar);
void drawDino(struct Obstacle *obstacle);
void drawDinos(int posx, int posy, int color);

void moveAvatar(struct Avatar *avatar);
void moveObstacle(struct Obstacle *obstacle);
int lookColision(struct Avatar *avatar, struct Obstacle *obstacle);


#endif //AVATAR_H
