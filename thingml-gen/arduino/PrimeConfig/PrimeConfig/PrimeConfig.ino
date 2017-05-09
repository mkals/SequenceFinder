/*****************************************************************************
 * Headers for type : Gen
 *****************************************************************************/

// Definition of the instance struct:
struct Gen_Instance {

// Instances of different sessions
bool active;
// Variables for the ID of the ports of the instance
uint16_t id_prime_serviceG;
// Variables for the current instance state
int Generator_behaviour_State;
// Variables for the properties of the instance
uint8_t PrimeMsgs_MAX_UINT8_var;
uint8_t Generator_testNumber_var;
uint8_t Gen_badIndex_var;
uint8_t Gen_goodIndex_var;
uint8_t * Gen_goodNumbers_var;
uint16_t Gen_goodNumbers_var_size;
uint8_t * Gen_badNumbers_var;
uint16_t Gen_badNumbers_var_size;

};
// Declaration of prototypes outgoing messages:
void Generator_behaviour_OnEntry(int state, struct Gen_Instance *_instance);
void Gen_handle_prime_serviceG_result(struct Gen_Instance *_instance, uint8_t isPrime);
// Declaration of callbacks for incoming messages:
void register_Gen_send_prime_serviceG_testForPrime_listener(void (*_listener)(struct Gen_Instance *, uint8_t));
void register_external_Gen_send_prime_serviceG_testForPrime_listener(void (*_listener)(struct Gen_Instance *, uint8_t));

// Definition of the states:
#define GENERATOR_BEHAVIOUR_STATE 0
#define GENERATOR_BEHAVIOUR_GENERATE_STATE 1
#define GENERATOR_BEHAVIOUR_LEARN_STATE 2


/*****************************************************************************
 * Headers for type : Check
 *****************************************************************************/

// Definition of the instance struct:
struct Check_Instance {

// Instances of different sessions
bool active;
// Variables for the ID of the ports of the instance
uint16_t id_prime_serviceC;
// Variables for the current instance state
int Checker_behaviour_State;
// Variables for the properties of the instance
uint8_t PrimeMsgs_MAX_UINT8_var;

};
// Declaration of prototypes outgoing messages:
void Checker_behaviour_OnEntry(int state, struct Check_Instance *_instance);
void Check_handle_prime_serviceC_testForPrime(struct Check_Instance *_instance, uint8_t number);
// Declaration of callbacks for incoming messages:
void register_Check_send_prime_serviceC_result_listener(void (*_listener)(struct Check_Instance *, uint8_t));
void register_external_Check_send_prime_serviceC_result_listener(void (*_listener)(struct Check_Instance *, uint8_t));

// Definition of the states:
#define CHECKER_BEHAVIOUR_STATE 0
#define CHECKER_BEHAVIOUR_ACTIVE_STATE 1



/* Adds and instance to the runtime and returns its id */
uint16_t add_instance(void * instance_struct);
/* Returns the instance with id */
void * instance_by_id(uint16_t id);

/* Returns the number of byte currently in the fifo */
int fifo_byte_length();
/* Returns the number of bytes currently available in the fifo */
int fifo_byte_available();
/* Returns true if the fifo is empty */
int fifo_empty();
/* Return true if the fifo is full */
int fifo_full();
/* Enqueue 1 byte in the fifo if there is space
   returns 1 for sucess and 0 if the fifo was full */
int fifo_enqueue(byte b);
/* Enqueue 1 byte in the fifo without checking for available space
   The caller should have checked that there is enough space */
int _fifo_enqueue(byte b);
/* Dequeue 1 byte in the fifo.
   The caller should check that the fifo is not empty */
byte fifo_dequeue();

#define MAX_INSTANCES 2
#define FIFO_SIZE 256

/*********************************
 * Instance IDs and lookup
 *********************************/

void * instances[MAX_INSTANCES];
uint16_t instances_count = 0;

void * instance_by_id(uint16_t id) {
  return instances[id];
}

uint16_t add_instance(void * instance_struct) {
  instances[instances_count] = instance_struct;
  return instances_count++;
}

/******************************************
 * Simple byte FIFO implementation
 ******************************************/

byte fifo[FIFO_SIZE];
int fifo_head = 0;
int fifo_tail = 0;

