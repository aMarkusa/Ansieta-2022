// Anxiety prompt module.
// Asks the user if they are anxious and helps the user real-time.


void showAnxietyPrompt() {
  // Set up the anxiety prompt state for the device.
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 12);
  // Display static text
  display.println("Feeling anxious?");
  display.display(); 
  display.setTextSize(2);

  // Display interactions
  display.setCursor(10, 50);
  // Display static text
  display.println("Yes");
  display.display(); 

  display.setCursor(90, 50);
  // Display static text
  display.println("No");

  display.display(); 

  // Setup handlers
  button1OnPress = &anxietyPromptNegativeReaction;
  button2OnPress = &anxietyPromptPositiveReaction;

  anxietyDetected = true;
}

void removeAnxietyPrompt() {
  // Tear down the anxiety prompt state.
  
  display.clearDisplay();
  display.display();  
  anxietyDetected = false;
}


void anxietyPromptNegativeReaction() {
    display.clearDisplay();
    display.display();
    // Display static text
    if(random(1,100)>50) {
      showAnxietyFact();
    }
    else {
      showFunPhoto();
    }

    // Tear down handlers
    button1OnPress = &noop;
    button2OnPress = &noop;

    scheduledFunction = &removeAnxietyPrompt;
    scheduledFunctionTime = millis() + 4000;
}
void anxietyPromptPositiveReaction() {
    display.clearDisplay();
    display.display();
    // Show the message
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 12);
    // Display static text
    display.println("Great!");
    display.display(); 

    // Tear down handlers
    button1OnPress = &noop;
    button2OnPress = &noop;
  
    scheduledFunction = &removeAnxietyPrompt;
    scheduledFunctionTime = millis() + 4000;
}
