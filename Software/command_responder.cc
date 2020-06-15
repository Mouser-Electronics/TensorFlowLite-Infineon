/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include "tensorflow/lite/micro/examples/micro_speech/command_responder.h"

#include "Arduino.h"


constexpr int yes_led = 24;
constexpr int no_led  = 25;

void RespondToCommand(tflite::ErrorReporter* error_reporter,
                      int32_t current_time, const char* found_command,
                      uint8_t score, bool is_new_command) {
  static bool is_initialized = false;
  if (!is_initialized) {
    pinMode(yes_led, OUTPUT);
    pinMode(no_led, OUTPUT);
    digitalWrite(yes_led, LOW;
    digitalWrite(no_led, LOW);
    is_initialized = true;
  }
  static int32_t last_command_time = 0;
  static int certainty = 220;

  if (is_new_command) {
    TF_LITE_REPORT_ERROR(error_reporter, "Heard %s (%d) @%dms", found_command,
                         score, current_time);
    // If we hear a command, light up the appropriate LED
    // Heard Yes
    if (found_command[0] == 'y') {
      last_command_time = current_time;
      digitalWrite(yes_led, HIGH);  // LED1 on for yes
      digitalWrite(no_led, LOW);  // LED2 off for no
    }

    // Heard No
    if (found_command[0] == 'n') {
      last_command_time = current_time;
      digitalWrite(yes_led, LOW);  // LED1 off for yes
      digitalWrite(no_led, HIGH);  // LED2 on for no
    }
    // Unknown what was heard
    if (found_command[0] == 'u') {
      last_command_time = current_time;
      digitalWrite(yes_led, LOW);  // LED1 on for yes
      digitalWrite(no_led, LOW);  // LED2 off for no
    }
  }

  // If last_command_time is non-zero but was >3 seconds ago, zero it
  // and switch off the LED.
  if (last_command_time != 0) {
    if (last_command_time < (current_time - 3000)) {
      last_command_time = 0;
     digitalWrite(yes_led, LOW);  // LED1 on for yes
      digitalWrite(no_led, LOW);  // LED2 off for no
    }
    // If it is non-zero but <3 seconds ago, do nothing.
    return;
  }
}

#endif  // ARDUINO_EXCLUDE_CODE
