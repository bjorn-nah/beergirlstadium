#include "sprite.h"

#include "sound/psglib.c"
#include "snginit.c"

#define DIR_LEFT 1
#define DIR_RIGHT 2
#define DIR_UP 3
#define DIR_DOWN 4
#define DIR_LEFT_UP 5
#define DIR_RIGHT_UP 6
#define DIR_LEFT_DOWN 7
#define DIR_RIGHT_DOWN 8

#define MAP_WIDTH 16
#define MAP_HEIGHT 15

#incspr(sprite,"sprite.pcx",0,0,8,6);
#incpal(spritepal,"sprite.pcx");

#incspr(fleche,"fleche.pcx",0,0,2,3);
#incpal(flechepal,"fleche.pcx");

#incspr(ennemi,"ennemi.pcx",0,0,8,4);
#incpal(ennemipal,"ennemi.pcx");

#incbin(levelmap,"bgs.FMP");
#inctile_ex(leveltiles,"background.pcx",0,0,8,4,0);
#incpal(levelpal,"background.pcx");

int tics_sprite, frame_sprite, state_sprite, curent_sprite, counter;
int j1, sprite_x, sprite_y, sprite_nx, sprite_ny, i, money, dead;
int ennemi_x[16], ennemi_y[16], ennemi_state[16], ennemi_dir[16], ennemi_wait[16], ennemi_beer[16], ennemi_type[16];
char dir_sprite;
int VSyncCnt, TimerCnt, MainCnt, SubCnt;

bgs()
{
	
	/* init squirel */
	int  song;
	int  jsv;

	int  psgMainStatus;
	int  psgSubStatus;

	init_satb();
	
	psgInit(5);
	psgOn(0); 
	psgPlay(0);	
	
	sngInit();
	psgDelay( 0 );
	
	/* fin init squirel */
	
	counter = 0;
	tics_sprite = 0;
	frame_sprite = 0;
	state_sprite = 0;
	money = 0;
	dead=0;
		
	sprite_x = 56;
	sprite_y = 48;
	dir_sprite = DIR_RIGHT;
	
	sprite_nx = sprite_x;
	sprite_ny = sprite_y;
	
	for(i=0;i<16;i++)
	{
		ennemi_state[i] = 0;
	}
	
	set_font_color(9, 1);
	load_default_font();
	
	set_map_data(levelmap,16,14);
	set_tile_data(leveltiles);
	load_tile(0x1000);
	load_map(0,0,0,0,16,14);
	load_palette(0,levelpal,1);
	
	put_string("Money :", 1, 27); 
	put_number(money, 5, 8, 27); 
	
	spr_make(0,56,48,0x5000,FLIP_X_MASK|SIZE_MAS,FLIP_X|SZ_32x16,0,1);
	spr_make(1,56,64,0x5200,FLIP_X_MASK|SIZE_MAS,FLIP_X|SZ_32x16,0,0);
	set_sprpal(0,spritepal,1);
	load_vram(0x5000,sprite,0x0b00);
	
	for(;;)
	{
		vsync();
		
		/*squirel stuff*/
		psgMainStatus = psgMStat();
		psgSubStatus  = psgSStat();
		
		/*srand(counter);*/		/* re-randomise le random! */
		j1 = joy(0);
		
		if(counter%60==0 && random(6)==1) create_ennemi();
		
		player_machine();
		ennemi_machine();
		physique();
		
		counter++;
	
		satb_update();
	}
}

player_machine()
{
/*
	0 - Repos
	1 - Marche
	2 - sert des bières
	3 - Meurt comme Marion Cotillard
	4 - Manges des mergez-frites
*/

	/* spr_set(0); */
	curent_sprite = 0;
	if ( state_sprite == 0) player_sprite_0();
	if ( state_sprite == 1) player_sprite_1();
	if ( state_sprite == 2) player_sprite_2();
	if ( state_sprite == 3) player_sprite_3();
	if ( state_sprite == 4) player_sprite_4();
	
}