// Returns the number of byte currently in the fifo
int fifo_byte_length() {
  if (fifo_tail >= fifo_head)
    return fifo_tail - fifo_head;
  return fifo_tail + FIFO_SIZE - fifo_head;
}

// Returns the number of bytes currently available in the fifo
int fifo_byte_available() {
  return FIFO_SIZE - 1 - fifo_byte_length();
}

// Returns true if the fifo is empty
int fifo_empty() {
  return fifo_head == fifo_tail;
}

// Return true if the fifo is full
int fifo_full() {
  return fifo_head == ((fifo_tail + 1) % FIFO_SIZE);
}

// Enqueue 1 byte in the fifo if there is space
// returns 1 for sucess and 0 if the fifo was full
int fifo_enqueue(byte b) {
  int new_tail = (fifo_tail + 1) % FIFO_SIZE;
  if (new_tail == fifo_head) return 0; // the fifo is full
  fifo[fifo_tail] = b;
  fifo_tail = new_tail;
  return 1;
}

// Enqueue 1 byte in the fifo without checking for available space
// The caller should have checked that there is enough space
int _fifo_enqueue(byte b) {
  fifo[fifo_tail] = b;
  fifo_tail = (fifo_tail + 1) % FIFO_SIZE;
  return 0; // Dummy added by steffend
}

// Dequeue 1 byte in the fifo.
// The caller should check that the fifo is not empty
byte fifo_dequeue() {
  if (!fifo_empty()) {
    byte result = fifo[fifo_head];
    fifo_head = (fifo_head + 1) % FIFO_SIZE;
    return result;
  }
  return 0;
}

/*****************************************************************************
 * Implementation for type : Check
 *****************************************************************************/

// Declaration of prototypes:
//Prototypes: State Machine
void Checker_behaviour_OnExit(int state, struct Check_Instance *_instance);
//Prototypes: Message Sending
void Check_send_prime_serviceC_result(struct Check_Instance *_instance, uint8_t isPrime);
//Prototypes: Function
uint8_t f_Check_mod(struct Check_Instance *_instance, uint8_t number, uint8_t divisor);
uint8_t f_Check_checkNumber(struct Check_Instance *_instance, uint8_t x);
// Declaration of functions:
// Definition of function mod
uint8_t f_Check_mod(struct Check_Instance *_instance, uint8_t number, uint8_t divisor) {
return number % divisor;
}
// Definition of function checkNumber
uint8_t f_Check_checkNumber(struct Check_Instance *_instance, uint8_t x) {
;uint8_t isPrime = 1;
;uint8_t d = x - 1;
while(d >= 2) {
if(f_Check_mod(_instance, x, d) == 0) {
isPrime = 0;

}
d = d - 1;

}
return isPrime;
}

// Sessions functionss:


// On Entry Actions:
void Checker_behaviour_OnEntry(int state, struct Check_Instance *_instance) {
switch(state) {
case CHECKER_BEHAVIOUR_STATE:{
_instance->Checker_behaviour_State = CHECKER_BEHAVIOUR_ACTIVE_STATE;
Checker_behaviour_OnEntry(_instance->Checker_behaviour_State, _instance);
break;
}
case CHECKER_BEHAVIOUR_ACTIVE_STATE:{
break;
}
default: break;
}
}

// On Exit Actions:
void Checker_behaviour_OnExit(int state, struct Check_Instance *_instance) {
switch(state) {
case CHECKER_BEHAVIOUR_STATE:{
Checker_behaviour_OnExit(_instance->Checker_behaviour_State, _instance);
break;}
case CHECKER_BEHAVIOUR_ACTIVE_STATE:{
break;}
default: break;
}
}

// Event Handlers for incoming messages:
void Check_handle_prime_serviceC_testForPrime(struct Check_Instance *_instance, uint8_t number) {
if(!(_instance->active)) return;
//Region behaviour
uint8_t Checker_behaviour_State_event_consumed = 0;
if (_instance->Checker_behaviour_State == CHECKER_BEHAVIOUR_ACTIVE_STATE) {
if (Checker_behaviour_State_event_consumed == 0 && 1) {
Checker_behaviour_OnExit(CHECKER_BEHAVIOUR_ACTIVE_STATE, _instance);
_instance->Checker_behaviour_State = CHECKER_BEHAVIOUR_ACTIVE_STATE;
Check_send_prime_serviceC_result(_instance, f_Check_checkNumber(_instance, number));
Checker_behaviour_OnEntry(CHECKER_BEHAVIOUR_ACTIVE_STATE, _instance);
Checker_behaviour_State_event_consumed = 1;
}
}
//End Region behaviour
//End dsregion behaviour
//Session list: 
}

