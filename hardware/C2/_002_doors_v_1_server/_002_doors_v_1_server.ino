/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for Getting Started with nRF24L01+ radios. 
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two 
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting 
 * with the serial monitor and sending a 'T'.  The ping node sends the current 
 * time to the pong node, which responds by sending the value back.  The ping 
 * node can then see how long the whole cycle took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

int rele_open  = 3;
int rele_close = 4;

int current_door_state = 0;
int last_door_state = 0;
int commanded_door_state = 0;
unsigned long state_change_timeout = 0;
unsigned long state_change_start = 0;

unsigned long last_radio_recieve_millis = 0;
unsigned long radio_wd_time = 2000;


int led = 6;
boolean led_state = 1;
unsigned long led_timer = 300;
unsigned long led_time = 10000;

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 

RF24 radio(9,10);

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.  
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;


void setup(void)
{
  
  digitalWrite(7, HIGH); //We need to set it HIGH immediately on boot
  pinMode(7,OUTPUT);     //We can declare it an output ONLY AFTER it's HIGH
  // (( thatsaHACKHACKHACKHACK ))
                         
                         
  pinMode(rele_open, OUTPUT);     
  pinMode(rele_close, OUTPUT);     
  digitalWrite(rele_open, LOW);
  digitalWrite(rele_close, LOW);

  pinMode(led, OUTPUT);     
  digitalWrite(led, led_state);

  //
  // Print preamble
  //

  Serial.begin(9600);
  Serial.println("BEGIN");
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
   radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
  
}

void door_loop() {
  
  // nothing can happen while waiting for minimum motor time
  //  - except changing the commanded_door_state, but that happend in loop(); 
  if (state_change_timeout > 0 && millis() - state_change_start < state_change_timeout) {
      // timeout still not expired
      return;
  }
  else { // available
  
      // timeout reached, reset
      state_change_timeout = 0;

      // for care-free if-logic, let's use an intermediate variable
      int next_door_state = current_door_state;
      
      // the logic if stopped
      if (current_door_state == 0) {                      // currently STOPPED
        if (commanded_door_state != 0) {                      // and commanded to MOVE
          next_door_state = commanded_door_state;
          state_change_timeout = 800;
        }
        else {                                                // and commanded to STOP
          ; // noop
        }
      }
          
      // the logic if moving
      if (current_door_state != 0){                       // currently MOVING
        if (commanded_door_state == 0) {                      // and commanded to STOP
          next_door_state = 0;
          state_change_timeout = 800;
        }
        if (commanded_door_state != current_door_state) {    // and commanded to CHANGE DIRECTION
          next_door_state = 0;
          state_change_timeout = 800;
        }
        if (commanded_door_state == current_door_state) {    // and already MOVING there
          ; // noop
        }
      }
      
      // run timeout if needed 
      if (state_change_timeout > 0) {
        state_change_start = millis();
      }
      current_door_state = next_door_state;
  } // available
    
  
  // make sure corrnet_door_state reflects the relay state
  
  if (current_door_state == 0) {
    digitalWrite(rele_open,  LOW);
    digitalWrite(rele_close, LOW);    
  }
  if (current_door_state == 1) {
    digitalWrite(rele_open,  HIGH);
    digitalWrite(rele_close, LOW);
  }
  if (current_door_state == 2) {
    digitalWrite(rele_open,  LOW);
    digitalWrite(rele_close, HIGH);
  }

}


void loop(void)
{
  //
  // Ping out role.  Repeatedly send the current time
  //
  
  led_timer--;
  if (led_timer == 0) {
    led_state = ! led_state;
    led_timer = led_time;  
    digitalWrite(led, led_state);
  }


  if (role == role_ping_out)
  {
    ; // noup!
  }

  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

  if ( role == role_pong_back )
  {
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      unsigned long door_action;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &door_action, sizeof(unsigned long) );

        // Spew it
        printf("Got payload %lu...",door_action);

	// Delay just a little bit to let the other unit
	// make the transition to receiver
	delay(20);
      }

      last_radio_recieve_millis = millis();
      
      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      radio.write( &door_action, sizeof(unsigned long) );
      printf("Sent response.\n\r");

      commanded_door_state = door_action;

      // Now, resume listening so we catch the next packets.
      radio.startListening();
      
    }
    
    if (millis() - last_radio_recieve_millis > radio_wd_time) {
      // reset!
      Serial.println("RESET!");
      digitalWrite(7, LOW); //Pulling the RESET pin LOW triggers the reset. 
    }
    
    door_loop();
  }

  //
  // Change roles
  //

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      // Become the primary transmitter (ping out)
      role = role_ping_out;
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]);
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");
      
      // Become the primary receiver (pong back)
      role = role_pong_back;
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
    }
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
