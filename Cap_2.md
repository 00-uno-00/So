# Processi

I Processi vengono eseguiti singolarmente anleternandosi dando l'illusione di essere eseguiti in parallelo.
Stati:
- **Nuovo**: processo appena creato
- **Ready**: processo pronto ad essere eseguito ma in attesa di CPU
- **Running**: processo in esecuzione
- **Waiting**: processo in attesa di un evento( I/O, segnale, ecc.)
- **Terminated**: processo terminato(exit, abort, kill)

Processi rappresentati da una struttura dati chiamata **PCB (Process Control Block)** che contiene:
- **Process State**: stato del processo
- **Program Counter**: indirizzo della prossima istruzione da eseguire
- **Copia dei Registri**: valori dei registri del processore quando passa da running a ready
- **Informazioni di Scheduling**: priorità, tempo di CPU usato, tempo di CPU totale richiesto
- **Informazioni di Gestione della Memoria**: informazioni sulla memoria assegnata al processo
- **Info risorse**
- **I/O State**

Uso il **CONTEXT SWITCH** per passare da un processo all'altro. (salvo info e carico nuovo processo)

## Code di Processo

- **Coda Ready**: processi in attesa di CPU
- **Coda di Dispositivo**: processi in attesa di operazione richiesta su dispositivo

## Scheduling

- **Scheduling a lungo termine**: coda dei processi ready in memoria secondaria, vengono caricati in memoria principale quando il processo in esecuzione termina. Come **criterio** si cerca di mantenere equilibrio tra processi I/O bound e CPU bound
- **Scheduling a medio termine**: alcuni processi sono spostati in memoria secondaria(*SWAPPING*) fino a quando non sono pronti per essere eseguiti
- **Scheduling a breve termine(CPU)**: processi caricati in ram per essere eseguiti(coda non sempre FIFO)

## Scheduling CPU
Dispatcher cambia il contesto del processore per eseguire un nuovo processo
Criteri di scheduling:
- **Throughput**: numero di processi completati in un'unità di tempo
- **Turnaround time**: tempo di completamento di un processo
- **Tempo di attesa**: tempo trascorso in coda ready
- **Tempo di Risposta**: tempo trascorso tra la richiesta e la prima risposta

## Algoritmi di Scheduling

- **First Come First Served (FCFS)**: FIFO
  Il primo processo che arriva è il primo a essere eseguito il tempo medio di attesa puo' essere abbssato se si eseguono prima i processi più brevi
- **Shortest Job First (SJF)**: il processo più breve è eseguito per primo
- **Shortest Remaining time First(PRELAZIONE)**: il processo più breve è eseguito per primo, se arriva un processo più breve di quello in esecuzione viene prelazionato(*interrompo il processo in esecuzione*)
 
Entrambi gli algoritmi sono soggetti a **starvation** se ci sono processi più brevi che arrivano continuamente, per risolvere si da' priorità maggiore ai processi più vecchi.

- **Round Robin**: ogni processo ha un tempo di esecuzione fisso(*quantum*), se il processo non termina entro il quantum viene messo in coda e si passa al prossimo processo. Se il quantum è troppo grande si avvicina a FCFS, se è troppo piccolo si avvicina a SJF.

## Creazione e Terminazione

I processi sono tutti generati da un processo padre, essendo SO un processo sara' la radice dell'albero dei processi che si andara' a creare.

**fork()**: crea un nuovo processo identico al processo chiamante, possono eseguire in parallelo o il padre aspetta che termini il figlio.

**exec()**: sovrascrive il processo chiamante con un nuovo processo, il processo chiamante termina e viene sostituito dal nuovo processo.

La terminazione avviene tramite system call conoscendo il PID del processo da terminare.

## Comunicazione tra processi

Diventa necessaria nel momento in cui voglio creare processi cooperanti tra loro.

- **Memoria Condivisa**: due processi condividono una regione di memoria definita da una struttura dati, bisogna controllare che non ci saino inconsistenza di dati tra i processi(lock possono risolvere)
- **Scambio di messaggi**: Comunicazione tramite messaggi tra processi, il SO si occupa di inviare e ricevere i messaggi. Posso avere comunicazione sincrona(uno aspetta l'altro) o asincrona(uno invia l'altro riceve), diretta o indiretta(buffer di dimensione >=0).
- **Pipe**: comunica tra processi, unidirezionale, un processo scrive e l'altro legge. Possono essere anonime o con nome.