// Observers for outgoing messages:
void (*external_Check_send_prime_serviceC_result_listener)(struct Check_Instance *, uint8_t)= 0x0;
void (*Check_send_prime_serviceC_result_listener)(struct Check_Instance *, uint8_t)= 0x0;
void register_external_Check_send_prime_serviceC_result_listener(void (*_listener)(struct Check_Instance *, uint8_t)){
external_Check_send_prime_serviceC_result_listener = _listener;
}
void register_Check_send_prime_serviceC_result_listener(void (*_listener)(struct Check_Instance *, uint8_t)){
Check_send_prime_serviceC_result_listener = _listener;
}
void Check_send_prime_serviceC_result(struct Check_Instance *_instance, uint8_t isPrime){
if (Check_send_prime_serviceC_result_listener != 0x0) Check_send_prime_serviceC_result_listener(_instance, isPrime);
if (external_Check_send_prime_serviceC_result_listener != 0x0) external_Check_send_prime_serviceC_result_listener(_instance, isPrime);
;
}



/*****************************************************************************
 * Implementation for type : Gen
 *****************************************************************************/

// Declaration of prototypes:
//Prototypes: State Machine
void Generator_behaviour_OnExit(int state, struct Gen_Instance *_instance);
//Prototypes: Message Sending
void Gen_send_prime_serviceG_testForPrime(struct Gen_Instance *_instance, uint8_t number);
//Prototypes: Function
uint8_t f_Gen_generateNumber(struct Gen_Instance *_instance);
void f_Gen_updateModel(struct Gen_Instance *_instance, uint8_t number, uint8_t isPrime);
// Declaration of functions:
// Definition of function generateNumber
uint8_t f_Gen_generateNumber(struct Gen_Instance *_instance) {
;uint8_t newNumber = random(0, 255);
return newNumber;
}
// Definition of function updateModel
void f_Gen_updateModel(struct Gen_Instance *_instance, uint8_t number, uint8_t isPrime) {
if(isPrime) {
Serial.printf("%d is prime\n",number);
_instance->Gen_goodNumbers_var[_instance->Gen_goodIndex_var] = number;
_instance->Gen_goodIndex_var = _instance->Gen_goodIndex_var + 1;

} else {
Serial.printf("%d not prime\n",number);
_instance->Gen_badNumbers_var[_instance->Gen_badIndex_var] = number;
_instance->Gen_badIndex_var = _instance->Gen_badIndex_var + 1;

}
}

// Sessions functionss:


// On Entry Actions:
void Generator_behaviour_OnEntry(int state, struct Gen_Instance *_instance) {
switch(state) {
case GENERATOR_BEHAVIOUR_STATE:{
_instance->Generator_behaviour_State = GENERATOR_BEHAVIOUR_GENERATE_STATE;
Generator_behaviour_OnEntry(_instance->Generator_behaviour_State, _instance);
break;
}
case GENERATOR_BEHAVIOUR_GENERATE_STATE:{
break;
}
case GENERATOR_BEHAVIOUR_LEARN_STATE:{
break;
}
default: break;
}
}

// On Exit Actions:
void Generator_behaviour_OnExit(int state, struct Gen_Instance *_instance) {
switch(state) {
case GENERATOR_BEHAVIOUR_STATE:{
Generator_behaviour_OnExit(_instance->Generator_behaviour_State, _instance);
break;}
case GENERATOR_BEHAVIOUR_GENERATE_STATE:{
break;}
case GENERATOR_BEHAVIOUR_LEARN_STATE:{
break;}
default: break;
}
}

