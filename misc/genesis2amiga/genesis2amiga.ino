/*
 * This sketchs maps Genesis 3 and 6 button controller to
 * the Amiga game port. I wrote it because my old Amiga
 * joysticks are not working anymore. Second hand hardware
 * on various internet markets had problems too. Luckily, Sega
 * Genesis gamepads are almost pin compatible and fresh replicas
 * can be bought everywhere. Therefore, I decided to build a
 * programmable Arduino based Genesis2Amiga adapter.
 * 
 * The theoretic aspects and pin layouts can be found here:
 * 
 * - https://jonthysell.com/2014/07/26/reading-sega-genesis-controllers-with-arduino/
 * - https://github.com/jonthysell/SegaController/wiki/How-To-Read-Sega-Controllers
 * 
 * Basically, three button Genesis gamepads have a select pin which controls a 
 * multiplexer. Depending on its state, certain buttons are mapped to the remaining
 * 6 output lines (9 pins - select pin - Vcc - ground == 6). The six button Sega 
 * gamepad is a little bit more complicated and the select pin performs transitions
 * in a state machine. 
 * 
 * My code runs in a 60 Hertz loop and checks whether a 3 or 6 button gamepad is 
 * connected. Afterwards, it uses the select pin to read all buttons of the 
 * controller and maps them to the Amiga game port. Three switches are available:
 * 
 * - SERIAL_OUTPUT: Send pressed buttons in a human readable format to the UART
 *                  to allow easy debugging
 * - LED_OUTPUT:    Arduino board LED is turned on when a button is pressed.
 * - PIN_OUTPUT     Genesis buttons are mapped to Amiga game port. This is what
 *                  almost everybody wants, so do not turn it off. ;)
 *                  
 * Whats implemented:
 * - Every Genesis button works
 * - D-pad and three fire buttons are mapped to the Amiga game port
 * - Button X: Activates autofire
 * - Button Y: Left/right alternate for these joystick killers like winter games ;)
 * - Button Mode: Switch to emulate UP with Button B
 * 
 * Whats planned:
 * - Record function for complexer combos like Mortal combat fatalities.
 * 
 * For questions, feel free contact christian.ammann@phobosys.de
 */

//#define SERIAL_OUTPUT
#define LED_OUTPUT
#define PIN_OUTPUT

#define STATE_CHANGE_TIME 10

#define INDEX_UP  0
#define INDEX_DOWN 1
#define INDEX_LEFT 2
#define INDEX_RIGHT 3
#define INDEX_A 4
#define INDEX_B 5
#define INDEX_C 6
#define INDEX_X 7
#define INDEX_Y 8
#define INDEX_Z 9
#define INDEX_START 10
#define INDEX_MODE 11

boolean btn_pressed;
char btn_details[] = "____________";

int btn_up_z = A0;       //pin 1
int btn_down_y = A1;     //pin 2
int btn_left_x = A2;     //pin 3
int btn_right_mode = A3; //pin 4
                         //pin 5 == Vcc
int btn_a_b = A5;        //pin 6
int output_select = A4;  //pin 7
                         //pin 8 == Gnd
int btn_start_c = 7;     //pin 9

#ifdef PIN_OUTPUT
boolean autofire_on;
boolean autofire_status;
boolean autoleftright_on;
boolean autoleftright_status;

boolean emulate_up_with_button_two;
boolean mode_pressed;

int amiga_up = 0;        //pin 1
int amiga_down = 1;      //pin 2
int amiga_left = 2;      //pin 3
int amiga_right = 3;     //pin 4
int amiga_btn_3 = 4;     //pin 5
int amiga_btn_1 = 5;     //pin 6
                         //pin 7 == Vcc
                         //pin 8 == Gnd
int amiga_btn_2 = 6;     //pin 9
#endif

#ifdef LED_OUTPUT
int led = 13;
#endif

