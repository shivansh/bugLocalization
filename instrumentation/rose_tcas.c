int CountElse[7];
int CountIf[7];

void evaluate()
{
  int i;
  i = 0;
  while(i < 7){
    printf("%d %d\n",CountIf[i],CountElse[i]);
    ++i;
  }
}

void increment_else(int index)
{
  ++CountElse[index];
}

void increment_if(int index)
{
  ++CountIf[index];
}
#include <stdio.h>
#include<stdlib.h>
#define OLEV       600		/* in feets/minute */
#define MAXALTDIFF 600		/* max altitude difference in feet */
#define MINSEP     300          /* min separation in feet */
#define NOZCROSS   100		/* in feet */
/* variables */
typedef int bool;
bool Own_Below_Threat();
bool Own_Above_Threat();
int Cur_Vertical_Sep;
bool High_Confidence;
bool Two_of_Three_Reports_Valid;
int Own_Tracked_Alt;
int Own_Tracked_Alt_Rate;
int Other_Tracked_Alt;
/* 0, 1, 2, 3 */
int Alt_Layer_Value;
int Positive_RA_Alt_Thresh[4];
int Up_Separation;
int Down_Separation;
/* state variables */
/* NO_INTENT, DO_NOT_CLIMB, DO_NOT_DESCEND */
int Other_RAC;
#define NO_INTENT 0
#define DO_NOT_CLIMB 1
#define DO_NOT_DESCEND 2
/* TCAS_TA, OTHER */
int Other_Capability;
#define TCAS_TA 1
#define OTHER 2
/* true/false */
int Climb_Inhibit;
#define UNRESOLVED 0
#define UPWARD_RA 1
#define DOWNWARD_RA 2

void initialize()
{
  Positive_RA_Alt_Thresh[0] = 400;
  Positive_RA_Alt_Thresh[1] = 500;
  Positive_RA_Alt_Thresh[2] = 640;
  Positive_RA_Alt_Thresh[3] = 740;
}

int ALIM()
{
  return Positive_RA_Alt_Thresh[Alt_Layer_Value];
}

int Inhibit_Biased_Climb()
{
  return Climb_Inhibit?Up_Separation + 100 : Up_Separation;
}

bool Non_Crossing_Biased_Climb()
{
  int upward_preferred;
  int upward_crossing_situation;
  bool result;
  upward_preferred = Inhibit_Biased_Climb() > Down_Separation;
  if (upward_preferred) {
    increment_if(0);
{
      result = !Own_Below_Threat() || Own_Below_Threat() && !(Down_Separation >= ALIM());
    }
  }
   else {
    increment_else(0);
{
/* logic change */
      result = Own_Above_Threat() && Cur_Vertical_Sep >= 300 || Up_Separation >= ALIM();
    }
  }
  return result;
}

bool Non_Crossing_Biased_Descend()
{
  int upward_preferred;
  int upward_crossing_situation;
  bool result;
  upward_preferred = Inhibit_Biased_Climb() > Down_Separation;
  if (upward_preferred) {
    increment_if(1);
{
      result = Own_Below_Threat() && Cur_Vertical_Sep >= 300 && Down_Separation >= ALIM();
    }
  }
   else {
    increment_else(1);
{
      result = !Own_Above_Threat() || Own_Above_Threat() && Up_Separation >= ALIM();
    }
  }
  return result;
}

bool Own_Below_Threat()
{
  return Own_Tracked_Alt < Other_Tracked_Alt;
}

bool Own_Above_Threat()
{
  return Other_Tracked_Alt < Own_Tracked_Alt;
}

int alt_sep_test()
{
  bool enabled;
  bool tcas_equipped;
  bool intent_not_known;
  bool need_upward_RA;
  bool need_downward_RA;
  int alt_sep;
  enabled = High_Confidence && Own_Tracked_Alt_Rate <= 600 && Cur_Vertical_Sep > 600;
  tcas_equipped = Other_Capability == 1;
  intent_not_known = Two_of_Three_Reports_Valid && Other_RAC == 0;
  alt_sep = 0;
  if (enabled && (tcas_equipped && intent_not_known || !tcas_equipped)) {
    increment_if(2);
{
      need_upward_RA = Non_Crossing_Biased_Climb() && Own_Below_Threat();
      need_downward_RA = Non_Crossing_Biased_Descend() && Own_Above_Threat();
      if (need_upward_RA && need_downward_RA) {
/* unreachable: requires Own_Below_Threat and Own_Above_Threat
           to both be true - that requires Own_Tracked_Alt < Other_Tracked_Alt
           and Other_Tracked_Alt < Own_Tracked_Alt, which isn't possible */
        increment_if(3);
        alt_sep = 0;
      }
       else {
        increment_else(3);
        if (need_upward_RA) {
          increment_if(4);
          alt_sep = 1;
        }
         else {
          increment_else(4);
          if (need_downward_RA) {
            increment_if(5);
            alt_sep = 2;
          }
           else {
            increment_else(5);
            alt_sep = 0;
          }
        }
      }
    }
  }
   else 
    increment_else(2);
  return alt_sep;
}

int main(argc,argv)
int argc;
char *argv[];
{
  if (argc < 13) {
    increment_if(6);
{
      fprintf(stdout,"Error: Command line arguments are\n");
      fprintf(stdout,"Cur_Vertical_Sep, High_Confidence, Two_of_Three_Reports_Valid\n");
      fprintf(stdout,"Own_Tracked_Alt, Own_Tracked_Alt_Rate, Other_Tracked_Alt\n");
      fprintf(stdout,"Alt_Layer_Value, Up_Separation, Down_Separation\n");
      fprintf(stdout,"Other_RAC, Other_Capability, Climb_Inhibit\n");
      exit(1);
    }
  }
   else 
    increment_else(6);
  initialize();
  Cur_Vertical_Sep = atoi(argv[1]);
  High_Confidence = atoi(argv[2]);
  Two_of_Three_Reports_Valid = atoi(argv[3]);
  Own_Tracked_Alt = atoi(argv[4]);
  Own_Tracked_Alt_Rate = atoi(argv[5]);
  Other_Tracked_Alt = atoi(argv[6]);
  Alt_Layer_Value = atoi(argv[7]);
  Up_Separation = atoi(argv[8]);
  Down_Separation = atoi(argv[9]);
  Other_RAC = atoi(argv[10]);
  Other_Capability = atoi(argv[11]);
  Climb_Inhibit = atoi(argv[12]);
  fprintf(stdout,"%d\n",(alt_sep_test()));
  evaluate();
  exit(0);
}