/* joueur */
player_sprite_0()
{
	/*
	if ( tics_sprite == 0 ) 
	{
		spr_set(0);
		spr_pattern(0x5000);
		spr_set(1);
		spr_pattern(0x5200);
	}
	*/
	tics_sprite = 1;
	if (j1 & JOY_LEFT)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 1;
		spr_set(0);
		spr_pattern(0x5400);
		spr_set(1);
		spr_pattern(0x5600);
		dir_sprite = DIR_LEFT;
	}
	if (j1 & JOY_RGHT)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 1;
		spr_set(0);
		spr_pattern(0x5400);
		spr_set(1);
		spr_pattern(0x5600);
		dir_sprite = DIR_RIGHT;
	}
	if (j1 & JOY_UP)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 1;
		spr_set(0);
		spr_pattern(0x5400);
		spr_set(1);
		spr_pattern(0x5600);
		dir_sprite = DIR_UP;
	}
	if (j1 & JOY_DOWN)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 1;
		spr_set(0);
		spr_pattern(0x5400);
		spr_set(1);
		spr_pattern(0x5600);
		dir_sprite = DIR_DOWN;
	}
	if (j1 & JOY_A)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 2;
	}
}

player_sprite_1()
{
	int walk_sprite;
	walk_sprite = 0;
	tics_sprite++;
	if (tics_sprite > 6)
	{
		tics_sprite = 0;
		frame_sprite++;
		if (frame_sprite > 3) frame_sprite = 0;
		if (frame_sprite == 0) 
		{
			spr_set(0);
			spr_pattern(0x5000);
			spr_set(1);
			spr_pattern(0x5200);
		}
		
		if (frame_sprite == 1) 
		{
			spr_set(0);
			spr_pattern(0x5480);
			spr_set(1);
			spr_pattern(0x5680);
		}
		
		if (frame_sprite == 2) 
		{
			spr_set(0);
			spr_pattern(0x5000);
			spr_set(1);
			spr_pattern(0x5200);
		}
		if (frame_sprite == 3) 
		{
			spr_set(0);
			spr_pattern(0x5400);
			spr_set(1);
			spr_pattern(0x5600);
		}
		
	}
	if (j1 & JOY_LEFT && dir_sprite == DIR_LEFT)
	{
		walk_sprite = 1;
	}
	if (j1 & JOY_RGHT && dir_sprite == DIR_RIGHT)
	{
		walk_sprite = 1;
	}
	if (j1 & JOY_UP && dir_sprite == DIR_UP)
	{
		walk_sprite = 1;
	}
	if (j1 & JOY_DOWN && dir_sprite == DIR_DOWN)
	{
		walk_sprite = 1;
	}

	if (walk_sprite == 0)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 0;
	}
	
	else
	{
		if(dir_sprite == DIR_LEFT)	sprite_nx--;
		if(dir_sprite == DIR_RIGHT)	sprite_nx++;
		if(dir_sprite == DIR_UP)	sprite_ny--;
		if(dir_sprite == DIR_DOWN)	sprite_ny++;
	}
	if (j1 & JOY_A)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 2;
	}
		
}

player_sprite_2()
{
	for(i=0;i<16;i++)
	{
		if(ennemi_state[i]==2 && 
		ennemi_x[i] > sprite_x-3 && ennemi_x[i] < sprite_x+29 && 
		ennemi_y[i] > sprite_y-17 && ennemi_y[i] < sprite_y+17)
		{
			ennemi_state[i]=3;
			spr_set(0);
			spr_pattern(0x5500);
			spr_set(1);
			spr_pattern(0x5700);
		}
			
	}
	if (j1 & JOY_LEFT)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 1;
		spr_set(0);
		spr_pattern(0x5400);
		spr_set(1);
		spr_pattern(0x5600);
		dir_sprite = DIR_LEFT;
	}
	if (j1 & JOY_RGHT)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 1;
		spr_set(0);
		spr_pattern(0x5400);
		spr_set(1);
		spr_pattern(0x5600);
		dir_sprite = DIR_RIGHT;
	}
	if (j1 & JOY_UP)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 1;
		spr_set(0);
		spr_pattern(0x5400);
		spr_set(1);
		spr_pattern(0x5600);
		dir_sprite = DIR_UP;
	}
	if (j1 & JOY_DOWN)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 1;
		spr_set(0);
		spr_pattern(0x5400);
		spr_set(1);
		spr_pattern(0x5600);
		dir_sprite = DIR_DOWN;
	}
	if (j1 & JOY_A)
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 2;
	}
	else
	{
		tics_sprite = 0;
		frame_sprite = 0;
		state_sprite = 0;
	}
}

player_sprite_3()
{
	spr_set(0);
	spr_pattern(0x5800);
	spr_set(1);
	spr_pattern(0x5a00);
	dead++;
}

player_sprite_4()
{
}

