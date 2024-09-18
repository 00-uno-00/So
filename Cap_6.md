# File System

## File 
Insieme di informazioni corrlate definte da un creatore, trattate come un'entità singola e identificate da **metadati**. I metadata sono informazioni che descrivono i dati stessi, in particolare riuslta utile il **magic number** per identificare il tipo di file.

In generale dividiamo i file in due categorie: **Alfanumerici** usati per testo e codice e **binari** usati per immagini, audio, video e programmi eseguibili.

## File System

Il file system è un insieme di strutture logiche per la memorizzazione dei dati, tendenzialmente vengono usate due tipi di memorizzazioni: **organizzazione Piatta**(memoria secondaria come un insieme di file allo stesso livello) e **organizzazione Gerarchica** (memoria secondaria come un albero di *directory*).

I file fengono gestiti dal file system come oggetti, possiedono un PCB contenente i metedati e in partioclare la **locazione** del file(puntatore al blocco di memoria).

## Operazioni sui file

### Apertura/Chiusura

L'operazione di apertura di un file consente di ottenere un **handle** del file, cioè un riferimento che consente di operare sul file stesso. IN maniera pratica ogni processo farà riferimento ad una **tabella dei file aperti** comune a livello di sistema dove verrà memorizzato l'handle del file e il numero di processi che lo stanno utilizzando. Quando il contatore arriva a 0 il file viene chiuso e le pagine usate per memorizzare i dati vengono liberate.

### Lettura/Scrittura

Viene richiesto l'handle del file e la posizione da cui iniziare a leggere o scrivere. Si può accedere al file in tre modi:
- **accesso sequenziale**: i contenuti del file vengono letti o scritti in sequenza. Ogni operazione di letture fa avanzare il puntatore di posizione mentre ongi scritture aggiunge contenuto in fondo al file.
- **accesso diretto**: sapendo il formato e la grandezza dei dati contenuti nel file si può accedere direttamente ad una posizione specifica.
- **accesso a indice**: un file indicizzato contiene un tabella con le posizioni dei dati e i **dati veri e propri(record)**. es db.

## Directory

Dal punto di vista astratto possiamo vedere una directory come una tabella che punta ai contenuti di un file a partire dal suo nome.

Utilizziamo una stuttura ad albero in cui i **file** sono **le foglie** e le **directory** sono i **nodi interni**. Si consente di definire per ogni utente una **working directory** cioè la directory in cui si trova l'utente quando accede al sistema. Potrà navigare all'interno della directory o cambiare posizioine all'interno dell'albero.

### Directory con grafo aciclico

E' possibile avere directory e file condivise, quindi accessibili da diversi cammini assoluti. Per poter aver accesso a file presenti in directory diverse si possono usare i **link** che sono dei puntatori a file o directory.

Quando l'**utente** vuole eliminare un link sorge il dubbio se eliminare anche il file, per risolvere utilizziamo:
- **link simbolico**: semplice collegamento a un file o directory, se si elimina il link il file rimane.
- **link fisico**: collegamento vero e proprio al file, se si elimina il link si elimina anche il file.

A livello di **sistema** possiamo memorizzare il **numero di link** verso un oggetto e quando il contatore arriva a 0 si elimina l'oggetto.