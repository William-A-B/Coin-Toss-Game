
#include "math.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


//void fileReadScore();
//void fileWriteScore(double rts, double rmeand, double rmaxd, double rmind, double rsd);
void fileClear();
char *fileReadPassword();


char *fileReadPassword()
{
    static char password[16];
    FILE *pwfp;
    pwfp = fopen("files/password.txt", "r");

    if (pwfp != NULL)
    {
        fscanf(pwfp, "%s", password);
        fclose(pwfp);
    }
    else
    {
        printf("Can not open password file");
    }


    return password;
}


void fileClear()
{
    FILE *fp;
    fp = fopen("files/leaderboard.txt", "w");

    if (fp != NULL)
    {
        fprintf(fp, "position,score\n");

        for (int j = 1; j < 6; j++)
        {
            fprintf(fp, "%d,000000\n", j);
        }
        fclose(fp);
    }
    else
    {
        printf("Can not open leaderboard file");
    }
}

///Debug
/* Called at line 1016 in the main file */
//void fileWriteScore(double rts, double rmeand, double rmaxd, double rmind, double rsd)
//{
//	FILE *fp;
//
//	fp = fopen("files/score.txt", "a");
//	fprintf(fp, "%lf,%lf,%lf,%lf,%lf\n", rts, rmeand, rmaxd, rmind, rsd);
//	fclose(fp);
//}

///Debug
/* Called at line 1017 in the main file */
//void fileReadScore()
//{
//    FILE *fp;
//
//    double rts;
//    double rmeand;
//    double rmaxd;
//    double rmind;
//    double rsd;
//
//    fp = fopen("files/score.txt", "r");
//
//    while(!feof(fp))
//   {
//        fscanf(fp, "%lf,%lf,%lf,%lf,%lf\n", &rts, &rmeand, &rmaxd, &rmind, &rsd);
//        printf("%lf,%lf,%lf,%lf,%lf\n", rts, rmeand, rmaxd, rmind, rsd);
//   }
//}