create_ennemi()
{
	int bool;
	i = 0;
	bool = 0;
	for(i=0;i<15;i++)
	{
		if (ennemi_state[i] == 0 && bool ==0)
		{
			ennemi_state[i] = 1;
			bool =1;
		}
	}
}


ennemi_machine()
{
	int current_ennemi;
	/*
	0 - regarde son match et c'est tout
	1 - init de la booze
	2 - demande de la booze
	3 - se fait servir
	4 - se leve pour casser la gueule au joueur
	5 - cours partout comme un con
	*/

	/*spr_set(0);*/
	for(current_ennemi =0;current_ennemi<16;current_ennemi++)
	{
		if ( ennemi_state[current_ennemi] == 0) ennemi_sprite_0(current_ennemi);
		if ( ennemi_state[current_ennemi] == 1) ennemi_sprite_1(current_ennemi);
		if ( ennemi_state[current_ennemi] == 2) ennemi_sprite_2(current_ennemi);
		if ( ennemi_state[current_ennemi] == 3) ennemi_sprite_3(current_ennemi);
		if ( ennemi_state[current_ennemi] == 4) ennemi_sprite_4(current_ennemi);
		if ( ennemi_state[current_ennemi] == 5) ennemi_sprite_5(current_ennemi);
	}
}


ennemi_sprite_0(current_ennemi)
int current_ennemi;
{
}

ennemi_sprite_1(current_ennemi)
int current_ennemi;
{
	int tule_nb;
	tule_nb = 0;
	while(!(tule_nb > 0 && tule_nb < 8))
	{
		ennemi_x[current_ennemi] = random(MAP_WIDTH);
		ennemi_y[current_ennemi] = random(MAP_HEIGHT);
		tule_nb = map_get_tile(ennemi_x[current_ennemi],ennemi_y[current_ennemi]);
	}
	ennemi_type[current_ennemi] = tule_nb;
	
	ennemi_x[current_ennemi] = ennemi_x[current_ennemi]*16;
	ennemi_y[current_ennemi] = ennemi_y[current_ennemi]*16;
	ennemi_wait[current_ennemi] = 0;
	ennemi_beer[current_ennemi] = 0;
	ennemi_dir[current_ennemi] = DIR_RIGHT;
	
	spr_make((current_ennemi*2 + 2),ennemi_x[current_ennemi],ennemi_y[current_ennemi]-8,0x6000,FLIP_X_MASK|SIZE_MAS,NO_FLIP_X|SZ_32x16,1,1);
	set_sprpal(1,flechepal,1);
	load_vram(0x6000,fleche,0x200);
	
	spr_show(current_ennemi*2 + 2); 
	
	ennemi_state[current_ennemi] = 2;
}

ennemi_sprite_2(current_ennemi)
int current_ennemi;
{
	ennemi_wait[current_ennemi]++;
	spr_set((current_ennemi*2 + 2));
	if(ennemi_wait[current_ennemi] > 180) spr_pattern(0x6080);
	if(ennemi_wait[current_ennemi] > 360) spr_pattern(0x6100);
	if(ennemi_wait[current_ennemi] > 540) ennemi_state[current_ennemi] = 4;
}

ennemi_sprite_3(current_ennemi)
int current_ennemi;
{
	ennemi_beer[current_ennemi]++;
	if(ennemi_beer[current_ennemi] > 180) 
	{
		ennemi_state[current_ennemi] = 0;
		spr_hide(current_ennemi*2 + 2);
		money += 5;
		put_number(money, 5, 8, 27); /* nan pasqueu c'est pas non plus la peine de le maj à chaque frame */
		srand(counter);		/* re-randomise le random! */
		/* je souris, ça se voie au téléphone */
		spr_set(0);
		spr_pattern(0x5580);
		spr_set(1);
		spr_pattern(0x5780);
	}
	else ennemi_state[current_ennemi] = 2;
}

