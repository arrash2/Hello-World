#include <sdl/sdl.h>

//structure de la map
typedef struct
{
	SDL_Rect R;
	int mur;
	// on peut rajouter de mur bloquant en ajoutant d'autre struct comme mur2
} TileProp;

typedef struct
{
	int LARGEUR_TILE,HAUTEUR_TILE;
	int nbtilesX,nbtilesY;
	SDL_Surface* tileset;
	TileProp* props;
	Uint16** schema;
	int nbtiles_largeur_monde,nbtiles_hauteur_monde;
	int xscroll,yscroll;
	int largeur_fenetre,hauteur_fenetre;
} Map;

Map* ChargerMap(const char* fic,int largeur_fenetre,int hauteur_fenetre);
int AfficherMap(Map* m,SDL_Surface* screen);
int LibererMap(Map* m);
int CollisionDecor(Map* carte,SDL_Rect* perso);
int FocusScrollCenter(Map* carte,SDL_Rect* perso);
int FocusScrollBox(Map* carte,SDL_Rect* perso,SDL_Rect* limit);
//structure de reception de commande appuyer par l'utilisateur
typedef struct
{
	char key[SDLK_LAST];
	char quit;
} Input;

void UpdateEvents(Input* in);
void InitEvents(Input* in);
