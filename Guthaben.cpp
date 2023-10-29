// Es sollen Kontostaende verwaltet werden und zwar in den Variablen konto..._guthaben.
// Dabei werden die Kontostaende mehr oder weniger beliebig hoch- und herunter gezaehlt.
// Um eventuelle Ueber- oder Unterlaeufe dabei geht es hier nicht.
// Weiterhin ist fuer ein Konto eine Variable ..._pruefwert vorhanden; fuer dieses Konto soll 
// immer der Wert von guthaben aber mit umgekehrtem Vorzeichen liegen.
// Ein Funktion "checkSum" prüft immer wieder, ob das auch der Fall ist.
// In den Funktionen "update....", wird der Kontostand veraendert.

// Ihr Programm soll 4 zusätzliche Threads starten. Einer davon soll checkSum() aufrufen,
// einer jeweils die betr. update-Funktion

// Bauen Sie die noetigen Synchronisationen ein, so das trotzdem moeglichst viel Parallelitaet moeglicht ist!!!
// Bauen Sie keine unnoetigen Synchronisationen ein.

// Kuemmern Sie sich abschliessend in main auch darum, dass auf den vollstaendigen Abschluss
// aller threads gewartet wird.

// (Falls im vorgegeben Programm aus Versehen kleine Fehler bei ihnen zu Warnings oder Problemen fuern, 
// dann korrigieren sie diese bitte selbststaendig, ohne die Bedeutung des Programmes zu veraendern.)

#undef UNICODE

//includes:
#include<stdio.h>
#include <windows.h>
#include <conio.h>
//globales:
HANDLE m1,m2;
//Anzupassen, je nach Rechnergeschwindigkeit:
#define  BUCHUNGEN      (1e7)
#define MUTEX 1
#define INTERLOCK 2
#define SYNCHRO_METHODE MUTEX
static long konto1_guthaben=  1000;
static long konto1_pruefwert= -1000;

static long konto2_guthaben= 100;

static long konto3_guthaben= 100;  // kein pruefwert bei Konto3;


// Pruef ob Check-Werte stimmen. (Das muss immer der Fall sein!)
void checkSum(void) {
    int sum;
    long int cnt;
    DWORD s;
    for(cnt= 0; cnt<BUCHUNGEN; cnt++){
        // Check-Wert für Konto1 pruefen:
   
                sum= konto1_guthaben + konto1_pruefwert;
        
   
        if(sum != 0) {
            printf("\n Fehler bei Kontrolle von pruefwert bei Konto 1: %d", sum);
        }
    }

}//checkSum


// Aendert den Betrag der Konten
DWORD WINAPI update1(LPVOID n) {
    long int cnt;
    DWORD s;
    Sleep(10);
    printf("Thread 1 gestartet...\n");
    for(cnt= 0; cnt<BUCHUNGEN; cnt++){
    #if (SYNCHRO_METHODE==MUTEX)
        s=WaitForSingleObject(m1, INFINITE);
            konto1_guthaben--;			
            konto1_pruefwert++;
         s=ReleaseMutex(m1);
      s=WaitForSingleObject(m2, INFINITE);
            konto2_guthaben++;
            s=ReleaseMutex(m2);
    #elif (SYNCHRO_METHODE==INTERLOCK)
        InterlockedDecrement(& konto1_guthaben);
        InterlockedIncrement(& konto1_pruefwert);
        InterlockedIncrement(& konto2_guthaben);
    #endif
    }
    printf("Thread 1 beendet...\n");
    return EXIT_SUCCESS;
}//update