// Event Handlers for incoming messages:
void Gen_handle_prime_serviceG_result(struct Gen_Instance *_instance, uint8_t isPrime) {
if(!(_instance->active)) return;
//Region behaviour
uint8_t Generator_behaviour_State_event_consumed = 0;
if (_instance->Generator_behaviour_State == GENERATOR_BEHAVIOUR_LEARN_STATE) {
if (Generator_behaviour_State_event_consumed == 0 && 1) {
Generator_behaviour_OnExit(GENERATOR_BEHAVIOUR_LEARN_STATE, _instance);
_instance->Generator_behaviour_State = GENERATOR_BEHAVIOUR_GENERATE_STATE;
f_Gen_updateModel(_instance, _instance->Generator_testNumber_var, isPrime);
Generator_behaviour_OnEntry(GENERATOR_BEHAVIOUR_GENERATE_STATE, _instance);
Generator_behaviour_State_event_consumed = 1;
}
}
//End Region behaviour
//End dsregion behaviour
//Session list: 
}
int Gen_handle_empty_event(struct Gen_Instance *_instance) {
 uint8_t empty_event_consumed = 0;
if(!(_instance->active)) return 0;
//Region behaviour
if (_instance->Generator_behaviour_State == GENERATOR_BEHAVIOUR_GENERATE_STATE) {
if (1) {
Generator_behaviour_OnExit(GENERATOR_BEHAVIOUR_GENERATE_STATE, _instance);
_instance->Generator_behaviour_State = GENERATOR_BEHAVIOUR_LEARN_STATE;
_instance->Generator_testNumber_var = f_Gen_generateNumber(_instance);
Gen_send_prime_serviceG_testForPrime(_instance, _instance->Generator_testNumber_var);
Generator_behaviour_OnEntry(GENERATOR_BEHAVIOUR_LEARN_STATE, _instance);
return 1;
}
}
//begin dispatchEmptyToSession
//end dispatchEmptyToSession
return empty_event_consumed;
}

// Observers for outgoing messages:
void (*external_Gen_send_prime_serviceG_testForPrime_listener)(struct Gen_Instance *, uint8_t)= 0x0;
void (*Gen_send_prime_serviceG_testForPrime_listener)(struct Gen_Instance *, uint8_t)= 0x0;
void register_external_Gen_send_prime_serviceG_testForPrime_listener(void (*_listener)(struct Gen_Instance *, uint8_t)){
external_Gen_send_prime_serviceG_testForPrime_listener = _listener;
}
void register_Gen_send_prime_serviceG_testForPrime_listener(void (*_listener)(struct Gen_Instance *, uint8_t)){
Gen_send_prime_serviceG_testForPrime_listener = _listener;
}
void Gen_send_prime_serviceG_testForPrime(struct Gen_Instance *_instance, uint8_t number){
if (Gen_send_prime_serviceG_testForPrime_listener != 0x0) Gen_send_prime_serviceG_testForPrime_listener(_instance, number);
if (external_Gen_send_prime_serviceG_testForPrime_listener != 0x0) external_Gen_send_prime_serviceG_testForPrime_listener(_instance, number);
;
}






/*****************************************************************************
 * Definitions for configuration : PrimeConfig
 *****************************************************************************/

uint8_t array_gen_Gen_goodNumbers_var[255];
uint8_t array_gen_Gen_badNumbers_var[255];
//Declaration of instance variables
//Instance gen
// Variables for the properties of the instance
struct Gen_Instance gen_var;
// Variables for the sessions of the instance
//Instance check
// Variables for the properties of the instance
struct Check_Instance check_var;
// Variables for the sessions of the instance


// Enqueue of messages Gen::prime_serviceG::testForPrime
void enqueue_Gen_send_prime_serviceG_testForPrime(struct Gen_Instance *_instance, uint8_t number){
if ( fifo_byte_available() > 5 ) {

_fifo_enqueue( (1 >> 8) & 0xFF );
_fifo_enqueue( 1 & 0xFF );

// ID of the source port of the instance
_fifo_enqueue( (_instance->id_prime_serviceG >> 8) & 0xFF );
_fifo_enqueue( _instance->id_prime_serviceG & 0xFF );

// parameter number
union u_number_t {
uint8_t p;
byte bytebuffer[1];
} u_number;
u_number.p = number;
_fifo_enqueue(u_number.bytebuffer[0] & 0xFF );
}
}
// Enqueue of messages Check::prime_serviceC::result
void enqueue_Check_send_prime_serviceC_result(struct Check_Instance *_instance, uint8_t isPrime){
if ( fifo_byte_available() > 5 ) {

_fifo_enqueue( (2 >> 8) & 0xFF );
_fifo_enqueue( 2 & 0xFF );

// ID of the source port of the instance
_fifo_enqueue( (_instance->id_prime_serviceC >> 8) & 0xFF );
_fifo_enqueue( _instance->id_prime_serviceC & 0xFF );

// parameter isPrime
union u_isPrime_t {
uint8_t p;
byte bytebuffer[1];
} u_isPrime;
u_isPrime.p = isPrime;
_fifo_enqueue(u_isPrime.bytebuffer[0] & 0xFF );
}
}


