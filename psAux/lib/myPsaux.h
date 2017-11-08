#include "./toolspsAux.h"

#include <dirent.h>
#include <time.h>

#include <pwd.h>

#define CMDLINE "/cmdline"
#define COMM "/comm"
#define STAT "/stat"
#define STATUS "/status"
#define DEV_PTS "/dev/pts/"
#define DEV "/dev/"
#define TASK "/task/"

#define MEM_INFO "meminfo"
#define STAT_FILE "stat"

#define ONE_MINUTE 60.0

#define NAME_POSITION 1
#define STATE_POSITION 2
#define TTY_POSITION 6
#define TIME_POSITION 11
#define START_TIME_POSITION 17
#define READ_CMD_LINE 15
#define PRIORITY_POSITION 18

#ifdef HAVE_ST_BIRTHTIME
#define birthtime(x) x.st_birthtime
#else
#define birthtime(x) x.st_ctime
#endif

#define TTY "tty"
#define UID "Uid"

char * recup_comm(char * path);
void afficher_cmdLine(char * path);

void afficher_state(char * path);

int recup_uid(char * path);
void afficher_user(char * path);


char * read_dev_pts(char * tty);
char * read_dev(char * tty);
char * getTTY(char * path);
void afficher_tty(char * path);

void afficher_start_time(char * path);

char * recup_rss(char * path);
void afficher_rss(char * path);

char * recup_vsz(char * path);
void afficher_vsz(char * path);

int recup_time( int idfichier );
void afficher_time(char * path);

void afficher_memory_pourcentage(char * path);

int get_cpu_time();
void afficher_cpu_pourcentage(char * path);

void detailsProcessus(char * processusPID);

void readProc();

void start_psaux();
