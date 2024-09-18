# Deadlock

**Definizione**: Un deadlock è una situazione in cui due o più processi sono incapaci di procedere perché ciascuno attende che l'altro finisca.

**Condizioni necessarie per il deadlock**:
1. **Mutua esclusione**: almeno un risorsa deve essere allocata in modo esclusivo ad un solo processo.
2. **Hold and wait**: un processo deve possedere almeno una risorsa e attendere per una risorsa detenuta da un altro processo.
3. **No preemption**: le risorse non possono essere riassegnate da un processo ad un altro.
4. **Circular wait**: deve esistere un insieme di processi {P0, P1, ..., Pn} tali che P0 sta aspettando una risorsa detenuta da P1, P1 sta aspettando una risorsa detenuta da P2, ..., Pn-1 sta aspettando una risorsa detenuta da Pn e Pn sta aspettando una risorsa detenuta da P0.

## Grafi di assegnazione delle risorse

Un grafo di assegnazione delle risorse è un grafo diretto in cui i **nodi** rappresentano i processi e le risorse e gli **archi** rappresentano le assegnazioni di risorse.

I *Cerchi*(Processi) richiedono attraverso frecce i *Puntini*(istanze) all'interno dei *Rettangoli*(Risorse) che gli mancano. Quando un'istanza è stata assegnata, si disegna una freccia da essa al processo.

Se il grafo contiene un ciclo, allora c'è un deadlock.(condizione necessaria ma non sufficiente)

## Deadlock Avoidance

**Deadlock avoidance** è una strategia che prevede di evitare le condizioni necessarie per il deadlock.

1. Safe State: uno stato è sicuro se c'è almeno una sequenza di esecuzione che permette a tutti i processi di terminare.
2. Unsafe State: uno stato è insicuro se non è sicuro.

Uso una sequenza di esecuzione per simulare se uno stato è sicuro o insicuro. Se una sequenza di esecuzione porta allo stato finale, allora è sicuro, altrimenti è insicuro in questo caso si esegue un rollback e si porcede con l'esplorazione di un'altra sequenza.

Possiamo introdurre gli **Archi di reclamo** per evitare i deadlock. Un processo può richiedere una risorsa che non possiede, bisogna comunque esplorare tutte le possibili sequenze di esecuzione.

## Algoritmo del Banchiere

M = numero di tipi di risorse
N = numero di processi

Variabili usate:
```
disponibili[M] = numero di istanze disponibili per ogni risorsa
massimo[N][M] = massimo numero di risorse che un processo può richiedere
assegnate[N][M] = numero di risorse assegnate ad ogni processo
necessarie[N][M] = numero di risorse che un processo deve ancora richiedere
```
**L'algoritmo soddisfa una richiesta di un processo SSE l'assegnazione delle risorse richieste porta ad uno stato sicuro**

Suddiviso in due parti:
1. Verifica Sicurezza di uno stato
2. Gestione di una richiesta
   
### Verifica Sicurezza di uno stato
In un array lavoro di lunghezza M, copio il vettore disponibili. In un array finito di N elementi, inizializzo a false.

Cerco tra 0 e N se esite un processo che non è stato eseguito e se la necessità di risorse è minore o uguale a lavoro. Se esiste, aggiungo le risorse assegnate al lavoro e marco il processo come eseguito.

Se tutti i processi sono stati eseguiti, allora lo stato è sicuro.

### Gestione di una richiesta

Controllo per ogni processo se la richiesta sia maggiore della necessità, se è maggiore della disponibilitànel primo caso il processo viene sospeso mentre nel secondo viene messo in attesa finchè non ci sono abbastanza risorse.

Se la richiesta è soddisfacibile, simulo l'assegnazione delle risorse e [verifico](#verifica-sicurezza-di-uno-stato) se lo stato è sicuro. Se lo è, allora assegno le risorse e se no, il processo viene sospeso e lo stato torna allo stato precedente.

