#include "fonction.h"

#define LARGEUR_FENETRE 900
#define HAUTEUR_FENETRE 600
#define MOVE_SPEED_PLAYER 2

#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1)))
#define ABS(X) ((((X)<0)?(-(X)):(X)))

void RecupererVecteur(Input* in,int* vx,int* vy)
{
	int vitesse = 2;//initialisation des vitesse
	*vx = 1;//vitesse constante sur l'axe des abscisse
	*vy = 2;//"gravité"qui pousse le perso vers le sol
	if (in->key[SDLK_UP])//direction du perso lorsque l'on appuye sur la fleche du haut
        *vy = -vitesse;//-vitesse permet d'avoir vy = -2 et fait "voler"le perso
}

int EssaiDeplacement(Map* carte,SDL_Rect* perso,int vx,int vy)//test les deplacements et renvoie des valeurs
{
	SDL_Rect test;
	test = *perso;
	test.x+=vx;
	test.y+=vy;
	if (CollisionDecor(carte,&test)==0)//on fait appelle a la fonction collisionDecor defini dans fonction.c
	{
		*perso = test;//si il n'y a pas de collision alors essaideplacement prend la valeur 1
		return 1;
	}

	return 0;
}


void Deplace(Map* carte,SDL_Rect* perso,int vx,int vy,int LARGEUR_TILE,int HAUTEUR_TILE)//fontion qui gere les deplacement
{
	int i;

   // affine
	for(i=0;i<ABS(vx);i++)
	{
		if (EssaiDeplacement(carte,perso,SGN(vx),0)==0)
			 {
                exit(0);
            }

	}
	for(i=0;i<ABS(vy);i++)
	{
		if (EssaiDeplacement(carte,perso,0,SGN(vy))==0)
			 {
               exit(0);
            }

	}

}

void Evolue(Input* in,Map* carte,SDL_Rect* perso,int LARGEUR_TILE,int HAUTEUR_TILE)//fontion qui gere l'evolution du perso sur la map
{
	int vx,vy;
	RecupererVecteur(in,&vx,&vy);//recuperation de la gravité et du mouvement perpetuel vers la droite
	Deplace(carte,perso,vx,vy,LARGEUR_TILE,HAUTEUR_TILE);//appliquation sur le deplacement


}

void AfficherPerso(SDL_Rect *perso,SDL_Surface* screen,int xscroll,int yscroll)//affichage perso
{
	SDL_Rect positionsurecran = *perso;//definition de la position du perso qui sera definit plus tard
	positionsurecran.x -= xscroll;//utilisation du scrolling pour redefinr la position du perso avec le scrolling
	positionsurecran.y -= yscroll;
	SDL_FillRect(screen,&positionsurecran,0xFFFFFF);//création du perso avec un rectangle blanc(0xFFFFFF)
}

int main(int argc, char *argv[])//Le main le coeur du jeu
{

    int p=1,temps=0,score=0,tempsPrecedent=0;//declaration des variables
	SDL_Rect perso,limit;//definit des zones de l'ecran
	SDL_Surface* screen ;//structure qui contient les pixel qui definiront l'écran
	SDL_Surface *image;//structure qui contient les pixel qui definiront l'image d'acceuil


	Map* carte;//appelle de structure map definit dans fonction.h
	Input in;// appelle de la structure des input definit dans fonction.h
	int LARGEUR_TILE,HAUTEUR_TILE;//declaration de la largeur et la hauteur d'un tile
	LARGEUR_TILE = 24;//ici 24 de largeur
	HAUTEUR_TILE = 16;//et 16 de hauteur
	memset(&in,0,sizeof(in));//appelle de memset definit dans fonction.h
    SDL_Init(SDL_INIT_VIDEO);		// preapare SDL

    while (p==1)//lancement du jeu mais d'abord de la page d'acceuil
    {

    screen = SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32,SDL_HWSURFACE|SDL_DOUBLEBUF);//initialisation de l'écran
    image = SDL_LoadBMP("acceuil.bmp");//"enregistrer" l'image d'acceuil qui en bmp
    SDL_BlitSurface(image, NULL, screen, NULL);//on blit l'image d'acceuil sur l'ecran
    SDL_UpdateRect(screen, 0, 0, 0, 0);

    UpdateEvents(&in);//fonction qui attend qu'un evenement ce passe et renvoie la touche appuyer

    if (in.key[SDLK_SPACE])//si on appuye sur espace
    {
        p=p+1;//on passe p a 2 pour que la boucle superieur ne soit plus verfiée
        screen = SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32,SDL_HWSURFACE|SDL_DOUBLEBUF);//redefinition de l'ecran

    carte = ChargerMap("level.txt",LARGEUR_FENETRE,HAUTEUR_FENETRE);//on charge la carte
	perso.x = 150;//inintialisation de la position de depart du perso
	perso.y = 150;
	perso.w = 25;//inintialisation de la taille du perso
	perso.h = 40;
	limit.x = 200;//mise en place des limites
	limit.y = 150;
	limit.w = 400;
	limit.h = 300;


        FILE* fichier = NULL;//ebauche d'un scorring pour le jeu
        fichier = fopen("test.txt", "w");//ecriture du score sur un texte




                while (!in.key[SDLK_ESCAPE])
                {

                    temps = SDL_GetTicks();//score definit grace au temps
                    if (temps - tempsPrecedent > 30)//boucle qui fais grossir le score toute les 30ms
                        {
                            tempsPrecedent = temps;
                            score = temps/10;
                            fprintf(fichier, "%d", score);
                            fclose(fichier);
                        }


                    UpdateEvents(&in);//fontion qui renvoie les touches appuyées
                    Evolue(&in,carte,&perso,LARGEUR_TILE,HAUTEUR_TILE);//utilisation de la fonction evolue
                    FocusScrollCenter(carte,&perso);//scrolling
                    AfficherMap(carte,screen);//affichage de la map en fonction des deplacement réalisé
                    AfficherPerso(&perso,screen,carte->xscroll,carte->yscroll);//affichage du perso avec le scrolling
                    SDL_Flip(screen);//rafraichissement de l'ecran
                    SDL_Delay(4);//attente ce qui permes de gere la vitesse du jeu et donc la difficultée


                }
            }
        }

return 0;
}