//New dispatcher for messages
void dispatch_testForPrime(uint16_t sender, uint8_t param_number) {
if (sender == gen_var.id_prime_serviceG) {
Check_handle_prime_serviceC_testForPrime(&check_var, param_number);

}

}


//New dispatcher for messages
void dispatch_result(uint16_t sender, uint8_t param_isPrime) {
if (sender == check_var.id_prime_serviceC) {
Gen_handle_prime_serviceG_result(&gen_var, param_isPrime);

}

}


int processMessageQueue() {
if (fifo_empty()) return 0; // return 0 if there is nothing to do

uint8_t mbufi = 0;

// Read the code of the next port/message in the queue
uint16_t code = fifo_dequeue() << 8;

code += fifo_dequeue();

// Switch to call the appropriate handler
switch(code) {
case 1:{
byte mbuf[5 - 2];
while (mbufi < (5 - 2)) mbuf[mbufi++] = fifo_dequeue();
uint8_t mbufi_testForPrime = 2;
union u_testForPrime_number_t {
uint8_t p;
byte bytebuffer[1];
} u_testForPrime_number;
u_testForPrime_number.bytebuffer[0] = mbuf[mbufi_testForPrime + 0];
mbufi_testForPrime += 1;
dispatch_testForPrime((mbuf[0] << 8) + mbuf[1] /* instance port*/,
 u_testForPrime_number.p /* number */ );
break;
}
case 2:{
byte mbuf[5 - 2];
while (mbufi < (5 - 2)) mbuf[mbufi++] = fifo_dequeue();
uint8_t mbufi_result = 2;
union u_result_isPrime_t {
uint8_t p;
byte bytebuffer[1];
} u_result_isPrime;
u_result_isPrime.bytebuffer[0] = mbuf[mbufi_result + 0];
mbufi_result += 1;
dispatch_result((mbuf[0] << 8) + mbuf[1] /* instance port*/,
 u_result_isPrime.p /* isPrime */ );
break;
}
}
return 1;
}


//external Message enqueue

void initialize_configuration_PrimeConfig() {
// Initialize connectors
register_Gen_send_prime_serviceG_testForPrime_listener(&enqueue_Gen_send_prime_serviceG_testForPrime);
register_Check_send_prime_serviceC_result_listener(&enqueue_Check_send_prime_serviceC_result);


// Network Initialization
// End Network Initialization

// Init the ID, state variables and properties for instance check
check_var.active = true;
check_var.id_prime_serviceC = add_instance( (void*) &check_var);
check_var.Checker_behaviour_State = CHECKER_BEHAVIOUR_ACTIVE_STATE;
check_var.PrimeMsgs_MAX_UINT8_var = 255;

Checker_behaviour_OnEntry(CHECKER_BEHAVIOUR_STATE, &check_var);
// Init the ID, state variables and properties for instance gen
gen_var.active = true;
gen_var.id_prime_serviceG = add_instance( (void*) &gen_var);
gen_var.Generator_behaviour_State = GENERATOR_BEHAVIOUR_GENERATE_STATE;
gen_var.PrimeMsgs_MAX_UINT8_var = 255;
gen_var.Generator_testNumber_var = 10;
gen_var.Gen_badIndex_var = 0;
gen_var.Gen_goodIndex_var = 0;
gen_var.Gen_goodNumbers_var = array_gen_Gen_goodNumbers_var;
gen_var.Gen_goodNumbers_var_size = 255;
gen_var.Gen_badNumbers_var = array_gen_Gen_badNumbers_var;
gen_var.Gen_badNumbers_var_size = 255;

Generator_behaviour_OnEntry(GENERATOR_BEHAVIOUR_STATE, &gen_var);
}




void setup() {
Serial.begin(9600);
initialize_configuration_PrimeConfig();

}

void loop() {

// Network Listener// End Network Listener

Gen_handle_empty_event(&gen_var);

    processMessageQueue();
}