ennemi_sprite_4(current_ennemi)
int current_ennemi;
{
	int haut, bas;
	put_tile(17, ennemi_x[current_ennemi]>>4, ennemi_y[current_ennemi]>>4); 
	put_tile(25, ennemi_x[current_ennemi]>>4, (ennemi_y[current_ennemi]>>4)+1); 
	ennemi_x[current_ennemi] = ennemi_x[current_ennemi] - 8;
	ennemi_y[current_ennemi] = ennemi_y[current_ennemi] + 16;
	
	if(ennemi_type[current_ennemi]==0 || ennemi_type[current_ennemi]==1) 
	{
		haut=0x7000;
		bas=0x7200;
	}
	if(ennemi_type[current_ennemi]==2 || ennemi_type[current_ennemi]==3)
	{
		haut=0x7100;
		bas=0x7300;
	}
	if(ennemi_type[current_ennemi]==4 || ennemi_type[current_ennemi]==5)
	{
		haut=0x7400;
		bas=0x7600;
	}
	if(ennemi_type[current_ennemi]==6 || ennemi_type[current_ennemi]==7)
	{
		haut=0x7500;
		bas=0x7700;
	}
	
	spr_make((current_ennemi*2 + 2),ennemi_x[current_ennemi],ennemi_y[current_ennemi],haut,FLIP_X_MASK|SIZE_MAS,FLIP_X|SZ_32x16,2,1);
	spr_make((current_ennemi*2 + 3),ennemi_x[current_ennemi],ennemi_y[current_ennemi]+16,bas,FLIP_X_MASK|SIZE_MAS,FLIP_X|SZ_32x16,2,0);
	set_sprpal(2,ennemipal,2);
	load_vram(0x7000,ennemi,0x1000);
	
	ennemi_state[current_ennemi] = 5;
}

ennemi_sprite_5(current_ennemi)
int current_ennemi;
{
	int dir, bool, tule_nb, res;
	bool = 0;
	if(ennemi_x[current_ennemi] == 0) 
		{
			ennemi_dir[current_ennemi] = DIR_RIGHT;
			bool = 1;
		}
		if(ennemi_x[current_ennemi] == 240) 
		{
			ennemi_dir[current_ennemi] = DIR_LEFT;
			bool = 1;
		}
		if(ennemi_y[current_ennemi] == 0) 
		{
			ennemi_dir[current_ennemi] = DIR_DOWN;
			bool = 1;
		}
		if(ennemi_y[current_ennemi] == 208) 
		{
			ennemi_dir[current_ennemi] = DIR_UP;
			bool = 1;
		}
	if((ennemi_x[current_ennemi]+8)%16 == 0 && ennemi_y[current_ennemi]%16 == 0)
	{
		dir = random(4) +1;
		/*put_number(dir, 5 , 5, 0);*/
		while(bool == 0)
		{
			if(dir == 1)
			{
				get_pxl_tile(ennemi_x[current_ennemi]-8,ennemi_y[current_ennemi]+16,&tule_nb);
				/*put_number(tule_nb, 5 , 5, 1);*/
				get_tangible_tiles(tule_nb,&res);
				/*put_number(res, 5 , 5, 2);*/
				if(res == 0)
				{
					ennemi_dir[current_ennemi] = dir;
					bool = 1;
				}
			}
			if(dir == 2)
			{
				get_pxl_tile(ennemi_x[current_ennemi]+24,ennemi_y[current_ennemi]+16,&tule_nb);
				/*put_number(tule_nb, 5 , 5, 1);*/
				get_tangible_tiles(tule_nb,&res);
				/*put_number(res, 5 , 5, 2);*/
				if(res == 0)
				{
					ennemi_dir[current_ennemi] = dir;
					bool = 1;
				}
			}
			if(dir == 3)
			{
				get_pxl_tile(ennemi_x[current_ennemi]+8,ennemi_y[current_ennemi],&tule_nb);
				/*put_number(tule_nb, 5 , 5, 1);*/
				get_tangible_tiles(tule_nb,&res);
				/*put_number(res, 5 , 5, 2);*/
				if(res == 0)
				{
					ennemi_dir[current_ennemi] = dir;
					bool = 1;
				}
			}
			if(dir == 4)
			{
				get_pxl_tile(ennemi_x[current_ennemi]+8,ennemi_y[current_ennemi]+32,&tule_nb);
				/*put_number(tule_nb, 5 , 5, 1);*/
				get_tangible_tiles(tule_nb,&res);
				/*put_number(res, 5 , 5, 2);*/
				if(res == 0)
				{
					ennemi_dir[current_ennemi] = dir;
					bool = 1;
				}
			}
			dir = random(4)+1;
		}
		/*put_number(ennemi_dir[current_ennemi], 5 , 5, 4);*/
	}
	if(ennemi_dir[current_ennemi] == DIR_LEFT) ennemi_x[current_ennemi]--;
	if(ennemi_dir[current_ennemi] == DIR_RIGHT) ennemi_x[current_ennemi]++;
	if(ennemi_dir[current_ennemi] == DIR_UP) ennemi_y[current_ennemi]--;
	if(ennemi_dir[current_ennemi] == DIR_DOWN) ennemi_y[current_ennemi]++;
	
}

