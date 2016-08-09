#include "huc.h"

#include "bgs.c"


main()
{
	int joie, golbal;
	cls();
	init_satb();
	reset_satb();
	golbal = 0;

	
	set_color(0,0); /* Je veux le noir */
	set_color(1,511); /* Je veux le blanc */
	set_font_color(1, 0);
	load_default_font();
	put_string("..:: Beer Girl Stadium ::..", 3, 3);
	put_string(" __-- Push RUN --__",6,6);
	put_string("Code - Gfx - Msx by Bjorn.",1,10);
	put_string("Original concept & design by",1,12);
	put_string("HoKKaido.",22,13);
	put_string("Entry for AGBIC 2016",1,16);
	for(;;)
	{	
		golbal++;
		joie = joy(0);
		if (joie & JOY_STRT) 
		{
			srand(golbal);
			bgs();
		}
		
	}
}