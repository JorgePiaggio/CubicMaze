#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <strings.h>
#include <unistd.h>
#include <ctype.h>
#include "gotoxy.h"

#define fila 27
#define colum 53
#define CARAS 6
#define LLAVES -61
#define PERSEGUIDORES -66
#define MARCOS -78
#define MUROS -80
#define JUGADOR 148
#define VACIO 32
#define TUMBA 241
#define SALIDA 63
#define MONEDA 79
#define CORRIMIENTOX 8
#define CORRIMIENTOY 11


typedef struct
{
    char nombre[15];
    char password[15];
    int partidasJugadas;
    int partidasGanadas;
    int nivelNormal; // ==CERO SI NO LO SUPERO, ==UNO SI LO SUPERO
    int nivelDificil; // ==CERO SI NO LO SUPERO, ==UNO SI LO SUPERO
    int highscoresNormal[4];//ARREGLOS CON MEJORES SCORES POR NIVEL
    int highscoresDificil[4];
    int highscoresInfinito[4];
    char fechayhoraNormal[4][100];//ARREGLOS CON LAS FECHAS Y HS CORRESPONDIENTES CADA JUGADOR
    char fechayhoraDificil[4][100];
    char fechayhoraInfinito[4][100];
} usuarios;

struct datosCubeMaze
{
    usuarios usr[100];//ARREGLO STRUCT USUARIOS
    int cantUsr;//CONTADOR DE USUARIOS CARGADOS
    int highscoresNormal[6];//ARREGLOS CON MEJORES SCORES POR NIVEL
    int highscoresDificil[6];
    int highscoresInfinito[6];
    char highscoresNormalNombres[6][15];//MATRICES CON NOMBRES DE HIGHSCORERS
    char highscoresDificilNombres[6][15];
    char highscoresInfinitoNombres[6][15];
    char fechayhoraNormalTOP[6][100];//ARREGLOS CON LAS FECHAS Y HS CORRESPONDIENTES A LOS MEJORES 5 JUGADORES
    char fechayhoraDificilTOP[6][100];
    char fechayhoraInfinitoTOP[6][100];
} datos, *dat=&datos;


void UbicarJugadorInicio(int filas, int columnas, char laberinto[0][filas][columnas],int *PJX, int *PJY);
void generarLaberinto(int cara,int filas, int columnas, char laberinto[cara][filas][columnas],char,int a[],int b[],int c[],int d[],int UbicarJX[], int UbicarJY[]);
void mostrarLaberinto (int cara,int fil, int col, char m[cara][fil][col]);
void llenarMatriz (int cara,int fil, int col, char m[cara][fil][col]);
void llenarDeO (int cara,int filas, int columnas, char laberinto[cara][filas][columnas], int *, int v[], int e[]);
void enviarVecinos(int cara,int filas, int columnas, char laberinto[cara][filas][columnas], int iActual, int jActual,int x[],int y[], int *, int *,int x1[], int x2[]);
void generarPuente(int cara,int filas, int columnas, char laberinto[cara][filas][columnas], int iActual, int jActual, int, int);
void abrirPuertas(int cara,int filas,int columnas,char laberinto[cara][filas][columnas], int x[], int y[],int UbicarJX[], int UbicarJY[]);
void ElegiryEliminarVecino(int cara,int filas, int columnas, char laberinto[cara][filas][columnas],int ColaX[], int ColaY[],int *cantF,int *, int *);
void imprimirTitulo(int);
void BuscaryEliminar0(int,int,int x[],int y[],int);
void GenerarMONEDAS(int cara,int filas, int columnas,char laberinto[CARAS][filas][columnas]);
//void espejo(int cara,int filas,int columnas,char laberinto[cara][filas][columnas]);
//void EspejarPosGate(int PX[], int PY[]);
void ubicarLlaves(int cara,int filas, int columnas, char laberinto[cara][filas][columnas],char);
void ubicarPerseguidor(int cara,int filas, int columnas, char laberinto[cara][filas][columnas], int x[],int Y[],char);
void ImprimirCascaroncubo(int);
void MoverJugador(int cara,int filas, int columnas, char laberinto[cara][filas][columnas], int *PJX, int *PJY,int*,int*);
int NuevoJuego(char dificultad, int *score, char[], char[], char[], char[], char[]);
int PasarCara(int *cara, int filas, int columnas, char laberinto[CARAS][filas][columnas],int *PJX, int *PJY, int PuertaX[], int PuertaY[],int UbicarJX[], int UbicarJY[]);
void AbrirSalidaFinal(int PuertasX[], int PuertasY[], int *SalidaX, int *SalidaY,int *);
float CalcularDistancia(int x0, int x1, int y0, int y1);
int BuscarMenor(float v[],int cantidad, int pos);
void MoverPerseguidores(int cara,int filas, int columnas, char laberinto[CARAS][filas][columnas],int PXp[], int PYp[], char dificultad, int indicePCercano, int PJX, int PJY,int*);
int ElegirPerseguidorCercano(int PX[],int PY[],int cara,int, int,int cantidad);
int CantidadEnemigos(char dificultad, int*);
void demoLab2();
int registroUsuarios(struct datosCubeMaze *dat);
int ingresoUsuarios(struct datosCubeMaze datos, int*);
int verPerfil(struct datosCubeMaze *dat, int*);
int buscarUsuario(struct datosCubeMaze datos, char[], int *);
int cambiarNombre(struct datosCubeMaze *dat, int);
int cambiarPassword(struct datosCubeMaze *dat, int);
int borrarUsuario(struct datosCubeMaze *dat, int);
void menuAdmin(struct datosCubeMaze *dat, int *);
void ordenarScores(struct datosCubeMaze *dat, int i, int score, char dificultad);
void highscores(struct datosCubeMaze datos);
void ayuda();
void inicializarDatosPrimeraVez(struct datosCubeMaze *dat);
void abrirParchEscribirDatosCerrarParch(struct datosCubeMaze *dat, FILE *parch,int *armaggedon);
void mostrarDemoLab(int, int fil, int col, char demoLab[1][fil][col]);
void PantallaResultado(char R[],int, int );
void PantallaTitulo(void);
void imprimirControles ();


int main()
{
    system("title CUBIC MAZE ©");
    int resultado=0; //GANASTE(0) O PERDISTE(1)         BOOLEAN^(-1)  :v
    char dificultad;
    int score=0;

    //VARIABLES DEL MENU PRINCIPAL
    char opcionIng;//INGRESAR O REGISTRARSE
    char opcion;//OPCIONES MENU PRINCIPAL
    int logIn=0;//USER LOGIN==1 //ADMIN LOGIN==2
    int usrCargado=0;//SI SE CARGO UN USUARIO==1
    int i=0;//POSICION USUARIO QUE INGRESO
    int ouroboros=0;//ES == 1 CUANDO SE CAMBIAN DATOS DEL USUARIO
    int exit=0;//EXIT 1==CERRAR PROGRAMA
    int control=0;


    //CARGAR DATOS DE ARCHIVO
    char archivo[]="datosCubeMaze.bin";
    //CHEQUEAR SI EL ARCHIVO YA EXISTE
    FILE *parch;
    if((parch=fopen(archivo, "rb"))==NULL)
    {
        if((parch=fopen(archivo, "wb"))==NULL)//CREARLO SI NO EXISTE (PRIMERA EJECUCION DEL PROGRAMA)
        {
            printf("NO SE PUDO CREAR EL ARCHIVO\n");
            exit=1;
        }
        else
        {
            //CARGAR ADMIN E INICIALIZAR DATOS EN CERO (PRIMERA EJECUCION DEL PROGRAMA)
            dat->cantUsr=0;
            inicializarDatosPrimeraVez(dat);
            fwrite(&datos, sizeof(datos),1,parch);//ESCRIBIR DATOS EN ARCHIVO
        }
    }
    else //SI YA EXISTE CARGAR DATOS A MEMORIA PARA EMPEZAR PROGRAMA
    {
        while(!feof(parch))
        {
            control=fread(&datos,sizeof(datos),1,parch);
            if(control!=1)
            {
                if(feof(parch))
                    break;
                else
                {
                    printf("ERROR DE LECTURA\n");
                    exit=1;
                }
            }
        }
    }
    control=fclose(parch);
    if(control!=0)
    {
        printf("ERROR CERRANDO ARCHIVO\n");
        exit=1;
    }


    //CARGAR AUDIOS
    char mus1[256];//LEER RUTA DONDE ESTA EL JUEGO PARA LINKEAR WAVS
    getcwd(mus1,255);
    char mus2[256];
    getcwd(mus2,255);
    char mus3[256];
    getcwd(mus3,255);
    char mus4[256];
    getcwd(mus4,255);
    char GameOver[256];
    getcwd(GameOver,255);
    char Victory[256];
    getcwd(Victory,255);
    char wav1[]="\\Guybrush Threepwood's Dream.wav";
    char wav2[]="\\Uno Bis.wav";
    char wav3[]="\\Nintendo Party.wav";
    char wav4[]="\\Cero Asoma.wav";
    char wav5[]="\\Game Over.wav";
    char wav6[]="\\Victory.wav";
    strcat(mus1,wav1);
    strcat(mus2,wav2);
    strcat(mus3,wav3);
    strcat(mus4,wav4);
    strcat(GameOver,wav5);
    strcat(Victory,wav6);
    int musica=0;

    //MENU PRINCIPAL
    hidecursor(0);
    while(exit==0)
    {
        //USUARIO NO LOGUEADO
        while(logIn==0)
        {
            PantallaTitulo();
            printf("\t# 1    REGISTRARSE\n\n\n\n");
            printf("\t# 2    INGRESAR\n\n\n\n\t");
            fflush(stdin);
            scanf("%c", &opcionIng);
            system("cls");
            if((isdigit(opcionIng))==0)//CHEQUEAR QUE INGRESE UN NUMERO...
            {
                PantallaTitulo();
                printf("\n\n\n\n\tINGRESA UNA OPCION VALIDA");
                Sleep(1000);
                system("cls");
            }
            else
            {
                switch(opcionIng)
                {
                case '1':
                    PantallaTitulo();
                    usrCargado=registroUsuarios(dat);
                    if(usrCargado==1)
                    {
                        datos.cantUsr++;
                        abrirParchEscribirDatosCerrarParch(dat,parch,&exit);
                        i=datos.cantUsr-1;
                        logIn=1;
                    }
                    system("cls");
                    break;
                case '2':
                    logIn=ingresoUsuarios(datos, &i);
                    system("cls");
                    break;
                default:
                    exit=1;
                    break;
                }
            }
        }
        //USUARIO LOGUEADO
        while(logIn==1)
        {
            color(10);
            PantallaTitulo();
            if(musica==0)
            {
                PlaySound(TEXT(mus1),NULL, SND_LOOP | SND_ASYNC);
                musica=1;
            }
            printf("\n\n\t# 1    COMENZAR JUEGO\n\n\n\n");//ESO
            printf("\t# 2    PERFIL\n\n\n\n");//VER PERFIL Y HIGHSCORES PERSONALES - EDITAR PERFIL
            printf("\t# 3    HIGHSCORES\n\n\n\n");//MOSTRAR HIGHSCORES GENERAL
            printf("\t# 4    DEMOLAB\n\n\n\n");//DEMO CREACION DE LABERINTO
            printf("\t# 5    AYUDA\n\n\n\n");//AYUDA DEL JUEGO - CREDITOS
            printf("\t# 6    LOGOUT\n\n\n\n");//LOGOUT CUENTA PERSONAL
            printf("\t# 7    SALIR\n\n\n\n\t");//SALIR DEL JUEGO
            fflush(stdin);
            scanf("%c", &opcion);
            system("cls");
            if((isdigit(opcion))==0)
            {
                printf("\n\n\n\n\tINGRESA UNA OPCION VALIDA");
                Sleep(1000);
                system("cls");
            }
            else
            {
                switch(opcion)
                {
                case '1':
                    color(10);
                    PantallaTitulo();
                    printf("\t# 1    NORMAL\n\n\n\n");  //DIFICULTAD==1
                    printf("\t# 2    DIFICIL\n\n\n\n");   //DIFICULTAD==2
                    printf("\t# 3    INFINITO\n\n\n\n\t");  //DIFICULTAD==3
                    fflush(stdin);
                    scanf("%c", &dificultad);
                    system("cls");
                    if((isdigit(dificultad))==0 || dificultad>'3' || dificultad=='0')
                    {
                        printf("\n\n\n\n\tINGRESA UNA OPCION VALIDA");
                        Sleep(1000);
                        system("cls");
                        break;
                    }// ---->BLOQUEAR MODO INFINITO, SE DESACTIVA CON DOS FLAG CUANDO GANA NIVELES NORMAL Y DIFICIL
                    if( dificultad=='3' && (datos.usr[i].nivelDificil==0 || datos.usr[i].nivelNormal==0))
                    {
                        printf("\n\n\n\n\n\tSUPERA LOS NIVELES NORMAL Y DIFICIL \n\n\tPARA ACCEDER AL MODO INFINITO\n\n\n\t");
                        getch();
                        system("cls");
                    }
                    else    //LLAMAR JUEGO NUEVO
                    {
                        musica=0;
                        if(datos.usr[i].partidasJugadas<7)
                        {
                            PantallaTitulo();
                            imprimirControles();
                        }
                        resultado=NuevoJuego(dificultad, &score, mus2, mus3, mus4, GameOver, Victory);
                        if(dificultad!='3')//EN INFINITO SIEMPRE SE PIERDE ASI QUE NO SE SUMAN A LA ESTADISTICA
                        {
                            datos.usr[i].partidasJugadas++;
                            if(resultado==0)//SI GANO
                                datos.usr[i].partidasGanadas++;
                            if(resultado==0 && dificultad=='1')
                                datos.usr[i].nivelNormal=1;     //FLAG NIVEL NORMAL
                            if(resultado==0 && dificultad=='2')
                                datos.usr[i].nivelDificil=1;        //FLAG DIFICIL
                        }
                        ordenarScores(&datos,i,score,dificultad);
                        abrirParchEscribirDatosCerrarParch(dat,parch,&exit);
                        system("cls");
                    }
                    break;
                case '2':
                    ouroboros=verPerfil(&datos, &i);
                    if(ouroboros==1)
                        abrirParchEscribirDatosCerrarParch(dat,parch,&exit);
                    system("cls");
                    break;
                case '3':
                    highscores(datos);
                    system("cls");
                    break;
                case '4':
                    demoLab2(fila,colum);
                    system("cls");
                    break;
                case '5':
                    ayuda();
                    system("cls");
                    break;
                case '6':
                    system("cls");
                    logIn=0;
                    break;
                case '7':
                    system("cls");
                    logIn=0;
                    exit=1;
                    break;
                default:
                    exit=1;
                    break;
                }
            }
            color(8);
        }
        //MENU DEL ADMIN
        while(logIn==2)
        {
            system("cls");
            menuAdmin(dat, &logIn);
            abrirParchEscribirDatosCerrarParch(dat,parch,&exit);
            system("cls");
        }
    }

    hidecursor(1);

    return 0;
}






























