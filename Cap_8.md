# Implementazione File System

### Memoria secondaria
- organizzata in blocchi(possono occupare piu' settori)
- accesso diretto a ogni blocco
- cessibile tramite file system

# Struttura File System

- Driver di dispositivo: carica dati da memoria secondaria in RAM
- File system base: gestisce i comandi al driver
- Modulo organizzazione file: gestione delle posizioni e come sono memorizzati i file in memoria secondaria, conoscenza blocchi liberi
- File system logico: gestione FS a livello di metadati.

## Struttura disco

### Blocchi Speciali
- **boot control block**: contiene SO e info di bootstrap
- **volume control block/superblocco**: info su FS stesso, come numero blocchi, blocchi liberi, ecc.
- **strutture delle directory**: organizzazione file e mantenimento \<nomefile,FCB>
- **lista FCB**: metadati dei file che permettono accesso, chiamati anche **inode**

### INODE/FCB(File Control Block)

Info relative al file, come dimensione, data creazione, data ultima modifica, permessi, *tabella blocchi*, ecc.

Hanno dimensione fissa per semplificare accesso.

## Strutture in RAM

- **mount table**: mantiene info su FS montati
- **directory**: mantiene info su directory aperte recentemente
- **Tabella file Aperti**: mantiene copia di FCB di tutti i file aperti, si usa questo tipo di tabella *per ogni processo*.

**INODE in RAM**: quando si apre un file per la prima volta viene copiato da disco, stato del inode, identificatore del FS a cui appartiene, numero di inode su disco, contatore riferimenti, ecc.

## OPEN
Per ogni open (conta anche fork):
1) si aggiunge un riferimento al file nella **tabella degli incore inode(globale)**, incrementando il contatore riferimenti altrimenti creo nuova entry.
2) viene creata una entry nella **tabella dei file(globale)** che contiente indirizzo del inode in memoria e puntatore alla posizione corrente del file.
3) viene creata una entry nella **tabella dei file del processo** che contiene puntatore alla entry della tabella dei file globale.


## Implementazione Directory

**Sequenza Lineare** directory come lista di coppie \<nomefile, inode_number> occorre scorrere tutta la lista per trovare un file.

**B-Tree**: albero in cui i nodi contengono coppie di indici che delimitano intervalli di ricerca, permette di trovare file in tempo logaritmico.

**Hashing**: mappatura diretta tra nome file e inode, permette di trovare file in tempo costante ma richiede spazio extra.

# Allocazione Memoria

### Allocazione Contigua

Alloco in blocchi contigui, ogni file occupa un numero di blocchi contigui.

- **Vantaggi**: accesso diretto(sia lineare che con hash), semplice e veloce(movimento testina minimo)
- **Svantaggi**: frammentazione esterna, dimensione fissa, difficile allocazione dinamica

### Allocazione Concatenata

Divido il file in parti che possono essere allocate in modo non contiguo. Ogni blocco contiene un puntatore al blocco successivo.

- **Vantaggi**: nessuna frammentazione esterna, allocazione dinamica, dimensione file variabile
- **Svantaggi**: solo accesso sequenziale efficente, accesso lento(movimenti testina), spazio extra per puntatori

### FAT(File Allocation Table)
Riservo una sezione della partizione per la tabella con tanti elementi quanti i blocchi, se un blocco fa parte di un file il contenuto della entry corrispondente punta al blocco successivo.

### Allocazione Indicizzata

Ogni file ha un blocco indice = array contenente puntatori ai blocchi del file. 

- **Vantaggi**: no frammentazione esterna, allocazione dinamica, riduzione frammentazione interna.
- **Svantaggi**: dimensione del blocco indice puo' causare problemi di frammentazione interna(se troppo grande) e impossibile allocazione dinamica(se troppo piccolo).

### Allocazione Indicizzata Multilivello

Come dice il nome utilizzo l'indicizzazione dell'ultimo blocco di memoria per puntare a un possibile blocco indice che punta a ... che punteranno dei blocchi del file.

### Allocazione Indicizzata Ibrida 

Tengo una tabella di accesso divisa in parti che permettono accesso diretto ai blocchi del file, e una parte che permette accesso multilivello. Man mano che aumenta l'indice aumenta la profondita' dell'indicizzazione.

## Gestione Spazio Libero

### BitMap

Uso un vettore con un bit per ogni blocco, 0 se occupato, 1 se libero.

### Lista concatenata

Lista di blocchi liberi, ogni blocco contiene un puntatore al blocco successivo.

### Raggruppamento

Uso un blocco libero di dimensione N per puntare a N-1 blocchi liberi con l'ultimo puntatore al prossimo blocco indice di liberi.

### Conteggio 

Variante del raggruppamento, se trovo blocchi liberi consecutivi tengo solo puntatore al primo e numero di blocchi liberi conscutivi.

# File System Virtuale

Interfaccia tra processo utente e FS che permette di operare a prescindere dal tipo di FS sottostante.

### VNODE al posto di INODE

Uso Vnode come astrazione di INODE che contiene info su diversi tipi di Oggetti, come file, directory, pipe, socket, ecc.

### Operazioni VFS

Il VFS mette a disposizione delle *system call* di base, i FS che si interfacciano con il VFS **devono** implementare queste operazioni.

### Buffer Cache
- Buffer = memoria in cui vengono mesis dati in attesa di essere consumati
- Cache = memoria in cui vengono messe copie di dati per velocizzare l'accesso

La memoria secondaria possiede una memoria di appoggio in ram per velocizzare le operazioni di lettura(cache)/scrittura(buffer).

# idk