#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      36
#define NUMROWS 8
#define NUMCOLS 8

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

uint16_t grid[NUMROWS][NUMCOLS];
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

const int numChars = 600;
char receivedChars[numChars];
boolean newData = false;
int receivedCount= 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(240);
  matrix.setTextColor(colors[0]);


  
}
int i = 0;
void loop() {

  recvWithStartEndMarkers();
  showNewData();
}



void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static int ndx = 0;
    char startMarker = 'G';
    char endMarker = ';';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if ((rc >= '0' && rc <= '9') || (rc >= 'A' && rc <= 'F')) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else if (rc == endMarker) {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                receivedCount = ndx;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.print(receivedCount);
        Serial.print(": ");
        Serial.println(receivedChars);

        clearDisplay();
        
        newData = false;

        int idx = 0;
        for (int x=0;x<8 && idx < receivedCount;x++){
          for (int y=0;y<8 && idx < receivedCount;y++){
                        
            int red = hex_to_int(receivedChars[idx],receivedChars[idx+1]);
            int green = hex_to_int(receivedChars[idx+2],receivedChars[idx+3]);
            int blue = hex_to_int(receivedChars[idx+4],receivedChars[idx+5]);
                       
            Serial.print("Color (");
            Serial.print(x);
            Serial.print(",");
            Serial.print(y);
            Serial.print("): {");
            
            Serial.print(receivedChars[idx+0]);
            Serial.print(receivedChars[idx+1]);
            Serial.print(" : ");
            Serial.print(red);
            Serial.print(" : ");
            Serial.print(red, HEX);
            Serial.print("},  {");

            Serial.print(receivedChars[idx+2]);
            Serial.print(receivedChars[idx+3]);
            Serial.print(" : ");
            Serial.print(green);
            Serial.print(" : ");
            Serial.print(green, HEX);
            Serial.print("},  {");

            Serial.print(receivedChars[idx+4]);
            Serial.print(receivedChars[idx+5]);
            Serial.print(" : ");
            Serial.print(blue);
            Serial.print(" : ");
            Serial.print(blue, HEX);

            Serial.println("}");
         
            matrix.drawPixel(x, y, matrix.Color(red,green,blue));
            

            idx = idx+6;
          }
        }  
        
        matrix.show();
        
    }
}

int hex_to_int(char carac1, char carac2){

    int result;
    result = (carac1 <= '9' ? carac1 - '0' : toupper(carac1) - 'A' + 10) << 4;
    result |= carac2 <= '9' ? carac2 - '0' : toupper(carac2) - 'A' + 10;
    return result;
}


void clearDisplay(){

for (int x =0; x<NUMROWS; x++){

      for (int y = 0; y<NUMCOLS; y++){

         matrix.drawPixel(x, y, 0);  
          
      }
  }
    matrix.show();
  
}