void generarLaberinto(int cara,int filas, int columnas, char laberinto[CARAS][filas][columnas],char dificultad,int gatex[],int gatey[], int EnemyX[],int EnemyY[],int UbicarJX[], int UbicarJY[])
{
    int contadorDePosiblesPasillos=0;
//ELEGIR POSICION 'O' OBJETIVO
    int iActual=0,jActual=0;


//PARA ELEGIR VECINOS
    int ianterior=0;
    int janterior=0;
    int colaX[1000];
    int colaY[1000];
    int ColaXO[1000];
    int ColaYO[1000];
    int contarEfes=0;


//LLENAR MATRIZ CON 'X' POR COMPLETO
    llenarMatriz(cara,filas, columnas, laberinto);
//LLENANDO POSIBLES PASILLOS CON 'O'
    llenarDeO(cara,filas, columnas, laberinto, &contadorDePosiblesPasillos,ColaXO,ColaYO);
//ELEGIMOS UNO DE LOS POSIBLES PASILLOS

    ElegiryEliminarVecino(cara,filas,columnas,laberinto,ColaXO,ColaYO,&contadorDePosiblesPasillos,&iActual, &jActual);
//ENVIAR VECINOS A MATRIZ
    enviarVecinos(cara,filas,columnas, laberinto, iActual, jActual, colaX, colaY, &contarEfes, &contadorDePosiblesPasillos,ColaXO,ColaYO);
//MOVERSE A POSICION NUEVA
    while(contadorDePosiblesPasillos>0 || contarEfes>0)
    {
        if(contarEfes>0)
        {
            ElegiryEliminarVecino(cara,filas,columnas,laberinto,colaX,colaY,&contarEfes,&iActual, &jActual);
            generarPuente(cara,filas, columnas, laberinto, iActual, jActual,ianterior,janterior);
        }
        else
        {

            ElegiryEliminarVecino(cara,filas,columnas,laberinto,ColaXO,ColaYO,&contadorDePosiblesPasillos,&iActual, &jActual);
        }

        enviarVecinos(cara,filas,columnas, laberinto, iActual, jActual, colaX, colaY, &contarEfes, &contadorDePosiblesPasillos,ColaXO,ColaYO);
        ianterior=iActual;
        janterior=jActual;

    }
    abrirPuertas(cara,filas,columnas,laberinto,gatex,gatey,UbicarJX,UbicarJY);
    ubicarPerseguidor(cara,filas,columnas,laberinto,EnemyX,EnemyY,dificultad);
    if(dificultad!='3')
    {
        ubicarLlaves(cara,filas, columnas, laberinto,dificultad);
    }
    else
    {
        GenerarMONEDAS(cara,filas,columnas,laberinto);
    }
}


void mostrarLaberinto (int cara,int fil, int col, char m[CARAS][fil][col])
{
    imprimirTitulo(9);
    for(int i=0; i<fil; i++)
    {
        printf("\t");
        for(int j=0; j<col; j++)
        {
//            getch();
//            printf("%d",m[cara][i][j]);
            switch(m[cara][i][j])
            {
            case JUGADOR:
                color(10);
                printf("%c", m[cara][i][j]);
                break;
            case LLAVES: //Pinta llaves
                color(14);
                printf("%c", m[cara][i][j]);
                break;
            case PERSEGUIDORES:// Pinta perseguidores
                color(12);
                printf("%c", m[cara][i][j]);
                break;
            case MUROS:// Pinta paredes
                if(i==0 || j==0 ||i==fil-1||j==col-1)
                {
                    color(9);
                }
                else
                {
                    color(cara+1);
                }
                printf("%c", m[cara][i][j]);
                break;
            case MARCOS:// Pinta paredes de puertas
                color(15);
                printf("%c", m[cara][i][j]);
                break;
            case SALIDA:// Pinta Salida final
                color(14);
                printf("%c", m[cara][i][j]);
                break;
            default:
                if(m[cara][i][j]==MONEDA)
                    color(14);
                printf("%c",m[cara][i][j]);
                break;
            }
        }
        printf("\n");
    }
    ImprimirCascaroncubo(9);
}



void llenarMatriz(int cara,int filas, int columnas, char laberinto[CARAS][filas][columnas])
{
//LLENAR MATRIZ CON 'X'/paredes
    for(int i=0; i<filas; i++)
    {
        for(int j=0; j<columnas; j++)
        {
            //if(j>columnas || i>filas)
            //{
            //BORDES CUBO
            //if(j==65 && i<filas-5)
            //laberinto[i][j]='X';
            //else
            //laberinto[i][j]=VACIO;
            //}
            //MATRIZ LABERINTO
            //else
            laberinto[cara][i][j]=MUROS;
        }
    }
    //BORDES CUBO
    /*for(int i=0; i<filas; i++)
    {
        for(int j=0; j<columnas; j++)
        {
            if(j==56 && i==filas-5)
                laberinto[i][j]='X';
            if(j==52 && i==filas-4)
                laberinto[i][j]='X';
            if(j==48 && i==filas-3)
                laberinto[i][j]='X';
            if(j==44 && i==filas-2)
                laberinto[i][j]='X';
        }
    }*/
}

void llenarDeO (int cara,int filas, int columnas, char laberinto[CARAS][filas][columnas], int *contadorPasillos, int colaXO[],int colaYO[])
{
    //LLENAR CON CANDIDATOS PARA EL CAMINO
    for(int i=1; i<filas-1; i++)
    {
        for(int j=1; j<columnas-1; j++)
        {
            if (i%2!=0 && j%2!=0)
            {
                laberinto[cara][i][j]='O';
                colaXO[*contadorPasillos]=i;
                colaYO[*contadorPasillos]=j;
                *contadorPasillos=*contadorPasillos+1;
            }
        }
    }
}


void enviarVecinos(int cara,int filas, int columnas, char laberinto[CARAS][filas][columnas], int iActual, int jActual, int colaX[], int colaY[], int *contadorDeF, int *contadorDeO, int ColaOx[], int ColaOy[])
{
    //ENVIAR POSICIONES DE VECINOS DE CADA 'O' VISITADA A UN ARREGLO PARALELO
    if(laberinto[cara][iActual+2][jActual]=='O' && iActual+2<filas-1)
    {
        laberinto[cara][iActual+2][jActual]='F';
        *contadorDeF=*contadorDeF+1;
        colaX[*contadorDeF-1]=iActual+2;
        colaY[*contadorDeF-1]=jActual;
        BuscaryEliminar0(iActual+2,jActual,ColaOx,ColaOy,*contadorDeO);
        *contadorDeO=*contadorDeO-1;
    }
    if(laberinto[cara][iActual-2][jActual]=='O')
    {
        laberinto[cara][iActual-2][jActual]='F';
        *contadorDeF=*contadorDeF+1;
        colaX[*contadorDeF-1]=iActual-2;
        colaY[*contadorDeF-1]=jActual;
        BuscaryEliminar0(iActual-2,jActual,ColaOx,ColaOy,*contadorDeO);
        *contadorDeO=*contadorDeO-1;
    }
    if(laberinto[cara][iActual][jActual+2]=='O')
    {
        laberinto[cara][iActual][jActual+2]='F';
        *contadorDeF=*contadorDeF+1;
        colaX[*contadorDeF-1]=iActual;
        colaY[*contadorDeF-1]=jActual+2;
        BuscaryEliminar0(iActual,jActual+2,ColaOx,ColaOy,*contadorDeO);
        *contadorDeO=*contadorDeO-1;
    }
    if(laberinto[cara][iActual][jActual-2]=='O')
    {
        laberinto[cara][iActual][jActual-2]='F';
        *contadorDeF=*contadorDeF+1;
        colaX[*contadorDeF-1]=iActual;
        colaY[*contadorDeF-1]=jActual-2;
        BuscaryEliminar0(iActual,jActual-2,ColaOx,ColaOy,*contadorDeO);
        *contadorDeO=*contadorDeO-1;
    }
}


void generarPuente(int cara,int filas, int columnas, char laberinto[CARAS][filas][columnas], int iActual, int jActual, int ianterior, int janterior)
{
//CONECTAR ESPACIOS VACIOS GENERANDO CAMINO
    if((ianterior<iActual) && (laberinto[cara][ianterior+1][jActual]!='O'))
        laberinto[cara][ianterior+1][jActual]=VACIO;
    if((ianterior>iActual) && (laberinto[cara][ianterior-1][jActual]!='O'))
        laberinto[cara][ianterior-1][jActual]=VACIO;
    if((janterior<jActual) && (laberinto[cara][iActual][janterior+1]!='O'))
        laberinto[cara][iActual][janterior+1]=VACIO;
    if((janterior>jActual) && (laberinto[cara][iActual][janterior-1]!='O'))
        laberinto[cara][iActual][janterior-1]=VACIO;
}

void ElegiryEliminarVecino(int cara,int filas, int columnas, char laberinto[CARAS][filas][columnas], int ColaX[], int ColaY[],int *cantF, int *iactual, int *jactual)
{
    srand(clock());
    int indicerandom=rand()%(*cantF);
    laberinto[cara][ColaX[indicerandom]][ColaY[indicerandom]]=VACIO;
    *iactual=ColaX[indicerandom];
    *jactual=ColaY[indicerandom];

//Corre los valores  y elimina dicha posicion
    for(int k=indicerandom; k<*cantF-1; k++)
    {
        ColaX[k]=ColaX[k+1];
        ColaY[k]=ColaY[k+1];
    }
    *cantF=*cantF-1;
}

// espeja la  matriz cuando cambiamos de cara
//void espejo(int cara,int filas,int columnas,char laberinto[CARAS][filas][columnas])
//{
//
//    char aux[cara][filas][columnas];
//    int f=columnas-1;
//    for(int i=filas-1; i>=0; i--)
//    {
//        for(int j=0; j<=columnas-1; j++)
//        {
//            aux[cara][i][f]=laberinto[cara][i][j];
//            f--;
//        }
//        f=columnas-1;
//    }
//    for(int i=0; i<=filas-1; i++)
//    {
//        for(int j=0; j<=columnas-1; j++)
//        {
//            laberinto[cara][i][j]=aux[cara][i][j];
//        }
//    }
//}

// Invierte o espeja las posiciones en el arreglo de puertas

//void EspejarPosGate(int PX[], int PY[])
//{
//    int aux=0;
//    aux=PY[3];
//    PY[3]=PY[2];
//    PY[2]=aux;
//
//    aux=PX[0];
//    PX[0]=PX[1];
//    PX[1]=aux;
//}