## Thread

Al posto di creare un singolo thread per la gestione di un processo, si creano più thread per gestire più attività in parallelo. I thread condividono lo stesso spazio di memoria e risorse del processo padre.

- **fork**: vedi sopra
- **exec**: vedi sopra

**Operazioni sui thread:**
- **Creazione**: creazione di un nuovo thread con relativa struttura dati per mantenimento informazioni
- **Terminazione**: veloce rispetto a processo in quanto non c'è bisogno di gestione risorse
- **Join**: attesa di terminazione di un thread da parte di un altro thread

## Modello di Thread Linux

**1:1** Ogni thread utente corrisponde a un thread nel kernel. Questo approccio consente una buona gestione della concorrenza e sfrutta al meglio i sistemi multicore. Ogni thread ha bisogno di un suo contesto di esecuzione nel kernel.

Ogni thread e' considerato un Light Weight Process(LWP) e il kernel si occupa di gestire i thread e non il processo.

**N:M** In un sistema molti a molti, *N* thread utente sono mappati su *M* thread kernel. Questo modello consente di avere più thread utente rispetto ai thread kernel, ma è più complesso da gestire.

I *LWP* corrispondono gli *M* thread kernel e vengono creati per la gestione dello scheduling tra thread utente.

In questo caso sara' necessario effettuare uno scheduling all'interno di un processo per decidere quale thread far eseguire su LWPs. Questo scheduling viene indicato come **Process-contention scope**.

Viene poi eseguito uno scheduling tra LWPs per decidere quale thread far eseguire su CPU, questo viene indicato come **System-contention scope**. Nel modello 1:1 non c'è bisogno del primo scheduling.

## Esecuzione concorrente asincrona

### Sezione Critica

Sezione di codice in cui un processo accede a una risorsa condivisa, se due processi accedono alla stessa risorsa in modo concorrente si deve effettuare **MUTUA ESCLUSIONE** si deve permettere l'utilizzo delle risorse condivise quando non sono necessarie limitando le ATTESE ATTIVE di accesso.

### Algortimo del Fornaio

Per $P_i$
```c
choosing[i] = true;
ticket[i] = max_ticket + 1;
choosing[i] = false;
for (j = 0; j<N; j++) {
  while (choosing[j]){
      no_op;
    }
  while ((ticket[j] != 0 && ticket[j] < ticket[i])// se ticket di un altro processo è minore 
        || (ticket[j] == ticket[i] && j < i)){// se ticket sono uguali e j < i (j più vecchio)
            no_op;
           }
}

// Sezione critica

ticket[i] = 0;

// Fine sezione critica
```

**Contro**: L'algoritmo lascia in attesa attiva i processi che non possono accedere alla sezione critica.

### Test and Set

```c
bool test_and_set(bool *lock){
  bool old = *lock;
  *lock = true;
  return old;
}
```

La funzione permette di entrare in sezione critica se il valore di ritorno è false, contemporaneamente bloccando l'accesso agli altri processi.

## Semafori

Variabile a cui si accede solo tramite due operazioni: P(wait) e V(signal).

```c
P (semaforo s){
  while (s <= 0){
    no_op;
  }
  s--;
}

V (semaforo s){
  s++;
}
```

Sono inizializzati a 1 e vengono decrementati quando un processo entra in sezione critica e incrementati quando esce.

### Problemi di sincronizzazione

- **Produttore-Consumatore**: un processo produce dati e l'altro li consuma, il produttore non deve produrre se il buffer è pieno e il consumatore non deve consumare se il buffer è vuoto.
- **Lettori-Scrittori**: più processi possono leggere contemporaneamente, ma solo uno può scrivere alla volta.
- **5 Filosofi**: 5 filosofi che mangiano e pensano, ogni filosofo ha una forchetta a destra e una a sinistra, per mangiare deve prendere entrambe le forchette.
  - **Deadlock**: nel caso in cui tutti prendano la posata a destra e nessuno possa prendere la posata a sinistra.
  - **Starvation**: du filosofi prendono sempre due posate ma gli altri non possono mangiare.
  - **LiveLock**: i filosofi prendono e lasciano le posate in modo sincronizzato, ma nessuno mangia.
  - **Soluzione con Monitor**: utilizzo monitor per gestire l'accesso alle posate definisco una coda di condition, quando un filosofo vuole mangiare deve chiedere al monitor di prendere le posate, se non può aspetta.