// Aendert den Betrag der Konten
DWORD WINAPI update2(LPVOID n) {
    long int cnt;
    DWORD s;
    Sleep(10);
    printf("Thread 2 gestartet...\n");
    for(cnt= 0; cnt<BUCHUNGEN; cnt++){
    #if (SYNCHRO_METHODE==MUTEX)

           s=WaitForSingleObject(m1, INFINITE);
            konto1_guthaben++;			
            konto1_pruefwert--;
            s=ReleaseMutex(m1);
            s=WaitForSingleObject(m2, INFINITE);
            konto2_guthaben--;
            s=ReleaseMutex(m2);
    #elif (SYNCHRO_METHODE==INTERLOCK)

        InterlockedIncrement(& konto1_guthaben);
        InterlockedDecrement(& konto1_pruefwert);
        InterlockedDecrement(& konto2_guthaben);
    #endif
    }
    printf("Thread 2 beendet...\n");
    return EXIT_SUCCESS;
} //update


// Aendert den Betrag der Konten
DWORD WINAPI  update3(LPVOID n) {
    long int cnt;
    DWORD s;
    Sleep(10);
    printf("Thread 3 gestartet...\n");
    for(cnt= 0; cnt<BUCHUNGEN; cnt++){
        
            konto3_guthaben++;
            			
    }
    printf("Thread 3 beendet...\n");
    return EXIT_SUCCESS;
}//update


//Platz fuer die Thread-Funktionen
DWORD WINAPI update4(LPVOID n){
    Sleep(10);
    printf("Thread 4 gestartet...\n");
     checkSum();
     printf("Thread 4 beendet...\n");
    return EXIT_SUCCESS;
 }

int main(void) {
    //Platz fuer Initialisierungen
    HANDLE hThread[4];  
    DWORD threadID[4];
    DWORD ExitCode[4];

    m1 = CreateMutex (NULL, FALSE, "Sem A");
    m2 = CreateMutex (NULL, FALSE, "Sem B");
    
    
    hThread[0] = CreateThread (NULL, 0, update1, NULL, 0, &threadID[0]);
    if(hThread[0]==0) printf("Error: Thread 1 not launched\n");
    hThread[1] = CreateThread (NULL, 0, update2, NULL, 0, &threadID[1]);
    if(hThread[1]==0) printf("Error: Thread 2 not launched\n");
    hThread[2] = CreateThread (NULL, 0, update3, NULL, 0, &threadID[2]);
    if(hThread[2]==0) printf("Error: Thread 3 not launched\n");
    hThread[3] = CreateThread (NULL, 0, update4, NULL, 0, &threadID[3]);
    if(hThread[3]==0) printf("Error: Thread 4 not launched\n");
     

 do {

    printf("\n\n neue Threads laufen gleich los..." );
     Sleep(10);
    //IHRE AUFGABE: auf alle gestarteten Threads warten (ohne sleep, ohne polling):
   printf("konto1_guthaben: %ld, konto1_pruefwert: %ld, konto2_guthaben: %ld, konto3_guthaben: %ld \n", konto1_guthaben, konto1_pruefwert,konto2_guthaben,konto3_guthaben);
        GetExitCodeThread(hThread[0], &ExitCode[0]);
        GetExitCodeThread(hThread[1], &ExitCode[1]);
        GetExitCodeThread(hThread[2], &ExitCode[2]);
        GetExitCodeThread(hThread[3], &ExitCode[3]);

       } while ((ExitCode[0]==STILL_ACTIVE)||(ExitCode[1]==STILL_ACTIVE)||(ExitCode[2]==STILL_ACTIVE)
             ||(ExitCode[3]==STILL_ACTIVE));

    printf("\n\n ------Stand am Ende:\n");
	//IHER AUFGABE: Ausgaben der Ende-Kontostaende und Pruefwerte
	
	printf("konto1_guthaben: %ld, konto1_pruefwert: %ld, konto2_guthaben: %ld, konto3_guthaben: %ld \n", konto1_guthaben, konto1_pruefwert,konto2_guthaben,konto3_guthaben);
	
    printf("\n Press any key to exit..\n");
    getchar();


    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);
    CloseHandle(hThread[2]);
    CloseHandle(hThread[3]);


    CloseHandle(m1);
    CloseHandle(m2);
    
   
    
    return EXIT_SUCCESS;
    //Platz fuer Aufraeumarbeiten --> hier nicht gefordert!!!
    
}//main