get_pxl_tile(in_x,in_y,res)
int in_x, in_y, *res;
{
	*res = map_get_tile(in_x >> 4, in_y >> 4);
}

get_tangible_tiles(in_val,res)
int in_val,*res;
{
	if (in_val > 7 && in_val < 16) *res = 1;
	else *res = 0;
}

manage_pants(tile_nb1,tile_nb2,tile_nb3,tile_nb4)
int tile_nb1,tile_nb2,tile_nb3,tile_nb4;
{
	int sprpri;
	if((tile_nb1==24 && tile_nb2==24)||(tile_nb3==24 && tile_nb4==24)) sprpri = 1;
	else sprpri = 0;
	spr_set(1);
	spr_pri(sprpri);
}

physique()
{
	int tile_nb1,tile_nb2,tile_nb3,tile_nb4;
	get_pxl_tile((sprite_nx+8),(sprite_ny+16),&tile_nb1); /* haut gauche*/
	get_pxl_tile((sprite_nx+23),(sprite_ny+16),&tile_nb2); /* haut droit*/
	get_pxl_tile((sprite_nx+8),(sprite_ny+31),&tile_nb3); /* bas gauche*/
	get_pxl_tile((sprite_nx+23),(sprite_ny+31),&tile_nb4); /* bas droit*/
	/*
	put_number(tile_nb1, 5 , 5, 0);
	put_number(tile_nb2, 5 , 5, 1);
	put_number(tile_nb3, 5 , 5, 2);
	put_number(tile_nb4, 5 , 5, 3);
	*/
	manage_pants(tile_nb1,tile_nb2,tile_nb3,tile_nb4);
	
	get_tangible_tiles(tile_nb1,&tile_nb1);
	get_tangible_tiles(tile_nb2,&tile_nb2);
	get_tangible_tiles(tile_nb3,&tile_nb3);
	get_tangible_tiles(tile_nb4,&tile_nb4);
	if(tile_nb1==1)
	{
		if(tile_nb2==1) sprite_ny = sprite_y;
		else
			{
				if(dir_sprite == DIR_UP) sprite_nx++;
			}
		if(tile_nb3==1) sprite_nx = sprite_x;
		else
			{
				if(dir_sprite == DIR_LEFT) sprite_ny++;
			}
	}
	if(tile_nb2==1)
	{
		if(tile_nb1==1) sprite_ny = sprite_y;
		else
			{
				if(dir_sprite == DIR_UP) sprite_nx--;
			}
		if(tile_nb4==1) sprite_nx = sprite_x;
		else
			{
				if(dir_sprite == DIR_RIGHT) sprite_ny++;
			}
	}
	if(tile_nb3==1)
	{
		if(tile_nb4==1) sprite_ny = sprite_y;
		else
			{
				if(dir_sprite == DIR_DOWN) sprite_nx++;
			}
		if(tile_nb1==1) sprite_nx = sprite_x;
		else
			{
				if(dir_sprite == DIR_LEFT) sprite_ny--;
			}
	}
	if(tile_nb4==1)
	{
		if(tile_nb3==1) sprite_ny = sprite_y;
		else
			{
				if(dir_sprite == DIR_DOWN) sprite_nx--;
			}
		if(tile_nb2==1) sprite_nx = sprite_x;
		else
			{
				if(dir_sprite == DIR_RIGHT) sprite_ny--;
			}
	}
	sprite_x = sprite_nx;
	sprite_y = sprite_ny;
	spr_put(0,sprite_x,sprite_y, dir_sprite);
	spr_put(1,sprite_x,sprite_y+16, dir_sprite);
	
	for(i=0;i<16;i++)
	{
		if(ennemi_state[i] == 5)
		{
			spr_put(i*2+2,ennemi_x[i],ennemi_y[i], ennemi_dir[i]);
			spr_put(i*2+3,ennemi_x[i],ennemi_y[i]+16, ennemi_dir[i]);
			if (ennemi_x[i] > sprite_x -2 && ennemi_x[i] < sprite_x +2 &&
			ennemi_y[i] > sprite_y -2 && ennemi_y[i] < sprite_y +2)
			{
				state_sprite = 3;
			}
		}
	}
	
	if(state_sprite ==3 && dead > 180) 
	{
		for(i=0;i<34;i++)
		{
			spr_hide(i);
		}
		satb_update();
		main();
	}
}