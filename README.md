# Homework C2

Per avviare i test:
```
docker-compose up
```

## Test
Gli elementi principali del progetto sono:

- Provider
- Accepter
- Reader
- Dispatcher

Provider, Accepter e Reader sono stati realizzati in
modo tale da essere accoppiati a Dispatcher. Il Dispatcher
invece è stato realizzato in modo tale da non dover conoscere
Accepter, Provider e Reader.

Per poter testare e realizzare Provider, Accepter e Reader prima del aver
realizzato il Dispatcher vero e proprio, ho in primo luogo definito
l'interfaccia pubblica del Dispatcher, successivamente ho realizzato
un DispatcherMock sulla base di questa interfaccia.

Questo mi ha anche consentito di realizzare dei test più isolati e che quindi
non dipendono dall'implementazione del Dispatcher vero e proprio.

## Provider
Il costruttore di Provider è il seguente. Viene dunque richiesta la lista di
messaggi che il provider dovrà spedire al dispatcher e il dispatcher.

La lista di messaggi non deve contenere un ultimo messaggio corrispondente
a una poison pill. La poison pill viene inviata direttamente dal providere
nel momento in cui è stata esaurita la lista di elementi della lista
messagesToSend.

Oltre alla lista di messaggi da spedire, viene richiesta una istanza di
dispatcher.
```c
Provider *_new_Provider(list_t *messagesToSend, Dispatcher *dispatcher);
```

Provider inoltre ha soli due metodi pubblici, in particolare:
```c
int (*start)(struct Provider*);
int (*wait)(struct Provider*);
```

Il metodo start permette di avviare il Provider. Questo significa che verrà
creato un flusso di esecuzione, che inizierà a fare put_bloccanti su provider_buffer.

Il metodo wait invece attende che il flusso di esecuzione di Provider abbia terminato
la sua esecuzione.

## Accepter
```c
msg_t* (*sendReader)(struct Accepter*, Reader *reader);
msg_t *(*sendPoisonPill)(struct Accepter*);

void (*start)(struct Accepter*);
void (*wait)(struct Accepter*);

list_t *(*getSubscribedReaders)(struct Accepter*);
unsigned short (*getIsRunning)(struct Accepter*);
```

L'accepter può inviare delle richieste al Dispatcher. In particolare può inviare la richiesta di aggiungere
un reader (mediante sendReader) o può inviare una poison pill, per instigare l'Accepter al suicidio.

Sia sendReader che sendPoisonPill restituiscono il controllo non quando sono state elaborate le richieste dall'accepter,
ma quando l'accepter le ha accettate e inserite nel buffer_accepter. Per questo motivo ha senso
fare una sendReader anche prima di aver avviato l'Accepter.

Il metodo start inizia a svuotare il buffer_accepter recapitando le richieste al Dispatcher. In particolare l'Accepter
fa l'estrazione bloccate dal buffer accepter. Se il messaggio è una poison pill termina il flusso di esecuzione, altrimenti
registra il reader al dispatcher e aggiunge il reader a una sua lista di reader registrati. 

L'add all'interno della lista di reader registrati non deve essere thread safe, in quanto non esistono altri flussi di
esecuzione che fanno scritture su di essa. Questa lista corrisponde alla "Lista dei reader correnti".
```c
static void* _startTask(void* args) {
  Accepter *this = (Accepter *)args;
  buffer_t *buffer = this->_bufferAccepter;
  msg_t *msg = get_bloccante(buffer);
  while(msg != POISON_PILL_MSG) {
    Reader *reader = msg->content;
    reader->subscribe(reader, this->_dispatcher);
    addElement(this->_listOfSubscribedReaders, reader);
    this->_requestSentToDispatcher++;
    msg = get_bloccante(buffer);
  }
  this->_isRunning = 0;
  pthread_exit(0);
}
```

Il metodo wait restituisce il controllo non appena l'accepter estrare dal buffer accepter una poison pill.

## Reader
```c
int (*subscribe)(struct Reader*, Dispatcher *dispatcher);
void (*sendPoisonPill)(struct Reader* this);
```

Il metodo più importante dei Reader è sicuramente subscribe. Come è possibile notare dal codice il dispatcher
inizialmente fa una subscribe sul dispatcher, in cambio ottiene un buffer. Poi viene creato un flusso di esecuzione
che ha come unico obiettivo quello di estrarre messaggi da questo buffer recapitato dal dispatcher ed eseguire una 
qualche operazione.
```c
static int subscribe(struct Reader *this, Dispatcher *dispatcher) {
  this->_buffer = dispatcher->subscribe(dispatcher);
  this->_isSubscribed = 1;
  //printf("Reader si è registrato al dispatcher.\n");
  pthread_t thread;
  pthread_create(&thread, NULL, this->_subscribeTask, this);
  pthread_detach(thread);
  return 1;
}
```

