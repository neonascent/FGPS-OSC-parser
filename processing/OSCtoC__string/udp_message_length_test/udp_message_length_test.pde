/**
 * (./) udp.pde - how to use UDP library as unicast connection
 * (cc) 2006, Cousot stephane for The Atelier Hypermedia
 * (->) http://hypermedia.loeil.org/processing/
 *
 * Create a communication between Processing<->Pure Data @ http://puredata.info/
 * This program also requires to run a small program on Pd to exchange data  
 * (hum!!! for a complete experimentation), you can find the related Pd patch
 * at http://hypermedia.loeil.org/processing/udp.pd
 * 
 * -- note that all Pd input/output messages are completed with the characters 
 * ";\n". Don't refer to this notation for a normal use. --
 */

// import UDP library
import hypermedia.net.*;


UDP udp;  // define the UDP object

/**
 * init
 */
void setup() {
  frameRate(10);
  // create a new datagram connection on port 6000
  // and wait for incomming message
  udp = new UDP( this, 6000 );
  //udp.log( true ); 		// <-- printout the connection activity
  udp.listen( true );
}

//process events
void draw() {

    String message  = "a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message: a big long test message:";	// the message to send
    String message2 = "and a short one and a short one and a short one and a short one and a short one and a short oneand a short one and a short one and a short one and a short one and a short one and a short one and a short one and a short one and a short one and a short one and a short one and a short oneand a short one and a short one and a short one and a short one and a short one and a short one";
    String ip       = "localhost";	// the remote IP address
    int port        = 7110;		// the destination port
    
    // formats the message for Pd
    message = message+";\n";
    
    message2 = message2.length() + message2;
    // send the message
    message3 = message.length() + " ";
    udp.send( message, ip, port );
    udp.send( message3, ip, port );

;}

