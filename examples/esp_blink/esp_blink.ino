#include <Automaton.h>
#include <Atm_esp8266.h>

Atm_esp8266_wifi wifi;
Atm_esp8266_httpd_simple server( 80 );
Atm_led led;

void setup() {
  Serial.begin( 9600 );

  led.begin( D5 );
  Serial.println( "Connecting to WIFI" );
  
  wifi.begin( "MySSID", "MyPASSWORD" )
    .onChange( true, [] ( int idx, int v, int up  ) {
      Serial.print( "Connected to Wifi, browse to: http://");
      Serial.println( wifi.ip() );
      server.start();
    })
    .onChange( false, [] ( int idx, int v, int up  ) {
      Serial.println( "Lost Wifi connection");      
    })
    .led( LED_BUILTIN, true ) // Built in led is active low!
    .start();

  server.begin()
    .onRequest( "/on", led, led.EVT_ON )
    .onRequest( "/off", led, led.EVT_OFF )
    .onRequest( "/blink", led, led.EVT_START )
    .onRequest( "/", [] ( int idx, int v, int up ) {
      server.send( 
        "<!DOCTYPE html><html><body>"
        "<a href='on'>On</a><br>" 
        "<a href='off'>Off</a><br>" 
        "<a href='blink'>Blink</a><br>" 
        "</body></html>"
      );
    });
}

void loop() {
  automaton.run();
}
