# Capitolo 1

## Eventi

**Interrupt**: sono causati dai dispositivi fisici, sono segnali inviati alla CPU attraverso il bus
**Trap**: sono causati dai programmi, possono codificare errori o corrispondere richieste di esecuzioni speciali

## Memoria Centrale(Volatile)

Utilizzata come storage di istruzioni e dati per la CPU. I dati vengono spostati prima nella **CACHE** e poi nei **REGISTRI** con **LOAD** e viceversa con **STORE**.

Non si mantengono grandi quantità di dati, ma solo quelli necessari per l'esecuzione delle istruzioni.

## Memoria Secondaria(Permanente)

Utilizzata per memorizzare dati e programmi in modo **permanente**. Esempi sono i dischi rigidi e le memorie flash.

## Dispositivi di I/O

CPU connessa a dispositivi di I/O tramite **BUS**. I dispositivi di I/O sono connessi al BUS tramite **Controller**.

**Controller:** gestisce uno o più dispositivi di I/O, ha associato un driver SW, possiede dei registri per la comunicazione con la CPU.

# Dual Mode

Applico un modello a livelli(*Kernel e User*) per differenziare processi Utnete e processi di sistema per non far sovrascrivere dati.

## System Call

I processi Utente possono fare chiamate di sistema tramite librerie che cambiano *modalita' di esecuzione*(**operating system trap**)

## Processi

SO e' un processo e deve poter:
- identificare 
- creare e cancellare
- gestire la comunicazione/sincronizzazione intraprocesso
- identificare deadlock
- evitare starvation

per ogni procresso. In aggiunta ogni processo viene mantenuto in RAM come astrazione.

## Gestione Memdoria

IL SO deve poter gestire in modo efficiente la memoria caricando e scaricanod i processi in RAM trasferendoli in memoria centrale o secondaria, di conseguenza deve anche gestire le operazioni di I/O.

## Struttura del Sistema Operativo

Definzione di **Criteri**(specifica di comportamento) e **Meccanismi**(implementazione di un criterio) in base all'obbiettivo prescelto.

Definizione di Architettura del SO, bisogna fare in modo che il SO sia modulare e scalabile.
Tecniche di Modularizzazione:
- **Stratificazione**: visione a strati del SO, dal basso verso l'alto ogni livello parla escusivamente con i livelli adiacenti. Permette di avere interfacce e criteri ben definiti tra i livelli.
- **MicroKernel**: SO con kernel minimale, i servizi sono implementati come processi utente. Questo permette di avere un SO piu' flessibile e scalabile costringendo a passare dal microkernel.
- **Moduli(MIGLIORE)**: SO con kernel minimale costituito da moduli aggiuntivi (anche runtime) per implementare i servizi, queste operazioni aggiuntive sono sempre a livello di KERNEL. I moduli possono usare altri moduli(flessibilita') e moduli comunicano in modo diretto tra di loro(efficenza).  