Altro metodo utile è la sendPoisonPill. Un reader infatti può ricevere una poison pill in due modi: o estraendo
un poison pill message dal buffer recapitato dal dispatcher o dal main in fase di chiusura. In questo secondo caso
verrà utilizzato il metodo sendPoisonPill.


## Dispatcher
```c
int (*addMessageOnProviderBuffer)(struct Dispatcher *, msg_t *message);
buffer_t *(*subscribe)(struct Dispatcher *);
void (*start)(struct Dispatcher *this);
void (*wait)(struct Dispatcher *this);
```

Il metodo addMessageOnProviderBuffer viene utilizzato dal provider per aggiungere un messaggio nel provider buffer
del dispatcher corrente.

Il metodo subscribe viene utilizzato dai reader per registrarsi a un certo dispatcher. In particolare dispatcher non
possiede una lista di Reader, lui non sa neanche che esiste il tipo Reader. Dispatcher ha una semplice lista di buffer,
la responsabilità di sapere che i messaggi di un certo reader devono essere estratti a un certo buffer vengono delegati
a qualcun'altro, in particolare al reader che ha effettuato la subscribe.

In questo pezzo di codice per la prima volta si è fatto uso di un semaforo, denominato _useListOfBufferReader. Questo
è un semaforo binario in stile competitivo, che serve a ottenere l'uso esclusivo della lista di buffer reader.

In particolare viene creato un buffer, aggiunto alla lista e restituito al reader.
```c
static buffer_t *subscribe(Dispatcher* this) {
  if (pc_sem_wait(this->_useListOfBufferReader) != 0) {
    return DISPATCHER_ERROR;
  }
  buffer_t *buffer = buffer_init(SIZE_BUFFER_READER);
  addElement(this->_listOfBufferReader, buffer);
  pc_sem_post(this->_useListOfBufferReader);
  return buffer;
}
```

Il metodo start avvia lo smistamento dei messaggi ai reader.
```c
static void *_startTask(void *args) {
  Dispatcher *this = (Dispatcher *) args;

  for(;;) {

    msg_t *message = get_bloccante(this->_bufferProvider);

    if(message == POISON_PILL_MSG) {
      this->_broadcastPoisonPill(this);
      break;
    }

    this->_broadcastMessage(this, message);

  }

  pthread_exit(0);
}
```

broadcastMessage invia i messaggi a tutti i reader registrati. Come è possibile vedere dal codice, in questo contesto
si è preferito utilizzare la put_non_bloccante. Se la put_non_bloccante riesce bene, se non riesce allora il dispatcher
considera quel reader troppo lento e gli invia una poison pill con _sendPoisonPill. Quest'ultimo metodo crea un thread
dedicato, che ha come unico obiettivo quello di fare una put_bloccante sul buffer del reader lento. Oltre a questo
il buffer considerato troppo lento viene immediatamente rimosso dalla lista di buffer registrati.

```c
static int _broadcastMessage(Dispatcher* this, msg_t* message) {

  if (pc_sem_wait(this->_useListOfBufferReader) != 0) {
    return DISPATCHER_ERROR;
  }

  iterator_t *iterator = iterator_init(this->_listOfBufferReader);

  while(hasNext(iterator)) {

    buffer_t *bufferReader = next(iterator);

    if(put_non_bloccante(bufferReader, message) == BUFFER_ERROR) {
      _sendPoisonPill(bufferReader);
      removeElement(this->_listOfBufferReader, bufferReader);
      printf("Reader troppo lento e quindi richiesta rimozione.\n");
    }
  }

  iterator_destroy(iterator);

  pc_sem_post(this->_useListOfBufferReader);

  return 1;
}
```

Infine quando il dispatcher riceve una poison pill, viene richiamato il metodo _broadcastPoisonPill. Questo metodo invia
a ciascun reader ancora registrato una poison pill. Quando ciascuna poison pill è stata consegnata allora restituisce 
il controllo.
```c
static int _broadcastPoisonPill(Dispatcher *this) {
  if (pc_sem_wait(this->_useListOfBufferReader) != 0) {
    return DISPATCHER_ERROR;
  }
  iterator_t *iterator = iterator_init(this->_listOfBufferReader);
  list_t *listOfPThread = list_init();
  while(hasNext(iterator)) {
    buffer_t *bufferReader = next(iterator);
    addElement(listOfPThread, _sendPoisonPill(bufferReader));
    removeElement(this->_listOfBufferReader, bufferReader);
  }
  iterator_destroy(iterator);
  iterator = iterator_init(listOfPThread);
  while(hasNext(iterator)) {
    pthread_join(next(iterator), NULL);
  }
  pc_sem_post(this->_useListOfBufferReader);
  return 1;
}
```