void abrirPuertas(int cara,int filas,int columnas,char laberinto[CARAS][filas][columnas],int PuertasX[],int PuertasY[],int UbicarJX[], int UbicarJY[] )
{
    //Poner un If de si es la primera cara
    srand(clock());
    int RandomFila=(rand()%(filas/3)-1)+1;
    int RandomFila3=RandomFila+(2*(filas/3))-2;
    srand(clock());
    int RandomColumna=(rand()%(((columnas-29)/3)-1))+1;
    int RandomColumna2=RandomColumna+((columnas-29)/3);
    int RandomColumna3=RandomColumna+(2*((columnas-29)/3))-1;
    //Poner un If de si es la primera cara
    //PASILLO FILA SUP
    for(int i=1; i<(filas-1)/2; i++)
    {
        for(int j=1; j<columnas-1; j++)
        {

            laberinto[cara][i][RandomColumna]=VACIO;
            laberinto[cara][i][RandomColumna2]=VACIO;
        }
    }
    //PASILLO FILA INF
    for(int i=(filas-1)/2; i<filas-1; i++)
    {
        for(int j=1; j<columnas-1; j++)
        {
            laberinto[cara][i][RandomColumna3]=VACIO;
        }
    }
    //PASILLO COL SUP
    for(int i=1; i<(filas-1); i++)
    {
        for(int j=1; j<(columnas)/2; j++)
        {
            laberinto[cara][RandomFila+1][j]=VACIO;
        }
    }
    //PASILLO COL INF
    for(int i=1; i<filas-1; i++)
    {
        for(int j=(columnas)/2; j<columnas-1; j++)
        {

            laberinto[cara][RandomFila3][j]=VACIO;
        }
    }
    //PUERTAS
    laberinto[cara][0][RandomColumna]=VACIO;
    laberinto[cara][0][RandomColumna+1]=MARCOS;
    laberinto[cara][0][RandomColumna-1]=MARCOS;
    laberinto[cara][filas-1][RandomColumna3]=VACIO;
    laberinto[cara][filas-1][RandomColumna3+1]=MARCOS;
    laberinto[cara][filas-1][RandomColumna3-1]=MARCOS;
    laberinto[cara][RandomFila+1][0]=VACIO;
    laberinto[cara][RandomFila+2][0]=MARCOS;
    laberinto[cara][RandomFila][0]=MARCOS;
    laberinto[cara][RandomFila3][columnas-1]=VACIO;
    laberinto[cara][RandomFila3+1][columnas-1]=MARCOS;
    laberinto[cara][RandomFila3-1][columnas-1]=MARCOS;

    //PUERTA SUPERIOR y UBICACION JUGADOR EN LA MISMA
    PuertasX[(4*cara)]=0;
    PuertasY[(4*cara)]=RandomColumna;
    UbicarJX[(4*cara)]=1;
    UbicarJY[(4*cara)]=RandomColumna;
    //PUERTA INFERIOR y UBICACION JUGADOR EN LA MISMA
    PuertasX[(4*cara)+1]=filas-1;
    PuertasY[(4*cara)+1]=RandomColumna3;
    UbicarJX[(4*cara)+1]=filas-2;
    UbicarJY[(4*cara)+1]=RandomColumna3;
    //PUERTA IZQUIERDA y UBICACION JUGADOR EN LA MISMA
    PuertasX[(4*cara)+2]=RandomFila+1;
    PuertasY[(4*cara)+2]=0;
    UbicarJX[(4*cara)+2]=RandomFila+1;
    UbicarJY[(4*cara)+2]=1;
    //PUERTA DERECHA y UBICACION JUGADOR EN LA MISMA
    PuertasX[(4*cara)+3]=RandomFila3;
    PuertasY[(4*cara)+3]=columnas-1;
    UbicarJX[(4*cara)+3]=RandomFila3;
    UbicarJY[(4*cara)+3]=columnas-2;
}



void ubicarLlaves(int cara,int filas, int columnas, char laberinto[CARAS][filas][columnas],char dificultad)
{
    //COLOCAR LLAVES EN LABERINTO

    int flag=0; // bandera por si las llaves coinciden con los perseguidores en posicion
    while(flag==0)
    {
        srand(clock());
        int posXllave=(rand()%13)+3;
        srand(clock());
        int posYllave=(rand()%(26))+3;
        srand(clock());
        int posXllave2=(rand()%13)+10;
        srand(clock());
        int posYllave2=(rand()%(26))+23;

        if(laberinto[cara][posXllave][posYllave]!=PERSEGUIDORES && laberinto[cara][posXllave2][posYllave2]!=PERSEGUIDORES && laberinto[cara][posXllave+1][posYllave]!=PERSEGUIDORES && laberinto[cara][posXllave2+1][posYllave2]!=PERSEGUIDORES && laberinto[cara][posXllave-1][posYllave]!=PERSEGUIDORES && laberinto[cara][posXllave2-1][posYllave2]!=PERSEGUIDORES && laberinto[cara][posXllave][posYllave+1]!=PERSEGUIDORES && laberinto[cara][posXllave2][posYllave2+1]!=PERSEGUIDORES && laberinto[cara][posXllave][posYllave-1]!=PERSEGUIDORES && laberinto[cara][posXllave2][posYllave2-1]!=PERSEGUIDORES)
        {

            laberinto[cara][posXllave][posYllave]=LLAVES;
            laberinto[cara][posXllave+1][posYllave]=VACIO;
            laberinto[cara][posXllave-1][posYllave]=VACIO;
            laberinto[cara][posXllave][posYllave+1]=VACIO;
            laberinto[cara][posXllave][posYllave-1]=VACIO;
            if(dificultad!='1')
            {
                laberinto[cara][posXllave2][posYllave2]=LLAVES;
                laberinto[cara][posXllave2+1][posYllave2]=VACIO;
                laberinto[cara][posXllave2-1][posYllave2]=VACIO;
                laberinto[cara][posXllave2][posYllave2+1]=VACIO;
                laberinto[cara][posXllave2][posYllave2-1]=VACIO;
            }
            flag=1;
        }
    }
}


void ubicarPerseguidor(int cara, int filas, int columnas, char laberinto[CARAS][filas][columnas], int PosXP[], int PosYP[],char dificultad)
{
    //COLOCAR Perseguidores EN LABERINTO
    int posXper3=0, posYper3=0,posYper4=0, posXper4=0;   // los inicializo aparte porque estos perseguidores son solo para nivel dificil o infinito

    srand(clock());
    int posXper=(rand()%((filas-5)/2))+2;
    srand(clock());
    int posYper=(rand()%((columnas-4)/2))+2;
    laberinto[cara][posXper][posYper]=PERSEGUIDORES;
    laberinto[cara][posXper+1][posYper]=VACIO;
    laberinto[cara][posXper-1][posYper]=VACIO;
    //laberinto[cara][posXper][posYper+1]=VACIO;
    //laberinto[cara][posXper][posYper-1]=VACIO;
    srand(clock());
    int posXper2=(rand()%((filas-5)/2))+2;
    srand(clock());
    int posYper2=(rand()% ((columnas-4)/2))+((columnas-4)/2);
    laberinto[cara][posXper2][posYper2]=PERSEGUIDORES;
    laberinto[cara][posXper2+1][posYper2]=VACIO;
    laberinto[cara][posXper2-1][posYper2]=VACIO;
    //laberinto[cara][posXper2][posYper2+1]=VACIO;
    //laberinto[cara][posXper2][posYper2-1]=VACIO;
    if(dificultad!='1')
    {
        srand(clock());
        posXper3=(rand()%((filas-5)/2))+((filas-5)/2)+2;
        srand(clock());
        posYper3=(rand()% ((columnas-4)/2))+((columnas-4)/2);
        laberinto[cara][posXper3][posYper3]=PERSEGUIDORES;
        laberinto[cara][posXper3+1][posYper3]=VACIO;
        laberinto[cara][posXper3-1][posYper3]=VACIO;
        //laberinto[cara][posXper3][posYper3+1]=VACIO;
        //laberinto[cara][posXper3][posYper3-1]=VACIO;
    }

    if(dificultad=='3')
    {
        srand(clock());
        posXper4=(rand()%((filas-5)/2))+((filas-5)/2)+2;
        srand(clock());
        posYper4=(rand()% ((columnas-4)/2))+2;
        laberinto[cara][posXper4][posYper4]=PERSEGUIDORES;
        laberinto[cara][posXper4+1][posYper4]=VACIO;
        laberinto[cara][posXper4-1][posYper4]=VACIO;
        //laberinto[cara][posXper3][posYper3+1]=VACIO;
        //laberinto[cara][posXper3][posYper3-1]=VACIO;
    }
    PosXP[(4*cara)]=posXper;
    PosXP[(4*cara)+1]=posXper2;
    PosXP[(4*cara)+2]=posXper3;
    PosXP[(4*cara)+3]=posXper4;
    PosYP[(4*cara)]=posYper;
    PosYP[(4*cara)+1]=posYper2;
    PosYP[(4*cara)+2]=posYper3;
    PosYP[(4*cara)+3]=posYper4;

}

