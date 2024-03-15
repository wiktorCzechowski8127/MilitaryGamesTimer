void bombInitializeProgressBarData(progressBarDataS* data, const gamemodeBombS* const gm){
  data->ammountOfStages = BAR_TYPE_TWO_TEAMS;
  
  //if (gm->takeOverTime != gm->fullTakeOverTime) {

    /*
    data->takeOverStageSquares = round(AMMOUNT_OF_SQUARES_PER_TEAM * ((float)gm->takeOverTime / gm->fullTakeOverTime));
    if (data->takeOverStageSquares == AMMOUNT_OF_SQUARES_PER_TEAM) {
      data->takeOverStageSquares--;
    } else if (data->takeOverStageSquares == 0) {
      data->takeOverStageSquares++;
    }
  } 
  else 
  {
    data->ammountOfStages = BAR_TYPE_TWO_TEAMS;
    data->takeOverStageSquares = AMMOUNT_OF_SQUARES_PER_TEAM;
  }

  data->filledSite = 1;
  data->timeOfOneTakeOverSquareFilled = (float)gm->takeOverTime / data->takeOverStageSquares;
  data->timeOfOneFullTakeOverSquareFilled = (float)(gm->fullTakeOverTime - gm->takeOverTime) / (8 - data->takeOverStageSquares);
  */
}

void processBomb(const gamemodeBombS* const gm) {
  // Initialization
  lcd.clear();

  bool isGameRunning = true;
  bool endButtonKeepPushed = false;
  bool exit = false;
  msTimeT pushingTime = 0;
  gamemodeTiming timing;
  initializeTiming(&timing, &gm->gameTime, &gm->alarmSpeaker);

  progressBarDataS progressBarData;
  bombInitializeProgressBarData(&progressBarData, gm);

  bombDataS data;
  memset(&data, 0, sizeof(data));
  /*
  //DEBUG
  printBombGamemodeSettingsOnSerial(gm);

  // 3. LCD prints
  printWinningPoints(&data.leftTeamWinningPoints, &data.rightTeamWinningPoints, false);
  calculateFilledSquares(gm, &progressBarData, &data.pointsInMs);
  //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
  printProgressBar(&progressBarData);


  while (isGameRunning) {
    // 1. Calculating time
    timing.currentTime = millis();
    //Serial.println("TIMEDIFF: " + (String)(timing.currentTime - timing.lastCurrentTime));  //DEBUG
    //Serial.println("CURRENT TIME:" + (String)timing.currentTime);
    timing.lastCurrentTime = timing.currentTime;  //DEBUG

    // 2. Reading game buttons
    //buttons is set as INPUT_PULLUP mode. If button is pushed digitalRead return 0.

    // 2.0 Checking switch status
    if (gm->enableSwitch) {
      data.swithStatus = digitalRead(SWITCH);
    }

    // 2.1 Right team
    if (!data.swithStatus) {
      if (!digitalRead(RIGHT_TEAM_BUTTON) && digitalRead(LEFT_TEAM_BUTTON)) {
        // Display button push character

        // continous or first button push
        if (data.isRightButtonPushed == true) {
          // Calculating Points
          data.pointsInMs += (timing.currentTime - data.lastPushedButtonTimeStamp);

          calculateFilledSquares(gm, &progressBarData, &data.pointsInMs);
          //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
          printProgressBar(&progressBarData);
        } 
        else  //first button push
        {
          data.isRightButtonPushed = true;
          data.isLeftButtonPushed = false;
        }

        data.lastPushedButtonTimeStamp = timing.currentTime;
      }
      // 2.2 Left Team
      else if (!digitalRead(LEFT_TEAM_BUTTON) && digitalRead(RIGHT_TEAM_BUTTON)) {
        // Display button push character

        // continous or first button push
        if (data.isLeftButtonPushed == true) {
          // Calculating Points
          data.pointsInMs -= (timing.currentTime - data.lastPushedButtonTimeStamp);

          // 3. LCD prints
          //printWinningPoints(&data.leftTeamWinningPoints, &data.rightTeamWinningPoints, false);
          calculateFilledSquares(gm, &progressBarData, &data.pointsInMs);
          //Serial.println("WINNING SITE:" + (String)progressBarData.filledSite); // DEBUG
          printProgressBar(&progressBarData);
        } else  //first button push
        {
          data.isRightButtonPushed = false;
          data.isLeftButtonPushed = true;
        }

        data.lastPushedButtonTimeStamp = timing.currentTime;
      } 
      else  // No button pushed
      {
        //clearButtonsStatus(&data);
      }
    } 
    else  // Blocked by switch
    {
      //clearButtonsStatus(&data);
    }


    // 4. Endgame weryfication
    if (isGameRunning == true) {
      isGameRunning = valideateEndGameOrPrintTimeLeft(&timing);

      // Turning game off using buttons.
      endButtonKeepPushed = false;
      if (!digitalRead(LEFT_BUTTON) && !digitalRead(RIGHT_BUTTON)) {
        endButtonKeepPushed = true;
        if (pushingTime == 0) {
          pushingTime = timing.currentTime;
        }
      }
      if (endButtonKeepPushed == true) {
        if (timing.currentTime > (pushingTime + 2000)) {
          isGameRunning = false;
          exit = true;
        }
      } else {
        pushingTime = 0;
      }
    }
  }  // End of main loop

  // 5. Printing summary
  printSummary(&gm->gameTime, &timing.timeLeft, &data.leftTeamWinningPoints, &data.rightTeamWinningPoints);

  //TODO MOVE TO COMMON FUNCTION
  delay(FREEZE_TIME);
  endButtonKeepPushed = false;
  pushingTime = 0;
  while (true) {
    timing.currentTime = millis();
    endButtonKeepPushed = false;
    if (!digitalRead(LEFT_BUTTON) && !digitalRead(RIGHT_BUTTON)) {
      endButtonKeepPushed = true;
      if (pushingTime == 0) {
        pushingTime = timing.currentTime;
      }
    }
    if (endButtonKeepPushed == true) {
      if (timing.currentTime > (pushingTime + 2000)) {
        if (exit == false) {
          digitalWrite(RELAY, false);
          pushingTime = 0;
          exit = true;
          Serial.println("EXIT");
        } else {
          break;
        }
      }
    } else {
      pushingTime = 0;
    }
    if ((timing.currentTime > timing.alarmSpeakerEnd) && (timing.alarmSpeakerEnd != 0)) {
      digitalWrite(RELAY, false);
      timing.alarmSpeakerEnd = 0;
      exit = true;
    }
    //Serial.println("MILLIS: " + String(millis()) + " ALARM SPEAKER END " + String(timing.alarmSpeakerEnd));
  }
  */
}

void printBombGamemodeSettingsOnSerial(const gamemodeBombS* const gm)
{
  Serial.println(" ");
  Serial.println("Bomba ustawienia:");
  Serial.println("gameTime: " + (String)gm->gameTime);
  Serial.println("armingTime: " + (String)gm->armingTime);
  Serial.println("defusingTime: " + (String)gm->defusingTime);
  Serial.println("enableSwitch: " + (String)gm->enableSwitch);
  Serial.println("slowReversing: " + (String)gm->slowReversing);
}