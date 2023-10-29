//***************************************************************
// BS03_Semaphore Aufgabe 2 - Vorlage-VarianteB=POSIX
//***************************************************************
// Folgendes Programm erzeugt zwei Threads.
// Der eine Thread inkrementiert die globalen Variablen CntA und 
// CntB ständig. Der andere Thread inkrementiert ebenfalls CntA, 
// dekrementiert aber CntB. Beide Threads enden nach je X
// Zyklen, sodass CntA zum Schluss den Wert 2* X und CntB 
// den Wert 0 aufweisen sollten. Der Primary Thread zeigt 
// fortlaufend den aktuellen Stand von CntA und CntB.
//***************************************************************
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
//***************************************************************
// Globale Variable
volatile long CntA = 0;
volatile long CntB = 0;
static int work_done[2]= {0, 0};  //warum nicht int work_done auf 2 zaehlen!?
//***************************************************************
// Hilfs-Funktionen
int Inc(int Wert){ return(++Wert); }
int Dec(int Wert){ return(--Wert); }
//***************************************************************/
// Thread-Funktionen:
void *thread_func_1(void *arg) {
  static int return_value=1;
  long Cnt;
  usleep(1000L);
  printf("Thread 1 gestartet...\n"); fflush(stdout);
	for(Cnt = 0; Cnt < 1e6; Cnt++){   
		// Simuliere Thread-Aktivität
		CntA= Inc(CntA);
		CntB= Inc(CntB);
		//CntA++; CntB++;//Alternative
	}
  printf("Thread 1 beendet...\n"); fflush(stdout);
  work_done[0]= 1;
  return((void *)return_value); //implizit pthread_exit
}//thread_func_1

void *thread_func_2(void *arg) {
	  static int return_value=1;
	  long Cnt;
	  usleep(1000L);
	  printf("Thread 2 gestartet...\n"); fflush(stdout);
	  for(Cnt = 0; Cnt < 1e6; Cnt++){   
		  // Simuliere Thread-Aktivität
			CntA= Inc(CntA);
			CntB= Dec(CntB);
			//CntA++; CntB--;//Alternative
	  }
      printf("Thread 2 beendet...\n"); fflush(stdout);
      work_done[1]= 1;
	  return((void *)return_value); //implizit pthread_exit
}//thread_func_2

//***************************************************************
// Code für Primary Thread
int main() {
  pthread_t threadId[2];    
  int arg= 0;
  int status;    


  printf( "S E M A - D E M O\n" );
  printf( "=================\n\n" );
  // Erzeuge die 2 Worker Threads
  status= pthread_create(&threadId[0], NULL, thread_func_1, &arg);
  if (status!=0) printf("Error: Thread 1 not launched\n");
  status= pthread_create(&threadId[1], NULL, thread_func_2, &arg);
  if (status!=0) printf("Error: Thread 2 not launched\n");
  // Abhaengen der Threads, um nicht im Warten (join) haengen zu bleiben
  pthread_detach(threadId[0]);
  pthread_detach(threadId[1]);
  // Zeige periodisch CntA und CntB an; bis beide Threads terminieren
  do {
	usleep(1000L);
	printf( "Wert A %12.1ld  Wert B = %12.1ld \n",CntA, CntB );
	fflush(stdout);
  } while ( (work_done[0]==0) || (work_done[1]==0));
  printf("\n Ausgabe der Endwerte: \n");
  printf( "Wert A %12.1ld  Wert B = %12.1ld \n",CntA, CntB );
  printf("\n\nPress any key to exit..\n"); fflush(stdout);
  getchar();
  return 0;
}//main
