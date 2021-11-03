# slackbuzzer
will buzz you when you get a slack message!

This app was created for the LILYGO TTGO T-Display ESP32 Wi-Fi BLE Module for Arduino Development Board 1.14 Inch LCD CH9102F Chip

It is a modified version of https://github.com/msurguy/esp8266-slack-pager

I have updated the code to work with newer dependencies and libraries. This is also updated to work with the latest slack api.

This app received messages that are posted in a channel where a bot is installed.
It does this via websockets using an authorization token from the bot.
It also can send an ACK message by pushing a button.

This code is for a very specific use in my case but some of the code may help you in other projects. Enjoy.
