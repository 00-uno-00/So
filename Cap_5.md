# Memoria Virtuale

Con memoria virtuale indichiamo la separazione tra memoria fisica e logica ovvero dalla visone dell'utente.

## Demand Paging - Lazy Swapping

### Lazy Swapping

I processi non vengono caricati nell loro interezza ma vengono caricate le pagine solo quando necessarie. Questo meccanismo è chiamato **Lazy Swapping** e se ne occupa il Sistema Operativo trami il **Pager**.

Nella tabella delle pagine per ogni pagina è presente un bit di validità che indica se la pagina è presente in memoria o meno, nel caso un processo cerchi di accedere ad una pagina non disponibile il sistema operativo lancia un **Page Fault**. Si verifica tramite la tabella nel PCB del processo che la pagina sia presente in memoria secondaria (e quindi non ancora caricata) e la carica in memoria aggiornando la tabella delle pagine. Nel caso contrario si termina il processo.

### Demand Paging

Il sistema operativo carica in memoria solo le pagine necessarie al processo, questo permette di avere un minor tempo di caricamento e di avere un minor utilizzo di memoria. Nel caso in cui si genrino page fault viene tolta la priorità al processo che riprenderà solo quando la pagina sarà caricata in memoria.

### Swap 

Area in memoria secondaria dove vengono salvate le pagine dei processi in attesa di essere caricate in memoria.

## Processi Padri e Figli

Quando creo processi figli metto in comune tabella delle pagine del processo padre. Se uno dei processi usa exec si carica il nuovo codice. Si parla di **Copiatura su scrittura** a causa del fatto che si debba prima compiare la pagina condivisa e poi sovrascritta.

Quando cerco di caricare una pagina ma la memoria è piena si deve fare un **Swap Out** ovvero salvare una pagina in memoria secondaria(solo nel caso in cui non sia gia presente in mem. sec.) per fare spazio alla nuova pagina.

## Algoritmi di Sostituzione

### FIFO

Associo a ogni pagina un marcatore tmeporale che indica il tempo di caricamento, sostituisco quella con il marcatore piu' basso( e quindi più vecchia). 

Non è però detto che la pagina con il tempo piu' basso sia quella che non verrà piu' usata. Questo causa un comportamento detto **anomalia di Belady** ovvero un aumento del numero di page fault all'aumentare della memoria.

### LRU(Least Recently Used)

Sostituisco la pagina che non è stata usata per il tempo piu' lungo. Per implementare questo algoritmo si può usare una coda o una lista ordinata.

### LRU con Bit di Riferimento

Ogni pagina ha un bit di riferimento che viene settato a 1 quando la pagina viene usata. Ogni tot tempo si trasla la sequenza di bit di riferimento e si sostituisce la pagina con il bit piu' basso.

### Seconda Chance

Le pagine vengono inserite in una coda circolare e ogni pagina ha un bit di riferimento. Quando una pagina viene sostituita se il bit di riferimento è a 1 viene settato a 0 e la pagina viene reinserita in coda.

### Seconda Chance con Bit di Modifica

Oltre al bit di riferimento si ha un bit di modifica che indica se la pagina è stata modificata. Se il bit di riferimento è a 1 e il bit di modifica è a 1 si setta il bit di riferimento a 0 e si reinserisce la pagina in coda.

# Allocazione di Frame

## Allocazione di Frame Processi Utente

Utilizzo per ogni processo un **pool di frame** liberi che vengono allocati dinamicamente.

In generale si possono avere 3 tipi di allocazione di frame:

- **Allocazione Proporzionale**(no priorità): Si assegna un numero di frame proporzionale alla dimensione del processo. Memoria logica($VM^i$) occupata da *N* processi: $V = \sum^{i\epsilon[1,N]}VM^i$ e indicando con *M* il numero di frame disponibili, per il processo $P^i$ si ha $F^i = \frac{VM^i}{V} * M$.
- **Allocazione Locale**(priorità): In caso di page fault si sostituisce una pagina del processo corrente.
- **Allocazione Globale**(priorità): In caso di page fault si sostituisce un frame di un qualsiasi processo. In entrambi i casi si può verificare il fenomeno del **Trashing**: Il sistema operativo spende piu' tempo a sostituire pagine, o localmente causando continui fault o globalmente causando fault a catena, che a eseguire i processi.

### Working Set

Al fine di evitare il trashing si può usare il concetto di **Working Set** ovvero l'insieme di pagine che un processo usa in un intervallo di tempo. Si definisce un parametro $\delta$ che indica l'ampiezza della finestra temporale da analizzare. Bisonga trovare un valore di $\delta$ tale che il Working Set non sia troppo piccolo (causando un numero eccessivo di page fault) o troppo grande (causando un numero eccessivo di swap).

### Somma dei Working Set

La somma dei Working Set di tutti i processi non deve superare il numero di frame disponibili. Se si supera si ha il trashing. In caso contrario posso far partire altri processi. Posso usare la **Prepaginazione** per tenere traccia delle pagine che un processo userà in futuro memorizzandole quando avviene uno swap.

## Allocazione di Frame Processi Kernel

### Sistema Gemellare

Ogni volta che devo allocare dei dati suddivido la zona di memoria riservata al kernel in due parti fino a quando non avro' la zona di memoria con il minor numero di spazio frammentato. Gli altri frame non utilizzati verrano fusionati.

### Allocazione Slab

Individuo con **Slab** una sequenza di pagine fisicamente contigue e con **Cache** un insieme di slab.

Uso le cache per allocare strutture dati usate dal SO. Ogni cache avra' un singolo tipo di struttura dati contenuto all'interno e nel caso sia piena si carca una nuova cache libera, nel caso in cui non ci siano cache libere si creano nuove slabe e sia ssegnano alla nuova cache.