void setup() {
  pinMode(btn_up_z, INPUT_PULLUP);
  pinMode(btn_down_y, INPUT_PULLUP);
  pinMode(btn_left_x, INPUT_PULLUP);
  pinMode(btn_right_mode, INPUT_PULLUP);
  pinMode(btn_a_b, INPUT_PULLUP);
  pinMode(btn_start_c, INPUT_PULLUP);
  
  pinMode(output_select, OUTPUT);
  digitalWrite(output_select, LOW);

  #ifdef LED_OUTPUT
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  #endif

  #ifdef SERIAL_OUTPUT
  Serial.begin(57600);    // USB serial port 0
  #endif

  #ifdef PIN_OUTPUT
  autofire_on = false;
  autofire_status = true;
  autoleftright_on = false;
  autoleftright_status = true;
  
  emulate_up_with_button_two = false;
  mode_pressed = false;
  
  pinMode(amiga_up, OUTPUT);
  pinMode(amiga_down, OUTPUT);
  pinMode(amiga_left, OUTPUT);
  pinMode(amiga_right, OUTPUT);
  pinMode(amiga_btn_3, OUTPUT);
  pinMode(amiga_btn_1, OUTPUT);
  pinMode(amiga_btn_2, OUTPUT);
  digitalWrite(amiga_up, HIGH);
  digitalWrite(amiga_down, HIGH);
  digitalWrite(amiga_left, HIGH);
  digitalWrite(amiga_right, HIGH);
  digitalWrite(amiga_btn_3, HIGH);
  digitalWrite(amiga_btn_1, HIGH);
  digitalWrite(amiga_btn_2, HIGH);
  #endif
}

void loop() {
  //reset variables and controller
  btn_pressed = false;
  for(int i=0;i<strlen(btn_details);i++){
    btn_details[i] = '_';
  }
  
  //wait for next frame
  delay(16);
  delayMicroseconds(600); 

  //process s0, s1, ..., s7
  start_fsm();

  //output to led, serial port or amiga pins
  //if buttons where pressed
  #ifdef LED_OUTPUT
  led_payload();
  #endif

  #ifdef SERIAL_OUTPUT
  serial_payload();
  #endif

  #ifdef PIN_OUTPUT
  pin_payload();
  #endif  
}

#ifdef PIN_OUTPUT
void pin_payload(){
  //check for emulation of up with sega button B
  if(btn_details[INDEX_MODE] != '_' && !mode_pressed){
    emulate_up_with_button_two = !emulate_up_with_button_two;
    mode_pressed = true;
  }
  else if(btn_details[INDEX_MODE] == '_'){
    mode_pressed = false;
  }
  
  //check for autofire
  if(btn_details[INDEX_X] != '_'){
    autofire_on = true;
    if(autofire_status){
      digitalWrite(amiga_btn_1, LOW);
      autofire_status = false;
    }
    else{
      digitalWrite(amiga_btn_1, HIGH);
      autofire_status = true;      
    }
  }
  else{
    autofire_on = false;
    autofire_status = true;
    digitalWrite(amiga_btn_1, HIGH); //TODO: not good
  }  

  //check for autoleftright
  if(btn_details[INDEX_Y] != '_'){
    autoleftright_on = true;
    if(autoleftright_status){
      digitalWrite(amiga_left, LOW);
      digitalWrite(amiga_right, HIGH);
      autoleftright_status = false;
    }
    else{
      digitalWrite(amiga_left, HIGH);
      digitalWrite(amiga_right, LOW);
      autoleftright_status = true;  
    }
  }
  else{
    autoleftright_on = false;
    autoleftright_status = true;
    digitalWrite(amiga_left, HIGH); //TODO: not good
    digitalWrite(amiga_right, HIGH); //TODO: not good
  }

  //normal buttons
  if(btn_details[INDEX_UP] != '_'){
    digitalWrite(amiga_up, LOW);
  }
  else{
    digitalWrite(amiga_up, HIGH);
  }

  if(btn_details[INDEX_DOWN] != '_'){
    digitalWrite(amiga_down, LOW);
  }
  else{
    digitalWrite(amiga_down, HIGH);
  }

  //left and right are disabled in autoleftright mode
  if(!autoleftright_on){
    if(btn_details[INDEX_LEFT] != '_'){
      digitalWrite(amiga_left, LOW);
    }
    else{
      digitalWrite(amiga_left, HIGH);
    }

    if(btn_details[INDEX_RIGHT] != '_'){
      digitalWrite(amiga_right, LOW);
    }
    else{
      digitalWrite(amiga_right, HIGH);
    }
  }

  //button 1 is disabled in autofire mode
  if(!autofire_on){
    if(btn_details[INDEX_A] != '_'){
      digitalWrite(amiga_btn_1, LOW);
    }
    else{
      digitalWrite(amiga_btn_1, HIGH);
    }  
  }

  //in this mode, we press B and send
  //button 2 signal to amiga
  if(!emulate_up_with_button_two){
    if(btn_details[INDEX_B] != '_'){
      digitalWrite(amiga_btn_2, LOW);
    }
    else{
      digitalWrite(amiga_btn_2, HIGH);
    }
  }
  //in this mode, we press B and send
  //button up signal to amiga. DO not interfere with
  // the regular UP button.  
  else if(emulate_up_with_button_two && btn_details[INDEX_UP] == '_'){
    if(btn_details[INDEX_B] != '_'){
      digitalWrite(amiga_up, LOW);
    }
    else{
      digitalWrite(amiga_up, HIGH);
    }     
  }
}
#endif

