//Oskari Virtanen ID: 1706816
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include<conio.h>
//struct that makes movement within the player map possible
struct vector
{
	int y, x;
};
//struct that holds the template for the player and game maps
//contains the char array pointer, a size vector and live of that map
struct map
{
	char **map;
	struct vector size;
	int lives;
};
//function that generates the map taking as parameters y and x maximum, frequency of bomb generation and is the map generated the player map or game map
//it uses the malloc() function to reserve desired size of memory for the map array
//it also uses the rand() function to generate bombs on a map if its generated as a game map
char** mapGenerator(int yMAX,int xMAX, int bombGen, int player)
{
	int y, x;
	char **map = (char**)malloc(yMAX * sizeof(char *));
	for (y = 0; y < yMAX; y++)
		map[y] = (char *)malloc(xMAX * sizeof(char));
	for (y = 0; y < yMAX; y++)
	{
		for (x = 0; x < xMAX; x++)
		{
			if ((rand() % bombGen) == 1 && player ==0)
			{
				map[y][x] = 'B';
			}
			else {
				map[y][x] = 'x';
			}
		}
	}
	return map;
}
//Gives the game map a win condition by calculating how many bombs are present in the array
void winCondition(struct map *mapG,int yMAX,int xMAX)
{
	mapG->lives = 0;
	for (int y = 0; y < yMAX; y++)
	{
		for (int x = 0; x < xMAX; x++)
		{
			if (mapG->map[y][x] == 'B')
			{
				mapG->lives += 1;
			}
		}
	}

}
//function that uses the vector struct to move players position in the player map array using W,A,S,D keys.
//funtion recognizes the limits of the map array and will not exceed them.
void move(struct vector *pPos, int c, int yMAX, int xMAX)
{
	switch (c)
	{
	case 'w':
		pPos->y -= 1;
		if (pPos->y < 0)
		{
			pPos->y = 0;
		}
		break;
	case 's':
		pPos->y += 1;
		if (pPos->y > yMAX)
		{
			pPos->y = yMAX;
		}
		break;
	case 'a':
		pPos->x -= 1;
		if (pPos->x < 0)
		{
			pPos->x = 0;
		}
		break;
	case 'd':
		pPos->x += 1;
		if (pPos->x > xMAX)
		{
			pPos->x = xMAX;
		}
		break;
	default:
		break;
	}
}
/*function that when activated reveals a 3x3 area around the players position calculating for each position a number based on bombs surrounding each
  point of observation in another 3x3 square
  so:          would reveal;

     x x x x x				x x x x x 
	 x x x x B				x 0 0 2 x
	 x x p x B				x 0 p 2 x
	 x x x x x				x 0 0 1 x
	 x x x x x			    x x x x x
  
  Funtion is also programmed to stay within the max and min limits of the game map
  if a bomb is present within the area of observation it stays as a x

*/
void reveal(struct vector pPos, struct map *mapP, struct map *mapG,int yMAX,int xMAX)
{
	if (mapG->map[pPos.y][pPos.x] != 'B' && mapG->map[pPos.y][pPos.x] != 'F' && mapP->map[pPos.y][pPos.x] == 'x')
	{
		mapP->map[pPos.y][pPos.x] = '0';
		for (int y = -1; y < 2; y++)
		{
			for (int x = -1; x < 2; x++)
			{
					if (pPos.y + y >= 0 && pPos.x + x >= 0 && pPos.y + y < yMAX && pPos.x + x < xMAX)
					{
						if (mapG->map[pPos.y + y][pPos.x + x] == 'x')
						{
							mapP->map[pPos.y + y][pPos.x + x] = '0';
							for (int i = -1; i < 2; i++)
							{
								for (int j = -1; j < 2; j++)
								{
									if (pPos.y + y + i >= 0 && pPos.x + x + j >= 0 && pPos.y + y + i < yMAX && pPos.x + x + j < xMAX)
									{
										if (mapG->map[pPos.y + y + i][pPos.x + x + j] == 'B')
										{
											mapP->map[pPos.y + y][pPos.x + x] += 1;
										}
									}
								}

							}
						}
					}
			}
		}
	}
}
/* Function that places and removes flags on positions where the player believes there is a bomb or checks sourrounding area for bombs
   if a bomb is revealed on the position of the player, the player loses one life
   if a bomb is flagged the flag becomes permanent and the game map loses one life
*/
void check(struct vector pPos, struct map *mapP, struct map *mapG, int c, int yMAX,int xMAX)
{
	switch (c)
	{
	case 'f':
		if (mapG->map[pPos.y][pPos.x] == 'B')
		{
			mapG->lives -= 1;
			mapG->map[pPos.y][pPos.x] = 'F';
		}
		if (mapP->map[pPos.y][pPos.x] == 'F' && mapG->map[pPos.y][pPos.x] != 'F')
		{
			mapP->map[pPos.y][pPos.x] = 'x';
		}
		else {
			mapP->map[pPos.y][pPos.x] = 'F';
		}

		break;
	case 'c':
		if (mapG->map[pPos.y][pPos.x] == 'B')
		{
			mapP->lives -= 1;
			mapP->map[pPos.y][pPos.x] = 'B';
		}
		else {
			reveal(pPos,mapP,mapG,yMAX,xMAX);
		}
		break;
	default:
		break;
	}
}
/* The main gameplay function that is invoked when starting the game from the main menu.
   It takes as parameters the values of the maps and uses other functions to generate them.
   It uses functions above to run the game until either player map or game map loses all lives, ending the game.
   Every action taken refreshes the printed screen showing the player all relevant gameplay information(map,lives,remaining bombs and players position on the map)
*/
void game(int yMAX, int xMAX, int bombGen, int lives)
{
	int action = 0, x, y;
	struct map mapG;
	mapG.map = mapGenerator(yMAX, xMAX, bombGen, 0);
	winCondition(&mapG, yMAX, xMAX);
	struct map mapP;
	mapP.map = mapGenerator(yMAX, xMAX, bombGen, 1);
	mapP.lives = lives;
	struct vector pPos = { 0,0 };
	while(mapP.lives > 0)
	{
		/*
		for (y = 0; y < yMAX; y++)
		{
			for (x = 0; x < xMAX; x++)
			{
				if (x == xMAX - 1)
				{
					printf("%c\n", mapG.map[y][x]);
				}
				else {
					printf("%c ", mapG.map[y][x]);
				}
			}
		}
		*/
		printf("\n");
		for (y = 0; y < yMAX; y++)
		{
			for (x = 0; x < xMAX; x++)
			{
				if (x == xMAX - 1)
				{
					if (y == pPos.y && x == pPos.x)
					{
						printf("p\n");
					}
					else {
						printf("%c\n", mapP.map[y][x]);
					}
				}
				else {
					if (y == pPos.y && x == pPos.x)
					{
						printf("p ");
					}
					else {
						printf("%c ", mapP.map[y][x]);
					}
				}
			}

		}
		printf("%d bombs remaining\n", mapG.lives);
		printf("%d lives remaining\n", mapP.lives);
		action = _getch();
		if (action == 'f')
		{
			check(pPos,&mapP,&mapG,action,yMAX,xMAX);
		}
		else if(action == 'c'){
			check(pPos, &mapP, &mapG, action, yMAX, xMAX);
		}
		else {
			move(&pPos, action, yMAX, xMAX);
		}
		if (mapG.lives == 0)
		{
			mapP.lives = 0;
		}
		system("cls");
	}
	if (mapG.lives == 0) 
	{
		for (y = 0; y < yMAX; y++)
		{
			for (x = 0; x < xMAX; x++)
			{
				if (x == xMAX - 1)
				{
					printf("%c\n", mapG.map[y][x]);
				}
				else {
					printf("%c ", mapG.map[y][x]);
				}
			}
		}
		printf("you won\nyou found all the bombs\n");
	}
	if (mapG.lives > 0)
	{
		for (y = 0; y < yMAX; y++)
		{
			for (x = 0; x < xMAX; x++)
			{
				if (x == xMAX - 1)
				{
					printf("%c\n", mapG.map[y][x]);
				}
				else {
					printf("%c ", mapG.map[y][x]);
				}
			}
		}
		printf("you lost\nthere were still %d bombs\n",mapG.lives);
	}
	system("pause");

}
/*The main menu of the game where the player can change settings from the default, run the game or exit the program.
  Shows the player relevant information about the game (size,lives,bomb frequency) and shows the player in which menu the player is.
  It also shows the player options for changing the settings and quitting or starting the game.
*/
int main()
{
	int yMAX = 10, xMAX = 10 , bombGen = 10, lives = 2, gamerunning = 1,inmenu = 1, choice;
	srand(time(NULL));
	while (gamerunning == 1)
	{
		system("cls");
		printf("Minesweeper game.\ncurrent settings:\n map settings: y axis %d x axis %d\n bomb frequency: 1/%d\n lives: %d",yMAX,xMAX,bombGen,lives);
		printf("\n\n To begin the game press 1 \n To change settings press 2 \n To exit press 3 \n");
		choice = _getch();
		switch (choice)
		{
		case '1':
			game(yMAX, xMAX, bombGen, lives);
			break;
		case '2':
			inmenu = 1;
			while (inmenu == 1)
			{
				system("cls");
				printf("current settings:\n map settings: y axis %d x axis %d\n bomb frequency: 1/%d\n lives: %d", yMAX, xMAX, bombGen, lives);
				printf("\n\n Return to main menu press 1\n To change both y and x axis press 2\n To change y and x axis seperately press 3\n To change bomb frequency press 4\n To change number of lives press 5\n");
				choice = _getch();
				switch (choice)
				{
				case '1':
					inmenu = 0;
					break;
				case '2':
					printf("\nenter new y and x: ");
					scanf_s("%d", &yMAX);
					xMAX = yMAX;
					break;
				case '3':
					printf("\nenter new y: ");
					scanf_s("%d", &yMAX);
					printf("\nenter new x: ");
					scanf_s("%d", &xMAX);
					break;
				case '4':
					printf("\nenter new frequency 1/");
					scanf_s("%d", &bombGen);
					break;
				case '5':
					printf("\nenter new number of lives: ");
					scanf_s("%d", &lives);
					break;
				default:
					break;
				}
			}
			//scanf_s("%d %d %d %d", &yMAX, &xMAX, &bombGen, &lives);
			break;
		case '3':
			gamerunning = 0;
			break;
		default:
			printf("error");
			break;
		}
		
	}

}