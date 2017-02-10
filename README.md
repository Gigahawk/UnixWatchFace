# UnixWatchFace

Pebble watchface based on a Unix style terminal.

Features: 
 - Time (Duh)
  - Use 24h time to have the time update every minute, 12 to update every second
 - Battery indicator
  - Bat0: Pebble battery
  - Bat1: Phone battery (Uses the [Battery Status API](https://developer.mozilla.org/en/docs/Web/API/Battery_Status_API))
 - Weather
  - Uses Yahoo's weather API
 - Bluetooth connection indicator
  - Watch will vibrate if phone disconnects
  
TODO:
  - Add configuration
    - Option to invert screen colors
    - Option to revert to old battery indicator (only Pebble battery, may reduce battery drain)
    - Option to pick weather API
  - Minor refactoring

Credit:
 - [TrekVolle](https://github.com/aHcVolle/TrekVolle) was the inspiration for the recent update's organization style as well as the Battery Status API implementation.