void imprimirTitulo(int colorear)
{
    // BORDE SUPERIOR DEL CUBO
    printf("\n\n\t\t\t\t");
    color(74);
    printf("  C U B I C M A Z E  \n\n\n");
    color(colorear);
    printf("\t\t\t    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    printf("\n\t                  X");
    color(13);
    printf("///////////////////////////////////////////////////");
    //printf("\033[1;34m");
    color(colorear);
    printf("X");
    printf("\n\t             X");
    //printf("\033[1;30m");
    color(13);
    printf("////////////////////////////////////////////////////");
    color(colorear);
    printf("X   X");
    printf("\n\t         X");
    color(13);
    printf("////////////////////////////////////////////////////");
    color(colorear);
    printf("X       X");
    printf("\n\t     X");
    color(13);
    printf("////////////////////////////////////////////////////");
    color(colorear);
    printf("X           X");
    printf("\n\t X");
    color(13);
    printf("////////////////////////////////////////////////////");
    color(colorear);
    printf("X               X");
    printf("\n");
}

void BuscaryEliminar0(int ieliminar, int jeliminar, int ColaOx[], int ColaOy[],int cantO)
{
    //ELMINAR '0' YA ELEGIDAS DEL ARREGLO PARALELO
    int encontrado=0;
    int poseliminar=0;
    int i=0;
    while(encontrado==0 && i<cantO)
    {

        if(ColaOx[i]==ieliminar && ColaOy[i]==jeliminar)
        {
            encontrado=1;
            poseliminar=i;
        }


        i++;
    }

    if(encontrado==1)
    {
        for(int i=poseliminar; i<cantO-1; i++)
        {

            ColaOx[i]=ColaOx[i+1];
            ColaOy[i]=ColaOy[i+1];
        }
    }
}



void MoverPerseguidores(int cara,int filas, int columnas, char laberinto[CARAS][filas][columnas],int PXp[], int PYp[], char dificultad, int indicePCercano, int PJX, int PJY,int *velocidad)
{
    int cantidadPerseguidores=CantidadEnemigos(dificultad,velocidad);
    int contarlibres=0;
    int PoslibresX[4]= {0};
    int PoslibresY[4]= {0};
    int posActualX=0;
    int posActualY=0;
    int indice=0;
    for (int i=(4*cara); i<(4*cara)+cantidadPerseguidores; i++)
    {
        posActualX=PXp[i];
        posActualY=PYp[i];
        if(laberinto[cara][posActualX+1][posActualY]==VACIO || laberinto[cara][posActualX+1][posActualY]==JUGADOR)
        {
            PoslibresX[contarlibres]=posActualX+1;
            PoslibresY[contarlibres]=posActualY;
            contarlibres++;

        }
        if(laberinto[cara][posActualX-1][posActualY]==VACIO || laberinto[cara][posActualX-1][posActualY]==JUGADOR)
        {
            PoslibresX[contarlibres]=posActualX-1;
            PoslibresY[contarlibres]=posActualY;
            contarlibres++;
        }
        if(laberinto[cara][posActualX][posActualY+1]==VACIO || laberinto[cara][posActualX][posActualY+1]==JUGADOR)
        {
            PoslibresX[contarlibres]=posActualX;
            PoslibresY[contarlibres]=posActualY+1;
            contarlibres++;
        }
        if(laberinto[cara][posActualX][posActualY-1]==VACIO || laberinto[cara][posActualX][posActualY-1]==JUGADOR)
        {
            PoslibresX[contarlibres]=posActualX;
            PoslibresY[contarlibres]=posActualY-1;
            contarlibres++;
        }

        if(contarlibres!=0)
        {

            if(i!=((4*cara)+indicePCercano))
            {
                srand(clock());
                indice=rand()%contarlibres;
            }
            else
            {
                indice=ElegirPerseguidorCercano(PoslibresX,PoslibresY,0,PJX,PJY,contarlibres); // re-utilizo esta funcion para buscar los espacios libres mas cercanos segun perseguidor mas cercano
            }
            laberinto[cara][PoslibresX[indice]][PoslibresY[indice]]=PERSEGUIDORES;
            gotoxy(0,0);
            gotoxy(posActualY+CORRIMIENTOX,posActualX+CORRIMIENTOY);
            printf("%c",VACIO);
            gotoxy(0,0);
            gotoxy(PoslibresY[indice]+CORRIMIENTOX,PoslibresX[indice]+CORRIMIENTOY);
            color(12);
            printf("%c",PERSEGUIDORES);
            laberinto[cara][posActualX][posActualY]=VACIO;

            contarlibres=0;
            PXp[i]=PoslibresX[indice];
            PYp[i]=PoslibresY[indice];
        }
    }
}



void UbicarJugadorInicio(int filas, int columnas, char laberinto[0][filas][columnas],int *PJX, int *PJY)
{
    int flag=0;
    while(flag==0)
    {
        srand(clock());
        *PJX=(rand()%((filas-5)/2))+((filas-5)/2)+3;
        *PJY=(rand()%((columnas-4)/2))+2;
        if(laberinto[0][*PJX][*PJY]!=MUROS && laberinto[0][*PJX][*PJY]!=LLAVES &&laberinto[0][*PJX+1][*PJY]!=MUROS && laberinto[0][*PJX+1][*PJY]!=LLAVES && laberinto[0][*PJX+2][*PJY]!=MUROS && laberinto[0][*PJX+2][*PJY]!=LLAVES && laberinto[0][*PJX-1][*PJY]!=MUROS && laberinto[0][*PJX-1][*PJY]!=LLAVES && laberinto[0][*PJX-2][*PJY]!=MUROS && laberinto[0][*PJX-2][*PJY]!=LLAVES)
        {
            laberinto[0][*PJX][*PJY]=JUGADOR;
            laberinto[0][*PJX+1][*PJY]=VACIO;
            laberinto[0][*PJX-1][*PJY]=VACIO;
            laberinto[0][*PJX+2][*PJY]=VACIO;
            laberinto[0][*PJX-2][*PJY]=VACIO;
            flag=1;
        }
    }
}



void MoverJugador(int cara,int filas, int columnas, char laberinto[CARAS][filas][columnas], int *PJX, int *PJY,int *contarllave, int *score)
{

    char teclado;
    teclado=getch();

    if((teclado=='w' || teclado=='W') && laberinto[cara][*PJX-1][*PJY]!=MARCOS && laberinto[cara][*PJX-1][*PJY]!=MUROS )
    {
        laberinto[cara][*PJX][*PJY]=VACIO;
        gotoxy(0,0);
        gotoxy(*PJY+CORRIMIENTOX,*PJX+CORRIMIENTOY);
        printf("%c",VACIO);

        *PJX=*PJX-1;
    }
    if((teclado=='s' || teclado=='S' ) && laberinto[cara][*PJX+1][*PJY]!=MARCOS && laberinto[cara][*PJX+1][*PJY]!=MUROS )
    {
        laberinto[cara][*PJX][*PJY]=VACIO;
        gotoxy(0,0);
        gotoxy(*PJY+CORRIMIENTOX,*PJX+CORRIMIENTOY);
        printf("%c",VACIO);

        *PJX=*PJX+1;
    }
    if((teclado=='d' || teclado=='D') && laberinto[cara][*PJX][*PJY+1]!=MARCOS && laberinto[cara][*PJX][*PJY+1]!=MUROS )
    {
        laberinto[cara][*PJX][*PJY]=VACIO;
        gotoxy(0,0);
        gotoxy(*PJY+CORRIMIENTOX,*PJX+CORRIMIENTOY);
        printf("%c",VACIO);

        *PJY=*PJY+1;
    }
    if((teclado=='a' || teclado=='A') && laberinto[cara][*PJX][*PJY-1]!=MARCOS && laberinto[cara][*PJX][*PJY-1]!=MUROS )
    {
        laberinto[cara][*PJX][*PJY]=VACIO;
        gotoxy(0,0);
        gotoxy(*PJY+CORRIMIENTOX,*PJX+CORRIMIENTOY);
        printf("%c",VACIO);

        *PJY=*PJY-1;
    }

    if(laberinto[cara][*PJX][*PJY]==LLAVES || laberinto[cara][*PJX][*PJY]==MONEDA )
    {
        *contarllave=*contarllave-1;
        *score+=100;
        Beep(1432,200);
    }
    laberinto[cara][*PJX][*PJY]=JUGADOR;
    gotoxy(0,0);
    gotoxy(*PJY+CORRIMIENTOX,*PJX+CORRIMIENTOY);
    color(10);
    printf("%c",JUGADOR);
}



void ImprimirCascaroncubo(int colorear)
{
    for(int i=10; i<30; i++ )
    {
        gotoxy(78,i);
        color(colorear);
        printf("X");
    }
    float a=7.4,b=30;
    for(int j=77; j>=61; j--)
    {
        for(float i=a; i<b; i++ )
        {
            gotoxy(j,i);
            color(13);
            printf("|");
        }
        a+=0.25;
        b+=0.45;
    }

    for(int i=0; i<9; i++)
    {
        gotoxy(78-(2*i),29+i);
        color(colorear);
        printf("X");
    }
    gotoxy(0,40);
}



int NuevoJuego(char dificultad, int *score, char mus2[], char mus3[], char mus4[], char GameOver[], char Victory[])

{
    int puertasx[24];
    int puertasy[24];
    int ubicarJugadorX[24]; /// Ubicaciones de las puertas corridas para ubicar al jugador cuando pasa de cara
    int ubicarJugadorY[24];
    int EnemigoX[24]= {0};
    int EnemigoY[24]= {0};
    char laberinto[CARAS][fila][colum];
    int x=0;
    int y=0;
    int perdiste=0, ganaste=0;
    int cara=0;
    int caraRandom=0;
    int cambioCara=0;
    int contllaves=0;
    int tiempoAnterior=0;
    int ciclo=0;
    int velocidad=0;
    int cantidadEnem=CantidadEnemigos(dificultad,&velocidad);
    int indiceCercano=0;
    int SalidaX=0, SalidaY=0;
    *score=0;

    //CARGAR AUDIO y LLAVES X NIVEL
    switch(dificultad)
    {
    case '1':
        PlaySound(TEXT(mus2),NULL, SND_LOOP | SND_ASYNC);
        contllaves=6;
        break;
    case '2':
        PlaySound(TEXT(mus3),NULL, SND_LOOP | SND_ASYNC);
        contllaves=12;
        break;
    case '3':
        PlaySound(TEXT(mus4),NULL, SND_LOOP | SND_ASYNC);
        contllaves=12;
        break;
    }

    for(int i=0; i<CARAS; i++)
    {
        generarLaberinto(i,fila,colum, laberinto,dificultad,puertasx,puertasy,EnemigoX,EnemigoY,ubicarJugadorX,ubicarJugadorY);
        Sleep(50); // Para que cambie la semilla del clock()
    }
    AbrirSalidaFinal(puertasx,puertasy,&SalidaX,&SalidaY,&caraRandom);
    UbicarJugadorInicio(fila,colum,laberinto,&x,&y);
    indiceCercano=ElegirPerseguidorCercano(EnemigoX,EnemigoY,0,x,y,cantidadEnem);
    mostrarLaberinto(0,fila,colum, laberinto);
    gotoxy(0,0);
    gotoxy(y+CORRIMIENTOX,x+CORRIMIENTOY);
    color(10);
    printf("%c",JUGADOR);

    while(perdiste==0 && ganaste==0)
    {

        if(contllaves==0 && dificultad!='3' && caraRandom==cara) //imprime la salida y establece la condicion de ganar
        {
            laberinto[caraRandom][SalidaX][SalidaY]=SALIDA;
            gotoxy(0,0);
            gotoxy(SalidaY+CORRIMIENTOX,SalidaX+CORRIMIENTOY);
            color(14);
            printf("%c",SALIDA);
            if(SalidaX==x && SalidaY==y)
            {
                *score=*score+100;
                ganaste=1;
            }
        }

        hidecursor(0);

        if(kbhit())
        {
            MoverJugador(cara,fila,colum,laberinto,&x,&y,&contllaves,score);
        }
        else
        {
            if(ciclo==0) // registra los tiempos para mover a los perseguidores cada cierto ciclo
            {
                tiempoAnterior=clock();
                ciclo=1;
            }
            if(clock()-tiempoAnterior>velocidad)
            {
                indiceCercano=ElegirPerseguidorCercano(EnemigoX,EnemigoY,cara,x,y,cantidadEnem);
                MoverPerseguidores(cara,fila,colum,laberinto,EnemigoX,EnemigoY,dificultad,indiceCercano,x,y,&velocidad);
                ciclo=0;
                if(dificultad!='3')
                {
                    if(*score>0)
                        *score=*score-1;
                }
                else
                {
                    if(cara>=0 && cara<CARAS-1)
                    {
                        GenerarMONEDAS(cara+1,fila,colum,laberinto);
                    }
                    *score=*score+1;
                }
            }
        }
        // condicion de game over
        if(laberinto[cara][x][y]==PERSEGUIDORES || laberinto[cara][x+1][y]==PERSEGUIDORES || laberinto[cara][x-1][y]==PERSEGUIDORES || laberinto[cara][x][y-1]==PERSEGUIDORES || laberinto[cara][x][y+1]==PERSEGUIDORES )
        {
            perdiste=1;
            laberinto[cara][x][y]=TUMBA;
            gotoxy(0,0);
            gotoxy(y+CORRIMIENTOX,x+CORRIMIENTOY);
            printf("%c",TUMBA);
            if(*score>20)
                *score=*score-20;

            Sleep(1000);
        }
        cambioCara=PasarCara(&cara,fila,colum,laberinto,&x,&y,puertasx,puertasy,ubicarJugadorX,ubicarJugadorY);
        if(cambioCara==1)
        {
            system("cls");
            mostrarLaberinto(cara,fila,colum,laberinto);
            gotoxy(0,0);
            gotoxy(y+CORRIMIENTOX,x+CORRIMIENTOY);
            color(10);
            printf("%c",JUGADOR);
            cambioCara=0;
            Beep(800,20);
        }
        gotoxy(0,0);
        gotoxy(10,40);
        color(10);
        printf("SCORE %d   ",*score);
        if(dificultad!='3')
        {
            gotoxy(0,0);
            gotoxy(10,39);
            color(14);
            printf("LLAVES RESTANTES: %d",contllaves);
        }
    }

    PlaySound(NULL, NULL, 0);
    system("cls");
    if(perdiste!=0)
    {
        PantallaResultado(GameOver,*score,perdiste);
    }
    else
    {
        PantallaResultado(Victory,*score,perdiste);
    }

    return perdiste;
}



int PasarCara(int *cara, int filas, int columnas, char laberinto[CARAS][filas][columnas],int *PJX, int *PJY, int PuertaX[], int PuertaY[],int UbicarJX[], int UbicarJY[])
{
//+0-PUERTA SUPERIOR
//+1-PUERTA INFERIOR
//+2-PUERTA IZQUIERDA
//+3-PUERTA DERECHA
    int cambioPuerta=0;
    int Cara=*cara;
    switch(Cara)
    {
    case 0:
        if(PuertaX[(4*Cara)]==*PJX && PuertaY[(4*Cara)]==*PJY  )
        {
            *cara=4;
            laberinto[*cara][UbicarJX[4*(*cara)]][UbicarJY[4*(*cara)]]=JUGADOR;
            laberinto[*cara][PuertaX[4*(*cara)]][PuertaY[4*(*cara)]]=VACIO;
            *PJX=UbicarJX[4*(*cara)];
            *PJY=UbicarJY[4*(*cara)];
        }
        if(PuertaX[(4*Cara)+1]==*PJX && PuertaY[(4*Cara)+1]==*PJY  )
        {
            *cara=1;
            laberinto[*cara][UbicarJX[(4*(*cara))+1]][UbicarJY[(4*(*cara))+1]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+1]][PuertaY[(4*(*cara))+1]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+1];
            *PJY=UbicarJY[(4*(*cara))+1];

        }
        if(PuertaX[(4*Cara)+2]==*PJX && PuertaY[(4*Cara)+2]==*PJY  )
        {
            *cara=2;
            laberinto[*cara][UbicarJX[(4*(*cara))+2]][UbicarJY[(4*(*cara))+2]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+2]][PuertaY[(4*(*cara))+2]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+2];
            *PJY=UbicarJY[(4*(*cara))+2];
        }

        if(PuertaX[(4*Cara)+3]==*PJX && PuertaY[(4*Cara)+3]==*PJY  )
        {
            *cara=3;
            laberinto[*cara][UbicarJX[(4*(*cara))+3]][UbicarJY[(4*(*cara))+3]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+3]][PuertaY[(4*(*cara))+3]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+3];
            *PJY=UbicarJY[(4*(*cara))+3];
        }
        break;
    case 1:
        if(PuertaX[(4*Cara)]==*PJX && PuertaY[(4*Cara)]==*PJY  )
        {
            *cara=0;
            laberinto[*cara][UbicarJX[4*(*cara)]][UbicarJY[4*(*cara)]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))]][PuertaY[(4*(*cara))]]=VACIO;
            *PJX=UbicarJX[4*(*cara)];
            *PJY=UbicarJY[4*(*cara)];
        }

        if(PuertaX[(4*Cara)+1]==*PJX && PuertaY[(4*Cara)+1]==*PJY  )
        {
            *cara=5;
            laberinto[*cara][UbicarJX[(4*(*cara))+1]][UbicarJY[(4*(*cara))+1]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+1]][PuertaY[(4*(*cara))+1]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+1];
            *PJY=UbicarJY[(4*(*cara))+1];
        }

        if(PuertaX[(4*Cara)+2]==*PJX && PuertaY[(4*Cara)+2]==*PJY  )
        {
            *cara=2;
            laberinto[*cara][UbicarJX[(4*(*cara))+2]][UbicarJY[(4*(*cara))+2]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+2]][PuertaY[(4*(*cara))+2]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+2];
            *PJY=UbicarJY[(4*(*cara))+2];
        }

        if(PuertaX[(4*Cara)+3]==*PJX && PuertaY[(4*Cara)+3]==*PJY  )
        {
            *cara=3;
            laberinto[*cara][UbicarJX[(4*(*cara))+3]][UbicarJY[(4*(*cara))+3]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+3]][PuertaY[(4*(*cara))+3]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+3];
            *PJY=UbicarJY[(4*(*cara))+3];
        }
        break;
    case 2:
        if(PuertaX[(4*Cara)]==*PJX && PuertaY[(4*Cara)]==*PJY  )
        {
            *cara=4;
            laberinto[*cara][UbicarJX[4*(*cara)]][UbicarJY[4*(*cara)]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))]][PuertaY[(4*(*cara))]]=VACIO;
            *PJX=UbicarJX[4*(*cara)];
            *PJY=UbicarJY[4*(*cara)];
        }

        if(PuertaX[(4*Cara)+1]==*PJX && PuertaY[(4*Cara)+1]==*PJY  )
        {
            *cara=1;
            laberinto[*cara][UbicarJX[(4*(*cara))+1]][UbicarJY[(4*(*cara))+1]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+1]][PuertaY[(4*(*cara))+1]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+1];
            *PJY=UbicarJY[(4*(*cara))+1];
        }

        if(PuertaX[(4*Cara)+2]==*PJX && PuertaY[(4*Cara)+2]==*PJY  )
        {
            *cara=5;
            laberinto[*cara][UbicarJX[(4*(*cara))+2]][UbicarJY[(4*(*cara))+2]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+2]][PuertaY[(4*(*cara))+2]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+2];
            *PJY=UbicarJY[(4*(*cara))+2];
        }

        if(PuertaX[(4*Cara)+3]==*PJX && PuertaY[(4*Cara)+3]==*PJY  )
        {
            *cara=0;
            laberinto[*cara][UbicarJX[(4*(*cara))+3]][UbicarJY[(4*(*cara))+3]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+3]][PuertaY[(4*(*cara))+3]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+3];
            *PJY=UbicarJY[(4*(*cara))+3];
        }
        break;
    case 3:
        if(PuertaX[(4*Cara)]==*PJX && PuertaY[(4*Cara)]==*PJY  )
        {
            *cara=4;
            laberinto[*cara][UbicarJX[4*(*cara)]][UbicarJY[4*(*cara)]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))]][PuertaY[(4*(*cara))]]=VACIO;
            *PJX=UbicarJX[4*(*cara)];
            *PJY=UbicarJY[4*(*cara)];
        }

        if(PuertaX[(4*Cara)+1]==*PJX && PuertaY[(4*Cara)+1]==*PJY  )
        {
            *cara=1;
            laberinto[*cara][UbicarJX[(4*(*cara))+1]][UbicarJY[(4*(*cara))+1]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+1]][PuertaY[(4*(*cara))+1]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+1];
            *PJY=UbicarJY[(4*(*cara))+1];
        }

        if(PuertaX[(4*Cara)+2]==*PJX && PuertaY[(4*Cara)+2]==*PJY  )
        {
            *cara=0;
            laberinto[*cara][UbicarJX[(4*(*cara))+2]][UbicarJY[(4*(*cara))+2]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+2]][PuertaY[(4*(*cara))+2]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+2];
            *PJY=UbicarJY[(4*(*cara))+2];
        }

        if(PuertaX[(4*Cara)+3]==*PJX && PuertaY[(4*Cara)+3]==*PJY  )
        {
            *cara=5;
            laberinto[*cara][UbicarJX[(4*(*cara))+3]][UbicarJY[(4*(*cara))+3]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+3]][PuertaY[(4*(*cara))+3]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+3];
            *PJY=UbicarJY[(4*(*cara))+3];
        }
        break;
    case 4:
        if(PuertaX[(4*Cara)]==*PJX && PuertaY[(4*Cara)]==*PJY  )
        {
            *cara=0;
            laberinto[*cara][UbicarJX[4*(*cara)]][UbicarJY[4*(*cara)]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))]][PuertaY[(4*(*cara))]]=VACIO;
            *PJX=UbicarJX[4*(*cara)];
            *PJY=UbicarJY[4*(*cara)];
        }

        if(PuertaX[(4*Cara)+1]==*PJX && PuertaY[(4*Cara)+1]==*PJY  )
        {
            *cara=5;
            laberinto[*cara][UbicarJX[(4*(*cara))+1]][UbicarJY[(4*(*cara))+1]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+1]][PuertaY[(4*(*cara))+1]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+1];
            *PJY=UbicarJY[(4*(*cara))+1];

        }

        if(PuertaX[(4*Cara)+2]==*PJX && PuertaY[(4*Cara)+2]==*PJY  )
        {
            *cara=3;
            laberinto[*cara][UbicarJX[(4*(*cara))+2]][UbicarJY[(4*(*cara))+2]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+2]][PuertaY[(4*(*cara))+2]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+2];
            *PJY=UbicarJY[(4*(*cara))+2];
        }

        if(PuertaX[(4*Cara)+3]==*PJX && PuertaY[(4*Cara)+3]==*PJY  )
        {
            *cara=2;
            laberinto[*cara][UbicarJX[(4*(*cara))+3]][UbicarJY[(4*(*cara))+3]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+3]][PuertaY[(4*(*cara))+3]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+3];
            *PJY=UbicarJY[(4*(*cara))+3];
        }
        break;
    case 5:
        if(PuertaX[(4*Cara)]==*PJX && PuertaY[(4*Cara)]==*PJY  )
        {
            *cara=4;
            laberinto[*cara][UbicarJX[4*(*cara)]][UbicarJY[4*(*cara)]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))]][PuertaY[(4*(*cara))]]=VACIO;
            *PJX=UbicarJX[4*(*cara)];
            *PJY=UbicarJY[4*(*cara)];
        }

        if(PuertaX[(4*Cara)+1]==*PJX && PuertaY[(4*Cara)+1]==*PJY  )
        {
            *cara=1;
            laberinto[*cara][UbicarJX[(4*(*cara))+1]][UbicarJY[(4*(*cara))+1]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+1]][PuertaY[(4*(*cara))+1]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+1];
            *PJY=UbicarJY[(4*(*cara))+1];

        }

        if(PuertaX[(4*Cara)+2]==*PJX && PuertaY[(4*Cara)+2]==*PJY  )
        {
            *cara=2;
            laberinto[*cara][UbicarJX[(4*(*cara))+2]][UbicarJY[(4*(*cara))+2]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+2]][PuertaY[(4*(*cara))+2]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+2];
            *PJY=UbicarJY[(4*(*cara))+2];
        }

        if(PuertaX[(4*Cara)+3]==*PJX && PuertaY[(4*Cara)+3]==*PJY  )
        {
            *cara=3;
            laberinto[*cara][UbicarJX[(4*(*cara))+3]][UbicarJY[(4*(*cara))+3]]=JUGADOR;
            laberinto[*cara][PuertaX[(4*(*cara))+3]][PuertaY[(4*(*cara))+3]]=VACIO;
            *PJX=UbicarJX[(4*(*cara))+3];
            *PJY=UbicarJY[(4*(*cara))+3];
        }
        break;
    }

    ///Limpia Las puertas de la Cara Anterior
    for(int i=0; i<4; i++)
    {
        laberinto[Cara][PuertaX[(4*Cara) +i]][PuertaY[(4*Cara) +i]]=VACIO;
    }

    if(Cara!=*cara)
        cambioPuerta=1;

    return cambioPuerta;
}