#ifdef LED_OUTPUT
void led_payload(){
  if(btn_pressed){
    digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(led, LOW);
  }
}
#endif

#ifdef SERIAL_OUTPUT
void serial_payload(){
  Serial.println(btn_details);
}
#endif

void start_fsm(){
  //----
  // S0
  //----
  digitalWrite(output_select, HIGH);
  delayMicroseconds(STATE_CHANGE_TIME);

  //----
  // S1
  //----
  digitalWrite(output_select, LOW);
  delayMicroseconds(STATE_CHANGE_TIME);

  //----
  // S2
  //----
  if(!controller_connected()){
    return;
  }
  process_s2();
  digitalWrite(output_select, HIGH);
  delayMicroseconds(STATE_CHANGE_TIME);

  //----
  // S3
  //----
  process_s3();
  digitalWrite(output_select, LOW);
  delayMicroseconds(STATE_CHANGE_TIME);

  //----
  // S4
  //----
  if(!six_buttons_connected()){
    return;
  }
  digitalWrite(output_select, HIGH);
  delayMicroseconds(STATE_CHANGE_TIME);

  //----
  // S5
  //----
  process_s5();
  digitalWrite(output_select, LOW);
  delayMicroseconds(STATE_CHANGE_TIME);

  //----
  // S6
  //----
  digitalWrite(output_select, HIGH);
  delayMicroseconds(STATE_CHANGE_TIME);

  //----
  // S7
  //----
  digitalWrite(output_select, LOW);
  delayMicroseconds(STATE_CHANGE_TIME);
}

boolean controller_connected(){
  boolean p3 = false;
  boolean p4 = false;
  
  if(!digitalRead(btn_left_x)){
    p3 = true;   
  }
  if(!digitalRead(btn_right_mode)){
    p4 = true;   
  }

  return (p3 & p4);
}

boolean six_buttons_connected(){
  boolean p1 = false;
  boolean p2 = false;
  
  if(!digitalRead(btn_up_z)){
    p1 = true;   
  }
  if(!digitalRead(btn_down_y)){
    p2 = true;   
  }

  return (p1 & p2);
}

void process_s2(){
  if(!digitalRead(btn_a_b)){
    btn_pressed = true;
    btn_details[INDEX_A] = 'A';
  }
  if(!digitalRead(btn_start_c)){
    btn_pressed = true;
    btn_details[INDEX_START] = 'S';
  }
}

void process_s3(){
  if(!digitalRead(btn_up_z)){
    btn_pressed = true;
    btn_details[INDEX_UP] = 'U';  
  }
  if(!digitalRead(btn_down_y)){
    btn_pressed = true;
    btn_details[INDEX_DOWN] = 'D';  
  }
  if(!digitalRead(btn_left_x)){
    btn_pressed = true;
    btn_details[INDEX_LEFT] = 'L';   
  }
  if(!digitalRead(btn_right_mode)){
    btn_pressed = true;
    btn_details[INDEX_RIGHT] = 'R';   
  }
  if(!digitalRead(btn_a_b)){
    btn_pressed = true;
    btn_details[INDEX_B] = 'B'; 
  }
  if(!digitalRead(btn_start_c)){
    btn_pressed = true;
    btn_details[INDEX_C] = 'C';   
  }
}

void process_s5(){
  if(!digitalRead(btn_up_z)){
    btn_pressed = true;
    btn_details[INDEX_Z] = 'Z';    
  }
  if(!digitalRead(btn_down_y)){
    btn_pressed = true;
    btn_details[INDEX_Y] = 'Y';  
  }
  if(!digitalRead(btn_left_x)){
    btn_pressed = true;
    btn_details[INDEX_X] = 'X';  
  }
  if(!digitalRead(btn_right_mode)){
    btn_pressed = true;
    btn_details[INDEX_MODE] = 'M';   
  }
}
