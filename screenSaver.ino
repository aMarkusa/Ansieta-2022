
byte runCount = 0;

void showScreenSaver() {
  // show a screen saver.
    // Clear the buffer
  display.clearDisplay();
  display.display();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(map(runCount,0,255,0,127), 12);
    display.println(".");
    display.display(); 
  
  runCount = runCount+6;

}