void AbrirSalidaFinal(int PuertasX[], int PuertasY[], int *SalidaX, int *SalidaY,int *caraRandom)
{
    srand(clock());
    *caraRandom=rand()%6;
    srand(clock());
    int puertaRandom=rand()%4;
    *SalidaX=PuertasX[(4*(*caraRandom))+puertaRandom];
    *SalidaY=PuertasY[(4*(*caraRandom))+puertaRandom];
/// Ubico la salida en el pasillo de la puerta correspondiente
    switch(puertaRandom)
    {
    case 0:
        *SalidaX=*SalidaX+3;
        break;
    case 1:
        *SalidaX=*SalidaX-3;
        break;
    case 2:
        *SalidaY=*SalidaY+3;
        break;
    case 3:
        *SalidaY=*SalidaY-3;
        break;
    }
}


void GenerarMONEDAS(int cara,int filas, int columnas,char laberinto[CARAS][filas][columnas])
{
    int k,j;
    for(int i=0; i<16; i++)
    {
        srand(clock());
        k=(rand()% (filas-2))+1;
        srand(clock());
        j=(rand()% (columnas-2))+1;
        if(laberinto[cara][k][j]!=PERSEGUIDORES && laberinto[cara][k][j]==VACIO )
            laberinto[cara][k][j]=MONEDA;
    }
}


float CalcularDistancia(int x0, int x1, int y0, int y1)
{
    float distancia=0;
    distancia=sqrt(pow(x1-x0,2)+pow(y1-y0,2));
    return distancia;
}

int BuscarMenor(float v[],int cantidad, int pos)
{
    float menor=v[pos];
    int posmenor=pos;

    for(int i=pos+1; i<cantidad; i++)
    {
        if(menor>v[i])
        {
            menor=v[i];
            posmenor=i;
        }
    }
    return posmenor;
}

int ElegirPerseguidorCercano(int PX[],int PY[],int cara,int PJX, int PJY,int cantidad)
{
    float Distancias[4]= {0};
    int cantD=0;
    int indiceMenor=0;
    for (int i=(4*cara); i<(4*cara)+cantidad; i++)
    {
        Distancias[cantD]=CalcularDistancia(PJX,PX[i],PJY,PY[i]);
        cantD++;
    }
    indiceMenor=BuscarMenor(Distancias,cantD,0);

    return indiceMenor;
}


int CantidadEnemigos(char dificultad,int* velocidad)
{

    int cantidadperseguidores;

    switch(dificultad)
    {

    case '2':
        cantidadperseguidores=3;
        *velocidad=500;
        break;

    case '3':
        cantidadperseguidores=4;
        *velocidad=400;
        break;

    default:
        cantidadperseguidores=2;
        *velocidad=700;
        break;

    }
    return cantidadperseguidores;
}




//REGISTRAR UN USUARIO NUEVO, NOMBRE Y CONTRASEÑA
int registroUsuarios(struct datosCubeMaze *dat)
{
    int cargado=0;
    char name[15];
    char contra1[15];
    char contra2[15];
    int valido=0;
    int caracteresCorrectos=0;//CONTADOR DE CARACTERES ALFANUM
    int y=0;
    int longNombre=0;//CANTIDAD DE CARACT DEL NOMBRE
    int longContra=0;//CANTIDAD DE CARACT DE LA CONTRASEÑA
    int u=0;
    int posic=0;


    //CARGAR NOMBRE
    while(valido==0 && dat->cantUsr<100)
    {
        system("cls");
        PantallaTitulo();
        printf("\n\tELIGE TU NOMBRE: (min. 4-max. 14 caracteres)\n\n\t");
        fflush(stdin);
        scanf("%s", name);
        longNombre=strlen(name);
        if(longNombre<15 && longNombre>3)
        {
            while(y<longNombre)//CHEQUEAR ALFANUM
            {
                if(isalnum(name[y])!=0)
                    caracteresCorrectos=caracteresCorrectos+1;
                y++;
            }
        }
        if(caracteresCorrectos==longNombre)
        {
            u=buscarUsuario(*dat, name, &posic);//CHEQUEAR QUE EL NOMBRE ESTE DISPONIBLE == 0
            if(u==1)
            {
                system("cls");
                PantallaTitulo();
                printf("\n\tYA EXISTE UN USUARIO CON ESE NOMBRE\n");
                Sleep(1000);
            }
            else//ESTA DISPONIBLE
                valido=1;
        }
        else
        {
            system("cls");
            PantallaTitulo();
            printf("\n\tUTILIZA CARACTERES ALFANUMERICOS (A-Z),(0-9)\n\n");
            y=0;
            caracteresCorrectos=0;
            Sleep(1000);
        }
    }

//CARGAR CONTRASEÑA
    y=0;
    int t=0;
    char c;
    caracteresCorrectos=0;
    while(valido==1)
    {
        system("cls");
        PantallaTitulo();
        printf("\n\tELIGE TU PASSWORD %s: (max. 14)\n\n\n\t", name);
        fflush(stdin);
        while((c=getch())!='\r')
        {
            contra1[t]=c;
            printf("*");
            t++;
        }
        contra1[t]='\0';
        t=0;
        longContra=strlen(contra1);
        while(y<longContra && valido==1)//CHEQUEAR ALFANUM
        {
            if(isalnum(contra1[y])!=0)
            {
                caracteresCorrectos=caracteresCorrectos+1;
                y++;
            }
            else
            {
                system("cls");
                PantallaTitulo();
                printf("\n\tUTILIZA CARACTERES ALFANUMERICOS (A-Z),(0-9)\n\n");
                Sleep(1000);
                y=0;
                caracteresCorrectos=0;
                valido=0;
            }
        }
        if(caracteresCorrectos==longContra)
        {
            system("cls");//SE PIDE CONFIRMAR PASSWORD Y SE COMPARA CON EL PRIMERO
            PantallaTitulo();
            printf("\n\tREESCRIBE TU PASSWORD:\n\n\n\t");
            fflush(stdin);
            while((c=getch())!='\r')
            {
                contra2[t]=c;
                printf("*");
                t++;
            }
            contra2[t]='\0';
        }
        t=0;

        if((strcmp(contra1,contra2)==0) && (caracteresCorrectos==longContra))
            valido=2;
        else //ESCRIBISTE MAL TU PASS, VOLVE A EMPEZAR
        {
            system("cls");
            PantallaTitulo();
            printf("\n\tLOS PASSWORDS NO COINCIDEN\n\n");
            Sleep(1000);
            valido=0;
        }


    }
    //TODO BIEN REGISTRAR USUARIO - INCIALIZAR DATOS PERSONALES
    if(valido==2)
    {
        strcpy(dat->usr[dat->cantUsr].nombre,name);
        strcpy(dat->usr[dat->cantUsr].password,contra1);
        dat->usr[dat->cantUsr].partidasGanadas=0;
        dat->usr[dat->cantUsr].partidasJugadas=0;
        dat->usr[dat->cantUsr].nivelNormal=0;
        dat->usr[dat->cantUsr].nivelDificil=0;
        for(int pos=0; pos<4 ; pos++)
        {
            dat->usr[dat->cantUsr].highscoresDificil[pos]=0;
            dat->usr[dat->cantUsr].highscoresInfinito[pos]=0;
            dat->usr[dat->cantUsr].highscoresNormal[pos]=0;
        }
        cargado=1;//USUARIO REGISTRADO
    }
    return cargado;
}


//INGRESAR USUARIO YA REGISTRADO, PUNTERO PARA DEVOLVER POSICION DEL USR
int ingresoUsuarios(struct datosCubeMaze datos, int*i)
{
    int ing=0;
    int j=0;//POSICION
    int k=0;//K==1 SI SE ENCONTRO AL USUARIO EN EL REGISTRO
    int compararContra=0;
    char nom[15];
    char contra[15];
    char adm[]="admin";
    char c;
    int t=0;
    system("cls");
    PantallaTitulo();
    printf("\n\tINGRESA TU NOMBRE DE USUARIO:\n\n\n\t");
    fflush(stdin);
    scanf("%s",nom);
    //BUSCAR USUARIO
    k=buscarUsuario(datos,nom,&j);
    //USUARIO NO ENCONTRADO
    if(k==0)
    {
        system("cls");
        PantallaTitulo();
        printf("\n\tUSUARIO NO ENCONTRADO\n\n");
        Sleep(1000);
    }
    //USUARIO ENCONTRADO, COMPARAR CONTRASEÑA
    else
    {
        system("cls");
        PantallaTitulo();
        printf("\n\tINGRESA TU PASSWORD:\n\n\n\t");
        fflush(stdin);
        while((c=getch())!='\r')
        {
            contra[t]=c;
            printf("*");
            t++;
        }
        contra[t]='\0';
        compararContra=strcmp(contra,datos.usr[j].password);
        t=0;
        //SI LA CONTRASEÑA ES CORRECTA:
        if(compararContra==0)
        {
            //ING==1 --> INGRESO USUARIO PEDORRO
            if(strcmp(datos.usr[j].nombre,adm)!=0)
            {
                system("cls");
                ing=1;
                PantallaTitulo();
                printf("\n\tINGRESANDO AL SISTEMA");
                Sleep(1000);
            }
            //ING==2 --> INGRESO ADMIN
            if(strcmp(datos.usr[j].nombre,adm)==0)
            {
                system("cls");
                ing=2;
                PantallaTitulo();
                printf("\n\tINGRESANDO AL SISTEMA");
                Sleep(1000);
            }
        }
        else //CONTRASEÑA INCORRECTA
        {
            system("cls");
            ing=0;
            PantallaTitulo();
            printf("\n\tPASSWORD INCORRECTO");
            Sleep(1000);
        }
        *i=j;
    }
    return ing;
}


//BUSCAR UN USUARIO, RETORNA 1 SI EXISTE, 0 SI NO EXISTE
//SE BUSCA EN TODO EL ARREGLO, EL PUNTERO POSICION ES PARA DEVOLVER DONDE SE ENCONTRO
int buscarUsuario(struct datosCubeMaze datos, char nombre[15], int *posicion)
{
    int compararNom;
    int i=0;
    int aux=0;
    //BUSCAR USUARIO POR NOMBRE
    while((aux<datos.cantUsr)&&(compararNom!=0))
    {
        compararNom=strcmp(nombre,datos.usr[aux].nombre);
        aux++;;
    }
    aux=aux-1;//CORREGIR POSICION

    if(compararNom==0)
    {
        i=1;//ENCONTRADO, DEVOLVER POSICION
        *posicion=aux;
    }
    return i;
}

//CAMBIAR NOMBRE DE USUARIO
int cambiarNombre(struct datosCubeMaze *dat, int posicion)
{
    int b=0;
    int longNombre=0;
    char name[15];
    int caracteresCorrectos=0;//ALFANUM
    int *pos=&posicion;
    int y=0;
    int existe=0;
    system("cls");
    PantallaTitulo();
    printf("\n\tINGRESE NUEVO NOMBRE: (max. 14)\n\n\n\t");
    fflush(stdin);
    scanf("%s", name);
    existe=buscarUsuario(*dat,name, pos);
    if(existe==1)
    {
        PantallaTitulo();
        printf("\tEL NOMBRE YA EXISTE\n");
        Sleep(1000);
    }
    else
    {
        longNombre=strlen(name);
        if(longNombre<15)
        {
            while(y<longNombre)//CHEQUEAR ALFANUM
            {
                if(isalnum(name[y])!=0)
                    caracteresCorrectos=caracteresCorrectos+1;
                y++;
            }
        }
        if(caracteresCorrectos==longNombre)
        {
            strcpy(dat->usr[*pos].nombre,name);
            system("cls");
            PantallaTitulo();
            printf("\n\tNOMBRE ACTUALIZADO\n\n");
            b=1;
            Sleep(1000);
        }
        else
        {
            system("cls");
            PantallaTitulo();
            printf("\n\tUTILIZA CARACTERES ALFANUMERICOS (A-Z),(0-9)\n\n");
            y=0;
            caracteresCorrectos=0;
            Sleep(1000);
        }
    }
    return b;
}


//CAMBIAR PASSWORD DE USUARIO
int cambiarPassword(struct datosCubeMaze *dat,int i)
{
    int b=0;
    int longContra=0;//CANTIDAD DE CHAR EN EL PASSWORD
    char pass[15];
    int caracteresCorrectos=0;//CONTADOR DE CARACTERES ACEPTADOS
    int y=0;
    int t=0;
    char c;
    system("cls");
    PantallaTitulo();
    printf("\n\tINGRESE NUEVO PASSWORD: (max. 14)\n\n\n\t");
    fflush(stdin);
    while((c=getch())!='\r')
    {
        pass[t]=c;
        printf("*");
        t++;
    }
    pass[t]='\0';
    t=0;
    longContra=strlen(pass);
    if(longContra<15)
    {
        while(y<longContra)//CHEQUEAR ALFANUM
        {
            if(isalnum(pass[y])!=0)
                caracteresCorrectos=caracteresCorrectos+1;
            y++;
        }
    }
    if(caracteresCorrectos==longContra)
    {
        strcpy(dat->usr[i].password,pass);
        system("cls");
        PantallaTitulo();
        printf("\n\tPASSWORD ACTUALIZADO\n\n");
        b=1;
        Sleep(1000);
    }
    else
    {
        system("cls");
        PantallaTitulo();
        printf("\n\tUTILIZA CARACTERES ALFANUMERICOS (A-Z),(0-9)\n\n");
        y=0;
        caracteresCorrectos=0;
        Sleep(1000);
    }
    return b;
}

//ELIMINAR USUARIO
int borrarUsuario(struct datosCubeMaze *dat, int posicionUsr)
{
    int b=0;
    char control='n';
    system("cls");
    PantallaTitulo();
    printf("\n\tDESEA ELIMINAR AL USUARIO %s? s/n\n\n\t", dat->usr[posicionUsr].nombre);
    fflush(stdin);
    scanf("%c", &control);
    if(control=='s')
    {
        //ACOMODAR ARREGLO DE USUARIOS TAPANDO AL USUARIO ELIMINADO
        while(posicionUsr<(dat->cantUsr-1))
        {
            dat->usr[posicionUsr]=dat->usr[posicionUsr+1];
            posicionUsr++;
        }
        dat->cantUsr--;
        system("cls");
        PantallaTitulo();
        printf("\n\tUSUARIO ELIMINADO\n\n");
        b=1;
        Sleep(1000);
    }
    return b;
}

//MOSTRAR PERFIL Y OPCIONES DATOS PERSONALES
int verPerfil(struct datosCubeMaze *dat, int *y)
{
    int a=0;
    int b=0;//PARA DEVOLVER SI HUBO CAMBIOS EN LOS DATOS PERSONALES
    int c=0;
    char opcion;
    float porcentajeVictorias=0;
    if(dat->usr[*y].partidasJugadas!=0)
    {
        porcentajeVictorias=(((dat->usr[*y].partidasGanadas)*100)/(dat->usr[*y].partidasJugadas));
    }
    system("cls");
    PantallaTitulo();
    printf("\t");
    color(46);
    printf(" PERFIL %s ", dat->usr[*y].nombre);
    color(2);
    printf("\n\n\tPARTIDAS JUGADAS: %d", dat->usr[*y].partidasJugadas);
    printf("\n\n\tPARTIDAS GANADAS: %d", dat->usr[*y].partidasGanadas);
    printf("\n\n\tPORCENTAJE DE VICTORIAS: %.0f %c", porcentajeVictorias,37);
    color(12);
    printf("\n\n\n\tHIGHSCORES - NIVEL NORMAL\n");
    color(2);
    printf("\n\t# 1 - %d PUNTOS - %s", dat->usr[*y].highscoresNormal[0],dat->usr[*y].fechayhoraNormal[0]);
    printf("\n\t# 2 - %d PUNTOS - %s", dat->usr[*y].highscoresNormal[1],dat->usr[*y].fechayhoraNormal[1]);
    printf("\n\t# 3 - %d PUNTOS - %s", dat->usr[*y].highscoresNormal[2],dat->usr[*y].fechayhoraNormal[2]);
    color(12);
    printf("\n\n\n\tHIGHSCORES - NIVEL DIFICIL\n");
    color(2);
    printf("\n\t# 1 - %d PUNTOS - %s", dat->usr[*y].highscoresDificil[0],dat->usr[*y].fechayhoraDificil[0]);
    printf("\n\t# 2 - %d PUNTOS - %s", dat->usr[*y].highscoresDificil[1],dat->usr[*y].fechayhoraDificil[1]);
    printf("\n\t# 3 - %d PUNTOS - %s", dat->usr[*y].highscoresDificil[2],dat->usr[*y].fechayhoraDificil[2]);
    color(12);
    printf("\n\n\n\tHIGHSCORES - NIVEL INFINITO\n");
    color(2);
    printf("\n\t# 1 - %d PUNTOS - %s", dat->usr[*y].highscoresInfinito[0],dat->usr[*y].fechayhoraInfinito[0]);
    printf("\n\t# 2 - %d PUNTOS - %s", dat->usr[*y].highscoresInfinito[1],dat->usr[*y].fechayhoraInfinito[1]);
    printf("\n\t# 3 - %d PUNTOS - %s", dat->usr[*y].highscoresInfinito[2],dat->usr[*y].fechayhoraInfinito[2]);
    color(12);
    printf("\n\n\t\t\t\t\t\tOPCIONES\n\n");
    color(10);
    printf("\t\t\t\t\t# 1    CAMBIAR NOMBRE\n");
    printf("\t\t\t\t\t# 2    CAMBIAR PASSWORD\n");
    printf("\t\t\t\t\t# 3    VOLVER AL MENU ANTERIOR\n\t");
    fflush(stdin);
    scanf("%c", &opcion);//CHEQUEAR QUE INGRESE UN NUMERO VALIDO.... SE INGRESA COMO CHAR
    system("cls");
    if((isdigit(opcion))==0)
    {
        color(10);
        PantallaTitulo();
        printf("\n\n\n\n\tINGRESA UNA OPCION VALIDA\n\n");
        Sleep(1000);
        system("cls");
        PantallaTitulo();
        color(12);
        printf("\tOPCIONES\n\n");
        color(10);
        printf("\t# 1    CAMBIAR NOMBRE\n");
        printf("\t# 2    CAMBIAR PASSWORD\n");
        printf("\t# 3    VOLVER AL MENU\n\n\n\t");
        fflush(stdin);
        scanf("%c", &opcion);
        color(8);
    }
    else
    {
        switch(opcion)
        {
        //SE VUELVE SIEMPRE AL MENU ANTERIOR POR SI CAMBIO EL NOMBRE/PASS Y DESPUES LO QUISO CAMBIAR
        //UNA SEGUNDA VEZ PERO NO LO CAMBIO ESTARIAMOS RECIBIENDO QUE NO HUBO CAMBIOS PERO SI HUBO
        case '1':
            a=cambiarNombre(dat, *y);
            opcion=3;
            break;
        case '2':
            c=cambiarPassword(dat,*y);
            opcion=3;
            break;
        case '3':
            opcion=3;
            break;
        default:
            break;
        }
    }//SI EL USUARIO CAMBIO NOMBRE O PASSWORD DEVOLVEMOS QUE SE CAMBIO UN DATO==ACTUALIZAR ARCHIVO
    if(a==1 || c==1)
        b=1;

    return b;
}


//MENU ESPECIAL PARA USER ADMIN
void menuAdmin(struct datosCubeMaze *dat, int *logg)
{
    char opcion;
    char opcionDos;
    int posicionUsr=0;
    char buscado[15];
    int aux=0;
    int v=0;
    PantallaTitulo();
    color(10);
    printf("\t# MENU ADMIN\n\n\n\n");
    printf("\t# 1    EDITAR USUARIO\n\n");
    printf("\t# 2    VER LISTA DE USUARIOS\n\n");
    printf("\t# 3    LOGOUT\n\n\n\t");
    fflush(stdin);
    scanf("%c", &opcion);
    system("cls");
    if((isdigit(opcion))==0)
    {
        color(10);
        PantallaTitulo();
        printf("\n\n\n\n\tINGRESA UNA OPCION VALIDA\n\n");
        Sleep(1000);
        system("cls");
    }
    else
    {
        switch(opcion)
        {
        case '1':
            system("cls");
            color(10);
            PantallaTitulo();
            printf("# MENU ADMIN\n\n");
            printf("\tINGRESE NOMBRE DEL USUARIO A EDITAR: \n\n\n\t");
            fflush(stdin);
            scanf("%s", buscado);
            aux=buscarUsuario(*dat,buscado, &posicionUsr);
            system("cls");
            if(aux==1)//SI EXISTE EL USUARIO
            {
                if(posicionUsr==0) //NO SE PUEDE EDITAR AL ADMIN
                {
                    PantallaTitulo();
                    printf("\t# MENU ADMIN\n\n\n\n");
                    printf("\tNO SE PUEDE EDITAR EL ADMIN\n\n");
                    Sleep(1000);
                }
                else// MOSTRAR SUBMENU
                {
                    PantallaTitulo();
                    printf("\t# MENU ADMIN\n\n\n\n");
                    printf("\n\tUSUARIO %s\n\n",buscado);
                    printf("\t# 1    CAMBIAR NOMBRE USUARIO\n\n");
                    printf("\t# 2    CAMBIAR PASSWORD USUARIO\n\n");
                    printf("\t# 3    BORRAR USUARIO\n\n\n\n");
                    color(12);
                    printf("\tINFORME AL USUARIO DE LOS CAMBIOS \n\tREALIZADOS EN SU CUENTA\n\n\n\t");
                    color(10);
                    fflush(stdin);
                    scanf("%c", &opcionDos);
                    system("cls");
                    if((isdigit(opcionDos))==0)
                    {
                        PantallaTitulo();
                        printf("\n\n\n\n\tINGRESA UNA OPCION VALIDA\n\n");
                        Sleep(1000);
                        system("cls");
                    }
                    else//QUE QUERES HACER CON ESE USUARIO
                    {
                        switch(opcionDos)
                        {
                        case '1':
                            cambiarNombre(dat, posicionUsr);
                            break;
                        case '2':
                            cambiarPassword(dat, posicionUsr);
                            break;
                        case '3':
                            borrarUsuario(dat, posicionUsr);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            else//EL USUARIO BUSCADO NO EXISTE
            {
                system("cls");
                color(10);
                PantallaTitulo();
                printf("\t# MENU ADMIN\n\n\n\n");
                printf("\n\tUSUARIO NO ENCONTRADO\n\n");
                Sleep(1000);
                opcion=0;
            }
            break;
        case '2'://VER LISTA DE USUARIOS REGISTRADOS
            system("cls");
            color(10);
            PantallaTitulo();
            printf("\t# MENU ADMIN\n\n\n\n");
            printf("\tCANTIDAD DE USUARIOS: %d\n\n\n", dat->cantUsr);
            while (v<dat->cantUsr)
            {
                printf("\t# %d - %s\n", v,dat->usr[v].nombre);
                v++;
                if(((v%20)==0) && (v>0))
                    getch();
            }
            getch();
            break;
        case '3'://ADMIN LOGOUT
            *logg=0;
            system("cls");
            break;
        default:
            break;
        }
    }
    color(8);
}


//FUNCION QUE ORDENA SCORES PERSONALES Y GENERALES A LA VEZ
void ordenarScores(struct datosCubeMaze *dat, int i, int score, char dificultad)
{
    //ARREGLO USR EN POSICION CERO ES EL ADMIN ------> EL ADMIN NO PUEDE JUGAR, QUE TRABAJE

    int b=3;//AUX QUE RECORRE ARREGLO SCORES USUARIO
    int c=5;//AUX QUE RECORRE ARREGLO SCORES GENERALES
    int aux=0;//AUX PARA COPIAR DATOS AL MOVER POSICIONES
    char auxNombre[15];
    char auxFecha[100];
    time_t t; // VARIABLES PARA REGISTRAR FECHA Y HORA
    struct tm *tm;
    t=time(NULL);
    tm=localtime(&t);

    if(dificultad=='1') //DIFICULTAD NORMAL
    {
        //ORDENAMIENTO EN CUENTA USR
        dat->usr[i].highscoresNormal[b]=score;//SE COPIA SCORE OBTENIDO A ULT POSICION DEL ARREGLO,
        /////////////////////////////////////////LUEGO SE ORDENA (ULT POSICION NO SE MUESTRA)
        strftime(dat->usr[i].fechayhoraNormal[b], 100, "%d/%m/%Y  %Hh:%Mm", tm);

        while(b>0)
        {
            if((dat->usr[i].highscoresNormal[b])>(dat->usr[i].highscoresNormal[b-1]))
            {
                aux=dat->usr[i].highscoresNormal[b-1];//SCORE
                dat->usr[i].highscoresNormal[b-1]=dat->usr[i].highscoresNormal[b];
                dat->usr[i].highscoresNormal[b]=aux;
                strcpy(auxFecha,dat->usr[i].fechayhoraNormal[b-1]);//FECHA
                strcpy(dat->usr[i].fechayhoraNormal[b-1],dat->usr[i].fechayhoraNormal[b]);
                strcpy(dat->usr[i].fechayhoraNormal[b],auxFecha);
            }
            b--;
        }


        //ORDENAMIENTO EN TABLA GENERAL DE TODOS LOS USUARIOS, PUNTAJE, FECHA Y NOMBRES
        dat->highscoresNormal[c]=score;
        strcpy(dat->highscoresNormalNombres[c],dat->usr[i].nombre);
        strftime(dat->fechayhoraNormalTOP[c], 100, "%d/%m/%Y  %Hh:%Mm", tm);
        while(c>0)
        {
            if((dat->highscoresNormal[c])>(dat->highscoresNormal[c-1]))
            {
                aux=dat->highscoresNormal[c-1];                             //SCORE
                dat->highscoresNormal[c-1]=dat->highscoresNormal[c];
                dat->highscoresNormal[c]=aux;
                strcpy(auxNombre,dat->highscoresNormalNombres[c-1]);           //NOMBRE
                strcpy(dat->highscoresNormalNombres[c-1],dat->highscoresNormalNombres[c]);
                strcpy(dat->highscoresNormalNombres[c],auxNombre);
                strcpy(auxFecha,dat->fechayhoraNormalTOP[c-1]);                 //FECHA
                strcpy(dat->fechayhoraNormalTOP[c-1],dat->fechayhoraNormalTOP[c]);
                strcpy(dat->fechayhoraNormalTOP[c],auxFecha);
            }
            c--;
        }

    }
    if(dificultad=='2') //DIFICULTAD DIFICIL
    {
        //ORDENAMIENTO EN CUENTA USR
        dat->usr[i].highscoresDificil[b]=score;
        strftime(dat->usr[i].fechayhoraDificil[b], 100, "%d/%m/%Y  %Hh:%Mm", tm);
        while(b>0)
        {
            if(dat->usr[i].highscoresDificil[b]>dat->usr[i].highscoresDificil[b-1])
            {
                aux=dat->usr[i].highscoresDificil[b-1];//SCORE
                dat->usr[i].highscoresDificil[b-1]=dat->usr[i].highscoresDificil[b];
                dat->usr[i].highscoresDificil[b]=aux;
                strcpy(auxFecha,dat->usr[i].fechayhoraDificil[b-1]);//FECHA
                strcpy(dat->usr[i].fechayhoraDificil[b-1],dat->usr[i].fechayhoraDificil[b]);
                strcpy(dat->usr[i].fechayhoraDificil[b],auxFecha);
            }
            b--;
        }

        //ORDENAMIENTO EN TABLA GENERAL DE TODOS LOS USUARIOS
        dat->highscoresDificil[c]=score;
        strcpy(dat->highscoresDificilNombres[c],dat->usr[i].nombre);
        strftime(dat->fechayhoraDificilTOP[c], 100, "%d/%m/%Y  %Hh:%Mm", tm);
        while(c>0)
        {
            if((dat->highscoresDificil[c])>(dat->highscoresDificil[c-1]))
            {
                aux=dat->highscoresDificil[c-1];                    //SCORE
                dat->highscoresDificil[c-1]=dat->highscoresDificil[c];
                dat->highscoresDificil[c]=aux;
                strcpy(auxNombre,dat->highscoresDificilNombres[c-1]);   //NOMBRE
                strcpy(dat->highscoresDificilNombres[c-1],dat->highscoresDificilNombres[c]);
                strcpy(dat->highscoresDificilNombres[c],auxNombre);
                strcpy(auxFecha,dat->fechayhoraDificilTOP[c-1]);        //FECHA
                strcpy(dat->fechayhoraDificilTOP[c-1],dat->fechayhoraDificilTOP[c]);
                strcpy(dat->fechayhoraDificilTOP[c],auxFecha);
            }
            c--;
        }
    }
    if(dificultad=='3') //DIFICULTAD INFINITO
    {
        //ORDENAMIENTO EN CUENTA USR
        dat->usr[i].highscoresInfinito[b]=score;
        strftime(dat->usr[i].fechayhoraInfinito[b], 100, "%d/%m/%Y  %Hh:%Mm", tm);
        while(b>0)
        {
            if(dat->usr[i].highscoresInfinito[b]>dat->usr[i].highscoresInfinito[b-1])
            {
                aux=dat->usr[i].highscoresInfinito[b-1];            //SCORE
                dat->usr[i].highscoresInfinito[b-1]=dat->usr[i].highscoresInfinito[b];
                dat->usr[i].highscoresInfinito[b]=aux;
                strcpy(auxFecha,dat->usr[i].fechayhoraInfinito[b-1]);       //FECHA
                strcpy(dat->usr[i].fechayhoraInfinito[b-1],dat->usr[i].fechayhoraInfinito[b]);
                strcpy(dat->usr[i].fechayhoraInfinito[b],auxFecha);
            }
            b--;
        }

        //ORDENAMIENTO EN TABLA GENERAL DE TODOS LOS USUARIOS
        dat->highscoresInfinito[c]=score;
        strcpy(dat->highscoresInfinitoNombres[c],dat->usr[i].nombre);
        strftime(dat->fechayhoraInfinitoTOP[c], 100, "%d/%m/%Y  %Hh:%Mm", tm);
        while(c>0)
        {
            if((dat->highscoresInfinito[c])>(dat->highscoresInfinito[c-1]))
            {
                aux=dat->highscoresInfinito[c-1];                       //SCORE
                dat->highscoresInfinito[c-1]=dat->highscoresInfinito[c];
                dat->highscoresInfinito[c]=aux;
                strcpy(auxNombre,dat->highscoresInfinitoNombres[c-1]);      //NOMBRE
                strcpy(dat->highscoresInfinitoNombres[c-1],dat->highscoresInfinitoNombres[c]);
                strcpy(dat->highscoresInfinitoNombres[c],auxNombre);
                strcpy(auxFecha,dat->fechayhoraInfinitoTOP[c-1]);           //FECHA
                strcpy(dat->fechayhoraInfinitoTOP[c-1],dat->fechayhoraInfinitoTOP[c]);
                strcpy(dat->fechayhoraInfinitoTOP[c],auxFecha);
            }
            c--;
        }
    }
}



//IMPRIMIR EN MENU PRINCIPAL LOS HIGHSCORES
void highscores (struct datosCubeMaze datos)
{
    int i=0;
    char h[]="HIGHSCORES";
    char normal[]="NIVEL NORMAL";
    char dificil[]="NIVEL DIFICIL";
    char infinito[]="NIVEL INFINITO";
    int g=0;
    system("cls");
    PantallaTitulo();
    printf("\t");

    while(g<10)
    {
        if(g%2==0)
            color(12);
        else
            color(10);
        printf("%c  ",h[g]);
        g++;
        Sleep(500);
    }
    g=0;
    printf("\n\n\n\n\t");
    color(10);
    while(g<12)
    {
        printf("%c",normal[g]);
        g++;
        Sleep(150);
    }
    printf("\n\n");
    Sleep(500);
    while(i<5)
    {
        color(12);
        printf("\t# %d -",(i+1));
        color(2);
        if(datos.highscoresNormal[i]==0)
            printf(" - - - - -\n");
        else
        {
            printf(" %s\t%d PUNTOS\t", datos.fechayhoraNormalTOP[i],datos.highscoresNormal[i]);
            color(12);
            printf("%s\n",datos.highscoresNormalNombres[i]);
        }
        i++;
        Sleep(200);
    }
    i=0;
    g=0;
    printf("\n\n\t");
    color(10);
    while(g<13)
    {
        printf("%c",dificil[g]);
        g++;
        Sleep(150);
    }
    printf("\n\n");
    Sleep(500);
    while(i<5)
    {
        color(12);
        printf("\t# %d -",(i+1));
        color(2);
        if(datos.highscoresDificil[i]==0)
            printf(" - - - - -\n");
        else
        {
            printf(" %s\t%d PUNTOS\t",datos.fechayhoraDificilTOP[i], datos.highscoresDificil[i]);
            color(12);
            printf("%s\n",datos.highscoresDificilNombres[i]);
        }
        i++;
        Sleep(200);
    }
    i=0;
    g=0;
    color(10);
    printf("\n\n\t");
    while(g<14)
    {
        printf("%c",infinito[g]);
        g++;
        Sleep(150);
    }
    printf("\n\n");
    Sleep(500);
    while(i<5)
    {
        color(12);
        printf("\t# %d -",(i+1));
        color(2);
        if(datos.highscoresInfinito[i]==0)
            printf(" - - - - -\n");
        else
        {
            printf(" %s\t%d PUNTOS\t",datos.fechayhoraInfinitoTOP[i],datos.highscoresInfinito[i]);
            color(12);
            printf("%s\n",datos.highscoresInfinitoNombres[i]);
        }
        i++;
        Sleep(200);
    }
    getch();
}


void ayuda()
{
    color(10);
    PantallaTitulo();
    color(12);
    printf("\t-----------------------------------------------------------------\n\n\t\t*");
    color(10);
    printf(" RECORRE EL LABERINTO MIENTRAS\n\t\t\t   ESCAPAS DE TUS PERSEGUIDORES ");
    color(12);
    printf("%c\n\n\n\t\t*",PERSEGUIDORES);
    color(10);
    printf(" RECOLECTA TODAS LAS LLAVES ");
    color(14);
    printf("%c",LLAVES);
    color(10);
    printf(" PARA DESBLOQUEAR \n\t\t\t  LA SALIDA FINAL DEL CUBO\n\n\n");
    color(12);
    printf("\t\t*");
    color(10);
    printf(" SUPERA LOS NIVELES NORMAL Y DIFICIL\n\t\t\t   PARA DESBLOQUEAR EL MODO INFINITO\n\n\n");
    color(12);
    printf("\t-----------------------------------------------------------------\n\n");
    color(10);
    printf("\t\t      SUPPORT: estemailnoexiste@mail.com\n\n");
    color(12);
    printf("\t-----------------------------------------------------------------\n\n");
    printf("\t\t\t      ");
    color(74);
    printf(" C U B I C M A Z E ");
    color(10);
    printf("\n\n\n\t\t\t\tFUE CREADO POR: \n\n\n\t\t MAXIMILIANO COMMISSO\t      JORGE PIAGGIO\n\n\n\n\t\t  LABORATORIO I - PROFESOR GUSTAVO SONVICO\n");
    printf("\n\n\t\t TECNICATURA UNIVERSITARIA EN PROGRAMACION\n\n\n\t\t\t  UTN - MAR DEL PLATA - 2019\n\n");
    color(12);
    printf("\t-----------------------------------------------------------------\n\t");
    color(10);
    getch();
    color(8);
}




//ESTO SE VA A EJECUTAR SOLO LA PRIMERA VEZ QUE CORRA EL PROGRAMA
//O SI SE BORRA EL ARCHIVO DATOSCUBEMAZE.BIN
void inicializarDatosPrimeraVez(struct datosCubeMaze *dat)
{
    //CARGAR ADMIN EN POSICION CERO
    if(dat->cantUsr==0)
    {
        char nom[15]="admin";
        strcpy(dat->usr[0].nombre,nom);
        strcpy(dat->usr[0].password,nom);
        dat->cantUsr=1;//CONTADOR DE USUARIOS CARGADOS
    }
//    INICIALIZAR EN CERO LOS HIGHSCORES GENERALES
    if(datos.cantUsr==1)
    {
        for(int z=0; z<6; z++)
        {
            dat->highscoresNormal[z]=0;
            dat->highscoresDificil[z]=0;
            dat->highscoresInfinito[z]=0;
        }
    }
}



//FUNCION QUE SE LLAMA CADA VEZ QUE SE ACTUALIZA ALGUN DATO EN EL TRANSCURSO DEL PROGRAMA
//NO QUEREMOS QUE LA COMPU SE CUELGUE CON SEMEJANTE JUEGO Y SE PIERDAN DATOS
void abrirParchEscribirDatosCerrarParch (struct datosCubeMaze *dat, FILE *parch,int *armaggedon)
{
    char archivo[]="datosCubeMaze.bin";
    int control=0;

    if((parch=fopen(archivo, "wb"))==NULL)
    {
        printf("NO SE PUDO ABRIR EL ARCHIVO, RAJEMOS\n");
        *armaggedon=1;
    }
    else
    {
        //SOBREESCRIBIR EN ARCHIVO LOS DATOS DE MEMORIA
        fwrite(dat,sizeof(datos),1,parch);
    }
    control=fclose(parch);
    if(control!=0)
        printf("ERROR CERRANDO EL ARCHIVO DE DATOS\n");
}



void demoLab2()
{
    int puertasx[24];
    int puertasy[24];
    int ubicarJX[24];
    int ubicarJY[24];
    char demoLab[1][fila][colum];
    int x=0;
    int y=0;
    int contadorDePosiblesPasillos=0;
    int iActual=0,jActual=0;
    int ianterior=0;
    int janterior=0;
    int colaX[1000];
    int colaY[1000];
    int ColaXO[1000];
    int ColaYO[1000];
    int contarEfes=0;
    int enemyX[2];
    int enemyY[2];

    llenarMatriz(0,fila, colum, demoLab);
    mostrarDemoLab(0,fila,colum, demoLab);
    color(10);
    printf("\n\n\tPRESIONE UNA TECLA\n");
    getch();
    system("cls");
    llenarDeO(0,fila, colum, demoLab, &contadorDePosiblesPasillos,ColaXO,ColaYO);
    mostrarDemoLab(0,fila,colum, demoLab);
    color(10);
    printf("\n\n\tPRESIONE UNA TECLA\n");
    getch();
    system("cls");
    ElegiryEliminarVecino(0,fila,colum,demoLab,ColaXO,ColaYO,&contadorDePosiblesPasillos,&iActual, &jActual);
    enviarVecinos(0,fila,colum, demoLab, iActual, jActual, colaX, colaY, &contarEfes, &contadorDePosiblesPasillos,ColaXO,ColaYO);
    int i=0;
    while(contadorDePosiblesPasillos>0 || contarEfes>0)
    {
        if(contarEfes>0)
        {
            ElegiryEliminarVecino(0,fila,colum,demoLab,colaX,colaY,&contarEfes,&iActual, &jActual);
            generarPuente(0,fila, colum, demoLab, iActual, jActual,ianterior,janterior);
            if(i==1 || i==0)
            {
                mostrarDemoLab(0,fila,colum, demoLab);
                color(10);
                printf("\n\n\tPRESIONE UNA TECLA\n");
                getch();
                system("cls");
            }
            if(i%7==0)
            {
                mostrarDemoLab(0,fila,colum, demoLab);
                Sleep(300);
                system("cls");
            }
            i++;
        }
        else
        {
            ElegiryEliminarVecino(0,fila,colum,demoLab,ColaXO,ColaYO,&contadorDePosiblesPasillos,&iActual, &jActual);
        }
        enviarVecinos(0,fila,colum, demoLab, iActual, jActual, colaX, colaY, &contarEfes, &contadorDePosiblesPasillos,ColaXO,ColaYO);
        ianterior=iActual;
        janterior=jActual;
    }
    abrirPuertas(0,fila,colum,demoLab,puertasx,puertasy,ubicarJX,ubicarJY);
    mostrarDemoLab(0,fila,colum, demoLab);
    Sleep(1000);
    system("cls");
    ubicarPerseguidor(0,fila,colum,demoLab,enemyX,enemyY,'1');
    mostrarDemoLab(0,fila,colum, demoLab);
    Sleep(1000);
    system("cls");
    ubicarLlaves(0,fila, colum, demoLab,'1');
    mostrarDemoLab(0,fila,colum, demoLab);
    Sleep(1000);
    system("cls");
    UbicarJugadorInicio(fila,colum,demoLab,&x,&y);
    mostrarDemoLab(0,fila,colum, demoLab);
    color(10);
    printf("\n\tPRESIONE UNA TECLA\n");
    getch();
}

void mostrarDemoLab(int cara,int fil, int col, char m[CARAS][fil][col])
{
    PantallaTitulo();
    printf("\t");
    color(71);
    printf("DEMOLAB\n\n");
    color(10);
    printf("\tGENERANDO LABERINTO... ESPERE POR FAVOR\n\n\n");
    color(8);
    for(int i=0; i<fil; i++)
    {
        printf("\t");
        for(int j=0; j<col; j++)
        {
            switch(m[cara][i][j])
            {
            case JUGADOR:
                color(10);
                printf("%c", m[cara][i][j]);
                break;
            case LLAVES: //Pinta llaves
                color(14);
                printf("%c", m[cara][i][j]);
                break;
            case PERSEGUIDORES:// Pinta perseguidores
                color(12);
                printf("%c", m[cara][i][j]);
                break;
            case MUROS:// Pinta paredes
                if(i==0 || j==0 ||i==fil-1||j==col-1)
                {
                    color(9);
                }
                else
                {
                    color(cara+1);
                }
                printf("%c", m[cara][i][j]);
                break;
            case MARCOS:// Pinta paredes de puertas
                color(15);
                printf("%c", m[cara][i][j]);
                break;
            case SALIDA:// Pinta Salida final
                color(14);
                printf("%c", m[cara][i][j]);
                break;
            default:
                if(m[cara][i][j]=='F')
                    color(12);
                if(m[cara][i][j]=='O')
                    color(2);
                printf("%c",m[cara][i][j]);
                break;
            }
        }
        printf("\n");
    }
}

void PantallaResultado(char Resultado[],int score, int perdiste )
{
    char aux;
    PlaySound(TEXT(Resultado),NULL,SND_ASYNC);
    char archi[]="gameoverimagen.txt";
    char archi2[]="ganaste.txt";
    FILE * archivo;
    hidecursor(0);
    if (perdiste!=0)
    {
        archivo=fopen(archi,"r");
    }
    else
    {
        archivo=fopen(archi2,"r");
    }

    if(archivo!=NULL)
    {
        while(fread(&aux,sizeof(char),1,archivo)>0)
        {

            color(10);
            printf("%c",aux);
        }
        printf("\t\t\tSCORE %d   ",score);
        Sleep(6000);
    }
    else
    {
        printf("\n Archivo inexistente \n");

    }

    if(fclose(archivo)!=0)
        printf("Error al Cerrar Archivo");
}


void PantallaTitulo(void)
{


    char aux;
    char archi[]="titulo.txt";
    FILE * archivo=fopen(archi,"r");
    if(archivo!=NULL)
    {
        while(fread(&aux,sizeof(char),1,archivo)>0)
        {
            color(10);
            printf("%c",aux);
        }
    }
    else
    {
        printf("\n Archivo inexistente \n");

    }

    printf("\n\n\n");
    if(fclose(archivo)!=0)
        printf("Error al Cerrar Archivo");

}


void imprimirControles ()
{
    color(6);
    printf("\n\n\tCONTROLES:\n\n");
    color(2);
    printf("\n\n\t\t\t^\n");
    printf("\t\t\t|\n");
    printf("\t\t\t|\n");
    color(12);
    printf("\n\t\t\tW\n");
    color(2);
    printf("\n\n\t<---");
    color(12);
    printf("   A\t\tD   ");
    color(2);
    printf("--->\n");
    color(12);
    printf("\n\n\t\t\tS\n\n");
    color(2);
    printf("\t\t\t|\n");
    printf("\t\t\t|\n");
    printf("\t\t\tv\n");
    Sleep(2500);
    getch();
    system("cls");
}
